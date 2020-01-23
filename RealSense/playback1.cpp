#include <librealsense2/rs.hpp>

int main()
{

// Create a simple OpenGL window for rendering:
//window app(1280, 720, "RealSense Video Playback");

// Declare two textures on the GPU, one for color and one for depth
    texture depth_image, color_image;

// Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    rs2::config cfg;
    cfg.enable_device_from_file("color.bag");
    rs2::pipeline pipe;
    pipe.start(cfg);
    while (true)
    {
        rs2::frameset frames;
        if (pipe.poll_for_frames(&frames))
        {
//use frames here
        }
    }
    pipe.stop();

    return 0;

}