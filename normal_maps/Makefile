com = g++
std = -std=c++17
flags = -ggdb
gl = -lGL
files = shader.cpp stb_image.cpp camera.cpp model.cpp
warnings = -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion

output: main.cpp
	$(com) main.cpp $(flags) $(files) glad.c -Og -o output.o $$(pkg-config --static --libs glfw3) $$(pkg-config --static --libs assimp) -lGL
	./output.o

debug:
	$(com) main.cpp $(flags) $(files) glad.c -Og -o output.o $$(pkg-config --static --libs glfw3) $$(pkg-config --static --libs assimp) -lGL

run:
	./output.o

warnings:
	$(com) main.cpp $(flags) $(files) glad.c -Og -o output.o $$(pkg-config --static --libs glfw3) $$(pkg-config --static --libs assimp) -lGL $(warnings)

clean:
	rm output.o
