//
//  LCRequest.h
//  LConnection
//
//  Created by glp on 2017/10/9.
//  Copyright © 2017年 lc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 第一个参数表示回调类型 0 表示成功,1表示失败,2表示服务器断开连接
 
 第二个参数是成功时候的返回值,可能为nil;其他情况都为nil
 
 第三个参数是失败的时候的错误码;或者是服务器断开连接的错误码;其他情况为nil
 第四个参数是失败的时候的请求json字符串;其他情况都为nil
 */
#define RESPONSE_SUCCESS 0
#define RESPONSE_FAILED 1
#define RESPONSE_CLOSED 2
typedef void (^response_bloct_t)(int,NSDictionary*,int,NSString*);
typedef void (^error_report_bloct_t)(NSError*);

#define IM_ERROR_DOMAIN @"IM_Domain"

@interface LCRequest : NSObject

@property(nonatomic,copy) NSString* mMethod;
@property(nonatomic,copy) response_bloct_t mResponse;
/**
 * 如果设置为false的时候那么需要你自己删除
 *
 * @see LConnection#removeReq(LCRequest)
 */
@property(nonatomic,assign) BOOL mAutoRemove;//

/**
 是否登录
 */
+(BOOL)IsLogin;
//设定最大重连次数
+(void)SetMaxReloginTime:(int)times;
//设置IM appkey和服务器参数
+(void)SetAppHostPort:(NSString*)appkey withIp:(NSString*)ip withPort:(short)port withTimeout:(int)timeout withErrorReport:(error_report_bloct_t)report;
+(void)ReportError:(NSError*)err;

-(instancetype)init:(NSString*)method withResponse:(response_bloct_t)response;
-(instancetype)init:(NSString*)method withResponse:(response_bloct_t)response withAuto:(BOOL) _auto;

/**
 * 监听服务器连接
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenConnect:(response_bloct_t)response withAuto:(BOOL) _auto;

/**
 * 登陆服务器
 *
 * @param uid
 * @param token
 * @param appkey
 * @param response
 * @return
 */
+(instancetype)login:(NSString*) uid withToken:(NSString*) token withResp:(response_bloct_t) response;

/**
 重连服务器
 */
+(instancetype)relogin:(response_bloct_t) response;
/**
 * 登出服务器
 *
 * @return
 */
+(BOOL)logout;

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
+(instancetype)sayTo:(int) type withFrom:(NSString*) from withTo:(NSString*) to withContent:(NSString*) content withExt:(NSString*)ext withResp:(response_bloct_t) response;

/**
 * 监听发言消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenSay:(response_bloct_t) response withAuto:(BOOL) _auto;

/**
 * 监听通知消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenNotify:(response_bloct_t) response withAuto:(BOOL)_auto;

/**
 * 进入房间
 *
 * @param room_id
 * @param response
 * @return
 */
+(instancetype)enterRoom:(NSString*) room_id withResp:(response_bloct_t) response;

/**
 * 监听进入房间的消息
 *
 * @param response
 * @param _auto
 * @return
 */
+(instancetype)listenEnterRoom:(response_bloct_t) response withAuto:(BOOL) _auto;

/**
 * 离开房间
 *
 * @param response
 * @return
 */
+(instancetype)exitRoom:(response_bloct_t) response;

/**
 * 监听离开房间的消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenExitRoom:(response_bloct_t) response withAuto:(BOOL) _auto;


/**
 * 监听房间用户进出的消息，会监听自己的消息
 *
 * @param response
 * @param auto
 * @return
 */
+(instancetype)listenRoomUser:(response_bloct_t) response withAuto:(BOOL) _auto;
@end

