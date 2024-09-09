#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t data_size = 30000;
size_t instruction_size = 200;
size_t instruction_count;
int *jumps;

char *instructions;
char *data;

int dp = 0;
int ip = 0;

static void debug_print(char instruction, char *data, int dp, int ip)
{
#ifdef DEBUG
	if (DEBUG) {
		printf("Instruction: %c; dp: %d; data[dp]: %d; ip: %d\n",
		       instruction, dp, data[dp], ip);
	}
#endif
}

static void build_jump_list(void)
{
	jumps = malloc(instruction_count);
	memset(jumps, -1, instruction_count);

	int jump_stack[instruction_count];
	int p_count = 0;

	for (int offset = 0; offset < instruction_size; offset++) {
		if (instructions[offset] == '[') {
			p_count += 1;
			jump_stack[p_count - 1] = offset;
		} else if (instructions[offset] == ']') {
			p_count -= 1;
			int match = jump_stack[p_count];
			jumps[offset] = match;
			jumps[match] = offset;
			jump_stack[p_count] = 0;
		}
	}
}

static void upsize(void **arr, size_t *curSize, size_t neededSize)
{
	while (neededSize >= *curSize) {
		void *tmp = realloc(*arr, 2 * *curSize * sizeof(*arr));
		if (tmp) {
			*arr = tmp;
			*curSize *= 2;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stdout, "usage: bcc filename.bf\n");
		return 1;
	}

	data = calloc(data_size, sizeof(char));
	instructions = calloc(instruction_size, sizeof(char));
	char c;

	FILE *file = fopen(argv[1], "r");

	if (file == NULL) {
		fprintf(stderr, "Couldn't open file: %s\n", argv[1]);
		return 1;
	}

	while ((c = (char)getc(file)) != EOF) {
		if (c != '[' && c != ']' && c != '>' && c != '<' && c != '+' &&
		    c != '-' && c != '.' && c != ',') {
			continue;
		}
		upsize((void **)&instructions, &instruction_size,
		       instruction_count);
		instructions[instruction_count++] = c;
	}

	fclose(file);

	build_jump_list();

	while (ip <= instruction_count) {
		uint8_t instruction = instructions[ip];
		switch (instruction) {
		case '>':
			if (dp < data_size)
				dp += 1;
			debug_print('>', data, dp, ip);
			break;
		case '<':
			if (dp > 0)
				dp -= 1;
			debug_print('<', data, dp, ip);
			break;
		case '+':
			data[dp] += 1;
			debug_print('+', data, dp, ip);
			break;
		case '-':
			data[dp] -= 1;
			debug_print('-', data, dp, ip);
			break;
		case '.':
			debug_print('.', data, dp, ip);
			putchar(data[dp]);
			break;
		case ',':
			debug_print(',', data, dp, ip);
			scanf("%c", &data[dp]);
			break;
		case '[': {
			debug_print('[', data, dp, ip);
			if (data[dp] == 0) {
				ip = jumps[ip];
			}
		} break;
		case ']': {
			debug_print(']', data, dp, ip);
			if (data[dp] != 0) {
				ip = jumps[ip];
			}
		} break;
		}
		ip += 1;
	}

	free(instructions);
	free(data);
	free(jumps);
	printf("\n");
	return 0;
}
