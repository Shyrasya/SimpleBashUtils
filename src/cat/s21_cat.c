#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int error = 0, file_index = 0;
  if (argc > 1) {
    flags cur_flags = {0};
    char *short_options = "beEnstTv";
    struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                    {"number", no_argument, NULL, 'n'},
                                    {"squeeze-blank", no_argument, NULL, 's'}};

    int par = parsing(argc, argv, short_options, long_options, &cur_flags,
                      &file_index);
    if (!par) {
      if (print_cur_symbol(argc, argv, &cur_flags, &file_index)) error = 1;
    } else
      error = 1;
  }
  return error;
}

int parsing(int argc, char *argv[], char *short_options,
            struct option long_options[], flags *cur_flags, int *file_index) {
  int active_flag = 0, error = 0, index = 0;

  while ((active_flag = getopt_long(argc, argv, short_options, long_options,
                                    &index)) != -1) {
    switch (active_flag) {
      case 'b':
        cur_flags->b_num_no_empty = 1;
        break;
      case 'e':
        cur_flags->e_dollend = 1;
        cur_flags->v_nonprint_char = 1;
        break;
      case 'E':
        cur_flags->e_dollend = 1;
        break;
      case 'n':
        cur_flags->n_num_all = 1;
        break;
      case 's':
        cur_flags->s_squeeze = 1;
        break;
      case 't':
        cur_flags->t_tab = 1;
        cur_flags->v_nonprint_char = 1;
        break;
      case 'T':
        cur_flags->t_tab = 1;
        break;
      case 'v':
        cur_flags->v_nonprint_char = 1;
        break;
      default:
        error = 1;
        break;
    }
  }
  *file_index = optind;

  return error;
}

int print_cur_symbol(int argc, char *argv[], flags *cur_flags,
                     int *file_index) {
  int error = 0;

  for (int i = *file_index; (i < argc); i++) {
    FILE *file = fopen(argv[i], "r");
    if (file != NULL) {
      print_file_not_null(file, cur_flags);
    } else {
      error = 1;
      fprintf(stderr, "%s: No such file or directory\n", argv[i]);
    }
  }
  return error;
}

void print_file_not_null(FILE *file, flags *cur_flags) {
  int symbol = fgetc(file), empty_line = 0, index_line = 0,
      previous_symbol = '\n';
  while (symbol != EOF) {
    if (!(cur_flags->s_squeeze == 1 && previous_symbol == '\n' &&
          symbol == '\n' && empty_line)) {
      if (previous_symbol == '\n' && symbol == '\n')
        empty_line = 1;
      else
        empty_line = 0;
      if ((((cur_flags->n_num_all == 1 && cur_flags->b_num_no_empty == 0) ||
            (cur_flags->b_num_no_empty == 1 && symbol != '\n')) &&
           (previous_symbol == '\n'))) {
        index_line++;
        printf("%6d\t", index_line);
      }
      if (cur_flags->e_dollend == 1 && symbol == '\n') printf("$");
      if (cur_flags->t_tab == 1 && symbol == '\t') {
        printf("^");
        symbol = '\t' + 64;
      }
      if (cur_flags->v_nonprint_char == 1) {
        if (symbol > 127 && symbol < 160) printf("M-^");
        if ((symbol < 32 && symbol != '\n' && symbol != '\t') || symbol == 127)
          printf("^");
        if ((symbol < 32 || (symbol > 126 && symbol < 160)) && symbol != '\n' &&
            symbol != '\t')
          symbol = symbol > 126 ? symbol - 128 + 64 : symbol + 64;
      }
      fputc(symbol, stdout);
    }
    previous_symbol = symbol;
    symbol = fgetc(file);
  }
  fclose(file);
}