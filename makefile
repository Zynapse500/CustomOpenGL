CC = g++

CPPFILES = $(wildcard src/*.cpp)

EXE = main.exe

INCLUDES = -I ./include -I ../glfw-3.2.bin.WIN32/include -I ..\glew-2.0.0\include -I ..\SOIL\src -I ..\glm -I ../assimp-3.2/include

LIBRARIES = -L ../glfw-3.2.bin.WIN32/lib-mingw-w64 -L ..\glew-2.0.0\lib -L ..\SOIL\lib -L ../assimp-3.2/build/code

LIBS = -lglew32 -lSOIL -lopengl32 -lglu32 -lglfw3 -lassimp

$(EXE): $(CPPFILES)
	@echo Compiling $(CPPFILES)...
	@$(CC) -o $@ $^ $(INCLUDES) $(LIBRARIES) $(LIBS) 
	@#-mwindows
	@echo Done!

.PHONY: $(EXE)

run: $(EXE)
	@cd ..
	@echo
	@echo Executing program...
	@echo _____________________________
	@echo
	@$^
	@echo
	@echo _____________________________
	@echo Finished!