cmake_minimum_required(VERSION 2.8)
project(3DSLAM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -pthread ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread ")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

set(CMAKE_CXX_FLAGS "${C_CXX_FLAGS_DEFAULT} ${CMAKE_CXX_FLAGS} --std=c++17 ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

#sudo apt-get install libglew-dev
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})


#find additional libraries GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS} ${GLFW_LIBRARIES})
#sudo apt-get install freeglut3-dev
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

#3Dviz 
include_directories( ${PROJECT_SOURCE_DIR}/modules/3Dviz_SLAM/include)

include_directories( ${PROJECT_SOURCE_DIR}/modules/data_loader/include)



#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/g2o/cmake_modules)
find_package(G2O REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(CSparse REQUIRED)
find_package(Cholmod REQUIRED)

include_directories( SYSTEM
  	${CMAKE_CURRENT_SOURCE_DIR}
  	${EIGEN3_INCLUDE_DIR}
  	${CSPARSE_INCLUDE_DIR}
  	${Cholmod_INCLUDE_DIR}
  	${G2O_INCLUDE_DIR}
  	${G2O_INCLUDE_DIRS}
  	/usr/include/suitesparse
  	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/imgui
	${PROJECT_SOURCE_DIR}/test/pbplot_tools

)


LIST(APPEND G2O_LIBS
  cxsparse
  cholmod
  g2o_cli g2o_ext_freeglut_minimal g2o_simulator
  g2o_solver_slam2d_linear g2o_types_icp g2o_types_slam2d
  g2o_core g2o_interface g2o_solver_csparse g2o_solver_structure_only
  g2o_types_sba g2o_types_slam3d g2o_csparse_extension
  g2o_opengl_helper g2o_solver_dense g2o_stuff
  g2o_types_sclam2d g2o_parser g2o_solver_pcg
  g2o_types_data g2o_types_sim3
)

#configuration for openCV
find_package( OpenCV REQUIRED )

#building main 

add_executable( app 
		app/app.cpp
		
		src/parameters.cpp
		src/message.cpp
		#src/slam_node.cpp
		src/map_builder.cpp
		src/node.cpp
		src/features2D.cpp
		src/utils.cpp
		src/motion_model.cpp
		src/stereo3D.cpp
		src/functions.cpp
		src/computeSE3.cpp
		src/localBA.cpp
		src/em.cpp
		src/P3D.cpp
		src/stereoP3D.cpp

		modules/3Dviz_SLAM/src/viz.cpp
		modules/3Dviz_SLAM/src/glad.c 
		modules/3Dviz_SLAM/src/axes.cpp
		modules/3Dviz_SLAM/src/pose.cpp	
		
		modules/data_loader/src/data_loader.cpp	

		) 

target_link_libraries(app 
			${G2O_LIBS} 
			${G2O_LIBRARIES} 
			${OpenCV_LIBS}
			${GLFW_STATIC_LIBRARIES} 
			${OPENGL_LIBRARIES} 
			${GLUT_LIBRARY})

		
