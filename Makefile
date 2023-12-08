compile:
	gcc UI/ui.c Filetree/filetree.c main.c -o main -I/usr/include/SDL2 -lSDL2 -lSDL2_mixer -lncurses -g
	
