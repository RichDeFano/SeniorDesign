const initialState = {
  distance: 0.0,
  duration: 0,
  speed: 0.0,
  avgSpeed: 0.0,
  gain: 0.0,
  loss: 0.0,
  alt: null,
  altMin: null,
  altMax: null,
  prev_alt: null,
  started: false,
  reset: false,
};
// Reducers (Modifies The State And Returns A New State)
const workoutReducer = (state = initialState, action) => {
  switch (action.type) {
    case 'gpsUpdate': {
      return {
        // State
        ...state,
        distance: action.distance,
        speed: action.speed,
        alt: action.alt,
        prev_alt: state.alt,
        avgSpeed: action.distance / (state.duration / 3600) || 0.0,
      };
    }
    case 'workoutStarted': {
      return {
        ...state,
        started: true,
      };
    }
    case 'workoutEnded': {
      return {
        ...state,
        started: false,
      };
    }
    case 'incDuration': {
      return {
        ...state,
        duration: state.duration + 1,
      };
    }
    case 'calcGain': {
      var options = {};
      if (state.altMax < state.alt || state.altMax === null) {
        options = {
          ...options,
          altMax: state.alt,
        };
      }
      if (state.altMin > state.alt || state.altMin === null) {
        options = {
          ...options,
          altMin: state.alt,
        };
      }
      if (state.prev_alt !== null) {
        var calc = state.alt - state.prev_alt;
        if (calc > 0) {
          options = {
            ...options,
            gain: state.gain + calc,
          };
        } else if (calc < 0) {
          options = {
            ...options,
            loss: state.loss + calc,
          };
        }
      }
      return Object.assign(state, options);
    }
    case 'resetWorkout': {
      return {
        distance: 0.0,
        duration: 0,
        speed: 0.0,
        avgSpeed: 0.0,
        gain: 0.0,
        loss: 0.0,
        alt: null,
        altMin: null,
        altMax: null,
        prev_alt: null,
        started: false,
        reset: true,
      };
    }
    // Default
    default: {
      return state;
    }
  }
};
// Exports
export default workoutReducer;
