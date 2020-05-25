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

#include "RendererFirerays.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Button.h"
#include "Input.h"
#include "Checkbox.h"
#include "Select.h"
#include "Spinner.h"
#include "View.h"
#include "OrthogonalLight.h"
#include "PerspectiveLight.h"
#include "PointLight.h"
#include "Model.h"
#include "ModelInstance.h"
#include "BodyPartInstance.h"

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

static RendererFirerays	*g_instance = nullptr;

RendererFirerays&	RendererFirerays::Get(void)
{
	if (!g_instance)
		g_instance = new RendererFirerays();
	return *g_instance;
}

void				 RendererFirerays::Destroy(void)
{
	if (g_instance)
	{
		delete g_instance;
		g_instance = nullptr;
	}
}

void RendererFirerays::initialize(const std::string& title, const int width, const int height, const WindowMode &mode, bool resizable)
{
	_scissorBit[0] = 0; _scissorBit[1] = 0; _scissorBit[2] = 0; _scissorBit[3] = 0;

	// Destroy if window already exist
	if (_pWindow)
		delete _pWindow;

	_pWindow = new Window(title, width, height, mode, resizable, _gamepad);
	resize();

	// Shaders
	loadShaders();

	// Buffers
	createBuffers();

	// Renderers
	_pSpriteRenderer = new SpriteRenderer();
	_pGUIRenderer = new GUIRenderer();
	_pTextRenderer = new TextRenderer();
	_pObjectRenderer = new ObjectRenderer(_pWindow->getCLContext(), _pWindow->getCLCommandQueue());
	_pObjectRenderer->setTarget(_pWindow->getCLTexture(), _pWindow->getContextWidth(), _pWindow->getContextHeight());
	_changes = true;
}

void RendererFirerays::resize()
{
	_UIScaleFactor = _pWindow->getWidth() / REFRENCE_RESOLUTION_WIDTH;

	_perspective = glm::perspective(glm::radians(90.0f), (float)(_pWindow->getWidth() / _pWindow->getHeight()), 0.1f, 100.f);
	_orthographic = glm::ortho(0.0f, (float)_pWindow->getWidth(), (float)_pWindow->getHeight(), 0.0f, 0.0f, 1.0f);
}

// Create
ICamera* RendererFirerays::createCamera(void)
{
	return new Camera();
}

IAxis* RendererFirerays::createAxis(void)
{
	return new Axis();
}

ITexture* RendererFirerays::createTexture(const std::string& filePath, TextureFilter filter)
{
	ITexture	*texture;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		texture = new Texture(filePath, filter);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (texture);
	}
	else
		return (new Texture(filePath, filter));
}

ITexture* RendererFirerays::createTexture(unsigned int width, unsigned int height, TextureFormat format, TextureFilter filter)
{
	ITexture	*texture;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		texture = new Texture(width, height, format, filter);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (texture);
	}
	else
		return (new Texture(width, height, format, filter));
}

IArrayTexture* RendererFirerays::createArrayTexture(int width, int height, std::vector<std::string> &textures, TextureFilter filter)
{
	ArrayTexture	*texture;
	GLsync			fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		texture = new ArrayTexture(width, height, textures, filter);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (new ArrayTexture(width, height, textures, filter));
	}
	else
		return (new ArrayTexture(width, height, textures, filter));
}

ICursor* RendererFirerays::createCursor()
{
	return new Cursor();
}

ILabel* RendererFirerays::createLabel()
{
	return new Label();
}

IButton* RendererFirerays::createButton(const std::shared_ptr<ITexture> &texture, ButtonType buttonType, bool withLabel)
{
	return new Button(texture, buttonType, withLabel, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());

}

