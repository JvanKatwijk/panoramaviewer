#
/*
 *    Copyright (C) 2020
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

//
//	Simple spectrum scope object
//	Shows the spectrum of the incoming data stream 
//	If made invisible, it is a "do nothing"
//
#ifndef		__SCOPE__
#define		__SCOPE__

#include	"panorama-constants.h"
#include	<QFrame>
#include	<QObject>
#include	<qwt.h>
#include	<qwt_plot.h>
#include	<qwt_plot_marker.h>
#include	<qwt_plot_grid.h>
#include	<qwt_plot_curve.h>
#include        <qwt_color_map.h>
#include        <qwt_plot_zoomer.h>
#include        <qwt_plot_textlabel.h>
#include        <qwt_plot_panner.h>
#include        <qwt_plot_layout.h>
#include	<qwt_picker_machine.h>
#include        <qwt_scale_widget.h>
#include        <QBrush>

class	Scope: public QObject {
Q_OBJECT
public:
			Scope	(QwtPlot *,
	                         int,		// DISPLAY_SIZE,
	                         int,		// Start Frequency
	                         int,		// MAX_FREQ,
	                         int,		// bitDepth ()
	                         int,		// scalerBase
	                         int		// scalerWidth
	                        );
			~Scope	();
	void		show	(double *);
private:
	QColor		displayColor;
	QColor		gridColor;
	QColor		curveColor;
	int16_t		displaySize;
	double		*X_Values;
	double		*Y_Values;
	QwtPlot		*plotgrid;
	QwtPlotGrid	*grid;
	QwtPlotCurve	*spectrumCurve;
	int32_t		normalizer;
	QwtPlotPicker   *lm_picker;
	int		baseLine;
	float		get_db		(float);
	int		scalerB;
	int		scalerW;
public slots:
	void		rightMouseClick	(const QPointF &);
signals:
	void		clickedwithRight	(int);
};

#endif

