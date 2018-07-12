About this section
------------------

Here you can find some examples how work with GATE by usig macros. Each one example focus on other problem.

How to correctly istall GATE from this repository?
---------------------------------------------------

# Required libraries

Befor we start discussing GATE installation it is required to install some extra libraries.

## CLHEP
1. Download CLHEP from http://proj-clhep.web.cern.ch/proj-clhep/
2. Installation steps:
 - unpack package and go do CLHEP directory
 - execute below commands:
 ```
 mkdir build install
 cd build
 cmake -DCMAKE_INSTALL_PREFIX=../install ..
 make
 make test
 make install
 ```
 - go to home directory, create file **.bash_my_tools** and add to it:
 ```
 # CHLEP
 export CLHEP_DIR=/path/to/CLHEP/install
 export CLHEP_INCLUDE_DIR=/path/to/CLHEP/install/include
 export CLHEP_LIBRARY=/path/to/CLHEP/install/lib
 export LD_LIBRARY_PATH=${CLHEP_LIBRARY}:${LD_LIBRARY_PATH}
 export PATH=/path/to/CLHEP/install/bin:$PATH
 ```
 - in home directory open file **.bashrc** and add to it:
 ```
 if [ -f ~/.bash_my_tools ]; then
    . ~/.bash_my_tools
 fi
 ```
 - close terminal and open new one


## Geant4
1. Download Geant4 with version 10.02 (path 01) - archive of Geant4 releases can be found here: https://geant4.web.cern.ch/support/download_archive
2. Installation steps:
 - unpack package and go to Geant4 directory **geant4.10.02**
 - execute below commands:
 ```
 mkdir build install
 cd build
 cmake -DCMAKE_INSTALL_PREFIX=../install -DGEANT4_USE_SYSTEM_CLHEP=ON ..
 make
 make test
 make install
 ```
 - add to file **.bash_my_tools** below line:
 ```
 source path/to/geant4.10.02/install/bin/geant4.sh
 ```
 - close terminal and open new one

# GATE installation
1. Close this repository on your computer:
```
git clone https://github.com/JPETTomography/Gate.git
```
2. Go to GATE folder and execute below commands:
 ```
 mkdir build install
 cd build
 cmake -DCMAKE_INSTALL_PREFIX=../install ..
 make
 make test
 make install
 ```

 If cmake command failed you can mauly modyfie cmake by command:
 ```
 ccmake ..
 ```
 Add needed modifications, call configure and next call generate - after this call make commands as above.

# Usign GATE
You can execute GATE macros in two ways:
1. In GATE run mode by call in terminal command:
```
Gate
```
Where you have to execute your Main.mac makro by command:
```
/control/execute Main.mac
```
To close a window and GATE call:
```
exit
```
This mode is useful when you want visualise simulation.

2. More often we execute Main.mac by calling single command:
```
Gate Main.mac
```

More information is provided in exaples.

 
