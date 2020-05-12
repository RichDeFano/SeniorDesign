//
//  SpotifyInfo.h
//  Bike_App
//
//  Created by Dylan Gardner on 2/13/20.
//  Copyright © 2020 Facebook. All rights reserved.
//

#import <React/RCTBridgeModule.h>
#import <SpotifyiOS/SpotifyiOS.h>
#import <React/RCTEventEmitter.h>


@interface SpotifyInfo : NSObject <RCTBridgeModule, SPTAppRemoteDelegate, SPTAppRemotePlayerStateDelegate>

@property SPTAppRemote *appRemote;

@end

