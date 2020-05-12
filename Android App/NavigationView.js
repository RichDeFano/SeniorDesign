import React, {Component} from 'react';
import PropTypes from 'prop-types';
import {requireNativeComponent} from 'react-native';

const MapboxNavigationView = requireNativeComponent(
  'MapboxNavigationView',
  NavigationView,
);

export default class NavigationView extends Component {
  render() {
    return <MapboxNavigationView style={this.props.style} {...this.props} />;
  }
}

NavigationView.propTypes = {
  origin: PropTypes.shape({
    lat: PropTypes.number,
    long: PropTypes.number,
  }).isRequired,
  destination: PropTypes.shape({
    lat: PropTypes.number,
    long: PropTypes.number,
  }).isRequired,
};
