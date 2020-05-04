How to build Debug
	cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/target/location -DCMAKE_BUILD_TYPE=Debug
How to build Release
	cmake .. -DCMAKE_BUILD_TYPE=Release
How to build Ninja
  cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
Windows
	mkdir build
	cd build
	cmake.exe .. -G "Visual Studio 12 Win64"
How to make binary distribution
	cpack --config CPackConfig.cmake
How to make source distribution
	cpack --config CPackSourceConfig.cmake
