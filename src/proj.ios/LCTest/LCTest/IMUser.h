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

-(instancetype)init:(NSString*)uid withType:(int)type;

@end
