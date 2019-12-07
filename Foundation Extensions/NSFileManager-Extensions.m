//
//  NSFileManager-Extensions.m
//  Disk Inventory X
//
//  Created by Doom on 08.11.19.
//

#import "NSFileManager-Extensions.h"

@implementation NSFileManager(PrivacyProtectedFolders)

// see https://developer.apple.com/documentation/bundleresources/information_property_list/protected_resources?language=objc
// for the list of protected resources
- (NSArray<NSURL*>*) localPrivacyProtectedFolders
{
    NSMutableArray<NSURL*> *protectedURLs = [NSMutableArray array];
    
    // add folders which can be identified by a NSSearchPathDirectory constant directly
    {
        NSSearchPathDirectory searchDirs[] = {NSDocumentDirectory,
                                                NSDesktopDirectory,
                                                NSDownloadsDirectory,
                                                /*NSPicturesDirectory*/};
        
        for (int i = 0; i < sizeof(searchDirs)/sizeof(searchDirs[0]); i++)
        {
            NSURL *url = [self URLForDirectory:searchDirs[i]
                             inDomain:NSUserDomainMask
                    appropriateForURL:nil
                               create:NO
                                error:nil];
            
            if ( url != nil )
                [protectedURLs addObject: url];
        }
    }
    
    // photo libraries: all .photoslibrary folders in ~/Pictures
    {
        // get ~/Pictures ...
        NSURL *url = [self URLForDirectory:NSPicturesDirectory
                                 inDomain:NSUserDomainMask
                        appropriateForURL:nil
                                   create:NO
                                    error:nil];
        
        if ( url != nil )
        {
            // .. and list all .photoslibrary folders (uniform type identifier: "com.apple.photos.library")
            NSDirectoryEnumerator *dirEnum = [self enumeratorAtURL:url
                                       includingPropertiesForKeys:[NSArray arrayWithObject:NSURLTypeIdentifierKey]
                                                          options:NSDirectoryEnumerationSkipsSubdirectoryDescendants
                                                     errorHandler:^(NSURL *url, NSError *error)
                                                                 {
                                                                     // Handle the error.
                                                                     // Return YES if the enumeration should continue after the error.
                                                                     NSLog(@"error: %@", error);
                                                                     return YES;
                                                                 }
                                            ];
            
            for ( url in dirEnum )
            {
                NSString *kind = nil;
                [url getResourceValue:&kind forKey:NSURLTypeIdentifierKey error:nil];
                
                if ( kind != nil && [kind isEqualToString:@"com.apple.photos.library"])
                    [protectedURLs addObject:url];
            }
        }
    }
    
    // calendars and reminders: ~/Library/Calendars and ~/Library/Reminders (10.15+)
    {
        NSURL *libUrl = [self URLForDirectory:NSLibraryDirectory
                                  inDomain:NSUserDomainMask
                         appropriateForURL:nil
                                    create:NO
                                     error:nil];
        
        if ( libUrl != nil )
        {
            NSURL *url = [libUrl URLByAppendingPathComponent: @"Calendars"];
        
            [protectedURLs addObject: url];
            
            url = [libUrl URLByAppendingPathComponent: @"Reminders"];
            
            [protectedURLs addObject: url];
        }

    }

    // addressbook: ~/Library/Application Support/AddressBook
    {
        NSURL *url = [self URLForDirectory:NSApplicationSupportDirectory
                                  inDomain:NSUserDomainMask
                         appropriateForURL:nil
                                    create:NO
                                     error:nil];
        
        if ( url != nil )
        {
            url = [url URLByAppendingPathComponent: @"AddressBook"];
        
            [protectedURLs addObject: url];
        }
    }

    return protectedURLs;
}

// list of folders and files below the specified URL which are under macOS' privacy protection
- (NSArray<NSURL*>*) privacyProtectedFoldersInURL: (NSURL *)url
{
    NSArray<NSURL*> *allURLs = [self localPrivacyProtectedFolders];
    NSMutableArray<NSURL*> *relevantURLs = [NSMutableArray array];
    for ( NSURL *protectedURL in allURLs )
    {
        NSURLRelationship urlRelation;
        if ( [self getRelationship:&urlRelation ofDirectoryAtURL:url toItemAtURL:protectedURL error:nil]
            && (urlRelation == NSURLRelationshipSame || urlRelation == NSURLRelationshipContains) )
        {
            [relevantURLs addObject:protectedURL];
        }
    }
    
    return relevantURLs;
}

// access the protected URLs to trigger macOS' consent dialogs
- (void) triggerConsentDialogForPrivacyProtectedFolders: (NSArray<NSURL*>*) urls
{
    for ( NSURL *protectedURL in urls )
     {
         NSError *err = nil;
         NSDictionary<NSFileAttributeKey, id> *attribs = [self attributesOfItemAtPath:[protectedURL path] error:&err];
         
         // sometimes the consent dialog is not shown already when getting the directories attributes,
         // but when getting the folder content; so do that, too
         NSNumber *isDir = nil;
         [protectedURL getResourceValue:&isDir forKey:NSURLIsDirectoryKey error:nil];
         
         if ( attribs == nil )
         {
             NSLog(@"cannot access '%@': %@", [protectedURL path], err);
         }
         else if ( isDir != nil && [isDir boolValue] )
         {
             [self contentsAtPath:[protectedURL path]];
         }
     }
}

// access the protected URLs residing below "url" to trigger macOS' consent dialogs
- (void) triggerCosentDialogForPrivacyProtectedFoldersInURL: (NSURL *)url
{
    NSArray<NSURL*> *protectedURLs = [self privacyProtectedFoldersInURL: url];
    [self triggerConsentDialogForPrivacyProtectedFolders:protectedURLs];
}

@end
