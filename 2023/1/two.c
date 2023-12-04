#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/*
--- Part Two ---

Your calculation isn't quite right. It looks like some of the digits are actually spelled out with letters: one, two, three, four, five, six, seven, eight, and nine also count as valid "digits".

Equipped with this new information, you now need to find the real first and last digit on each line. For example:

two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen
In this example, the calibration values are 29, 83, 13, 24, 42, 14, and 76. Adding these together produces 281.

What is the sum of all of the calibration values?

Your puzzle answer was 54277.
*/

typedef struct {
	const char *data;
	size_t len;
} Slice;

static Slice readFile(const char *path) {
	FILE* file = fopen(path, "rb");
	
	if (file == NULL) {
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		exit(74);
	}
	
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);
	
	char *buffer = (char*)malloc(fileSize + 1);
	if (buffer == NULL) {
		fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
		exit(74);
	}
	
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if (bytesRead < fileSize) {
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(74);
	}
	buffer[bytesRead] = '\0';
	
	Slice slice = {
		.data = buffer,
		.len = bytesRead
	};
	
	fclose(file);
	return slice;
}

bool match(const char *what, Slice input, size_t i) {
	size_t len = strlen(what);
	if (i + len >= input.len) {
		return false;
	}
	
	return memcmp(input.data + i, what, len) == 0;
}

int main(int argc, char *argv[]) {
	// need to parse digits 0-9 and as strings one, two, three, four, five, six, seven, eight, nine
	
	/*
	eight
	
	five
	four
	
	nine
	
	seven
	six
	
	three
	two
	
	one
	
	caveat: the last letter could be the beginning of another number: eightwo...
	*/
	
	if (argc < 2) {
		printf("missing input file.");
		return EXIT_FAILURE;
	}
	
	Slice input = readFile(argv[argc - 1]);
	
	int8_t first = -1;
	int8_t last = -1;
	uint32_t sum = 0;
	int line = 1;
	
	for (size_t i = 0; i < input.len; i++) {
		char c =  input.data[i];
		switch (c) {
			case 'e':
				if (match("ight", input, i + 1)) {
					i += 3; // since t can start a two
					c = '8';
				}
				break;
			
			case 'f':
				if (match("ive", input, i + 1)) {
					i += 2; // 8
					c = '5';
				}
				if (match("our", input, i + 1)) {
					i += 3;
					c = '4';
				}
				break;
			
			case 'n':
				if (match("ine", input, i + 1)) {
					i += 2; // e can start eight
					c = '9';
				}
				break;
			
			case 's':
				if (match("even", input, i + 1)) {
					i += 3; // 9
					c = '7';
				}
				if (match("ix", input, i + 1)) {
					i += 2;
					c = '6';
				}
				break;
			
			case 't':
				if (match("hree", input, i + 1)) {
					i += 4;
					c = '3';
				}
				if (match("wo", input, i + 1)) {
					i += 1; // 1
					c = '2';
				}
				break;
			
			case 'o':
				if (match("ne", input, i + 1)) {
					i += 1; // 8
					c = '1';
				}
				break;
			
			case '\n': {
				assert(first > 0 && last > 0);
				int val = first * 10 + last;
				sum += val;
				first = -1;
				last = -1;
				
				printf("%d: %d\n", line++, val);
//				printf("%d\n", val);
				continue;
			}	
				break;
		}
		
		if (isdigit(c)) {
			last = c - '0';
			if (first < 0) {
				first = last;
			}
		}
	}
	free((void *)(input.data));
	
	printf("Sum: %d\n", sum);
	
	return EXIT_SUCCESS;
}