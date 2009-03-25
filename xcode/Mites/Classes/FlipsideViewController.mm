#import "dmzMitesModuleiPhone.h"
#import "FlipsideViewController.h"
#import "dmzUIKitUtil.h"


@implementation FlipsideViewController

@synthesize mitesLabel, chipsLabel, speedLabel, maxTurnLabel, turnDelayLabel, huntDelayLabel;
@synthesize mitesSlider, chipsSlider, speedSlider, maxTurnSlider, turnDelaySlider, huntDelaySlider;
@synthesize stopButton, resetButton;
@synthesize resetRequired;


- (void)viewDidLoad {
   
   [super viewDidLoad];
   self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor]; 
   self.view.contentMode = UIViewContentModeScaleToFill;
   
   UIKitImproveSliderAccuracy (mitesSlider);
   UIKitImproveSliderAccuracy (chipsSlider);
   UIKitImproveSliderAccuracy (speedSlider);
   UIKitImproveSliderAccuracy (maxTurnSlider);
   UIKitImproveSliderAccuracy (turnDelaySlider);
   UIKitImproveSliderAccuracy (huntDelaySlider);
   
   [stopButton removeFromSuperview];
//   [resetButton removeFromSuperview];
}


- (void)dealloc {

   [mitesLabel release];
   [chipsLabel release];
   [speedLabel release];
   [maxTurnLabel release];
   [turnDelayLabel release];
   [huntDelayLabel release];
   
   [mitesSlider release];
   [chipsSlider release];
   [speedSlider release];
   [maxTurnSlider release];
   [turnDelaySlider release];
   [huntDelaySlider release];
   
   [stopButton release];
   [resetButton release];
   
   [super dealloc];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
   
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
   
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (IBAction)sliderValueChanged:(id)sender {

   [self updateLabels];
}


- (IBAction)stopButtonPressed {
   
//   dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
//   if (mod) { mod->reset_lua (); }   
}


- (IBAction)resetButtonPressed {

   dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
   if (mod) { mod->reset_lua (); }   
}


- (void)updateLabels {
   
   mitesLabel.text = [NSString stringWithFormat:@"%.0f", [mitesSlider value]];
   chipsLabel.text = [NSString stringWithFormat:@"%.0f", [chipsSlider value]];
   speedLabel.text = [NSString stringWithFormat:@"%.0f", [speedSlider value]];
   maxTurnLabel.text = [NSString stringWithFormat:@"%.0f", [maxTurnSlider value]];
   turnDelayLabel.text = [NSString stringWithFormat:@"%.0f", [turnDelaySlider value]];
   huntDelayLabel.text = [NSString stringWithFormat:@"%.0f", [huntDelaySlider value]];
}


@end
