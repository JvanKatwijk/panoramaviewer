#
/*
 *    Copyright (C)  2020
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

#ifndef __PANORAMA_VIEWER__
#define __PANORAMA_VIEWER__

#include	<QWidget>
#include	"ui_scopewidget.h"
#include	"ringbuffer.h"
#include	<atomic>
#include	<complex>
#include	<QCloseEvent>
#include	"panorama-constants.h"

class	QSettings;
class	deviceHandler;
class	Scope;
class	Processor;
/*
 *	The main gui object. It inherits from
 *	QDialog and the generated form
 */
class panoramaViewer: public QWidget,
		      private Ui_scopeWidget {
Q_OBJECT
public:
		panoramaViewer	(QSettings *, 
	                         int,
	                         int,
	                         float,
	                         QWidget *parent = NULL);
		~panoramaViewer	(void);

private:
	RingBuffer<double>	_C_Buffer;
	int		maxFreq;
	int		minFreq;
	int		fftFreq;
	int		colibriIndex;
	int		delayFraction;
	int		segmentCoverage;
	float		overlapFraction;
	int		nrSegments;
	int		startFreq;
	int		displaySize;
	Processor	*theProcessor;
	deviceHandler	*theDevice;

	QSettings	*spectrumSettings;
	Scope		*theScope;
	void		TerminateProcess	(void);
	std::atomic<bool>	paused;
public slots:
	void		activateDevice		(const QString &);
	void		handle_showDisplay	();
	void		closeEvent		(QCloseEvent *event);
	void		handle_pauseButton	();
	void		handle_clickedwithRight	(int);
	void		handle_startButton	();
};

#endif

