//
//  ViewController.h
//  LConnectionTest
//
//  Created by glp on 2017/10/13.
//  Copyright © 2017年 lc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *mEtUid;
@property (weak, nonatomic) IBOutlet UITextField *mEtToken;
@property (weak, nonatomic) IBOutlet UITextField *mEtRoomId;
@property (weak, nonatomic) IBOutlet UITextField *mEtOtherUid;
@property (weak, nonatomic) IBOutlet UITextField *mEtSay;

- (IBAction)clickLogin:(id)sender;
- (IBAction)clickLogout:(id)sender;
- (IBAction)clickEnterRoom:(id)sender;
- (IBAction)clickExitRoom:(id)sender;
- (IBAction)clickP2P:(id)sender;
- (IBAction)clickSay:(id)sender;
@end

