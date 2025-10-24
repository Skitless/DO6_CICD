#ifndef _GREP_UTILS_H_
#define _GREP_UTILS_H_
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 5000

typedef struct {
  int option_e;
  int option_i;
  int option_v;
  int option_c;
  int option_l;

  int option_n;
  int option_h;
  int option_s;
  int option_f;
  int option_o;

} Options;

void get_options(int argc, char *argv[], Options *option, char *pattern);
void processing(Options *option, FILE *fp, regex_t reg, char *file);
void grep_file(Options *option, char *pattern, char *filename);
int process_flag(char *pattern, char *filename);
void print_grep(Options *option, int argc, char **argv, char *buff);

#endif