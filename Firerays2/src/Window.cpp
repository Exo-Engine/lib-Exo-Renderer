/*
 *	MIT License
 *
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include <SDL2/SDL_image.h>
#include <iostream>
#include <thread>

#include "SDLException.h"
#include "Window.h"
#include "OGLCall.h"

#if defined (__APPLE__)
# include <OpenGL/CGLCurrent.h>
# include <OpenCL/opencl.h>
#elif defined (__linux__)
# include <GL/glx.h>
#elif (defined _WIN32 || defined _WIN64)
# include <windows.h>
#endif

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

ExoRendererFirerays2::Window::Window(const std::string &title, uint32_t width, uint32_t height, const WindowMode &mode, bool resizable, GamepadManager& gamepad)
: IWindow(), _window(nullptr), _pFrameBuffer(nullptr), _frameTexture(nullptr), _depthTexture(nullptr)
{
	initialize(title, width, height, mode, resizable, gamepad);
}

ExoRendererFirerays2::Window::~Window(void)
{
	if (_pFrameBuffer)
		delete _pFrameBuffer;

	if (_frameTexture)
		delete _frameTexture;

	if (_depthTexture)
		delete _depthTexture;

	SDL_GL_DeleteContext(_glContext);
	SDL_GL_DeleteContext(_threadContext);
	SDL_DestroyWindow(_window);
	IMG_Quit();
	SDL_Quit();
}

#ifndef USE_TEST_SHADERS

static const std::vector<std::string>	g_defaultShader = {
	"#version 330 core",
	"layout(location = 0) in vec3 position;",
	"layout(location = 1) in vec2 texCoord;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main(void) ",
	"{",
	"	gl_Position = vec4(position.x, position.y, 0.0, 1.0);",
	"	TexCoords = texCoord;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"uniform sampler2D screenTexture;",
	"",
	"in vec2 TexCoords;",
	"out vec4 color;",
	"",
	"void main(void) ",
	"{",
	"	color = texture(screenTexture, TexCoords);",
	"}"
};

#endif

void	ExoRendererFirerays2::Window::initialize(const std::string& title, uint32_t width, uint32_t height, const WindowMode &mode, bool resizable, GamepadManager &gamepad)
{

	_width = width;
	_height = height;

	_contextWidth = width;
	_contextHeight = height;

#ifndef __APPLE__
	static bool	glew_init = false;
	GLenum		error;
#endif

	if (!(SDL_WasInit(SDL_INIT_EVERYTHING) & SDL_INIT_VIDEO))
		if (SDL_Init(SDL_INIT_EVERYTHING))
			throw (SDLException());

	IMG_Init(IMG_INIT_PNG);
	IMG_Init(IMG_INIT_JPG);

	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1))
	{
		SDL_Quit();
		throw (SDLException());
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
	{
		SDL_Quit();
		throw (SDLException());
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
	{
		SDL_Quit();
		throw (SDLException());
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2))
	{
		SDL_Quit();
		throw (SDLException());
	}
	if (SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1))
	{
		SDL_Quit();
		throw (SDLException());
	}
#ifdef __APPLE__
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG))
	{
		SDL_Quit();
		throw (SDLException());
	}
#endif

	_windowMode = mode;
	auto windowModeFlag = 0;
	if (_windowMode == WindowMode::FULLSCREEN)
		windowModeFlag = SDL_WINDOW_FULLSCREEN;
	else if (_windowMode == WindowMode::BORDERLESS)
		windowModeFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;

	if (resizable)
		windowModeFlag += SDL_WINDOW_RESIZABLE;

	if (!(_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | windowModeFlag)))
	{
		SDL_Quit();
		throw (SDLException());
	}
	if (!(_threadContext = SDL_GL_CreateContext(_window)))
	{
		SDL_GL_DeleteContext(_threadContext);
		SDL_DestroyWindow(_window);
		SDL_Quit();
		throw (SDLException());
	}
	if (!(_glContext = SDL_GL_CreateContext(_window)))
	{
		SDL_DestroyWindow(_window);
		SDL_Quit();
		throw (SDLException());
	}
	SDL_GL_MakeCurrent(_window, _glContext);
	SDL_ShowCursor(SDL_DISABLE); // Disable cursor

#ifndef __APPLE__
	if (glew_init == false)
		if ((error = glewInit()) != GLEW_OK)
			throw (error);
#endif

	if (SDL_GameControllerAddMappingsFromFile("resources/SDL2/gamecontrollerdb.txt") == -1)
		;	//	silent

	// OpenGL setup
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glCullFace(GL_BACK));

	// Get Context size (can be different if HIGHDPI / Retina)
	GLint dims[4] = {0};
	glGetIntegerv(GL_VIEWPORT, dims);
	_contextWidth = dims[2] / 2;
	_contextHeight = dims[3] / 2;

	_highDPIFactor = _contextWidth / _width;

	// Post Processing
	const static float		tmp1[] = {
		-1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		-1.0f,	1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,	1.0f, 0.0f
	};
	const static float		tmp2[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

#ifdef USE_TEST_SHADERS
	_postProcessing.initialize("resources/shaders/OpenGL3/post-processing/default.glsl");
#else
	_postProcessing.initialize(g_defaultShader);
#endif
	_postProcessing.bind();

	_postVertexArrayObject.initialize(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	_postArrayBuffer.initialize(18, 3, &tmp1, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);
	_postUVMappingBuffer.initialize(12, 2, &tmp2, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 1, true);

	initPostProcessing();
	initOpenCL();
}

void	ExoRendererFirerays2::Window::initOpenCL(void)
{
	std::vector<CLWPlatform>	platforms;
	bool						deviceFound = false;
	cl_platform_id				platformId;
	CLWPlatform::CreateAllPlatforms(platforms);

	std::cout << "listing available platforms:" << std::endl;
	for (CLWPlatform& platform : platforms)
	{
		std::cout << " - " << platform.GetName() << std::endl;
		for (size_t deviceIndex = 0; deviceIndex < platform.GetDeviceCount(); deviceIndex++)
		{
			CLWDevice device = platform.GetDevice(deviceIndex);
			std::cout << "   * " << device.GetName() << std::endl;
			if (device.GetType() == CL_DEVICE_TYPE_GPU)
			{
				platformId = platform;
				std::cout << "platform: " << platform.GetName() << " " << platform.GetVersion() << std::endl;
				_device = device;
				deviceFound = true;
				break ;
			}
		}
	}
	if (!deviceFound)
	{
		// throw (StrException("no suitable device found"));
	}
	std::cout << "device " << _device.GetName() << " chosen" << std::endl;

#if defined __APPLE__

	static cl_context_properties		properties[] = {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		0,
		0
	};
	static cl_queue_properties_APPLE	queue_properties = 0;

	properties[1] = (cl_context_properties)CGLGetShareGroup(
		CGLGetCurrentContext());

	_context = CLWContext::Create(_device, properties);
	_commandQueue = CLWCommandQueue::Create(_device, _context);
	_commandQueue = CLWCommandQueue::Create(clCreateCommandQueueWithPropertiesAPPLE(_context, _device, &queue_properties, nullptr));

#elif defined __linux__

	static cl_context_properties	properties[] = {
		CL_CONTEXT_PLATFORM, 0,
		CL_GL_CONTEXT_KHR, 0,
		CL_GLX_DISPLAY_KHR, 0,
		0
	};

	properties[1] = (cl_context_properties)platformId;
	properties[3] = (cl_context_properties)glXGetCurrentContext();
	properties[5] = (cl_context_properties)glXGetCurrentDisplay();

	_context = CLWContext::Create(_device, properties);
	_commandQueue = CLWCommandQueue::Create(clCreateCommandQueueWithProperties(_context, _device, nullptr, nullptr));

#elif (defined _WIN32 || defined _WIN64)

	static cl_context_properties	properties[] = {
		CL_CONTEXT_PLATFORM, 0,
		CL_GL_CONTEXT_KHR, 0,
		CL_WGL_HDC_KHR, 0,
		0
	};

	properties[1] = (cl_context_properties)_clPlatform;
	properties[3] = (cl_context_properties)wglGetCurrentContext();
	properties[5] = (cl_context_properties)wglGetCurrentDC();

	_context = CLWContext::Create(_device, properties);
	_commandQueue = CLWCommandQueue::Create(clCreateCommandQueue(_context, _device, nullptr, nullptr));

#endif

	// _renderingEngine = new FireraysEngine(_context, _commandQueue);
	_renderCLTexture = _context.CreateImage2DFromGLTexture(_frameTexture->getBuffer());
}

void	ExoRendererFirerays2::Window::handleEvents(Keyboard& keyboard, Mouse& mouse, GamepadManager& gamepad)
{
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
			case SDL_QUIT:
				_close = true;
				break;
			case SDL_KEYDOWN:
				keyboard.keyDown(keyboard.getKeyboardInput(_event.key.keysym.sym));
				break;
			case SDL_KEYUP:
				keyboard.keyUp(keyboard.getKeyboardInput(_event.key.keysym.sym));
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse.keyDown(mouse.getMouseInput(_event.button.button));
				break;
			case SDL_MOUSEBUTTONUP:
				mouse.keyUp(mouse.getMouseInput(_event.button.button));
				break;
			case SDL_MOUSEWHEEL:
				mouse.wheelX = _event.wheel.x;
				mouse.wheelY = _event.wheel.y;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = _event.motion.x;
				mouse.y = _event.motion.y;
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				gamepad.remove(_event.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				gamepad.add(_event.cdevice.which);
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				gamepad.keyDown(_event.cdevice.which, Gamepad::getGamepadInput(_event.cbutton.button));
				break;
			case SDL_CONTROLLERBUTTONUP:
				gamepad.keyUp(_event.cdevice.which, Gamepad::getGamepadInput(_event.cbutton.button));
				break;
			case SDL_CONTROLLERAXISMOTION:
				switch (_event.caxis.axis) {
					case SDL_CONTROLLER_AXIS_LEFTX:
						gamepad.leftStickX(_event.cdevice.which, _event.caxis.value);
						break;
					case SDL_CONTROLLER_AXIS_LEFTY:
						gamepad.leftStickY(_event.cdevice.which, _event.caxis.value);
						break;
					case SDL_CONTROLLER_AXIS_RIGHTX:
						gamepad.rightStickX(_event.cdevice.which, _event.caxis.value);
						break;
					case SDL_CONTROLLER_AXIS_RIGHTY:
						gamepad.rightStickY(_event.cdevice.which, _event.caxis.value);
						break;
					case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
						gamepad.triggerLeft(_event.cdevice.which, _event.caxis.value);
						break;
					case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
						gamepad.triggerRight(_event.cdevice.which, _event.caxis.value);
						break;
				}
				break;
		}
	}
}

void	ExoRendererFirerays2::Window::clearScreen(void)
{
	_last = _now;
	_now = SDL_GetPerformanceCounter();

	_pFrameBuffer->clear();
}

void	ExoRendererFirerays2::Window::swap(void)
{
	_pFrameBuffer->unbind(); // back to default
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_postProcessing.bind();
	GL_CALL(glBindVertexArray(_postVertexArrayObject.getBuffer()));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _postArrayBuffer.getBuffer()));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _postUVMappingBuffer.getBuffer()));

	// Bind texture
	GL_CALL(glActiveTexture(GL_TEXTURE0));
	_frameTexture->bind();

	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	SDL_GL_SwapWindow(_window);
	float	pixel[4] = {0, 0, 0, 0};

	GL_CALL(glClearTexImage(_frameTexture->getBuffer(), 0, GL_RGBA, GL_FLOAT, &pixel));
}

void	ExoRendererFirerays2::Window::handleThread(void)
{
	static std::thread::id	prev = std::thread::id();
	int					 error;

	if (std::this_thread::get_id() != prev)
		if ((error = SDL_GL_MakeCurrent(_window, _threadContext)))
			prev = std::this_thread::get_id();
}

IFrameBuffer*	ExoRendererFirerays2::Window::getFrameBuffer(void) const
{
	return (_pFrameBuffer);
}

void	ExoRendererFirerays2::Window::initPostProcessing(void)
{
	// If already exist
	if (_pFrameBuffer)
		delete _pFrameBuffer;

	if (_frameTexture)
		delete _frameTexture;

	if (_depthTexture)
		delete _depthTexture;

	// Color Texture
	_frameTexture = new Texture(_contextWidth, _contextHeight, RGBA, NEAREST);
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	_depthTexture = new Texture(_contextWidth, _contextHeight, DEPTH, NEAREST);

	// Framebuffer
	_pFrameBuffer = new FrameBuffer();
	_pFrameBuffer->attach(_frameTexture);
	_pFrameBuffer->attach(_depthTexture);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw ("Error::FRAMEBUFFER:: Framebuffer is not complete!");

	_pFrameBuffer->unbind();
}

// Setters
void	ExoRendererFirerays2::Window::setWindowSize(int		w, int		h)
{
	SDL_SetWindowSize(_window, w, h);
	_width = w;
	_height = h;
	glViewport(0, 0, w * _highDPIFactor, h * _highDPIFactor);

	// Update Post Processing Buffer
	//> Get Context size (can be different if HIGHDPI / Retina)
	GLint dims[4] = {0};
	glGetIntegerv(GL_VIEWPORT, dims);
	_contextWidth = dims[2];
	_contextHeight = dims[3];

	//> Post Processing
	initPostProcessing();
}

void	ExoRendererFirerays2::Window::setWindowMode(const WindowMode &mode)
{
	int		error = 0;
	switch (mode)
	{
		case FULLSCREEN:
			error = SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
			break;
		case BORDERLESS:
			error = SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;
		default: // Windowed
			error = SDL_SetWindowFullscreen(_window, 0);
			break;
	}
	_windowMode = mode;

	// Check error
	if (error == -1)
		throw (SDLException());

	// Update Post Processing Buffer
	//> Get Context size (can be different if HIGHDPI / Retina)
	if (_windowMode == WindowMode::FULLSCREEN)
	{
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);

		_contextWidth = displayMode.w;
		_contextHeight = displayMode.h;
	}
	else
	{
		SDL_GetWindowSize(_window, &_width, &_height);
		glViewport(0, 0, _width * _highDPIFactor, _height * _highDPIFactor);

		GLint dims[4] = {0};
		glGetIntegerv(GL_VIEWPORT, dims);
		_contextWidth = dims[2];
		_contextHeight = dims[3];
	}

	//> Post Processing
	initPostProcessing();
}

void	ExoRendererFirerays2::Window::setVsync(bool vsync)
{
	if (SDL_GL_SetSwapInterval(vsync == true ? 1 : 0) == -1)
		throw (SDLException());
}

// Getters
double	ExoRendererFirerays2::Window::getDelta(void) const
{
	return (double)((_now - _last) * 1000 / (double)SDL_GetPerformanceFrequency());
}

float	ExoRendererFirerays2::Window::getWidth(void) const
{
	return _width;
}

float	ExoRendererFirerays2::Window::getHeight(void) const
{
	return _height;
}

int		ExoRendererFirerays2::Window::getContextWidth(void) const
{
	return _contextWidth;
}

int		ExoRendererFirerays2::Window::getContextHeight(void) const
{
	return _contextHeight;
}

int		ExoRendererFirerays2::Window::getHighDPIFactor(void) const
{
	return _highDPIFactor;
}

bool	ExoRendererFirerays2::Window::isFullscreen(void) const
{
	return _windowMode == WindowMode::FULLSCREEN;
}

bool	ExoRendererFirerays2::Window::getIsClosing(void) const
{
	return _close;
}

CLWContext&			ExoRendererFirerays2::Window::getCLContext(void)
{
	return (_context);
}

CLWCommandQueue&	ExoRendererFirerays2::Window::getCLCommandQueue(void)
{
	return (_commandQueue);
}

CLWImage2D&			ExoRendererFirerays2::Window::getCLTexture(void)
{
	return (_renderCLTexture);
}
