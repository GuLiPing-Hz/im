//
//  ViewController.m
//  LConnectionTest
//
//  Created by glp on 2017/10/13.
//  Copyright © 2017年 lc. All rights reserved.
//

#import "ViewController.h"
#include "WSProgressHUD.h"
#import "IMUser.h"

#define APPKEY @"4ed38057df363e8355229ec53687c549"
//#define HOST @"192.168.1.67"
#define HOST @"192.168.0.18"
//#define HOST  @"123.206.229.213"
#define PORT  27710

#define UID1 @"1000001"
#define TOKEN1 @"018f0be0cb075ee761cf8d44be2b287e"
#define UID2  @"1000002"
#define TOKEN2  @"9a78580de4e75b65b9b3e6840aede0dd"
#define UID11 @"1000011"
#define TOKEN11 @"06c16a5cf83d77f8f531c3e0d65e31a9"
#define UID12 @"1000012"
#define TOKEN12 @"30f9f8feceee317a66df684915be86f9"


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.mMyUID = nil;
    self.mMyRoomId = nil;
    self.mIsLogin = NO;
    self.mIsInRoom = NO;
    self.mRoomUser = [[NSMutableDictionary alloc] init];
    
    [self.mEtUid setText:UID1];
    [self.mEtToken setText:TOKEN1];
    
    [LConnection initLConnection];
    [LCRequest SetMaxReloginTime:3];
    [LCRequest SetAppHostPort:APPKEY withIp:HOST withPort:PORT withTimeout:10];
}

-(void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    
    //记得回收资源
    [LConnection stopClient:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)showTip:(NSString*)tip{
    [WSProgressHUD showImage:nil status:tip];
}

-(void)showStatus:(NSString*)status{
    [self.mTextStatus setText:status];
}

-(void)listenIMConnect
{
    self.mConnectReq = [LCRequest listenConnect:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson){
        //连接断开了.我们先移除监听
        [LConnection removeReq:self.mConnectReq];
        //[LConnection removeReq:self.mSayReq];
        
        //把房间信息先清空
        self.mMyRoomId = nil;
        self.mIsInRoom = NO;
        
        //监听不会有成功的回调
        if(type == RESPONSE_FAILED){
            [self showStatus:[NSString stringWithFormat:@"服务器异常 code =%d",failedCode]];
            
            //重连
            [LCRequest relogin:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
                if(type == RESPONSE_SUCCESS){
                    [self showStatus:[NSString stringWithFormat:@"重连成功"]];
                    
                    //重连成功的时候,我们再加上
                    [LConnection appendReq:self.mConnectReq];
                    //[LConnection appendReq:self.mSayReq];
                } else {
                    [self showStatus:[NSString stringWithFormat:@"重连超过最大次数,返回登录界面"]];
                    
                    self.mMyUID = nil;
                    self.mIsLogin = NO;
                }
            }];
        } else if(type == RESPONSE_CLOSED){
            if(failedCode == 0){//服务器直接close
                [self showTip:@"服务器主动断开,请退到登录界面重新登录"];
                
                [self showStatus:[NSString stringWithFormat:@"服务器主动断开,请退到登录界面重新登录"]];
            }
            else if(failedCode == 9){//收到服务器的关闭code 参见服务器文档code原因
                [self showTip:[NSString stringWithFormat:@"有其他设备登录您的账号" ]];
                
                [self showStatus:[NSString stringWithFormat:@"有其他设备登录您的账号"]];
            }
            else{
                [self showTip:[NSString stringWithFormat:@"服务器主动断开,请退到登录界面重新登录code=%d",failedCode ]];
                
                [self showStatus:[NSString stringWithFormat:@"服务器主动断开,请退到登录界面重新登录code=%d",failedCode ]];
            }
            
            self.mMyUID = nil;
            self.mIsLogin = NO;
        }
        
    } withAuto:NO];
}

-(void)listenIMMsg
{
    self.mSayReq = [LCRequest listenSay:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        if(type == RESPONSE_SUCCESS){
            
            NSNumber* temp1 = successData[@"type"];
            int talkType = [temp1 intValue];
            NSString* from = successData[@"from"];
            NSString* to = successData[@"to"];
            NSString* content = successData[@"content"];
            NSString* ext = successData[@"ext"];
            //聊天消息这里只是log了一下
            NSString* log = [NSString stringWithFormat:@"[%@] |%@| 对 |%@| 说{%@} ext={%@}",talkType==TYPE_P2P?@"P2P":@"TEAM",from,to,content,ext];
            NSLog(log);
            [self.mTextStatus setText:log];
            
        }//监听不会收到失败消息
    } withAuto:NO];
}

