//
//  HLJServiceFactory.h
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HLJService.h"
@protocol HLJServiceFactoryDataSource <NSObject>

/*
 * key为service的Identifier
 * value为service的Class的字符串
 */
- (NSDictionary<NSString *,NSString *> *)servicesKindsOfServiceFactory;

@end
@interface HLJServiceFactory : NSObject
@property (nonatomic, weak) id<HLJServiceFactoryDataSource> dataSource;

+ (instancetype)sharedInstance;
- (HLJService<HLJServiceProtocol> *)serviceWithIdentifier:(NSString *)identifier;
@end
