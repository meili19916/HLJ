//
//  HLJBaseApiManager.h
//  HLJ
//
//  Created by HeLijuan on 17/5/31.
//  Copyright © 2017年 HeLijuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HLJURLResponse.h"
@class HLJAPIBaseManager;

static NSString * const kHLJAPIBaseManagerRequestID = @"kHLJAPIBaseManagerRequestID";

/*************************************************************************************************/
/*                               HLJAPIManagerApiCallBackDelegate                                 */
/*************************************************************************************************/

@protocol HLJAPIManagerCallBackDelegate <NSObject>

@required
- (void)managerCallAPIDidSuccess:(HLJAPIBaseManager *)manager;
- (void)managerCallAPIDidFailed:(HLJAPIBaseManager *)manager;
@end

/*************************************************************************************************/
/*                               HLJAPIManagerDataReformer                                 */
/*************************************************************************************************/
@protocol HLJAPIManagerDataReformer <NSObject>

@required
- (id)manager:(HLJAPIBaseManager *)manager reformData:(NSDictionary *)data;
@end

/*************************************************************************************************/
/*                               HLJAPIManagerValidator                                 */
/*                               数据检查                                 */
/*************************************************************************************************/
@protocol HLJAPIManagerValidator <NSObject>
@required
/*
 所有的callback数据都应该在这个函数里面进行检查，事实上，到了回调delegate的函数里面是不需要再额外验证返回数据是否为空的。
 因为判断逻辑都在这里做掉了。
 而且本来判断返回数据是否正确的逻辑就应该交给manager去做，不要放到回调到controller的delegate方法里面去做。
 */
- (BOOL)manager:(HLJAPIBaseManager *)manager isCorrectWithCallBackData:(NSDictionary *)data;

- (BOOL)manager:(HLJAPIBaseManager *)manager isCorrectWithParamsData:(NSDictionary *)data;
@end

/*************************************************************************************************/
/*                                HLJAPIManagerParamSourceDelegate                                */
/*************************************************************************************************/
//让manager能够获取调用API所需要的数据
@protocol HLJAPIManagerParamSource <NSObject>
@required
- (NSDictionary *)paramsForApi:(HLJAPIBaseManager *)manager;
@end


typedef NS_ENUM (NSUInteger, HLJAPIManagerErrorType){
    HLJAPIManagerErrorTypeDefault,       //没有产生过API请求，这个是manager的默认状态。
    HLJAPIManagerErrorTypeSuccess,       //API请求成功且返回数据正确，此时manager的数据是可以直接拿来使用的。
    HLJAPIManagerErrorTypeNoContent,     //API请求成功但返回数据不正确。如果回调数据验证函数返回值为NO，manager的状态就会是这个。
    HLJAPIManagerErrorTypeParamsError,   //参数错误，此时manager不会调用API，因为参数验证是在调用API之前做的。
    HLJAPIManagerErrorTypeTimeout,       //请求超时。APIProxy设置的是20秒超时，具体超时时间的设置请自己去看HLJAPIProxy的相关代码。
    HLJAPIManagerErrorTypeNoNetWork      //网络不通。在调用API之前会判断一下当前网络是否通畅，这个也是在调用API之前验证的，和上面超时的状态是有区别的。
};


typedef NS_ENUM (NSUInteger, HLJAPIManagerRequestType){
    HLJAPIManagerRequestTypeGet,
    HLJAPIManagerRequestTypePost,
    HLJAPIManagerRequestTypePut,
    HLJAPIManagerRequestTypeDelete
};

/*************************************************************************************************/
/*                                         HLJAPIManager                                          */
/*************************************************************************************************/
/*
 HLJAPIBaseManager的派生类必须符合这些protocal
 */
@protocol HLJAPIManager <NSObject>

@required
- (NSString *)methodName;
- (NSString *)serviceType;
- (HLJAPIManagerRequestType)requestType;
- (BOOL)shouldCache;

// used for pagable API Managers mainly
@optional
- (void)cleanData;
- (NSDictionary *)reformParams:(NSDictionary *)params;
- (NSInteger)loadDataWithParams:(NSDictionary *)params;
- (BOOL)shouldLoadFromNative;

@end


/*************************************************************************************************/
/*                                    HLJAPIManagerInterceptor                                    */
/*************************************************************************************************/
/*
 HLJAPIBaseManager的派生类必须符合这些protocal
 */
