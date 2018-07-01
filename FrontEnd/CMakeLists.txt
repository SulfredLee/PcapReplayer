set(targetName "FrontEnd")
get_filename_component(folderName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" folderName ${folderName})

# Handle QT libraries
file(GLOB ${folderName}_src_proxy "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB ${folderName}_hdr_proxy "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB ${folderName}_ui_proxy "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
set(${folderName}_src ${FrontEnd_src_proxy})
set(${folderName}_hdr ${FrontEnd_hdr_proxy})
set(${folderName}_ui  ${FrontEnd_ui_proxy})
set(${folderName}_rcc ${CMAKE_CURRENT_SOURCE_DIR}/resource.qrc)
qt5_wrap_cpp(${folderName}_hdr_moc ${${folderName}_hdr})
qt5_wrap_ui (${folderName}_ui_moc  ${${folderName}_ui})
qt5_add_resources(${folderName}_rcc_moc ${${folderName}_rcc})

# handle other resource
file(GLOB PlayerEngine_inc
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.h")
file(GLOB PlayerEngine_src
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.cpp")

include_directories(${libpcap_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine
  ${PROJECT_SOURCE_DIR}/Utils
  ${PROJECT_BINARY_DIR}/FrontEnd
  ${QT_INCLUDE_DIR}/QtWidgets
  ${Boost_INCLUDE_DIR})

add_library(${targetName}
  ${${folderName}_src}
  ${${folderName}_hdr}
  ${${folderName}_hdr_moc}
  ${${folderName}_ui_moc}
  ${${folderName}_rcc_moc}
  ${PlayerEngine_src})

target_link_libraries(${targetName}
  ${Qt5Widgets_LIBRARIES}
  Utils)

# Creates a folder "libraries" and adds target project (*.vcproj) under it
set_property(TARGET ${targetName} PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy *.lib to destination directory
install (TARGETS ${targetName} DESTINATION bin)
install (FILES ${${folderName}_hdr_proxy} DESTINATION include)
