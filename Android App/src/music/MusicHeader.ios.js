import React, {Component} from 'react';
import {connect} from 'react-redux';

import {songUpdate} from '../redux/actions/musicActions';
import Location from '../location/Location.js';

import {
  auth as SpotifyAuth,
  remote as SpotifyRemote,
  ApiScope,
  remote,
} from 'react-native-spotify-remote';

// Api Config object, replace with your own applications client id and urls
const spotifyConfig = {
  clientID: '3ec3064e4dfc4b78b14b72ad3e914c2c',
  redirectURL: 'spotify-ios-quick-start://spotify-login-callback',
  tokenRefreshURL: 'http://192.168.0.240:3000/refresh',
  tokenSwapURL: 'http://192.168.0.240:3000/swap',
  scope: ApiScope.AppRemoteControlScope | ApiScope.StreamingScope,
};

class MusicHeader extends Component {
  constructor(props) {
    super(props);
  }

  async componentDidMount() {
    try {
      const token = await SpotifyAuth.initialize(spotifyConfig);
      await SpotifyRemote.connect(token);
    } catch (err) {
      console.error("Couldn't authorize with or connect to Spotify", err);
    }
    this.interval = setInterval(() => {
      try {
        remote.getPlayerState().then(player => {
          this.props.songUpdate(
            player.track.name,
            player.track.artist.name,
            player.track.duration,
            player.isPaused,
            player.playbackPosition,
          );
        });
      } catch (err) {
        console.error("Couldn't get info from Spotify", err);
      }
    }, 1000);
  }

  componentWillUnmount() {
    clearInterval(this.interval);
  }

  render() {
    return <Location />;
  }
}

// Map State To Props (Redux Store Passes State To Component)
const mapStateToProps = state => {
  // Redux Store --> Component
  return {
    trackName: state.musicReducer.trackName,
    artistName: state.musicReducer.artistName,
    trackLength: state.musicReducer.trackLength,
    isPlaying: state.musicReducer.isPlaying,
    playbackPosition: state.musicReducer.playbackPosition,
  };
};
// Map Dispatch To Props (Dispatch Actions To Reducers. Reducers Then Modify The Data And Assign It To Your Props)
const mapDispatchToProps = dispatch => {
  // Action
  return {
    // Increase Counter
    songUpdate: (trackName, artistName, trackLength, isPaused, position) => {
      dispatch(
        songUpdate(trackName, artistName, trackLength, isPaused, position),
      );
    },
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps,
)(MusicHeader);
