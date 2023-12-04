#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

/*
AoC 2023 day 4
*/

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("missing input file.");
		return EXIT_FAILURE;
	}
	
	FILE *input = fopen(argv[argc - 1], "r");
	
	int total = 0;
	int8_t c = 0;
	
	
	int extracopies[200] = {0};
	int cardid = 0;
	while ((c = fgetc(input)) != EOF) {
		bool winning[100] = {0};
		// header
		while ((c = fgetc(input)) != ':') {}
		c = fgetc(input); // :

		// winning numbers
		while ((c = fgetc(input)) != '|') {
			if (c == ' ') {
				while ((c = fgetc(input)) == ' ') {}
				if (c == '|') {
					break;
				}
			}
			
			int8_t c2 = fgetc(input);
			int val = 0;
			if (isnumber(c2)) {
				val = (c - '0') * 10 + (c2 - '0');
				
			} else {
				val = (c - '0');
			}
			winning[val] = true;
		}
		
		// have
		int cardval = 0;
		int winningcount = 0;
		while ((c = fgetc(input)) != '\n') {
			if (c == ' ') {
				while ((c = fgetc(input)) == ' ') {}
				if (c == '\n') {
					break;
				}
			}
			
			int8_t c2 = fgetc(input);
			int val = 0;
			if (isnumber(c2)) {
				val = (c - '0') * 10 + (c2 - '0');
				
			} else {
				val = (c - '0');
			}
			
			if (winning[val]) {
				winningcount++;
				if (!cardval) {
					cardval = 1;
				} else {
					cardval *= 2;
				}
			}
			
			if (c2 == '\n') {
				break;
			}
		}
				
		for (int i = 0; i < winningcount; i++) {
			extracopies[cardid + 1 + i]++;
		}
		
		for (int i = 0; i < extracopies[cardid]; i++) {
			for (int i = 0; i < winningcount; i++) {
				extracopies[cardid + 1 + i]++;
			}
		}
		
		total += cardval;
		cardid++;
	}
	
	int totalcards = cardid;
	for (int i = 0; i < cardid; i++) {
		totalcards += extracopies[i];
	}
	
	printf("1:%d, 2:%d", total, totalcards);
	
	fclose(input);
	return EXIT_SUCCESS;
}