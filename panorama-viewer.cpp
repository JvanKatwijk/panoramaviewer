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
#ifdef	HAVE_RTLSDR
#include	"rtlsdr-handler.h"
#endif
#ifdef	HAVE_COLIBRI
#include	"colibri-handler.h"
#endif
#ifdef	HAVE_ELAD_S1
#include	"elad-s1.h"
#endif
#ifdef __MINGW32__
#include	<iostream>
#endif
#
//
static inline
int32_t nearestTwoPower (int16_t n) {
int32_t res     = 1;

        if (n < 100)
           return 128;
        while (n != 0) {
           n >>= 1;
           res <<= 1;
        }

        return res;
}

	panoramaViewer::panoramaViewer (QSettings	*Si,
	                                QWidget		*parent):
	                                           QWidget (parent),
	                                           _C_Buffer (1024 * 1024) {
int k;
// 	the setup for the generated part of the ui
	spectrumSettings		= Si;
	setupUi (this);
	minFreq		= spectrumSettings -> value ("lowEnd", 80). toInt ();
	lowEnd		-> setValue (minFreq);
	maxFreq		= spectrumSettings -> value ("highEnd", 240). toInt ();
	highEnd		-> setValue (maxFreq);
	overlapFraction	= spectrumSettings	-> value ("fraction", 0.875). toFloat ();
	this -> show ();

#ifdef	HAVE_SDRPLAY
	deviceSelector	-> addItem ("sdrplay-v2");
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
	if (s == "sdrplay-v2") {
	   try {
	   theDevice	= new sdrplayHandler (spectrumSettings);
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
	      theDevice	= new eladHandler (spectrumSettings);
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
	      theDevice	= new colibriHandler (spectrumSettings);
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
	if (theProcessor != nullptr)
	   delete theProcessor;
	if (theScope != nullptr)
	   delete theScope;
	this	-> minFreq		= MHz (lowEnd  -> value ()); 
	this	-> maxFreq		= MHz (highEnd -> value ());
	this	-> segmentCoverage	= overlapFraction * fftFreq;
	this	-> nrSegments		= (int)floor ((maxFreq - minFreq) / segmentCoverage);
	this	-> startFreq		= minFreq + (overlapFraction - 0.5) * fftFreq;
	this	-> displaySize		= nrSegments * overlapFraction * SEGMENT_SIZE;
	this	-> maxFreq		= minFreq + nrSegments * segmentCoverage;
	theScope	= new Scope (panoramaScope,
	                             displaySize,
	                             minFreq,
	                             this -> maxFreq,
	                             theDevice -> bitDepth ());
	connect (theScope, SIGNAL (clickedwithRight (int)),
	         this, SLOT (handle_clickedwithRight (int)));
	theProcessor	= new Processor (theDevice,
	                                 &_C_Buffer,
	                                 FFT_SIZE,
	                                 startFreq,
	                                 segmentCoverage,
	                                 displaySize,
	                                 nrSegments,
	                                 SEGMENT_SIZE,
	                                 OVERLAP_SIZE);
	connect (theProcessor, SIGNAL (showDisplay ()),
	         this, SLOT (handle_showDisplay ()));

	theProcessor	-> start ();
}

void	panoramaViewer::TerminateProcess () {
	if (theProcessor != nullptr) {
	   theProcessor	-> stop ();
	}
	spectrumSettings	-> setValue ("lowEnd", lowEnd -> value ());
	spectrumSettings	-> setValue ("highEnd", highEnd -> value ());
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

void	panoramaViewer::handle_clickedwithRight (int n) {
	selectedFreq	-> display ((int)(n / 1000));
}

