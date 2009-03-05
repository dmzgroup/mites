#import "dmzMitesModuleiPhone.h"
#import "FlipsideViewController.h"


@implementation FlipsideViewController

@synthesize mitesLabel, chipsLabel, speedLabel, waitLabel;
@synthesize mitesSlider, chipsSlider, speedSlider, waitSlider;


- (void)viewDidLoad {
   
   [super viewDidLoad];
   self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];      
}


- (void)dealloc {

   [mitesLabel release];
   [chipsLabel release];
   [speedLabel release];
   [waitLabel release];

   [mitesSlider release];
   [chipsSlider release];
   [speedSlider release];
   [waitSlider release];
   
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

   if (sender == mitesSlider) {
    
      mitesLabel.text = [NSString stringWithFormat:@"%.0f", [mitesSlider value]];
   }
   else if (sender == chipsSlider) {
      
      chipsLabel.text = [NSString stringWithFormat:@"%.0f", [chipsSlider value]];
   }
   else if (sender == speedSlider) {
      
      speedLabel.text = [NSString stringWithFormat:@"%.0f", [speedSlider value]];
   }
   else if (sender == waitSlider) {
      
      waitLabel.text = [NSString stringWithFormat:@"%.1f", [waitSlider value]];
   }
}


- (IBAction)resetButtonPressed {

   dmz::MitesModuleiPhone *mod (dmz::MitesModuleiPhone::get_instance ());
   if (mod) { mod->reset_lua (); }   
}


- (void)updateLabels {
   
   mitesLabel.text = [NSString stringWithFormat:@"%.0f", [mitesSlider value]];
   chipsLabel.text = [NSString stringWithFormat:@"%.0f", [chipsSlider value]];
   speedLabel.text = [NSString stringWithFormat:@"%.0f", [speedSlider value]];
   waitLabel.text = [NSString stringWithFormat:@"%.1f", [waitSlider value]];
}


@end
