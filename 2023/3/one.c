#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "../common/stb_ds_x.h"

/*
--- Day 3: Gear Ratios ---

You and the Elf eventually reach a gondola lift station; he says the gondola lift will take you up to the water source, but this is as far as he can bring you. You go inside.

It doesn't take long to find the gondolas, but there seems to be a problem: they're not moving.

"Aaah!"

You turn around to see a slightly-greasy Elf with a wrench and a look of surprise. "Sorry, I wasn't expecting anyone! The gondola lift isn't working right now; it'll still be a while before I can fix it." You offer to help.

The engineer explains that an engine part seems to be missing from the engine, but nobody can figure out which one. If you can add up all the part numbers in the engine schematic, it should be easy to work out which part is missing.

The engine schematic (your puzzle input) consists of a visual representation of the engine. There are lots of numbers and symbols you don't really understand, but apparently any number adjacent to a symbol, even diagonally, is a "part number" and should be included in your sum. (Periods (.) do not count as a symbol.)

Here is an example engine schematic:

467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
In this schematic, two numbers are not part numbers because they are not adjacent to a symbol: 114 (top right) and 58 (middle right). Every other number is adjacent to a symbol and so is a part number; their sum is 4361.

Of course, the actual engine schematic is much larger. What is the sum of all of the part numbers in the engine schematic?

Your puzzle answer was 529618.

--- Part Two ---

The engineer finds the missing part and installs it in the engine! As the engine springs to life, you jump in the closest gondola, finally ready to ascend to the water source.

You don't seem to be going very fast, though. Maybe something is still wrong? Fortunately, the gondola has a phone labeled "help", so you pick it up and the engineer answers.

Before you can explain the situation, she suggests that you look out the window. There stands the engineer, holding a phone in one hand and waving with the other. You're going so slowly that you haven't even left the station. You exit the gondola.

The missing part wasn't the only issue - one of the gears in the engine is wrong. A gear is any * symbol that is adjacent to exactly two part numbers. Its gear ratio is the result of multiplying those two numbers together.

This time, you need to find the gear ratio of every gear and add them all up so that the engineer can figure out which gear needs to be replaced.

Consider the same engine schematic again:

467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
In this schematic, there are two gears. The first is in the top left; it has part numbers 467 and 35, so its gear ratio is 16345. The second gear is in the lower right; its gear ratio is 451490. (The * adjacent to 617 is not a gear because it is only adjacent to one part number.) Adding up all of the gear ratios produces 467835.

What is the sum of all of the gear ratios in your engine schematic?

Your puzzle answer was 77509019.
*/

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
		ValTypeGear,
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
			
			type = c == '.' ? ValTypePeriod : c == '*' ? ValTypeGear : ValTypeSymbol;
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
	int ratioSum = 0;
	for (int i = 0; i < arrlen(cells); i++) {
		// find x,y
		int x = i % stride;
		int y = i / stride;
		
		Cell c = cells[i];
		typedef struct {
			int key;
			int value;
		} Ratio;
		
		Ratio *ratios = NULL;
		
		if (c.type == ValTypeSymbol || c.type == ValTypeGear) {			
			if (x - 1 >= 0 && y - 1 >= 0) {
				int idx = (y - 1) * stride + (x - 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (y - 1 >= 0) {
				int idx = (y - 1) * stride + (x);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (x + 1 < stride && y - 1 >= 0) {
				int idx = (y - 1) * stride + (x + 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (x + 1 < stride) {
				int idx = (y) * stride + (x + 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (x + 1 < stride && y + 1 < stride) {
				int idx = (y + 1) * stride + (x + 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (y + 1 < stride) {
				int idx = (y + 1) * stride + (x);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (x - 1 >= 0 && y + 1 < stride) {
				int idx = (y + 1) * stride + (x - 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (x - 1 >= 0) {
				int idx = (y) * stride + (x - 1);
				Cell cell = cells[idx];
				if (cell.type == ValTypeNum) {
					values[cell.index].isPart = true;
					hmput(ratios, cell.index, values[cell.index].val);
				}
			}
			
			if (c.type == ValTypeGear) {
				if (hmlen(ratios) == 2) {
					ratioSum += ratios[0].value * ratios[1].value;
				}
			}
			
			hmfree(ratios);
		}
	}
	
	int sum = 0;
	for (int i = 0; i < arrlen(values); i++) {
		Value v = values[i];
		if (v.isPart) {
			sum += v.val;
		}
	}
	
	arrfree(values);
	arrfree(cells);
	
	printf("sum: %d\n", sum);
	printf("ratios: %d", ratioSum);
	return 0;
}