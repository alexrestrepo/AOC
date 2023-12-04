#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/*
--- Day 2: Cube Conundrum ---

You're launched high into the atmosphere! The apex of your trajectory just barely reaches the surface of a large island floating in the sky. You gently land in a fluffy pile of leaves. It's quite cold, but you don't see much snow. An Elf runs over to greet you.

The Elf explains that you've arrived at Snow Island and apologizes for the lack of snow. He'll be happy to explain the situation, but it's a bit of a walk, so you have some time. They don't get many visitors up here; would you like to play a game in the meantime?

As you walk, the Elf shows you a small bag and some cubes which are either red, green, or blue. Each time you play this game, he will hide a secret number of cubes of each color in the bag, and your goal is to figure out information about the number of cubes.

To get information, once a bag has been loaded with cubes, the Elf will reach into the bag, grab a handful of random cubes, show them to you, and then put them back in the bag. He'll do this a few times per game.

You play several games and record the information from each game (your puzzle input). Each game is listed with its ID number (like the 11 in Game 11: ...) followed by a semicolon-separated list of subsets of cubes that were revealed from the bag (like 3 red, 5 green, 4 blue).

For example, the record of a few games might look like this:

Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
In game 1, three sets of cubes are revealed from the bag (and then put back again). The first set is 3 blue cubes and 4 red cubes; the second set is 1 red cube, 2 green cubes, and 6 blue cubes; the third set is only 2 green cubes.

The Elf would first like to know which games would have been possible if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?

In the example above, games 1, 2, and 5 would have been possible if the bag had been loaded with that configuration. However, game 3 would have been impossible because at one point the Elf showed you 20 red cubes at once; similarly, game 4 would also have been impossible because the Elf showed you 15 blue cubes at once. If you add up the IDs of the games that would have been possible, you get 8.

Determine which games would have been possible if the bag had been loaded with only 12 red cubes, 13 green cubes, and 14 blue cubes. What is the sum of the IDs of those games?

Your puzzle answer was 2528.
*/

typedef enum {
	TokenValue,
	TokenGame,	
	TokenRed,
	TokenGreen,
	TokenBlue,
	TokenComma,
	TokenSemi,
	TokenEOL,
	TokenEOF
} TokenType;

const char *TypeNames[] = {
	"#", "Game", "red", "green", "blue", ",", ";", "EOF"
};

typedef struct {
	TokenType type;
	int value;
} Token;

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

static bool isalpha(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static Token nextToken(Slice *input, size_t *pos) {	
	char c = input->data[*pos];
	(*pos)++;
	
	// skip white;
	while ((c == ' ' || c == ':') && (*pos) < input->len) {
		c = input->data[*pos];
		(*pos)++;
	}
	
	if (*pos >= input->len) {
		return (Token) {TokenEOF};
	}
	
	if (c == '\n') {
		return (Token) { TokenEOL };
	}
	
	if (c == ',') {
		return (Token) {TokenComma};
	}
	
	if (c == ';') {
		return (Token) {TokenSemi};
	}
	
	if (isalpha(c)) {
		Token t = {0};
		
		switch (c) {
			case 'G':
			case 'g':
				c = input->data[*pos];
				(*pos)++;
				if (c == 'r') {
					t.type = TokenGreen;
					
				} else {
					t.type = TokenGame;
				}
				break;
			
			case 'r':
				t.type = TokenRed;
				break;
			
			case 'b':
				t.type = TokenBlue;
				break;
		}
		
		while (isalpha(input->data[*pos])) {
			(*pos)++;
		}

		return t;
	}
	
	assert(c >= '0' && c <= '9');
	char *end = (char *)&input->data[*pos];
	int val = (int)strtod(input->data + (*pos) - 1, &end);
	(*pos) += end - ((char *)&input->data[*pos]);
	
	return (Token) {
		TokenValue,
		val
	};
	
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("missing input file.");
		return EXIT_FAILURE;
	}
	
	Slice input = readFile(argv[argc - 1]);
	size_t pos = 0;
	Token t = {0};
	
	/*
	Determine which games would have been possible if the bag had been loaded with only 
	12 red cubes, 13 green cubes, and 14 blue cubes. 
	What is the sum of the IDs of those games?
	*/
	
	int sum = 0;
	while ((t = nextToken(&input, &pos)).type != TokenEOF) {
		assert(t.type == TokenGame);
		
		t = nextToken(&input, &pos);
		assert(t.type == TokenValue);
			
		int gameid = t.value;
		
		int r = 0, g = 0, b = 0;
		while ((t = nextToken(&input, &pos)).type != TokenEOL) {
			if (t.type == TokenComma || t.type == TokenSemi) {
				continue;
			}
			
			if (t.type == TokenEOF) {
				break;
			}
			
			assert(t.type == TokenValue);
			int v = t.value;
			
			t = nextToken(&input, &pos);
			switch (t.type) {
				case TokenRed:
					if (v > r) r = v;
					break;
				
				case TokenGreen:
					if (v > g) g = v;
					break;
				
				case TokenBlue:
					if (v > b) b = v;
					break;
				
				default:
					assert(false);
			}
		}
		
		if (gameid ==99) {
			printf("");
		}
		
		if (r <= 12 && g <= 13 && b <= 14) {
			sum += gameid;
		}
	}
	printf("%d", sum);
	free((void *)input.data);
	
	return EXIT_SUCCESS;
}