#ifndef _CAT_UTILS_H_
#define _CAT_UTILS_H_
#define _GNU_SOURCE

#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  bool option_b;
  bool option_e;
  bool option_n;
  bool option_s;
  bool option_t;
  bool option_v;
  // bool number_files;

} Options;

void CatArgsHelper(Options option, char *ptr, char *end, bool *empty);
bool CatArgsPerform(Options *option, char *file, char *name);
bool CatNoArgs(int fd, char *name);
bool CatParseArg(Options *option, char *argv, char *name);
bool CatArgs(int argc, char *argv[]);

#endif