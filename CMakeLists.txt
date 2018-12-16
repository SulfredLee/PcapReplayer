cmake_minimum_required (VERSION 2.8.12)

# Maps to a solution filed (*.sln). The solution will
# have all targets (exe, lib, dll) as projects (.vcproj)
project(PcapReplayer)

# The version number.
set (Project_VERSION_MAJOR 1)
set (Project_VERSION_MINOR 0)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
set(CMAKE_CXX_FLAGS "-Wall -fPIC -std=c++11 -g")
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
find_package(Boost 1.59.0 REQUIRED
  log
  system
  filesystem
  date_time
  log_setup
  chrono
  regex
  thread
  serialization)
message(STATUS "Info - Boost library status:")
message(STATUS "Info -     version: ${Boost_VERSION}")
message(STATUS "Info -     libraries: ${Boost_LIBRARIES}")
message(STATUS "Info -     include path: ${Boost_INCLUDE_DIR}")

# Handle QT libaries
if (UNIX)
  set(QT_ROOT "/usr/local/Qt/5.6.2/5.6/gcc_64/")
else ()
  set(QT_ROOT "C:\\Qt\\5.6.2\\5.6\\msvc2013_64\\")
endif ()
set(QT_INCLUDE_DIR "${QT_ROOT}include")
set(QT_LIBRARY_DIR "${QT_ROOT}lib")
set(CMAKE_PREFIX_PATH ${QT_ROOT})
# Handle QT libraries
find_package(Qt5 REQUIRED
  Core
  Gui
  Widgets)
message(STATUS "Info - QT library status:")
message(STATUS "Info -     version: ${Qt5Widgets_VERSION}")
message(STATUS "Info -     libraries: ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Core_QTMAIN_LIBRARIES} ${Qt5Gui_LIBRARIES}")
message(STATUS "Info -     include path: ${QT_INCLUDE_DIR}")

# Handle linux libraries
if (UNIX)
  find_package(
    Threads) # include this package to fix problem 001
else ()
endif ()
message(STATUS "Info - CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")

# Handle pcap libaries
if (UNIX)
  set(libpcap_ROOT "/usr/local/libpcap/1.8.1/")
else ()
  set(libpcap_ROOT "C:\\WpdPack\\4.1.2\\")
endif ()
set(libpcap_INCLUDE_DIR "${libpcap_ROOT}include")
set(libpcap_LIBRARY_DIR "${libpcap_ROOT}lib")
message(STATUS "Info - libpcap include path: ${libpcap_INCLUDE_DIR}")

# Handle rpath while installation
if (UNIX)
  set(CMAKE_SKIP_BUILD_RPATH  FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  # the RPATH to be used when installing, but only if it's not a system directory
  LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  IF("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  ENDIF("${isSystemDir}" STREQUAL "-1")
else ()
endif ()

# Sub-directories where more CMakeLists.txt exist
add_subdirectory(FrontEnd)
add_subdirectory(BackEnd)
add_subdirectory(Utils)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Project_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Project_VERSION_MINOR}")
include (CPack)

# Problem 001
#        /usr/bin/ld: ... undefined reference to symbol 'pthread_rwlock_wrlock@@GLIBC_2.2.5'
