//
//  NSDictionary+HLJNetworking.h
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSDictionary (HLJNetworking)
- (NSString *)HLJ_getMD5AndBase64Encrypt;
- (NSArray *)HLJ_transformedUrlParamsArraySignature:(BOOL)isForSignature;
@end
