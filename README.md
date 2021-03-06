**How to build DOSBox SVN (64-bit) in macOS**

(These instructions assume you are familiar with using the Terminal)

_**A. Install DOSBox Dependencies**_

1. Install Xcode command line tools:

		xcode-select --install

2. Install Homebrew

	https://docs.brew.sh/Installation

3. Install dependencies with Homebrew:

		brew install libpng

		brew install lzlib

		brew install sdl_net

		brew install sdl_sound
	
If you are using macOS 11 (Big Sur), you need this dependency as well:	

		brew install svn

_Packages should install by default to usr/local/Cellar_

_**B. Prepare the build folders**_

- These instructions result in the various build folders being installed on the desktop
- DOSBox working folder is 'build'
- SDL working folder is 'staticbuild'

1. Obtain the SVN source code

$ cd $HOME/Desktop
$ mkdir build
$ cd build
$ svn checkout https://github.com/almeath/DOSBox-SVN-64-bit-for-macOS
$ cd DOSBox-SVN-64-bit-for-macOS

2. Obtain autoconf

$ cd $HOME/Desktop/build
$ curl -LOs http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
$ tar xzpf autoconf-2.69.tar.gz
$ cd autoconf-2.69
$ ./configure
$ make
$ sudo make install

3. Obtain automake

$ cd $HOME/Desktop/build
$ curl -LOs http://ftp.gnu.org/gnu/automake/automake-1.15.tar.gz
$ tar xzpf automake-1.15.tar.gz
$ cd automake-1.15
$ ./configure
$ make
$ sudo make install

4. Download SDL 1.2x (Mercurial)

https://hg.libsdl.org/SDL/file/ab7529cb9558

- Download from 'zip' link at the top of the page
- Unzip the folder to the desktop and rename it to SDL

$ cd $HOME/Desktop/SDL
$ ./configure --prefix=$HOME/Desktop/staticbuild --enable-static --disable-shared --disable-video-x11
$ perl -p -i -e "/CGDirectPaletteRef palette;/d" ./src/video/quartz/SDL_QuartzVideo.h
$ make
$ make install

- Results in a folder called 'staticbuild' on your desktop

_**C. Build DOSBox**_

$ cd $HOME/Desktop/build/DOSBox-SVN-64-bit-for-macOS/
$ DOSBOXVERSION=$(svn log | head -2 | awk '/^r/ { print $1 }')
$ ./autogen.sh
$ ./configure --with-sdl-prefix=$HOME/Desktop/staticbuild/

D. Patch Make files to ensure a static build

1. Patch the primary Makefile

	•	The Makefile is located in $HOME/Desktop/build/DOSBox-SVN-64-bit-for-macOS/
	•	Open it in Xcode and edit at Line 241 (starts with LIBS =):

LIBS =  -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit -framework CoreMIDI /usr/local/Cellar/libpng/1.6.37/lib/libpng.a /usr/local/Cellar/zlib/1.2.11/lib/libz.a /usr/local/Cellar/sdl_net/1.2.8/lib/libSDL_net.a /usr/local/Cellar/sdl_sound/1.0.3_1/lib/libSDL_sound.a /usr/local/Cellar/libogg/1.3.4/lib/libogg.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbis.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisfile.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisenc.a

- "Your username" will be substituted with your 
- Check the path to each dependency to make sure it exactly matches what is in your user/local folder
- This presumes you installed dependencies to their default locations with homebrew (otherwise edit accordingly)
- The version numbers above may vary depending on when you installed the dependencies
- Check that line starting with "SDL_LIBS =" reads as follows:

SDL_LIBS = -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit

- As above, your username will be substituted in the highlights

2. Patch the SRC Makefile

- The SRC Make file is located in $HOME/Desktop/build/DOSBox-SVN-64-bit-for-macOS/src
- Open it in Xcode (or a text editor) and edit at the line starting with "LIBS =":

LIBS =  -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit -framework CoreMIDI -framework AudioUnit -framework AudioToolbox /usr/local/Cellar/libpng/1.6.37/lib/libpng.a /usr/local/Cellar/zlib/1.2.11/lib/libz.a /usr/local/Cellar/sdl_net/1.2.8/lib/libSDL_net.a /usr/local/Cellar/sdl_sound/1.0.3_1/lib/libSDL_sound.a /usr/local/Cellar/libogg/1.3.4/lib/libogg.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbis.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisfile.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisenc.a -framework OpenGL /usr/local/lib/libmt32emu.a

- These edits are identical to the primary make file, except you will see an extra path for "libmt32emu.a" to ensure the Munt static library is applied
- Check that Line 262 (starts with SDL_LIBS =) reads as follows:

SDL_LIBS = -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit

E. Compile DOSBox SVN

$ cd $HOME/Desktop/build/DOSBox-SVN-64-bit-for-macOS/
$ make -j4

- The DOSBox binary should successfully build and be located in $HOME/Desktop/build/dosbox-code-0/src
- Replace the binary in your DOSBox app under Contents/MacOS
