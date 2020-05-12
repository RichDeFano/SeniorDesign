import React, { Component } from 'react';
import { View, Text } from 'react-native';
import Spotify from './Spotify'
import {connect} from'react-redux'

import {connected, disconnected} from '../redux/actions/bluetoothActions'



class Settings extends Component {
    render() {

        let bluetooth;
        if(this.props.deviceConnected) {
            bluetooth= <Text>
                            Device Connected
                        </Text>
        }else{
            bluetooth = <Text>
                            Device not connected, ensure it is on and in range
                        </Text>
        }
        return (
            <View>
                <Text>
                    Settings
                </Text>
                <Spotify />
                {bluetooth}
            </View>
        );
    }
}

const mapStateToProps = (state) => {
    // Redux Store --> Component
    return {
      deviceConnected: state.bluetoothReducer.deviceConnected
    };
  };
  // Map Dispatch To Props (Dispatch Actions To Reducers. Reducers Then Modify The Data And Assign It To Your Props)
  const mapDispatchToProps = (dispatch) => {
    // Action
    return {
      deviceConnect: () => {dispatch(connected())},
      deviceDisconected: () => {dispatch(disconnected())},
    };
  };
  
export default connect(mapStateToProps, mapDispatchToProps)(Settings);