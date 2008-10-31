#import "MainView.h"
#import "Canvas.h"

@implementation MainView

@synthesize canvas = _canvas;


- (void) awakeFromNib {
   
   _canvas = [[Canvas alloc] initWithFrame:CGRectMake (0, 0, 3200, 4600)];
   
   self.contentSize = _canvas.bounds.size;
   self.minimumZoomScale = 0.1;
   self.maximumZoomScale = 1.0;
   self.delegate = self;
   
   [self addSubview:_canvas];
   
   UIImage *miteImage = [UIImage imageNamed:@"mite.png"];
   UIImageView *item = [[UIImageView alloc] initWithImage:miteImage];
   [_canvas addSubview:item];
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


@end
