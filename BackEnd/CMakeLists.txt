# Handle linux libraries
if (UNIX)
  find_package(
    Threads) # include this package to fix problem 001
else ()
endif ()

message(STATUS "BackEnd - CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")

# Handle boost libraries
find_package(Boost REQUIRED
	log
	system
	filesystem
	date_time
	log_setup
	chrono
	regex
  thread)

message(STATUS "Backend - Boost library status:")
message(STATUS "Backend -     version: ${BoosT_VERSION}")
message(STATUS "Backend -     libraries: ${Boost_LIBRARIES}")
message(STATUS "Backend -     include path: ${Boost_INCLUDE_DIR}")

# Handle QT libraries
find_package(Qt5 REQUIRED
	Core
	Gui
	Widgets)
message(STATUS "BackEnd - QT library status:")
message(STATUS "BackEnd -     version: ${Qt5Widgets_VERSION}")
message(STATUS "BackEnd -     libraries: ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Core_QTMAIN_LIBRARIES} ${Qt5Gui_LIBRARIES}")
message(STATUS "BackEnd -     include path: ${QT_INCLUDE_DIR}")

file(GLOB BackEnd_src
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.cpp"
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.h"
  "${PROJECT_SOURCE_DIR}/BackEnd/*.cpp"
  "${PROJECT_SOURCE_DIR}/BackEnd/*.h")

include_directories(${libpcap_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/FrontEnd
  ${PROJECT_SOURCE_DIR}/BackEnd
  ${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine
  ${PROJECT_SOURCE_DIR}/Utils
  ${QT_INCLUDE_DIR}/QtWidgets
  ${Boost_INCLUDE_DIR})

# variable checking
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
#   message(STATUS "Checking - dir='${dir}'")
# endforeach()

message(STATUS "BackEnd - libpcap include path: ${libpcap_INCLUDE_DIR}")

add_executable(PcapReplayer ${BackEnd_src})
add_dependencies(PcapReplayer FrontEnd Utils)

if (UNIX)
  file(GLOB BackEnd_lib
    ${PROJECT_BINARY_DIR}/FrontEnd/libFrontEnd.a
    ${PROJECT_BINARY_DIR}/Utils/libUtils.a
    ${libpcap_LIBRARY_DIR}/libpcap.a)
  target_link_libraries(PcapReplayer
    debug ${BackEnd_lib}
    optimized ${BackEnd_lib}
    ${CMAKE_THREAD_LIBS_INIT}) # include this package to fix problem 001
else ()
  file(GLOB BackEnd_lib
    ${PROJECT_BINARY_DIR}/FrontEnd/libFrontEnd.lib
    ${PROJECT_BINARY_DIR}/Utils/libUtils.lib)
  target_link_libraries(PcapReplayer
    debug ${BackEnd_lib}
    optimized ${BackEnd_lib})
endif()
target_link_libraries(PcapReplayer
  ${Qt5Core_LIBRARIES}
  ${Qt5Core_QTMAIN_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Boost_LIBRARIES})

# Creates a folder "executables" and adds target
# project (*.vcproj) under it
set_property(TARGET PcapReplayer PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy *.exe to destination directory
install (TARGETS PcapReplayer
  RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
  LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin
  ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin)

# Problem 001
#        /usr/bin/ld: ... undefined reference to symbol 'pthread_rwlock_wrlock@@GLIBC_2.2.5'
