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

static NSString* sAppKey;
static NSString* sUId;
static NSString* sToken;

static NSString* sIp;
static short sPort = 0;
static int sTimeout = 10;

static int sMaxReLoginTimes = 3;
static int sCurReloginTimes = 0;
static BOOL sIsLogin = NO;
static BOOL sISLogining = NO;

+(BOOL)IsLogin
{
    return sIsLogin;
}
+(void)SetMaxReloginTime:(int)times
{
    sMaxReLoginTimes = times;
}
+(void)SetAppHostPort:(NSString*)appkey withIp:(NSString*)ip withPort:(short)port withTimeout:(int)timeout
{
    sAppKey = appkey;
    sIp = ip;
    sPort = port;
    sTimeout = timeout;
}

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
    return [[LCRequest alloc] init:@"connectLobby" withResponse:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson){
        if(type == RESPONSE_FAILED || type == RESPONSE_CLOSED){
            sIsLogin = NO;
        }
        if(response != NULL)
            response(type,successData,failedCode,reqJson);
    } withAuto:_auto];
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
+(instancetype)login:(NSString*) uid withToken:(NSString*) token withResp:(response_bloct_t) response
{
    if(sAppKey == NULL || sIp == NULL || sPort == 0)
        return NULL;
    
    if(sISLogining)//正在登录中
        return NULL;
    
    sISLogining = YES;
    sIsLogin = NO;
    sUId = uid;
    sToken = token;
    
    [LConnection disconnect];
    if([LConnection connectTo:sIp withPort:sPort withTimeout:sTimeout]){
        return [LCRequest listenConnect:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
            if(type == RESPONSE_SUCCESS){
                if ([LConnection login:uid withToken:token withKey:sAppKey])
                    LCRequest* ret = [[LCRequest alloc] init:@"login" withResponse:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson){
                        sISLogining = NO;
                        if(type == RESPONSE_SUCCESS){
                            sIsLogin = YES;
                        } else {
                            sIsLogin = NO;
                        }
                        if(response)
                            response(type,successData,failedCode,reqJson);
                    }];
                else {
                    sISLogining = NO;
                    if(response)
                        response(RESPONSE_FAILED,NULL,RESULT_REQ_NOT_SEND,NULL);
                }
            } else {
                sISLogining = NO;
                if(response)
                    response(type,successData,failedCode,reqJson);
            }
        } withAuto:YES];
    }
    sISLogining = NO;
    return NULL;
}

+(instancetype)relogin:(response_bloct_t) response
{
    sCurReloginTimes = 0;
    return [LCRequest reloginInner:response];
}

+(instancetype)reloginInner:(response_bloct_t) response
{
    LCRequest* ret = [LCRequest login:sUId withToken:sToken withResp:^(int type,NSDictionary* successData,int failedCode,NSString* reqJson) {
        if(type == RESPONSE_SUCCESS){
            if(response)
                response(type,successData,failedCode,reqJson);
        } else if(type == RESPONSE_FAILED){
            sCurReloginTimes ++;
            if(sCurReloginTimes < sMaxReLoginTimes){
                [LCRequest reloginInner:response];
            } else {
                if(response){
                    response(type,successData,failedCode,reqJson);
                }
            }
        }
    }];
    
    return ret;
}

/**
 * 登出服务器
 *
 * @return
 */
+(BOOL)logout
{
    return [LConnection disconnect];
    sIsLogin = NO;
    /*
    if([LConnection logout]){
        sIsLogin = NO;
        
        //执行一个异步延迟操作,断开服务器连接
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC));
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
            [LConnection disconnect];
        });
        return YES;
    }
    return NO;
    */
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


