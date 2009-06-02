#import "MainViewController.h"
#import "MainView.h"

@implementation MainViewController


@synthesize mainView = _mainView;


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
   
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
       
        // Custom initialization
    }
   
    return self;
}


- (void)dealloc {

   [_mainView release];
   [super dealloc];
}


- (void)viewDidLoad {

   [super viewDidLoad];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
   
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
   
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


@end
