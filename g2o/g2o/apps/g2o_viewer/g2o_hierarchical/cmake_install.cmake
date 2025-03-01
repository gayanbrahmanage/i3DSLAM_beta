# Install script for directory: /home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_viewer/g2o_hierarchical/libhierarchical.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/apps/g2o_hierarchical/backbone_tree_action.h;/apps/g2o_hierarchical/edge_creator.h;/apps/g2o_hierarchical/edge_labeler.h;/apps/g2o_hierarchical/edge_types_cost_function.h;/apps/g2o_hierarchical/g2o_hierarchical_api.h;/apps/g2o_hierarchical/simple_star_ops.h;/apps/g2o_hierarchical/star.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/apps/g2o_hierarchical" TYPE FILE FILES
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/backbone_tree_action.h"
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/edge_creator.h"
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/edge_labeler.h"
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/edge_types_cost_function.h"
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/g2o_hierarchical_api.h"
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/simple_star_ops.h"
    "/home/bgs/3DSLAM22/3DSLAMv2.0/g2o/g2o/apps/g2o_hierarchical/star.h"
    )
endif()

