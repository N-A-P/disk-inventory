// Copyright 2006-2022 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.

#import <Foundation/NSExpression.h>

@class NSMutableSet;

@interface NSExpression (OFExtensions)
- (void)addReferencedKeys:(NSMutableSet *)keys;
@end
