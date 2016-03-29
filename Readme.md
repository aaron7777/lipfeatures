## Analysis of Lips/Mouth Motion for Deception Detection

###SUMMARY
Previous research is focused on detecting eye-activity level features for deception detection. My research puts emphasis on lips/mouth motion, which is also an important part of facial expression. For instance, deceptive people are more likely to press or bite their lips. Lips/mouth features (lip corners positions) are extracted from describers' facial expression. With the analysis of these extractd features, deception detection can be more accurate.

###PROBLEM STATEMENT

Implement a program to help deception detection through extracting and analyzing the features of lips/mouth of a describer from video on flimflam.me.

####Step 1 - Face Detection
For an input raw video, the program can capture each frame. For each frame, face detection is necessary before any feature extraction.
####Step 2 - Lips Features Extraction
Based on the result of face detection, four lip corners (left, right, highest and lowest) positions are extracted based on face normalization.
####Step 3 - Analysis of Lips Motion
With lips features of each frame, lips motion is recorded according to the time line. Frequency of lip pressing/biting or smiling can be analyzed to help judge whether a person is telling the truth or lying. 
#### Difficulties
* Low quality video files with noise.
* Limited number of raw videos.
* Faces with rotation/different sizes caused by camera position and describer's movement.
* Fast and accurate computation needed to avoid latency

###BACKGROUND

####OpenCV (version 2.4.12)
A library aimed at real-time computer vision. Used for frame capture and basic image processing.

####Dlib (version 18.18)
An open source software written in C++, containing software components for dealing with image processing. Dlib is used for face detection and feature extraction.

####Xlib (XQuartz version 2.7.8)
A library that allows you to draw graphics on the screen of any X server using C language. XQuartz is the Apple Inc. version of X server, and X server is a component of the  X Window System, providing framework for a GUI enviroment. Xlib is used to showing and drawing image windows to indicate lips features on the face.

###METHODS
####Face Components Detection
Face detector is made using the classic Histogram of Oriented Gradients (HOG) feature combined with a linear classifier, an image pyramid,
and sliding window detection scheme. Dlib provides a "shape_predictor_68_face_landmarks.dat" model, based on the iBUG 300-W dataset. The dataset has more than 10,000 images annotated with the 68-point convention.
![ScreenShot](https://github.com/aaron7777/lipfeatures/blob/master/pic/figure_68_markup.jpg)

####Normalization & Feature Extraction



###COMPILING

###TEST

###FUTURE WORK

###REFERENCE