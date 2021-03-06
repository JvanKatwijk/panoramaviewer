#
/*
 *    Copyright (C) 2014
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the panoramaViewer
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

#ifndef __ELAD_HANDLER__
#define	__ELAD_HANDLER__

#include	<QObject>
#include	<QFrame>
#include	<QFileDialog>
#include	"panorama-constants.h"
#include	"ringbuffer.h"
#include	"ui_elad-widget.h"
#include	"device-handler.h"
#include	<libusb-1.0/libusb.h>

class	QSettings;
class	eladWorker;
class	eladLoader;
typedef	std::complex<float>(*makeSampleP)(uint8_t *);

class	eladHandler: public deviceHandler, public Ui_eladWidget {
Q_OBJECT
public:
		eladHandler		(QSettings *, int);
		~eladHandler		();
	int32_t	get_fftWidth		();
	void	setVFOFrequency		(int32_t);
	bool	restartReader		(int32_t);
	void	stopReader		();
	int32_t	getSamples		(std::complex<float> *, int32_t);
	int32_t	Samples			();
	int16_t	bitDepth		();
	void	resetBuffer		();
private	slots:
	void	setGainReduction	();
	void	setFilter		();
private:
	QSettings	*eladSettings;
	int32_t		inputRate;
	QFrame		myFrame;
	RingBuffer<uint8_t>	_I_Buffer;
	int16_t		depth;
	bool		deviceOK;
	eladLoader	*theLoader;
	eladWorker	*theWorker;
	uint64_t	vfoFrequency;
	int32_t		vfoOffset;
	int		gainReduced;
	int		localFilter;
	uint8_t		conversionNumber;
	int16_t		iqSize;
	int		delayFraction;
//signals:
//	void		samplesAvailable	(int);
};
#endif

