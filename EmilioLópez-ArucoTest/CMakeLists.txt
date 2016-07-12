cmake_minimum_required(VERSION 2.8)

project(glui)
SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
find_package(aruco REQUIRED )
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLUT_INCLUDE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

SET(GLUI_SRCS
 glui_src/algebra3.cpp
 glui_src/arcball.cpp
 glui_src/glui_add_controls.cpp
 glui_src/glui_bitmap_img_data.cpp
 glui_src/glui_bitmaps.cpp
 glui_src/glui_button.cpp
 glui_src/glui_checkbox.cpp
 glui_src/glui_column.cpp
 glui_src/glui_commandline.cpp
 glui_src/glui_control.cpp
 glui_src/glui.cpp
 glui_src/glui_edittext.cpp
 glui_src/glui_filebrowser.cpp
 glui_src/glui_listbox.cpp
 glui_src/glui_list.cpp
 glui_src/glui_mouse_iaction.cpp
 glui_src/glui_node.cpp
 glui_src/glui_panel.cpp
 glui_src/glui_radio.cpp
 glui_src/glui_rollout.cpp
 glui_src/glui_rotation.cpp
 glui_src/glui_scrollbar.cpp
 glui_src/glui_separator.cpp
 glui_src/glui_spinner.cpp
 glui_src/glui_statictext.cpp
 glui_src/glui_string.cpp
 glui_src/glui_textbox.cpp
 glui_src/glui_translation.cpp
 glui_src/glui_tree.cpp
 glui_src/glui_treepanel.cpp
 glui_src/glui_window.cpp
 glui_src/quaternion.cpp
 glui_src/viewmodel.cpp
)
add_library(glui_obj OBJECT ${GLUI_SRCS})
# we need to enable -fPIC this so that the same object code can be used to
# create static *and* shared libraries without double compilation
set_property(TARGET glui_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(glui SHARED $<TARGET_OBJECTS:glui_obj>)
target_link_libraries(glui ${GLUT_LIBRARIES})

add_library(glui_static STATIC $<TARGET_OBJECTS:glui_obj>)
target_link_libraries(glui_static ${GLUT_LIBRARIES})

add_executable(ppm2array tools/ppm.cpp tools/ppm2array.cpp)
target_link_libraries(ppm2array)

add_executable(aruco_test aruco_test.cpp Point3D.cpp TypeV.cpp TypeF.cpp)
target_link_libraries(aruco_test glui_static ${aruco_LIBS} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
