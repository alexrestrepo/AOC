#import <Foundation/Foundation.h>

int main(int argc, char *argv[]) {
	@autoreleasepool {
		NSString *input = [NSString stringWithContentsOfFile:@"input.txt"];
		NSMutableArray *left = [NSMutableArray new];
		NSMutableDictionary *right = [NSMutableDictionary new];
		
		NSScanner *scanner = [NSScanner scannerWithString:input];
		scanner.charactersToBeSkipped = [NSCharacterSet whitespaceAndNewlineCharacterSet];
		
		while (!scanner.atEnd) {
			int value = 0;
			[scanner scanInt:&value];
			[left addObject:@(value)];
			
			[scanner scanInt:&value];
			
			NSNumber *count = right[@(value)];
			if (!count) {
				count = @(0);
			}
			count = @([count intValue] + 1);
			right[@(value)] = count;
		}
		
		NSInteger score = 0;
		for (int i = 0; i < left.count; i++) {
			int value = [left[i] intValue];
			NSNumber *similarity = right[@(value)];
			
			score += value * (similarity ? [similarity intValue] : 0);
		}
		
		printf("Sum: %ld", (long)score);
	}
}