ICheckbox* RendererFirerays::createCheckbox(const std::shared_ptr<ITexture> &texture, bool checked)
{
	return new Checkbox(texture, checked, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

IInput* RendererFirerays::createInput(const std::shared_ptr<ITexture> &texture, const std::string &text, InputType type)
{
	return new Input(type, texture, text, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

IImage* RendererFirerays::createImage(const std::shared_ptr<ITexture> &texture)
{
	return new Image(texture, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

ISpinner* RendererFirerays::createSpinner(const std::shared_ptr<ITexture> &texture)
{
	return new Spinner(texture, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

ISlider* RendererFirerays::createSlider(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& barTexture)
{
	return new Slider(buttonTexture, barTexture, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

ISelect* RendererFirerays::createSelect(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, const std::shared_ptr<Font>& font)
{
	return new Select(buttonTexture, backgroundTexture, scrollTexture, font, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

IView* RendererFirerays::createView(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns)
{
	return new View(scrollTexture, numberOfRows, numberOfColumns, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

IView* RendererFirerays::createView(const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns)
{
	return new View(backgroundTexture, scrollTexture, numberOfRows, numberOfColumns, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
}

ILight	*RendererFirerays::createOrthogonalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z)
{
	ILight		*light;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		light = new OrthogonalLight(ambient, diffuse, pos, dir, up, x, y, z);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (light);
	}
	else
		return (new OrthogonalLight(ambient, diffuse, pos, dir, up, x, y, z));
}

ILight	*RendererFirerays::createPerspectivelLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far)
{
	ILight		*light;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		light = new PerspectiveLight(ambient, diffuse, pos, dir, up, fovy, aspect, near, far);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (light);
	}
	else
		return (new PerspectiveLight(ambient, diffuse, pos, dir, up, fovy, aspect, near, far));
}

ILight	*RendererFirerays::createPointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far)
{
	ILight		*light;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		light = new PointLight(ambient, diffuse, pos, dir, up, fovy, aspect, near, far);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (light);
	}
	else
		return (new PointLight(ambient, diffuse, pos, dir, up, fovy, aspect, near, far));
}

IFrameBuffer	*RendererFirerays::createFrameBuffer(void)
{
	IFrameBuffer	*frameBuffer;
	GLsync			fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		frameBuffer = new FrameBuffer();
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (frameBuffer);
	}
	else
		return (new FrameBuffer());
}

// Push
void RendererFirerays::add(sprite &s)
{
	_pSpriteRenderer->add(s);
}

void RendererFirerays::add(IWidget *widget)
{
	widget->update(getMouse(), getKeyboard(), getGamepadManager()->getGamepad(0), getNavigationType());

	// Update
	switch (widget->getType())
	{
		case IWidget::BUTTON: {
			auto button = (Button*)widget;
			add(button->getLabel());
			break;
		}
		case IWidget::SELECT: {
			auto select = (Select*)widget;
			add(select->getLabel());
			break;
		}
		case IWidget::INPUT: {
			auto input = (Input*)widget;
			add(input->getLabel());
			break;
		}
		case IWidget::SPINNER: {
			auto spinner = (Spinner*)widget;
			spinner->update(_pWindow->getDelta());
			break;
		}
		default: break;
	}

	// Push in renderer
	_pGUIRenderer->add(widget);
}

void RendererFirerays::add(ILabel *label)
{
	label->contextInfo(_UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
	_pTextRenderer->add((Label*)label);
}

void RendererFirerays::add(std::shared_ptr<ILight> &light)
{
}

void RendererFirerays::add(IModelInstance* object)
{
	_pObjectRenderer->add(static_cast<ModelInstance*>(object));
}

// Push
void RendererFirerays::remove(sprite &s)
{
	_pSpriteRenderer->remove(s);
}

void RendererFirerays::remove(IWidget *widget)
{
	// Update
	switch (widget->getType())
	{
		case IWidget::BUTTON: {
			auto button = (Button*)widget;
			remove(button->getLabel());
			break;
		}
		case IWidget::SELECT: {
			auto select = (Select*)widget;
			remove(select->getLabel());
			break;
		}
		case IWidget::INPUT: {
			auto input = (Input*)widget;
			remove(input->getLabel());
			break;
		}
		default: break;
	}
	// Push in renderer
	_pGUIRenderer->remove(widget);
}

void RendererFirerays::remove(ILabel *label)
{
	_pTextRenderer->remove((Label*)label);
}

void RendererFirerays::remove(std::shared_ptr<ILight> &light)
{
}

void RendererFirerays::remove(IModelInstance* object)
{
	_pObjectRenderer->remove(static_cast<ModelInstance*>(object));
}

void RendererFirerays::draw(void)
{
	// Renderers
	if (_pCurrentCamera)
	{
		if (_pMousePicker)
			_pMousePicker->update((IMouse*)&_mouse, _pWindow->getWidth(), _pWindow->getHeight(), ((Camera*)_pCurrentCamera)->getLookAt(), _perspective);

		_pSpriteRenderer->render((Camera*)_pCurrentCamera, _perspective);

		if (_changes)
		{
			_pObjectRenderer->commitChanges();
			_changes = false;
		}

		_pObjectRenderer->render((Camera*)_pCurrentCamera, _perspective);

		if (_pAxis)
			((Axis*)_pAxis)->render(((Camera*)_pCurrentCamera)->getLookAt(), _perspective);
	}

	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	_pGUIRenderer->render(_orthographic);
	_pTextRenderer->render(_orthographic);

	GL_CALL(glDisable(GL_BLEND));
}

void RendererFirerays::swap(void)
{
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	draw();
	GL_CALL(glDisable(GL_BLEND));

	_mouse.updateLastBuffer();
	_keyboard.updateLastBuffer();
	_gamepad.update();

	_pWindow->handleEvents(_keyboard, _mouse, _gamepad);

	_pWindow->swap();

	// _pWindow->clearScreen();
}

void RendererFirerays::beginScissor(glm::vec2 position, glm::vec2 size, glm::vec2 parentPosition, glm::vec2 parentSize)
{
	position		*= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);
	size			*= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);
	parentPosition	*= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);
	parentSize		*= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);

	if (parentPosition.x != 0 && parentPosition.y != 0 && parentSize.x != 0 && parentSize.y != 0)
	{
		glGetIntegerv(GL_SCISSOR_BOX, _scissorBit);
		// X
		if (position.x + size.x > parentPosition.x + parentSize.x) // Right
			size.x = (parentPosition.x + parentSize.x) - position.x;

		if (position.x < parentPosition.x) // Left
		{
			size.x = (position.x + size.x) - parentPosition.x;
			position.x = parentPosition.x;
		}

		// Y
		if (position.y + size.y > parentPosition.y + parentSize.y) // Top
			size.y = (parentPosition.y + parentSize.y) - position.y;

		if (position.y < parentPosition.y) // Bottom
		{
			size.y = (position.y + size.y) - parentPosition.y;
			position.y = parentPosition.y;
		}
	}

	glEnable(GL_SCISSOR_TEST);
	glScissor(position.x, position.y, size.x, size.y);
}

void RendererFirerays::endScissor(void)
{
	glScissor(_scissorBit[0], _scissorBit[1], _scissorBit[2], _scissorBit[3]);
	glDisable(GL_SCISSOR_TEST);

	// Reset
	_scissorBit[0] = 0; _scissorBit[1] = 0; _scissorBit[2] = 0; _scissorBit[3] = 0;
}

// Getters
IWindow* RendererFirerays::getWindow(void)
{
	return _pWindow;
}

IKeyboard* RendererFirerays::getKeyboard(void)
{
	return &_keyboard;
}

IMouse* RendererFirerays::getMouse(void)
{
	return &_mouse;
}

IGamepadManager* RendererFirerays::getGamepadManager(void)
{
	return &_gamepad;
}

unsigned int RendererFirerays::getTime(void) const
{
	return SDL_GetTicks();
}

// Setters
void RendererFirerays::setCursor(ICursor* cursor)
{
	if (_pCursor)
		remove(_pCursor->getImage());
	_pCursor = (Cursor*)cursor;
	if (_pCursor)
		add(_pCursor->getImage());
}

void RendererFirerays::setMousePicker(MousePicker* picker)
{
	_pMousePicker = picker;
}

void RendererFirerays::setAxis(IAxis* axis)
{
	_pAxis = axis;
}

void RendererFirerays::setGridEnable(bool val)
{
	if (_pSpriteRenderer)
		_pSpriteRenderer->setGrid(val);
}

IModelInstance*	RendererFirerays::instanciate(Model* model)
{
	// ModelInstance* instance = new ModelInstance();

	// instance->setBody(instanciate(model->getBody(), instance));
	// return (instance);
	IModelInstance* instance = _pObjectRenderer->newModelInstance(model);
	return (instance);
}

void	RendererFirerays::changesMade(void)
{
	_changes = true;
}

void	RendererFirerays::commitChanges(void)
{
	_pObjectRenderer->commitChanges();
}

// IBodyPartInstance*	RendererFirerays::instanciate(BodyPart* bodyPart, IModelInstance* model, IBodyPartInstance* parent)
// {
// 	BodyPartInstance*	instance = new BodyPartInstance(bodyPart, model, static_cast<BodyPartInstance*>(parent));

// 	for (BodyPart* child : bodyPart->getChilds())
// 		instance->addChild(instanciate(child, model, instance));
// 	return (instance);
// }

void	RendererFirerays::loadModel(Model* model)
{
	_pObjectRenderer->loadModel(model);
	// _pObjectRenderer->commitChanges();
	// loadBodyPart(model->getBody());

	_pWindow->clearScreen();
}

// Private
RendererFirerays::RendererFirerays(void)
: IRenderer(), _pWindow(nullptr), _pSpriteRenderer(nullptr), _pGUIRenderer(nullptr), _pTextRenderer(nullptr), _pObjectRenderer(nullptr), _pCursor(nullptr)
{
	_mainThread = std::this_thread::get_id();
}

RendererFirerays::~RendererFirerays(void)
{
	if (_pWindow)
		delete _pWindow;

	if (_pCursor)
		delete _pCursor;

	// Renderers
	if (_pSpriteRenderer)
		delete _pSpriteRenderer;

	if (_pGUIRenderer)
		delete _pGUIRenderer;

	if (_pTextRenderer)
		delete _pTextRenderer;

	if (_pObjectRenderer)
		delete _pObjectRenderer;

	// Buffers
	if (TextRenderer::vaoBuffer)
		delete TextRenderer::vaoBuffer;

	if (TextRenderer::vertexBuffer)
		delete TextRenderer::vertexBuffer;

	// Shaders
	if (SpriteRenderer::pShader)
		delete SpriteRenderer::pShader;

	if (GUIRenderer::pGuiShader)
		delete GUIRenderer::pGuiShader;

	if (TextRenderer::pTextShader)
		delete TextRenderer::pTextShader;
}

void RendererFirerays::createBuffers(void)
{
	// SpriteRenderer
	const float vertexBuffer[] = {
		-0.5f,	0.5f, 0.0f,	// top left
		-0.5f, -0.5f, 0.0f,	// bottom left
		 0.5f, -0.5f, 0.0f,	// bottom right
		 0.5f, 0.5f, 0.0f,	// top right
	};

	const unsigned int indexBuffer[] = {
		0, 1, 3,
		1, 2, 3
	};
	const float UVBuffer[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	SpriteRenderer::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	SpriteRenderer::vertexBuffer = new Buffer(12, 3, &vertexBuffer, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);
	SpriteRenderer::indexBuffer = new Buffer(6, 3, &indexBuffer, BufferType::INDEXBUFFER, BufferDraw::STATIC, 0, false);
	SpriteRenderer::uvBuffer = new Buffer(8, 2, &UVBuffer, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 1, true);

	// TextRenderer
	TextRenderer::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	TextRenderer::vertexBuffer = new Buffer(24, 4, NULL, BufferType::ARRAYBUFFER, BufferDraw::DYNAMIC, 0, false);

	// Grid
	const float line[] = {
		0.0,	0.0,
		1.0,	0.0,
	};

	Grid::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	Grid::vertexBuffer = new Buffer(4, 2, &line, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);

	// Axis
	const float triangleVertexBuffer[] = {
		 0.0f,	0.15f, 0.0f,	// top
		-0.05f, -0.05f, 0.0f,	// bottom left
		 0.05f, -0.05f, 0.0f	// bottom right
	};

	Axis::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	Axis::vertexBuffer = new Buffer(9, 3, &triangleVertexBuffer, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);
}

#ifndef USE_TEST_SHADERS

static const std::vector<std::string>	g_2DShader = {
	"#version 330 core",
	"",
	"layout(location = 0) in vec3 position;",
	"layout(location = 1) in vec2 texCoord;",
	"",
	"uniform mat4 view;",
	"uniform mat4 projection;",
	"uniform mat4 model;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main(void) ",
	"{",
	"    gl_Position = projection * view * model * vec4(position, 1.0);",
	"    TexCoords = texCoord;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"in vec2 TexCoords;",
	"",
	"uniform sampler2DArray ourTexture;",
	"uniform int layer;",
	"uniform int flipHorizontal;",
	"uniform int flipVertical;",
	"uniform float size;",
	"",
	"out vec4 color;",
	"",
	"void main(void) ",
	"{    ",
	"    vec4 color_out = texture(ourTexture, vec3(TexCoords.x * size * flipHorizontal, TexCoords.y * size * flipVertical, layer));",
	"",
	"    if(color_out.a < 0.1)",
	"        discard;",
	"",
	"	color = color_out;",
	"}"
};

static const std::vector<std::string>	g_guiShader = {
	"#version 330 core",
	"layout (location = 0) in vec2 position;",
	"",
	"uniform mat4 transformation;",
	"uniform mat4 projection;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main(void)",
	"{",
	"    gl_Position = projection * transformation * vec4(position, 0.0, 1.0);",
	"    TexCoords = vec2((position.x + 1.0) / 2, 1 - (-1 * position.y + 1.0) / 2.0);",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"in vec2 TexCoords;",
	"",
	"out vec4 color;",
	"",
	"uniform sampler2D guiTexture;",
	"uniform float opacity;",
	"uniform float numberOfRows;",
	"uniform float numberOfColumns;",
	"uniform vec2 offset;",
	"",
	"void main(void)",
	"{    ",
	"    color = texture(guiTexture, vec2(TexCoords.x / numberOfRows, TexCoords.y / numberOfColumns) + offset);",
	"    color.a = color.a - opacity;",
	"}"
};

static const std::vector<std::string>	g_fontShader = {
	"#version 330 core",
	"layout (location = 0) in vec4 vertex;",
	"",
	"uniform mat4 projection;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main()",
	"{",
	"    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);",
	"    TexCoords = vertex.zw;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"in vec2 TexCoords;",
	"out vec4 color;",
	"",
	"uniform sampler2D fontAtlas;",
	"uniform vec3 textColor;",
	"",
	"void main()",
	"{    ",
	"    color = vec4(textColor, texture(fontAtlas, TexCoords).r);",
	"}"
};

static const std::vector<std::string>	g_lineShader = {
	"#version 330",
	"",
	"layout(location = 0) in vec3 position;",
	"",
	"uniform mat4 view;",
	"uniform mat4 projection;",
	"uniform mat4 model;",
	"uniform vec4 color;",
	"",
	"out vec4 in_color;",
	"",
	"void main(void)",
	"{",
	"  gl_Position = projection * view * model * vec4(position, 1.0);",
	"  in_color = color;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330",
	"",
	"in vec4 in_color;",
	"out vec4 color;",
	"",
	"void main(void)",
	"{",
	"  color = in_color;",
	"}"
};

static const std::vector<std::string>	g_axisShader = {
	"#version 330",
	"",
	"layout(location = 0) in vec3 position;",
	"",
	"uniform mat4 view;",
	"uniform mat4 projection;",
	"uniform mat4 model;",
	"uniform vec4 color;",
	"",
	"out vec4 in_color;",
	"",
	"void main(void)",
	"{",
	"  gl_Position = projection * view * model * vec4(position, 1.0);",
	"  in_color = color;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330",
	"",
	"in vec4 in_color;",
	"out vec4 color;",
	"",
	"void main(void)",
	"{",
	"  color = in_color;",
	"}"
};

#endif

void RendererFirerays::loadShaders(void)
{
#ifdef USE_TEST_SHADERS
	SpriteRenderer::pShader = new Shader("resources/shaders/OpenGL3/2D.glsl");
	GUIRenderer::pGuiShader = new Shader("resources/shaders/OpenGL3/gui.glsl");
	TextRenderer::pTextShader = new Shader("resources/shaders/OpenGL3/font.glsl");
	Grid::pShader = new Shader("resources/shaders/OpenGL3/line.glsl");
	Axis::pShader = new Shader("resources/shaders/OpenGL3/axis.glsl");
#else
	SpriteRenderer::pShader = new Shader(g_2DShader);
	GUIRenderer::pGuiShader = new Shader(g_guiShader);
	TextRenderer::pTextShader = new Shader(g_fontShader);
	Grid::pShader = new Shader(g_lineShader);
	Axis::pShader = new Shader(g_axisShader);
#endif
}
