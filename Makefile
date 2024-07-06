includes:=-I src/include/SDL2 -I src/include/GLM -I src/include/IMGUI -I inc
links   :=-Lsrc/lib -lmingw32 -limgui -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
cflags  :=-Os -O2 -s -falign-functions=1 -falign-jumps=1 -falign-loops=1 -finline-functions -fomit-frame-pointer

all:
	cls
	g++ main.cpp -o out/main $(cflags) $(includes) $(links) 
	cd out && main

