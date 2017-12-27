# Handle boost libraries
find_package (Boost REQUIRED
	log
	system
	filesystem
	date_time
	log_setup
	chrono
	regex
  thread
)

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
message(STATUS "BackEnd -     version: ${QT_VERSION}")
message(STATUS "BackEnd -     libraries: ${QT_LIBRARIES}")
message(STATUS "BackEnd -     include path: ${QT_INCLUDE_DIR}")

file(GLOB BackEnd_src
  "${PROJECT_SOURCE_DIR}/BackEnd/*.cpp"
  "${PROJECT_SOURCE_DIR}/BackEnd/*.h"
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.cpp"
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.h")

include_directories(${PROJECT_SOURCE_DIR}/FrontEnd
  ${PROJECT_SOURCE_DIR}/BackEnd
  ${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine
  ${PROJECT_SOURCE_DIR}/Utils
  ${QT_INCLUDE_DIR}/QtWidgets
  ${Boost_INCLUDE_DIR})

add_executable(PcapReplayer ${BackEnd_src})
add_dependencies(PcapReplayer FrontEnd Utils)

if (UNIX)
  file(GLOB FrontEnd_lib
    ${PROJECT_BINARY_DIR}/FrontEnd/*.a
    ${PROJECT_BINARY_DIR}/Utils/*.a)
  target_link_libraries(PcapReplayer
    debug ${FrontEnd_lib}
    optimized ${FrontEnd_lib})
else ()
  file(GLOB FrontEnd_lib
    ${PROJECT_BINARY_DIR}/FrontEnd/*.lib
    ${PROJECT_BINARY_DIR}/Utils/*.lib)
  target_link_libraries(PcapReplayer
    debug ${FrontEnd_lib}
    optimized ${FrontEnd_lib})
endif()
target_link_libraries(PcapReplayer
  Qt5::Widgets
  ${Boost_LIBRARIES})

# Creates a folder "executables" and adds target
# project (*.vcproj) under it
set_property(TARGET PcapReplayer PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy *.exe to destination directory
install (TARGETS PcapReplayer
  RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
  LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin
  ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin)
