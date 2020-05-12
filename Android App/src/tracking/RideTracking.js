import React, {Component} from 'react';
import {View, Text, StyleSheet, TouchableOpacity} from 'react-native';
import {connect} from 'react-redux';
import {NativeModules} from 'react-native';
import Icon from 'react-native-vector-icons/MaterialIcons';

import {
  workoutStarted,
  workoutEnded,
  incDuration,
  reset,
} from '../redux/actions/workoutActions';

class RideTracking extends Component {
  constructor(props) {
    super(props);
    this.state = {
      timer: null,
    };
  }

  componentDidMount() {
    this.props.screenProps.callback({
      ride_tracking: {
        running: false,
      },
    });
  }

  componentWillUnmount() {
    clearInterval(this.state.timer);
  }
  toggleWorkout = () => {
    if (!this.props.workout.started) {
      NativeModules.KalmanFilter.startService();
      this.props.toggleWorkout(this.props.workout.started);
      let timr = setInterval(this.tick, 1000);
      this.setState({timer: timr});
    } else {
      this.props.toggleWorkout(this.props.workout.started);
      clearInterval(this.state.timer);
      NativeModules.KalmanFilter.endService();
      this.props.screenProps.callback({
        ride_tracking: {
          running: false,
        },
      });
    }
  };

  tick = () => {
    this.props.incDuration();

    var gain = Math.round(this.props.workout.gain + this.props.workout.loss);
    var measuredTime = new Date(null);
    measuredTime.setSeconds(this.props.workout.duration || 0);
    var Time = measuredTime.toISOString().substr(11, 8);
    if (Time.substr(0, 3) === '00:') {
      Time = Time.substr(3, 5);
    }

    this.props.screenProps.callback({
      ride_tracking: {
        distance: this.props.workout.distance.toFixed(1) + ' mi',
        speed: Math.round(this.props.workout.speed) + ' mph',
        avg_speed: 'avg ' + Math.round(this.props.workout.avgSpeed) + ' mph',
        gain: gain + ' ft',
        time_ride: Time,
      },
    });
  };

  resetWorkout = () => {
    if (this.props.workout.started) {
      this.toggleWorkout();
    }
    this.props.reset();
  };

  render() {
    if (this.props.toggle) {
      console.log("toggled");
      this.toggleWorkout;
      this.props.toggleCallback();
    }
    var measuredTime = new Date(null);
    measuredTime.setSeconds(this.props.workout.duration || 0);
    var Time = measuredTime.toISOString().substr(11, 8);
    if (Time.substr(0, 3) === '00:') {
      Time = Time.substr(3, 5);
    }

    return (
      <View style={styles.container}>
        <View style={styles.row}>
          <View style={styles.box}>
            <Text style={styles.boxText}>
              {this.props.workout.distance.toFixed(1) || 0.0}
            </Text>
            <Text style={styles.labelText}>MI</Text>
          </View>
          <View style={styles.box}>
            <Text style={styles.boxText}>
              {this.props.workout.speed.toFixed(0) || 0.0}
            </Text>
            <Text style={styles.labelText}>MPH</Text>
          </View>
        </View>
        <View style={styles.row}>
          <View style={styles.box}>
            <Text style={styles.boxText}>{Time}</Text>
            <Text style={styles.labelText}>DURATION</Text>
          </View>
          <View style={styles.box}>
            <Text style={styles.boxText}>
              {this.props.workout.avgSpeed.toFixed(0) || 0}
            </Text>
            <Text style={styles.labelText}>AVG SPEED</Text>
          </View>
        </View>
        <View style={styles.row}>
          <View style={styles.alt_box}>
            <Text style={styles.boxText}>
              {Math.round(this.props.workout.gain + this.props.workout.loss) ===
                0 && '0'}
              {Math.round(this.props.workout.gain + this.props.workout.loss) !==
                0 &&
                (this.props.workout.gain + this.props.workout.loss).toFixed(0)}
            </Text>
            <Text style={styles.labelText}>ALT GAIN</Text>
          </View>
          <View style={styles.box}>
            <View style={styles.row}>
              <Icon name={'arrow-downward'} size={30} />
              <Text style={styles.boxText}>
                {(!!this.props.workout.altMin &&
                  this.props.workout.altMin.toFixed(0)) ||
                  '-'}
              </Text>
            </View>
            <View style={styles.row}>
              <Icon name={'arrow-upward'} size={30} />
              <Text style={styles.boxText}>
                {(!!this.props.workout.altMax &&
                  this.props.workout.altMax.toFixed(0)) ||
                  '-'}
              </Text>
            </View>
          </View>
        </View>
        <View style={styles.buttonBox}>
          <TouchableOpacity
            style={styles.reset_button}
            onPress={this.resetWorkout}>
            <Text style={styles.reset_text}>Reset</Text>
          </TouchableOpacity>
          {this.props.workout.started && (
            <TouchableOpacity
              style={styles.stop_button}
              onPress={this.toggleWorkout}>
              <Text style={styles.buttonText}>Stop Workout</Text>
            </TouchableOpacity>
          )}
          {!this.props.workout.started && (
            <TouchableOpacity
              style={styles.start_button}
              onPress={this.toggleWorkout}>
              <Text style={styles.buttonText}>Start Workout</Text>
            </TouchableOpacity>
          )}
          <View style={{flex: 1, paddingRight: 10, paddingLeft: 10}} />
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  row: {
    flex: 2,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
  },
  box: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  alt_box: {
    flex: 2,
    justifyContent: 'center',
    alignItems: 'center',
  },
  buttonBox: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'space-around',
    alignItems: 'center',
  },
  start_button: {
    flex: 3,
    alignItems: 'center',
    justifyContent: 'center',
    height: '35%',
    backgroundColor: 'green',
    borderRadius: 10,
  },
  stop_button: {
    flex: 3,
    alignItems: 'center',
    justifyContent: 'center',
    width: '50%',
    height: '35%',
    backgroundColor: 'red',
    borderRadius: 10,
  },
  reset_button: {
    flex: 1,
    paddingLeft: 10,
    paddingRight: 10,
  },
  reset_text: {
    paddingLeft: 15,
    fontWeight: 'bold',
  },
  buttonText: {
    fontSize: 20,
    color: 'white',
    fontWeight: 'bold',
  },
  boxText: {
    fontSize: 30,
    fontWeight: 'bold',
  },
  labelText: {
    fontSize: 18,
    //fontWeight: 'bold',
  },
});

const mapStateToProps = state => {
  // Redux Store --> Component
  return {
    workout: state.workoutReducer,
  };
};
// Map Dispatch To Props (Dispatch Actions To Reducers. Reducers Then Modify The Data And Assign It To Your Props)
const mapDispatchToProps = dispatch => {
  return {
    toggleWorkout: workout => {
      if (workout) {
        dispatch(workoutEnded());
      } else {
        dispatch(workoutStarted());
      }
    },
    incDuration: duration => {
      dispatch(incDuration(duration));
    },
    reset: () => {
      dispatch(reset());
    },
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps,
)(RideTracking);
