**Install DOSBox Dependencies**

1. Install Xcode command line tools

xcode-select --install

2. Install Homebrew

https://docs.brew.sh/Installation

brew install cmake 

brew install libpng

brew install lzlib

brew install sdl_net

brew install sdl_sound

If running macOS 11 (Big Sur), Subversion was removed and needs to be installed as well:

brew install svn

	•	Packages should install by default to usr/local/Cellar
  
**Prepare DOSBox build folders**

• These instructions result in the various build folders being installed on the desktop
• DOSBox working folder is 'build'
• SDL working folder is 'staticbuild'

1. Download the source code

https://github.com/almeath/DOSBox-SVN-64-bit-for-macOS.git

2. Obtain autoconf



