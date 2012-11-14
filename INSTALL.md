Installation Notes
================================================================================
These MOOS applications were developed and tested on Ubuntu. 

Contact Kevin DeMarco <kevin.demarco@gmail.com> if you have difficulties 
installing or using any of the packages.

Commands that should be entered at the terminal are denoted with a '$'

Dependencies
------------
MOOS / MOOS-IvP

Acquiring the Code
------------------
Install git  
$ sudo apt-get install git-core

I keep all of my git repositories in the same folder for organizational 
reasons.  Typically, I keep them in the ~/git-repos folder.  So create it...

$ mkdir ~/git-repos  
$ cd ~/git-repos

Make a clone of the repo  
$ git clone git://github.com/SyllogismRXS/syllo-moos.git

Building the Code
-----------------

$ mkdir build

$ cd build 

$ cmake .. 

$ make 
$ sudo make install 

----------------------------------------
Kevin DeMarco <kevin.demarco@gmail.com>
