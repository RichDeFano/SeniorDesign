// Imports: Dependencies
import {combineReducers} from 'redux';
// Imports: Reducers
import musicReducer from './musicReducer';
import mapReducer from './mapReducer';
import bluetoothReducer from './bluetoothReducer';
import workoutReducer from './workoutReducer';
import initReducer from './initReducer';

// Redux: Root Reducer
const rootReducer = combineReducers({
  musicReducer: musicReducer,
  mapReducer: mapReducer,
  bluetoothReducer: bluetoothReducer,
  workoutReducer: workoutReducer,
  initReducer: initReducer,
});
// Exports
export default rootReducer;
