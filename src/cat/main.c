#include "cat_utils.h"

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc == 1) {
    if (CatNoArgs(STDIN_FILENO, argv[0])) {
      return 1;
    }
  } else {
    if (CatArgs(argc, argv)) {
      return 1;
    }
  }
  return 0;
}

void CatArgsHelper(Options option, char *ptr, char *end, bool *empty) {
  static int line_number = 1;
  if (end == ptr) {
    if (!option.option_s) {
      if (option.option_n) {
        printf("%6d\t", line_number);
        line_number++;
      }
      if (option.option_e) {
        if (option.option_b) {
          printf("%6s\t$\n", "");
        } else {
          printf("$\n");
        }
      } else {
        printf("\n");
      }
      return;
    }
    if (!*empty) {
      printf("\n");
    }
    *empty = true;
    return;
  } else {
    *empty = false;
  }

  if (option.option_b) {
    printf("%6d\t", line_number);
    line_number++;
  }

  if (option.option_n) {
    printf("%6d\t", line_number);
    line_number++;
  }

  if (option.option_e) {
    if (end) {
      for (char *c = ptr; c < end; c++) {
        if (*c == '\n') {
          printf("$\n");
        } else if (*c == '\t') {
          printf("\t");
        } else {
          if (*c < ' ' || *c >= 127) {
            printf("^%c", *c + 64);
          } else {
            printf("%c", *c);
          }
        }
      }
      if (*(end - 1) != '\n') {
        printf("$\n");
      }
    } else {
      if (option.option_n) {
        printf("%d ", line_number);
        line_number++;
      }
      for (char *c = ptr; *c; c++) {
        if (*c == '\n') {
          printf("$");
        } else if (*c == '\t') {
          printf("\t");
        } else {
          if (*c < ' ' || *c >= 127) {
            printf("^%c", *c + 64);
          } else {
            printf("%c", *c);
          }
        }
      }
    }
    *empty = false;
    return;
  }

  if (option.option_v) {
    if (end) {
      for (char *c = ptr; c < end; c++) {
        if (*c == '\n') {
          printf("\n");
        } else if (*c == '\t') {
          printf("\t");
        } else {
          if (*c < ' ' || *c >= 127) {
            printf("^%c", *c + 64);
          } else {
            printf("%c", *c);
          }
        }
      }
      if (*(end - 1) != '\n') {
        printf("\n");
      }
    } else {
      if (option.option_n) {
        printf("%d ", line_number);
        line_number++;
      }
      for (char *c = ptr; *c; c++) {
        if (*c == '\n') {
          printf("\n");
        } else if (*c == '\t') {
          printf("\t");
        } else {
          if (*c < ' ' || *c >= 127) {
            printf("^%c", *c + 64);
          } else {
            printf("%c", *c);
          }
        }
      }
    }
    *empty = false;
    return;
  }

  if (option.option_t) {
    if (end) {
      for (char *c = ptr; c < end; c++) {
        if (*c == '\n') {
          printf("\n");
        } else if (*c == '\t') {
          printf("^I");
        } else {
          if (*c < ' ' || *c >= 127) {
            printf("^%c", *c + 64);
          } else {
            printf("%c", *c);
          }
        }
      }
      if (*(end - 1) != '\n') {
        printf("\n");
      }
    } else {
      if (option.option_n) {
        printf("%d ", line_number);
        line_number++;
      }
      for (char *c = ptr; *c; c++) {
        if (*c == '\n') {
          printf("$");
        } else if (*c == '\t') {
          printf("^I");
        } else {
          if (*c < ' ' || *c >= 127) {
            printf("^%c", *c + 64);
          } else {
            printf("%c", *c);
          }
        }
      }
    }
    *empty = false;
    return;
  }

  if (!end) {
    printf("%s", ptr);
  } else {
    printf("%.*s", (int)(end - ptr + 1), ptr);
  }

  if (end == NULL && option.option_e && !*empty) {
    printf("$\n\n");
  }
}

bool CatArgsPerform(Options *option, char *file, char *name) {
  char buf[4096];
  int fd;
  int bytes_read;
  char *end;
  char *ptr;
  bool empty = false;

  fd = open(file, O_RDONLY);

  if (fd == -1) {
    perror(name);
    return false;
  }

  bytes_read = read(fd, buf, 4095);
  while (bytes_read > 0) {
    buf[bytes_read] = 0;
    ptr = buf;
    end = strchr(ptr, '\n');
    while (end) {
      CatArgsHelper(*option, ptr, end, &empty);
      ptr = end + 1;
      end = strchr(ptr, '\n');
    }
    CatArgsHelper(*option, ptr, NULL, &empty);
    bytes_read = read(fd, buf, 4095);
  }
  return bytes_read != -1;
}

bool CatNoArgs(int fd, char *name) {
  char buf[4096];
  int bytes_read;

  if (fd == -1) {
    perror(name);
    return false;
  }

  bytes_read = read(fd, buf, 4096);

  while (bytes_read > 0) {
    printf("%.*s", bytes_read, buf);
    bytes_read = read(fd, buf, 4096);
  }
  return bytes_read != -1;
}

bool CatParseArg(Options *option, char *argv, char *name) {
  if (argv[0] == '-') {
    if (argv[1] == '-') {
      if (strcmp(argv + 2, "number-nonblank") == 0) {
        option->option_b = true;
      } else if (strcmp(argv + 2, "number") == 0) {
        option->option_n = true;
      } else if (strcmp(argv + 2, "squeeze-blank") == 0) {
        option->option_s = true;
      } else {
        dprintf(STDERR_FILENO, "%s: invalid option '%s'\n", name, argv);
        return false;
      }
    } else {
      for (int i = 1; argv[i]; ++i) {
        switch (argv[i]) {
          case 'b':
            option->option_b = true;
            break;
          case 'e':
            option->option_e = true;
            break;
          case 'n':
            option->option_n = true;
            break;
          case 's':
            option->option_s = true;
            break;
          case 't':
            option->option_t = true;
            break;
          case 'v':
            option->option_v = true;
            break;
          default:
            dprintf(STDERR_FILENO, "%s: invalid option '-%c'\n", name, argv[i]);
            return false;
        }
      }
    }
  } else {
    dprintf(STDERR_FILENO, "%s: invalid option '%s'\n", name, argv);
    return false;
  }

  return true;
}

bool CatArgs(int argc, char *argv[]) {
  Options option = {0, 0, 0, 0, 0, 0};

  for (int i = 1; i < argc; ++i) {
    if (*argv[i] == '-') {
      if (!CatParseArg(&option, argv[i], argv[0])) {
        return false;
      }
    }
  }

  if (!(option.option_b || option.option_e || option.option_n ||
        option.option_s || option.option_t || option.option_v)) {
    for (int i = 1; i < argc; ++i) {
      if (*argv[i] != '-') {
        if (CatNoArgs(open(argv[i], O_RDONLY), argv[0])) {
          return false;
        }
      }
    }
  } else {
    for (int i = 1; i < argc; ++i) {
      if (*argv[i] != '-') {
        if (CatArgsPerform(&option, argv[i], argv[0])) {
          return false;
        }
      }
    }
  }
  return true;
}
