#
/*
 *    Copyright (C) 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the panorama
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
#
#ifndef __PANORAMA_CONSTANTS__
#define	__PANORAMA_CONSTANTS__

#define	QT_STATIC_CONST
#include	<math.h>
#include	<complex>
#include	<stdint.h>
#include	<unistd.h>
#include	<limits>
#include	"stdlib.h"

using namespace std;
#include	<malloc.h>

#ifdef __MINGW32__
#define	CURRENT_VERSION		"1.0:x64-Windows"
#include	"windows.h"
#else
#include	"alloca.h"
#define	CURRENT_VERSION		"1.0:x32-Linux"
#include	"dlfcn.h"
typedef	void	*HINSTANCE;
#endif

//
//      fundamental parameters 

//#define MAX_FREQ        950000000
//#define	MIN_FREQ	85000000
//#define FFT_FREQ	8000000
//
//	derived values
//#define	START_FREQ	MIN_FREQ + (0.375 * FFT_FREQ)
#define SEGMENT_SIZE    128
#define FFT_SIZE	4096
//#define	SEGMENT_COVERAGE	(0.875 * FFT_FREQ)
//#define NR_SEGMENTS	(int)((MAX_FREQ - MIN_FREQ) / SEGMENT_COVERAGE)
#define OVERLAP_SIZE	(SEGMENT_SIZE / 8)
//#define DISPLAY_SIZE    (NR_SEGMENTS * (SEGMENT_SIZE - OVERLAP_SIZE))

/*
 */
typedef	float DSPFLOAT;

typedef	std::complex<DSPFLOAT>	DSPCOMPLEX;

/*
 */
#define	MINIMUM(x, y)	((x) < (y) ? x : y)
#define	MAXIMUM(x, y)	((x) > (y) ? x : y)

#define	Hz(x)	(x)
#define	Khz(x)	(Hz (x) * 1000)
#define	KHz(x)	(Hz (x) * 1000)
#define	Mhz(x)	(KHz (x) * 1000)
#define	MHz(x)	(KHz (x) * 1000)
//
//	common, simple but useful, functions
static inline
bool	isIndeterminate (DSPFLOAT x) {
	return x != x;
}

static inline
bool	isInfinite (double x) {
	return x == numeric_limits<DSPFLOAT>::infinity ();
}
//
static inline
DSPCOMPLEX cmul (DSPCOMPLEX x, float y) {
	return DSPCOMPLEX (real (x) * y, imag (x) * y);
}

static inline
DSPCOMPLEX cdiv (DSPCOMPLEX x, float y) {
	return DSPCOMPLEX (real (x) / y, imag (x) / y);
}

static inline
float	get_db (DSPFLOAT x) {
	return 20 * log10 ((x + 1) / (float)(1024));
}
#endif
