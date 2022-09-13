#include "Application.h"
#include "common/physics-engine.h"

int main(int argc, char *args[]) {
	Configuration config;

    // Parse args
	char const* const ARG_ENGINE = "-engine";
	size_t const ARG_ENGINE_LEN = strlen(ARG_ENGINE);
	char const* const ARG_ENGINE_OPTIONS[(size_t)EPhysicsEngine::Count] = {
		"box2d",
		"rapier2d",
	};

	char const* const ARG_SCENE = "-scene";
	size_t const ARG_SCENE_LEN = strlen(ARG_SCENE);

	char const* const ARG_RENDER = "-render";
	char const* const ARG_NO_RENDER = "-norender";

	for (int i = 0; i < argc; ++i)
	{
		char* arg = args[i];
		size_t arg_len = strlen(arg);

		if (strncmp(arg, ARG_ENGINE, ARG_ENGINE_LEN) == 0) {
			if ((arg_len > ARG_ENGINE_LEN + 1) && (arg[ARG_ENGINE_LEN] == '=')) {
				for (int engineType = 0; engineType < (int)EPhysicsEngine::Count; ++engineType) {
					if (strcmp(arg + ARG_ENGINE_LEN + 1, ARG_ENGINE_OPTIONS[engineType]) == 0) {
						config.engine_type = (EPhysicsEngine)engineType;
						break;
					}
				}
			}
		} else if (strncmp(arg, ARG_SCENE, ARG_SCENE_LEN) == 0) {
			if ((arg_len > ARG_SCENE_LEN + 1) && (arg[ARG_SCENE_LEN] == '=')) {
				config.scene_index = atoi(arg + ARG_SCENE_LEN + 1);
			}
		} else if (strcmp(arg, ARG_RENDER) == 0) {
			config.render = true;
		} else if (strcmp(arg, ARG_NO_RENDER) == 0) {
			config.render = false;
		}
	}

    // Start the application
    Application app;
    app.Setup(config);

    while (app.IsRunning()) {
        app.Input();
        app.Update();
    }

    app.Destroy();

    return 0;
}