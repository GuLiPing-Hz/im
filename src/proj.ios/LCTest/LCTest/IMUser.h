//
//  IMUser.h
//  LCTest
//
//  Created by Happy on 2018/5/25.
//  Copyright © 2018年 lc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface IMUser : NSObject

@property(nonatomic,copy) NSString* uid;
@property(nonatomic,assign) int type;
@property(nonatomic,copy) NSString* avatar;
@property(nonatomic,copy) NSString* name;
@property(nonatomic,assign) int roleId;
@property(nonatomic,assign) int level;

-(instancetype)init:(NSString*)uid withType:(int)type;
-(instancetype)init:(NSString*)uid withType:(int)type withAvatar:(NSString*)avatar 
	withName:(NSString*)name withRoleId:(int)roleId withLevel:(int)level;

-(NSString*)toString;

@end
