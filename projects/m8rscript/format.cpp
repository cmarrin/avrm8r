// Adapted from GNU libc (vfprintf.c) by Chris Marrin
// Changed interface to take a String. Also string
// params are UTF8Strings

/*-
* Copyright (c) 1990 The Regents of the University of California.
* All rights reserved.
*
* This code is derived from software contributed to Berkeley by
* Chris Torek.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*	This product includes software developed by the University of
*	California, Berkeley and its contributors.
* 4. Neither the name of the University nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

#include <stdlib.h>
#include <locale.h>
#include <cstdarg>

#include "config.h"
#include "ucString.h"

using namespace UC;

/* 11-bit exponent (VAX G floating point) is 308 decimal digits */
#define	MAXEXP		308
/* 128 bit fraction takes up 39 decimal digits; max reasonable precision */
#define	MAXFRACT	39

#define	BUF		(MAXEXP+MAXFRACT+1)	/* + decimal point */
#define	DEFPREC		6

/*
* Macros for converting digits to letters and vice versa
*/
#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

/*
* Flags used during conversion.
*/
#define	ALT			0x001		/* alternate form */
#define	HEXPREFIX	0x002		/* add 0x or 0X prefix */
#define	LADJUST		0x004		/* left adjustment */
#define	LONGDBL		0x008		/* long double; unimplemented */
#define	LONGINT		0x010		/* long integer */
#define	QUADINT		0x020		/* quad integer */
#define	SHORTINT	0x040		/* short integer */
#define	ZEROPAD		0x080		/* zero (as opposed to blank) pad */

