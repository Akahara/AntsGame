CC = clang
CXX = clang++

CFLAGS := -O0 -g
CXXFLAGS := $(CFLAGS) -std=c++2b

LDFLAGS := -O0 -g

FLAGS := -IMarbleEngine/Libraries/include -IMarbleEngine
LIBS := -lglfw -lm

MARBLE_ENGINE := MarbleEngine
ANT_GAME := AntGame

SRC_MARBLE_ENGINE := $(MARBLE_ENGINE)/Libraries/include/glm/detail/glm.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/VertexArray.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Mesh.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Cubemap.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/UnifiedRenderer.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Texture.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Window.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Inputs.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Camera.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/Shader.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/VertexBufferObject.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/IndexBufferObject.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/stb_image_impl.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/TextureAtlas.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/FrameBufferObject.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/ComputeShader.cpp \
	$(MARBLE_ENGINE)/marble/abstraction/animation/Joint.cpp \
	$(MARBLE_ENGINE)/marble/Utils/Debug.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui_demo.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui_tables.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui_impl_glfw.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui_impl_opengl3.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui_draw.cpp \
	$(MARBLE_ENGINE)/marble/vendor/imgui/imgui_widgets.cpp \
	$(MARBLE_ENGINE)/marble/Sandbox/Scene.cpp \
	$(MARBLE_ENGINE)/marble/World/TerrainGeneration/HeightMap.cpp \
	$(MARBLE_ENGINE)/marble/World/TerrainGeneration/Noise.cpp \
	$(MARBLE_ENGINE)/marble/World/TerrainGeneration/Terrain.cpp \
	$(MARBLE_ENGINE)/marble/World/Props/PropsManager.cpp \
	$(MARBLE_ENGINE)/marble/World/Player.cpp \
	$(MARBLE_ENGINE)/marble/World/SunCameraHelper.cpp \
	$(MARBLE_ENGINE)/marble/World/Sky.cpp \
	$(MARBLE_ENGINE)/marble/World/Grass.cpp \
	$(MARBLE_ENGINE)/marble/World/Water/WaterRenderer.cpp \
	$(MARBLE_ENGINE)/marble/World/Water/Water.cpp \
	$(MARBLE_ENGINE)/marble/World/Water/WaterSource.cpp \
	$(MARBLE_ENGINE)/marble/vendor/glad.c

OBJ_MARBLE_ENGINE := $(SRC_MARBLE_ENGINE:.cpp=.o)
OBJ_MARBLE_ENGINE := $(OBJ_MARBLE_ENGINE:.c=.o)

SRC_ANTGAME := $(ANT_GAME)/ants/GameLogic/Pheremones.cpp \
	$(ANT_GAME)/ants/GameLogic/AntsPlayer.cpp \
	$(ANT_GAME)/ants/GameLogic/Score.cpp \
	$(ANT_GAME)/ants/main.cpp \
	$(ANT_GAME)/ants/Maze/MazeGeneration_proj/CException.cpp \
	$(ANT_GAME)/ants/Maze/MazeGeneration_proj/CMaze.cpp \
	$(ANT_GAME)/ants/Maze/MazeGeneration_proj/CMazeGenerator.cpp \
	$(ANT_GAME)/ants/Maze/MazeGeneration_proj/CTile.cpp \
	$(ANT_GAME)/ants/Maze/MazeGeneration_proj/libAntMaze.cpp \
	$(ANT_GAME)/ants/Maze/MazeMeshGenerator.cpp \
	$(ANT_GAME)/ants/Maze/MazeTileSystem.cpp \
	$(ANT_GAME)/ants/Client/client.cpp \
	$(ANT_GAME)/ants/Client/JSON.cpp \
	$(ANT_GAME)/ants/GameScenes/Scene.cpp

OBJ_ANTGAME := $(SRC_ANTGAME:.cpp=.o)

all: game

clean:
	rm -f $(OBJ_MARBLE_ENGINE) libmarbleengine.a game

%.o: %.cpp
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(FLAGS)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(FLAGS)

libmarbleengine.a: $(OBJ_MARBLE_ENGINE)
	$(AR) rcu $@ $^

game: $(OBJ_ANTGAME) libmarbleengine.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: all libmarbleengine.a game
