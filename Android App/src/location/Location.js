import React, {Component} from 'react';
import {connect} from 'react-redux';
import RNLocation from 'react-native-location';
import {currUpdate} from '../redux/actions/mapActions';
import {initUpdate} from '../redux/actions/initActions';
import {gpsUpdate, calcGain} from '../redux/actions/workoutActions';
import TabHeader from '../TabHeader.js';
import {NativeModules, NativeEventEmitter} from 'react-native';

class Location extends Component {
  constructor(props) {
    super(props);
    this.state = {};
  }
  async componentDidMount() {
    RNLocation.configure({
      distanceFilter: 0,
      interval: 1000, // Milliseconds
      fastestInterval: 500, // Milliseconds
      maxWaitTime: 2000, // Milliseconds
      desiredAccuracy: {
        ios: 'best',
        android: 'balancedPowerAccuracy',
      },
    });
    await RNLocation.requestPermission({
      ios: 'whenInUse',
      android: {
        detail: 'fine',
      },
    }).then(granted => {
      if (granted) {
        this.locationSubscription = RNLocation.subscribeToLocationUpdates(
          location => {
            if (
              !location.fromMockProvider &&
              Object.keys(location[0]).length !== 0 &&
              !this.props.navigation
            ) {
              // TODO: CHANGE TO NOT MOCK FOR PRODUCTION ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
              var lat = parseFloat(location[0].latitude);
              var long = parseFloat(location[0].longitude);

              this.props.currUpdate(lat, long);
            }
          },
        );
      }
    });
    RNLocation.getLatestLocation()
      .then(location => {
        if (!location.fromMockProvider && Object.keys(location).length !== 0) {
          // TODO: CHANGE TO NOT MOCK FOR PRODUCTION ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
          var lat = parseFloat(location.latitude);
          var long = parseFloat(location.longitude);
          this.props.currUpdate(lat, long);
          this.props.initUpdate(lat, long);
        }
      })
      .catch(error => console.log(error));

    NativeModules.KalmanFilter.init();
    const eventEmitter = new NativeEventEmitter(NativeModules.GPSDataLogger);
    eventEmitter.addListener('GPS', event => {
      this.props.gpsUpdate(event.speed, event.distance, event.alt);
    });
  }
  reset() {
    NativeModules.KalmanFilter.resetService();
  }

  render() {
    return <TabHeader sendMessageCallback={this.props.sendMessageCallback} />;
  }
}

const mapStateToProps = state => {
  // Redux Store --> Component
  return {
    current: state.mapReducer.current,
    workout: state.workoutReducer,
    navigation: state.mapReducer.navigation,
    init: state.initReducer.init,
  };
};
// Map Dispatch To Props (Dispatch Actions To Reducers. Reducers Then Modify The Data And Assign It To Your Props)
const mapDispatchToProps = dispatch => {
  return {
    currUpdate: (latitude, longitude) => {
      dispatch(currUpdate(latitude, longitude));
    },
    gpsUpdate: (speed, distance, altitude) => {
      dispatch(gpsUpdate(speed, distance, altitude));
      dispatch(calcGain());
    },
    initUpdate: (latitude, longitude) => {
      dispatch(initUpdate(latitude, longitude));
    },
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps,
)(Location);
