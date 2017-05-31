//
//  NSDictionary+HLJNetworking.m
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import "NSDictionary+HLJNetworking.h"
#import "CommonCrypto/CommonDigest.h"

static NSInteger const kMD5Length = 16;

@implementation NSDictionary (HLJNetworking)
- (NSString *)HLJ_getMD5AndBase64Encrypt{
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


/** 转义参数 */
- (NSArray *)HLJ_transformedUrlParamsArraySignature:(BOOL)isForSignature
{
    NSMutableArray *result = [[NSMutableArray alloc] init];
    [self enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
        if (![obj isKindOfClass:[NSString class]]) {
            obj = [NSString stringWithFormat:@"%@", obj];
        }
        if (!isForSignature) {
            obj = (NSString *)CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(NULL,  (CFStringRef)obj,  NULL,  (CFStringRef)@"!*'();:@&;=+$,/?%#[]",  kCFStringEncodingUTF8));
        }
        if ([obj length] > 0) {
            [result addObject:[NSString stringWithFormat:@"%@=%@", key, obj]];
        }
    }];
    NSArray *sortedResult = [result sortedArrayUsingSelector:@selector(compare:)];
    return sortedResult;
}
@end
