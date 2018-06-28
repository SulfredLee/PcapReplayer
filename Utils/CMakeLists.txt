set(targetName "Utils")
set(folderName "Utils")

file(GLOB ${folderName}_inc
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB ${folderName}_src
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIR})

# state that this project is a library
add_library(${targetName} ${${folderName}_src}) # static library
# add_library(Utils SHARED ${${folderName}_src}) # dynamic library

# Adds logic to INSTALL.vcproj to copy *.a to destination directory
install (TARGETS ${targetName} DESTINATION bin)
install (FILES ${${folderName}_inc} DESTINATION include)
