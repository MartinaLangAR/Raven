cmake_minimum_required(VERSION 3.25)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
add_compile_options(-std=c++17 -Wall -Wstrict-aliasing -Wno-unused-parameter -faligned-new )

add_definitions(-D_FILE_OFFSET_BITS=64)
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wno-psabi)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

project( Raven )

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVENT REQUIRED libevent)
include_directories(${LIBEVENT_INCLUDE_DIRS})


find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )
find_package( Armadillo  REQUIRED )
# find_package(Threads REQUIRED)
# pkg_check_modules(LIBCAMERA REQUIRED libcamera)

include_directories( ${OpenCV_INCLUDE_DIRS} /home/pi/Documents/Raven/mlpack-4.3.0/src)
# include_directories(${LIBCAMERA_INCLUDE_DIRS})
# include_directories(PUBLIC /home/pi/Documents/Raven/libevent-2.1.12-stable/include)
# include_directories(PUBLIC /home/pi/Documents/Raven/libevent-2.1.12-stable/include/event2)

# Additional library paths
# link_directories(${LIBCAMERA_LIBRARY_DIRS})

add_library(RavenLib facedetector.cpp servos.cpp)

# add_library(libevent SHARED IMPORTED)
# set_target_properties(libevent PROPERTIES IMPORTED_LOCATION /home/pi/Documents/Raven/libevent-2.1.12-stable/lib/libevent.so)


#add_executable(CollectRaven collect_data_v2.cpp)
add_executable(Raven main.cpp) 
add_executable(TrainRaven train_your_raven.cpp)
add_executable(ReinforceRaven reinforce_train_your_raven.cpp)
#add_executable(CmdServo cmd_app.cpp)
#add_executable(MovePercentage move_to_percentage_via_input.cpp)
add_executable(Manual_training manual_training.cpp)
# add_executable(OpenCvTest opencvtest.cpp)
# target_link_libraries(OpenCvTest stdc++fs camera camera-base event event_pthreads Threads::Threads ${OpenCV_LIBS})


target_link_libraries( Raven "${OpenCV_LIBS}" armadillo lgpio RavenLib)
target_link_libraries( TrainRaven "${OpenCV_LIBS}" armadillo lgpio RavenLib)
target_link_libraries( ReinforceRaven "${OpenCV_LIBS}" armadillo lgpio RavenLib )
#target_link_libraries( CollectRaven "${OpenCV_LIBS}"  lgpio RavenLib)
#target_link_libraries( CmdServo lgpio RavenLib)
#target_link_libraries( MovePercentage lgpio RavenLib)
target_link_libraries( Manual_training "${OpenCV_LIBS}" lgpio RavenLib)
# target_link_libraries( OpenCvTest ${LIBCAMERA_LIBRARIES} "${OpenCV_LIBS}" -llccv)
