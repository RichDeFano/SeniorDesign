import React from 'react';
import PropTypes from 'prop-types';
import {View, StyleSheet, TouchableOpacity} from 'react-native';

const styles = StyleSheet.create({
  container: {
    borderRadius: 45/2,
    position: 'absolute',
    bottom: 85,
    width: 45,
    height:45,
    right: 25,
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: 'white',
  },
});

class CenterBubble extends React.PureComponent {
  static propTypes = {
    onPress: PropTypes.func,
    children: PropTypes.any,
    style: PropTypes.any,
  };

  render() {
    let innerChildView = this.props.children;

    if (this.props.onPress) {
      innerChildView = (
        <TouchableOpacity onPress={this.props.onPress}>
          {this.props.children}
        </TouchableOpacity>
      );
    }

    return (
      <View style={[styles.container, this.props.style]}>{innerChildView}</View>
    );
  }
}

export default CenterBubble;