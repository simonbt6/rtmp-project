compile_test_windows:
	echo "Compiling RTMP lib for Windows..."
	cl test/test.cpp /std:c++17 src/core/RTMPParser.cpp src/core/Netconnection.cpp src/core/Netstream.cpp /o'bin\test.exe' /EHsc
	rm Netstream.obj Netconnection.obj test.obj RTMPParser.obj

compile_server_windows:
	echo "Compiling win32_server for Windows..."
	cl test/win32_server.cpp src/core/Netconnection.cpp src/core/Netstream.cpp /o'bin\win32_server.exe'
	rm Netstream.obj Netconnection.obj win32_server.obj

compile_test_mac:
	echo "Compiling RTMP lib test for MacOS..."
	c++ test/test.cpp src/core/Netconnection.cpp src/core/Netstream.cpp -o 'bin/test.out'