**This build includes the following patches:**

- Munt : for MT-32/CM-32L emulation
	
- Glide : ready to use with the OpenGlide library for 3dfx graphics "pass-through" support (see my OpenGlide library on Github)

- 3dfx Voodoo : software emulation of the 3dfx Voodoo graphics card
	
- Nuked OPL3 : emulates the Yamaha YMF262/CT1747
	
- Memory : increases memory limit to 384mb for use with Windows 9x
	
- Large HD : increases hard drive image size limit (seems to work reliable up to 8GB), also useful for Windows 9x

- CGA monochrome : machine type cga_mono is available, use F11 to cycle through amber, green, white, paper white


**If you want to build your own copy of DOSBox SVN using this repository, please follow the instructions below.**

For those unable or unwilling to compile directly, I can provide a pre-built application wrapper:

https://www.dropbox.com/s/51ni2k0d2wlis4k/DOSBox%20SVN%204441%20wrapper.zip?dl=0


**Notes on usage:**

- The wrapper is entirely self-contained and can be transported between different Macs (it is a static binary with no external library dependencies). I have tested it on Mojave, Catalina and Big Sur on a range of iMacs and MacBooks going back to 2013 vintage. It may work fine in High Sierra and earlier, but I cannot guarantee it. It is based on SDL1 so that should ensure maximum backwards compatibility.
- The wrapper structure should be self-explanatory if you check the contents of /Contents/Resources and read the "dosbox_launch.conf" and "dosbox_settings.conf" files
- Shaders are included and it is enabled with lottes_fast_tweaked by default
- Your C drive is "hardisk"
- ISOs and CD images go in "media" (see the dosbox_launch.conf for suggested mounting commands - i.e. remove the # before the one you want)
- You can edit /Contents/Info.plist where marked (i.e. game title, copyright date etc.)
- The entire package is already codesigned by me, but in macOS Catalina and Big Sur you may have to enable DOSBox to run without restrictions from the macOS Security and Privacy preference pane
- You get best results by setting fullresolution= to your desktop native resolution (i.e. 1920x1080, 2560x1440, 5120x2880 etc.) as the "desktop" setting makes things look a bit blurry on modern Macs with retina screens (I do not think the desktop setting is high-DPI aware, so it has to be forced)
- I have increased all audio rates to 49716 as this apparently provides the best quality audio on Mac hardware
- Unforunately, I cannot provide the MT-32 and CM32-L roms within the package, due to copyright issues, but the prefrences are pointing to the roms directory, ready for you to put your roms in there. Scroll down to the end of the instructions below for more information on using Munt.
- Gravis Ultrasound with pro patches is included (look on the C drive for ultrasnd/mididemo.exe for an awesome sound demo)
- I will periodically update the source code to stay in line with latest SVN - if you build the binary with the instructions below, you can just drop it into /Contents/Resources/dosbox and the wrapper should continue to work normally (note however, that the instructions below do not yet cover static builds, as it is more complicated)
- As mentioned below, this build includes support for up to 384MB of RAM when using Windows 95 or 98, but remember that you should not set the memory any higher than 63MB in a DOS environment
- Large hard disk images are supported, for when you create .img drives for use with Windows 95 and 98. I have tested up to 8GB and from what I read it is not recommended to go any higher and it may in fact not work at all if you do. Img file creation is a whole other subject of its own. There are ample guides on the web if you search for it, but if you want some help please post an issue and I will provide what information I have. I have successfully built wrappers with Windows 3.11, Windows 95B and Windows 98SE.
- 3dfx Voodoo emulation support is included. If installing Windows 9x, you can find drivers and instructions here:

http://dosbox95.darktraveler.com/important.html

- For more information on OpenGlide (i.e. 3dfx passthrough), I have patched and forked it, with full compilation instructions:

https://github.com/almeath/openglide

It is already included in this wrapper, configured with 'optimal' settings (as far as I can tell). I have tested this in a few games, but please note that OpenGlide has many bugs and issues and it is not a perfect solution. Unfortunately, it is the _only_ solution currently available on macOS.

I have included the "glide2x.ovl" file inside the C drive. It needs to be placed inside the game directory of any DOS glide-compatible game, if necessary replacing the one that comes bundled with the game. I have tested it successfully with Lands of Lore 2 (1.30 patch) in DOS.

_If you have any questions or problems, please feel free to lodge an issue here and I will do my best to help. Have fun!_


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
