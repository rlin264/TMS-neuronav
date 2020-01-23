//
// Created by rlin on 2019-11-19.
//
//#include <GL/glew.h>    // Initialize with glewInit()
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#endif

#include "GLFW/glfw3.h"

//#include <librealsense2/rs.hpp>
#include "example.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

//#include <GLFW/glfw3.h>
//#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
//#include <GL/gl3w.h>    // Initialize with gl3wInit()
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
//#include <GL/glew.h>    // Initialize with glewInit()
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
//#include <glad/glad.h>  // Initialize with gladLoadGL()
//#endif



// Helper function for displaying time conveniently
std::string pretty_time(std::chrono::nanoseconds duration);
// Helper function for rendering a seek bar
void draw_seek_bar(rs2::playback& playback, int* seek_pos, float2& location, float width);
float get_depth_scale(rs2::device dev);

int main(int argc, char* argv[]) try {
    //create a simple OpenGl window for rendering:
//    ImGui::CreateContext();
//    window app(1280,720,"RealSense Example");
    window app(1600,720,"RealSense Example");
    ImGui_ImplGlfw_Init(app, false);

    //create booleans to control GUI (recorded - allow play button, recording - show 'recording to file' text

    bool recorded = false;
    bool recording = false;

    //declare a texture;
    texture depth_image;
    texture color_image;

    // Declare frameset and frames which will hold the data from the camera
    rs2::frameset frames;
    rs2::frame depth;
    rs2::frame color;

    rs2::colorizer color_map;

    //create a shared pointer to the pipeline
    auto pipe = std::make_shared<rs2::pipeline>();
    //start streaming with default configuration
    rs2::config cfg; // Declare a new configuration
    cfg.enable_device_from_file("a.bag");

//    pipe->start(cfg);
    rs2::pipeline_profile profile = pipe->start(cfg);
    float depth_scale = get_depth_scale(profile.get_device());
//    cout <<depth_scale<<endl;
    rs2::device device = pipe->get_active_profile().get_device();
    rs2::playback playback = device.as<rs2::playback>();


    cout << "Hello" << endl;

    //create var to control seek bar
    int seek_pos;

    // While application is running
    while (app) {
        // Flags for displaying ImGui window
        static const int flags = ImGuiWindowFlags_NoCollapse
                                 | ImGuiWindowFlags_NoScrollbar
                                 | ImGuiWindowFlags_NoSavedSettings
                                 | ImGuiWindowFlags_NoTitleBar
                                 | ImGuiWindowFlags_NoResize
                                 | ImGuiWindowFlags_NoMove;

        ImGui_ImplGlfw_NewFrame(1);
        ImGui::SetNextWindowSize({ app.width(), app.height() });
        ImGui::Begin("app", nullptr, flags);

        //interface
        ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, { 1, 1, 1, 1 });
        ImGui::PushStyleColor(ImGuiCol_Button, { 36 / 255.f, 44 / 255.f, 51 / 255.f, 1 });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 40 / 255.f, 170 / 255.f, 90 / 255.f, 1 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 36 / 255.f, 44 / 255.f, 51 / 255.f, 1 });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);

        //play recorded file
        ImGui::SetCursorPos({ app.width() / 2 - 100, 4 * app.height() / 5 + 30 });
        ImGui::Text("Click 'play' to start playing");
        ImGui::SetCursorPos({ app.width() / 2 - 100, 4 * app.height() / 5 + 50});
        if (ImGui::Button("play", { 50, 50 }))
        {
            if (!device.as<rs2::playback>())
            {
                pipe->stop(); // Stop streaming with default configuration
                pipe = std::make_shared<rs2::pipeline>();
                rs2::config cfg;
                cfg.enable_device_from_file("a.bag");
                pipe->start(cfg); //File will be opened in read mode at this point
                device = pipe->get_active_profile().get_device();
            }
            else
            {
                device.as<rs2::playback>().resume();
            }
        }


        // If device is playing a recording, we allow pause and stop
        if (device.as<rs2::playback>())
        {
            rs2::playback playback = device.as<rs2::playback>();
//            if (pipe->poll_for_frames(&frames)) // Check if new frames are ready
//            {
//                frames = pipe->wait_for_frames();
//                depth = color_map.process(frames.get_depth_frame()); // Find and colorize the depth data for rendering
//            }
            frames = pipe->wait_for_frames();
            depth = color_map.process(frames.get_depth_frame()); // Find and colorize the depth data for rendering
            color = frames.get_color_frame();

            cout << color.get_data_size() << endl;
            cout << color.get_data() << endl;
            cout << "" <<endl;
            auto vf = color.as<rs2::video_frame>();
            const auto* p_depth_frame = reinterpret_cast<const uint16_t*>(depth.get_data());
            cout << vf.get_height() << endl;
            cout << vf.get_width() << endl;

            for(int y = 0; y < vf.get_height(); y++){
                auto depth_pixel_index = y*vf.get_width();
                for(int x = 0; x < vf.get_width();x++, ++depth_pixel_index){
                    auto pixels_distance = depth_scale * p_depth_frame[depth_pixel_index];
                    cout <<pixels_distance<<" ";
                }
                cout <<""<<endl;
            }

            cout << p_depth_frame <<endl;
            getchar();
            // Render a seek bar for the player
            float2 location = { app.width() / 4, 4 * app.height() / 5 + 110 };
            draw_seek_bar(playback , &seek_pos, location, app.width() / 2);

            ImGui::SetCursorPos({ app.width() / 2, 4 * app.height() / 5 + 50 });
            if (ImGui::Button(" pause\nplaying", { 50, 50 }))
            {
                playback.pause();
            }

            ImGui::SetCursorPos({ app.width() / 2 + 100, 4 * app.height() / 5 + 50 });
            if (ImGui::Button("  stop\nplaying", { 50, 50 }))
            {
                pipe->stop();
                pipe = std::make_shared<rs2::pipeline>();
                pipe->start();
                device = pipe->get_active_profile().get_device();
            }
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        ImGui::End();
        ImGui::Render();
        // Render depth frames from the default configuration, the recorder or the rs-record-playback
//        cout << "Hello" << endl;
        std::cout<<"a is of type: "<<typeid(depth).name()<<std::endl;
        depth_image.render(depth, { app.width() * 0.00f, app.height() * 0.25f, app.width() * 0.5f, app.height() * 0.75f  });
        color_image.render(color, { app.width() * 0.5f, app.height() * 0.25f, app.width() * 0.5f, app.height() * 0.75f  });
    } //end while loop
}
catch (const rs2::error & e)
{
    std::cout << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
std::string pretty_time(std::chrono::nanoseconds duration)
{
    using namespace std::chrono;
    auto hhh = duration_cast<hours>(duration);
    duration -= hhh;
    auto mm = duration_cast<minutes>(duration);
    duration -= mm;
    auto ss = duration_cast<seconds>(duration);
    duration -= ss;
    auto ms = duration_cast<milliseconds>(duration);

    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(hhh.count() >= 10 ? 2 : 1) << hhh.count() << ':' <<
           std::setfill('0') << std::setw(2) << mm.count() << ':' <<
           std::setfill('0') << std::setw(2) << ss.count();
    return stream.str();
}


void draw_seek_bar(rs2::playback& playback, int* seek_pos, float2& location, float width)
{
    int64_t playback_total_duration = playback.get_duration().count();
    auto progress = playback.get_position();
    double part = (1.0 * progress) / playback_total_duration;
    *seek_pos = static_cast<int>(std::max(0.0, std::min(part, 1.0)) * 100);
    auto playback_status = playback.current_status();
    ImGui::PushItemWidth(width);
    ImGui::SetCursorPos({ location.x, location.y });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
    if (ImGui::SliderInt("##seek bar", seek_pos, 0, 100, ""))
    {
        //Seek was dragged
        if (playback_status != RS2_PLAYBACK_STATUS_STOPPED) //Ignore seek when rs-record-playback is stopped
        {
            auto duration_db = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(playback.get_duration());
            auto single_percent = duration_db.count() / 100;
            auto seek_time = std::chrono::duration<double, std::nano>((*seek_pos) * single_percent);
            playback.seek(std::chrono::duration_cast<std::chrono::nanoseconds>(seek_time));
        }
    }
    std::string time_elapsed = pretty_time(std::chrono::nanoseconds(progress));
    ImGui::SetCursorPos({ location.x + width + 10, location.y });
    ImGui::Text("%s", time_elapsed.c_str());
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
}
float get_depth_scale(rs2::device dev)
{
    // Go over the device's sensors
    for (rs2::sensor& sensor : dev.query_sensors())
    {
        // Check if the sensor if a depth sensor
        if (rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>())
        {
            return dpt.get_depth_scale();
        }
    }
    throw std::runtime_error("Device does not have a depth sensor");
}