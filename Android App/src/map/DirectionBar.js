import React, {Component} from 'react';
import {
  View,
  Text,
  StyleSheet,
  Dimensions,
  TouchableOpacity,
} from 'react-native';
import RNGooglePlaces from 'react-native-google-places';
import {connect} from 'react-redux';
import {destUpdate} from '../redux/actions/mapActions';
import NavigationBubble from './bubbles/NavigationBubble';
import Icon from 'react-native-vector-icons/MaterialIcons';

var {height, width} = Dimensions.get('window');

class DirectionBar extends Component {
  constructor(props) {
    super(props);
  }
  openSearchModal() {
    RNGooglePlaces.openAutocompleteModal({
      initialQuery: this.props.destination.address,
    })
      .then(place => {
        var pieces = place.address.split(',');
        pieces.length = pieces.length - 1;
        var address = pieces.join(',');
        address = address.split(' ');
        address.length = address.length - 1;
        address = address.join(' ');
        console.log(place.location);
        this.props.destUpdate(
          place.location.latitude,
          place.location.longitude,
          address,
        );
        this.props.destCallback();
      })
      .catch(error => console.log(error.message)); // error is a Javascript Error object
  }

  clearAddress() {
    this.props.destUpdate(null, null, null);
    this.props.clearCallback();
  }
  render() {
    return (
      <NavigationBubble style={styles.bubble}>
        <View style={styles.bar}>
          <TouchableOpacity
            style={styles.input}
            onPress={() => this.openSearchModal()}>
            {this.props.destination.address == null && <Text>Search</Text>}
            {this.props.destination.address != null && (
              <Text> {this.props.destination.address} </Text>
            )}
          </TouchableOpacity>
          <TouchableOpacity
            style={styles.buttons}
            onPress={() => this.clearAddress()}>
            <Icon name={'clear'} size={20} />
          </TouchableOpacity>
        </View>
      </NavigationBubble>
    );
  }
}

const styles = StyleSheet.create({
  bubble: {
    justifyContent: 'center',
    flexDirection: 'row',
    backgroundColor: '#4285F4',
  },
  bar: {
    justifyContent: 'flex-end',
    flexDirection: 'row',
    alignItems: 'center',
    backgroundColor: 'white',
    borderRadius: 8,
    borderWidth: 1,
  },
  input: {
    height: 40,
    width: width - 85,
    alignItems: 'flex-start',
    paddingLeft: 8,
    justifyContent: 'center',
    borderBottomLeftRadius: 8,
    borderTopLeftRadius: 8,
  },
  buttons: {
    height: 40,
    width: 45,
    alignItems: 'center',
    justifyContent: 'center',
    borderBottomRightRadius: 8,
    borderTopRightRadius: 8,
  },
});

const mapStateToProps = state => {
  return {
    destination: state.mapReducer.destination,
  };
};
const mapDispatchToProps = dispatch => {
  return {
    destUpdate: (latitude, longitude, address) => {
      dispatch(destUpdate(latitude, longitude, address));
    },
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps,
)(DirectionBar);
