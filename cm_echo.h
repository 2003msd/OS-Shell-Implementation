


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