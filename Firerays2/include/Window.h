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

#pragma once

#include "OGLCall.h"
#include <SDL2/SDL.h>
#include <string>

#include "IWindow.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "GamepadManager.h"

#include "CLWCommandQueue.h"
#include "CLWContext.h"
#include "CLWDevice.h"
#include "CLWPlatform.h"

namespace	ExoRendererFirerays2
{

	class Window : public ExoRenderer::IWindow
	{
		public:
			Window(const std::string& title, uint32_t width, uint32_t height, const ExoRenderer::WindowMode &mode, bool resizable, GamepadManager &gamepad);
			~Window(void);

			void	handleEvents(Keyboard& keyboard, Mouse& mouse, GamepadManager& gamepad);
			void	clearScreen(void);
			void	swap(void);

			//	Setters
			virtual void	setWindowSize(int w, int h);
			virtual void	setWindowMode(const ExoRenderer::WindowMode &mode);
			virtual void	setVsync(bool vsync);

			void			handleThread(void);
			virtual ExoRenderer::IFrameBuffer	*getFrameBuffer(void) const;

			//	Getters
			virtual double	getDelta(void) const;
			virtual float	getWidth(void) const;
			virtual float	getHeight(void) const;

			virtual int		getContextWidth(void) const;
			virtual int		getContextHeight(void) const;

			virtual int		getHighDPIFactor(void) const;
			virtual bool	isFullscreen(void) const;

			virtual bool	getIsClosing(void) const;

			CLWContext&			getCLContext(void);
			CLWCommandQueue&	getCLCommandQueue(void);
			CLWImage2D&			getCLTexture(void);

		private:
			void			initialize(const std::string& title, uint32_t width, uint32_t height, const ExoRenderer::WindowMode &mode, bool resizable, GamepadManager& gamepad);
			void			initPostProcessing(void);
			void			initOpenCL(void);

		private:
			SDL_Window*		_window;
			SDL_Event		_event;
			SDL_GLContext	_glContext;
			SDL_GLContext	_threadContext;

			//	Post Processing
			FrameBuffer*	_pFrameBuffer;
			Texture*		_frameTexture;
			Texture*		_depthTexture;

			Shader			_postProcessing;
			Buffer			_postVertexArrayObject;
			Buffer			_postArrayBuffer;
			Buffer			_postUVMappingBuffer;

			//	OpenCL
			CLWImage2D		_renderCLTexture;
			CLWDevice		_device;
			CLWContext		_context;
			CLWCommandQueue	_commandQueue;
};

}
