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




find_package(PkgConfig REQUIRED)
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
	${PROJECT_SOURCE_DIR}/modules/gui/include

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
pkg_search_module(GTKMM REQUIRED gtkmm-3.0)

include_directories(SYSTEM
  ${GTKMM_INCLUDE_DIRS}
  ${SIGCPP_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS})
  
link_directories(${GTKMM_LIBRARY_DIRS})

#building main 

add_executable( app 
		app/app.cpp
		
		src/parameters.cpp
		src/message.cpp
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

	
		modules/gui/src/gui.cpp
		modules/gui/src/video_widget.cpp
		modules/gui/src/i3D.cpp
		
	
		modules/data_loader/src/data_loader.cpp	

		) 

target_link_libraries(app 
			${G2O_LIBS} 
			${G2O_LIBRARIES} 
			${OpenCV_LIBS}
			${GTKMM_LIBRARIES}
			epoxy)

		
