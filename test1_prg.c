#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH 1024
#define MAX_HISTORY 20
#define DEFAULT_HISTORY_SHOW 10
#define HISTORY_FILE ".shell_history"

char prev_dir[MAX_PATH] = "";
char history[MAX_HISTORY][1024];
int history_count = 0;

void load_hist() {
    char history_path[MAX_PATH];
    snprintf(history_path, sizeof(history_path), "%s/%s", getenv("HOME"), HISTORY_FILE);
    FILE *fp = fopen(history_path, "r");
    if (fp != NULL) {
        while (history_count < MAX_HISTORY && fgets(history[history_count], 1024, fp) != NULL) {
            history[history_count][strcspn(history[history_count], "\n")] = '\0';
            history_count++;
        }
        fclose(fp);
    }
}

void save_hist() {
    char history_path[MAX_PATH];
    snprintf(history_path, sizeof(history_path), "%s/%s", getenv("HOME"), HISTORY_FILE);
    FILE *fp = fopen(history_path, "w");
    if (fp != NULL) {
        for (int i = 0; i < history_count; i++) {
            fprintf(fp, "%s\n", history[i]);
        }
        fclose(fp);
    }
}

void add_hist(const char *cmd) {
    if (history_count > 0 && strcmp(history[history_count - 1], cmd) == 0) {
        return;
    }
    
    if (history_count == MAX_HISTORY) {
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        history_count--;
    }
    
    strcpy(history[history_count], cmd);
    history_count++;
    save_hist();
}

void show_hist() {
    int start = history_count > DEFAULT_HISTORY_SHOW ? history_count - DEFAULT_HISTORY_SHOW : 0;
    for (int i = start; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

void proc_echo(const char *input) {
    const char *rest = input;
    char output[1024] = "";
    int out_pos = 0;
    int in_quotes = 0;
    
    while (*rest && (*rest == ' ' || *rest == '\t')) {
        rest++;
    }
    
    if (!*rest) {
        printf("\n");
        return;
    }

    while (*rest) {
        if (*rest == '"') {
            in_quotes = !in_quotes;
        } else if (*rest == ' ' || *rest == '\t') {
            if (in_quotes) {
                output[out_pos++] = *rest;
            } else {
                if (out_pos > 0 && output[out_pos - 1] != ' ') {
                    output[out_pos++] = ' ';
                }
            }
        } else {
            output[out_pos++] = *rest;
        }
        rest++;
    }
    
    if (!in_quotes && out_pos > 0 && output[out_pos - 1] == ' ') {
        out_pos--;
    }
    
    output[out_pos] = '\0';
    printf("%s\n", output);
}

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

int main() {
    char curr_dir[4096];
    char input[1024];
    char original_input[1024];
    char *extract_usrname = getlogin();
    char extract_sysname[2000];
    gethostname(extract_sysname, sizeof(extract_sysname));
    
    load_hist();

    while (1) {
        getcwd(curr_dir, sizeof(curr_dir));
        printf("<%s@%s:%s> ", extract_usrname, extract_sysname, curr_dir);
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        strcpy(original_input, input);
        
        char *args[10] = {NULL};
        char *token = strtok(input, " ");
        int i = 0;
        
        while (token != NULL && i < 10) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        
        if (args[0] == NULL) {
            continue;
        }
        
        add_hist(original_input);
        
        if (strcmp(args[0], "echo") == 0) {
            char *echo_start = strstr(original_input, "echo") + 4;
            proc_echo(echo_start);
        } else if (strcmp(args[0], "pwd") == 0) {
            printf("%s\n", curr_dir);
        } else if (strcmp(args[0], "cd") == 0) {
            exec_cd(args);
        } else if (strcmp(args[0], "history") == 0) {
            show_hist();
        } else if (strcmp(args[0], "clear") == 0) {
            system("clear");
        } else {
            printf("Unsupported command: %s\n", original_input);
        }
    }
    
    return 0;
}