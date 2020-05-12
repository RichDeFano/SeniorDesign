import MapboxDirectionsFactory from '@mapbox/mapbox-sdk/services/directions';

import env from './env.json';

const clientOptions = {accessToken: env.accessToken};
const directionsClient = MapboxDirectionsFactory(clientOptions);

export {directionsClient};
