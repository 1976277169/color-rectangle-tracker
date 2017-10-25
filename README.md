# color-rectangle-tracker
A Unity plugin which tracks colored rectangles from camera view on Android and iOS

You can set a RGB color range to filter tracked rectangles withing certain color range.

It is also possible to add a square filter to track only square shapes.

## Project structure
### android-rectangle-detector
Contains a standalone debug application for android

### android-rectangle-lib
Contains an Android library project to compile the detector library to be used in Unity projects

### iOSRectangleLib
Contains an iOS library project to compile the detector library for iOS to be used in Unity projects

### UnityProject
Contains a demo unity project which has the Android and iOS plugins included
