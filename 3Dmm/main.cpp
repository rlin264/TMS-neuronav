#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

std::string GetCurrentWorkingDir( void ) {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}

using cv::Mat;

int main() {
    std::cout << GetCurrentWorkingDir() << std::endl;
    Mat image = cv::imread("/home/rlin/TMS/3Dmm/data/test_img.png");
    if (!image.data)    // image was not created
    {
        exit(1);
    }
    cv::imshow("Display window", image);
    cv::waitKey(0);
    return 0;
}
