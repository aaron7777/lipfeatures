// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    


    This face detector is made using the classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  The pose estimator was created by
    using dlib's implementation of the paper:
        One Millisecond Face Alignment with an Ensemble of Regression Trees by
        Vahid Kazemi and Josephine Sullivan, CVPR 2014
    and was trained on the iBUG 300-W face landmark dataset.  

    Also, note that you can train your own models using dlib's machine learning
    tools.  See train_shape_predictor_ex.cpp to see an example.

    


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#include <iostream>
#include <fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>

using namespace dlib;
using namespace std;
using namespace cv;

// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./flandmark shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }

        cv::VideoCapture cap(0);
        image_window win;

        // We need a face detector.  We will use this to get bounding boxes for
        // each face in an image.
        frontal_face_detector detector = get_frontal_face_detector();
        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
        // as a command line argument.
        shape_predictor sp;
        deserialize(argv[1]) >> sp;

        // File to record lipfeatures 
        ofstream outfile;
        outfile.open("/Users/xinxu/Desktop/lipfeatures/LandMark/lipdata.txt");

        int frameCounter = 0;
        image_window win_faces;
         // Grab and process frames until the main window is closed by the user.
        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat frame;
            cap >> frame;
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
            std::vector<dlib::image_window::overlay_line> lines;
            const rgb_pixel color = rgb_pixel(0,255,0);

            for (unsigned long i = 0; i < faces.size(); ++i) {

                full_object_detection shape = sp(cimg, faces[i]);
                /* Rectangles of each part of face */
                /*
                for (unsigned long m = 1; m <= 16; ++m) // Face
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));

                for (unsigned long m = 28; m <= 30; ++m) // Nose (Upper)
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));

                for (unsigned long m = 18; m <= 21; ++m) // Left Brow
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));

                for (unsigned long m = 23; m <= 26; ++m) // Right Brow
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));

                for (unsigned long m = 31; m <= 35; ++m) // Nose (Lower)
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));
                lines.push_back(image_window::overlay_line(shape.part(30), shape.part(35), color));

                for (unsigned long m = 37; m <= 41; ++m) // Left Eye
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));
                lines.push_back(image_window::overlay_line(shape.part(36), shape.part(41), color));

                for (unsigned long m = 43; m <= 47; ++m) // Right Eye
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));
                lines.push_back(image_window::overlay_line(shape.part(42), shape.part(47), color));*/

                for (unsigned long m = 49; m <= 59; ++m) // Lip (Outer)
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));
                lines.push_back(image_window::overlay_line(shape.part(48), shape.part(59), color));

                for (unsigned long m = 61; m <= 67; ++m) // Lip (Inner)
                    lines.push_back(image_window::overlay_line(shape.part(m), shape.part(m-1), color));
                lines.push_back(image_window::overlay_line(shape.part(60), shape.part(67), color));

                // Four points to be recorded (48, 51, 54, 57) 
                /*
                lines.push_back(image_window::overlay_line(shape.part(48), shape.part(51), color));
                lines.push_back(image_window::overlay_line(shape.part(51), shape.part(54), color));
                lines.push_back(image_window::overlay_line(shape.part(54), shape.part(57), color));
                lines.push_back(image_window::overlay_line(shape.part(57), shape.part(48), color));*/

                shapes.push_back(shape);
            }
            
            // Now let's view lines of lips on the screen.
            win.clear_overlay();
            win.set_image(cimg);
            win.add_overlay(lines);

            // View the entire face
            //win.add_overlay(render_face_detections(shapes));
      

            // We can also extract copies of each face that are cropped, rotated upright,
            // and scaled to a standard size as shown here:
            dlib::array<array2d<rgb_pixel> > face_chips;
            std::vector<chip_details> face_details;
            face_details = get_face_chip_details(shapes, 100, 0.2);
            extract_image_chips(cimg, face_details, face_chips);

            // View the scaled/normalized face 
            win_faces.set_image(tile_images(face_chips));

            // Map the point from the original face to the normalized one
            point_transform_affine p = get_mapping_to_chip(face_details[0]);

            // Record the landmarks(lip features) needed to the file 
            point point_in_chip1, point_in_chip2, point_in_chip3, point_in_chip4;
            point_in_chip1 = p(shapes[0].part(48));
            point_in_chip2 = p(shapes[0].part(51));
            point_in_chip3 = p(shapes[0].part(54));
            point_in_chip4 = p(shapes[0].part(57));

            outfile << frameCounter++;
            outfile << ",";
            outfile << point_in_chip1.x();
            outfile << ",";
            outfile << point_in_chip1.y();
            outfile << ",";
            outfile << point_in_chip2.x();
            outfile << ",";
            outfile << point_in_chip2.y();
            outfile << ",";
            outfile << point_in_chip3.x();
            outfile << ",";
            outfile << point_in_chip3.y();
            outfile << ",";
            outfile << point_in_chip4.x();
            outfile << ",";
            outfile << point_in_chip4.y();
            outfile << endl;
        }
        outfile.close();
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------------

