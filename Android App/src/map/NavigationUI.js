import React, {Component} from 'react';
import {Platform, StyleSheet, View, PermissionsAndroid} from 'react-native';
import NavigationView from '../../NavigationView';
import {NativeModules} from 'react-native';
import {connect} from 'react-redux';

class NavigationUI extends Component {
  state = {
    granted: Platform.OS === 'ios',
    fromLat: 34.028092,
    fromLong: -118.484868,
    toLat: 34.019444,
    toLong: -118.409259,
  };

  componentDidMount() {
    if (!this.state.granted) {
      this.requestFineLocationPermission();
    }
  }

  async requestFineLocationPermission() {
    try {
      const granted = await PermissionsAndroid.request(
        PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
        {
          title: 'ACCESS_FINE_LOCATION',
          message: 'Mapbox navigation needs ACCESS_FINE_LOCATION',
        },
      );
      if (granted === PermissionsAndroid.RESULTS.GRANTED) {
        this.setState({granted: true});
      } else {
        console.log('ACCESS_FINE_LOCATION permission denied');
      }
    } catch (err) {
      console.warn(err);
    }
  }

  render() {
    const {granted, fromLat, fromLong, toLat, toLong} = this.state;
    return (
      <View style={styles.container}>
        <NavigationView
          style={styles.navigation}
          destination={{
            lat: this.props.destination.latitude,
            long: this.props.destination.longitude,
          }}
          origin={{
            lat: this.props.current.latitude,
            long: this.props.current.longitude,
          }}
        />
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'stretch',
    backgroundColor: 'whitesmoke',
  },
  subcontainer: {
    flex: 1,
    justifyContent: 'center',
    backgroundColor: 'whitesmoke',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  navigation: {
    backgroundColor: 'gainsboro',
    flex: 1,
  },
});

const mapStateToProps = state => {
  // Redux Store --> Component
  return {
    current: state.mapReducer.current,
    destination: state.mapReducer.destination,
    view: state.mapReducer.view,
  };
};

export default connect(mapStateToProps)(NavigationUI);
