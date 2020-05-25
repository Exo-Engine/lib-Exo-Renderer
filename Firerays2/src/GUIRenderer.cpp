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

#include <glm/gtc/matrix_transform.hpp>

#include "GUIRenderer.h"
#include "RendererFirerays.h"

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

Shader* GUIRenderer::pGuiShader = nullptr;

GUIRenderer::GUIRenderer(void)
: _vaoBuffer(nullptr), _vertexBuffer(nullptr)
{
	const float tmp[] = {
		-1.0f,	1.0f,
		1.0f,	1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f
	};

	_vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	_vertexBuffer = new Buffer(8, 2, &tmp, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);
}

GUIRenderer::~GUIRenderer(void)
{
	if (_vaoBuffer)
		delete _vaoBuffer;

	if (_vertexBuffer)
		delete _vertexBuffer;
}

void GUIRenderer::add(IWidget *widget)
{
	_renderQueue.push_back(widget);
}

void GUIRenderer::remove(IWidget *widget)
{
	for (std::deque<IWidget*>::iterator iterator = _renderQueue.begin(); iterator != _renderQueue.end(); iterator++)
	{
		if (*iterator == widget)
		{
			_renderQueue.erase(iterator);
			return ;
		}
	}
}

void GUIRenderer::render(const glm::mat4& orthographic)
{
	_orthographic = orthographic;
	prepare(orthographic);

	for (IWidget* widget : _renderQueue)
	{
		if (render(widget, pGuiShader) == 1)
			prepare(orthographic);
	}

	// Front
	for (IWidget* widget : _renderFrontQueue)
	{
		switch (widget->getType())
		{
			case IWidget::SELECT: {
				auto select = (Select*)widget;
				render(select->getView(), pGuiShader);
				break;
			}
			default: break;
		}

		prepare(_orthographic);
	}
}

// private
void GUIRenderer::prepare(const glm::mat4& orthographic)
{
	pGuiShader->bind();
	pGuiShader->setMat4("projection", orthographic);

	// Render
	_vaoBuffer->bind();
	_vertexBuffer->bind();
}

unsigned char GUIRenderer::render(IWidget* widget, Shader* shader)
{
	switch (widget->getType())
	{
		case IWidget::BUTTON: {
			auto button = (Button*)widget;
			return render(button, shader);
		}
		case IWidget::CHECKBOX: {
			auto checkbox = (Checkbox*)widget;
			return render(checkbox, shader);
		}
		case IWidget::SELECT: {
			auto select = (Select*)widget;
			return render(select, shader);
		}
		case IWidget::INPUT: {
			auto input = (Input*)widget;
			return render(input, shader);
		}
		case IWidget::SLIDER: {
			auto slider = (Slider*)widget;
			return render(slider, shader);
		}
		case IWidget::IMAGE: {
			auto image = (Image*)widget;
			return render(image, shader);
		}
		case IWidget::SPINNER: {
			auto spinner = (Spinner*)widget;
			return render(spinner, shader);
		}
		case IWidget::VIEW: {
			auto view = (View*)widget;
			return render(view, shader);
		}
		default: return 0;
	}
}

