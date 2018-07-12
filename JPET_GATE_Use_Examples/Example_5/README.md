   Example no 5: Point source of gammas from pPs decay with two layers geometry

What will you learn from this example ?
---------------------------------------

In this example, you'll learn how to run simulations for a set number of events and how to archive simulation data. Here we will use dectector createt with two layers. 
In addition, you will learn how to set selected physical processes and define **point** source of particles. Please read descriptions in makro files to know more - feel free to modifying macros.

Files description
------------------

**Main.mac** - This is the main macro you call in GATE, it manages the remaining macros.

**Visualisation.mac** - contains information how present geometry

**Geometry.mac** - describes detector geometry

**Actor.mac** - specifies a data acquisition from simulation  - when and what save

**Application.mac** - specifies how run a simulation - number of events to generate or time of simulation

**Physics.mac** - describes which physics processes take under consideration during simulation and for which particles

**Source.mac** - contains declarations of particles sources

**GateMaterials.db** - database of materials


How to run makro ?
------------------

In makros directory call:
```
Gate Main.mac
```
and wait until the simulation is done.
After this call:
```
root -l
```
and open ROOT files browser:
```
TBrowser* tb = new TBrowser();
```
and choose your ROOT file in macros directory.
To close ROOT call:
```
.q
```

Links to detailed descriptions
-------------------------------

For more information and exmples please use below links:

[Setting up the physics](http://wiki.opengatecollaboration.org/index.php/Users_Guide:Setting_up_the_physics)

[Defining the source](http://wiki.opengatecollaboration.org/index.php/Users_Guide:Source)

[Setting up application and a general introduction to GATE](http://wiki.opengatecollaboration.org/index.php/Users_Guide:Getting_started)

[Defining geometry](http://wiki.opengatecollaboration.org/index.php/Users_Guide:Defining_a_geometry)

[Visualisation](http://wiki.opengatecollaboration.org/index.php/Users_Guide:Defining_a_system)

[ROOT users guide](https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html)







