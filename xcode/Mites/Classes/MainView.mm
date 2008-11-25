#import "MainView.h"
#import "Canvas.h"

@implementation MainView

@synthesize canvas = _canvas;
@synthesize controlsDelegate;


- (void) awakeFromNib {
   
   _canvas = [[Canvas alloc] initWithFrame:CGRectMake (0, 0, 3200, 4600)];
   
   self.contentSize = _canvas.bounds.size;
   self.minimumZoomScale = 0.1;
   self.maximumZoomScale = 1.0;
   self.delegate = self;
   
   [self addSubview:_canvas];
}


- (void)dealloc {
   
   [_canvas release];
   [super dealloc];
}


- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView {
   
   return _canvas;
} 


- (void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(UIView *)view atScale:(float)scale {
   
}

/*
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
   
   //   UITouch *touch = [touches anyObject];
   
//   if (controlsDelegate != nil) {
//      
//      [controlsDelegate toggleControls:self];
//   }
   
   [super touchesBegan:touches withEvent:event];
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
   
   [super touchesMoved:touches withEvent:event];
}
*/

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {

   UITouch *touch = [touches anyObject];
   
   if ([touch tapCount] == 2) {
      
      [controlsDelegate toggleControls];
   }
   else {
      
      [super touchesEnded:touches withEvent:event];
   }
}

/*
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {

   [super touchesCancelled:touches withEvent:event];
}
*/

@end
