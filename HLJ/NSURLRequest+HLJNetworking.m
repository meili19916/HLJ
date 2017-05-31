//
//  NSURLRequest+HLJNetworking.m
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import "NSURLRequest+HLJNetworking.h"
#import <objc/runtime.h>
static void *CTNetworkingRequestParams;

@implementation NSURLRequest (HLJNetworking)
- (void)setRequestParams:(NSDictionary *)requestParams
{
    objc_setAssociatedObject(self, &CTNetworkingRequestParams, requestParams, OBJC_ASSOCIATION_COPY);
}

- (NSDictionary *)requestParams
{
    return objc_getAssociatedObject(self, &CTNetworkingRequestParams);
}
@end
