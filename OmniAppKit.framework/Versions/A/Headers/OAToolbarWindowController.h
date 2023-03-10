// Copyright 2002-2021 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.

#import <AppKit/NSWindowController.h>
#import <AppKit/NSToolbarItem.h>
#import <OmniAppKit/OAToolbar.h>

NS_ASSUME_NONNULL_BEGIN

@class OAToolbar, OAToolbarItem;
@class NSBundle, NSDictionary;

/**
 Posted when we detect that an item will be added to or was removed from the toolbar and when the toolbar's visibility, display mode, or size mode have changed.
 */
extern NSString * const OAToolbarDidChangeNotification;
extern NSString * const OAToolbarDidChangeKindKey;

typedef NS_ENUM(NSUInteger, OAToolbarDidChangeKind) {
    OAToolbarDidChangeKindAddItem,
    OAToolbarDidChangeKindRemoveItem,
    OAToolbarDidChangeKindSetVisible,
    OAToolbarDidChangeKindSetDisplayMode,
    OAToolbarDidChangeKindSetSizeMode,
    OAToolbarDidChangeKindPresentCustomizationSheet,
};

@protocol OAToolbarHelper

// A path suffix that should be on the toolbar items returned from `allowedItems`
@property(nonatomic,readonly) NSString *itemIdentifierExtension;

// The identifier of a toolbar item identifier in a .toolbar file where the various labels can have '%@' as a placeholder for the item name. This template gets replaced by the items generated by -allowedItems.
@property(nonatomic,readonly) NSString *templateItemIdentifier;

// The list of item identifiers provided by this helper. The identifiers should be <name>.<extension>, where the name will be passed into the format strings in the template specified by templateItemIdentifier.
@property(nonatomic,readonly) NSArray<NSToolbarItemIdentifier> *allowedItems;

- (nullable OAToolbarItem *)finishSetupForToolbarItem:(OAToolbarItem *)item toolbar:(NSToolbar *)toolbar willBeInsertedIntoToolbar:(BOOL)willInsert;

@optional

@property(nonatomic,readonly) Class toolbarItemClass;

@property(nonatomic,readonly) Class toolbarItemButtonClass;

@end

@interface OAToolbarWindowController : NSWindowController <OAToolbarDelegate, NSToolbarItemValidation>

+ (void)registerToolbarHelper:(NSObject <OAToolbarHelper> *)helperObject;

@property(class,nonatomic,readonly) NSBundle *toolbarBundle;
@property(class,nonatomic,readonly) Class toolbarClass;
@property(class,nonatomic,readonly) Class toolbarItemClass;

@property(nonatomic,readonly) OAToolbar *toolbar;
- (void)createToolbar;
@property(nonatomic,readonly) BOOL isCreatingToolbar;
- (nullable NSDictionary *)toolbarInfoForItem:(NSToolbarItemIdentifier)identifier;
- (nullable NSDictionary *)localizedToolbarInfoForItem:(NSToolbarItemIdentifier)identifier;

// implement in subclasses to control toolbar
@property(nonatomic,readonly) NSString *toolbarConfigurationName; // file name to lookup .toolbar plist
@property(nonatomic,readonly) NSToolbarIdentifier toolbarIdentifier; // identifier used for preferences - defaults to configurationName if unimplemented
@property(nonatomic,readonly) BOOL shouldAllowUserToolbarCustomization;
@property(nonatomic,readonly) BOOL shouldAutosaveToolbarConfiguration;
@property(nonatomic,readonly) NSToolbarDisplayMode defaultToolbarDisplayMode;
@property(nonatomic,readonly) NSDictionary *toolbarConfigurationDictionary; // Used when -shouldAutosaveConfiguration returns NO

// declare that we implement these optional OAToolbarDelegate methods so subclasses that override them can (and must!) call super
- (void)toolbarWillAddItem:(NSNotification *)notification NS_REQUIRES_SUPER;
- (void)toolbarDidRemoveItem:(NSNotification *)notification NS_REQUIRES_SUPER;
- (void)toolbar:(OAToolbar *)aToolbar didSetVisible:(BOOL)visible NS_REQUIRES_SUPER;
- (void)toolbar:(OAToolbar *)aToolbar didSetDisplayMode:(NSToolbarDisplayMode)displayMode NS_REQUIRES_SUPER;
- (void)toolbar:(OAToolbar *)aToolbar didSetSizeMode:(NSToolbarSizeMode)sizeMode NS_REQUIRES_SUPER;


@end

NS_ASSUME_NONNULL_END
