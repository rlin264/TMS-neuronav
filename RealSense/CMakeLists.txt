cmake_minimum_required(VERSION 3.15)
project(rs-record-playback)

set(CMAKE_CXX_STANDARD 14)
#set(SOURCE_FILES imgui/imgui.cpp)

#add_executable(testing rs-record-playback.cpp rs-record-playback.h)
#if(BUILD_GRAPHICAL_EXAMPLES)
include(${CMAKE_SOURCE_DIR}/CMakeFiles/opengl_config.cmake)
    set(sources
    #        main.cpp
            rs-record-playback.cpp
    #        playback.cpp
            example.hpp
            imgui/imconfig.h
    #        imgui/imgui_demo.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_impl_glfw.cpp
            imgui/imgui_impl_glfw.h
            imgui/imgui_internal.h
    #        imgui/imgui_widgets.cpp
            imgui/imgui.cpp
            imgui/imgui.h
            imgui/stb_rect_pack.h
            imgui/stb_textedit.h
            imgui/stb_truetype.h
    #        imgui/imgui_impl_opengl3.h)
        )
set(sources1
        #        main.cpp
#        rs-record-playback.cpp
        playback.cpp
        example.hpp
        imgui/imconfig.h
#        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_glfw.h
        imgui/imgui_internal.h
#        imgui/imgui_widgets.cpp
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/stb_rect_pack.h
        imgui/stb_textedit.h
        imgui/stb_truetype.h
        imgui/imgui-fonts-fontawesome.hpp
        imgui/imgui-fonts-karla.hpp)

add_executable(${CMAKE_PROJECT_NAME} ${sources})
add_executable(playback ${sources1})
#add_executable(playback playback.cpp ${sources})
#set_property(TARGET rs-record-playback PROPERTY CXX_STANDARD 14)
target_link_libraries(rs-record-playback ${DEPENDENCIES})
#target_link_libraries(playback glfw ${GLFW_LIBRARIES})
target_link_libraries(playback ${DEPENDENCIES})
include_directories(rs-record-playback .imgui) #../ ../../third-party/tclap/include
include_directories(playback imgui)
set_target_properties (rs-record-playback PROPERTIES FOLDER "Examples")
install(TARGETS rs-record-playback RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})