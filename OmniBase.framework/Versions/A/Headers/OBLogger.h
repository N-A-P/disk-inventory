// Copyright 2013-2020 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.

#import <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

typedef void(^OBLogFileHandler)(NSURL *url);

/**
 This is a utility class for doing user-preference and environment-variable controlled logging with customizable levels. Log messages are written to the console log and, optionally, to files in iOS (where the console log can be truncated). Typical interaction with the class is via the OBLoggerInitializeLogLevel() function and by the OBLog() function, perhaps wrapping the later in a custom macro for a particular logger instance.
 
 SWIFT: Go see OBLogger.swift. It adds a swifty veneer.
 */
@interface OBLogger : NSObject
@property (nonatomic, readonly) NSInteger level;
@property (nullable, nonatomic, readonly) NSString *suiteName;
@property (nonatomic, readonly) NSString *key;
/// `name` accessor provided as backwards compatibility; cover for now preferred `key` property.
@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) BOOL shouldLogToFile;

/**
 Convenience provided as backwards compatible cover. Calls through to `-[OBLogger initWithSuiteName:key:shouldLogToFile:]` with an empty `suiteName` and uses `name` parameter for the `key`.
 */
- (id)initWithName:(NSString *)name shouldLogToFile:(BOOL)shouldLogToFile; // We support logging to files for iOS only, where the console is truncated by the system.

/**
 Initializes the logger according to user preferences or environment variables.
 
 If an environment variable matching the key parameter exists and has a numeric value greater than 0, then that is used as the threshold for messages to be logged by the logger. If such an environment variable exists and has a non-numeric value or has the value 0, then nil is returned. Otherwise, if a user preference matching the key parameter exists, it is used similarly. An optional suiteName can be provided to retrieve the current value from a specific database, rather than the shared default. The latter is used in cases where no suiteName is provided by the caller.
 Messages sent to the logger---via OBLog()---that are at the preference level or higher are logged. Messages with a lower level are ignored.
 If the log level is set at 0, then before returning nil, this method removes any existing log files for 'key'. This allows the logger to clean up after itself when logging is turned off.
 \param suiteName specifies the user defaults database to use; otherwise the shared default is used.
 \param key identifies the logger, also used as the environment variable or user default for setting the logging level, and in the file name for any logs persisted to disk.
 \param shouldLogToFile if YES, then log to files in addition to console.
 */
- (id)initWithSuiteName:(nullable NSString *)suiteName key:(NSString *)key shouldLogToFile:(BOOL)shouldLogToFile; // We support logging to files for iOS only, where the console is truncated by the system.

- (void)log:(NSString *)format arguments:(va_list)args;

/**
 A convenience method for processing log files. The handler will be invoked with every current log file for this logger.
 */
- (void)processLogFilesWithHandler:(OBLogFileHandler)handler;
@end

/**
 Log the given format string and arguments to the given logger at the given message priority level.
 \param logger may be nil, in which case the function returns immediately, useful when preferences specify no logging
 \param messageLevel priority level for the current message. Higher levels are more likely to be logged. Level 0 is never logged, so you probably want to start at 1. 
 \note Log levels operate the opposite of Unix verbosity. 1 is "all" and higher numbers in the user default filter out all but the most severe messages.
 \param format a format string, as in -[NSString initWithFormat:]
 \param ... additional arguments to format string
 */
extern void OBLog(OBLogger * _Nullable logger, NSInteger messageLevel, NSString *format, ...) NS_FORMAT_FUNCTION(3,4);

/// Log just the given message, without any format arguments. (Useful for Swift compatibility.)
extern void OBLogSwiftVariadicCover(OBLogger * _Nullable logger, NSInteger messageLevel, NSString *message);

/// Helper for initializing log level globals. Invokes -[OBLogger initWithName:shouldLogToFile] and sets outLogger to point to the result.
extern void _OBLoggerInitializeLogLevel(OBLogger * __strong _Nullable * _Nonnull outLogger, NSString *name, BOOL useFile);
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OBLoggerInitializeLogLevel(name) _OBLoggerInitializeLogLevel(&name, @#name, YES)
#else
#define OBLoggerInitializeLogLevel(name) _OBLoggerInitializeLogLevel(&name, @#name, NO)
#endif

NS_ASSUME_NONNULL_END

