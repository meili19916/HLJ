//
//  HLJService.m
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import "HLJService.h"
#import "NSURLRequest+HLJNetworking.h"
#import "NSDictionary+HLJNetworking.h"
@interface HLJService()

@property (nonatomic, weak, readwrite) id<HLJServiceProtocol> child;

@end
@implementation HLJService

- (instancetype)init
{
    self = [super init];
    if (self) {
        if ([self conformsToProtocol:@protocol(HLJServiceProtocol)]) {
            self.child = (id<HLJServiceProtocol>)self;
        }
    }
    return self;
}

- (NSString *)urlGeneratingRuleByMethodName:(NSString *)methodName {
    NSString *urlString = [NSString stringWithFormat:@"%@/%@", self.apiBaseUrl, methodName];
    return urlString;
}

#pragma mark - HLJServiceProtocol
- (NSDictionary *)extraHttpHeadParmasWithMethodName:(NSString *)method withRequest:(NSURLRequest*)request{
    return @{};
}
@end
