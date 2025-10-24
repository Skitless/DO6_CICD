#include "grep_utils.h"

int main(int argc, char *argv[]) {
  Options option = {0};
  char pattern[BUFF_SIZE] = {0};

  get_options(argc, argv, &option, pattern);
  if (argc >= 3) {
    print_grep(&option, argc, argv, pattern);
  }
  return 0;
}

void get_options(int argc, char *argv[], Options *option, char *pattern) {
  int opt;
  const char *smbl_options = "e:ivclnhsf:o";

  while ((opt = getopt(argc, argv, smbl_options)) != -1) {
    switch (opt) {
      case 'e':
        option->option_e = 1;
        snprintf(pattern, BUFF_SIZE, "%s", optarg);
        break;
      case 'i':
        option->option_i = 1;
        break;
      case 'v':
        option->option_v = 1;
        break;
      case 'c':
        option->option_c = 1;
        break;
      case 'l':
        option->option_l = 1;
        break;
      case 'n':
        option->option_n = 1;
        break;
      case 'h':
        option->option_h = 1;
        break;
      case 's':
        option->option_s = 1;
        break;
      case 'f':
        option->option_f = 1;
        snprintf(pattern, BUFF_SIZE, "%s", optarg);
        break;
      case 'o':
        option->option_o = 1;
        break;
      default:
        printf("errror\n");
        break;
    }
  }
}

void processing(Options *option, FILE *fp, regex_t reg, char *file) {
  char text[BUFF_SIZE] = {0};
  regmatch_t pmatch[1];
  int line_match = 0;
  int nline = 0;

  while (fgets(text, BUFF_SIZE - 1, fp) != NULL) {
    int match = 0;
    int succsess = regexec(&reg, text, 1, pmatch, 0);

    if (strchr(text, '\n') == NULL) {
      strcat(text, "\n");
    }

    if (succsess == 0 && !option->option_v) {
      match = 1;
    }
    if (succsess == REG_NOMATCH && option->option_v) {
      match = 1;
    }
    if (match && !option->option_l && !option->option_c && option->option_n) {
      printf("%d:", nline);
    }
    if (match && !option->option_l && !option->option_c && !option->option_o) {
      printf("%s", text);
    }
    if (match && !option->option_o) {
      for (int i = pmatch[0].rm_eo; i < pmatch[0].rm_eo; i++) {
        printf("%c", text[i]);
      }
      // printf("\n");
    }
    line_match += match;
    nline++;
  }
  if (option->option_l && line_match > 0) {
    printf("%s\n", file);
  }

  if (option->option_c && !option->option_l) {
    printf("%d\n", line_match);
  }
}

void grep_file(Options *option, char *pattern, char *filename) {
  int cflags = REG_EXTENDED;
  regex_t reg;
  FILE *file;
  file = fopen(filename, "r");

  if (option->option_i) {
    cflags = REG_ICASE;
  }
  if (file != NULL) {
    regcomp(&reg, pattern, cflags);
    processing(option, file, reg, filename);
    regfree(&reg);
    fclose(file);
  }
}

int process_flag(char *pattern, char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  int i = 0;
  if (file != NULL) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch == 13 || ch == 10) {
        pattern[i++] = '|';
      }
      if (ch != 13 && ch != 10) {
        pattern[i++] = ch;
      }
    }
    fclose(file);  ///////
  } else {
    printf("error\n");
    i = -1;
  }

  if (pattern[i - 1] == '|') {
    pattern[i - 1] = '\0';
  }
  return i;
}

void print_grep(Options *option, int argc, char **argv, char *buff) {
  char pattern[BUFF_SIZE] = {0};
  int flg_patt = 0;

  if (!option->option_f && !option->option_e) {
    snprintf(pattern, BUFF_SIZE, "%s", argv[optind++]);
  }

  if (option->option_f) {
    flg_patt = process_flag(pattern, buff);
  }

  if (!option->option_f && option->option_e) {
    snprintf(pattern, BUFF_SIZE, "%s", buff);
  }

  if (flg_patt != -1) {
    int file_cnt = 0;
    if (argc - optind > 1) file_cnt = 1;
    for (int i = optind; i < argc; i++) {
      if (file_cnt && !option->option_h && !option->option_l)
        printf("%s:", argv[i]);
      grep_file(option, pattern, argv[i]);
    }
  }
}