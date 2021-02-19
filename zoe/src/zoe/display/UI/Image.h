//
// Created by Florian on 18.02.2021.
//

#pragma once

#include "../Component.h"
#include <memory>

namespace Zoe {

class Image : public BaseComponent {
public:

    Image();

    void setSource(const File& source);
    [[nodiscard]] const File& getSource() const;

    inline void setSize(const vec2& v) { size = v; }
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
    verc2 size;
};

}
