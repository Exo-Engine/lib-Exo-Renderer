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

#include <string>
#include <thread>
#include <glm/mat4x4.hpp>

#include "IRenderer.h"

#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "GamepadManager.h"
#include "Gamepad.h"
#include "Cursor.h"
#include "SpriteRenderer.h"
#include "GUIRenderer.h"
#include "Grid.h"
#include "TextRenderer.h"
#include "Shader.h"
#include "Texture.h"
#include "ArrayTexture.h"
#include "ObjectRenderer.h"

#include <vector>

namespace	ExoRendererFirerays2
{

	class	RendererFirerays : public ExoRenderer::IRenderer
	{
		public:

			//	Singleton methods
			static RendererFirerays&	Get(void);
			static void					 Destroy(void);

			//	class methods
			virtual void	initialize(const std::string& title, const int width, const int height, const ExoRenderer::WindowMode &mode, bool resizable);
			virtual void	resize();

			virtual ExoRenderer::ICamera*		createCamera(void);
			virtual ExoRenderer::IAxis*			createAxis(void);
			virtual ExoRenderer::ITexture*		createTexture(const std::string& filePath, ExoRenderer::TextureFilter filter = ExoRenderer::TextureFilter::LINEAR);
			virtual ExoRenderer::ITexture*		createTexture(unsigned int width, unsigned int height, ExoRenderer::TextureFormat format = ExoRenderer::TextureFormat::RGBA, ExoRenderer::TextureFilter filter = ExoRenderer::TextureFilter::LINEAR);
			virtual ExoRenderer::IArrayTexture*	createArrayTexture(int width, int height, std::vector<std::string> &textures, ExoRenderer::TextureFilter filter = ExoRenderer::TextureFilter::LINEAR);

			virtual ExoRenderer::ICursor*		createCursor();
			virtual ExoRenderer::ILabel*		createLabel();
			virtual ExoRenderer::IButton*		createButton(const std::shared_ptr<ExoRenderer::ITexture> &texture, ExoRenderer::ButtonType buttonType = ExoRenderer::ButtonType::NORMAL, bool withLabel = true);
			virtual ExoRenderer::ICheckbox*		createCheckbox(const std::shared_ptr<ExoRenderer::ITexture> &texture, bool checked = false);
			virtual ExoRenderer::IInput*		createInput(const std::shared_ptr<ExoRenderer::ITexture> &texture, const std::string &text = "", ExoRenderer::InputType type = ExoRenderer::InputType::TEXT);
			virtual ExoRenderer::IImage*		createImage(const std::shared_ptr<ExoRenderer::ITexture> &texture);
			virtual ExoRenderer::ISpinner*		createSpinner(const std::shared_ptr<ExoRenderer::ITexture> &texture);
			virtual ExoRenderer::ISlider*		createSlider(const std::shared_ptr<ExoRenderer::ITexture>& buttonTexture, const std::shared_ptr<ExoRenderer::ITexture>& barTexture);
			virtual ExoRenderer::ISelect*		createSelect(const std::shared_ptr<ExoRenderer::ITexture>& buttonTexture, const std::shared_ptr<ExoRenderer::ITexture>& backgroundTexture, const std::shared_ptr<ExoRenderer::ITexture>& scrollTexture, const std::shared_ptr<ExoRenderer::Font>& font);
			virtual ExoRenderer::IView*			createView(const std::shared_ptr<ExoRenderer::ITexture>& scrollTexture, unsigned int numberOfRows = 1, unsigned int numberOfColumns = 1);
			virtual ExoRenderer::IView*			createView(const std::shared_ptr<ExoRenderer::ITexture>& backgroundTexture, const std::shared_ptr<ExoRenderer::ITexture>& scrollTexture, unsigned int numberOfRows = 1, unsigned int numberOfColumns = 1);
			virtual ExoRenderer::ILight*		createOrthogonalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z);
			virtual ExoRenderer::ILight*		createPerspectivelLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far);
			virtual ExoRenderer::ILight*		createPointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far);
			virtual ExoRenderer::IFrameBuffer*	createFrameBuffer(void);

			virtual void	add(ExoRenderer::sprite &s);
			virtual void	add(ExoRenderer::IWidget *widget);
			virtual void	add(ExoRenderer::ILabel *label);
			virtual void	add(std::shared_ptr<ExoRenderer::ILight> &light);
			virtual void	add(ExoRenderer::IModelInstance* object);

			virtual void	remove(ExoRenderer::sprite &s);
			virtual void	remove(ExoRenderer::IWidget *widget);
			virtual void	remove(ExoRenderer::ILabel *label);
			virtual void	remove(std::shared_ptr<ExoRenderer::ILight> &light);
			virtual void	remove(ExoRenderer::IModelInstance* object);

			virtual void	draw(void);
			virtual void	swap(void);

			void			beginScissor(glm::vec2 position, glm::vec2 size, glm::vec2 parentPosition, glm::vec2 parentSize);
			void			endScissor(void);

			// Getters
			virtual ExoRenderer::IWindow*			getWindow(void);

			virtual ExoRenderer::IKeyboard*			getKeyboard(void);
			virtual ExoRenderer::IMouse*			getMouse(void);
			virtual ExoRenderer::IGamepadManager*	getGamepadManager(void);
			virtual unsigned int					getTime(void) const;

			// Setters
			virtual void	setCursor(ExoRenderer::ICursor* cursor);
			virtual void	setMousePicker(ExoRenderer::MousePicker* picker);
			virtual void	setAxis(ExoRenderer::IAxis* axis);
			virtual void	setGridEnable(bool val);

			virtual ExoRenderer::IModelInstance*	instanciate(ExoRenderer::Model* model);

			void	changesMade(void);
			void	commitChanges(void);
		protected:
			RendererFirerays(void);
			virtual ~RendererFirerays(void);

			void			createBuffers(void);
			void			loadShaders(void);

			virtual void	loadModel(ExoRenderer::Model* model);
			void			loadBodyPart(ExoRenderer::BodyPart* bodyPart);
		private:
			Window*			_pWindow;

			Keyboard		_keyboard;
			Mouse			_mouse;
			GamepadManager	_gamepad;

			SpriteRenderer*	_pSpriteRenderer;
			GUIRenderer*	_pGUIRenderer;
			TextRenderer*	_pTextRenderer;
			ObjectRenderer*	_pObjectRenderer;

			glm::mat4		_perspective, _orthographic;
			int				_scissorBit[4];

			std::thread::id	_mainThread;
			Cursor*			_pCursor;
			bool			_changes;
	};

}
