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

@end
