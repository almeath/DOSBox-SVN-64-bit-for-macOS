**This macOS fork of DOSBox SVN (4470) includes the following patches:**

- Munt : for MT-32/CM-32L emulation

- Glide : ready to use with the OpenGlide library for 3dfx graphics "pass-through" support (see my OpenGlide fork on Github)

- 3dfx Voodoo : software emulation of the 3dfx Voodoo graphics card

- Nuked OPL3 : emulates the Yamaha YMF262/CT1747

- Memory : increases memory limit to 384mb for use with Windows 9x

- Large HD : increases hard drive image size limit (seems to work reliably up to 8GB), also useful for Windows 9x

- CGA monochrome : machine type cga_mono is available, use F11 to cycle through amber, green, white, paper white

- Fluidsynth : Fluidsynth software MIDI synthesizer

- 4mb vram : Increases video ram for emulated S3 video chip to 4mb (better performance in some games such as Duke Nukem 3D) 

- PC speaker : patch to improve the authenticity of PC speaker emulation (can be enabled/disabled)


**If you want to build your own copy of DOSBox SVN using this repository, please follow the instructions below.**

For those unable to compile directly, I can provide a pre-built version that contains both a 'standard' and an entirely 'self-contained' application bundle. Full instructions on how to use these apps are included in the DMG file.

https://www.dropbox.com/s/aqhk8e42jmf13aw/DOSBox%20macOS%2064.dmg.zip?dl=0


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
       
       brew install cmake

If you are using macOS 11 (Big Sur), you need this dependency as well:    

       brew install svn

Packages should install by default to: usr/local/Cellar

_**B. Install Munt**_

Run the following commands in the Terminal:

       git clone https://github.com/munt/munt.git
       cd munt
       mkdir build
       cd build
       cmake -Dmunt_WITH_MT32EMU_SMF2WAV=OFF -Dmunt_WITH_MT32EMU_QT=OFF -Dlibmt32emu_SHARED=0 ../
       make
       sudo make install

This will install the Munt static library to usr/lib/libmt32emu.a

Munt source files should install to 'munt' folder in your home folder. This folder does not need to be retained after running the installation command.

_**C. Build DOSBox**_

Download the DOSBox source as a zip file and extract to the desktop. Rename the folder to dosbox.

Run the following commands in the Terminal:

   cd $HOME/Desktop/dosbox
   ./autogen.sh
   ./configure
   make

For a faster build, you can use 4 threads with the following command:

       make -j4

The DOSBox binary should successfully build and be located in $HOME/Desktop/dosbox/src

The resulting binary can then replace the one inside a standard DOSBox 0.74 application bundle (under /Contents/MacOS)

You can edit /Contents/Info.plist to change the app name, copyright etc.

Running the application should then generate a separate "DOSBox SVN Preferences" in the $HOME/Library/Preferences folder
