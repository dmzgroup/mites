#import <UIKit/UIKit.h>

@class Canvas;


@interface MainView : UIScrollView <UIScrollViewDelegate> {

   Canvas *_canvas;
}

@property (nonatomic, retain) Canvas *canvas;

@end
