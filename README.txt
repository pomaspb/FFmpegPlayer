FFmpeg 2.6.3 compilation steps
Target platform iOS 7/8. Tested on Mac mini with OS X 10.10.3

1. Download FFmpeg library
wget https://www.ffmpeg.org/releases/ffmpeg-2.6.3.tar.gz

2. Extract library
tar xvfz ffmpeg-2.6.3.tar

3. Get build script
wget https://raw.githubusercontent.com/kewlbear/FFmpeg-iOS-build-script/master/build-ffmpeg.sh

4. Make it executable
chmod +x build-ffmpeg.sh

5. Change in the script line:
SOURCE="ffmpeg-2.7.1"
to:
SOURCE="ffmpeg-2.6.3"

6. Download yasm source code
wget http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz

7. Unpack yasm source code
tar xvzf yasm-1.2.0.tar.gz

8. Configure and build yasm
cd yasm-1.2.0
./configure && make -j 4 && sudo make install
cd ..

9. Build FFmpeg library
./build-ffmpeg.sh

It will install gas-preprocessor.pl (https://github.com/libav/gas-preprocessor/raw/master/gas-preprocessor.pl) automatically. Ready to use iOS libraries are placed to Fmpeg-iOS/lib/lib/ folder.
