#
/*
 *    Copyright (C)  2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of panoramaViewer
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

#include	"scope.h"
#include	<QColor>

static inline
int	shifter (int n) {
int res	= 1;
	while (n > 2) {
	   res <<= 1;
	   n --;
	}
	return res;
}

		Scope::Scope   (QwtPlot	*plot,
                                 int	displaySize,	// DISPLAY_SIZE,
	                         int	startFreq,
                                 int	maxFreq,	// MAX_FREQ,
                                 int	bitDepth, 	// bitDepth ()
	                         int	scalerBase,
	                         int	scalerWidth
                                ) {

QString	colorString	= "black";
bool	brush;
	plotgrid		= plot;
	this	-> displaySize	= displaySize;
	this	-> baseLine	= shifter (bitDepth);
	this	-> scalerB	= scalerBase;
	this	-> scalerW	= scalerWidth;
	displayColor		= QColor ("black");
	gridColor		= QColor ("white");
	curveColor		= QColor ("white");
	plotgrid		-> setCanvasBackground (displayColor);
	grid			= new QwtPlotGrid;
#if defined QWT_VERSION && ((QWT_VERSION >> 8) < 0x0601)
	grid	-> setMajPen (QPen(gridColor, 0, Qt::DotLine));
#else
	grid	-> setMajorPen (QPen(gridColor, 0, Qt::DotLine));
#endif
	grid	-> enableXMin (true);
	grid	-> enableYMin (true);
#if defined QWT_VERSION && ((QWT_VERSION >> 8) < 0x0601)
	grid	-> setMinPen (QPen(gridColor, 0, Qt::DotLine));
#else
	grid	-> setMinorPen (QPen(gridColor, 0, Qt::DotLine));
#endif
	grid	-> attach (plotgrid);

	lm_picker       = new QwtPlotPicker (plotgrid -> canvas ());
        QwtPickerMachine *lpickerMachine =
                             new QwtPickerClickPointMachine ();

        lm_picker       -> setStateMachine (lpickerMachine);
        lm_picker       -> setMousePattern (QwtPlotPicker::MouseSelect1,
                                            Qt::RightButton);
        connect (lm_picker, SIGNAL (selected (const QPointF&)),
                 this, SLOT (rightMouseClick (const QPointF&)));
	spectrumCurve	= new QwtPlotCurve ("");
	spectrumCurve   -> setPen (QPen(curveColor, 2.0));
	spectrumCurve	-> setOrientation (Qt::Horizontal);
	spectrumCurve	-> setBaseline	(get_db (0));
	spectrumCurve	-> attach (plotgrid);
	plotgrid	-> enableAxis (QwtPlot::yLeft);

	X_Values	= new double [displaySize];
	Y_Values	= new double [displaySize];

	for (int i = 0; i < displaySize; i ++)
	   X_Values [i] = (startFreq + ((double)i) / displaySize * (maxFreq - startFreq)) / 1000;
}

	Scope::~Scope	() {
	delete		grid;
	delete		spectrumCurve;
	delete		X_Values;
	delete		Y_Values;
}

void	Scope::show	(double *v) {

	for (int i = 0; i < displaySize; i ++) {
	   Y_Values [i] =  get_db (v [i]);
	}


	plotgrid        -> setAxisScale (QwtPlot::xBottom,
                                         X_Values [0],
                                         X_Values [displaySize - 1]);
        plotgrid        -> enableAxis (QwtPlot::xBottom);
        plotgrid        -> setAxisScale (QwtPlot::yLeft,
                                         scalerB * -10,
	                                 scalerB * -10 + scalerW * 10);
        plotgrid        -> enableAxis (QwtPlot::yLeft);
        spectrumCurve	-> setBaseline  (scalerB * -10);
        Y_Values [0]			= scalerB * -10;
        Y_Values [displaySize - 1]	= scalerB * -10;
        spectrumCurve	-> setSamples (X_Values, Y_Values, displaySize);
        plotgrid        -> replot();
}

void	Scope::rightMouseClick (const QPointF&point) {
	clickedwithRight ((int)(point. x()));
	fprintf (stderr, "de x as waarde is %d\n", (int)(point. x()));
}

float	Scope::get_db (float x) {
	if (x == 0)
	   return -60;
	return 20 * log10 ((x + 1) / baseLine);
}

