//
//  IMUser.m
//  LCTest
//
//  Created by Happy on 2018/5/25.
//  Copyright © 2018年 lc. All rights reserved.
//

#import "IMUser.h"

@implementation IMUser

-(instancetype)init:(NSString*)uid withType:(int)type
{
    if(self=[super init]){
        self.uid = uid;
        self.type = type;
    }
    
    return self;
}

-(instancetype)init:(NSString*)uid withType:(int)type withAvatar:(NSString*)avatar 
	withName:(NSString*)name withRoleId:(int)roleId withLevel:(int)level
{
	if(self=[super init]){
        self.uid = uid;
        self.type = type;
        self.avatar = avatar;
        self.name = name;
        self.roleId = roleId;
        self.level = level;
    }
    
    return self;
}

-(NSString*)toString
{
    return [NSString stringWithFormat:@"IMUser{uid='%@', type=%d, avatar='%@', name='%@', roleId=%d, level=%d",self.uid,self.type,self.avatar,self.name,self.roleId,self.level];
}

@end
