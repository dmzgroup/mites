#import "dmzMitesModuleiPhone.h"
#import "ControlsViewController.h"
#import "RootViewController.h"
#import "MainViewController.h"
#import "MainView.h"
#import "FlipsideViewController.h"

#define kAccelerometerFrequency      25 //Hz
#define kFilteringFactor             0.1
#define kMinEraseInterval            0.5
#define kEraseAccelerationThreshold  2.0


@implementation RootViewController

@synthesize infoButton;
@synthesize flipsideNavigationBar;
@synthesize mainViewController;
@synthesize flipsideViewController;
@synthesize controlsViewController;


- (void)loadMainViewController {
   
   MainViewController *viewController = [[MainViewController alloc] initWithNibName:@"MainView" bundle:nil];
   self.mainViewController = viewController;
   [viewController release];
}


- (void)loadFlipsideViewController {
   
   FlipsideViewController *viewController = [[FlipsideViewController alloc] initWithNibName:@"FlipsideView" bundle:nil];
   self.flipsideViewController = viewController;
   [viewController release];
   
   // Set up the navigation bar
   UINavigationBar *aNavigationBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0.0, 0.0, 320.0, 44.0)];
   aNavigationBar.barStyle = UIBarStyleBlackOpaque;
   self.flipsideNavigationBar = aNavigationBar;
   [aNavigationBar release];
   
   UIBarButtonItem *buttonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(toggleView)];
   UINavigationItem *navigationItem = [[UINavigationItem alloc] initWithTitle:@"DMZ: Mites"];
   navigationItem.rightBarButtonItem = buttonItem;
   [flipsideNavigationBar pushNavigationItem:navigationItem animated:NO];
   [navigationItem release];
   [buttonItem release];
}


- (void)loadControlsViewController {
   
   ControlsViewController *controlsController = [[ControlsViewController alloc] initWithNibName:@"ControlsView" bundle:nil];
   self.controlsViewController = controlsController;
   [controlsController release];
}


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
   
   if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
      
      // Custom initialization
   }
   
   return self;
}


- (void)viewDidLoad {
   
   [self loadMainViewController];
   [self.view insertSubview:mainViewController.view belowSubview:infoButton];

   [self.mainViewController.mainView setControlsDelegate:self];
   
   //Configure and enable the accelerometer
   [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
   [[UIAccelerometer sharedAccelerometer] setDelegate:self];
}


- (void)dealloc {
   
   [infoButton release];
   [flipsideNavigationBar release];
   [mainViewController release];
   [flipsideViewController release];
   [controlsViewController release];
   [super dealloc];
}


- (void)toggleControls {

   CGRect appFrame = [[UIScreen mainScreen] applicationFrame];
   
   if (controlsViewController == nil) {
    
      [self loadControlsViewController];
      
      CGRect frame = controlsViewController.view.frame;
      frame = CGRectMake (frame.origin.x, appFrame.size.height, frame.size.width, frame.size.height);
      controlsViewController.view.frame = CGRectMake (0, appFrame.size.height, frame.size.width, frame.size.height);
      
      [self.mainViewController.view addSubview:controlsViewController.view];
   }
   
   [UIView beginAnimations:nil context:NULL];
   [UIView setAnimationDuration:1];

   CGRect frame = controlsViewController.view.frame;
   
   int offset = frame.size.height;
   
   if (frame.origin.y >= appFrame.size.height) {
      
      dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
      
      if (mod) {
         
         [infoButton removeFromSuperview];

         [controlsViewController.mitesSlider setValue:mod->get_mites ()];
         [controlsViewController.chipsSlider setValue:mod->get_chips ()];
         [controlsViewController.speedSlider setValue:mod->get_speed ()];
         [controlsViewController.waitSlider setValue:mod->get_wait ()];
         [controlsViewController updateLabels];
      }
      
      offset = -offset;
   }
   else {
      
      [self.view insertSubview:infoButton aboveSubview:mainViewController.view];
   }
   
   controlsViewController.view.frame = CGRectOffset (frame, 0, offset);
      
   [UIView commitAnimations];
}


- (IBAction)toggleView {    
   
    if (flipsideViewController == nil) {
       
       [self loadFlipsideViewController];
    }
    
    UIView *mainView = mainViewController.view;
    UIView *flipsideView = flipsideViewController.view;

    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:1];
    [UIView setAnimationTransition:([mainView superview] ? UIViewAnimationTransitionFlipFromRight : UIViewAnimationTransitionFlipFromLeft) forView:self.view cache:YES];
    
    if ([mainView superview] != nil) {

       dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
       
       if (mod) {
          
          [flipsideViewController.mitesSlider setValue:mod->get_mites ()];
          [flipsideViewController.chipsSlider setValue:mod->get_chips ()];
          [flipsideViewController.speedSlider setValue:mod->get_speed ()];
          [flipsideViewController.waitSlider setValue:mod->get_wait ()];
          
          [flipsideViewController updateLabels];
       }
       
      [flipsideViewController viewWillAppear:YES];
      [mainViewController viewWillDisappear:YES];
      [mainView removeFromSuperview];
      [infoButton removeFromSuperview];
      [self.view addSubview:flipsideView];
      [self.view insertSubview:flipsideNavigationBar aboveSubview:flipsideView];
      [mainViewController viewDidDisappear:YES];
      [flipsideViewController viewDidAppear:YES];
    }
    else {
      
      dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
       
       if (mod) {
        
          mod->set_mites (dmz::Int64 ([flipsideViewController.mitesSlider value] + 0.5f));
          mod->set_chips (dmz::Int64 ([flipsideViewController.chipsSlider value] + 0.5f));
          mod->set_speed ([flipsideViewController.speedSlider value] + 0.5f);
          mod->set_wait ([flipsideViewController.waitSlider value] + 0.5f);
       }
      
      [mainViewController viewWillAppear:YES];
      [flipsideViewController viewWillDisappear:YES];
      [flipsideView removeFromSuperview];
      [flipsideNavigationBar removeFromSuperview];
      [self.view addSubview:mainView];
      [self.view insertSubview:infoButton aboveSubview:mainViewController.view];
      [flipsideViewController viewDidDisappear:YES];
      [mainViewController viewDidAppear:YES];
    }
   
    [UIView commitAnimations];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
   
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
   
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


// Called when the accelerometer detects motion; plays the erase sound and redraws the view if the motion is over a threshold.
- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration {
   
   UIAccelerationValue length, x, y, z;
   
   //Use a basic high-pass filter to remove the influence of the gravity
   myAccelerometer[0] = acceleration.x * kFilteringFactor + myAccelerometer[0] * (1.0 - kFilteringFactor);
   myAccelerometer[1] = acceleration.y * kFilteringFactor + myAccelerometer[1] * (1.0 - kFilteringFactor);
   myAccelerometer[2] = acceleration.z * kFilteringFactor + myAccelerometer[2] * (1.0 - kFilteringFactor);
   
   // Compute values for the three axes of the acceleromater
   x = acceleration.x - myAccelerometer[0];
   y = acceleration.y - myAccelerometer[0];
   z = acceleration.z - myAccelerometer[0];
   
   //Compute the intensity of the current acceleration 
   length = sqrt(x * x + y * y + z * z);
   
   // If above a given threshold, play the erase sounds and erase the drawing view
   if((length >= kEraseAccelerationThreshold) && (CFAbsoluteTimeGetCurrent() > lastTime + kMinEraseInterval)) {
      
      dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
      if (mod) { mod->reset_lua (); }  
      
      lastTime = CFAbsoluteTimeGetCurrent();
   }
}


@end
