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

#include "main.h"

#include "osdep.h"

/* ------------------------------------------------------------------------- */

#ifndef _MSC_VER

#include <sys/time.h>
#include <time.h>

unsigned int GetTimer(void) {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return (tv.tv_sec*1000000 + tv.tv_usec);
}

#else

#error "I suspended the MSVC port for now. Use cygwin to compile for Windows or send patches if you want."

#endif

/* ------------------------------------------------------------------------- */

