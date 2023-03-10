// Copyright 2006-2021 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.

#import <Foundation/NSFileWrapper.h>
#import <OmniBase/OBUtilities.h>

@class UTType;

#if (!defined(TARGET_OS_IPHONE) || !TARGET_OS_IPHONE)
    #import <AppKit/NSFileWrapperExtensions.h>
#endif

@interface NSFileWrapper (OAExtensions)
+ (NSFileWrapper *)fileWrapperWithFilename:(NSString *)filename contents:(NSData *)data;
@property(nonatomic,readonly) UTType *fileType;
- (NSString *)fileTypeIdentifier:(BOOL *)isHFSType OB_DEPRECATED_ATTRIBUTE;
- (void)addFileWrapperMovingAsidePreviousWrapper:(NSFileWrapper *)wrapper;
@end
