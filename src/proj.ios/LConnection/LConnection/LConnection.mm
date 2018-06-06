//
//  LConnection.m
//  LConnection
//
//  Created by glp on 2017/10/9.
//  Copyright © 2017年 lc. All rights reserved.
//

#import "LConnection.h"
#import "LCRequest.h"
#include "SimpleBridgeIos.h"
#include <string.h>

/*
 查看framework
lipo -info 编译的exe
 
 生成真机和模拟都能使用的framework
 lipo -create framework名  XXX/framework名   -output  XXX/framework名
*/
 
#define RESULT_METHOD @"method"
#define RESULT_CODE  @"code"
#define RESULT_ARG0  @"arg0"
#define RESULT_ARG1  @"arg1"
#define RESULT_ARG2  @"arg2"
#define RESULT_ARG3  @"arg3"
#define RESULT_ARG4  @"arg4"
#define RESULT_ARG5  @"arg5"
#define RESULT_ARG6  @"arg6"
#define RESULT_REQUEST @"request"

@implementation LConnection

static BOOL sIsInit = NO;
static NSMutableArray* sLCRequests = NULL;
static NSLock* sLock = NULL;

// 将NSDictionary或NSArray转化为JSON字符串
+(NSString*) ObjToJSONString:(id)obj
{
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:obj options:NSJSONWritingPrettyPrinted error:NULL];
    
    if (jsonData){
        return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];;
    }else{
        return nil;
    }
}

//将JSON字符串转化为NSDictionary或NSArray
+(id) JSONStringToObj:(NSString*)str
{
    //将NSString转化为NSData
    NSData* data = [str dataUsingEncoding:NSASCIIStringEncoding];
    
    if(data){
        id jsonObject = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:NULL];
        
        return jsonObject;
    }else
        return nil;
}

+(int)getVersionNumber
{
    return 1;
}

+(NSString*)getVersionString
{
    return @"1.0.0";
}

+(void)appendReq:(LCRequest*) request
{
    [sLock lock];
    //需要监听的请求才会放到监听列表中,否则没有意义
    if(request.mResponse)
        [sLCRequests addObject:request];
    [sLock unlock];
}

+(void)removeReq:(LCRequest*) request
{
    [sLock lock];
    [sLCRequests removeObject:request];
    [sLock unlock];
}

