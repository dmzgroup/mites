#import <UIKit/UIKit.h>

@class MainViewController;
@class FlipsideViewController;
@class ControlsViewController;

@protocol ControlsViewDelegate <NSObject>
@required
- (void)toggleControls;
@end

@interface RootViewController : UIViewController <UIAccelerometerDelegate, ControlsViewDelegate> {

   UIButton *infoButton;
   MainViewController *mainViewController;
   FlipsideViewController *flipsideViewController;
   ControlsViewController *controlsViewController;
   UINavigationBar *flipsideNavigationBar;
   UIAccelerationValue myAccelerometer[3];
   CFTimeInterval lastTime;
}

@property (nonatomic, retain) IBOutlet UIButton *infoButton;
@property (nonatomic, retain) MainViewController *mainViewController;
@property (nonatomic, retain) UINavigationBar *flipsideNavigationBar;
@property (nonatomic, retain) FlipsideViewController *flipsideViewController;
@property (nonatomic, retain) ControlsViewController *controlsViewController;

- (IBAction)toggleView;

- (void)toggleControls;


@end
