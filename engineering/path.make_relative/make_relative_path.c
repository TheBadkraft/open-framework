#include <stdio.h>
#include <string.h>

/*
	Composed by Leo AI on 8/24/2024
	Where did Leo get the source?
*/
void make_relative(const char *base, const char *target, char *relative) {
    char base_components[256][256], target_components[256][256];
    int base_parts = 0, target_parts = 0;

    // Tokenize base and target paths
    const char *base_token = strtok(base, "/");
    while (base_token != NULL) {
        strcpy(base_components[base_parts++], base_token);
        base_token = strtok(NULL, "/");
    }

    const char *target_token = strtok(target, "/");
    while (target_token != NULL) {
        strcpy(target_components[target_parts++], target_token);
        target_token = strtok(NULL, "/");
    }

    // Compare components
    int i;
    for (i = 0; i < target_parts; i++) {
        if (i < base_parts && strcmp(base_components[i], target_components[i]) == 0) {
            continue;
        }
        break;
    }

    // Construct relative path
    strcpy(relative, "");
    for (; i < target_parts; i++) {
        if (strcmp(target_components[i], "..") == 0) {
            if (i == 0) continue;
            strcat(relative, "../");
        } else {
            strcat(relative, target_components[i]);
            if (i != target_parts - 1) strcat(relative, "/");
        }
    }

    if (strcmp(relative, "") == 0) strcat(relative, ".");
}

int main() {
    const char base[] = "/home/user/documents";
    const char target[] = "/home/user/documents/file.txt";
    char relative[512];

    make_relative(base, target, relative);
    printf("Relative path is: %s\n", relative);

    return 0;
}