unsigned char GUIRenderer::render(Button* button, Shader* shader)
{
	shader->setFloat("opacity", button->getOpacity());
	button->getTexture()->bind();

	if (button->getSliced())
	{
		shader->setFloat("numberOfRows", 3.0f);
		shader->setFloat("numberOfColumns", 6.0f);

		glm::vec2 tempPosition = button->getRealPosition() + button->getVirtualOffset() + button->getRelativeParentPosition();
		bool isHoverOffset = button->getTextureIndex() == 1 ? true : false;

		float cornerSize = button->getScaleSize().y / 3.0f;
		if (button->getScaleSize().y > button->getScaleSize().x)
			cornerSize = button->getScaleSize().x / 3.0f;

		drawSliced(shader, 0, 0, tempPosition.x - button->getScaleSize().x + cornerSize, tempPosition.y - button->getScaleSize().y + cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Top left
		drawSliced(shader, 0, 2, tempPosition.x - button->getScaleSize().x + cornerSize, tempPosition.y + button->getScaleSize().y - cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Bottom left
		drawSliced(shader, 1, 0, tempPosition.x, tempPosition.y - button->getScaleSize().y + cornerSize, button->getScaleSize().x - cornerSize * 2, cornerSize, isHoverOffset, 3, 6);			 // Top center
		drawSliced(shader, 2, 0, tempPosition.x + button->getScaleSize().x - cornerSize, tempPosition.y - button->getScaleSize().y + cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Top right
		drawSliced(shader, 2, 1, tempPosition.x + button->getScaleSize().x - cornerSize, tempPosition.y, cornerSize, button->getScaleSize().y - cornerSize * 2, isHoverOffset, 3, 6);			 // Center right
		drawSliced(shader, 1, 2, tempPosition.x, tempPosition.y + button->getScaleSize().y - cornerSize, button->getScaleSize().x - cornerSize * 2, cornerSize, isHoverOffset, 3, 6);			 // Bottom center
		drawSliced(shader, 2, 2, tempPosition.x + button->getScaleSize().x - cornerSize, tempPosition.y + button->getScaleSize().y - cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Bottom left
		drawSliced(shader, 0, 1, tempPosition.x - button->getScaleSize().x + cornerSize, tempPosition.y, cornerSize, button->getScaleSize().y - cornerSize * 2, isHoverOffset, 3, 6);			 // Center left
		drawSliced(shader, 1, 1, tempPosition.x, tempPosition.y, button->getScaleSize().x - cornerSize * 2, button->getScaleSize().y - cornerSize * 2, isHoverOffset, 3, 6);						// Center
	}
	else
	{
		shader->setFloat("numberOfRows", button->getNumberOfRows());
		shader->setFloat("numberOfColumns", button->getNumberOfColumns());

		static glm::mat4 transformationMatrix;
		transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button->getRealPosition() + button->getVirtualOffset() + button->getRelativeParentPosition(), 0.0f)); // Translate
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(button->getScaleSize(), 0.0f)); // Scale
		shader->setMat4("transformation", transformationMatrix);

		shader->setVec2("offset", button->getOffset());
		GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}

	return 0;
}

unsigned char GUIRenderer::render(Checkbox* checkbox, Shader* shader)
{
	static glm::mat4 transformationMatrix;
	transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(checkbox->getRealPosition() + checkbox->getVirtualOffset() + checkbox->getRelativeParentPosition(), 0.0f)); // Translate
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(checkbox->getScaleSize(), 0.0f)); // Scale
	shader->setMat4("transformation", transformationMatrix);

	shader->setFloat("opacity", checkbox->getOpacity());
	shader->setFloat("numberOfRows", 1.0f);
	shader->setFloat("numberOfColumns", 4.0f);
	shader->setVec2("offset", checkbox->getOffset());

	checkbox->getTexture()->bind();
	GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

	return 0;
}

unsigned char GUIRenderer::render(Select* select, Shader* shader)
{
	if (select->isOpen())
		_renderFrontQueue.push_back(select);

	return render(select->getButton(), shader);
}

