export const currUpdate = (latitude, longitude) => ({
  type: 'CURR_POS',
  latitude: latitude,
  longitude: longitude,
});

export const destUpdate = (latitude, longitude, address) => ({
  type: 'DEST_POS',
  latitude: latitude,
  longitude: longitude,
  address: address,
});

export const navStart = () => ({
  type: 'NAV_START',
});

export const navStop = () => ({
  type: 'NAV_STOP',
});
