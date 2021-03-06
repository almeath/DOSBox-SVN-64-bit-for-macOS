A. Install DOSBox Dependencies

1. Install Xcode command line tools

$ xcode-select --install

2. Install Homebrew

https://docs.brew.sh/Installation

3. Install dependencies with Homebrew:

$ brew install cmake 
$ brew install libpng

$ brew install lzlib

$ brew install sdl_net

$ brew install sdl_sound

	•	Packages should install by default to usr/local/Cellar

B. Build Munt Dynamic Library

1. Install Munt

$ git clone https://github.com/munt/munt.git
$ cd munt
$ mkdir build
$ cd build
$ cmake -Dmunt_WITH_MT32EMU_SMF2WAV=OFF -Dmunt_WITH_MT32EMU_QT=OFF -Dlibmt32emu_SHARED=0 ../
$ make -j4
$ sudo make install

	•	Munt static library should install to usr/lib/libmt32emu.a
	•	Munt source files should install to 'munt' folder in your home folder

C. Prepare DOSBox SVN build folders

	•	These instructions result in the various build folders being installed on the desktop
	•	DOSBox working folder is 'build'
	•	SDL working folder is 'staticbuild'

1. Obtain the SVN source code

$ cd $HOME/Desktop
$ mkdir build
$ cd build
$ svn checkout svn://svn.code.sf.net/p/dosbox/code-0/dosbox/trunk dosbox-code-0
$ cd dosbox-code-0

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

	•	Download from 'zip' link at the top of the page
	•	Unzip the folder to the desktop and rename it to SDL

$ cd $HOME/Desktop/SDL
$ ./configure --prefix=$HOME/Desktop/staticbuild --enable-static --disable-shared --disable-video-x11
$ perl -p -i -e "/CGDirectPaletteRef palette;/d" ./src/video/quartz/SDL_QuartzVideo.h
$ make
$ make install

	•	Results in a folder called 'staticbuild' on your desktop

C. Apply the Munt patch

	•	Ensure that the 'munt' folder is installed to your home folder as detailed above
	•	Patch files are located by default in munt/DOSBox-mt32-patch
	•	The specific name of the patch file may vary by version number (i.e. dosbox-SVN-r4319-mt32-patch)
	•	The SVN patch will be used - NOT the patch for vanilla DOSBox 0.74x etc.

$ cd $HOME/Desktop/build/dosbox-code-0
$ patch -p1 < $HOME/munt/DOSBox-mt32-patch/dosbox-SVN-r4319-mt32-patch.diff

D. Configure DOSBox SVN

$ cd $HOME/Desktop/build/dosbox-code-0/ $ DOSBOXVERSION=$(svn log | head -2 | awk '/^r/ { print $1 }') $ ./autogen.sh $ ./configure --with-sdl-prefix=$HOME/Desktop/staticbuild/

E. Patch Make files to ensure a static build

1. Patch the primary Makefile

	•	The Makefile is located in $HOME/Desktop/build/dosbox-code-0/
	•	Open it in Xcode and edit at Line 241 (starts with LIBS =):

LIBS =  -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit -framework CoreMIDI /usr/local/Cellar/libpng/1.6.37/lib/libpng.a /usr/local/Cellar/zlib/1.2.11/lib/libz.a /usr/local/Cellar/sdl_net/1.2.8/lib/libSDL_net.a /usr/local/Cellar/sdl_sound/1.0.3_1/lib/libSDL_sound.a /usr/local/Cellar/libogg/1.3.4/lib/libogg.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbis.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisfile.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisenc.a

	•	Your username will be substituted in the highlights above
	•	Check the path to each dependency to make sure it exactly matches what is in your user/local folder
	•	This presumes you installed dependencies to their default locations with homebrew (otherwise edit accordingly)
	•	The version numbers above may vary depending on when you installed the dependencies

	•	Check that Line 257 (starts with SDL_LIBS =) reads as follows:

SDL_LIBS = -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit

	•	As above, your username will be substituted in the highlights

2. Patch the SRC Makefile

	•	The SRC Make file is located in $HOME/Desktop/build/dosbox-code-0/src
	•	Open it in Xcode and edit at Line 246 (starts with LIBS =):

LIBS =  -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit -framework CoreMIDI -framework AudioUnit -framework AudioToolbox /usr/local/Cellar/libpng/1.6.37/lib/libpng.a /usr/local/Cellar/zlib/1.2.11/lib/libz.a /usr/local/Cellar/sdl_net/1.2.8/lib/libSDL_net.a /usr/local/Cellar/sdl_sound/1.0.3_1/lib/libSDL_sound.a /usr/local/Cellar/libogg/1.3.4/lib/libogg.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbis.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisfile.a /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbisenc.a -framework OpenGL /usr/local/lib/libmt32emu.a

	•	These edits are identical to the primary make file, except you will see an extra path for "libmt32emu.a" to ensure the Munt static library is applied
	•	Check that Line 262 (starts with SDL_LIBS =) reads as follows:

SDL_LIBS = -L/Users/yourusername/Desktop/staticbuild/lib /Users/yourusername/Desktop/staticbuild/lib/libSDLmain.a /Users/yourusername/Desktop/staticbuild/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit

F. Compile DOSBox SVN

$ cd $HOME/Desktop/build/dosbox-code-0/
$ make

	•	The DOSBox binary should successfully build and be located in $HOME/Desktop/build/dosbox-code-0/src
	•	Replace the binary in your DOSBox app under Contents/MacOS
