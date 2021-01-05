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

//	derived values
#define SEGMENT_SIZE    256
#define FFT_SIZE	2048

/*
 */
//typedef	float DSPFLOAT;

//typedef	std::complex<DSPFLOAT>	DSPCOMPLEX;

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
bool	isIndeterminate (float x) {
	return x != x;
}

static inline
bool	isInfinite (double x) {
	return x == numeric_limits<float>::infinity ();
}
//
static inline
std::complex<float> cmul (std::complex<float> x, float y) {
	return std::complex<float> (real (x) * y, imag (x) * y);
}

static inline
std::complex<float> cdiv (std::complex<float> x, float y) {
	return std::complex<float> (real (x) / y, imag (x) / y);
}

static inline
float	get_db (float x) {
	return 20 * log10 ((x + 1) / (float)(1024));
}
#endif