-(void)listenRoomUser
{
    [self.mRoomUser removeAllObjects];
    self.mRoomUserReq = [LCRequest listenRoomUser:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        //监听不会收到失败消息
        NSNumber* temp1 = successData[@"is_enter"];
        BOOL isEnter = [temp1 boolValue];
        NSArray* uids = successData[@"uids"];
        
        //排除自己进入房间的消息
        if(uids.count == 1 && [uids[0][@"uid"] caseInsensitiveCompare:self.mMyUID] == NSOrderedSame){
            return;
        }
        
        
        NSString* log;
        for(NSDictionary* uid in uids){
            NSString* dicType = uid[@"type"];
            
            if(isEnter){
                IMUser* user = [[IMUser alloc] init:uid[@"uid"] withType:dicType.intValue];
                [self.mRoomUser setObject:user forKey:uid[@"uid"]];
            } else {
                [self.mRoomUser removeObjectForKey:uid[@"uid"]];
            }
            
            //离开房间的时候不会传入type，可根据自己的在线列表知晓用户类型
            if(isEnter){
                log = [NSString stringWithFormat:@"用户【%@[%@]】 %@",uid[@"uid"],[self getIMUserType:dicType.intValue],@"进入房间"];
            } else {
                log = [NSString stringWithFormat:@"用户【%@】 %@",uid[@"uid"],@"离开房间"];
            }
            
            NSLog(@"%@",log);
        }
        //消息这里只是log了一下
        //显示最后一条消息
        [self.mTextStatus setText:log];
        
        [self logRoomUser];
        
    } withAuto:NO];
}

-(void) logRoomUser {
    NSLog(@"查看房间其它用户 begin");
    for(NSString* key in self.mRoomUser){
        IMUser* user = self.mRoomUser[key];
        NSLog(@"房间用户 【uid=%@(%@)】",key,[self getIMUserType:user.type]);
    }
    NSLog(@"查看房间其它用户 end");
}

-(NSString*)getIMUserType:(int) type
{
    NSString* typeDec = @"未知";
    if(type == 0){
        typeDec = @"普通用户";
    } else if(type == 1){
        typeDec = @"机器人";
    }
    return typeDec;
}

- (IBAction)clickLogin:(id)sender {
    if(self.mIsLogin){
        [self showTip:@"您已经登录"];
        return;
    }
    
    NSString* uid = self.mEtUid.text;
    NSString* token = self.mEtToken.text;
    
    if(uid.length == 0 || token.length == 0){
        [self showTip:@"请输入UID和TOKNE"];
        return;
    }
    
    LCRequest* req = [LCRequest login:uid withToken:token withResp:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        if(type == RESPONSE_SUCCESS){
            /*
             成功之后需要监听服务器连接
             */
            [self listenIMConnect];
            
            //监听发言
            [self listenIMMsg];
            
            
            [self showStatus:@"登录成功"];
            self.mMyUID = uid;
            self.mIsLogin = YES;
            
        } else if(type == RESPONSE_FAILED){
            NSLog(@"失败,请求内容是:(code=%d) %@",failedCode,reqJson);
            [self showStatus:[NSString stringWithFormat:@"登录失败 code =%d",failedCode]];
        }//请求里面不会有连接断开的回调
    }];
    
    if(req != nil){
        [self showStatus:@"正在登录..."];
    } else {
        [self showStatus:@"无法请求登录"];
    }
}

- (IBAction)clickLogout:(id)sender {
    if(!self.mIsLogin){
        [self showTip:@"您未登录"];
        return ;
    }
    
    if([LCRequest logout]){
        
        [LConnection removeReq:self.mSayReq];
        [LConnection removeReq:self.mConnectReq];
        [LConnection removeReq:self.mRoomUserReq];
        
        self.mMyUID = nil;
        self.mIsLogin = NO;
        
        self.mMyRoomId = nil;
        self.mIsInRoom = NO;
        
        [self showStatus:@"登出成功"];
        [self showTip:@"登出成功"];
    } else {
        [self showTip:@"登出失败"];
    }
}

- (IBAction)clickEnterRoom:(id)sender {
    if(!self.mIsLogin){
        [self showTip:@"您未登录"];
        return ;
    }
    
    if(self.mIsInRoom){
        [self showTip:@"您已经在房间"];
        return ;
    }
    
    NSString* roomId = self.mEtRoomId.text;
    if(roomId.length == 0){
        [self showTip:@"请输入房间ID"];
        return;
    }
    
    [self listenRoomUser];
    LCRequest* req = [LCRequest enterRoom:roomId withResp:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        if(type == RESPONSE_SUCCESS){
            [self showStatus:@"进入房间成功"];
            
            self.mIsInRoom = YES;
            self.mMyRoomId = roomId;
        } else if(type == RESPONSE_FAILED){
            NSLog(@"失败,请求内容是:(code=%d) %@",failedCode,reqJson);
            [self showStatus:[NSString stringWithFormat:@"进入房间失败 code =%d",failedCode]];
        }
    }];
    
    if(req != nil)
       [self showStatus:@"正在进入房间..."];
    else
        [self showStatus:@"进入房间请求失败"];
}

