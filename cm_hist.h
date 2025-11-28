
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