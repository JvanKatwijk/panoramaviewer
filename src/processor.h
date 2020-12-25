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

#ifndef	__PROCESSOR__
#define	__PROCESSOR__

#include	<QThread>
#include	"panorama-constants.h"
#include	<fftw3.h>
#include	"ringbuffer.h"
#include	<atomic>
#include	"device-handler.h"

class	Processor:public QThread {
Q_OBJECT
public:
		Processor	(deviceHandler *theDevice,
	                         RingBuffer<double> *theBuffer,
	                         int    fftSize,
                                 int    startFreq,
                                 int    freqIncrement,
                                 int    displaySize,
                                 int    nrSegments,
                                 int    segmentSize,
                                 int    overlapSize);
		~Processor	();
	int     freq		(int segmentNumber);
	void	stop		();
	void	run		();
	void	switchPause	(bool);
private:
	void	process_segment (int segmentNo,
	                            std::complex<float> *Buffer,
	                            double *displayVector);
	int	fftSize;
	int	startFreq;
	int	freqIncrement;
	int	displaySize;
	int	nrSegments;
	int	segmentSize;
	int	overlapSize;

	deviceHandler		*theDevice;
	RingBuffer<double>	*_C_Buffer;
	float			*Window;
        fftwf_plan		plan;
        std::complex<float>	*fftBuffer;
	std::atomic<bool>	running;
	std::atomic<bool>	paused;
signals:
	void	showDisplay ();
};

#endif