- (IBAction)clickExitRoom:(id)sender {
    if(!self.mIsInRoom){
        [self showTip:@"您未在房间"];
        return ;
    }
    
    //离开房间移除监听
    [LConnection removeReq:self.mRoomUserReq];
    LCRequest* req = [LCRequest exitRoom:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        if(type == RESPONSE_SUCCESS){
            [self showStatus:@"离开房间成功"];
        } else if(type == RESPONSE_FAILED){
            NSLog(@"失败,请求内容是:(code=%d) %@",failedCode,reqJson);
            [self showStatus:[NSString stringWithFormat:@"离开房间失败 code =%d",failedCode]];
        }
        
        self.mIsInRoom = NO;
        self.mMyRoomId = nil;
    }];
    
    if(req != nil)
        [self showStatus:@"离开房间..."];
    else
        [self showStatus:@"离开房间请求失败"];
}

- (IBAction)clickP2P:(id)sender {
    NSLog(@"P2P : %@",self.mShP2P.on?@"YES":@"NO");
    
    [self.mEtOtherUid setEnabled:self.mShP2P.on];
}

-(void)say{
    if(!self.mIsLogin){
        [self showTip:@"您未登录"];
        return ;
    }
    
    NSString* sayId = nil;
    int type = TYPE_P2P;
    if(self.mShP2P.on){
        sayId = self.mEtOtherUid.text;
    } else {
        if(!self.mIsInRoom || self.mMyRoomId == nil){
            [self showTip:@"您未在房间"];
            return;
        }
        
        type = TYPE_TEAM;
        sayId = self.mMyRoomId;
    }
    
    NSString* content = self.mEtSay.text;
    if(sayId.length == 0 || content.length == 0){
        [self showTip:@"请输入对话ID和想说的内容"];
        return;
    }
    
    //content = @"<gpmsg from=\"3001041\" dateline=\"2017-10-27 11:25:13\" id=\"30010411509074713903\" type=\"1\" to=\"3002167\"><send avatar=\"http://eaglelive-10077467.image.myqcloud.com/31cc83ff-2ef6-43fe-9b96-5d5df6ce66dc?imageView2/1/w/260/h/260 \" uid=\"3001041\" nickname=\"Erennnnnnnnnnnnn\" level=\"100\" grade=\"0\" role_id=\"1\" icon=\"0\"/><gift image=\"\" id=\"\" mode=\"\" number=\"\" name=\"\"/><praise num=\"\"/><action avatar=\"\" gender=\"\" uid=\"\" nickname=\"\" age=\"\" total_num=\"\" role_id=\"\" type=\"\"/><msg file_location=\"\" file=\"\" body=\"是\"/></gpmsg>";
    
    LCRequest* req = [LCRequest sayTo:type withFrom:self.mMyUID withTo:sayId withContent:content withExt:@"" withResp:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        if(type == RESPONSE_SUCCESS){
            [self showStatus:@"发言成功"];
        } else if(type == RESPONSE_FAILED){
            NSLog(@"失败,请求内容是:(code=%d) %@",failedCode,reqJson);
            [self showStatus:[NSString stringWithFormat:@"发言失败 code =%d",failedCode]];
        }
    }];
    
    if(req != nil)
        [self showStatus:@"正在发送聊天..."];
    else
        [self showStatus:@"发送了聊天请求失败"];
}

- (IBAction)clickSay:(id)sender {
    //单独发送一次聊天
    [self say];
}

- (IBAction)uidChanged:(id)sender {
    if ([self.mEtUid.text caseInsensitiveCompare:UID1] == NSOrderedSame) {
        [self.mEtToken setText:TOKEN1];
    } else if([self.mEtUid.text caseInsensitiveCompare:UID2] == NSOrderedSame){
        [self.mEtToken setText:TOKEN2];
    }else if([self.mEtUid.text caseInsensitiveCompare:UID11] == NSOrderedSame){
        [self.mEtToken setText:TOKEN11];
    }else if([self.mEtUid.text caseInsensitiveCompare:UID12] == NSOrderedSame){
        [self.mEtToken setText:TOKEN12];
    }
}

- (IBAction)clickSpace:(id)sender {
    //点击空白隐藏键盘
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
}

- (IBAction)clickFor:(id)sender {
    //启动一个timer 0.01次发送房间信息
    [NSTimer scheduledTimerWithTimeInterval:0.001 target:self selector:@selector(say) userInfo:NULL repeats:YES];
}

@end
