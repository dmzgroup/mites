#import "Canvas.h"


@implementation Canvas

- (id)initWithFrame:(CGRect)frame {
   
	if (self = [super initWithFrame:frame]) {
      
      self.backgroundColor = [UIColor lightGrayColor];
	}
   
	return self;
}


- (void)dealloc {
   
	[super dealloc];
}


- (void)drawRect:(CGRect)rect {

   CGContextRef context = UIGraphicsGetCurrentContext ();
   CGContextSaveGState(context);
   
   CGContextBeginPath (context);
   CGContextSetRGBStrokeColor (context, 0.8, 0.8, 0.8, 1.0);
   
   int x = 0;
   
   for (x = 0; x < self.bounds.size.width; x += 100) {
      
      CGContextMoveToPoint (context, x, 0);
      CGContextAddLineToPoint (context, x, self.bounds.size.height - 1);
   }
   
   int y = 0;
   
   for (y = 0; y < self.bounds.size.height; y += 100) {
      
      CGContextMoveToPoint (context, 0, y);
      CGContextAddLineToPoint (context, self.bounds.size.width - 1, y);
   }
   
   CGContextStrokePath (context);      
   
   CGContextRestoreGState (context);
}


@end
