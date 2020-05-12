// Initial State
const initialState = {
  trackName: 'NA',
  artistName: 'NA',
  isPlaying: false,
  trackLength: 0,
  playbackPosition: 0,
};
// Reducers (Modifies The State And Returns A New State)
const musicReducer = (state = initialState, action) => {
  switch (action.type) {
    case 'SongUpdate': {
      return {
        // State
        ...state,
        // Redux Store
        trackName: action.track,
        artistName: action.artist,
        trackLength: action.track_length,
        isPlaying: action.isPlaying,
        playbackPosition: action.playbackPosition,
      };
    }

    // Default
    default: {
      return state;
    }
  }
};
// Exports
export default musicReducer;