unsigned char GUIRenderer::render(Input* input, Shader* shader)
{
	shader->setFloat("opacity", input->getOpacity());
	input->getTexture()->bind();

	if (input->getSliced())
	{
		shader->setFloat("numberOfRows", 3.0f);
		shader->setFloat("numberOfColumns", 6.0f);

		glm::vec2 tempPosition = input->getRealPosition() + input->getVirtualOffset() + input->getRelativeParentPosition();
		bool isHoverOffset = input->getSelected() == 1 ? true : false;

		float cornerSize = input->getScaleSize().y / 3.0f;
		if (input->getScaleSize().y > input->getScaleSize().x)
			cornerSize = input->getScaleSize().x / 3.0f;

		drawSliced(shader, 0, 0, tempPosition.x - input->getScaleSize().x + cornerSize, tempPosition.y - input->getScaleSize().y + cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Top left
		drawSliced(shader, 0, 2, tempPosition.x - input->getScaleSize().x + cornerSize, tempPosition.y + input->getScaleSize().y - cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Bottom left
		drawSliced(shader, 1, 0, tempPosition.x, tempPosition.y - input->getScaleSize().y + cornerSize, input->getScaleSize().x - cornerSize * 2, cornerSize, isHoverOffset, 3, 6);			 // Top center
		drawSliced(shader, 2, 0, tempPosition.x + input->getScaleSize().x - cornerSize, tempPosition.y - input->getScaleSize().y + cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Top right
		drawSliced(shader, 2, 1, tempPosition.x + input->getScaleSize().x - cornerSize, tempPosition.y, cornerSize, input->getScaleSize().y - cornerSize * 2, isHoverOffset, 3, 6);			 // Center right
		drawSliced(shader, 1, 2, tempPosition.x, tempPosition.y + input->getScaleSize().y - cornerSize, input->getScaleSize().x - cornerSize * 2, cornerSize, isHoverOffset, 3, 6);			 // Bottom center
		drawSliced(shader, 2, 2, tempPosition.x + input->getScaleSize().x - cornerSize, tempPosition.y + input->getScaleSize().y - cornerSize, cornerSize, cornerSize, isHoverOffset, 3, 6);	// Bottom left
		drawSliced(shader, 0, 1, tempPosition.x - input->getScaleSize().x + cornerSize, tempPosition.y, cornerSize, input->getScaleSize().y - cornerSize * 2, isHoverOffset, 3, 6);			 // Center left
		drawSliced(shader, 1, 1, tempPosition.x, tempPosition.y, input->getScaleSize().x - cornerSize * 2, input->getScaleSize().y - cornerSize * 2, isHoverOffset, 3, 6);						// Center
	}
	else
	{
		shader->setFloat("numberOfRows", (float)input->getNumberOfRows());
		shader->setFloat("numberOfColumns", (float)input->getNumberOfColumns());

		static glm::mat4 transformationMatrix;
		transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(input->getRealPosition() + input->getVirtualOffset() + input->getRelativeParentPosition(), 0.0f)); // Translate
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(input->getScaleSize(), 0.0f)); // Scale
		shader->setMat4("transformation", transformationMatrix);

		shader->setVec2("offset", 0, 0);
		GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}
	return 0;
}

unsigned char GUIRenderer::render(Image* image, Shader* shader)
{
	static glm::mat4 transformationMatrix;
	transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(image->getRealPosition() + image->getVirtualOffset() + image->getRelativeParentPosition(), 0.0f)); // Translate
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(image->getScaleSize(), 0.0f)); // Scale
	transformationMatrix = glm::rotate(transformationMatrix, image->getRotation(), glm::vec3(0, 0, 1)); // Rotation
	shader->setMat4("transformation", transformationMatrix);

	shader->setFloat("opacity", image->getOpacity());
	shader->setFloat("numberOfRows", image->getNumberOfRows());
	shader->setFloat("numberOfColumns", image->getNumberOfColumns());
	shader->setVec2("offset", image->getOffset());

	image->getTexture()->bind();
	GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

	return 0;
}

unsigned char GUIRenderer::render(Spinner* spinner, Shader* shader)
{
	static glm::mat4 transformationMatrix;
	transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(spinner->getRealPosition() + spinner->getVirtualOffset() + spinner->getRelativeParentPosition(), 0.0f)); // Translate
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(spinner->getScaleSize(), 0.0f)); // Scale
	transformationMatrix = glm::rotate(transformationMatrix, spinner->getRotation(), glm::vec3(0, 0, 1)); // Rotation
	shader->setMat4("transformation", transformationMatrix);

	shader->setFloat("opacity", spinner->getOpacity());
	shader->setFloat("numberOfRows", 1);
	shader->setFloat("numberOfColumns", 1);
	shader->setVec2("offset", 0, 0);

	spinner->getTexture()->bind();
	GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

	return 0;
}

