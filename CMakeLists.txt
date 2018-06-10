cmake_minimum_required (VERSION 2.8.12)

# Maps to a solution filed (*.sln). The solution will
# have all targets (exe, lib, dll) as projects (.vcproj)
project(PcapReplayer)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
set(CMAKE_CXX_FLAGS "-Wall -fPIC -std=c++11")
message(STATUS "Root - cxx Flags: " ${CMAKE_CXX_FLAGS})

# Handle Preprocess Flags
if (UNIX)
  add_definitions(-DUNIX)
else ()
  add_definitions(-DWINDOWS -DWIN32 "/EHsc")
endif ()


# Handle Boost libaries
if (UNIX)
  set(Boost_INCLUDE_DIR "/usr/local/boost/1.59.0/include")
  set(Boost_LIBRARY_DIR "/usr/local/boost/1.59.0/lib")
else ()
  set(Boost_INCLUDE_DIR "C:\\boost\\1.59.0")
  set(Boost_LIBRARY_DIR "C:\\boost\\1.59.0\\lib64-msvc-12.0")
endif ()
set(Boost_USE_STATIC_LIBS   ON)

# Handle QT libaries
if (UNIX)
  set(QT_ROOT "/usr/local/Qt/5.6.2/5.6/gcc_64/")
else ()
  set(QT_ROOT "C:\\Qt\\5.6.2\\5.6\\msvc2013_64\\")
endif ()
set(QT_INCLUDE_DIR "${QT_ROOT}include")
set(QT_LIBRARY_DIR "${QT_ROOT}lib")
set(CMAKE_PREFIX_PATH ${QT_ROOT})

# Handle pcap libaries
if (UNIX)
  set(libpcap_ROOT "/usr/local/libpcap/1.8.1/")
else ()
  set(libpcap_ROOT "C:\\WpdPack\\4.1.2\\")
endif ()
set(libpcap_INCLUDE_DIR "${libpcap_ROOT}include")
set(libpcap_LIBRARY_DIR "${libpcap_ROOT}lib")

# Sub-directories where more CMakeLists.txt exist
add_subdirectory(FrontEnd)
add_subdirectory(BackEnd)
add_subdirectory(Utils)
