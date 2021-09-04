compile_test_windows:
	echo "Compiling RTMP lib for Windows..."
	cl test/test.cpp /std:c++17 src/core/RTMPHandler.cpp src/core/RTMPParser.cpp /o'bin\test.exe' /EHsc
	rm test.obj RTMPParser.obj

compile_server_windows:
	echo "Compiling win32_server for Windows..."
	cl test/win32_server.cpp /std:c++17 src/core/RTMPHandler.cpp src/core/RTMPParser.cpp /o'bin\win32_server.exe' /EHsc
	rm win32_server.obj RTMPParser.obj

compile_test_clang:
	echo "Compiling RTMP lib test with CLANG"
	c++ test/test.cpp -std=c++17 src/core/RTMPHandler.cpp src/core/RTMPParser.cpp -o 'bin/test'

compile_test_mac:
	echo "Compiling RTMP lib test for MacOS..."
	c++ test/test.cpp -std=c++17 src/core/RTMPHandler.cpp src/core/RTMPParser.cpp -o 'bin/test.out'

test_server:
	ffmpeg -re -i C:/Users/simon/Documents/costarica.mp4 -vcodec libx264 -profile:v main -preset:v medium -r 30 -g 60 -keyint_min 60 -sc_threshold 0 -b:v 2500k -maxrate 2500k -bufsize 2500k -filter:v scale="trunc(oha/2)2:720" -sws_flags lanczos+accurate_rnd -acodec ac3_fixed -b:a 96k -ar 48000 -ac 2 -f flv rtmp://127.0.0.1:1935
