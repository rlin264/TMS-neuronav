cmake_minimum_required(VERSION 3.15)
project(3Dmm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lboost_program_options")
set(CMAKE_CXX_FLAGS "-lstdc++fs")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
#set(3Dmm 3RDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/eos/3rdparty") # this is used by the CMakeLists.txt files in the subdirectories
#add_executable(fit-model fit-model.cpp)

find_package(OpenCV REQUIRED)
if("${OpenCV_VERSION_MAJOR}$" EQUAL 2)
    message(STATUS "OpenCV 2.x detected")
    find_package(OpenCV 2.4.3 REQUIRED core imgproc highgui)
elseif("${OpenCV_VERSION_MAJOR}$" EQUAL 3)
    message(STATUS "OpenCV 3.x detected - including imgcodecs for compatibility")
    find_package(OpenCV 3 REQUIRED core imgproc imgcodecs)
elseif("${OpenCV_VERSION_MAJOR}$" EQUAL 4)
    message(STATUS "OpenCV 4.x detected - including imgcodecs for compatibility")
    find_package(OpenCV 4 REQUIRED core imgproc imgcodecs)
endif()
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

# Find the directory to OpenCV's face detector:
if(NOT OpenCV_haarcascades_DIR)
    # This will fail for system-packaged OpenCV
    list(GET OpenCV_INCLUDE_DIRS 0 OpenCV_first_include_dir)
    set(OpenCV_haarcascades_DIR "${OpenCV_first_include_dir}/../share/OpenCV/haarcascades")
endif()
if(NOT EXISTS "${OpenCV_haarcascades_DIR}/haarcascade_frontalface_alt2.xml")
    message(FATAL_ERROR "Could not find OpenCV's face detector haarcascade_frontalface_alt2.xml in ${OpenCV_haarcascades_DIR}. Please set OpenCV_haarcascades_DIR to the directory with that file.")
endif()

if(MSVC)
    # The standard find_package for boost on Win finds the dynamic libs, so for dynamic linking to boost we need to #define:
    add_definitions(-DBOOST_ALL_NO_LIB) # Don't use the automatic library linking by boost with VS (#pragma ...). Instead, we specify everything here in cmake.
    add_definitions(-DBOOST_ALL_DYN_LINK) # Link against the dynamic boost lib - needs to match with the version that find_package finds.
    add_definitions(-D_HAS_AUTO_PTR_ETC) # Boost 1.65.1 still does not work with VS C++17 mode, this is the workaround
endif()
find_package(Boost 1.50.0 COMPONENTS system filesystem program_options REQUIRED)
message(STATUS "Boost found at ${Boost_INCLUDE_DIRS}")


#FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

#add_executable(fit-model fit-model.cpp)

#TARGET_LINK_LIBRARIES(fit-model ${Boost_LIBRARIES})

add_executable(fit-model fit-model.cpp)
add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(main PUBLIC ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

target_link_libraries(fit-model ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_link_libraries(fit-model "$<$<CXX_COMPILER_ID:GNU>:-pthread>$<$<CXX_COMPILER_ID:Clang>:-pthreads>")
target_include_directories(fit-model PUBLIC ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(testing2d testing2d.cpp)
target_link_libraries(testing2d ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(testing2d PUBLIC ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


#target_include_directories(fit-model eos/include)
include_directories(eos/include
                            eos/3rdparty/cereal/include
                            eos/3rdparty/glm
                            eos/3rdparty/nanoflann/include
                            eos/3rdparty/eigen/Eigen
                            eos/3rdparty/eigen3-nnls/src
                            eos/3rdparty/toml11
                            eos/3rdparty
                    superviseddescent/include
                    superviseddescent/3rdparty/cereal-1.1.1/include
                    eigen
                    eos/share
                    $(Eigen_INCLUDE_DIRS))

#set(SOURCES eos/3rdparty/eigen3/Eigen)