unsigned char GUIRenderer::render(View* view, Shader* shader)
{
	// Render background
	if (view->getBackgroundTexture().get() != nullptr)
	{
		static glm::mat4 transformationMatrix;
		transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(view->getRealPosition() + view->getVirtualOffset() + view->getRelativeParentPosition(), 0.0f)); // Translate
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(view->getScaleSize(), 0.0f)); // Scale
		shader->setMat4("transformation", transformationMatrix);
		shader->setFloat("opacity", 0.0f);

		view->getBackgroundTexture()->bind();
		GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}

	// Render scroll
	if (view->getLastScrollHeight() > view->getRealPosition().y + view->getRelativeParentPosition().y + view->getScaleSize().y)
	{
		auto scrollbarButton = view->getScrollbarButton();
		render(scrollbarButton, shader);
	}

	// Render childs
	RendererFirerays::Get().beginScissor(glm::vec2(view->getRealPosition().x + view->getVirtualOffset().x + view->getRelativeParentPosition().x - view->getScaleSize().x,
													RendererFirerays::Get().getWindow()->getHeight() - (view->getRealPosition().y + view->getVirtualOffset().y + view->getRelativeParentPosition().y + view->getScaleSize().y)),
											glm::vec2(view->getScaleSize().x * 2, view->getScaleSize().y * 2),
											glm::vec2(view->getParentScissor().x, view->getParentScissor().y),
											glm::vec2(view->getParentScissor().z, view->getParentScissor().w));

	TextRenderer tempTextRenderer;

	// Back
	for (IWidget* widget : view->getRenderQueue())
	{
		//widget->contextInfo(RendererFirerays::Get().getUIScaleFactor(), RendererFirerays::Get().getWindow()->getWidth(), RendererFirerays::Get().getWindow()->getHeight());

		// Temporary
		switch (widget->getType())
		{
			case IWidget::SELECT: {
				auto select = (Select*)widget;
				tempTextRenderer.add(select->getLabel());
				break;
			}
			case IWidget::BUTTON: {
				auto button = (Button*)widget;
				tempTextRenderer.add(button->getLabel());
				break;
			}
			case IWidget::INPUT: {
				auto input = (Input*)widget;
				tempTextRenderer.add(input->getLabel());
				break;
			}
			default: break;
		}

		if (render(widget, shader) == 1)
			prepare(_orthographic);
	}

	for (Label* label : view->getLabelRenderQueue())
	{
		label->contextInfo(RendererFirerays::Get().getUIScaleFactor(), RendererFirerays::Get().getWindow()->getWidth(), RendererFirerays::Get().getWindow()->getHeight());
		tempTextRenderer.add(label);
	}
	tempTextRenderer.render(_orthographic);
	prepare(_orthographic);

	RendererFirerays::Get().endScissor();

	return 1;
}

unsigned char GUIRenderer::render(Slider* slider, Shader* shader)
{
	render(slider->getBarImage(), shader);
	render(slider->getSliderButton(), shader);
	return 0;
}

void GUIRenderer::drawSliced(Shader* shader, unsigned int offsetX, unsigned int offsetY, float positionX, float positionY, float sizeX, float sizeY, bool isHoverOffset, unsigned int numberOfRows, unsigned int numberOfColumns)
{
	static glm::mat4 transformationMatrix;

	transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(positionX, positionY, 0.0f)); // Translate
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(sizeX, sizeY, 0.0f)); // Scale
	shader->setMat4("transformation", transformationMatrix);

	shader->setVec2("offset", (float)offsetX / numberOfRows, ((offsetY + (isHoverOffset == true ? 3 : 0)) % numberOfColumns) / (float)numberOfColumns);
	GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
}
