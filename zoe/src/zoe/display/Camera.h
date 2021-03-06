//
// Created by Florian on 11.07.2020.
//

#pragma once

#include "../core/Core.h"
#include "../math/mat.h"
#include "../math/vec.h"
#include "../render/api/Render.h"
#include "ChaiScriptComponent.h"
#include "Component.h"

namespace Zoe {

class Model;

class Material;

/**
 * Cameras encapsulate a render object with a specific target and the transformation needed to draw an object with a model matrix.
 */
class DLL_PUBLIC Camera : public BaseComponent {
public:
    /**
     * Creates a new emtpy Camera. An empty Camera has empty view and projection matrices so it cannot draw anything until both matrices are changed.
     */
    Camera();

    /**
     * Destructs the Camera object.
     */
    virtual ~Camera();

    /**
     * Specifies the new view matrix. InvViewMatrix is also calculated. The determinant of viewMatrix cannot be equal to 0.
     * @param viewMatrix the new view matrix
     */
    void setViewMatrix(const mat4x4& viewMatrix);

    /**
     * Specifies the new projection matrix.
     * @param projectionMatrix the new projection matrix
     */
    void setProjectionMatrix(const mat4x4& projectionMatrix);

    /**
     * Returns the view matrix.
     * @return the view matrix
     */
    [[nodiscard]] const mat4x4& getViewMatrix() const;

    /**
     * Returns the inverse of the view matrix. This matrix is calculated when the view matrix is set.
     * @return the inverse of the view matrix.
     */
    [[nodiscard]] const mat4x4& getInvViewMatrix() const;

    /**
     * Returns the projection matrix.
     * @return the projection matrix
     */
    [[nodiscard]] const mat4x4& getProjectionMatrix() const;

    /**
     * Draw a specified model. It uses the cameras projection and view matrix and the models model matrix. Does nothing if the render object is not set.
     * @param model the specified model
     */
    void draw(Model& model) const;

    /**
     * Specifies the new Render object.
     * @param render the Render object
     */
    inline void setRender(const std::shared_ptr<Render>& render) { this->render = render; }

    /**
     * Returns the used Render object.
     * @return the used Render object
     */
    inline const std::shared_ptr<Render>& getRender() const { return render; }


protected:

    /**
     * Fills this object with attributes from the XMLNode
     * @param node the node from which the attributes are extracted
     */
    void fill(const XMLNode &node) override;
    /**
     * Fills this object after initialization of all children to finish initialization
     */
    void postFill() override;
    /**
     * Does nothing
     * @param time time in seconds since last onUpdate call
     */
    void onUpdate(double time) override;
    /**
     * Does nothing
     * @param camera the camera used to request the draw
     */
    void onDraw(const Camera &camera) override;
    /**
     * Does nothing
     * @param event the input event that was raised
     */
    void onInputEvent(Event &event) override;
protected:

    /**
     * Updates the camera position.
     */
    virtual void updateCameraMatrix() = 0;

private:
    mat4x4 viewMatrix;
    mat4x4 projectionMatrix;
    mat4x4 invViewMatrix;

    std::shared_ptr<Render> render;
};

/**
 * A Camera used for 2D scenes. A parallel projection is used.
 */
class DLL_PUBLIC Camera2D : public Camera {
public:
    /**
     * Create a Camera2D with a specified position and size.
     * @param position the specified camera position
     * @param width the specified camera width
     * @param height the specified camera height
     */
    Camera2D(const vec2& position, const double& width, const double& height);

    /**
     * Create an empty Camera2D.
     */
    Camera2D() = default;

protected:
    /**
     * Updates the camera position.
     */
    void updateCameraMatrix() override;

    /**
     * Fills this object with attributes from the XMLNode
     * @param node the node from which the attributes are extracted
     */
    void fill(const XMLNode &node) override;

private:
    double width, height;
};

/**
 * A Camera used for 3D scenes. A perspective projection is used.
 */
class DLL_PUBLIC Camera3D : public Camera {
public:
    /**
     * Create a Camera3D with a specified position, rotation field of view and width to height ratio.
     * @param position the specified position
     * @param rotation the specified rotation. The x-value corresponds to a rotation by x radians around the x-axis. The same for y and z.
     * @param fov the specified field of view. The Fov is given in degrees
     * @param ratio the specified ration. It can be calculated by width/height
     */
    Camera3D(const vec3& position, const vec3& rotation, const double& fov, const double& ratio);

    /**
     * Create an empty Camera3D.
     */
    Camera3D() = default;

    /**
     * Set the rotation of the Camera. The view matrix and inverse view matrix are updated.
     * @param rotation the new camera rotation in radians
     */
    void setRotation(const vec3& rotation);

    /**
     * Set the pitch of the Camera. The view matrix and inverse view matrix are updated.
     * @param pitch the new camera pitch in radians
     */
    void setPitch(const double& pitch);

    /**
     * Set the yaw of the Camera. The view matrix and inverse view matrix are updated.
     * @param yaw the new camera yaw in radians
     */
    void setYaw(const double& yaw);

    /**
     * Set the roll of the Camera. The view matrix and inverse view matrix are updated.
     * @param roll the new camera roll in radians
     */
    void setRoll(const double& roll);

    /**
     * Set the rotation of the Camera to the specified pitch, yaw and roll. This function equals `Camera::setRotation(vec3({pitch, yaw, roll}))`.
     * The view matrix and inverse view matrix are updated.
     * @param pitch the specified pitch in radians
     * @param yaw the specified yaw in radians
     * @param roll the specified roll in radians
     */
    void setRotation(const double& pitch, const double& yaw, const double& roll);

    /**
     * Set the direction that the camera is pointing.
     * The view matrix and inverse view matrix are updated.
     * @param direction the new direction
     */
    void setDirection(const vec3& direction);

    /**
     * Set the direction and roll that the camera is pointing.
     * The view matrix and inverse view matrix are updated.
     * @param direction the new direction
     * @param roll the new roll
     */
    void setDirection(const vec3& direction, const double& roll);

    /**
     * Set the field of view of the camera.
     * Thew view matrix and inverse view matrix are updated.
     * @param fov the fov in degrees.
     */
    void setFoV(const double& fov);

    /**
     * Set the ratio of width and height. Can be calculated by width/height.
     * Thew view matrix and inverse view matrix are updated.
     * @param ratio the width and height
     */
    void setRatio(const double& ratio);

    /**
     * Rotates this camera by the given value in radians.
     * Thew view matrix and inverse view matrix are updated.
     * @param value the given rotation in radians
     */
    void rotate(const vec3& value);

    /**
     * Returns the rotation of the Camera.
     * @return the rotation of the Camera
     */
    [[nodiscard]] const vec3& getRotation() const;

    /**
     * Calculates the direction the Camera is pointing.
     * @return the direction the Camera is pointing
     */
    [[nodiscard]] vec3 getDirection() const;

    /**
     * Returns the field of view of the Camera.
     * @return the FoV in degrees
     */
    [[nodiscard]] inline const double& getFoV() const { return fov; }
    /**
     * Returns the ratio of width and height. Can be calculated by `width/height`
     * @return the ratio
     */
    [[nodiscard]] inline const double& getRatio() const { return ratio; }
protected:

    /**
     * Updates the camera position.
     */
    void updateCameraMatrix() override;

    /**
     * Fills this object with attributes from the XMLNode
     * @param node the node from which the attributes are extracted
     */
    void fill(const XMLNode &node) override;

private:
    vec3 rotation;
    double fov, ratio;
};

}