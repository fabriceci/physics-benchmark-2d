#pragma once

#include "./Graphics.h"
#include "common/Vector2D.h"
#include "common/Constant.h"
#include "common/physics-body.h"
#include "common/physics-engine.h"
#include "common/physics-material.h"
#include "common/physics-shape.h"
#include "common/utils.h"
#include "tests/Scene.h"
#include "tests/StackBoxScene.h"
#include "tests/PerfScene.h"
#include <vector>
#include <string>

enum class PhysicScene {
    StackBox,
    BoxBoxPerf,
    BoxCirclePerf,
    CircleCirclePerf,
    PolygonCirclePerf,
    PolygonBoxPerf,
    PolygonPolygonPerf,
    Count
};

struct Configuration {
    bool render = true;
    bool debug = true;
    bool limitFps = false;
    bool pause = false;
    bool oneStep = false;
    EPhysicsEngine engine_type = DEFAULT_PHYSICS_ENGINE;
    int scene_index = 0;
};

class Application {
    private:
        int currentSceneIndex = 0;
        bool running = false;
      
        PhysicsEngine* physics_engine = nullptr;
        Scene* currentScene = nullptr;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup(Configuration const& config);
        void EngineInit(EPhysicsEngine engine);
        void SceneInit(int index);
        void Input();
        void Update();
        void Render();
        void Destroy();

        static Configuration configuration;
};