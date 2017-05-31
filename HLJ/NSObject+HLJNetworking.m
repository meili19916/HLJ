//
//  NSObject+HLJNetworking.m
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import "NSObject+HLJNetworking.h"

@implementation NSObject (HLJNetworking)
- (id)HLJ_defaultValue:(id)defaultData
{
    if (![defaultData isKindOfClass:[self class]]) {
        return defaultData;
    }

    if ([self HLJ_isEmptyObject]) {
        return defaultData;
    }

    return self;
}

- (BOOL)HLJ_isEmptyObject
{
    if ([self isEqual:[NSNull null]]) {
        return YES;
    }

    if ([self isKindOfClass:[NSString class]]) {
        if ([(NSString *)self length] == 0) {
            return YES;
        }
    }

    if ([self isKindOfClass:[NSArray class]]) {
        if ([(NSArray *)self count] == 0) {
            return YES;
        }
    }

    if ([self isKindOfClass:[NSDictionary class]]) {
        if ([(NSDictionary *)self count] == 0) {
            return YES;
        }
    }

    return NO;
}

@end
