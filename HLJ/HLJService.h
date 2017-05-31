//
//  HLJService.h
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import <Foundation/Foundation.h>
@class HLJURLResponse;
@protocol HLJServiceProtocol <NSObject>

@property (nonatomic, readonly) BOOL isOnline;

@property (nonatomic, readonly) NSString *debugApiBaseUrl;
@property (nonatomic, readonly) NSString *releaseApiBaseUrl;

@optional

//为某些Service需要拼凑额外字段到URL处
- (NSDictionary *)extraParmas;

//为某些Service需要拼凑额外的HTTPToken，如accessToken
- (NSDictionary *)extraHttpHeadParmasWithMethodName:(NSString *)method withRequest:(NSMutableURLRequest*)request;

- (NSString *)urlGeneratingRuleByMethodName:(NSString *)method;

//- (void)successedOnCallingAPI:(CTURLResponse *)response;

//提供拦截器集中处理Service错误问题，比如token失效要抛通知等
- (BOOL)shouldCallBackByFailedOnCallingAPI:(HLJURLResponse *)response;

@end
@interface HLJService : NSObject
@property (nonatomic, strong, readonly) NSString *publicKey;
@property (nonatomic, strong, readonly) NSString *privateKey;
@property (nonatomic, strong, readonly) NSString *apiBaseUrl;
@property (nonatomic, strong, readonly) NSString *apiVersion;

@property (nonatomic, weak, readonly) id<HLJServiceProtocol> child;

/*
 * 因为考虑到每家公司的拼凑逻辑都有或多或少不同，
 * 如有的公司为http://abc.com/v2/api/login或者http://v2.abc.com/api/login
 * 所以将默认的方式，有versioin时，则为http://abc.com/v2/api/login
 * 否则，则为http://abc.com/v2/api/login
 */
- (NSString *)urlGeneratingRuleByMethodName:(NSString *)method;
@end
