import React, {Component} from 'react';
import Icon from 'react-native-vector-icons/MaterialIcons';
import CenterBubble from './bubbles/CenterBubble';
import {TouchableOpacity} from 'react-native';

class CenterButton extends Component {
  constructor(props) {
    super(props);
  }
  render() {
    return (
      <CenterBubble>
        <TouchableOpacity onPress={this.props.centerCallback}>
          <Icon name={'my-location'} size={30} />
        </TouchableOpacity>
      </CenterBubble>
    );
  }
}

export default CenterButton;
