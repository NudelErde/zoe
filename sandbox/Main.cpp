//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"

using namespace Zoe;

class MyScript: public NativeScript{
public:
    mat4x4 originalMatrix;
    mat4x4 lambdaMatrix;
    double lambda = 0;
    double startLambda = 0;
    double endLambda = 3;
    double duration = 20;

    void onInit() override {
        originalMatrix[0][0] = 3;originalMatrix[0][1] = 0;originalMatrix[0][2] = 0;originalMatrix[0][3] = 0;
        originalMatrix[1][0] =-9;originalMatrix[1][1] = 6;originalMatrix[1][2] = 0;originalMatrix[1][3] = 0;
        originalMatrix[2][0] = 0;originalMatrix[2][1] = 0;originalMatrix[2][2] = 1;originalMatrix[2][3] = 0;
        originalMatrix[3][0] = 0;originalMatrix[3][1] = 0;originalMatrix[3][2] = 0;originalMatrix[3][3] = 1;

        lambdaMatrix[0][0] = 1;lambdaMatrix[0][1] = 0;lambdaMatrix[0][2] = 0;lambdaMatrix[0][3] = 0;
        lambdaMatrix[1][0] = 0;lambdaMatrix[1][1] = 1;lambdaMatrix[1][2] = 0;lambdaMatrix[1][3] = 0;
        lambdaMatrix[2][0] = 0;lambdaMatrix[2][1] = 0;lambdaMatrix[2][2] = 0;lambdaMatrix[2][3] = 0;
        lambdaMatrix[3][0] = 0;lambdaMatrix[3][1] = 0;lambdaMatrix[3][2] = 0;lambdaMatrix[3][3] = 0;
    }
    void onUpdate(const double & time) override {
        if(std::shared_ptr<WorldObject> worldObject = std::dynamic_pointer_cast<WorldObject>(component.lock())){
            lambda += time/(60.0/(endLambda-startLambda));
            if(lambda >= endLambda) {
                lambda = startLambda;
            }
            mat4x4 m = originalMatrix-(lambda*lambdaMatrix);
            info("Lambda: ", lambda);
            info("det: ", m.determinant());
            worldObject->getModel().setModelMatrix(m);
        }
    }
};

class App : public Application {
public:
    App() {
        std::shared_ptr<ComponentLayer> ptr = std::make_shared<ComponentLayer>();
        ptr->load(File("sampleObjects/Cube.xml"));
        std::shared_ptr<Camera3D> cam = std::make_shared<Camera3D>(vec3({0, 0, -6}), vec3({0, 0, 0}),
                                                                             70.0f, 16.0f / 9.0f);
        ptr->setCamera(cam);
        getLayerStack().pushLayer(ptr);

        NativeScriptComponent::registerNativeScript<MyScript>("test");
    }

    ~App() override = default;
};

Application* Zoe::createApplication() {
    return new App();
}
