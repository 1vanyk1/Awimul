cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 11)
add_library(ipc SHARED ipc.cpp ipc.h)
add_library(xserver SHARED
        x11/data/XKeysymDB x11/data/XErrorDB

        main_wm.cpp main_wm.h window_manager.cpp window_manager.h util.cpp util.h

        x11/headers/xlib.h x11/headers/x.h x11/headers/xosdefs.h x11/headers/xfuncproto.h
        x11/headers/xutil.h x11/headers/keysym.h x11/headers/keysymdef.h x11/headers/xlibint.h
        x11/headers/xproto.h x11/headers/xprotostr.h x11/headers/xmd.h x11/headers/xlibconf.h
        x11/headers/xfuncs.h x11/headers/xatom.h x11/headers/xresource.h x11/headers/xthreads.h
        x11/headers/xwindows.h x11/headers/xlib-xcb.h x11/headers/XF86keysym.h
        x11/headers/xos.h x11/headers/xw32defs.h x11/headers/xarch.h x11/headers/xpoll.h
        x11/headers/xwinsock.h x11/headers/xauth.h x11/headers/xos_r.h x11/headers/xlocale.h


        x11/headers/Xtrans/Xtrans.h x11/headers/Xtrans/Xtransint.h
        x11/headers/Xtrans/Xtrans.c x11/headers/Xtrans/Xtranslcl.c x11/headers/Xtrans/Xtranssock.c
        x11/headers/Xtrans/Xtransutil.c x11/headers/Xtrans/transport.c

        x11/headers/Xtrans/bits/stropts.h x11/headers/Xtrans/bits/types.h
        x11/headers/Xtrans/bits/wordsize.h x11/headers/Xtrans/bits/typesizes.h


        x11/xlc/xlcint.h x11/xlc/xlcPubI.h x11/xlc/xlcPublic.h x11/xlc/XlcGeneric.h x11/xlc/Ximint.h
        x11/xlc/XimintL.h x11/xlc/XimProto.h x11/xlc/XimImSw.h x11/xlc/XimThai.h x11/xlc/XimTrInt.h
        x11/xlc/XimTrX.h x11/xlc/XimTrans.h

        x11/xlc/lcWrap.c x11/xlc/lcInit.c x11/xlc/lcUtil.c x11/xlc/lcUTF8Load.c x11/xlc/lcGeneric.c
        x11/xlc/lcUTF8.c x11/xlc/lcPubWrap.c x11/xlc/lcDB.c x11/xlc/lcCT.c x11/xlc/lcCharSet.c
        x11/xlc/lcGenConv.c x11/xlc/lcDefConv.c x11/xlc/lcPublic.c x11/xlc/lcFile.c x11/xlc/lcConv.c
        x11/xlc/lcRM.c x11/xlc/lcPrTxt.c x11/xlc/lcTxtPr.c x11/xlc/lcStd.c x11/xlc/IMWrap.c
        x11/xlc/ICWrap.c x11/xlc/XDefaultIMIF.c x11/xlc/imKStoUCS.c x11/xlc/XDefaultOMIF.c
        x11/xlc/xim_trans.c

        x11/xlc/lcUniConv/armscii_8.h x11/xlc/lcUniConv/ascii.h x11/xlc/lcUniConv/big5.h
        x11/xlc/lcUniConv/big5_emacs.h x11/xlc/lcUniConv/cp1133.h x11/xlc/lcUniConv/cp1251.h
        x11/xlc/lcUniConv/cp1255.h x11/xlc/lcUniConv/cp1256.h x11/xlc/lcUniConv/gb2312.h
        x11/xlc/lcUniConv/georgian_academy.h x11/xlc/lcUniConv/georgian_ps.h
        x11/xlc/lcUniConv/iso8859_1.h x11/xlc/lcUniConv/iso8859_2.h x11/xlc/lcUniConv/iso8859_3.h
        x11/xlc/lcUniConv/iso8859_4.h x11/xlc/lcUniConv/iso8859_5.h x11/xlc/lcUniConv/iso8859_6.h
        x11/xlc/lcUniConv/iso8859_7.h x11/xlc/lcUniConv/iso8859_8.h x11/xlc/lcUniConv/iso8859_9.h
        x11/xlc/lcUniConv/iso8859_9e.h x11/xlc/lcUniConv/iso8859_10.h x11/xlc/lcUniConv/iso8859_11.h
        x11/xlc/lcUniConv/iso8859_13.h x11/xlc/lcUniConv/iso8859_14.h x11/xlc/lcUniConv/iso8859_15.h
        x11/xlc/lcUniConv/iso8859_16.h x11/xlc/lcUniConv/jisx0201.h x11/xlc/lcUniConv/jisx0208.h
        x11/xlc/lcUniConv/jisx0212.h x11/xlc/lcUniConv/koi8_c.h x11/xlc/lcUniConv/koi8_r.h
        x11/xlc/lcUniConv/koi8_u.h x11/xlc/lcUniConv/ksc5601.h x11/xlc/lcUniConv/mulelao.h
        x11/xlc/lcUniConv/tatar_cyr.h x11/xlc/lcUniConv/tcvn.h x11/xlc/lcUniConv/tis620.h
        x11/xlc/lcUniConv/ucs2be.h x11/xlc/lcUniConv/utf8.h x11/xlc/lcUniConv/viscii.h


        x11/xau/AuGetBest.c x11/xau/AuDispose.c x11/xau/AuRead.c x11/xau/AuFileName.c


        x11/Xintconn.h x11/Xprivate.h x11/Xxcbint.h x11/Xresinternal.h x11/Key.h x11/ks_tables.h
        x11/locking.c x11/locking.h x11/utlist.h x11/reallocarray.h x11/Xintatom.h x11/Cr.h
        x11/Xatomtype.h x11/pathmax.h x11/XomGeneric.h

        x11/window.c x11/ChWindow.c x11/CrWindow.c x11/GetProp.c x11/MapWindow.c x11/RepWindow.c
        x11/DestWind.c x11/ChSaveSet.c x11/UnmapWin.c x11/OpenDis.c x11/SelInput.c x11/GetIFocus.c
        x11/SetIFocus.c x11/RaiseWin.c x11/ReconfWin.c x11/MoveWin.c x11/LowerWin.c x11/ConfWind.c
        x11/ChkWinEv.c x11/CirWin.c x11/CirWinDn.c x11/CirWinUp.c x11/KeysymStr.c x11/StrKeysym.c
        x11/KeyBind.c x11/KillCl.c x11/WinEvent.c x11/SendEvent.c x11/GetEventData.c
        x11/GetWMProto.c x11/Geom.c x11/GetGeom.c x11/GrKey.c x11/GrKeybd.c x11/GrButton.c
        x11/ChWAttrs.c x11/GetWAttrs.c x11/ErrDes.c x11/ErrHndlr.c x11/ChkTypEv.c x11/ChkTypWEv.c
        x11/NextEvent.c x11/UngrabSvr.c x11/reallocarray.c x11/QuTree.c x11/Context.c x11/XlibInt.c
        x11/GrServer.c x11/DisName.c x11/ClDisplay.c x11/LockDis.c x11/ScrResStr.c x11/StrToText.c
        x11/TextToStr.c x11/Sync.c x11/Macros.c x11/IntAtom.c x11/XlibAsync.c x11/LoadFont.c
        x11/UnldFont.c x11/SetFont.c x11/Font.c x11/FontInfo.c x11/FontNames.c
        x11/xcb_io.c x11/xcb_disp.c x11/x11_xcb.c x11/SetLocale.c x11/Xrm.c x11/Quarks.c
        x11/Synchro.c x11/FreeEData.c x11/ModMap.c x11/SetPntMap.c x11/GetPntMap.c
        x11/ParseGeom.c x11/ChGC.c x11/CopyGC.c x11/CrGC.c x11/FreeGC.c x11/GCMisc.c x11/GetGCVals.c
        x11/EvToWire.c x11/evtomask.c x11/RegstFlt.c x11/Bell.c x11/GetSOwner.c x11/PutBEvent.c
        x11/IfEvent.c x11/ConvSel.c x11/GetAtomNm.c x11/GetRGBCMap.c x11/ChProp.c x11/Flush.c
        x11/FSWrap.c x11/OMWrap.c x11/Text.c x11/Text16.c x11/TextExt.c x11/TextExt16.c
        x11/ImText.c x11/ImText16.c x11/OCWrap.c x11/FillRct.c x11/SetFore.c x11/SetFunc.c
        x11/GetFProp.c x11/imConv.c


        xcb/xcb.h xcb/xproto.h xcb/xcbext.h xcb/xcbint.h xcb/xcb_windefs.h xcb/bigreq.h
        xcb/xc_misc.h

        xcb/xcb_conn.c xcb/xcb_in.c xcb/xcb_list.c xcb/xproto.c xcb/xcb_out.c xcb/xcb_ext.c
        xcb/xcb_util.c xcb/bigreq.c xcb/xcb_auth.c xcb/xcb_xid.c xcb/xc_misc.c


        im/imInt.c im/imImSw.c im/imInsClbk.c im/imThaiFlt.c im/imThaiIc.c im/imThaiIm.c
        im/imLcIm.c im/imDefIm.c im/imTransR.c im/imDefLkup.c im/imDefIc.c im/imRm.c
        im/imRmAttr.c im/imCallbk.c im/imDefFlt.c im/imLcFlt.c im/imLcLkup.c im/imLcGIc.c
        im/imLcIc.c im/imEvToWire.c im/imExten.c im/imDispch.c im/imLcPrs.c im/imLcSIc.c
        im/imTrans.c im/imTrX.c


        om/omDefault.c om/omGeneric.c om/omImText.c om/omText.c om/omTextEsc.c om/omTextExt.c
        om/omTextPer.c om/omXChar.c

        impl/mblen.c


        windows/windows.h windows/windows2.h windows/guidDef.h windows/types.h windows/windef.h
        windows/winsock2.h windows/ws2tcpip.h windows/ws2ipdef.h windows/in6addr.h)
find_library(log-lib log )
target_link_libraries(ipc ${log-lib} )
target_link_libraries(xserver ${log-lib} )