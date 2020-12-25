#
/*
 *    Copyright (C) 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the panoranaviewer
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
 *
 *	Main program
 */
#include	<Qt>
#include	<QApplication>
#include	<QSettings>
#include	<QDir>
#include	"panorama-constants.h"
#include	"panorama-viewer.h"

#define	DEFAULT_INI	".panoramaviewer.ini"

QString	fullPathfor (QString v) {
QString	fileName;

	if (v == QString ("")) 
	   return QString ("/tmp/xxx");

	if (v. at (0) == QChar ('/')) 		// full path specified
	   return v;

	fileName = QDir::homePath ();
	fileName. append ("/");
	fileName. append (v);
	fileName = QDir::toNativeSeparators (fileName);

	if (!fileName. endsWith (".ini"))
	   fileName. append (".ini");

	return fileName;
}

int	main (int argc, char **argv) {
/*
 *	The default values
 */
QSettings	*ISettings;		/* input .ini file	*/
QString	initFileName	= fullPathfor (QString (DEFAULT_INI));
panoramaViewer	*myRadioInterface;
int	maxFreq		= 0;
int	minFreq		= 0;
int	opt;
	ISettings	= new QSettings (initFileName, QSettings::IniFormat);

	maxFreq		= ISettings -> value ("maxFreq", 200). toInt ();
	minFreq		= ISettings -> value ("minFreq", 85). toInt ();

	while ((opt = getopt (argc, argv, "M:m:f:")) != -1) {
	   switch (opt) {
	      case 'm':
	         minFreq	= atoi (optarg);
	         break;
	      case 'M':
	         maxFreq	= atoi (optarg);
	         break;
	      default:		// cannot happen
	         break;
	   }
	}

	if ((minFreq < 0) || (minFreq >= 1999) ||
	    (maxFreq < 0) || (maxFreq >= 1999) ||
	    (minFreq >= maxFreq)) {
	   fprintf (stderr, "specify minimum and maximum frequency in MHz\n");
	   exit (21);
	}
/*
 *	Before we connect control to the gui, we have to
 *	instantiate
 */
#if QT_VERSION >= 0x050600
	QGuiApplication::setAttribute (Qt::AA_EnableHighDpiScaling);
#endif
	QApplication a (argc, argv);
	myRadioInterface = new panoramaViewer (ISettings,
	                                       MHz (minFreq),
	                                       MHz (maxFreq));

	myRadioInterface -> show ();
	a. exec ();
/*
 *	done:
 */
	fflush (stdout);
	fflush (stderr);
	qDebug ("It is done\n");
	ISettings	-> sync ();
	delete	myRadioInterface;
	exit (1);
}

