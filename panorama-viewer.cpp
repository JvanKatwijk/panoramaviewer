#
/*
 *    Copyright (C) 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the panoramaviewer
 *
 *    panoramaviewer is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    panoramaviewer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with panoramaviewer; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include	<QSettings>
#include	<QMessageBox>
#include	<QFileDialog>
#include	<QDebug>
#include	<QDateTime>
#include	"device-handler.h"
#include	"panorama-viewer.h"
#include	"scope.h"
#include	"processor.h"
#ifdef	HAVE_SDRPLAY
#include	"sdrplay-handler.h"
#endif
#ifdef	HAVE_SDRPLAY_V3
#include	"sdrplay-handler-v3.h"
#endif
#ifdef	HAVE_COLIBRI
#include	"colibri-handler.h"
#endif
#ifdef	HAVE_ELAD_S1
#include	"elad-s1.h"
#endif
#ifdef	HAVE_RTLSDR
#include	"rtlsdr-handler.h"
#endif
#ifdef	HAVE_AIRSPY
#include	"airspy-handler.h"
#endif
#ifdef __MINGW32__
#include	<iostream>
#endif
#
//
	panoramaViewer::panoramaViewer (QSettings	*Si,
	                                int		colibriIndex,
	                                int		delayFraction,
	                                float		overlap,
	                                QWidget		*parent):
	                                           QWidget (parent),
	                                           _C_Buffer (1024 * 1024) {
int k;
// 	the setup for the generated part of the ui
	spectrumSettings		= Si;
	this	-> colibriIndex		= colibriIndex;
	this	-> delayFraction	= delayFraction;
	setupUi (this);
	int center		= spectrumSettings -> value ("centerFreq", 80). toInt ();
	int widthS		= spectrumSettings -> value ("width", 80). toInt ();
	int avg		= spectrumSettings -> value ("averaging", 1). toInt ();
	int scaleW	= spectrumSettings -> value ("scaleW", 6). toInt ();
	int scaleB	= spectrumSettings -> value ("scaleB", 7). toInt ();
	centerFreq	-> setValue (center);
	bandWidth	-> setValue (widthS);
	scalerWidth	-> setValue (scaleW);
	scalerBase	-> setValue (scaleB);
	averager	-> setValue (avg);
	this		-> overlapFraction	= overlap;
	this -> show ();

#ifdef	HAVE_SDRPLAY
	deviceSelector	-> addItem ("sdrplay-v2-8000000");
	deviceSelector	-> addItem ("sdrplay-v2-5000000");
	deviceSelector	-> addItem ("sdrplay-v2-2000000");
#endif
#ifdef	HAVE_SDRPLAY_V3
	deviceSelector	-> addItem ("sdrplay-v3");
#endif
#ifdef	HAVE_ELAD_S1
	deviceSelector	-> addItem ("elad-S1");
#endif
#ifdef	HAVE_COLIBRI
	deviceSelector	-> addItem ("colibriNano");
#endif
#ifdef	HAVE_RTLSDR
	deviceSelector	-> addItem ("rtlsdr");
#endif
#ifdef	HAVE_AIRSPY
	deviceSelector	-> addItem ("airspy-10000000");
	deviceSelector	-> addItem ("airspy-2500000");
#endif

	if (deviceSelector	-> count () == 0) {
	   fprintf (stderr, "please configure at least one device\n");
	   exit (21);
	}

	connect (deviceSelector, SIGNAL (activated (const QString &)),
	         this, SLOT (activateDevice (const QString &)));

	paused. store (false);
	theProcessor	= nullptr;
	theDevice	= nullptr;
	theScope	= nullptr;
}

	panoramaViewer::~panoramaViewer (void) {
}

void	panoramaViewer::activateDevice (const QString &s) {
#ifdef	HAVE_SDRPLAY
	if (s == "sdrplay-v2-8000000") {
	   try {
	   theDevice	= new sdrplayHandler (spectrumSettings, 8000000);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                          tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else
	if (s == "sdrplay-v2-5000000") {
	   try {
	   theDevice	= new sdrplayHandler (spectrumSettings, 5000000);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                          tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else
	if (s == "sdrplay-v2-2000000") {
	   try {
	   theDevice	= new sdrplayHandler (spectrumSettings, 2000000);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                          tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else
#endif
#ifdef	HAVE_SDRPLAY_V3
	if (s == "sdrplay-v3") {
	   try {
	      theDevice	= new sdrplayHandler_v3 (spectrumSettings);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                       tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else
#endif
#ifdef	HAVE_ELAD_S1
	if (s == "elad-S1") {
	   try {
	      theDevice	= new eladHandler (spectrumSettings, this -> delayFraction);
	      fprintf (stderr, "elad device started\n");
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                       tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else
#endif
#ifdef	HAVE_COLIBRI
	if (s == "colibriNano") {
	   try {
	      theDevice	= new colibriHandler (spectrumSettings,
	                                      this -> colibriIndex,
	                                      this -> delayFraction);
	      fprintf (stderr, "colibri device started\n");
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                       tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else 	// cannot happen
#endif
#ifdef	HAVE_RTLSDR
	if (s == "rtlsdr") {
	   try {
	      theDevice	= new rtlsdrHandler (spectrumSettings);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                       tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else 	// cannot happen
#endif
#ifdef	HAVE_AIRSPY
	if (s == "airspy-10000000") {
	   try {
	      theDevice	= new airspyHandler (spectrumSettings, 10000000);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                       tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else
	if (s == "airspy-2500000") {
	   try {
	      theDevice	= new airspyHandler (spectrumSettings, 2500000);
	   } catch (int e) {
	      QMessageBox::warning (this, tr ("sdr"),
                                       tr ("Opening  device failed\n"));
	      return;
	   }
	}
	else 	// cannot happen
#endif
	{  QMessageBox::warning (this, tr ("panorama"),
	                              tr ("something went wrong, call an expert\n"));
	   return;
	}

	disconnect (deviceSelector, SIGNAL (activated (const QString &)),
	            this, SLOT (activateDevice (const QString &)));

	connect (startButton, SIGNAL (clicked ()),
	         this, SLOT (handle_startButton ()));
	connect (pauseButton, SIGNAL (clicked ()),
	         this, SLOT (handle_pauseButton ()));
	this	-> fftFreq	= theDevice -> get_fftWidth ();
	fprintf (stderr, "fftWidth = %d\n", fftFreq);
	theScope		= nullptr;
	theProcessor		= nullptr;
}

void	panoramaViewer::handle_startButton () {
	theDevice	-> stopReader ();
	if (theProcessor != nullptr)
	   delete theProcessor;
	if (theScope != nullptr)
	   delete theScope;
	int	midden			= MHz (centerFreq -> value ());
	int	breedte			= MHz (bandWidth -> value ());
	this	-> minFreq		= midden - breedte / 2;
	this	-> maxFreq		= midden + breedte / 2;
	this	-> segmentCoverage	= this -> overlapFraction * fftFreq;
	fprintf (stderr, "segment coverage %d\n", segmentCoverage);
	if (minFreq < 0)
	   minFreq = MHz (10);
	if (maxFreq > MHz (2000))
	   maxFreq = MHz (1999);
	if ((minFreq >= maxFreq) || (maxFreq < minFreq + fftFreq)) {
	   this -> nrSegments	= 1;
	   this -> maxFreq	= minFreq + fftFreq;
	}
	else 
	   this	-> nrSegments		= (int)floor ((maxFreq - minFreq) / segmentCoverage);

	fprintf (stderr, "nr segments %d\n", nrSegments);
	this	-> displaySize		= nrSegments * overlapFraction * SEGMENT_SIZE;
	this	-> maxFreq		= minFreq + nrSegments * segmentCoverage;
	
	theScope	= new Scope (panoramaScope,
	                             displaySize,
	                             minFreq + (1 - overlapFractiomn) * fftFreq,
	                             minFreq + nrSegments * segmentCoverage,
	                             theDevice -> bitDepth (),
	                             scalerBase,
	                             scalerWidth);
	connect (theScope, SIGNAL (clickedwithRight (int, int)),
	         this, SLOT (handle_clickedwithRight (int, int)));
	theProcessor	= new Processor (theDevice,
	                                 &_C_Buffer,
	                                 FFT_SIZE,
	           	                 minFreq + fftFreq / 2,
	                                 segmentCoverage,
	                                 displaySize,
	                                 nrSegments,
	                                 SEGMENT_SIZE,
	                                 (1 - overlapFraction) * SEGMENT_SIZE,
	                                 averager -> value ());
	connect (theProcessor, SIGNAL (showDisplay ()),
	         this, SLOT (handle_showDisplay ()));

	theProcessor	-> start ();
}

void	panoramaViewer::TerminateProcess () {
	if (theProcessor != nullptr) {
	   theProcessor	-> stop ();
	}
	spectrumSettings	-> setValue ("centerFreq",
	                                         centerFreq -> value ());
	spectrumSettings	-> setValue ("width", bandWidth -> value ());
	spectrumSettings	-> setValue ("averaging", averager -> value ());
	spectrumSettings	-> setValue ("scaleW", scalerWidth -> value ());
	spectrumSettings	-> setValue ("scaleB", scalerBase -> value ());
	spectrumSettings	-> sync ();
	if (theProcessor != nullptr)
	   delete		theProcessor;
	if (theScope != nullptr)
	   delete		theScope;
	if (theDevice != nullptr)
	   delete		theDevice;
	fprintf (stderr, "End of termination procedure");
}

void	panoramaViewer::handle_showDisplay () {
double buffer [displaySize];

	_C_Buffer. getDataFromBuffer (buffer, displaySize);
	theScope	-> show (buffer);
}

#include <QCloseEvent>
void	panoramaViewer::closeEvent (QCloseEvent *event) {

        QMessageBox::StandardButton resultButton =
                        QMessageBox::question (this, "spectrumViewer",
                                               tr("Are you sure?\n"),
                                               QMessageBox::No | QMessageBox::Yes,
                                               QMessageBox::Yes);
        if (resultButton != QMessageBox::Yes) {
           event -> ignore();
        } else {
           TerminateProcess ();
           event -> accept ();
        }
}

void	panoramaViewer::handle_pauseButton	() {
	paused. store (!paused. load ());
	if (!paused. load ())
	   pauseButton -> setText ("pause");
	else
	   pauseButton -> setText ("PAUSED");
	if (theProcessor !=  nullptr)
	   theProcessor	-> switchPause (paused. load ());
}

void	panoramaViewer::handle_clickedwithRight (int n, int v) {
	selectedFreq	-> display ((int)(n / 1000));
	Strength	-> display (v);
}

