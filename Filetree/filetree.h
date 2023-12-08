#ifndef FILETREE_H
#define FILETREE_H

int handle_input(int ch, int selected_index, char **current_path);
char *change_directory(char *current_path, int selected_index);
int path_exists(const char *path);

#endif
