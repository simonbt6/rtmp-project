compile_test_windows:
	echo "Compiling RTMP lib for Windows..."
	cl test/test.cpp /std:c++17 src/core/RTMPParser.cpp /o'bin\test.exe' /EHsc
	rm test.obj RTMPParser.obj

compile_server_windows:
	echo "Compiling win32_server for Windows..."
	cl test/win32_server.cpp /o'bin\win32_server.exe'
	rm win32_server.obj

compile_test_clang:
	echo "Compiling RTMP lib test with CLANG"
	c++ test/test.cpp -std=c++17 src/core/RTMPParser.cpp -o 'bin/test'

compile_test_mac:
	echo "Compiling RTMP lib test for MacOS..."
	c++ test/test.cpp -std=c++17 src/core/RTMPParser.cpp -o 'bin/test.out'

