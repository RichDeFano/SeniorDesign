import React, {Component} from 'react';
import {StyleSheet, Text, TouchableOpacity} from 'react-native';
import Icon from 'react-native-vector-icons/MaterialIcons';
import CenterBubble from './bubbles/CenterBubble';

class FitRouteButton extends Component {
  constructor(props) {
    super(props);
    this.state = {};
  }
  render() {
    return (
      <CenterBubble style={styles.bubble}>
        <TouchableOpacity onPress={this.props.fitRoute}>
          <Icon name={'near-me'} size={25} />
        </TouchableOpacity>
      </CenterBubble>
    );
  }
}

const styles = StyleSheet.create({
  bubble: {
    bottom: 140,
  },
});

export default FitRouteButton;