bool
_vformat(String& str, const char* fmt, va_list args)
{
	register int ch;				/* character from fmt */
	register int n, m;				/* handy integers (short term usage) */
	String cp;					/* handy string storage (short term usage) */
	register int flags;				/* flags as above */
	int ret;						/* return value accumulator */
	int width;						/* width from format (%8d), or 0 */
	int prec;						/* precision from format (%.3d), or -1 */
	char sign;						/* sign prefix (' ', '+', '-', or \0) */
	double _double = 0;				/* double precision arguments %[eEfgG] */
	uint32_t _uquad;				/* integer arguments %[diouxX] */
	enum { OCT, DEC, HEX } base;	/* base for [diouxX] conversion */
	int dprec;						/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;						/* field size expanded by dprec */
	char *xdigs = NULL;				/* digits for [xX] conversion */
	char buf[BUF];					/* space for %c, %[diouxX], %[eEfgG] */
	char ox[2];						/* space for 0x hex-prefix */

	/*
	* Choose PADSIZE to trade efficiency vs. size.  If larger printf
	* fields occur frequently, increase PADSIZE and make the initialisers
	* below longer.
	*/
#define	PADSIZE	16		/* pad chunk size */
	static const char blanks[PADSIZE] =
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	static const char zeroes[PADSIZE] =
	{'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

#define _PRINT(s,n) (str += String(s,n))

#define	PAD(howmany, with) { \
	if ((n = (howmany)) > 0) { \
	while (n > PADSIZE) { \
	_PRINT(with, PADSIZE); \
	n -= PADSIZE; \
	} \
	_PRINT(with, n); \
	} \
	}

	ret = 0;

	/*
	* Scan the format for conversions (`%' character).
	*/
	for (;;) {
		const char *temp = fmt;
		while (*fmt != '\0') {
			if (*fmt++ == '%') {
				fmt--;
				break;
			}
		}
		if ((m = (int) (fmt - temp)) != 0) {
			str += String(temp, m);
			ret += m;
		}
		if (*fmt == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
		/*
		* ``If the space and + flags both appear, the space
		* flag will be ignored.''
		*	-- ANSI X3J11
			*/
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
		/*
		* ``A negative field width argument is taken as a
		* - flag followed by a positive field width.''
		*	-- ANSI X3J11
		* They don't exclude field widths read from args.
			*/
			if ((width = va_arg(args, int32_t)) >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				n = va_arg(args, int32_t);
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
		/*
		* ``Note that 0 is taken as a flag, not as the
		* beginning of a field width.''
		*	-- ANSI X3J11
			*/
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (*fmt == 'l') {
				fmt++;
				flags |= QUADINT;
			} else {
				flags |= LONGINT;
			}
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			buf[0] = (char) va_arg(args, int32_t);
			buf[1] = 0;
			cp = buf;
			sign = '\0';
			break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			_uquad = (uint32_t) va_arg(args, int32_t);
			if ((int32_t) _uquad < 0) {
				_uquad = (uint32_t) -(int32_t) _uquad;
				sign = '-';
			}
			base = DEC;
			goto number;
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
		{
			if (prec == -1) {
				prec = DEFPREC;
			} else if ((ch == 'g' || ch == 'G') && prec == 0) {
				prec = 1;
			}

			_double = va_arg(args, double);

			/* do this before tricky precision changes */
			if (IS_INF(_double)) {
				if (_double < 0)
					sign = '-';
				cp = "Inf";
				break;
			}
			if (IS_NAN(_double)) {
				cp = "NaN";
				break;
			}

			// let sprintf do the work
			char numfmt[20];
			char* nump = numfmt;
			*nump++ = '%';
			if (flags & ALT)
				*nump++ = '#';
			if (flags & ZEROPAD)
				*nump++ = '0';
			if (flags & LADJUST)
				*nump++ = '-';
			if (sign == ' ')
				*nump++ = ' ';
			else if (sign == '+')
				*nump++ = '+';
			*nump++ = '*';
			*nump++ = '.';
			*nump++ = '*';
			*nump++ = ch;
			*nump++ = '\0';
			char numbuf[100];
			sprintf(numbuf, numfmt, width, prec, _double);
			cp = String(numbuf);
			sign = '\0';
			break;
		}
		case 'n':
			{
				void* p = va_arg(args, void*);
				if (p != NULL)
					*((int*) p) = ret;
			}
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			_uquad = (uint32_t) va_arg(args, int32_t);
			base = OCT;
			goto nosign;
		case 'p':
		/*
		* ``The argument shall be a pointer to void.  The
		* value of the pointer is converted to a sequence
		* of printable characters, in an implementation-
		* defined manner.''
		*	-- ANSI X3J11
			*/
			/* NOSTRICT */
			{
				void* v = va_arg(args, void*);
				_uquad = (uint32_t) (uint64_t) v;
				base = HEX;
				xdigs = "0123456789abcdef";
				flags |= HEXPREFIX;
				ch = 'x';
				goto nosign;
			}
		case 's':
			{
				cp = String(va_arg(args, char*));

				if (prec >= 0)
					cp.deleteSubString(prec);
				sign = '\0';
				break;
			}
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			_uquad = (uint32_t) va_arg(args, int32_t);
			base = DEC;
			goto nosign;
		case 'X':
			xdigs = "0123456789ABCDEF";
			goto hex;
		case 'x':
			xdigs = "0123456789abcdef";
hex:
			_uquad = (uint32_t) va_arg(args, int32_t);
			base = HEX;
			/* leading 0x/X only if non-zero */
			if (flags & ALT && _uquad != 0)
				flags |= HEXPREFIX;

			/* unsigned conversions */
nosign:		sign = '\0';
/*
* ``... diouXx conversions ... if a precision is
* specified, the 0 flag will be ignored.''
*	-- ANSI X3J11
			*/
number:		if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

				/*
				* ``The result of converting a zero value with an
				* explicit precision of zero is no characters.''
				*	-- ANSI X3J11
			*/
			{
				char *ptr = buf + BUF;
				if (_uquad != 0 || prec != 0) {
				/*
				* Unsigned mod is hard, and unsigned mod
				* by a constant is easier than that by
				* a variable; hence this switch.
					*/
					switch (base) {
					case OCT:
						do {
							*--ptr = (char) to_char(_uquad & 7);
							_uquad >>= 3;
						} while (_uquad);
						/* handle octal leading 0 */
						if (flags & ALT && *cp != '0')
							*--ptr = '0';
						break;

					case DEC:
						/* many numbers are 1 digit */
						while (_uquad >= 10) {
							*--ptr = (char) to_char(_uquad % 10);
							_uquad /= 10;
						}
						*--ptr = (char) to_char(_uquad);
						break;

					case HEX:
						do {
							*--ptr = xdigs[_uquad & 15];
							_uquad >>= 4;
						} while (_uquad);
						break;

					default:
						cp = "bug in format: bad base";
						goto skipsize;
					}
				}
				cp = String(ptr, (int32_t) (buf + BUF - ptr));
			}
skipsize:
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			buf[0] = ch;
			buf[1] = 0;
			cp = buf;
			sign = '\0';
			break;
		}

		/*
		* All reasonable formats wind up here.  At this point, `cp'
		* points to a string which (if not flags&LADJUST) should be
		* padded out to `width' places.  If flags&ZEROPAD, it should
		* first be prefixed by any sign or other prefix; otherwise,
		* it should be blank padded before the prefix is emitted.
		* After any left-hand padding and prefixing, emit zeroes
		* required by a decimal [diouxX] precision, then print the
		* string proper, then emit zeroes required by any leftover
		* floating precision; finally, if LADJUST, pad with blanks.
		*
		* Compute actual size, so we know how much to pad.
		* size excludes decimal prec; realsz includes it.
		*/
		realsz = cp.getLength();
		if (dprec > realsz)
			realsz = dprec;
		if (sign)
			realsz++;
		else if (flags & HEXPREFIX)
			realsz+= 2;

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0)
			PAD(width - realsz, blanks);

		/* prefix */
		if (sign) {
			_PRINT(&sign, 1);
		} else if (flags & HEXPREFIX) {
			ox[0] = '0';
			ox[1] = ch;
			_PRINT(ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
			PAD(width - realsz, zeroes);

		/* leading zeroes from decimal precision */
		PAD(dprec - cp.getLength(), zeroes);

		/* the string or number proper */
		_PRINT(cp, -1);

		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += width > realsz ? width : realsz;
	}
done:
	return true;
}
