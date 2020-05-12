import React, {Component} from 'react';
import {NativeModules, NativeEventEmitter} from 'react-native';
import {connect} from 'react-redux';

import {songUpdate} from '../redux/actions/musicActions';
import Location from '../location/Location.js';

var count = 0;

class MusicHeader extends Component {
  constructor(props) {
    super(props);
    this.state = {
      toggle: false,
    };
  }
  componentDidMount() {
    const {childRef} = this.props;
    childRef(this);
    NativeModules.SpotifyInfo.startService();
    const eventEmitter = new NativeEventEmitter(NativeModules.SpotifyInfo);
    eventEmitter.addListener('SongUpdate', event => {
      //console.log(event);
      this.props.songUpdate(
        event.track_name,
        event.artist_name,
        event.track_length,
        event.isPaused,
        event.position,
      );
      var dur = new Date(null);
      dur.setSeconds(this.props.trackLength / 1000 || 0);
      var durTime = dur.toISOString().substr(11, 8);
      if (durTime.substr(0, 3) === '00:') {
        durTime = durTime.substr(3, 5);
      }
      var pos = new Date(null);
      pos.setSeconds(this.props.playbackPosition / 1000 || 0);
      var posTime = pos.toISOString().substr(11, 8);
      if (posTime.substr(0, 3) === '00:') {
        posTime = posTime.substr(3, 5);
      }
      var progBar = Math.round(
        (this.props.playbackPosition / this.props.trackLength) * 180,
      );
      this.props.sendMessageCallback({
        music: {
          track: this.props.trackName,
          artist: this.props.artistName,
          track_length: durTime,
          playing: this.props.isPlaying,
          position: posTime,
          progressBar: 180 - progBar,
        },
      });
    });
  }

  componentWillUnmount() {
    const {childRef} = this.props;
    childRef(undefined);
  }

  skip() {
    NativeModules.SpotifyInfo.skip();
  }

  prev() {
    NativeModules.SpotifyInfo.prev();
  }

  playpause() {
    if (this.props.isPlaying) {
      NativeModules.SpotifyInfo.pause();
    } else {
      NativeModules.SpotifyInfo.resume();
    }
  }

  render() {
    return <Location sendMessageCallback={this.props.sendMessageCallback} />;
  }
}

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
    songUpdate: (
      trackName,
      artistName,
      trackLength,
      isPaused,
      playbackPosition,
    ) => {
      dispatch(
        songUpdate(
          trackName,
          artistName,
          trackLength,
          isPaused,
          playbackPosition,
        ),
      );
    },
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps,
)(MusicHeader);