+(BOOL)initLConnection
{
    sLock = [[NSLock alloc] init];
    sLCRequests = [NSMutableArray array];
    
    FUNCIOS callback = [=](const char* method,const char* param)->void{
        //将字符串写到缓冲区。
        NSData* jsonData = [[NSString stringWithUTF8String:param] dataUsingEncoding:NSUTF8StringEncoding];
        //解析json数据，使用系统方法 JSONObjectWithData:  options: error:
        NSDictionary* result = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableLeaves error:nil];
        
        NSNumber* nsCode = result[RESULT_CODE];
        int code = nsCode.intValue;
        
//        LOGI("method = %s",method);
        
        [sLock lock];
        for(int i = (int)sLCRequests.count-1;i>=0;i--) {
            LCRequest* request = sLCRequests[i];
            
//            LOGI("for method = %s , request method = %s",method,request.mMethod.UTF8String);
            
            if ([request.mMethod caseInsensitiveCompare:@"connectLobby" ] == NSOrderedSame && (strcmp(method, "onLobbyTunnelConnectSuccess") == 0
                                                                      || strcmp(method , "onLobbyTunnelConnectTimeout" ) == 0|| strcmp(method , "onLobbyTunnelConnectError")==0
                                                                      || strcmp(method , "onLobbyTunnelClose")==0 || strcmp(method , "onLobbyTunnelError")==0 || strcmp(method,"driveAway") == 0)) {
                //优先移除监听
                if (request.mAutoRemove) {//写在判断里面是因为必须找到对应的回调才能移除，否则依旧继续监听
                //检查是否需要移除当前监听
                    [sLCRequests removeObject:request];
                }
                
                if (strcmp(method , "onLobbyTunnelConnectSuccess") == 0) {
                    dispatch_async(dispatch_get_main_queue(), ^{//分发到主线程
                        request.mResponse(RESPONSE_SUCCESS, nil, 0, nil);
                    });
                } else if (strcmp(method , "onLobbyTunnelClose") == 0 || strcmp(method,"driveAway") == 0) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        request.mResponse(RESPONSE_CLOSED, nil, code, nil);
                    });
                } else {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        request.mResponse(RESPONSE_FAILED, nil, code, nil);
                    });
                }
            }
            else if(strcmp(request.mMethod.UTF8String, "login") == 0 && strcmp(method , "onLobbyTunnelError")==0){
                
                //优先移除监听
                if (request.mAutoRemove) {
                    //检查是否需要移除当前监听
                    [sLCRequests removeObject:request];
                }
                
                //如果我们在登录的时候监听到error
                dispatch_async(dispatch_get_main_queue(), ^{
                    request.mResponse(RESPONSE_FAILED, nil, code, result[RESULT_REQUEST]);
                });
            }
            else if(strcmp(request.mMethod.UTF8String, "roomUser") == 0 && (strcmp(method , "roomUser")==0 ||
                strcmp(method , "enterRoom")==0 || strcmp(method , "exitRoom")==0)){
                
                //优先移除监听
                if (request.mAutoRemove) {
                    //检查是否需要移除当前监听
                    [sLCRequests removeObject:request];
                }
                
                NSDictionary* data = nil;
                if (strcmp(method , "enterRoom")==0 ) {
                    //RESULT_ARG0 是room_id
                    NSString* strType =result[RESULT_ARG2];
                    //默认当初是普通用户
                    NSNumber* type = strType!=nil?[NSNumber numberWithInt:strType.intValue]:[NSNumber numberWithInteger:-1];
                    
                    NSDictionary* user = [NSDictionary dictionaryWithObjectsAndKeys:result[RESULT_ARG1],@"uid",type,@"type"
                    	,result[RESULT_ARG3],@"avatar",result[RESULT_ARG4],@"name",result[RESULT_ARG5],@"roleId"
                    	,result[RESULT_ARG6],@"level", nil];
                    NSArray* users = [[NSArray alloc] initWithObjects:user, nil];

                    data = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES],@"is_enter",result[RESULT_ARG0]
                        ,@"room_id",users,@"uids",nil];
                } else if(strcmp(method , "exitRoom")==0){
                    NSDictionary* user = [NSDictionary dictionaryWithObjectsAndKeys:result[RESULT_ARG0],@"uid",[NSNumber numberWithInteger:-1],@"type", nil];
                    NSArray* users = [[NSArray alloc] initWithObjects:user, nil];
                    data = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO],@"is_enter",result[RESULT_ARG0]
                            ,@"room_id",users,@"uids",nil];
                } else{
                    NSArray* tempResult = result[RESULT_ARG1];
//                    NSData* tempJsonData = [tempUsers dataUsingEncoding:NSUTF8StringEncoding];
//                    //解析json数据，使用系统方法 JSONObjectWithData:  options: error:
//                    NSArray* tempResult = [NSJSONSerialization JSONObjectWithData:tempJsonData options:NSJSONReadingMutableLeaves error:nil];
                    
                    NSMutableArray* users = [[NSMutableArray alloc] init];
                    for (int i=0; i<[tempResult count]; i++) {
                        NSArray* arraUserBefore = [tempResult objectAtIndex:i];
                        NSString* strType = arraUserBefore[1];
                        NSDictionary* dicUser = [NSDictionary dictionaryWithObjectsAndKeys:arraUserBefore[0],@"uid"
                        	,[NSNumber numberWithInt:strType.intValue],@"type"
                        	,arraUserBefore[2],@"avatar",arraUserBefore[3],@"name"
                        	,,arraUserBefore[4],@"roleId",arraUserBefore[5],@"level", nil];
                        [users addObject:dicUser];
                    }
                    data = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES],@"is_enter",users,@"uids",nil];
                }

                //如果我们在登录的时候监听到error
                dispatch_async(dispatch_get_main_queue(), ^{
                    request.mResponse(RESPONSE_SUCCESS, data, 0, nil);
                });
            }
            else if (strcmp(request.mMethod.UTF8String, method) == 0) {

                //优先移除监听
                if (request.mAutoRemove) {
                    //检查是否需要移除当前监听
                    [sLCRequests removeObject:request];
                }

                if (code == 0) {
                    if (strcmp(method , "login") == 0) {
                        dispatch_async(dispatch_get_main_queue(), ^{
                            request.mResponse(RESPONSE_SUCCESS, nil, 0, nil);
                        });
                    } else if (strcmp(method , "sayTo") == 0) {
                        id param4 = result[RESULT_ARG4];
                        NSDictionary* data = [NSDictionary dictionaryWithObjectsAndKeys:result[RESULT_ARG0]
                        ,@"type",result[RESULT_ARG1],@"from",result[RESULT_ARG2],@"to",result[RESULT_ARG3],@"content",@"ext",param4 == nil?@"":param4, nil];
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            request.mResponse(RESPONSE_SUCCESS, data, 0, nil);
                        });
                    } else if (strcmp(method , "notify") == 0) {
                        NSDictionary* data = [NSDictionary dictionaryWithObjectsAndKeys:result[RESULT_ARG0]
                                              ,@"from",result[RESULT_ARG1],@"content", nil];
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            request.mResponse(RESPONSE_SUCCESS, data, 0, nil);
                        });
                    } else if (strcmp(method , "enterRoom") == 0) {
                        NSDictionary* data = [NSDictionary dictionaryWithObjectsAndKeys:result[RESULT_ARG0]
                                              ,@"room_id",result[RESULT_ARG1],@"uid", nil];
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            request.mResponse(RESPONSE_SUCCESS, data, 0, nil);
                        });
                    } else if (strcmp(method , "exitRoom") == 0) {
                        NSDictionary* data = [NSDictionary dictionaryWithObjectsAndKeys:result[RESULT_ARG0]                                              ,@"uid", nil];
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            request.mResponse(RESPONSE_SUCCESS, data, 0, nil);
                        });
                    }
                } else {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        request.mResponse(RESPONSE_FAILED, nil, code, result[RESULT_REQUEST]);
                        //[request.mResponse onFailed:code withReq:result[RESULT_REQUEST]];
                    });
                }
            }
        }
        
        [sLock unlock];
    };
    SimpleBridgeIos::getInstance()->setIosBridge(callback);
    
    sIsInit = [LConnection startClient];
    return sIsInit;
}