@protocol HLJAPIManagerInterceptor <NSObject>

@optional
- (BOOL)manager:(HLJAPIBaseManager *)manager beforePerformSuccessWithResponse:(HLJURLResponse *)response;
- (void)manager:(HLJAPIBaseManager *)manager afterPerformSuccessWithResponse:(HLJURLResponse *)response;

- (BOOL)manager:(HLJAPIBaseManager *)manager beforePerformFailWithResponse:(HLJURLResponse *)response;
- (void)manager:(HLJAPIBaseManager *)manager afterPerformFailWithResponse:(HLJURLResponse *)response;

- (BOOL)manager:(HLJAPIBaseManager *)manager shouldCallAPIWithParams:(NSDictionary *)params;
- (void)manager:(HLJAPIBaseManager *)manager afterCallingAPIWithParams:(NSDictionary *)params;

@end
@interface HLJBaseApiManager : NSObject
@property (nonatomic, weak) id<HLJAPIManagerCallBackDelegate> delegate;
@property (nonatomic, weak) id<HLJAPIManagerParamSource> paramSource;
@property (nonatomic, weak) id<HLJAPIManagerValidator> validator;
@property (nonatomic, weak) NSObject<HLJAPIManager> *child; //里面会调用到NSObject的方法，所以这里不用id
@property (nonatomic, weak) id<HLJAPIManagerInterceptor> interceptor;

/*
 baseManager是不会去设置errorMessage的，派生的子类manager可能需要给controller提供错误信息。所以为了统一外部调用的入口，设置了这个变量。
 派生的子类需要通过extension来在保证errorMessage在对外只读的情况下使派生的manager子类对errorMessage具有写权限。
 */
@property (nonatomic, copy, readonly) NSString *errorMessage;
@property (nonatomic, readonly) HLJAPIManagerErrorType errorType;
@property (nonatomic, strong) HLJURLResponse *response;

@property (nonatomic, assign, readonly) BOOL isReachable;
@property (nonatomic, assign, readonly) BOOL isLoading;

- (id)fetchDataWithReformer:(id<HLJAPIManagerDataReformer>)reformer;

//尽量使用loadData这个方法,这个方法会通过param source来获得参数，这使得参数的生成逻辑位于controller中的固定位置
- (NSInteger)loadData;

- (void)cancelAllRequests;
- (void)cancelRequestWithRequestId:(NSInteger)requestID;

// 拦截器方法，继承之后需要调用一下super
- (BOOL)beforePerformSuccessWithResponse:(HLJURLResponse *)response;
- (void)afterPerformSuccessWithResponse:(HLJURLResponse *)response;

- (BOOL)beforePerformFailWithResponse:(HLJURLResponse *)response;
- (void)afterPerformFailWithResponse:(HLJURLResponse *)response;

- (BOOL)shouldCallAPIWithParams:(NSDictionary *)params;
- (void)afterCallingAPIWithParams:(NSDictionary *)params;

/*
 用于给继承的类做重载，在调用API之前额外添加一些参数,但不应该在这个函数里面修改已有的参数。
 子类中覆盖这个函数的时候就不需要调用[super reformParams:params]了
 HLJAPIBaseManager会先调用这个函数，然后才会调用到 id<HLJAPIManagerValidator> 中的 manager:isCorrectWithParamsData:
 所以这里返回的参数字典还是会被后面的验证函数去验证的。

 假设同一个翻页Manager，ManagerA的paramSource提供page_size=15参数，ManagerB的paramSource提供page_size=2参数
 如果在这个函数里面将page_size改成10，那么最终调用API的时候，page_size就变成10了。然而外面却觉察不到这一点，因此这个函数要慎用。

 这个函数的适用场景：
 当两类数据走的是同一个API时，为了避免不必要的判断，我们将这一个API当作两个API来处理。
 那么在传递参数要求不同的返回时，可以在这里给返回参数指定类型。

 具体请参考AJKHDXFLoupanCategoryRecommendSamePriceAPIManager和AJKHDXFLoupanCategoryRecommendSameAreaAPIManager

 */
- (NSDictionary *)reformParams:(NSDictionary *)params;
- (void)cleanData;
- (BOOL)shouldCache;

- (void)successedOnCallingAPI:(HLJURLResponse *)response;
- (void)failedOnCallingAPI:(HLJURLResponse *)response withErrorType:(HLJAPIManagerErrorType)errorType;
@end
