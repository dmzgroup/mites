#import <UIKit/UIKit.h>

@class MainView;


@interface MainViewController : UIViewController {

   IBOutlet MainView *_mainView;
}


@property (nonatomic, retain) MainView *mainView;


@end
