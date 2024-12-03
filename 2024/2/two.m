#import <Foundation/Foundation.h>

BOOL checkLine(NSArray *values, int skip) {
    NSMutableArray *copy = [values mutableCopy];
    if (skip >= 0) {
        [copy removeObjectAtIndex:skip];
    }
    
    int direction = 0;
    for (int i = 1; i < copy.count; i++) {
        int prev = [copy[i - 1] intValue];
        int curr = [copy[i] intValue];
        
        int diff = curr - prev;
        int dir = diff / (diff != 0 ? ABS(diff) : 1);
        if (direction == 0) {
            direction = dir;
        }
        
        if (diff == 0 || direction != dir || ABS(diff) < 1 || ABS(diff) > 3) {
            return NO;
        }
    }
    return YES;
}

int main(int argc, char *argv[]) {
	@autoreleasepool {
		NSString *input = [NSString stringWithContentsOfFile:@"input.txt"];
		NSScanner *scanner = [NSScanner scannerWithString:input];
		scanner.charactersToBeSkipped = [NSCharacterSet whitespaceCharacterSet];
		
		int safe_count = 0;
        NSMutableArray *values = [NSMutableArray new];
		while (!scanner.atEnd) {
			int value = 0;
			
			if ([scanner scanInt:&value]) {
				[values addObject:@(value)];
                
			} else {
				[scanner scanCharactersFromSet:[NSCharacterSet newlineCharacterSet] intoString:nil];
                BOOL safe = NO;
                for (int i = 0; i < values.count; i++) {
                    if (checkLine(values, i)) {
                        safe = YES;
                    }
                }
                safe_count += safe ? 1 : 0;
				[values removeAllObjects];
			}
		}
		printf("safe count: %d", safe_count);
	}
}