//
// Created by Florian on 02.09.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Font.h"

namespace Zoe {

/**
 * A Textbox is used to display a specified text at a specific position.
 */
class DLL_PUBLIC TextBox : public BaseComponent {
public:
    /**
     * Constructs an empty TextBox.
     */
    TextBox();

    /**
     * Specifies the new text.
     * @param str the new text
     */
    inline void setText(const std::string& str) { text = str; }
    /**
     * Returns the current text.
     * @returns the current text.
     */
    inline const std::string& getText() { return text; }

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
    void onClick(MouseButtonPressedEvent& event);
    void onKeyPress(KeyPressedEvent& event);

private:
    std::string text;
    Font font;
    vec4 color{};
    bool writeable{};
    bool multiLine{};
};

}

