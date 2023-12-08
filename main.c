#include <stdio.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <ncurses.h>
#include "UI/ui.h"
#include "Filetree/filetree.h"
#define SIZE 60

void handle_interrupt() {
    endwin();
    Mix_CloseAudio();
    SDL_Quit();
    exit(0);
}

int init_sdl() {
    init();
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    return 0;
}

void error(const char *message){
	mvprintw(LINES - 1, 0, message);
    refresh();
    getch();  // Wait for user input before clearing the error message
};
void execute_command(char* command, char** current_path) {
	char *cmd;
	char *args;
  	size_t pos = strcspn(command, " ");
    strncpy(cmd, command, pos);
    args = (command[pos] != '\0') ? &command[pos + 1] : NULL;

	
    cmd[pos] = '\0';

    if (strcmp(cmd, "cd") == 0){
    	if (args == NULL){
			error("cd command needs path argument");
			return;
    	};

    	if (!path_exists(*current_path)){
    		error("path doesn't exist");
    		return;
    	};
       	*current_path = args;
    } else {
		mvprintw(LINES - 1, 0, "Unknown command: %s", command);
        refresh();
        getch();  // Wait for user input before clearing the error message    	
    };





    
       
    
}

int main() {
	signal(SIGINT, handle_interrupt);

    if (init_sdl() != 0) {
        return 1;
    }

    int ch;

    int selected_index = 0;
    char *current_path = malloc(512);
    strcpy(current_path, "/home/user/Projects");

    char command_buffer[64] = ""; // Buffer to store the command

    while ((ch = getch()) != 'q') {
        // Clear the command bar at the bottom
        mvprintw(LINES - 1, 0, ":%s", command_buffer);
        clrtoeol(); // Clear the rest of the line
        refresh();

        // Process command bar input
        if (ch == ':') {
            // Command mode
            echo(); // Enable input echoing for command input
            mvprintw(LINES - 1, 0, ":"); // Display the command prompt at the bottom
            getstr(command_buffer);
            noecho(); // Disable input echoing

            // Execute the command
            execute_command(command_buffer, &current_path);

            // Clear the command buffer
            command_buffer[0] = '\0';
        } else {
            // Handle file tree
            selected_index = handle_input(ch, selected_index, &current_path);
        }
    }

    free(current_path);
    handle_interrupt();

    return 0;
}
