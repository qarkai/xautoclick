/* ------------------------------------------------------------------------- */

/*
 * $Id$
 * 
 * xAutoClick
 *
 * Copyright (C) 2006 Ivo van Poorten
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* ------------------------------------------------------------------------- */

#ifndef __OSDEP_H__
#define __OSDEP_H__

/* ------------------------------------------------------------------------- */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* ------------------------------------------------------------------------- */

unsigned int GetTimer(void);

/* ------------------------------------------------------------------------- */

/* -ansi -pedantic stuff */

#ifdef __STRICT_ANSI__
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};
int usleep(unsigned long usec);
#endif

/* ------------------------------------------------------------------------- */

/* Microsoft Visual Studio 2005 Express work-arounds
 */

#ifdef _MSC_VER

#include <Windows.h>

#define strdup(a) _strdup(a)
#define usleep(a) Sleep( a<1000 ? 1000 : a / 1000)

#endif

/* ------------------------------------------------------------------------- */

/*  BEOS stuff I came across. Should help running this on BeOS (not tested)
 */

#ifdef __BEOS__

#define usleep(t) snooze(t)

#endif

/* ------------------------------------------------------------------------- */

#endif

/* ------------------------------------------------------------------------- */

