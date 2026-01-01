#include <random>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Rotate.h"
#include "Application.h"
#include "Camera.h"
#include "CompositeTransformation.h"
#include "DynamicRotate.h"
#include "FireFlyObject.h"
#include "Light.h"
#include "Scale.h"
#include "Transform.h"
#include "Scene.h"
#include "Models/sphere.h"
#include "Models/triangle.h"
#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/plain.h"

Scene scenes[10];

int main() {
    Application app;
    Camera camera;

    ShaderProgram skybox("Shaders/skyBox.vert", "Shaders/skyBox.frag");
    ShaderProgram constant("Shaders/basic.vert", "Shaders/constant.frag");
    ShaderProgram lambert("Shaders/basic.vert", "Shaders/lambert.frag");
    ShaderProgram phong("Shaders/basic.vert", "Shaders/phong.frag");
    ShaderProgram blinn("Shaders/basic.vert", "Shaders/blinn.frag");
    ShaderProgram phongMulti("Shaders/basic.vert", "Shaders/phongMultiLight.frag");
    ShaderProgram phongMultiTexture("Shaders/normalsTexture.vert", "Shaders/phongMultiLightTexture.frag");

    // Create light sources with its own positions, direction, colors and light intensity
    Light flashlight(LightType::Spotlight,
                 camera.getPosition(),
                 camera.getFront(),
                 glm::vec3(1.0f, 1.0f, 1.0f),
                 1.0f);

    Light pointLight(LightType::Point,
             glm::vec3(0.0f,0.0f, 0.0f),
             glm::vec3(0.0f),
             glm::vec3(1.0f, 1.0f, 1.0f),
             1.0f);
    Light pointLight2(LightType::Point,
         glm::vec3(0.0f,2.0f, 0.0f),
         glm::vec3(0.0f),
         glm::vec3(1.0f, 1.0f, 1.0f),
         1.0f);
    Light pointLight3(LightType::Point,
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        2.0f);
    Light pointLight4(LightType::Point,
         glm::vec3(0.0f,1.0f, -2.0f),
         glm::vec3(0.0f),
         glm::vec3(1.0f, 1.0f, 1.0f),
         5.0f);

    // Load models from Models using .h file
    Model sphereModel(sphere, sizeof(sphere), 2880);
    Model triangleModel(triangle, sizeof(triangle), 3);
    Model treeModel(tree, sizeof(tree), 92814);
    Model bushModel(bushes, sizeof(bushes), 9035);
    Model plainModel(plain, sizeof(plain), 6);

    // Load models from assets using .obj file
    Model shrekModel("shrek.obj");
    Model fionaModel("fiona.obj");
    Model toiledModel("toiled.obj");
    Model loginModel("text2.obj");

    // Create transforms for object position
    auto translateBall1 = std::make_shared<Transform>(glm::vec3(-2.0f, 0.0f, 0.0f));
    auto translateBall2 = std::make_shared<Transform>(glm::vec3(0.0f, 2.0f, 0.0f));
    auto translateBall3 = std::make_shared<Transform>(glm::vec3(0.0f, -2.0f, 0.0f));
    auto translateBall4 = std::make_shared<Transform>(glm::vec3(2.0f, 0.0f, 0.0f));
    auto translateTriangle = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
    auto translatePlain = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));

    // Create scales for object size
    auto scalePlain = std::make_shared<Scale>(glm::vec3(100.0f));
    auto scale = std::make_shared<Scale>(glm::vec3(0.2f));

    // Add rotation to an object
    auto rotateAnimation = std::make_shared<DynamicRotate>(0.01f, glm::vec3(0, 0, 1));

    // Create combined transformations
    auto chainedBall1 = std::make_shared<CompositeTransformation>();
    auto chainedBall2 = std::make_shared<CompositeTransformation>();
    auto chainedBall3 = std::make_shared<CompositeTransformation>();
    auto chainedBall4 = std::make_shared<CompositeTransformation>();

    auto chainedTriangle = std::make_shared<CompositeTransformation>();

    auto chainedPlain = std::make_shared<CompositeTransformation>();

    // Combine all the different transformations for an object to execute in an order
    chainedBall1->add(scale);
    chainedBall1->add(translateBall1);
    chainedBall2->add(scale);
    chainedBall2->add(translateBall2);
    chainedBall3->add(scale);
    chainedBall3->add(translateBall3);
    chainedBall4->add(scale);
    chainedBall4->add(translateBall4);
    chainedBall4->add(rotateAnimation);


    chainedTriangle->add(rotateAnimation);
    //chainedTriangle->add(scale);
    //chainedTriangle->add(rotate);

    chainedPlain->add(translatePlain);
    chainedPlain->add(scalePlain);

    // Create the object with its own model, shader, transformations and color
    DrawableObject ball1(&sphereModel, &phongMultiTexture, chainedBall1.get(), glm::vec3(1.0f, 0.0f, 0.0f));
    DrawableObject ball2(&sphereModel, &phongMultiTexture, chainedBall2.get(), glm::vec3(0.0f, 1.0f, 0.0f));
    DrawableObject ball3(&sphereModel, &phongMultiTexture, chainedBall3.get(), glm::vec3(0.0f, 0.0f, 1.0f));
    DrawableObject ball4(&sphereModel, &phong, chainedBall4.get(), glm::vec3(1.0f, 1.0f, 1.0f));
    DrawableObject shrekObject3(&shrekModel, &phongMultiTexture, chainedBall4.get(), glm::vec3(1.0f, 1.0f, 1.0f));

    DrawableObject triangleObject(&triangleModel, &constant, chainedTriangle.get(), glm::vec3(1.0f, 1.0f, 0.0f));

    DrawableObject plainObject(&plainModel, &phongMultiTexture, chainedPlain.get(), glm::vec3(0.0f, 0.4f, 0.0f));

    // First scene with rotating triangle
    Scene scene1;
    scene1.add(std::make_shared<DrawableObject>(triangleObject));
    scene1.addShader(&constant);
    scene1.addLight(pointLight);

    // An example scene with four differently colored balls to show the usage of Phong shader
    Scene scene2;
    scene2.add(std::make_shared<DrawableObject>(ball1));
    scene2.add(std::make_shared<DrawableObject>(ball2));
    scene2.add(std::make_shared<DrawableObject>(ball3));
    scene2.add(std::make_shared<DrawableObject>(shrekObject3));

    //scene2.add(std::make_shared<DrawableObject>(ball4));

    //scene2.addShader(&phong);
    scene2.addShader(&phongMultiTexture);
    scene2.addLight(pointLight);
    //scene2.addLight(pointLight2);

    // Dark forest scene with moving fireflies as light sources and models from Shrek
    Scene scene3;

    // Initialize randomizer
    int min = -50;
    int max = 50;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);

    std::vector<std::shared_ptr<CompositeTransformation>> transforms;

    // Add trees to the forest
    for (int i = 0; i < 50; ++i) {
        // Random X and Z, fixed Y (e.g., ground level)
        float x = dist(gen);
        float z = dist(gen);
        float y = 0.0f; // trees at ground level

        // Random scale variation
        float scaleVal = 0.8f + static_cast<float>(rand()) / RAND_MAX * 0.4f; // between 0.8–1.2
        auto scale = std::make_shared<Scale>(glm::vec3(scaleVal));

        // Create transformation for each tree
        auto translate = std::make_shared<Transform>(glm::vec3(x, y, z));

        // Combine transformations
        auto composite = std::make_shared<CompositeTransformation>();
        composite->add(scale);
        composite->add(translate);
        transforms.push_back(composite);

        // Create drawable tree object
        auto treeObject = std::make_shared<DrawableObject>(&treeModel, &phongMultiTexture, composite.get(), glm::vec3(0.0f, 0.39f, 0.0f));

        // Add to scene
        scene3.add(treeObject);
    }

    // Add bushes to the forest
    for (int i = 0; i < 50; ++i) {
        // Random X and Z, fixed Y (e.g., ground level)
        float x = dist(gen);
        float z = dist(gen);
        float y = 0.0f; // assuming bushes grow from ground

        // Random scale variation
        float scaleVal = 0.8f + static_cast<float>(rand()) / RAND_MAX * 0.4f; // between 0.8–1.2
        auto scale = std::make_shared<Scale>(glm::vec3(scaleVal));

        // Create transformation for each bush
        auto translate = std::make_shared<Transform>(glm::vec3(x, y, z));

        // Combine transformations
        auto composite = std::make_shared<CompositeTransformation>();
        composite->add(scale);
        composite->add(translate);
        transforms.push_back(composite);

        // Create drawable bush object
        auto bushObject = std::make_shared<DrawableObject>(&bushModel, &phongMultiTexture, composite.get(), glm::vec3(0.0f, 1.0f, 0.0f));

        // Add to scene
        scene3.add(bushObject);
    }

    // Add fireflies to the scene
    const int FIREFLY_COUNT = 15;
    std::vector<std::shared_ptr<FireFlyObject>> fireflies;

    std::uniform_real_distribution<float> distPos(-100.0f, 100.0f);
    std::uniform_real_distribution<float> distVel(-0.5f, 0.5f);
    std::uniform_real_distribution<float> distColor(0.7f, 1.0f);

    for (int i = 0; i < FIREFLY_COUNT; ++i) {
        glm::vec3 pos(distPos(gen), 3.0f, distPos(gen));
        glm::vec3 vel(distVel(gen), 0.0f, distVel(gen));
        glm::vec3 color(distColor(gen), distColor(gen) * 0.8f, 0.3f);

        auto f = std::make_shared<FireFlyObject>(&sphereModel, &phongMultiTexture, pos, color, vel, 0.05f, 12.0f);

        scene3.addFirefly(f);
        scene3.addLight(f->getLightRef());
    }

    // Add forest floor
    scene3.add(std::make_shared<DrawableObject>(plainObject));
    scene3.addShader(&phongMultiTexture);

    auto translateShrek = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
    auto translateFiona = std::make_shared<Transform>(glm::vec3(-2.0f, 0.0f, 0.0f));
    auto translateToiled = std::make_shared<Transform>(glm::vec3(2.0f, 0.0f, 0.0f));

    auto chainedShrek = std::make_shared<CompositeTransformation>();
    auto chainedFiona = std::make_shared<CompositeTransformation>();
    auto chainedToiled = std::make_shared<CompositeTransformation>();

    chainedShrek->add(translateShrek);
    //chainedShrek->add(scale);

    chainedFiona->add(translateFiona);
    //chainedFiona->add(scale);

    chainedToiled->add(translateToiled);
    //chainedToiled->add(scale);

    DrawableObject shrekObject(&shrekModel, &phongMultiTexture, chainedShrek.get(), glm::vec3(1.0f, 1.0f, 1.0f));
    DrawableObject fionaObject(&fionaModel, &phongMultiTexture, chainedFiona.get(), glm::vec3(1.0f, 1.0f, 1.0f));
    DrawableObject toiledObject(&toiledModel, &phongMultiTexture, chainedToiled.get(), glm::vec3(1.0f, 1.0f, 1.0f));

    DrawableObject shrekObject2(&shrekModel, &skybox, chainedShrek.get(), glm::vec3(1.0f, 1.0f, 1.0f));
    DrawableObject fionaObject2(&fionaModel, &skybox, chainedFiona.get(), glm::vec3(1.0f, 1.0f, 1.0f));
    DrawableObject toiledObject2(&toiledModel, &skybox, chainedToiled.get(), glm::vec3(1.0f, 1.0f, 1.0f));

    Scene scene4;


    //scene4.add(std::make_shared<DrawableObject>(shrekObject2));
    //scene4.add(std::make_shared<DrawableObject>(fionaObject2));
    //scene4.add(std::make_shared<DrawableObject>(toiledObject2));

    scene3.add(std::make_shared<DrawableObject>(shrekObject));
    scene3.add(std::make_shared<DrawableObject>(fionaObject));
    scene3.add(std::make_shared<DrawableObject>(toiledObject));
    scene3.addLight(pointLight3);

    std::vector<std::string> skyFaces = {
        "assets/cubemap/negx.jpg",
        "assets/cubemap/posx.jpg",
        "assets/cubemap/posy.jpg",
        "assets/cubemap/negy.jpg",
        "assets/cubemap/negz.jpg",
        "assets/cubemap/posz.jpg"
    };

    auto skyboxObject = std::make_shared<SkyboxObject>(skyFaces, &skybox);

    scene4.setSkybox(skyboxObject);

    scene4.addShader(&skybox);

    scene4.addLight(pointLight2);

    // Solar system scene
    Scene sceneSolar;
    sceneSolar.addShader(&phongMultiTexture);

    std::vector<std::shared_ptr<CompositeTransformation>> solarTransforms;

    auto sunScale = std::make_shared<Scale>(glm::vec3(4.0f));
    auto sunTranslate = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
    auto sunComposite = std::make_shared<CompositeTransformation>();
    sunComposite->add(sunScale);
    sunComposite->add(sunTranslate);

    solarTransforms.push_back(sunComposite);

    auto sunDrawable = std::make_shared<DrawableObject>(&sphereModel, &phongMultiTexture, sunComposite.get(), glm::vec3(1.0f, 0.9f, 0.6f));
    sceneSolar.add(sunDrawable);

    Light sunLight(LightType::Point, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 0.95f, 0.8f), 70.0f);
    //sunLight.constant = 1.0f;
    //sunLight.linear = 0.014f;
    //sunLight.quadratic = 0.0007f;
    sceneSolar.addLight(sunLight);

    struct PlanetOut {
        std::shared_ptr<CompositeTransformation> parent;
        std::shared_ptr<CompositeTransformation> local;
    };

    auto makePlanet = [&](float distance, float scaleVal, glm::vec3 color,
                          float orbitSpeedDeg, const std::string& name) -> PlanetOut
    {
        float speedRad = glm::radians(orbitSpeedDeg);

        auto orbitRotate = std::make_shared<DynamicRotate>(speedRad, glm::vec3(0,1,0));
        auto translate   = std::make_shared<Transform>(glm::vec3(distance, 0, 0));
        auto scale       = std::make_shared<Scale>(glm::vec3(scaleVal));

        auto localComposite = std::make_shared<CompositeTransformation>();
        localComposite->add(translate);
        localComposite->add(scale);

        auto parentComposite = std::make_shared<CompositeTransformation>();
        parentComposite->add(orbitRotate);
        parentComposite->add(localComposite);

        solarTransforms.push_back(localComposite);
        solarTransforms.push_back(parentComposite);

        auto drawable = std::make_shared<DrawableObject>(
            &sphereModel, &phongMultiTexture, parentComposite.get(), color
        );
        sceneSolar.add(drawable);

        return { parentComposite, localComposite };
    };

    makePlanet( 8.0f, 0.4f, glm::vec3(0.7f, 0.5f, 0.3f),  0.48f, "Mercury");
    makePlanet(12.0f, 0.95f, glm::vec3(0.9f, 0.8f, 0.6f), 0.35f, "Venus");
    PlanetOut earth = makePlanet(16.0f, 1.0f, glm::vec3(0.2f, 0.5f, 1.0f), 0.24f, "Earth");
    makePlanet(20.0f, 0.53f, glm::vec3(1.0f, 0.3f, 0.2f), 0.20f, "Mars");
    makePlanet(42.0f, 2.5f, glm::vec3(0.95f, 0.85f, 0.5f), 0.12f, "Jupiter");
    makePlanet(56.0f, 2.1f, glm::vec3(0.95f, 0.9f, 0.7f), 0.10f, "Saturn");
    makePlanet(68.0f, 1.5f, glm::vec3(0.5f, 0.7f, 0.95f), 0.08f,  "Uranus");
    makePlanet(80.0f, 1.4f, glm::vec3(0.2f, 0.5f, 0.7f), 0.06f,  "Neptune");
    float moonDistance = 3.0f;
    float moonScale    = 0.27f;
    float moonSpeedDeg = 0.24f;

    float speedRad = glm::radians(moonSpeedDeg);

    auto orbitMoon  = std::make_shared<DynamicRotate>(speedRad, glm::vec3(0,1,0));
    auto transMoon  = std::make_shared<Transform>(glm::vec3(moonDistance, 0, 0));
    auto scaleMoon  = std::make_shared<Scale>(glm::vec3(moonScale));

    auto moonLocal = std::make_shared<CompositeTransformation>();
    moonLocal->add(transMoon);
    moonLocal->add(scaleMoon);


    auto moonParent = std::make_shared<CompositeTransformation>();
    moonParent->add(earth.parent);
    moonParent->add(orbitMoon);
    moonParent->add(moonLocal);

    solarTransforms.push_back(moonLocal);
    solarTransforms.push_back(moonParent);

    auto moonDrawable = std::make_shared<DrawableObject>(
        &sphereModel, &phongMultiTexture, moonParent.get(),
        glm::vec3(0.85f)
    );

    sceneSolar.add(moonDrawable);

    // Rotating login scene
    Scene sceneLogin;

    sceneLogin.addShader(&phongMultiTexture);

    auto translateLogin = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
    auto rotateLogin = std::make_shared<DynamicRotate>(0.02f,glm::vec3(1,1,1));

    auto chainedLogin = std::make_shared<CompositeTransformation>();

    chainedLogin->add(translateLogin);
    chainedLogin->add(rotateLogin);

    DrawableObject loginObject(&loginModel, &phongMultiTexture, chainedLogin.get(), glm::vec3(1.0f, 1.0f, 1.0f));

    sceneLogin.add(std::make_shared<DrawableObject>(loginObject));

    sceneLogin.addLight(pointLight4);

    // Add individual scenes to a scenes array
    scenes[0] = scene1;
    scenes[1] = scene2;
    scenes[2] = scene3;
    scenes[3] = scene4;
    scenes[4] = sceneSolar;
    scenes[5] = sceneLogin;

    // Give application an array of scenes to run
    app.run(scenes);
    return 0;
}