test_server:
	ffmpeg -re -i C:/Users/simon/Documents/costarica.mp4 -vcodec libx264 -profile:v main -preset:v medium -r 30 -g 60 -keyint_min 60 -sc_threshold 0 -b:v 2500k -maxrate 2500k -bufsize 2500k -filter:v scale="trunc(oha/2)2:720" -sws_flags lanczos+accurate_rnd -acodec ac3_fixed -b:a 96k -ar 48000 -ac 2 -f flv rtmp://127.0.0.1:1935/live/mystream
