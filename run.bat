g++ -g src/*.cpp src/Hirsch3D/*.cpp -I.\src\Hirsch3D\lib\SDL2-devel-2.0.12-VC\SDL2-2.0.12\include -I.\src\Hirsch3D\lib\glew-2.1.0\include -L.\src\Hirsch3D\lib\SDL2-devel-2.0.12-VC\SDL2-2.0.12\lib\x86 -L.\src\Hirsch3D\lib\glew-2.1.0\lib\Release\Win32 -lSDL2 -lglew32 -lSDL2main -lopengl32 -D HIRSCH_DEBUG -oDebug
Debug