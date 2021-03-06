cmake_minimum_required(VERSION 3.3)
project(tomviz)

set(CMAKE_MODULE_PATH "${tomviz_SOURCE_DIR}/cmake")

# Request C++11 standard, using new CMake variables.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

include(BuildType)
include(BuildLocation)
include(CompilerFlags)
include(tomvizDetermineVersion)
include(Git)

determine_version("${tomviz_SOURCE_DIR}" "${GIT_EXECUTABLE}" "tomviz")
# Hard coded for source tarballs, releases, etc.
set(tomviz_version_major 0)
set(tomviz_version_minor 9)
set(tomviz_version_patch 4)
set(tomviz_version_extra)
set(tomviz_version
  "${tomviz_version_major}.${tomviz_version_minor}.${tomviz_version_patch}")
if(tomviz_VERSION)
  set(tomviz_version_major ${tomviz_VERSION_MAJOR})
  set(tomviz_version_minor ${tomviz_VERSION_MINOR})
  set(tomviz_version_patch ${tomviz_VERSION_PATCH})
  set(tomviz_version_extra ${tomviz_VERSION_PATCH_EXTRA})
endif()
set(tomviz_version
  "${tomviz_version_major}.${tomviz_version_minor}.${tomviz_version_patch}")

# Location where python modules will be installed.
set(tomviz_python_install_dir "lib/tomviz/site-packages")
if(APPLE)
  set(tomviz_python_install_dir "Applications/tomviz.app/Contents/Python")
endif()

# Location where python modules will be copied to in binary tree.
set(tomviz_python_binary_dir "${tomviz_BINARY_DIR}/lib/site-packages")

# Location where sample data will be installed.
set(tomviz_data_install_dir "share/tomviz")
if(APPLE)
  set(tomviz_data_install_dir "Applications/tomviz.app/Contents/share/tomviz")
endif()

set(PYBIND11_PYTHON_VERSION "2.7" CACHE STRING "")
set(PYBIND11_CPP_STANDARD "-std=c++11" CACHE STRING "")
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/pybind11)

# These dependencies are inherited from ParaView.
find_package(Qt5 REQUIRED COMPONENTS Network Widgets)
find_package(ParaView REQUIRED)

# Use automoc, autouic, and autorcc for our Qt code.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(NOT PARAVIEW_BUILD_QT_GUI)
  message(FATAL_ERROR
    "Tomviz requires PARAVIEW_BUILD_QT_GUI to be enabled. "
    "Please rebuild ParaView with PARAVIEW_BUILD_QT_GUI set to TRUE.")
endif()
if(NOT PARAVIEW_ENABLE_PYTHON)
  message(FATAL_ERROR
    "Tomviz requires PARAVIEW_ENABLE_PYTHON to be enabled. "
    "Please rebuild ParaView with PARAVIEW_ENABLE_PYTHON set to TRUE.")
endif()
if(NOT PARAVIEW_QT_VERSION STREQUAL "5")
  message(FATAL_ERROR
    "Tomviz requires PARAVIEW_QT_VERSION to be 5, please rebuild ParaView.")
endif()

find_package(ITK 4.9)
if(ITK_FOUND AND NOT ITK_WRAP_PYTHON)
  message(FATAL_ERROR
    "Tomviz requires ITK_WRAP_PYTHON to be enabled. "
    "Please rebuild ITK with ITK_WRAP_PYTHON set to TRUE.")
endif()

add_subdirectory(tomviz)

option(ENABLE_TESTING "Enable testing and building the tests." OFF)
if(ENABLE_TESTING)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

# -----------------------------------------------------------------------------
# Add web application
# -----------------------------------------------------------------------------
option(TOMVIZ_DOWNLOAD_WEB  "Enable downloading web application." OFF)
set(tomviz_html_source_path "${CMAKE_SOURCE_DIR}/web/tomviz.html")
set(tomviz_html_binary_dir "${CMAKE_BINARY_DIR}/web/www")
set(tomviz_html_binary_path "${tomviz_html_binary_dir}/tomviz.html")


if (TOMVIZ_DOWNLOAD_WEB)
  set(tomviz_html_sha512 "d0a54ea61a53d941e22c43cd75394d9f5f8cffd8703b9f4be7f72dc869ec9f41232c88a66cf796bbb9c1c7988bbb8e00e0bf8421e592232000b1e54cccf4a45c")
  if (NOT EXISTS "${tomviz_html_source_path}")
    message(STATUS "Downloading tomviz.html.")
    file(DOWNLOAD
      "https://data.kitware.com/api/v1/file/hashsum/sha512/${tomviz_html_sha512}/download"
      "${tomviz_html_binary_path}"
      EXPECTED_HASH SHA512=${tomviz_html_sha512}
      SHOW_PROGRESS
    )
  endif()
endif()

set(tomviz_web_install_dir "share/tomviz/web")
if(APPLE)
  set(tomviz_web_install_dir "Applications/tomviz.app/Contents/share/tomviz/web")
endif()

if (EXISTS "${tomviz_html_source_path}")
    file(COPY "${tomviz_html_source_path}" DESTINATION "${tomviz_html_binary_dir}")
endif()

if(EXISTS "${tomviz_html_binary_path}")
  install(
    FILES "${tomviz_html_binary_path}"
    DESTINATION "${tomviz_web_install_dir}"
    COMPONENT runtime)
endif()
