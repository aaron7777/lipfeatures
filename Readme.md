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
* Faces with rotation/different sizes caused by camera position and describer's movement.
* Fast and accurate computation needed to avoid latency


###BACKGROUND

###METHODS

###COMPILING

####TEST

###FUTURE WORK

###REFERENCE