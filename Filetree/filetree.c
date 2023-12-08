#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <unistd.h>


int count_items(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return -1; // Error opening directory
    }

    int count = 0;
    struct dirent *entry;

    // Count the items in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG) {
            count++;
        }
    }

    closedir(dir);
    return count;
}

void print_directory(const char *path, int selected_index) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    clear(); // Clear the screen

    int index = 0; // Track the current index
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
    	if (strcmp(entry->d_name, ".") == 0){continue;};
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        // Check if the entry is a directory or a regular file
        if (S_ISDIR(file_stat.st_mode)) {
            if (index == selected_index) {
                attron(A_STANDOUT); // Highlight the selected entry
            }
            printw("[%s] \n", entry->d_name);
            if (index == selected_index) {
                attroff(A_STANDOUT);
            }
        } else {
            if (index == selected_index) {
                attron(A_STANDOUT); // Highlight the selected entry
            }
            printw("%s\n", entry->d_name);
            if (index == selected_index) {
                attroff(A_STANDOUT);
            }
        }

        index++;
    }


    closedir(dir);
}

int path_exists(const char *path) {
    return access(path, F_OK) != -1;
}
int is_directory(const char *path, int selected_index) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return 0;
    }

    for (int i = 0; i <= selected_index; i++) {
        entry = readdir(dir);
        if (!entry) {
            closedir(dir);
            return 0; // Invalid index
        }
    }

    int result = entry->d_type == DT_DIR;
    closedir(dir);
    return result;
}

char *change_directory(const char *current_path, int selected_index) {
    struct dirent *entry;
    DIR *dir = opendir(current_path);
    if (!dir) {
        perror("opendir");
        return NULL; // Handle error
    }

    for (int i = 0; i < selected_index + 1; i++) {
        entry = readdir(dir);
        if (!entry) {
            closedir(dir);
            return NULL; // Invalid index
        }
    }

    closedir(dir);

    char *new_path = malloc(256);

    if (!strcmp(entry->d_name, "..")) {
        // Handle going up one directory level
        size_t len = strlen(current_path);
        size_t last_slash = len - 1;
        while (last_slash > 0 && current_path[last_slash] != '/') {
            last_slash--;
        }

        snprintf(new_path, 256, "%.*s", (int)last_slash, current_path);
    } else {
        snprintf(new_path, 256, "%s/%s", current_path, entry->d_name);
    }

    return new_path;
}



int handle_input(int ch, int selected_index, char **current_path){

	 switch (ch) {
            case KEY_UP:
            	if (selected_index > 0){
                	selected_index--;
                }
                break;
            case KEY_DOWN:
               	selected_index++;
                break;
          case 261:
			    *current_path = change_directory(*current_path, selected_index);

        }

	print_directory(*current_path, selected_index);
	return selected_index;
    
};


