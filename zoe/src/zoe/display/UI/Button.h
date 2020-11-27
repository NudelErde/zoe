//
// Created by Florian on 23.07.2020.
//

#pragma once

#include "../NativeScriptComponent.h"
#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Font.h"

namespace Zoe {

/**
 * A button is a basic UI element which is used to handle clicks.
 */
class DLL_PUBLIC Button : public BaseComponent {
public:

    /**
     * Constructs an empty Button.
     */
    Button();

    /**
     * Specifies the click callback. The handle is called when a MouseButtonReleasedEvent is triggered, the component is
     * active and the mouse is inside this component.
     * @param handler the click handler
     */
    void setClickHandler(const std::function<void()>& handler);

    /**
     * Returns the current size of this component, where x is the width and y is the height.
     * @returns the current size
     */
    inline const vec2& getSize() const { return size; }
    /**
     * Specifies the new size if this component is x the width and y is the height.
     * @param size the new size
     */
    inline void setSize(const vec2& size) { this->size = size; }

    /**
     * Returns the current background color of this component, where x, y, z, w are r, g, b, a.
     * @returns the current color
     */
    const vec4& getBackgroundColor() const;
    /**
     * Specifies the new background color of this component, where x, y, z, w are r, g, b, a.
     * @param color the new color
     */
    void setBackgroundColor(const vec4& color);

    /**
     * Returns the current background color of this component, where x, y, z, w are r, g, b, a.
     * @returns the current color
     */
    const vec4& getTextColor() const;
    /**
     * Specifies the new background color of this component, where x, y, z, w are r, g, b, a.
     * @param color the new color
     */
    void setTextColor(const vec4& color);

    /**
     * Returns the displayed text.
     * @return the displayed text
     */
    const std::string& getText();

    /**
     * Sets the displayed text.
     * @param text the new text
     */
    void setText(const std::string& text);

    /**
     * Return the used font.
     * @return the used font
     */
    const Font& getFont();

    /**
     * Set the used font.
     * @param font the used font
     */
    void setFont(const Font& font);

protected:
    /**
     * Draws this component on the specified camera.
     * @param camera the specified camera
     */
    void onDraw(const Camera& camera) override;
    /**
     * Updates this component.
     * @param time the time in seconds since the last update
     */
    void onUpdate(double time) override;
    /**
     * Handles input events.
     * @param event the input event
     */
    void onInputEvent(Event& event) override;
    /**
     * Fills this component with the information in the specified xml node.
     * @param node the xml node
     */
    void fill(const XMLNode& node) override;
    /**
     * Completes initialization. This method is called after all children are initialized. It is used to extract information for children.
     */
    void postFill() override;

private:
    void onMouseClick(MouseButtonReleasedEvent& mbre);

private:
    std::function<void()> onClick;
    vec2 size{};
    vec4 backgroundColor{};
    vec4 textColor{};
    std::string text;
    Font font;
    std::weak_ptr<NativeScriptComponent> script{};
};

}