# Required tools
Before you start do anything be sure you have installed below listed tools:
 - cmake >= 3.10.2
 - ccmake >=3.10.2
 - gcc >= 7.3.0
 - git >= 2.17.1
 - depends on system you have install ***all required and optional*** packages listed [here](https://root.cern/build-prerequisites)

# Required libraries
#### ROOT installation
In place of ***N*** in ***-jN*** set a number of cores you want to use.
The best solution is use a one core less then your computer has e.g. you have 4 cores, so set N=3)
```sh
git clone http://github.com/root-project/root.git
cd root
mkdir build
cd build
cmake ..
make -jN
```
Open your home directory and open file ***.bashrc***
```sh
vim .bashrc
```
To start editing file use key **a** and for close and save changes **Esc** and then write **:qw** and **Enter**.
In ***.bashrc*** file add at the end of file a line:
```sh
# ROOT
export ROOTSYS=path/to/directory/root/build
export LD_LIBRARY_PATH=$ROOTSYS/lib:${LD_LIBRARY_PATH}
export PATH=$ROOTSYS/bin/:$PATH
```
In place of **path/to/directory/build/root** set a directory to ROOT **build** directory.
If everything is OK, after opening a new terminal write ```root``` and you will see a ROOT logo. To close ROOT call ```.q```

**Step finished: You have installed a ROOT framework**

#### CLHEP installation

```sh
git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
cd CLHEP
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make
make install
```
Open your home directory and open file ***.bashrc***
```sh
vim .bashrc
```
To start editing file use key **a** and for close and save changes **Esc** and then write **:qw** and **Enter**.
In ***.bashrc*** file add at the end of file a line:
```sh
# CHLEP
export CLHEP_DIR=/path/to/CLHEP/install
export CLHEP_INCLUDE_DIR=${CLHEP_DIR}/include
export CLHEP_LIBRARY=${CLHEP_DIR}/lib
export LD_LIBRARY_PATH=${CLHEP_LIBRARY}:${LD_LIBRARY_PATH}
export PATH=${CLHEP_DIR}/bin:$PATH
```
In place of **/path/to/CLHEP/install** set a directory to CLHEP **install** directory (this one made with mkdir).

#### GEANT4 installation
Download Geant4 version [10.04.p02](http://cern.ch/geant4-data/releases/geant4.10.04.p02.tar.gz)

If installation steps below fail, please look at [Official Geant4 installation user guide](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/fo/BookInstalGuide.pdf) for more information.

In place of ***N*** in ***-jN*** set a number of cores you want to use.
The best solution is use a one core less then your computer has e.g. you have 4 cores, so set N=3)

Unpack and go to directory geant4.10.04.p02 and in this directory:
```sh
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DGEANT4_INSTALL_DATA=ON ..
make -jN
make install
```
Open your home directory and open file ***.bashrc***
```sh
vim .bashrc
```
To start editing file use key **a** and for close and save changes **Esc** and then write **:qw** and **Enter**.
In ***.bashrc*** file add at the end of file a line:
```sh
#GEANT4
source /path/to/geant4.10.04.p02/install/bin/geant4.sh
```
#### GATE installation
Open new terminal.
```sh
git clone https://github.com/JPETTomography/Gate.git
cd Gate
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make
make install
```
Open your home directory and open file ***.bashrc***
```sh
vim .bashrc
```
To start editing file use key **a** and for close and save changes **Esc** and then write **:qw** and **Enter**.
In ***.bashrc*** file add at the end of file a line:
```sh
#GATE
export PATH=$PATH:/path/to/Gate/install/bin
```
When you open new terminal and call ```Gate``` you will see a  welcome mode of GATE. To close GATE call ```exit```.

