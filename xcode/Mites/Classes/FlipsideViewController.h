#import <UIKit/UIKit.h>

@interface FlipsideViewController : UIViewController {

   IBOutlet UILabel *mitesLabel;
   IBOutlet UILabel *chipsLabel;
   IBOutlet UILabel *speedLabel;
   IBOutlet UILabel *maxTurnLabel;
   IBOutlet UILabel *turnDelayLabel;
   IBOutlet UILabel *huntDelayLabel;
   
   IBOutlet UISlider *mitesSlider;
   IBOutlet UISlider *chipsSlider;
   IBOutlet UISlider *speedSlider;
   IBOutlet UISlider *maxTurnSlider;
   IBOutlet UISlider *turnDelaySlider;
   IBOutlet UISlider *huntDelaySlider;  
   
   IBOutlet UIButton *stopButton;
   IBOutlet UIButton *resetButton;
   
   BOOL resetRequired;
}

@property (nonatomic, retain) UILabel *mitesLabel;
@property (nonatomic, retain) UILabel *chipsLabel;
@property (nonatomic, retain) UILabel *speedLabel;
@property (nonatomic, retain) UILabel *maxTurnLabel;
@property (nonatomic, retain) UILabel *turnDelayLabel;
@property (nonatomic, retain) UILabel *huntDelayLabel;

@property (nonatomic, retain) UISlider *mitesSlider;
@property (nonatomic, retain) UISlider *chipsSlider;
@property (nonatomic, retain) UISlider *speedSlider;
@property (nonatomic, retain) UISlider *maxTurnSlider;
@property (nonatomic, retain) UISlider *turnDelaySlider;
@property (nonatomic, retain) UISlider *huntDelaySlider;

@property (nonatomic, retain) UIButton *stopButton;
@property (nonatomic, retain) UIButton *resetButton;

@property (nonatomic) BOOL resetRequired;


- (IBAction)sliderValueChanged:(id)sender;
- (IBAction)stopButtonPressed;
- (IBAction)resetButtonPressed;

- (void) updateLabels;

@end
