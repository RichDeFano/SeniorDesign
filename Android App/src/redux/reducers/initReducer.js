const initialState = {
  init: {
    longitude: null,
    latitude: null,
  },
};
// Reducers (Modifies The State And Returns A New State)
const initReducer = (state = initialState, action) => {
  switch (action.type) {
    case 'InitUpdate': {
      return {
        init: {
          longitude: action.longitude,
          latitude: action.latitude,
        },
      };
    }
    // Default
    default: {
      return state;
    }
  }
};
// Exports
export default initReducer;
