How to build Debug
	cmake .. -DCMAKE_BUILD_TYPE=Debug
How to build Release
	cmake .. -DCMAKE_BUILD_TYPE=Release
How to build Ninja
  cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
Windows
	mkdir build
	cd build
	cmake.exe .. -G "Visual Studio 12 Win64"
