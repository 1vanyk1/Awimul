#ifndef _XIMIMSW_H
#define _XIMIMSW_H

typedef struct {
    Bool	(*checkprocessing)(
            Xim   im
    );
    Bool	(*im_open)(
            Xim   im
    );
    void	(*im_free)(
            Xim   im
    );
} XimImsportSW;

extern XimImsportSW _XimImSportRec[];

#endif /* _XIMIMSW_H */