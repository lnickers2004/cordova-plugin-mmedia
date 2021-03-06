//
//  SKGenericCommand.h
//  SpeechKit
//
// Copyright 2012, Nuance Communications Inc. All rights reserved.
//
// Nuance Communications, Inc. provides this document without representation 
// or warranty of any kind. The information in this document is subject to 
// change without notice and does not represent a commitment by Nuance 
// Communications, Inc. The software and/or databases described in this 
// document are furnished under a license agreement and may be used or 
// copied only in accordance with the terms of such license agreement.  
// Without limiting the rights under copyright reserved herein, and except 
// as permitted by such license agreement, no part of this document may be 
// reproduced or transmitted in any form or by any means, including, without 
// limitation, electronic, mechanical, photocopying, recording, or otherwise, 
// or transferred to information storage and retrieval systems, without the 
// prior written permission of Nuance Communications, Inc.
// 
// Nuance, the Nuance logo, Nuance Recognizer, and Nuance Vocalizer are 
// trademarks or registered trademarks of Nuance Communications, Inc. or its 
// affiliates in the United States and/or other countries. All other 
// trademarks referenced herein are the property of their respective owners.
//

#import <Foundation/Foundation.h>
#import <SpeechKit/SKGenericResult.h>


@protocol SKGenericCommandDelegate;


/*!
 @discussion The SKGenericCommand class manages a single command process 
 including log revision, reset user profile, data upload, etc. SKGenericCommand 
 is designed to carry out a single command so after receiving the response the 
 class should be released.  Subsequent results should each be generated by 
 instantiating a new SKGenericCommand instance.
 */
@interface SKGenericCommand : NSObject
{
    id<SKGenericCommandDelegate> delegate;
}

@property (nonatomic, assign) id <SKGenericCommandDelegate> delegate;

/*!
 @abstract Returns a reset user profile command and begins the command. This is
 to be invoked to delete all records related to a specific user id
 
 @param delegate The receiver for command responses.  The delegate must 
 implement the SKGenericCommandDelegate protocol and will receive a message when the 
 command has completed.
 @result A command object corresponding to the command request.
 */
- (id)initWithResetUserProfileCmd:(id <SKGenericCommandDelegate>)aDelegate;

/*!
 @abstract Returns a log revision command and begins the command. This is 
 to send post-recognition meta-data to the server, to support model training 
 performed periodically on the modeling grid. 
 
 @param event The "event" key is used to identify what type or revision is being
 logged and therefore what keys needs to be extracted from the "content" dictionary.
 
 @param appSessionId This is the application session unique identifier. It could be
 tied to multiple network sessions.

 @param logContent Log content sent to server, the log content is of the following format:
	dict: {
		"event" : < utf8> "event_name"
		"content": 
		{
			"key1" : val1,
			"key2" " val2, ... etc
		}
	}
 
 @param delegate The receiver for command responses.  The delegate must 
 implement the SKGenericCommandDelegate protocol and will receive a message when the 
 command has completed.
 @result A command object corresponding to the command request.
 */
- (id)initWithLogRevisionCmd:(NSString *)event appId:(NSString *)appSessionId logContent:(NSObject *)logContent delegate:(id <SKGenericCommandDelegate>)aDelegate;

/*!
 @abstract Cancels the command request.
 
 @discussion This method will terminate the command request.  This will result 
 in the delegate receiving an error message via the 
 genericCommand:didFinishWithError:suggestion method unless a command result 
 has been or is already being sent to the delegate.
 */
- (void)cancel;

@end


/*!
 @discussion The SKGenericCommandDelegate protocol defines the messages sent to a 
 delegate of the SKGenericCommand class.  These delegate methods indicate the flow 
 of the command process.  The receiver will be notified when the command is finished
 or has error.
 */
@protocol SKGenericCommandDelegate <NSObject>

@required
/*!
 @abstract Sent when the command completes successfully.
 
 @param command The command sending the request
 @param results The SKGenericResult object containing the command results.
 
 @discussion This method is only called when the command completes 
 successfully.  The results object contains one string result.
 */
- (void)genericCommand:(SKGenericCommand *)command didFinishWithResults:(SKGenericResult *)results;

/*!
 @abstract Sent when the command completes with an error.
 
 @param command The command sending the request
 @param error The command error. Possible numeric values for the 
 SKSpeechErrorDomain are listed in SpeechKitError.h and a text description is
 available via the localizedDescription method.
 @param suggestion This is a suggestion to the user about how he or she can 
 improve the command process.
 
 @discussion This method is called when the command results in an 
 error due to any number of circumstances. The server connection may be disrupted 
 or a parameter specified during initialization, such as language or authentication 
 information was invalid.
 */
- (void)genericCommand:(SKGenericCommand *)command didFinishWithError:(NSError *)error suggestion:(NSString *)suggestion;

@end
