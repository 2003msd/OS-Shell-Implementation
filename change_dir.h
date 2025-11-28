
void exec_cd(char *args[]) {
    char current_dir[MAX_PATH];
    char temp_dir[MAX_PATH];
    char *home_dir = getenv("HOME");
    
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        return;
    }
    
    if (args[1] == NULL) {
        if (home_dir == NULL) {
            perror("cd");
            return;
        }
        strcpy(temp_dir, prev_dir);
        strcpy(prev_dir, current_dir);
        if (chdir(home_dir) == -1) {
            perror("cd");
            strcpy(prev_dir, temp_dir);
        }
        return;
    }
    
    if (strcmp(args[1], "-") == 0) {
        if (strlen(prev_dir) == 0) {
            printf("cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n", prev_dir);
        strcpy(temp_dir, current_dir);
        if (chdir(prev_dir) == -1) {
            perror("cd");
            return;
        }
        strcpy(prev_dir, temp_dir);
        return;
    }
    
    if (strcmp(args[1], "..") == 0) {
        strcpy(prev_dir, current_dir);
        if (chdir("..") == -1) {
            perror("cd");
        }
        return;
    }
    
    if (strcmp(args[1], "~") == 0) {
        if (home_dir == NULL) {
            perror("cd");
            return;
        }
        strcpy(prev_dir, current_dir);
        if (chdir(home_dir) == -1) {
            perror("cd");
        }
        return;
    }
    
    if (strcmp(args[1], ".") == 0) {
        return;
    }
    
    if (args[2] != NULL) {
        printf("cd: too many arguments\n");
        return;
    }
    
    strcpy(prev_dir, current_dir);
    if (chdir(args[1]) == -1) {
        perror("cd");
    }
}
//                   OSA ASSIGNMENT 3:-
//                  NAME: MANAS SACHIN DESHMUKH
//                  ROLL NO:2022102040
//                 DATE:22ND OCTOBER 2024
// COMMANDS TO RUN SHELL:
// 1) gcc main_prg.c
// 2)./a.out
// 3)type in commands to execute various functionalities of the shell