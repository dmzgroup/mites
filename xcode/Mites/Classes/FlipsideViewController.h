#import <UIKit/UIKit.h>

@interface FlipsideViewController : UIViewController {

   IBOutlet UILabel *mitesLabel;
   IBOutlet UILabel *chipsLabel;
   IBOutlet UILabel *speedLabel;
   IBOutlet UILabel *waitLabel;
   
   IBOutlet UISlider *mitesSlider;
   IBOutlet UISlider *chipsSlider;
   IBOutlet UISlider *speedSlider;
   IBOutlet UISlider *waitSlider;   
}

@property (nonatomic, retain) UILabel *mitesLabel;
@property (nonatomic, retain) UILabel *chipsLabel;
@property (nonatomic, retain) UILabel *speedLabel;
@property (nonatomic, retain) UILabel *waitLabel;

@property (nonatomic, retain) UISlider *mitesSlider;
@property (nonatomic, retain) UISlider *chipsSlider;
@property (nonatomic, retain) UISlider *speedSlider;
@property (nonatomic, retain) UISlider *waitSlider;


- (IBAction)sliderValueChanged:(id)sender;
- (IBAction)resetButtonPressed;

- (void) updateLabels;

@end
