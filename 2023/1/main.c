#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define STB_DS_IMPLEMENTATION
#include "../common/stb_ds_x.h"

typedef struct {
	uint8_t first;
	uint8_t last;
	
} CalibrationValue;

int main(int argc, char *argv[]) {
	if (argc < 1) {
		printf("missing input file.");
		return EXIT_FAILURE;
	}
	
	uint32_t sum = 0;
	int8_t first = -1;
	int8_t last = -1;
	
	FILE *file = fopen(argv[argc - 1], "r");
	while (true) {
		int8_t c = fgetc(file);
		if (c == EOF) {
			break;
		}
		
		if (isdigit(c)) {
			last = c - '0';
			if (first < 0) {
				first = last;
			}
		}
		
		if (c == '\n') {
			sum += first * 10 + last;
			first = -1;
			last = -1;
		}
	}
	fclose(file);
	
	printf("Sum: %d\n", sum);
	return EXIT_SUCCESS;
}