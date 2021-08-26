# Test data informations

## handshake.bin
- Handshake C0+C1
    - Protocol version: 03
    - Handshake data: (1528 bytes)

## setchunksize.bin
- RTMP Header
    - Format: 0
        - 00xx xxxx
    - Chunk Stream ID: 2
        - xx00 0010
    - Timestamp: 0
    - Body size: 4
    - Type ID: Set Chunk Size (0x01)
    - Stream ID: 0
- RTMP Body:
    - Chunk size: 4096

## videodata1.bin
- RTMP Header
    - Format: 0
        - 00xx xxxx
    - Chunk Stream ID: 4
        - xx00 0100
    - Timestamp: 0
    - Body size: 59
    - Type ID: Video Data (0x09)
- RTMP Body
    - Control: 0x17 (keyframe H.264)
        - Type: keyframe (1)
            - 0001 xxxx
        - Format: H.264 (7)
            - xxxx 0111
    - Video data: 
        - 0000000001640033ffe1002767640033ac2cac02800b5b016a02020a800001f40000ea6070000009896800002625a0dde5c14001000468ee3cb0

## audiodata1.bin
- RTMP Header
    - Format: 0
        - 00xx xxxx
    - Chunk Stream ID: 4
        - xx00 0100
    - Timestamp delta: 0
    - Type ID: Audio Data (0x08)
    - Stream ID: 1
- RTMP Body
    - Control: 0xaf (HE-AAC 44kHz 16 bit stereo)
        - Format: HE-AAC (10)
            - 1010 xxxx
        - Sample rate: 44kHz (3)
            - xxxx 11xx
        - Sample size: 16 bit (1)
            - xxxx xx1x
        - Channels: stereo (1)
            - xxxx xxx1
    - Audio data: 00121056e500
