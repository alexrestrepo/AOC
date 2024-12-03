#import <Foundation/Foundation.h>

int main(int argc, char *argv[]) {
	@autoreleasepool {
		NSString *input = [NSString stringWithContentsOfFile:@"input.txt"];
		NSMutableArray *left = [NSMutableArray new];
		NSMutableArray *right = [NSMutableArray new];
		
		NSScanner *scanner = [NSScanner scannerWithString:input];
		scanner.charactersToBeSkipped = [NSCharacterSet whitespaceAndNewlineCharacterSet];
		
		while (!scanner.atEnd) {
			int value = 0;
			[scanner scanInt:&value];
			[left addObject:@(value)];
			
			[scanner scanInt:&value];
			[right addObject:@(value)];
		}
		
		NSCAssert(left.count == right.count, @"missmatched values");
		[left sortUsingSelector:@selector(compare:)];
		[right sortUsingSelector:@selector(compare:)];
		
		NSInteger sum = 0;
		for (int i = 0; i < left.count; i++) {
			int lv = [left[i] intValue];
			int rv = [right[i] intValue];
			sum += ABS(lv - rv);
		}
		
		printf("Sum: %ld", (long)sum);
	}
}