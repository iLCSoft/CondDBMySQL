/*
 Copyright (C) 1999-2003 FreeDevices.org  geral@freedevices.org

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id: tclPerformance.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

/* 
 * TCL Wrapper for ConditionsDB
 * Lisbon ATLAS-DAQ Collaboration
 * Written by Jorge Lima (2003/04/15)
 * 
 * This is a conddb wrapper for TCL. The main purppose is to provide an 
 * easily configurable test framework. So do not expect the complete 
 * functionality of the bare C++ API. Nevertheless, care was taken to mimic
 * the object oriented nature of the C++ API into the TCL API. So we can, 
 * in principle, build any kind of performance tests in a way that exposes
 * what is being done at the C++ language level. 
 */

#include <tcl.h>

//#include <sys/times.h>
#include <sys/time.h>


// Performance measurements

// C style coding an linkage avoids a lot of trouble while loading into TCL

extern "C" {
    static double lastElapsedTime = 0;
    static int measurePerformance = 0;
    static struct timeval __timeOffset__;
    void startChronometer()
    {
	if (measurePerformance==0)
	    return;
	lastElapsedTime=0;
	gettimeofday(&__timeOffset__, NULL);
    }
    
    void stopChronometer()
    {
	if (measurePerformance==0)
	    return;
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
	{
	    lastElapsedTime=(double)-1;
	}
	else {
	    long diffusec = (tv.tv_usec-__timeOffset__.tv_usec);
	    long diffsec = (tv.tv_sec-__timeOffset__.tv_sec);
	    lastElapsedTime=diffsec+(double)diffusec/1000000;
	}
    }

    int performanceInit(Tcl_Interp *interp)
    {
	int err;
	err = Tcl_LinkVar(interp, "conddb_perf_measurement",
			  (char *)&measurePerformance, TCL_LINK_BOOLEAN);
	if (err) return err;
	err = Tcl_LinkVar(interp, "conddb_elapsed_time",
			  (char *)&lastElapsedTime, TCL_LINK_DOUBLE);
	return err;
    }
}





