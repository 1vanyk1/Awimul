#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "CharSet.h"
#include "CurUtil.h"
#include "../headers/cursorfont.h"
#include <string.h>

int
XmuCursorNameToIndex(_Xconst char *name)
{
    static _Xconst struct _CursorName {
        _Xconst char	*name;
        unsigned int	shape;
    } cursor_names[] = {
            {"x_cursor",		XC_X_cursor},
            {"arrow",		XC_arrow},
            {"based_arrow_down",	XC_based_arrow_down},
            {"based_arrow_up",	XC_based_arrow_up},
            {"boat",		XC_boat},
            {"bogosity",		XC_bogosity},
            {"bottom_left_corner",	XC_bottom_left_corner},
            {"bottom_right_corner",	XC_bottom_right_corner},
            {"bottom_side",		XC_bottom_side},
            {"bottom_tee",		XC_bottom_tee},
            {"box_spiral",		XC_box_spiral},
            {"center_ptr",		XC_center_ptr},
            {"circle",		XC_circle},
            {"clock",		XC_clock},
            {"coffee_mug",		XC_coffee_mug},
            {"cross",		XC_cross},
            {"cross_reverse",	XC_cross_reverse},
            {"crosshair",		XC_crosshair},
            {"diamond_cross",	XC_diamond_cross},
            {"dot",			XC_dot},
            {"dotbox",		XC_dotbox},
            {"double_arrow",	XC_double_arrow},
            {"draft_large",		XC_draft_large},
            {"draft_small",		XC_draft_small},
            {"draped_box",		XC_draped_box},
            {"exchange",		XC_exchange},
            {"fleur",		XC_fleur},
            {"gobbler",		XC_gobbler},
            {"gumby",		XC_gumby},
            {"hand1",		XC_hand1},
            {"hand2",		XC_hand2},
            {"heart",		XC_heart},
            {"icon",		XC_icon},
            {"iron_cross",		XC_iron_cross},
            {"left_ptr",		XC_left_ptr},
            {"left_side",		XC_left_side},
            {"left_tee",		XC_left_tee},
            {"leftbutton",		XC_leftbutton},
            {"ll_angle",		XC_ll_angle},
            {"lr_angle",		XC_lr_angle},
            {"man",			XC_man},
            {"middlebutton",	XC_middlebutton},
            {"mouse",		XC_mouse},
            {"pencil",		XC_pencil},
            {"pirate",		XC_pirate},
            {"plus",		XC_plus},
            {"question_arrow",	XC_question_arrow},
            {"right_ptr",		XC_right_ptr},
            {"right_side",		XC_right_side},
            {"right_tee",		XC_right_tee},
            {"rightbutton",		XC_rightbutton},
            {"rtl_logo",		XC_rtl_logo},
            {"sailboat",		XC_sailboat},
            {"sb_down_arrow",	XC_sb_down_arrow},
            {"sb_h_double_arrow",	XC_sb_h_double_arrow},
            {"sb_left_arrow",	XC_sb_left_arrow},
            {"sb_right_arrow",	XC_sb_right_arrow},
            {"sb_up_arrow",		XC_sb_up_arrow},
            {"sb_v_double_arrow",	XC_sb_v_double_arrow},
            {"shuttle",		XC_shuttle},
            {"sizing",		XC_sizing},
            {"spider",		XC_spider},
            {"spraycan",		XC_spraycan},
            {"star",		XC_star},
            {"target",		XC_target},
            {"tcross",		XC_tcross},
            {"top_left_arrow",	XC_top_left_arrow},
            {"top_left_corner",	XC_top_left_corner},
            {"top_right_corner",	XC_top_right_corner},
            {"top_side",		XC_top_side},
            {"top_tee",		XC_top_tee},
            {"trek",		XC_trek},
            {"ul_angle",		XC_ul_angle},
            {"umbrella",		XC_umbrella},
            {"ur_angle",		XC_ur_angle},
            {"watch",		XC_watch},
            {"xterm",		XC_xterm},
    };
#define NUM_CURSOR_NAMES    (sizeof (cursor_names) / sizeof (cursor_names[0]))
    register _Xconst struct _CursorName *table;
    register int i;
    char tmp[40];

    if (strlen (name) >= sizeof tmp) return -1;
    XmuCopyISOLatin1Lowered (tmp, name);

    for (i=0, table=cursor_names; i < NUM_CURSOR_NAMES; i++, table++ ) {
        if (strcmp(tmp, table->name) == 0) return table->shape;
    }

    return -1;
}
