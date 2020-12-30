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

#include	"processor.h"
#include	"device-handler.h"

	Processor::Processor	(deviceHandler *theDevice,
	                         RingBuffer<double> *theBuffer,
	                         int	fftSize,
	                         int	startFreq,
	                         int	freqIncrement,
	                         int	displaySize,
	                         int	nrSegments,
	                         int	segmentSize,
	                         int	overlapSize,
	                         int	averaging) {
	this	-> theDevice	= theDevice;
	this	-> _C_Buffer	= theBuffer;
	this	-> fftSize	= fftSize;
	this	-> startFreq	= startFreq;
	this	-> freqIncrement	= freqIncrement;
	this	-> displaySize	=  displaySize;
	this	-> nrSegments	= nrSegments;
	this	-> segmentSize	= segmentSize;
	this	-> overlapSize	= overlapSize;
	this	-> averaging	= averaging;
	fftBuffer       = (std::complex<float> *)
	                  fftwf_malloc (sizeof (fftwf_complex) * fftSize);
	plan    = fftwf_plan_dft_1d (fftSize,
	                reinterpret_cast <fftwf_complex *>(fftBuffer),
	                reinterpret_cast <fftwf_complex *>(fftBuffer),
	                FFTW_FORWARD, FFTW_ESTIMATE);

	Window                  = new float [fftSize];
	for (int i = 0; i < fftSize; i ++)
	   Window [i] =
	      0.42 - 0.5 * cos ((2.0 * M_PI * i) / (fftSize - 1)) +
	         0.08 * cos ((4.0 * M_PI * i) / (fftSize - 1));
	running. store (false);
	paused. store (false);
}

	Processor::~Processor	() {
	if (running. load ())
	   stop ();
	fftwf_destroy_plan (plan);
	fftwf_free      (fftBuffer);
}

int	Processor::freq (int segmentNumber) {
        return segmentNumber == 0?
                      startFreq :
                      startFreq +
                            segmentNumber * freqIncrement;
}

void	Processor::stop		() {
	fprintf (stderr, "stop called\n");
	if (!running. load ())
	   return;
	running. store (false);
        while (isRunning ())
           usleep (1000);
}

void	Processor::switchPause	(bool b) {
	paused. store (b);
}
#define	ZZ	60
void	Processor::run		() {
std::complex<float>Buffer [fftSize];
double displayVector	[displaySize];

	running. store (true);
	fprintf (stderr, "we are starting\n");
	bool b = theDevice -> restartReader (freq (0));
#ifdef	__DEBUG__
	fprintf (stderr, "we started at frequency %d with %s\n",
	                         freq (0), b ? "success" : "failure");
#endif
	while (running. load ()) {
	   for (int i = 0; i < nrSegments; i ++) {
	      if (!running. load ())
	         return;
	      if (paused. load ()) {
	         usleep (1000000);
	         continue;
	      }
#ifdef	__DEBUG__
	      fprintf (stderr, "now ready for next segment at %d\n", freq (i));
#endif
	      theDevice -> setVFOFrequency (freq (i));
	      theDevice	-> resetBuffer ();
	      while ((theDevice -> Samples () < (averaging + 1) * fftSize) &&
	                                                   running. load ())
	         usleep (1000);
	      if (!running. load ())
	         goto L_end;
	      for (int j = 0; j < fftSize; j ++)
	         Buffer [j] = 0;
	      theDevice	-> getSamples (fftBuffer, fftSize); // one to ignore
	      for (int k = 0; k < averaging; k ++) {
	         theDevice	-> getSamples (fftBuffer, fftSize);
	         for (int j = 0; j < fftSize; j ++)
                    fftBuffer [j] = cmul (fftBuffer [j], Window [j]);
                 fftwf_execute (plan);
	         for (int j = 0; j < fftSize; j ++)
	            Buffer [j] += cmul (fftBuffer [j], 1.0 / averaging);
	      }
	      process_segment (i, Buffer, displayVector);
	   }
	   _C_Buffer  -> putDataIntoBuffer (displayVector, displaySize);
	   emit showDisplay ();
	}
L_end:
	theDevice -> stopReader ();
}

void	Processor::process_segment (int segmentNo,
	                            std::complex<float> *fftBuffer,
	                            double *displayVector) {
double segment [segmentSize];
	for (int i = 0; i < segmentSize / 2; i ++) {
	   double f = 0;
	   for (int j = 0; j < fftSize / segmentSize; j ++)
	      f += abs (fftBuffer [fftSize / segmentSize * i + j]);
	   segment [segmentSize / 2 + i] = f / (fftSize / segmentSize);

	   f = 0;
	   for (int j = 0; j < fftSize / segmentSize; j ++)
	      f += abs (fftBuffer [fftSize / 2 + fftSize / segmentSize * i + j]);
	   segment [i] = f / (fftSize / segmentSize);
	}

//	now map segment onto the displayVector
	if (segmentNo == 0)
	   for (int i = 0; i < segmentSize - overlapSize; i ++)
	      displayVector [i] = segment [overlapSize + i];
	else {
	   int startPos	= segmentSize - overlapSize +
	                  (segmentNo - 1) * (segmentSize - overlapSize);
//	   fprintf (stderr, "segment %d starts at %d (%d)\n",
//	                                     segmentNo, startPos, displaySize);
	   for (int i = 0; i < overlapSize / 2; i ++)
	      displayVector [startPos - overlapSize / 2 + i] = 
//	      0.5 * displayVector [startPos - overlapSize / 2 + i] +
	                                segment [overlapSize / 2 + i];
	   for (int i = overlapSize; i < segmentSize; i ++)
	      displayVector [startPos + (i - overlapSize)] = segment [i];
	}
}
