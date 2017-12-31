# Handle QT libraries
find_package(Qt5 REQUIRED
  Widgets)
message(STATUS "FrontEnd - QT library status:")
message(STATUS "FrontEnd -     version: ${Qt5Widgets_VERSION}")
message(STATUS "FrontEnd -     libraries: ${Qt5Widgets_LIBRARIES}")
message(STATUS "FrontEnd -     include path: ${QT_INCLUDE_DIR}")

set(FrontEndPath ${PROJECT_SOURCE_DIR}/FrontEnd)
file(GLOB FrontEnd_src_proxy "${FrontEndPath}/*.cpp")
file(GLOB FrontEnd_hdr_proxy "${FrontEndPath}/*.h")
file(GLOB FrontEnd_ui_proxy "${FrontEndPath}/*.ui")
set(FrontEnd_src ${FrontEnd_src_proxy})
set(FrontEnd_hdr ${FrontEnd_hdr_proxy})
set(FrontEnd_ui  ${FrontEnd_ui_proxy})

qt5_wrap_cpp(FrontEnd_hdr_moc ${FrontEnd_hdr})
qt5_wrap_ui (FrontEnd_ui_moc  ${FrontEnd_ui})
qt5_add_resources(FrontEnd_rcc_moc ${FrontEnd_rcc})

# handle additional custome classes
file(GLOB PlayerEngine_src
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.cpp"
  "${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine/*.h")

include_directories(${libpcap_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/FrontEnd
  ${PROJECT_SOURCE_DIR}/BackEnd/PlayerEngine
  ${PROJECT_SOURCE_DIR}/Utils
  ${PROJECT_BINARY_DIR}/FrontEnd
  ${QT_INCLUDE_DIR}/QtWidgets
  ${Boost_INCLUDE_DIR})

add_library(FrontEnd
  ${FrontEnd_src}
  ${FrontEnd_hdr}
  ${FrontEnd_hdr_moc}
  ${FrontEnd_ui_moc}
  ${FrontEnd_rcc_moc}
  ${PlayerEngine_src})

target_link_libraries(FrontEnd
  ${Qt5Widgets_LIBRARIES}
  Utils)

# Creates a folder "libraries" and adds target project (*.vcproj) under it
set_property(TARGET FrontEnd PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy *.lib to destination directory
install (TARGETS FrontEnd
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin
		 ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin)
