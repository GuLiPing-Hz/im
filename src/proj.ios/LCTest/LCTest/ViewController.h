//
//  ViewController.h
//  LConnectionTest
//
//  Created by glp on 2017/10/13.
//  Copyright © 2017年 lc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <LConnection/LConnection.h>

@interface ViewController : UIViewController

@property(nonatomic,retain) LCRequest* mConnectReq;
@property(nonatomic,retain) LCRequest* mSayReq;

@property(nonatomic,retain) NSString* mMyUID;
@property(nonatomic,retain) NSString* mMyRoomId;

@property(nonatomic,assign) BOOL mIsLogin;
@property(nonatomic,assign) BOOL mIsInRoom;

@property (weak, nonatomic) IBOutlet UILabel *mTextStatus;
@property (weak, nonatomic) IBOutlet UITextField *mEtUid;
@property (weak, nonatomic) IBOutlet UITextField *mEtToken;
@property (weak, nonatomic) IBOutlet UITextField *mEtRoomId;
@property (weak, nonatomic) IBOutlet UITextField *mEtOtherUid;
@property (weak, nonatomic) IBOutlet UITextField *mEtSay;
@property (weak, nonatomic) IBOutlet UISwitch *mShP2P;

- (IBAction)clickLogin:(id)sender;
- (IBAction)clickLogout:(id)sender;
- (IBAction)clickEnterRoom:(id)sender;
- (IBAction)clickExitRoom:(id)sender;
- (IBAction)clickP2P:(id)sender;
- (IBAction)clickSay:(id)sender;
- (IBAction)uidChanged:(id)sender;
- (IBAction)clickSpace:(id)sender;
- (IBAction)clickFor:(id)sender;

@end

