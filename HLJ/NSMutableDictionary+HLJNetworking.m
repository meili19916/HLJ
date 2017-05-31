//
//  NSMutableDictionary+HLJNetworking.m
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import "NSMutableDictionary+HLJNetworking.h"
#import "CommonCrypto/CommonDigest.h"

static NSInteger const kMD5Length = 16;

@implementation NSMutableDictionary (HLJNetworking)
- (NSString *)getMD5AndBase64Encrypt{
    NSMutableDictionary *mutableParams = [self mutableCopy];
    NSString *secret = @"mingyi!@#$%";
    NSSet *keySet = [NSSet setWithArray:mutableParams.allKeys];
    NSArray *sortDesc = @[[[NSSortDescriptor alloc] initWithKey:nil ascending:YES]];
    NSArray *sortSetArray = [keySet sortedArrayUsingDescriptors:sortDesc];
    NSString  *kvStr = @"";
    for (NSString *key in sortSetArray) {
        kvStr = [kvStr stringByAppendingString:key];
        if ([mutableParams[key] isKindOfClass:[NSNumber class]]) {
            mutableParams[key] = [NSString stringWithFormat:@"%@",mutableParams[key]];
        }
        kvStr = [kvStr stringByAppendingString:mutableParams[key]];
    }
    kvStr = [kvStr stringByAppendingString:secret];
    const char* str = [kvStr UTF8String];
    unsigned char result[kMD5Length];
    CC_MD5(str, (CC_LONG)strlen(str), result);
    NSData *data = [NSData dataWithBytes:result length:kMD5Length];
    NSString *base64Encoded1 = [data base64EncodedStringWithOptions:0];
    return base64Encoded1;
}
@end
