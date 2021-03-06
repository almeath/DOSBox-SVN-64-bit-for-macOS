**How to build DOSBox SVN (64-bit) in macOS**

These instructions assume you are familiar with using the Terminal.

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
	DOSBOXVERSION=$(svn log | head -2 | awk '/^r/ { print $1 }')
	./autogen.sh
	./configure --with-sdl-prefix=$HOME/Desktop/staticbuild/
	make 

For a faster build, you can use 4 threads with the following command:

		make -j4

The DOSBox binary should successfully build and be located in $HOME/Desktop/build/dosbox-code-0/src

The resulting binary can then replace the one inside a standard DOSBox 0.74 application bundle (under /Contents/MacOS)

You can edit /Contents/Info.plist to change the app name, copyright etc.

Running the application should then generate a separate "DOSBox SVN Preferences" in the $HOME/Library/Preferences folder
