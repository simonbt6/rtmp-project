# RTMP Lib C++ implementation.

# Requirements:
- CMake 3.8+
- C/C++ compiler installed & on path.

# Installation:
- Clone repository.  
```git clone https://github.com/simonbt6/rtmp-lib.git```
- Compile desired target.    
    - Build all targets:  
```cmake --build . ```  
    - Build windows server:  
```cmake --build . --target server```
    - Build handshake test data:  
```cmake --build . --target test_handshake```