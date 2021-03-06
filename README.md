
---------------------------------------------------------------------
A simple panorama viewer
---------------------------------------------------------------------

![panoramaviewer](/panoramaviewer.png?raw=true)

Sometimes one wants to get a glimpse of a whole band, no details,
just an overview. The above picture shows - as an example - what one gets
with a simple whip in the range 60 .. 1100 MHz. Other tools focus on details,
this one on the global view

The picture clearly shows the bands up to 240 Mhz the FM and DAB signals,
the various digital TV signals around 500 Mhz, telephone in the 8 and 900
MHz bands.

![panoramaviewer](/fm-band.png?raw=true)

In the second picture one gets a closer look at both the FM band and the
DAB signals (again, reception is just with a simple whip).

panoramaViewer is - still work in progress - a simple tool to show an
arbitrary wide segment of the spectrum - provided the frequencies are
supported by the input device.

--------------------------------------------------------------------------
How to use panoramaViewer
--------------------------------------------------------------------------

The controls for the panoramaViewer are located
at the left side of the widget.

 * the vertical range of the scope can be set - in steps of 5 dB. The left spinbox defines the width, the right spinbox defines the baseline. Settings are immediate.

 * a center frequency and a band width for the scan can be set. Of course,
specfication is in Mhz

 * since the spectrum shown is built up from a(large) number of segments,
an *overlap* can be specified. The basic idea is to overlap the last
part of a segment with the begin of the next segment. The number - default
125 - gives the fraction (range 1 .. 1000) to be used as overlap area.


 * the **averager** determines the amount of averaging applied on fft output, see the tooltip for details.

 * a device can be selected. The panoramaViewer supports the SDRplay devices.
A selection can be made for the support library, either 2.13 or 3.07.
One a device is selected, selection will be fixed for the duration of the
program execution.

 * the program can be started with the (re)start button. As the name
suggests, the button can be used to restart the program as well.
A restart is useful when changing the parameters. Note that changing 
the setting of the minimum or the maximum frequency has no
effect on the current run of the program. It will have effect after a
restart.

 * Touching a location in the specturm window with the mouse and clicking
with the right mouse button will cause the frequency to be shown on
the top of the left part of the widget.

 * the **scaler** detemines the length of the Y-axis of the spectrum, see the
tooltip for details

---------------------------------------------------------------------------
Windows
-----------------------------------------------------------------------------

A windows installer is available

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

