//
// Created by Florian on 18.02.2021.
//

#pragma once

#include "../Component.h"
#include <memory>

namespace Zoe {

/**
 * An Image component is used to display an Image at a specific Position.
 */
class Image : public BaseComponent {
public:

    Image(); ///< Constructs an empty Image.

    /**
     * Sets the image file for this component.
     * @param source the image file
     */
    void setSource(const File& source);

    /**
     * Returns the image file for this component.
     * @return the image file
     */
    [[nodiscard]] const File& getSource() const;

    /**
     * Sets the size of this component.
     * @param v the size
     */
    inline void setSize(const vec2& v) { size = v; }

    /**
     * Returns the size of this component.
     * @return the size
     */
    [[nodiscard]] inline const vec2& getSize() const { return size; }

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
    std::unique_ptr<File> imageSource;
    std::shared_ptr<Texture> texture;
    vec2 size;
};

}
