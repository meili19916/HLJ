//
//  HLJNetworkingCacheObject.m
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import "HLJNetworkingCacheObject.h"
#import "HLJNetworkingConfigurationManager.h"
@interface HLJNetworkingCacheObject ()

@property (nonatomic, copy, readwrite) NSData *content;
@property (nonatomic, copy, readwrite) NSDate *lastUpdateTime;

@end
@implementation HLJNetworkingCacheObject

#pragma mark - life cycle
- (instancetype)initWithContent:(NSData *)content
{
    self = [super init];
    if (self) {
        self.content = content;
    }
    return self;
}

#pragma mark - public method
- (void)updateContent:(NSData *)content
{
    self.content = content;
}
#pragma mark - getters and setters
- (BOOL)isEmpty
{
    return self.content == nil;
}

- (BOOL)isOutdated
{
    NSTimeInterval timeInterval = [[NSDate date] timeIntervalSinceDate:self.lastUpdateTime];
    return timeInterval > [HLJNetworkingConfigurationManager sharedInstance].cacheOutdateTimeSeconds;
}

- (void)setContent:(NSData *)content
{
    _content = [content copy];
    self.lastUpdateTime = [NSDate dateWithTimeIntervalSinceNow:0];
}
@end
