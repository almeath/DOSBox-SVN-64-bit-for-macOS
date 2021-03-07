**How to build DOSBox SVN (64-bit) in macOS**

These instructions assume you are familiar with using the Terminal.

This build includes the following patches:

- Munt - for MT-32/CM-32L emulation
	
- Glide - ready to use with the OpenGlide library for 3dfx graphics "pass-through" support (see my OpenGlide library on Github)
	
- Nuked OPL3 - emulates the Yamaha YMF262/CT1747
	
- Memory - increases memory limit to 384mb for use with Windows 9x
	
- Large HD - increases hard drive image size limit (seems to work reliable up to 8GB), also useful for Windows 9x

_**A. Install DOSBox Dependencies**_

1. Install Xcode command line tools:

		xcode-select --install

2. Install Homebrew:

	https://docs.brew.sh/Installation

3. Install dependencies with Homebrew:

		brew install libpng

		brew install lzlib

		brew install sdl_net

		brew install sdl_sound
	
If you are using macOS 11 (Big Sur), you need this dependency as well:	

		brew install svn

Packages should install by default to: usr/local/Cellar

_**B. Prepare the build folders**_

These instructions result in the various build folders being installed on the desktop. The DOSBox working folder is 'build'. The SDL working folder is 'staticbuild'.

1. Obtain the SVN source code:

	Create a folder on your desktop called 'build'.

	Download the SVN source above, in a zip file. Extract the contents to a folder, and rename the folder to 'dosbox'.

	Place the 'dosbox' folder in the 'build' folder.

2. Obtain autoconf:

		cd $HOME/Desktop/build
		curl -LOs http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
		tar xzpf autoconf-2.69.tar.gz
		cd autoconf-2.69
		./configure
		make
		sudo make install

3. Obtain automake:

		cd $HOME/Desktop/build
		curl -LOs http://ftp.gnu.org/gnu/automake/automake-1.15.tar.gz
		tar xzpf automake-1.15.tar.gz
		cd automake-1.15
		./configure
		make
		sudo make install

4. Download SDL 1.2x (Mercurial)

	https://hg.libsdl.org/SDL/file/ab7529cb9558

Download from source in a 'zip' file. Unzip the folder to the desktop and rename it to SDL.

	cd $HOME/Desktop/SDL
	./configure --prefix=$HOME/Desktop/staticbuild --enable-static --disable-shared --disable-video-x11
	perl -p -i -e "/CGDirectPaletteRef palette;/d" ./src/video/quartz/SDL_QuartzVideo.h
	make
	make install

This results in a folder called 'staticbuild' on your desktop.

_**C. Build DOSBox**_

	cd $HOME/Desktop/build/dosbox/
	./autogen.sh
	./configure --with-sdl-prefix=$HOME/Desktop/staticbuild/
	make 

For a faster build, you can use 4 threads with the following command:

		make -j4

The DOSBox binary should successfully build and be located in $HOME/Desktop/build/dosbox/src

The resulting binary can then replace the one inside a standard DOSBox 0.74 application bundle (under /Contents/MacOS)

You can edit /Contents/Info.plist to change the app name, copyright etc.

Running the application should then generate a separate "DOSBox SVN Preferences" in the $HOME/Library/Preferences folder

**Configuring Munt**

Open your DOSBox SVN Preferences ($HOME/Library/Preferences/DOSBox SVN Preferences)

These are the default settings to enable the Munt emulator functionality:

[midi]

mpu401=intelligent

mididevice=mt32

midiconfig=

mt32.romdir=

mt32.reverse.stereo=false

mt32.verbose=false

mt32.thread=false

mt32.chunk=16

mt32.prebuffer=32

mt32.partials=32

mt32.dac=0

mt32.analog=2

mt32.reverb.mode=auto

mt32.reverb.time=5

mt32.reverb.level=3

- Specify an absolute path to the ROM files next to mt32.romdir=
- The ROM files for either the MT-32 or CM-32L must be named as follows:

MT32_CONTROL.ROM
MT32_PCM.ROM

or

CM32L_CONTROL.ROM
CM32L_PCM.ROM
