cmake_minimum_required(VERSION 2.8)
project(3DSLAM)

# Specify the version of the project
set(i3DSLAM_VERSION_MAJOR 1)
set(i3DSLAM_VERSION_MINOR 0)
set(i3DSLAM_VERSION_PATCH 0)

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

# Define input and output files
set(GRESOURCE_XML ${CMAKE_CURRENT_SOURCE_DIR}/modules/gui/shader/gui.gresource.xml)
set(GENERATED_C ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
set(GENERATED_H ${CMAKE_CURRENT_BINARY_DIR}/resources.h)

add_custom_command(
    OUTPUT ${GENERATED_C} ${GENERATED_H}
    COMMAND glib-compile-resources ${GRESOURCE_XML} --target=${GENERATED_C} --generate-source --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND touch ${GENERATED_H}  # Touch the header file to ensure it's considered up to date
    DEPENDS ${GRESOURCE_XML}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Add custom target to ensure the resources are compiled before building the main target
add_custom_target(resources DEPENDS ${GENERATED_C} ${GENERATED_H})


#building main

add_executable( i3DSLAM
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
		src/utils2.cpp


		modules/gui/src/gui.cpp
		modules/gui/src/video_widget.cpp
		modules/gui/src/i3D.cpp
		modules/gui/src/logo.cpp
		modules/gui/src/gui_dispatcher.cpp
		modules/gui/src/axis.cpp
		modules/gui/src/i3D_widget.cpp
		modules/gui/src/axis.cpp
		modules/gui/src/grid.cpp
		modules/gui/src/camerapose.cpp
		modules/gui/src/trajectory.cpp
		modules/gui/src/gui_button_click.cpp


		modules/data_loader/src/data_loader.cpp
		${GENERATED_C}
		)

add_dependencies(i3DSLAM resources)


target_link_libraries(i3DSLAM
			${G2O_LIBS}
			${G2O_LIBRARIES}
			${OpenCV_LIBS}
			${GTKMM_LIBRARIES}
			epoxy)


add_executable( test
		test/testgl.cpp

		)

target_link_libraries(test
			${GTKMM_LIBRARIES}
			epoxy)

add_executable( test2
		test/testgl2.cpp

		)

target_link_libraries(test2 
			${GTKMM_LIBRARIES}
			epoxy)
			
install(TARGETS i3DSLAM DESTINATION /usr/local/bin)		

	
# CPack setup

# Function to scan for source files and exclude them
function(scan_for_source_files dir)
    # Get list of all files and directories in the current directory
    file(GLOB entries ${dir}/*)
    
    # Iterate over each entry
    foreach(entry ${entries})
        # Check if the entry is a directory
        if(IS_DIRECTORY ${entry})
            # Recursively call the function for the directory
            scan_for_source_files(${entry})
        else()
            # Check if the entry is a source file
            string(REGEX MATCH "\\.(cpp|h)$" is_source_file ${entry})
            if(is_source_file)
                # Add source file to the list of files to be excluded
                list(APPEND CPACK_SOURCE_IGNORE_FILES "${entry}")
            endif()
        endif()
    endforeach()
endfunction()

# Call the function with the root directory
scan_for_source_files(${CMAKE_CURRENT_SOURCE_DIR}/src)


set(CPACK_PACKAGE_NAME "i3DSLAM")
set(CPACK_PACKAGE_VERSION_MAJOR ${i3DSLAM_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${i3DSLAM_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${i3DSLAM_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${i3DSLAM_VERSION_MAJOR}.${i3DSLAM_VERSION_MINOR}.${i3DSLAM_VERSION_PATCH}")

set(CPACK_PACKAGE_DESCRIPTION "V-SLAM package by Gayan Brahmanage")
set(CPACK_PACKAGE_VENDOR "The university of Calgary")
set(CPACK_PACKAGE_CONTACT "gayansampathefac@gmail.com")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Linux")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

# Specify generator (DEB for Ubuntu)
set(CPACK_GENERATOR "DEB")

# DEB specific settings
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "gayansampathefac@gmail.com") # required

# Include license, readme, etc.
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
#set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")


# Exclude common files from the package
set(CPACK_SOURCE_IGNORE_FILES "${CPACK_SOURCE_IGNORE_FILES};/CVS/;/\\.svn/;/\\.bzr/;/\\.hg/;/\\.git/;\\.swp\\$;\\.\\#;/#;/\\.git/;/\\.gitignore;/\\.DS_Store;/\\.idea/;/build/;\\.cpp$;\\.h$")



# Create the package
include(CPack)			
