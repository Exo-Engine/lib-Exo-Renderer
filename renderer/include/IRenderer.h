/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <string>

#include "Enums.h"
#include "IWindow.h"
#include "IKeyboard.h"
#include "ICamera.h"
#include "IGamepadManager.h"
#include "IMouse.h"
#include "ICursor.h"
#include "IShader.h"
#include "ITexture.h"
#include "IArrayTexture.h"
#include "IWidget.h"
#include "IButton.h"
#include "ICheckbox.h"
#include "IInput.h"
#include "IImage.h"
#include "ISpinner.h"
#include "ISelect.h"
#include "ISlider.h"
#include "IView.h"
#include "ILabel.h"
#include "ILight.h"
#include "IFrameBuffer.h"
#include "sprite.h"
#include "MousePicker.h"
#include "IAxis.h"

namespace   ExoRenderer
{

class IRenderer
{
public:
    IRenderer(void): _UIScaleFactor(1.0f), _pCurrentCamera(nullptr), _pMousePicker(nullptr), _pAxis(nullptr), _currentNavigationType(NavigationType::MOUSE)
    { };
    
    virtual ~IRenderer(void)
    { };
    
    virtual void initialize(const std::string& title, const int width, const int height, const WindowMode &mode, bool resizable) = 0;
    virtual void resize() = 0;
    
    virtual ICamera         *createCamera(void) = 0;
    virtual IAxis           *createAxis(void) = 0;
    virtual ITexture        *createTexture(const std::string& filePath, TextureFilter filter = TextureFilter::LINEAR) = 0;
    virtual ITexture        *createTexture(unsigned int width, unsigned int height, TextureFormat format = TextureFormat::RGBA, TextureFilter filter = TextureFilter::LINEAR) = 0;
    virtual IArrayTexture   *createArrayTexture(int width, int height, std::vector<std::string> &textures, TextureFilter filter = TextureFilter::LINEAR) = 0;
    
    virtual ICursor         *createCursor() = 0;
    virtual ILabel          *createLabel() = 0;
    virtual IButton         *createButton(const std::shared_ptr<ITexture> &texture, ButtonType buttonType = ButtonType::NORMAL, bool withLabel = true) = 0;
    virtual ICheckbox       *createCheckbox(const std::shared_ptr<ITexture> &texture, bool checked = false) = 0;
    virtual IInput          *createInput(const std::shared_ptr<ITexture> &texture, const std::string &text = "", InputType type = InputType::TEXT) = 0;
    virtual IImage          *createImage(const std::shared_ptr<ITexture> &texture) = 0;
    virtual ISpinner        *createSpinner(const std::shared_ptr<ITexture> &texture) = 0;
    virtual ISlider         *createSlider(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& barTexture) = 0;
    virtual ISelect         *createSelect(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, const std::shared_ptr<Font>& font) = 0;
    virtual IView           *createView(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows = 1, unsigned int numberOfColumns = 1) = 0;
    virtual IView           *createView(const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows = 1, unsigned int numberOfColumns = 1) = 0;
	virtual ILight          *createOrthogonalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z) = 0;
	virtual ILight          *createPerspectivelLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far) = 0;
	virtual ILight          *createPointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far) = 0;
    virtual IFrameBuffer    *createFrameBuffer(void) = 0;

	virtual void push(sprite &s) = 0;
	virtual void push(IWidget *widget) = 0;
	virtual void push(ILabel *label) = 0;
	virtual void push(std::shared_ptr<ILight> &light) = 0;
	
	virtual void draw(void) = 0;
    virtual void swap(void) = 0;

    // Getters
    NavigationType &getNavigationType(void) { return _currentNavigationType; }
    float getUIScaleFactor(void) { return _UIScaleFactor; }
    ICamera *getCurrentCamera(void) { return _pCurrentCamera; }
    virtual IWindow *getWindow(void) = 0;
    
    virtual IKeyboard *getKeyboard(void) = 0;
    virtual IMouse *getMouse(void) = 0;
    virtual IGamepadManager *getGamepadManager(void) = 0;
    virtual unsigned int getTime(void) const = 0;
    
    // Setters
    void setNavigationType(const NavigationType &type) { _currentNavigationType = type; }
    void setCurrentCamera(ICamera* camera) { _pCurrentCamera = camera; }
    virtual void setCursor(ICursor* cursor) = 0;
    virtual void setMousePicker(MousePicker* picker) = 0;
    virtual void setAxis(IAxis* axis) = 0;
    virtual void setGridEnable(bool val) = 0;
protected:
    NavigationType _currentNavigationType;
	float       _UIScaleFactor;
	ICamera     *_pCurrentCamera;
    MousePicker *_pMousePicker;
    IAxis       *_pAxis;
};

}
