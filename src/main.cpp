#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>

using namespace dlib;
using namespace std;
using namespace cv;

// ----------------------------------------------------------------------------------------

class interval {
    public:
        int begin;
        int end;
        int num; // question num
        interval(double b, double e, int n):begin(b),end(e),num(n) {}
};

cv::Mat debugImage;
std::string main_window_name = "Capture - Face detection";

int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        if (argc < 4)
        {
            printf("Call this program like this:\n");
            printf("/lipfeatures [video name] [silent interval file name] [output file name]\n");
            printf("eg: /lipfeatures ../../res/1ruthen.mp4 ../../res/1interval.txt lipfeatures1.txt\n");
            return 0;
        }

        // Read silent interval file
        FILE* f, *f1;
        int begin, end;
        int num;
        std::vector<interval*> silentIntvl;
        cv::Mat frame;

        f = fopen(argv[2], "r");

        if(f == NULL) {
            printf("The silent interval file %s cannot be opened\n", argv[2]);
            return 0;
        }

        while(fscanf(f, "%d %d %d", &num, &begin, &end) == 3) {
            silentIntvl.push_back(new interval(begin, end, num));
        }

        fclose(f);

        int intervalNum = silentIntvl.size();
        int intervalCounter = 0;
        if (intervalNum == 0) return 0;
        bool ifRecord = false;
        //begin = silentIntvl[0]->begin;
        //end = silentIntvl[0]->end;
        //num = silentIntvl[0]->num;

        //cv::VideoCapture cap(0);
        cv::VideoCapture cap(argv[1]);

        int totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
        double fps = cap.get(CV_CAP_PROP_FPS);
        
        printf("fps = %lf\n", fps);
        printf("framenumber = %d\n", totalFrameNumber);
        
        //image_window win;

        // We need a face detector.  We will use this to get bounding boxes for
        // each face in an image.
        frontal_face_detector detector = get_frontal_face_detector();
        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
        // as a command line argument.
        shape_predictor sp;
        deserialize("../../res/shape_predictor_68_face_landmarks.dat") >> sp;

        f1 = fopen(argv[3], "w");

        int frameCounter = 0;
        //image_window win_faces;

        long compTime;
        struct timeval startRun, endFrame;

         // Grab and process frames until the main window is closed by the user.
        while(true)
        {
            gettimeofday(&startRun, NULL);
            // Grab a frame
            cap >> frame;
            frame.copyTo(debugImage);

            if(!frame.empty()) {

                if (intervalCounter >= intervalNum) ifRecord = false;
                else if (frameCounter < silentIntvl[intervalCounter]->begin) ifRecord = false;
                else if (frameCounter > silentIntvl[intervalCounter]->end) ifRecord = false;
                else ifRecord = true;

                //printf("intervalCounter = %d\n", intervalCounter);
                printf("frameCounter = %d\n", frameCounter);
                printf("ifRecord = %d\n", ifRecord);

                // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
                // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
                // long as frame is valid.  Also don't do anything to frame that would cause it
                // to reallocate the memory which stores the image as that will make cimg
                // contain dangling pointers.  This basically means you shouldn't modify frame
                // while using cimg.
                cv_image<bgr_pixel> cimg(frame);

                // Detect faces 
                std::vector<dlib::rectangle> faces = detector(cimg);

                // Find the pose of each face. 
                // Here: there is only one face.
                std::vector<full_object_detection> shapes;
                //std::vector<dlib::image_window::overlay_line> lines;
                const rgb_pixel color = rgb_pixel(0,255,0);

                for (unsigned long i = 0; i < faces.size(); ++i) {

                    full_object_detection shape = sp(cimg, faces[i]);
                    shapes.push_back(shape);
                        
                }

                // Now let's view lines of lips on the screen.

                if (faces.size() > 0) {
                    if (ifRecord) {
                        //win.add_overlay(lines);

                        // View the entire face
                        //win.add_overlay(render_face_detections(shapes));
                  

                        // We can also extract copies of each face that are cropped, rotated upright,
                        // and scaled to a standard size as shown here:
                        dlib::array<array2d<rgb_pixel> > face_chips;
                        std::vector<chip_details> face_details;
                        face_details = get_face_chip_details(shapes, 200, 0.2);
                        extract_image_chips(cimg, face_details, face_chips);

                        // View the scaled/normalized face 
                        //win_faces.set_image(tile_images(face_chips));

                        // Map the point from the original face to the normalized one
                        point_transform_affine p = get_mapping_to_chip(face_details[0]);

                        // Record the landmarks(lip features) needed to the file 
                        point point_in_chip1, point_in_chip2, point_in_chip3, point_in_chip4, point_in_chip5, point_in_chip6;
                        point_in_chip1 = p(shapes[0].part(48));
                        point_in_chip2 = p(shapes[0].part(51));
                        point_in_chip3 = p(shapes[0].part(54));
                        point_in_chip4 = p(shapes[0].part(57));
                        point_in_chip5 = p(shapes[0].part(62));
                        point_in_chip6 = p(shapes[0].part(66));

                        fprintf(f1, "%d,%d,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", 
                            silentIntvl[intervalCounter]->num, frameCounter, point_in_chip1.x(), point_in_chip1.y(), 
                                point_in_chip2.x(), point_in_chip2.y(), point_in_chip3.x(), point_in_chip3.y(),
                                    point_in_chip4.x(), point_in_chip4.y(), point_in_chip5.x(), point_in_chip5.y(),
                                        point_in_chip6.x(), point_in_chip6.y());
                    }

                    for (unsigned long m = 49; m <= 59; ++m) {// Lip (Outer)
                        cv::Point pt(shapes[0].part(m).x(), shapes[0].part(m).y());
                        cv::Point pt2(shapes[0].part(m-1).x(), shapes[0].part(m-1).y());
                        line(debugImage, pt, pt2, 92384798);
                    }
                    cv::Point pt(shapes[0].part(48).x(), shapes[0].part(48).y());
                    cv::Point pt2(shapes[0].part(59).x(), shapes[0].part(59).y());
                    line(debugImage, pt, pt2, 92384798);

                    for (unsigned long m = 61; m <= 67; ++m) {// Lip (Inner)
                        cv::Point pt(shapes[0].part(m).x(), shapes[0].part(m).y());
                        cv::Point pt2(shapes[0].part(m-1).x(), shapes[0].part(m-1).y());
                        line(debugImage, pt, pt2, 92384798);
                    }
                    cv::Point pt3(shapes[0].part(60).x(), shapes[0].part(60).y());
                    cv::Point pt4(shapes[0].part(67).x(), shapes[0].part(67).y());
                    line(debugImage, pt3, pt4, 92384798);

                    gettimeofday(&endFrame, NULL);
                    compTime = (endFrame.tv_sec - startRun.tv_sec) * 1000 + (endFrame.tv_usec - startRun.tv_usec)/1000;
                    //printf("compTime = %ld ms\n", compTime);
                }
            }

            else {
                printf(" --(!) No captured frame -- Break!");
                break;
            }

            imshow(main_window_name,debugImage);

            if (cv::waitKey(2) >= 0)  break;

            if (intervalCounter < intervalNum) {
                if (frameCounter == silentIntvl[intervalCounter]->end) intervalCounter++;
            }
            frameCounter++;
        }

        fclose(f1);
    }
    catch (exception& e)
    {
        printf("\nexception thrown!\n");
        printf("%s\n", e.what());
    }
}

// ----------------------------------------------------------------------------------------

