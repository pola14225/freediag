#ifndef _DIAG_OS_H_
#define _DIAG_OS_H_

/*
 *	freediag - Vehicle Diagnostic Utility
 *
 *
 * Copyright (C) 2001 Richard Almeida & Ibex Ltd (rpa@ibex.co.uk)
 * (c) 2014-2015 fenugrec
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *************************************************************************
 *
 * Public functions; wrappers for OS-specific functions
 *
 */

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef WIN32
	#include <windows.h>
	typedef DWORD OS_ERRTYPE;
#else
	typedef int OS_ERRTYPE;
#endif

#define ALARM_TIMEOUT 300	// ms interval timeout for timer callbacks (keepalive etc)

/* Common prototypes but note that the source
 * is different and defined in OS specific
 * c files.
 */
//init, close : ret 0 if ok
int diag_os_init(void);
int diag_os_close(void);

/** Millisecond sleep (blocking)
 *
 * @param ms requested delay
 * @note This makes or breaks freediag... Coding this function
 * is usually a nightmare on most OS'es. See doc/sourcetree_notes.txt
 */
void diag_os_millisleep(unsigned int ms);

/** Check if a key was pressed
 *
 * @return 0 if no key was pressed
 *
 * Currently, it is only used in a few places to break long loops, with "press any key to stop" semantics.
 * This returns immediately, to allow polling within a loop.
 * The linux/unix implementation needs an "Enter" keypress since stdin is buffered !
 */
int diag_os_ipending(void);

/** Measure & adjust OS timing performance.
 *
 * @note Should be called only once.
 */
void diag_os_calibrate(void);

/** Return OS-specific error message
 *
 * @return error string or empty string if not found.
 * @note Caller must not free() the string.
 * This should only be used from OS-specific code ! (diag_os*.c and diag_tty_*.c )
 */
const char * diag_os_geterr(OS_ERRTYPE os_errno);

/* Scheduler */
int diag_os_sched(void);

/** Return current "time" in milliseconds.
 *
 * This must use a monotonic (i.e. always increasing) clock source; this
 * means a *lot* of gettimeofday & similar functions are inadequate.
 * This is important because this value is used to
 * calculate time differentials.
 * Time zero can be any reference unrelated to actual
 * wall-clock time (unix EPOCH, system boot time, etc.)
 * This does not need fine resolutions; 15-20ms is good enough.
 *
 * @return Monotonic time, in milliseconds, from an arbitrary 0 reference.
 */
unsigned long diag_os_getms(void);

/** Get highest-resolution monotonic timestamp available.
 *
 * For use as a short duration stopwatch.
 * Use diag_os_hrtus() to convert delta values to microseconds.
 * @return Monotonic timestamp, in arbitrary units. @see diag_os_hrtus
 */
unsigned long long diag_os_gethrt(void);

/** Convert an hrt timestamp delta to microseconds.
 *
 * @return microseconds
 * @see diag_os_gethrt
 */
unsigned long long diag_os_hrtus(unsigned long long hrdelta);

//diag_os_chronoms: used for logging purposes; it doesn't need
//to be monotonic. if treset !=0, the internal offset is adjusted
// (offset += treset); all subsequent calls to
//chronoms will subtract offset before returning. This allows
//to "reset" it like a stopwatch. Ex.:
// a = diag_os_chronoms(0)	//get current relative time
// b = diag_os_chronoms(a) //for this and the next calls, chronoms
//				//will return elapsed time since chronoms(0) was called.
//Internally, chronoms must adjust the offset before calculating the
//return value.
unsigned long diag_os_chronoms(unsigned long treset);

#if defined(__cplusplus)
}
#endif
#endif /*_DIAG_OS_H_ */
