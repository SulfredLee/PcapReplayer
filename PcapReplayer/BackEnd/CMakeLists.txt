set(targetName "PcapReplayer")
get_filename_component(folderName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" folderName ${folderName})

file(GLOB ${folderName}_inc
  "${CMAKE_CURRENT_SOURCE_DIR}/PlayerEngine/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB ${folderName}_src
  "${CMAKE_CURRENT_SOURCE_DIR}/PlayerEngine/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories(${libpcap_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/PlayerEngine
  ${PROJECT_SOURCE_DIR}/FrontEnd
  ${PROJECT_SOURCE_DIR}/Utils
  ${QT_INCLUDE_DIR}/QtWidgets
  ${Boost_INCLUDE_DIR})

add_executable(${targetName} ${${folderName}_src})
add_dependencies(${targetName} FrontEnd Utils)

if (UNIX)
  target_link_libraries(${targetName}
    ${CMAKE_THREAD_LIBS_INIT} # include this package to fix problem 001
    debug ${libpcap_LIBRARY_DIR}/libpcap.so
    optimized ${libpcap_LIBRARY_DIR}/libpcap.so)
    # debug ${libpcap_LIBRARY_DIR}/libpcap.so.1.8.1
    # optimized ${libpcap_LIBRARY_DIR}/libpcap.so.1.8.1)
  # handle library files for installation
  file(GLOB libpcap_libFile "${libpcap_LIBRARY_DIR}/libpcap.so")
else ()
  set(TPN_WIN32_LIB ws2_32.lib)
  target_link_libraries(${targetName}
	  debug ${libpcap_LIBRARY_DIR}/x64/Packet.lib
	  debug ${libpcap_LIBRARY_DIR}/x64/wpcap.lib
	  optimized ${libpcap_LIBRARY_DIR}/x64/Packet.lib
	  optimized ${libpcap_LIBRARY_DIR}/x64/wpcap.lib
	  ${TPN_WIN32_LIB})
endif()
target_link_libraries(${targetName}
  Utils
  FrontEnd
  ${Qt5Core_LIBRARIES}
  ${Qt5Core_QTMAIN_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Boost_LIBRARIES})

# Creates a folder "executables" and adds target
# project (*.vcproj) under it
set_property(TARGET ${targetName} PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy *.exe to destination directory
install (TARGETS ${targetName} DESTINATION bin)
install (FILES ${libpcap_libFile} DESTINATION lib)
