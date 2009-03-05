#import "RootViewController.h"
#import <UIKit/UIKit.h>

@class Canvas;


@interface MainView : UIScrollView <UIScrollViewDelegate> {

   Canvas *_canvas;
   id controlsDelegate;
}

@property (nonatomic, assign) id <ControlsViewDelegate> controlsDelegate;
@property (nonatomic, retain) Canvas *canvas;

@end
