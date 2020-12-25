

---------------------------------------------------------------------
A simple panorama viewer
---------------------------------------------------------------------

![panoramaviewer](/panoramaviewer.png?raw=true)

Sometimes one wants to get a glimpse of a whole band, no details,
just an overview. The above picture shows - as an example - what one gets
with a simple whip in the range 60 .. 1999 MHz. Other tools focus on details,
this one on the global view

The picture clearly shows the bands up to 240 Mhz the FM and DAB signals,
the various digital TV signals around 500 Mhz, telephone in the 8 and 900
MHz bands.

![panoramaviewer](/FM-and-DAB.png?raw=true)

In the second picture one gets a closer look at both the FM band and the
DAB signals (again, reception is just with a simple whip).

panoramaViewer is - still work in progress - a simple tool to show an
arbitrary wide segment of the spectrum - provided the frequencies are
supported by the input device.


---------------------------------------------------------------------------
Windows
-----------------------------------------------------------------------------

At some point in time an installer for Windows will be created

--------------------------------------------------------------------------------
Linux
------------------------------------------------------------------------

To construct the spectrumviewer adapt the ".pro" file to your needs
For the devices included (by uncommenting the appropriate lines
in the ".pro" file) you need to install the appropriate libraries

-----------------------------------------------------------------------
Supported devices
-----------------------------------------------------------------------

The program is configured to handle the SDRplay (both with the 2.13
and the 3.07 support library).

--------------------------------------------------------------------------

