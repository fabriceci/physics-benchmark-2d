debug:
	g++ -std=c++17 -L ../rapier2d-wrapper/target/release -I ../box2d/includes/ -I ../rapier2d-wrapper/includes/ -I ../sdl2/includes/SDL2/ -I ../sdl2_gfx/includes/ -I ../sdl2_ttf/includes/ -I. -Wall ./common/*.cpp ./rapier2d/*.cpp ./box2d/*.cpp ./tests/*.cpp ./*.cpp -lm -lSDL2 -lSDL2_gfx -lSDL2_ttf -lrapier2d_wrapper -Wall -Wno-unknown-pragmas -g -o app

opti:
	g++ -std=c++17 -L ../rapier2d-wrapper/target/release -L ../box2d-optimized/libs/x64/release -I ../box2d-optimized/includes/ -I ../rapier2d-wrapper/includes/ -I ../sdl2/includes/SDL2/ -I ../sdl2_gfx/includes/ -I ../sdl2_ttf/includes/ -I. -Wall ./common/*.cpp ./rapier2d/*.cpp ./box2d/*.cpp ./tests/*.cpp ./*.cpp -lm -lSDL2 -lSDL2_gfx -lSDL2_ttf -lrapier2d_wrapper -lbox2d -Wall -O3 -Wno-unknown-pragmas -o app

build:
	g++ -std=c++17 -L ../rapier2d-wrapper/target/release -I ../box2d/includes/ -I ../rapier2d-wrapper/includes/ -I ../sdl2/includes/SDL2/ -I ../sdl2_gfx/includes/ -I ../sdl2_ttf/includes/ -I. -Wall ./common/*.cpp ./rapier2d/*.cpp ./box2d/*.cpp ./tests/*.cpp ./*.cpp -lm -lSDL2 -lSDL2_gfx -lSDL2_ttf -lrapier2d_wrapper -lbox2d -O3 -Wall -Wno-unknown-pragmas -o app

run:
	g++ -std=c++17 -L ../rapier2d-wrapper/target/release -I ../box2d/includes/ -I ../rapier2d-wrapper/includes/ -I ../sdl2/includes/SDL2/ -I ../sdl2_gfx/includes/ -I ../sdl2_ttf/includes/ -I. -Wall ./common/*.cpp ./rapier2d/*.cpp ./box2d/*.cpp ./tests/*.cpp ./*.cpp -lm -lSDL2 -lSDL2_gfx -lSDL2_ttf -lrapier2d_wrapper -lbox2d -O3 -Wall -Wno-unknown-pragmas -o app
	./app

clean:
	rm app
