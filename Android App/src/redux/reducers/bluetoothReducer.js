const initialState = {
    deviceConnected: false
}

const BluetoothReducer = (state = initialState, action) => {
    switch (action.type) {
        case 'CONNECT': {
            return {
                deviceConnected: true
            }
        } 

        case 'DISCONNECT': {
            return {
                deviceConnected: false
            }
        }
            
        default:
            return state;
    }
};

export default BluetoothReducer;