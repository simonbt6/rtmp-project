# Test data informations

## handshakeC0C1.bin
- Handshake C0+C1
    - Protocol version: 03
    - Time: 13 B8 5A 18
    - Zeros: 00 00 00 00
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

## createstream.bin
- RTMP Header
    - Format: 1
        - 00xx xxxx
    - Chunk Stream ID: 3
        - xx00 0011
    - Timestamp delta: 0
    - Timestamp: 0
    - Body size: 25
    - Type ID: AMF0 Command (0x14)
- RTMP Body
    - String 'createStream'
        - AMF0 type: String (0x02)
        - String Length: 12
        - String: createStream
    - Number 4
        - AMF0 type: Number (0x00)
        - Number: 4
    - Null
        - AMF0 type: Null (0x05)

## _result.bin
- RTMP Header
    - Format: 0
        - 00xx xxxx
    - Chunk Stream ID: 3
        - xx00 0011
    - Timestamp: 0
    - Body size: 29
    - Type ID: AMF0 Command (0x14)
    - Stream ID: 0
- RTMP Body
    - String '_result'
        - AMF0 type: String (0x02)
        - String Length: 7
        - String: _result
    - Number 4
        - AMF0 type: Number (0x00)
        - Number: 4
    - Null
        - AMF0 type: Null (0x05)
    - Number 1
        - AMF0 type: Number (0x00)
        - Number: 1

## publishtest.bin
- RTMP Header
    - Format: 0
        - 00xx xxxx
    - Chunk Stream ID: 4
        - xx00 0100
    - Timestamp: 0
    - Body size: 34
    - Type ID: AMF0 Command (0x14)
    - Stream ID: 1
- RTMP Body
    - String 'publish'
        - AMF0 type: String (0x02)
        - String Length: 7
        - String: publish
    - Number 5
        - AMF0 type: Number (0x00)
        - Number: 5
    - Null
        -AMF0 type: Null (0x05)
    - String 'test'
        - AMF0 type: String (0x02)
        - String Length: 4
        - String: test
    - String 'live'
        - AMF0 type: String (0x02)
        - String Length: 4
        - String: live
