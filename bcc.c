#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dataSize = 30000;
int instructionSize = 200;

int dp = 0;
int ip = 0;

void debugPrint(char instruction, char *data, int dp, int ip) {
#ifdef DEBUG
  if (DEBUG) {
    printf("Instruction: %c; dp: %d; data[dp]: %d; ip: %d\n", instruction, dp,
           data[dp], ip);
  }
#endif
}

void upsize(void **arr, int *curSize, int neededSize) {
  if (neededSize == *curSize) {
    void *tmp = realloc(*arr, 2 * *curSize * sizeof(*arr));
    if (tmp) {
      *arr = tmp;
      *curSize *= 2;
    }
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stdout, "usage: bcc filename.bf\n");
    return 1;
  }

  char *data = calloc(dataSize, sizeof(char));
  char *instructions = calloc(instructionSize, sizeof(char));
  int c;

  FILE *file = fopen(argv[1], "r");
  int instructionCount = 0;

  if (file) {
    while ((c = getc(file)) != EOF) {
      if (c != '[' && c != ']' && c != '>' && c != '<' && c != '+' &&
          c != '-' && c != '.' && c != ',') {
        continue;
      }
      upsize((void **)&instructions, &instructionSize, instructionCount);
      instructions[instructionCount++] = c;
    }
    fclose(file);
  } else {
    fprintf(stderr, "Couldn't open file: %s\n", argv[1]);
    return 1;
  }

  // build parentheses relationships
  int parenRelations[instructionCount];
  memset(parenRelations, 0, instructionCount);

  int parenStack[instructionCount];
  int pCount = 0;
  int tp = 0;

  while (tp < instructionCount) {
    if (instructions[tp] == '[') {
      pCount += 1;
      parenStack[pCount - 1] = tp;
    } else if (instructions[tp] == ']') {
      pCount -= 1;
      int matchTP = parenStack[pCount];
      parenRelations[matchTP] = tp;
      parenRelations[tp] = matchTP;
      parenStack[pCount] = 0;
    }
    tp += 1;
  }

  while (ip <= instructionCount) {
    uint8_t instruction = instructions[ip];
    switch (instruction) {
      case '>':
        if (dp < dataSize) dp += 1;
        debugPrint('>', data, dp, ip);
        break;
      case '<':
        if (dp > 0) dp -= 1;
        debugPrint('<', data, dp, ip);
        break;
      case '+':
        data[dp] += 1;
        debugPrint('+', data, dp, ip);
        break;
      case '-':
        data[dp] -= 1;
        debugPrint('-', data, dp, ip);
        break;
      case '.':
        debugPrint('.', data, dp, ip);
        putchar(data[dp]);
        break;
      case ',':
        debugPrint(',', data, dp, ip);
        scanf("%c", &data[dp]);
        break;
      case '[': {
        debugPrint('[', data, dp, ip);
        if (data[dp] == 0) {
          ip = parenRelations[ip];
        }
      } break;
      case ']': {
        debugPrint(']', data, dp, ip);
        if (data[dp] != 0) {
          ip = parenRelations[ip];
        }
      } break;
    }
    ip += 1;
  }

  free(instructions);
  free(data);

  printf("\n");
  return 0;
}
