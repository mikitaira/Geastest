# Install script for directory: D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/BULLET_PHYSICS")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/HelloWorld/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/BasicDemo/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/ExampleBrowser/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/RobotSimulator/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/SharedMemory/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/ThirdPartyLibs/Gwen/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/ThirdPartyLibs/BussIK/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/ThirdPartyLibs/clsocket/cmake_install.cmake")
  include("D:/GeasSimulator-0.0.1/GeasEngine-0.0.1/bullet3-2.87/build/examples/OpenGLWindow/cmake_install.cmake")

endif()
