#include "../x11/libXt/Intrinsic.h"
#include <setjmp.h>
#include "xcalc.h"
#include "../main_wm.h"

#ifndef IEEE
#define XCALC_PRE_OP(keynum) { if (pre_op(keynum)) return; \
		       if (setjmp (env)) {fail_op(); return;}}
#else
#define XCALC_PRE_OP(keynum) if (pre_op(keynum)) return;
#endif

static void add(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void and(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void back(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void base(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void bell(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void clearit(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void cosine(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void decimal(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void degree(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void digit(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void divide(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void e(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void enter(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void epower(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void equal(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void exchange(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void factorial(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void inverse(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void leftParen(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void logarithm(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void mod(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void multiply(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void naturalLog(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void negate(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void nop(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void not(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void off(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void or(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void pi(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void power(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void quit(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void recall(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void reciprocal(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void rightParen(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void roll(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void scientific(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void selection(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void shl(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void shr(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void sine(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void square(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void squareRoot(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void store(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void subtract(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void sum(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void tangent(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void tenpower(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void xtrunc(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void xor(Widget w, XEvent *ev, String *vector, Cardinal *count);
static void XexchangeY(Widget w, XEvent *ev, String *vector, Cardinal *count);

/*
 * 	calculator action table
 */

XtActionsRec	Actions[] = {
        {"add",		add},		/* addition */
        {"and",		and},		/* bitwise and */
        {"back",	back},		/* HP-specific backspace */
        {"base",	base},		/* base conversion */
        {"bell",	bell},		/* ring bell */
        {"clear",	clearit},	/* TI-specific clear calculator state */
        {"cosine",	cosine},	/* trigonometric function cosine */
        {"decimal",	decimal},	/* decimal point */
        {"degree",	degree},	/* degree, radian, grad switch */
        {"digit",	digit},		/* numeric key */
        {"divide",	divide},	/* division */
        {"e",		e},		/* the natural number e */
        {"enter",	enter},		/* HP-specific enter */
        {"epower",	epower},	/* e raised to a power */
        {"equal",	equal},		/* TI-specific = */
        {"exchange",	exchange},	/* TI-specific exchange memory and display */
        {"factorial",	factorial},	/* factorial function */
        {"inverse", 	inverse},	/* inverse */
        {"leftParen",	leftParen},	/* TI-specific left parenthesis */
        {"logarithm",	logarithm},	/* logarithm base 10 */
        {"mod",		mod},		/* modulus */
        {"multiply",	multiply},	/* multiplication */
        {"naturalLog",	naturalLog},	/* natural logarithm base e */
        {"negate",	negate},	/* change sign */
        {"nop",		nop},		/* no operation, rings bell */
        {"not",		not},		/* bitwise not */
        {"off",		off},		/* clear state */
        {"or",		or},		/* bitwise or */
        {"pi",		pi},		/* the number pi */
        {"power",	power},		/* raise to an arbitrary power */
        {"quit",	quit},		/* quit */
        {"recall",	recall},	/* memory recall */
        {"reciprocal",  reciprocal},	/* reciprocal function */
        {"rightParen",	rightParen},	/* TI-specific left parenthesis */
        {"roll",	roll},		/* HP-specific roll stack */
        {"scientific",	scientific},	/* scientfic notation (EE) */
        {"selection",	selection},	/* copy selection */
        {"shl",		shl},		/* arithmetic shift left */
        {"shr",		shr},		/* arithmetic shift right */
        {"sine",	sine},		/* trigonometric function sine */
        {"square",	square},	/* square */
        {"squareRoot",	squareRoot},	/* square root */
        {"store",	store},		/* memory store */
        {"subtract", 	subtract},	/* subtraction */
        {"sum",		sum},		/* memory summation */
        {"tangent",	tangent},	/* trigonometric function tangent */
        {"tenpower",	tenpower},	/* 10 raised to to an arbitrary power */
        {"trunc",	xtrunc}, 	/* truncate to integer */
        {"xor",		xor},		/* bitwise xor */
        {"XexchangeY",	XexchangeY}	/* HP-specific exchange X and Y registers */
};

int ActionsCount = XtNumber(Actions);

/*ARGSUSED*/
static void add(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kADD);
    rpn ? twof(kADD) : twoop(kADD);
    post_op();
}

/*ARGSUSED*/
static void and(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kAND);
    rpn ? twof(kAND) : twoop(kAND);
    post_op();
}

/*ARGSUSED*/
static void back(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kBKSP);
    bkspf();
    post_op();
}

/*ARGSUSED*/
static void base(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kBASE);
    change_base();
    post_op();
}

/*ARGSUSED*/
static void bell(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    ringbell();
}

/*ARGSUSED*/
static void clearit(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kCLR);
    clearf();
    post_op();
}

/*ARGSUSED*/
static void cosine(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kCOS);
    oneop(kCOS);
    post_op();
}

/*ARGSUSED*/
static void decimal(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kDEC);
    decf();
    post_op();
}

/*ARGSUSED*/
static void degree(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kDRG);
    drgf();
    post_op();
}

/*ARGSUSED*/
static void digit(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    switch (vector[0][0])
    {
        case '0': XCALC_PRE_OP(kZERO); numeric(kZERO); break;
        case '1':	XCALC_PRE_OP(kONE); numeric(kONE); break;
        case '2': XCALC_PRE_OP(kTWO); numeric(kTWO); break;
        case '3': XCALC_PRE_OP(kTHREE); numeric(kTHREE); break;
        case '4': XCALC_PRE_OP(kFOUR); numeric(kFOUR); break;
        case '5': XCALC_PRE_OP(kFIVE); numeric(kFIVE); break;
        case '6': XCALC_PRE_OP(kSIX); numeric(kSIX); break;
        case '7': XCALC_PRE_OP(kSEVEN); numeric(kSEVEN); break;
        case '8': XCALC_PRE_OP(kEIGHT); numeric(kEIGHT); break;
        case '9': XCALC_PRE_OP(kNINE); numeric(kNINE); break;
        case 'A': XCALC_PRE_OP(kxA); numeric(kxA); break;
        case 'B': XCALC_PRE_OP(kxB); numeric(kxB); break;
        case 'C': XCALC_PRE_OP(kxC); numeric(kxC); break;
        case 'D': XCALC_PRE_OP(kxD); numeric(kxD); break;
        case 'E': XCALC_PRE_OP(kxE); numeric(kxE); break;
        case 'F': XCALC_PRE_OP(kxF); numeric(kxF); break;
    }
    post_op();
}

/*ARGSUSED*/
static void divide(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kDIV);
    rpn  ? twof(kDIV) : twoop(kDIV);
    post_op();
}

/*ARGSUSED*/
static void e(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kE);
    oneop(kE);
    post_op();
}

/*ARGSUSED*/
static void enter(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kENTR);
    entrf();
    post_op();
}

/*ARGSUSED*/
static void epower(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEXP);
    oneop(kEXP);
    post_op();
}

/*ARGSUSED*/
static void equal(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEQU);
    equf();
    post_op();
}

/*ARGSUSED*/
static void exchange(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEXC);
    oneop(kEXC);
    post_op();
}

/*ARGSUSED*/
static void factorial(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kFACT);
    oneop(kFACT);
    post_op();
}

/*ARGSUSED*/
static void inverse(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kINV);
    invf();
    post_op();
}

/*ARGSUSED*/
static void leftParen(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kLPAR);
    lparf();
    post_op();
}

/*ARGSUSED*/
static void logarithm(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kLOG);
    oneop(kLOG);
    post_op();
}

/*ARGSUSED*/
static void mod(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kMOD);
    rpn ? twof(kMOD) : twoop(kMOD);
    post_op();
}

/*ARGSUSED*/
static void multiply(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kMUL);
    rpn ? twof(kMUL) : twoop(kMUL);
    post_op();
}

/*ARGSUSED*/
static void naturalLog(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kLN);
    oneop(kLN);
    post_op();
}

/*ARGSUSED*/
static void negate(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kNEG);
    negf();
    post_op();
}

/*ARGSUSED*/
static void nop(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    ringbell();
}

/*ARGSUSED*/
static void not(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kNOT);
    oneop(kNOT);
    post_op();
}

/*ARGSUSED*/
static void off(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kOFF);
    offf();
    post_op();
}

/*ARGSUSED*/
static void or(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kOR);
    rpn ? twof(kOR) : twoop(kOR);
    post_op();
}

/*ARGSUSED*/
static void pi(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kPI);
    oneop(kPI);
    post_op();
}

/*ARGSUSED*/
static void power(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kPOW);
    rpn ? twof(kPOW) : twoop(kPOW);
    post_op();
}

/*ARGSUSED*/
static void quit(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    ALOGE("QUIT");
    if (ev->type == ClientMessage &&
        ((Atom) ev->xclient.data.l[0]) != wm_delete_window)
        ringbell();
    else
        Quit();
}

/*ARGSUSED*/
static void recall(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kRCL);
    rpn ? memf(kRCL) : oneop(kRCL);
    post_op();
}

/*ARGSUSED*/
static void reciprocal(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kRECIP);
    oneop(kRECIP);
    post_op();
}

/*ARGSUSED*/
static void rightParen(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kRPAR);
    rparf();
    post_op();
}

/*ARGSUSED*/
static void roll(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kROLL);
    rollf();
    post_op();
}

/*ARGSUSED*/
static void scientific(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kEE);
    eef();
    post_op();
}

/*ARGSUSED*/
static void selection(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    do_select(((XButtonReleasedEvent *)ev)->time);
}

/*ARGSUSED*/
static void shl(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSHL);
    rpn ? twof(kSHL) : twoop(kSHL);
    post_op();
}

/*ARGSUSED*/
static void shr(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSHR);
    rpn ? twof(kSHR) : twoop(kSHR);
    post_op();
}

/*ARGSUSED*/
static void sine(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSIN);
    oneop(kSIN);
    post_op();
}

/*ARGSUSED*/
static void square(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSQR);
    oneop(kSQR);
    post_op();
}

/*ARGSUSED*/
static void squareRoot(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSQRT);
    oneop(kSQRT);
    post_op();
}

/*ARGSUSED*/
static void store(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSTO);
    rpn ? memf(kSTO) : oneop(kSTO);
    post_op();
}

/*ARGSUSED*/
static void subtract(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSUB);
    rpn ? twof(kSUB) : twoop(kSUB);
    post_op();
}

/*ARGSUSED*/
static void sum(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kSUM);
    rpn ? memf(kSUM) : oneop(kSUM);
    post_op();
}

/*ARGSUSED*/
static void tangent(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kTAN);
    oneop(kTAN);
    post_op();
}

/*ARGSUSED*/
static void tenpower(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(k10X);
    oneop(k10X);
    post_op();
}

/*ARGSUSED*/
static void xtrunc(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kTRUNC);
    oneop(kTRUNC);
    post_op();
}

/*ARGSUSED*/
static void xor(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kXOR);
    rpn ? twof(kXOR) : twoop(kXOR);
    post_op();
}

/*ARGSUSED*/
static void XexchangeY(Widget w, XEvent *ev, String *vector, Cardinal *count)
{
    XCALC_PRE_OP(kXXY);
    twof(kXXY);
    post_op();
}
