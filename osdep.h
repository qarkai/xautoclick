#ifndef __OSDEP_H__
#define __OSDEP_H__

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* -ansi -pedantic stuff */

#ifdef __STRICT_ANSI__
int usleep(unsigned long usec);
#endif

/* Microsoft Visual Studio 2005 Express work-arounds
 */

#ifdef _MSC_VER

#include <Windows.h>

#define usleep(a) Sleep( a<1000 ? 1000 : a / 1000)

#endif

/*  BEOS stuff I came across. Should help running this on BeOS (not tested)
 */

#ifdef __BEOS__

#define usleep(t) snooze(t)

#endif

#endif
