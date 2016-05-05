## Analysis of Lips/Mouth Motion for Deception Detection

###SUMMARY
Previous research is focused on detecting eye-activity level features for deception detection. My research puts emphasis on lips/mouth motion, which is also an important part of facial expression. For instance, deceptive people are more likely to press or bite their lips. Lips/mouth features (lip corners positions) can be extracted from describers' facial expression. With the analysis of these extractd features, deception detection can be more accurate.

###PROBLEM STATEMENT

Implement a program to help deception detection through extracting and analyzing the features of lips/mouth of a describer when answering questions of an interrogator from 15-FPS videos on flimflam.me. All significant lip features of the describer in the silent interval when he/she is answering a question should be captured.

####Step 1 - Silent Interval Capture
According to the file recording each time interval between the interrogator finished raising a question and the describer finished answering it, the program can extract all the silent intervals of the describer for each question. As the conversation contains two sections (general questions section and questions about the picture showed), section number (1 or 2) is also recorded together with each silent interval.
####Step 2 - Face Detection
For an input raw video, the program can capture frame by frame. For each frame, face detection is necessary before any feature extraction.
####Step 3 - Lip Features Extraction
Based on the result of silent interval capture, only the frames included in the intervals are necessary. According to the result of face detection, six lip corners (point 49, 55, 52, 58, 63, 67 in the 68_face_landmark figure) positions of each necessary frame are extracted based on face normalization. 
####Step 4 - Analysis of Lips Motion
With lips features of corresponding frame, lips motion is recorded according to the time line. Frequency of lip pressing can be analyzed to help judge whether a person is telling the truth or lying. Average distance between upper and lower lips can be calculated to provide reference to analysis.

#### Difficulties
* Low quality video files with noise.
* Limited number of raw videos.
* Unsynchronized frame and sound of raw videos.
* Faces with rotation/different sizes caused by camera position and describers' movement.
* Fast and accurate computation needed to avoid latency

###BACKGROUND

Emphasis was put on how to extract the position points of lips after accurate face detection at the beginning. Haar-cascade Detection in OpenCV and Active Shape Model (ASM) library both can recognize face in high accuracy. However, they cannot capture the significant points from the detected faces perfectly. In the end, Dlib C++ library was found to have great performance in real-time face landmark extraction.

The following are tools selected to deal with real-time face detection, lip features extraction, silent interval capture, and data analysis and plot.

####OpenCV (version 2.4.12)
A library aimed at real-time computer vision. Used for frame capture and basic image processing.

####Dlib (version 18.18)
An open source software written in C++, containing software components for dealing with image processing. Dlib is used for face detection and feature extraction.

####Xlib (XQuartz version 2.7.8)
A library that allows you to draw graphics on the screen of any X server using C language. XQuartz is the Apple Inc. version of X server, and X server is a component of the  X Window System, providing framework for a GUI enviroment.

####Praat (version 6.0.17 of on MacOS)
A computer program to analyze, synthesize, and manipulate speech, and create high-quality pictures for your articles and thesis. Praat is used to get the silent and sounding intervals of .wav files. Silence threshold (dB) can be manually set.

####Python (version 2.7.6) with numpy (version 1.8.0rc1) and matplotlib (version 1.3.1)
To calculate the average distance between upper and lower lips and plot the distances for each frame.

###METHODS

####Silent Interval Capture
Transcode .mp4 videos to .wav files. Get all the silent and sounding intervals of a describer and the corresponding interrogator with Praat. Record the starting time and the ending time fo each question. Capture every silent interval of the describer for each question period. Transfer the silent intervals from form of sec to frame number.

####Face Components Detection
Face detector is made using the classic Histogram of Oriented Gradients (HOG) feature combined with a linear classifier, an image pyramid,and sliding window detection scheme. Dlib provides a "shape_predictor_68_face_landmarks.dat" model based on the iBUG 300-W dataset. The dataset has more than 10,000 images annotated with the 68-point convention.
<img src="https://raw.githubusercontent.com/aaron7777/lipfeatures/master/pic/figure_68_markup.jpg" width="300px" height="300px">

####Normalization & Feature Extraction
Take use of dlib::get_face_chip_details to normalize the face and extract a copy of face that has been rotated upright, centered, and scaled to a standard size. Map the points we need from the original face to the normalized one. Then write the positions (x, y) to the output .txt file. One frame one line.


###COMPILING & RUNNING

The program is divided into 3 small program. Here we take 2016-03-02_14-03-04-887-RUTHEH and 2016-03-02_14-03-04-887-Simplyme as an example.

####Silent Interval Capture (getinterval.cpp)
Use g++ to compile.
```bash
g++ getinterval.cpp -o getinterval
```
Given 1des-ruthen.txt (silent and sounding intervals of the describer) and 1ques.txt (question intervals) as input files. Set output file (silent intervals of the describer for each question) name.
```bash
./getinterval ../res/1des-ruthen.txt ../res/1ques.txt ../res/1interval.txt
```
####Lip features Extraction (main.cpp)
Use CMake to compile. Also, the face detector is fastest when compiled with at least SSE2 instructions enabled.
```bash
mkdir build
cd build
cmake .. -DUSE_AVX_INSTRUCTIONS=ON
cmake --build . --config Release
```
Run as the following: ./lipfeatures [video name] [silent interval file name] [output file name]
```bash
./lipfeatures ../../res/2016-03-02_14-03-04-887-RUTHEH.mp4 ../../res/1interval.txt lipfeatures1.txt
```
####Plot (plot.py)
Compile and run with one command line.
```bash
python plot.py
```

###TEST
Also, take 2016-03-02_14-03-04-887-RUTHEH as an example to test lip features extraction.
The test result is satisfying.

<img src="https://raw.githubusercontent.com/aaron7777/lipfeatures/master/pic/f1.png" width="200px" height="200px">
<img src="https://raw.githubusercontent.com/aaron7777/lipfeatures/master/pic/f2.png" width="200px" height="200px">
<img src="https://raw.githubusercontent.com/aaron7777/lipfeatures/master/pic/f3.png" width="200px" height="200px">

The following shows the output lipfeatures1.txt file that records the lips features of each frame in form of (section number, frame number, piont 49, 55, 52, 58, 63, 67 position):

<img src="https://raw.githubusercontent.com/aaron7777/lipfeatures/master/pic/lipdata.png" width="500px" height="300px">

##RESULT
Due to unsynchronized frame and sound raw videos, only 5 pairs of videos were selected in the program.
| Name | Avg Distance between point 52 & 58 in Section 1| Avg Distance between point 63 & 67  in Section 1|
| ------------ | ------------- | ------------ |
| 2016-03-02_14-03-04-887-RUTHEH | 1  | 1 |
| 2016-03-11_16-16-40-42-pamela3 | 1 | 1 |

###FUTURE WORK

The future work is to apply the extracted lips features to deception detection. As mentioned above, I plan to focus on the difference of frequency of lip pressing/biting or smiling of truth tellers and liars.

###REFERENCES

- [Dlib example program - face_landmark_detection](http://dlib.net/face_landmark_detection_ex.cpp.html)
- [Dlib image processing header file - render_face_detections.h](http://dlib.net/dlib/image_processing/render_face_detections.h.html)
- [Shaowei Su's Eye-Activity Level Program](https://github.com/shaowei-su/Eye_Activity_Level)
