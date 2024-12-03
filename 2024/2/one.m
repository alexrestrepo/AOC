#import <Foundation/Foundation.h>

int main(int argc, char *argv[]) {
	@autoreleasepool {
		NSString *input = [NSString stringWithContentsOfFile:@"input.txt"];
		NSScanner *scanner = [NSScanner scannerWithString:input];
		scanner.charactersToBeSkipped = [NSCharacterSet whitespaceCharacterSet];
		
		int safe_count = 0;
		int previous_val = -1;
		enum {
			UNKNOWN,
			UNSAFE,
			SAFE,
		} row_safe = UNKNOWN; // 0 unknown, 1 no, 2 yes
		int direction = 0;
		while (!scanner.atEnd) {
			int value = 0;
			
			if ([scanner scanInt:&value]) {
				if (previous_val >= 0) {
					int diff = value - previous_val;
					if (diff == 0) {
						row_safe = UNSAFE;
						continue;
					}
					
					int dir = diff / ABS(diff);
					
					if (direction == 0) {
						direction = dir;
					}
					
					if (direction != dir) {
						row_safe = UNSAFE;
						continue;
					}
					
					if (ABS(diff) < 1 || ABS(diff) > 3) {
						row_safe = UNSAFE;
						continue;
					}
					
					if (row_safe != UNSAFE) {
						row_safe = SAFE;
					}
				}
				previous_val = value;
				
			} else {
				[scanner scanCharactersFromSet:[NSCharacterSet newlineCharacterSet] intoString:nil];
				if (row_safe == 2) {
					safe_count++;
				}
				direction = 0;
				row_safe = UNKNOWN;
				previous_val = -1;
			}
		}
		printf("safe count: %d", safe_count);
	}
}