#include "Application.h"

// Libs needed to link SDL2
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "version")
#pragma comment(lib, "Setupapi")

Configuration Application::configuration;

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup(Configuration const& config) {
    configuration = config;

    currentSceneIndex = config.scene_index;

    printf("Initializing rendering...\n");
    running = Graphics::OpenWindow(false);
    EngineInit(configuration.engine_type);
    SceneInit(currentSceneIndex);
    printf("Initialization Done...\n");
}

void Application::EngineInit(EPhysicsEngine engine) {
    if (currentScene) {
        delete currentScene;
        currentScene = nullptr;
    }
    if (physics_engine) {
        PhysicsEngine::destroy_engine(physics_engine);
    }
    physics_engine = PhysicsEngine::create_engine(engine);
    printf("Initializing physics engine: %s\n", physics_engine->get_name());
	Vector2D gravity(0.0f, -9.81f);
    Check(physics_engine->initialize(gravity));
}

void Application::SceneInit(int index) {
    if (currentScene) {
        delete currentScene;
        currentScene = nullptr;
    }
    if(index < 0) {
        index = 0;
    }
    currentSceneIndex = index % (int)PhysicScene::Count;

    PhysicScene selected = static_cast<PhysicScene>(currentSceneIndex);
    switch (selected) 
    {
    case PhysicScene::BoxBoxPerf:
       currentScene = new BoxPerfScene(physics_engine, PerfSceneShape::Box, PerfSceneShape::Box);
       break;
    case PhysicScene::BoxCirclePerf:
       currentScene = new BoxPerfScene(physics_engine, PerfSceneShape::Box, PerfSceneShape::Circle);
       break;
    case PhysicScene::CircleCirclePerf:
        currentScene = new BoxPerfScene(physics_engine, PerfSceneShape::Circle, PerfSceneShape::Circle);
       break;
    case PhysicScene::PolygonCirclePerf:
        currentScene = new BoxPerfScene(physics_engine, PerfSceneShape::Polygon, PerfSceneShape::Circle);
        break;
    case PhysicScene::PolygonBoxPerf:
        currentScene = new BoxPerfScene(physics_engine, PerfSceneShape::Polygon, PerfSceneShape::Box);
        break;
    case PhysicScene::PolygonPolygonPerf:
        currentScene = new BoxPerfScene(physics_engine, PerfSceneShape::Polygon, PerfSceneShape::Polygon);
        break;
    case PhysicScene::StackBox:
        currentScene = new StackBoxScene(physics_engine);
        break;
    default:
        Assert(false);
        return;
    }
    
    printf("Initializing Scene: %s...\n", currentScene->get_name());
    currentScene->Setup();
    for (PhysicsBody* body : currentScene->bodies) {
		physics_engine->insert_body(body);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_d:
                    configuration.debug = !configuration.debug;
                    break;
                case SDLK_r:
                    configuration.render = !configuration.render;
                    break;
                case SDLK_l:
                    configuration.limitFps = !configuration.limitFps;
                    break;
                case SDLK_p:
                    configuration.pause = !configuration.pause;
                    break;
                case SDLK_SPACE:
                    configuration.oneStep = true;
                    break;
                case SDLK_UP:
                    SceneInit(currentSceneIndex + 1);
                    break;
                case SDLK_DOWN:
                    SceneInit(currentSceneIndex - 1);
                    break;
                case SDLK_LEFT:
                    EngineInit(EPhysicsEngine::Box2D);
                    SceneInit(currentSceneIndex);
                    break;
                case SDLK_RIGHT:
                    EngineInit(EPhysicsEngine::Rapier2D);
                    SceneInit(currentSceneIndex);
                    break;
                case SDLK_RETURN:
                    SceneInit(currentSceneIndex);
                    break;
                default:
                    break;
                }

            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        break;
                    case SDL_BUTTON_RIGHT:
                        break;
                    default:
                        break;
                }

        }
    }
    currentScene->Input();
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    float deltaTime = 1.0f / 60.0f;
    
    if (configuration.limitFps) {
        // Wait some time until the reach the target frame time in milliseconds
        static int timePreviousFrame;
        int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
        if (timeToWait > 0.0f)
            SDL_Delay(timeToWait);

        // Calculate the deltatime in seconds
        deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
        if (deltaTime > 0.016f)
            deltaTime = 0.016f;

        // Set the time of the current frame to be used in the next one
        timePreviousFrame = SDL_GetTicks();
    }

    if (!configuration.pause || configuration.oneStep) {
        configuration.oneStep = false;
        currentScene->Update(deltaTime);
        physics_engine->step(deltaTime);
    }

    if (configuration.render) {
        Render();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    if (!configuration.render) return;
    Graphics::ClearScreen(0xFF0F0721);

    currentScene->Render();

    if (configuration.debug) {
        char textBuffer[1024];
        snprintf(textBuffer, 1024, "%s | %s [LimitFPS: %s]", currentScene->get_name(), physics_engine->get_name(), configuration.limitFps ? "ON" : "OFF");

        Graphics::DrawText(textBuffer, 0, 0);
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    printf("Releasing the scene...\n");
    if (currentScene) {
        delete currentScene;
    }

    printf("Releasing physics engine...\n");
	Check(physics_engine->release());
	PhysicsEngine::destroy_engine(physics_engine);

	printf("Releasing rendering...\n");
    Graphics::CloseWindow();
}