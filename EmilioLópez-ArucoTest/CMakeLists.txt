cmake_minimum_required(VERSION 3.0)
PROJECT( aruco_test )

#SET(LIBS /usr/local/lib/libfltk.a /usr/lib/x86_64-linux-gnu/libXext.a /usr/lib/
#libX11.a /usr/lib/x86_64-linux-gnu/libdl.a /usr/lib/x86_64-linux-gnu/libm.a)

#SET(LIBS /usr/local/lib/libfltk.a /usr/lib/x86_64-linux-gnu/libXcursor.a /usr/lib/x86_64-linux-gnu/libXfixes.a 
#/usr/lib/x86_64-linux-gnu/libXext.a /usr/lib/x86_64-linux-gnu/libXft.a /usr/lib/x86_64-linux-gnu/libfontconfig.a
#/usr/lib/x86_64-linux-gnu/libXinerama.a /usr/lib/x86_64-linux-gnu/libpthread.a /usr/lib/x86_64-linux-gnu/libdl.a
#/usr/lib/x86_64-linux-gnu/libm.a /usr/lib/x86_64-linux-gnu/libX11.a)
SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
find_package(FLTK)
find_package(aruco REQUIRED )

find_package(GLUT REQUIRED )
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#add_library(aruco_test_lib STATIC ${aruco_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FLTK_LIBRARIES} ${CMAKE_DL_LIBS})
#set(libr ${aruco_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FLTK_LIBRARIES} ${CMAKE_DL_LIBS})
#add_library(aruco_test_lib STATIC ${libr})
add_library(aruco_lib STATIC Point3D.cpp TypeV.cpp TypeF.cpp)
#ADD_EXECUTABLE(aruco_test aruco_test.cpp Point3D.cpp TypeV.cpp TypeF.cpp)
ADD_EXECUTABLE(aruco_test aruco_test.cpp)

TARGET_LINK_LIBRARIES(aruco_test aruco_lib aruco_lib_so ${aruco_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FLTK_LIBRARIES} ${CMAKE_DL_LIBS})

#ADD_DEPENDENCIES(aruco_test aruco_lib)

#TARGET_LINK_LIBRARIES(aruco_test ${aruco_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FLTK_LIBRARIES} ${CMAKE_DL_LIBS})
