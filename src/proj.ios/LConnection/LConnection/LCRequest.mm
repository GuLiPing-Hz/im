//
//  LCRequest.m
//  LConnection
//
//  Created by glp on 2017/10/9.
//  Copyright © 2017年 lc. All rights reserved.
//

#import "LCRequest.h"
#include "LConnection.h"

@implementation LCRequest

-(instancetype)init:(NSString*)method withResponse:(response_bloct_t)response
{
    return [self init:method withResponse:response withAuto:YES];
}

-(instancetype)init:(NSString*)method withResponse:(response_bloct_t)response withAuto:(BOOL) _auto
{
    self = [super init];
    if(self){
        self.mMethod = method;
        self.mResponse = response;
        self.mAutoRemove = _auto;
        
        [LConnection appendReq:self ];
    }
    
    return self;
}

/**
 * 连接服务器
 *
 * @param ip
 * @param port
 * @param timeout
 * @param response
 * @return
 */
+(instancetype)connectTo:(NSString*) ip withPort:(short)port withTimeout:(int) timeout wihtResp: (response_bloct_t) response
{
    if ([LConnection connectTo:ip withPort:port withTimeout:timeout])
        return [LCRequest listenConnect:response withAuto:YES];
    return NULL;
}

/**
 * 监听服务器连接
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenConnect:(response_bloct_t)response withAuto:(BOOL) _auto
{
    return [[LCRequest alloc] init:@"connectLobby" withResponse:response withAuto:_auto];
}

/**
 * 断开连接
 *
 * @return
 */
+(BOOL)disconnect
{
    return [LConnection disconnect];
}

/**
 * 登陆服务器
 *
 * @param uid
 * @param token
 * @param appkey
 * @param response
 * @return
 */
+(instancetype)login:(NSString*) uid withToken:(NSString*) token withKey:(NSString*) appkey withResp:(response_bloct_t) response
{
    if ([LConnection login:uid withToken:token withKey:appkey])
        return [[LCRequest alloc] init:@"login" withResponse:response];
    return NULL;
}

/**
 * 登出服务器
 *
 * @return
 */
+(BOOL)logout
{
    return [LConnection logout];
}

/**
 * 发言
 *
 * @param type
 * @param from
 * @param to
 * @param content
 * @param response
 * @return
 */
+(instancetype)sayTo:(int) type withFrom:(NSString*) from withTo:(NSString*) to withContent:(NSString*) content withExt:(NSString*)ext withResp:(response_bloct_t) response
{
    if ([LConnection sayTo:type withFrom:from withTo:to withContent:content withExt:ext])
        return [LCRequest listenSay:response withAuto:YES];
    return NULL;
}

/**
 * 监听发言消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenSay:(response_bloct_t) response withAuto:(BOOL) _auto
{
    return [[LCRequest alloc] init:@"sayTo" withResponse:response withAuto:_auto];
}

/**
 * 监听通知消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenNotify:(response_bloct_t) response withAuto:(BOOL)_auto
{
    return [[LCRequest alloc] init:@"notify" withResponse:response withAuto:_auto];
}

/**
 * 进入房间
 *
 * @param room_id
 * @param response
 * @return
 */
+(instancetype)enterRoom:(NSString*) room_id withResp:(response_bloct_t) response
{
    if ([LConnection enterRoom:room_id])
        return [LCRequest listenEnterRoom:response withAuto:YES];
    return NULL;
}

/**
 * 监听进入房间的消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenEnterRoom:(response_bloct_t) response withAuto:(BOOL) _auto
{
    return [[LCRequest alloc] init:@"enterRoom" withResponse:response withAuto:_auto];
}

/**
 * 离开房间
 *
 * @param response
 * @return
 */
+(instancetype)exitRoom:(response_bloct_t) response
{
    if ([LConnection exitRoom])
        return [LCRequest listenExitRoom:response withAuto:YES];
    return NULL;
}

/**
 * 监听离开房间的消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenExitRoom:(response_bloct_t) response withAuto:(BOOL) _auto
{
    return [[LCRequest alloc] init:@"exitRoom" withResponse:response withAuto:_auto];
}

@end


