//
//  LCResponse.h
//  LConnection
//
//  Created by glp on 2017/10/9.
//  Copyright © 2017年 lc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol LCResponse <NSObject>

@required
/**
 * 请求成功
 * @param data 没有数据的时候为null
 */
-(void)onSuccess:(NSDictionary*)data;
/**
 * 请求失败
 * @param code 错误码
 * @param jsonReq 请求json
 */
-(void)onFailed:(int)code withReq:(NSString*)jsonReq;

@optional
/**
 * 服务器主动断开连接
 */
-(void)onClose;

@end