+(BOOL)startClient
{
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
     [param setValue: @"startClient" forKey:RESULT_METHOD];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)stopClient:(BOOL) finished
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"stopClient" forKey:RESULT_METHOD];
    [param setValue:finished?@"1":@"0" forKey:RESULT_ARG0];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)isConnected
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"isConnected" forKey:RESULT_METHOD];
    return 1 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)connectTo:(NSString*) ip withPort:(short) port withTimeout:(int) timeout
{
    if (!sIsInit)
        return false;
    
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"connectLobby" forKey:RESULT_METHOD];
    [param setValue:ip forKey:RESULT_ARG0];
    [param setValue:[NSString stringWithFormat:@"%d",port] forKey:RESULT_ARG1];
    [param setValue:[NSString stringWithFormat:@"%d",timeout] forKey:RESULT_ARG2];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)disconnect
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"disConnectLobby" forKey:RESULT_METHOD];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)login:(NSString*) uid withToken:(NSString*) token withKey:(NSString*) appkey
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"login" forKey:RESULT_METHOD];
    [param setValue:uid forKey:RESULT_ARG0];
    [param setValue:token forKey:RESULT_ARG1];
    [param setValue:appkey forKey:RESULT_ARG2];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)logout
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"logout" forKey:RESULT_METHOD];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)sayTo:(int) type withFrom:(NSString*) from withTo:(NSString*) to withContent:(NSString*) content withExt:(NSString*) ext
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"sayTo" forKey:RESULT_METHOD];
    [param setValue:[NSString stringWithFormat:@"%d",type] forKey:RESULT_ARG0];
    [param setValue:from forKey:RESULT_ARG1];
    [param setValue:to forKey:RESULT_ARG2];
    [param setValue:content forKey:RESULT_ARG3];
    [param setValue:ext forKey:RESULT_ARG4];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)enterRoom:(NSString*) room_id
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"enterRoom" forKey:RESULT_METHOD];
    [param setValue:room_id forKey:RESULT_ARG0];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

+(BOOL)exitRoom
{
    if (!sIsInit)
        return false;
    
    NSMutableDictionary* param = [[NSMutableDictionary alloc] init];
    [param setValue: @"exitRoom" forKey:RESULT_METHOD];
    return 0 == SimpleBridgeIos::getInstance()->callByNative([self ObjToJSONString:param].UTF8String);
}

@end
