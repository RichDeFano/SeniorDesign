# Arrow 
### 2020 UI ECE Senior Design Project

The diagram below illustrates the general overview of how the system works and all the parts
![Bike Diagram](https://github.com/Dylan-Gardner/Arrow_App/blob/master/BikeDiagram.png)

In this repo you will find source code for both the Screen interface ,which runs off a Adafruit Bluefruit Feather 32u4, and
the source code for the Android application built with React Native, iOS code is very limited at the moment and will not work
(more iOS features to come later)

Also in this repo is the documentation we used for the hardware components as well as a hardware schematic.

List of major libraries used:  
[Spotify SDK](https://developer.spotify.com/documentation/android/) - Used to get info about playback and make playback state changes  
[mad-location-manager](https://github.com/maddevsio/mad-location-manager) - Used to filter and normalize noisy GPS data  
[react-native-ble-plx](https://github.com/Polidea/react-native-ble-plx) - Bluetooth BLE communication with Feather  
[Mapbox Map](https://docs.mapbox.com/android/maps/overview/) and [Naviagtion SDK](https://docs.mapbox.com/android/navigation/overview/) - Map view and Navigation View  
[react-redux](https://react-redux.js.org/) - State managment  
