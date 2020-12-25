#
/*
 *    Copyright (C) 2014
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the panoramaViewer
 *
 *    panoramaViewer is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    panoramaViewer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with panoramaViewer; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include	"panorama-constants.h"	// some general definitions
#include	"elad-loader.h"		// the API definition
#include	"elad-worker.h"		// our header
#include	"ringbuffer.h"		// the buffer

//	The elad-worker is a simple wrapper around the elad
//	interface. It is a pretty simple thread performing the
//	basic functions.
//	
//	It is a pretty local class

	eladWorker::eladWorker (int32_t		theRate,
	                        int32_t		defaultFreq,
	                        eladLoader	*f,
	                        RingBuffer<uint8_t> *buf,
	                        int16_t		iqSize,
	                        bool	*OK) {

	this	-> theRate	= theRate;
	this	-> defaultFreq	= defaultFreq;
	this	-> functions	= f;
	_I_Buffer		= buf;
	this	-> iqSize	= iqSize;
	*OK			= false;	// just the default

	if (!functions	-> OK ())
	   return;
	lastFrequency		= defaultFreq;	// the parameter!!!!
	runnable		= true;
	functions	-> StartFIFO (functions -> getHandle ());
	start ();
	*OK			= true;
}

//	As usual, killing objects containing a thread need to
//	be done carefully.
void	eladWorker::stop	(void) {
	if (runnable)
	   functions	-> StopFIFO (functions -> getHandle ());
	runnable	= false;
}

	eladWorker::~eladWorker	(void) {
	stop ();
	while (isRunning ())
	   msleep (1);
}
//
//	The actual thread does not do much more than reading the
//	values made available through the elad api
//	and passing the values on
//
#define	BUFFER_SIZE	(8 * 8192)
uint8_t buffer [BUFFER_SIZE];

void	eladWorker:: run (void) {
int32_t	amount;
int	rc;
//
//	we build in a delay such that it can be expected that the
//	data is in place. We read app 100 times a second a buffer
//	per read we have BUFFERSIZE / 8 samples, so we wait

	fprintf (stderr, "New elad-worker\n");
	while (runnable) {
	   rc = libusb_bulk_transfer (functions -> getHandle (),
	                              (6 | LIBUSB_ENDPOINT_IN),
	                              (uint8_t *)buffer,
	                              BUFFER_SIZE * sizeof (int8_t),
	                              &amount,
	                              2000);
	   if (rc) {
              fprintf (stderr,
	               "Error in libusb_bulk_transfer: [%d] %s\n",
	               rc,
	               libusb_error_name (rc));
	      if (rc != 7)
	         break;
	   }

	   _I_Buffer	-> putDataIntoBuffer (buffer, amount);
	   usleep (20);
	   if (_I_Buffer -> GetRingBufferReadAvailable () > theRate / (10 * iqSize))
	      emit samplesAvailable (theRate / (10 * iqSize));
	}
	fprintf (stderr, "eladWorker now stopped\n");
}

void	eladWorker::setVFOFrequency	(uint64_t f) {
	if (!runnable)
	   return;
//	fprintf (stderr, "Setting freq to %d\n", f);
	lastFrequency	= (long int)f;
	functions	-> SetHWLO (functions -> getHandle (), &lastFrequency);
}

uint64_t	eladWorker::getVFOFrequency	(void) {
	return  (uint64_t)lastFrequency;
}

