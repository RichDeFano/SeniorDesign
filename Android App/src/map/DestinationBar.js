import React, {Component} from 'react';
import {Text, StyleSheet, TouchableOpacity} from 'react-native';
import {connect} from 'react-redux';
import DestinationBubble from './bubbles/DestinationBubble';
import Icon from 'react-native-vector-icons/MaterialIcons';

class DestinationBar extends Component {
  constructor(props) {
    super(props);
    this.state = {};
  }

  render() {
    return (
      <DestinationBubble style={styles.container}>
        <Text style={styles.text}>{this.props.duration} min</Text>
        <Text style={styles.text}>{this.props.distance} mi</Text>
        <TouchableOpacity
          style={styles.navigation}
          onPress={this.props.launchNavigation}>
          <Text style={(styles.text, styles.gotext)}>Go</Text>
          <Icon
            style={styles.icon}
            name={'directions'}
            size={30}
            color={'#ffffff'}
          />
        </TouchableOpacity>
      </DestinationBubble>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    padding: 0,
    margin: 0,
    width: '75%',
  },
  text: {
    paddingLeft: 10,
    paddingRight: 10,
    fontSize: 15,
  },
  gotext: {
    color: 'white',
  },
  icon: {
    paddingLeft: 7,
    paddingRight: 0,
  },
  navigation: {
    paddingLeft: 10,
    paddingRight: 10,
    justifyContent: 'center',
    alignItems: 'center',
    flexDirection: 'row',
    backgroundColor: '#4285F4',
    borderRadius: 10,
  },
});

const mapStateToProps = state => {
  // Redux Store --> Component
  return {
    destination: state.mapReducer.destination,
  };
};

export default connect(mapStateToProps)(DestinationBar);
