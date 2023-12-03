#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

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