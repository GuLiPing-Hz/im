//
//  LConnection.h
//  LConnection
//
//  Created by glp on 2017/10/9.
//  Copyright © 2017年 lc. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "LCRequest.h"

/**
 * 0：点对点发消息，1：群内消息
 */
#define TYPE_P2P 0
#define TYPE_TEAM 1

/**
 SDK 内部code
 */
#define RESULT_TIMEOUT -9999
#define RESULT_ERROR_PARSE -9998
#define RESULT_OOM -9997

@interface LConnection : NSObject

+(int)getVersionNumber;

+(NSString*)getVersionString;

+(void)appendReq:(LCRequest*) request;

+(void)removeReq:(LCRequest*) request;

+(BOOL)initLConnection;

+(BOOL)stopClient:(BOOL) finished;

+(BOOL)isConnected;

+(BOOL)connectTo:(NSString*) ip withPort:(short) port withTimeout:(int) timeout;

+(BOOL)disconnect;

+(BOOL)login:(NSString*) uid withToken:(NSString*) token withKey:(NSString*) appkey;

+(BOOL)logout;

+(BOOL)sayTo:(int) type withFrom:(NSString*) from withTo:(NSString*) to withContent:(NSString*) content withExt:(NSString*) ext;

+(BOOL)enterRoom:(NSString*) room_id;

+(BOOL)exitRoom;

@end
