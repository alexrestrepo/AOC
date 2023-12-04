#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "../common/stb_ds_x.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("missing input file.");
		return EXIT_FAILURE;
	}
	
	typedef struct {
		int val;
		bool isPart;
	} Value;
	
	typedef enum {
		ValTypeNum,
		ValTypeSymbol,
		ValTypePeriod
	} ValType;
	
	typedef struct {
		ValType type;
		int index;
	} Cell;
	
	char *current = NULL;
	int currentIndex = -1;
	
	Cell *cells = NULL;
	Value *values = NULL;
	
	FILE *input = fopen(argv[argc - 1], "r");
	int stride = 0;
	int pos = 0;
	while (true) {
		int8_t c = fgetc(input);
		
		if (c == EOF) {
			break;
		}
		
		if (c == '\n') {
			if (stride != 0) {
				assert(stride == pos);
			}
			stride = pos;
			pos = 0;
			continue;
		}
		
		pos++;
		
		ValType type = ValTypePeriod;
		
		if (isnumber(c)) {
			type = ValTypeNum;
			if (currentIndex < 0) {
				currentIndex = arrlen(values);
				arrpush(values, (Value){0});
			}
			
			arrpush(current, c);
			
		} else {
			if (arrlen(current)) {
				assert(currentIndex >= 0);
				arrpush(current, '\0');
				
				int v = strtod(current, NULL);
				values[currentIndex].val = v;
			}
			
			arrfree(current);
			current = NULL;
			currentIndex = -1;
			
			type = c == '.' ? ValTypePeriod : ValTypeSymbol;
		}
		
		Cell cell = {
			.type = type,
			.index = currentIndex
		};
		arrpush(cells, cell);
	}
	if (arrlen(current)) {
		assert(currentIndex >= 0);
		arrpush(current, '\0');
		
		int v = strtod(current, NULL);
		values[currentIndex].val = v;
	}
	fclose(input);
	
	printf("size %d x %ld\n", stride, arrlen(cells) / stride);
	
	// mark all "parts"
	for (int i = 0; i < arrlen(cells); i++) {
		// find x,y
		int x = i % stride;
		int y = i / stride;
		
		Cell c = cells[i];
		if (c.type == ValTypeSymbol) {
			if (x - 1 >= 0 && y - 1 >= 0) {
				int idx = (y - 1) * stride + (x - 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (y - 1 >= 0) {
				int idx = (y - 1) * stride + (x);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (x + 1 < stride && y - 1 >= 0) {
				int idx = (y - 1) * stride + (x + 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (x + 1 < stride) {
				int idx = (y) * stride + (x + 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (x + 1 < stride && y + 1 < stride) {
				int idx = (y + 1) * stride + (x + 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (y + 1 < stride) {
				int idx = (y + 1) * stride + (x);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (x - 1 >= 0 && y + 1 < stride) {
				int idx = (y + 1) * stride + (x - 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
			
			if (x - 1 >= 0) {
				int idx = (y) * stride + (x - 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
				}
			}
		}
	}
	
	int sum = 0;
	for (int i = 0; i < arrlen(values); i++) {
		Value v = values[i];
		if (v.isPart) {
			sum += v.val;
		}
	}
	
	printf("%d", sum);
	return 0;
}