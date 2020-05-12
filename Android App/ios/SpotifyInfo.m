//
//  SpotifyInfo.m
//  Bike_App
//
//  Created by Dylan Gardner on 2/13/20.
//  Copyright © 2020 Facebook. All rights reserved.
//

#import "SpotifyInfo.h"
#import <React/RCTLog.h>

@implementation SpotifyInfo
@synthesize appRemote;

  RCT_EXPORT_MODULE()

  - (NSArray<NSString *> *)supportedEvents
  {
    return @[@"SongUpdate", @"PosUpdate"];
  }

  RCT_EXPORT_METHOD(startService)
  {
    NSLog(@"Music Service started");
    SPTConfiguration *configuration = [[SPTConfiguration alloc] initWithClientID:@"3ec3064e4dfc4b78b14b72ad3e914c2c" redirectURL:[NSURL URLWithString:@"spotify-ios-quick-start://spotify-login-callback"]];
    
    self.appRemote = [[SPTAppRemote alloc] initWithConfiguration:configuration logLevel:SPTAppRemoteLogLevelDebug];
    BOOL spotifyInstalled = false;
    dispatch_async(dispatch_get_main_queue(), ^{
      spotifyInstalled = [self.appRemote authorizeAndPlayURI:@""];
    });
    
    NSLog(@"%@", spotifyInstalled ? @"YES" : @"NO");
    if (!spotifyInstalled) {
        RCTLog(@"Not Installed");
        /*
        * The Spotify app is not installed.
        * Use SKStoreProductViewController with [SPTAppRemote spotifyItunesItemIdentifier] to present the user
        * with a way to install the Spotify app.
        */
    }
    else{
       self.appRemote.delegate = self;
       [self.appRemote connect];
    }
    
  }

  - (void)appRemoteDidEstablishConnection:(SPTAppRemote *)appRemote
  {
      // Connection was successful, you can begin issuing commands
      NSLog(@"Established Connection");
      appRemote.playerAPI.delegate = self;

      [appRemote.playerAPI subscribeToPlayerState:^(id  _Nullable result, NSError * _Nullable error) {
          // Handle Errors
        NSLog(@"%@",error.description);
      }];
  }

  - (void)appRemote:(SPTAppRemote *)appRemote didFailConnectionAttemptWithError:(NSError *)error
  {
      // Connection failed
    NSLog(@"%@",error.description);
  }

  - (void)appRemote:(SPTAppRemote *)appRemote didDisconnectWithError:(nullable NSError *)error
  {
      // Connection disconnected
    NSLog(@"%@",error.description);
  }

  - (void)playerStateDidChange:(id<SPTAppRemotePlayerState>)playerState
  {
      NSLog(@"Track name: %@", playerState.track.name);
  }


@end
