cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 11)
add_library(xserver SHARED
        x11/data/XKeysymDB x11/data/XErrorDB

        x11/headers/xlib.h x11/headers/x.h x11/headers/xosdefs.h x11/headers/xfuncproto.h
        x11/headers/xutil.h x11/headers/keysym.h x11/headers/keysymdef.h x11/headers/xlibint.h
        x11/headers/xproto.h x11/headers/xprotostr.h x11/headers/xmd.h x11/headers/xlibconf.h
        x11/headers/xfuncs.h x11/headers/xatom.h x11/headers/xresource.h x11/headers/xthreads.h
        x11/headers/xwindows.h x11/headers/xlib-xcb.h x11/headers/XF86keysym.h
        x11/headers/xos.h x11/headers/xw32defs.h x11/headers/xarch.h x11/headers/xpoll.h
        x11/headers/xwinsock.h x11/headers/xauth.h x11/headers/xos_r.h x11/headers/xlocale.h
        x11/headers/xdefs.h x11/headers/dixevents.h x11/headers/opaque.h x11/headers/globals.h
        x11/headers/window.h x11/headers/extinit.h x11/headers/screenint.h x11/headers/misc.h
        x11/headers/os.h x11/headers/Xprintf.h x11/headers/extnsionst.h x11/headers/gc.h
        x11/headers/pixmap.h x11/headers/region.h x11/headers/regionstr.h x11/headers/miscstruct.h
        x11/headers/privates.h x11/headers/resource.h x11/headers/dixaccess.h x11/headers/dix.h
        x11/headers/extension.h x11/headers/dixstruct.h x11/headers/cursor.h x11/headers/client.h
        x11/headers/callback.h x11/headers/input.h x11/headers/list.h x11/headers/events.h
        x11/headers/xkbrules.h x11/headers/scrnintstr.h x11/headers/colormap.h
        x11/headers/validate.h x11/headers/windowstr.h x11/headers/pixmapstr.h
        x11/headers/picturestr.h x11/headers/glyphstr.h x11/headers/picture.h x11/headers/property.h
        x11/headers/gcstruct.h x11/headers/colormapst.h x11/headers/cursorstr.h
        x11/headers/selection.h x11/headers/servermd.h x11/headers/xorg-server.h
        x11/headers/hotplug.h x11/headers/closure.h x11/headers/registry.h x11/headers/exevents.h
        x11/headers/inputstr.h x11/headers/dixgrabs.h x11/headers/inpututils.h
        x11/headers/exglobals.h x11/headers/ptrveloc.h x11/headers/xkbsrv.h x11/headers/xkbstr.h
        x11/headers/dispatch.h x11/headers/dixfontstr.h x11/headers/swaprep.h x11/headers/swapreq.h
        x11/headers/enterleave.h x11/headers/xserver-properties.h x11/headers/xkbfile.h
        x11/headers/presentext.h x11/headers/eventconvert.h x11/headers/probes.h
        x11/headers/Xserver-dtrace.h x11/headers/protocol-versions.h x11/headers/XIstubs.h
        x11/headers/optionstr.h x11/headers/closestr.h x11/headers/eventstr.h
        x11/headers/xsha1.h x11/headers/xserver_poll.h x11/headers/busfault.h
        x11/headers/glx_extinit.h x11/headers/XWDFile.h x11/headers/glxvndabi.h
        x11/headers/dix-config.h x11/headers/xwin-config.h x11/headers/ImUtil.h x11/headers/xcms.h
        x11/headers/XKBlib.h x11/headers/Xalloca.h x11/headers/poly.h x11/headers/Xregion.h
        x11/headers/cursorfont.h


        x11/headers/extensions/randr.h x11/headers/extensions/XI2.h x11/headers/extensions/geproto.h
        x11/headers/extensions/ge.h x11/headers/extensions/XI.h x11/headers/extensions/shapeconst.h
        x11/headers/extensions/shapeproto.h x11/headers/extensions/XKB.h
        x11/headers/extensions/renderproto.h x11/headers/extensions/render.h
        x11/headers/extensions/XI2proto.h x11/headers/extensions/XIproto.h
        x11/headers/extensions/panoramiXproto.h x11/headers/extensions/dpmsconst.h
        x11/headers/extensions/XKBproto.h x11/headers/extensions/syncconst.h
        x11/headers/extensions/shm.h x11/headers/extensions/shmproto.h
        x11/headers/extensions/xtestconst.h x11/headers/extensions/xtestproto.h
        x11/headers/extensions/dri3proto.h x11/headers/extensions/XvMC.h
        x11/headers/extensions/Xv.h x11/headers/extensions/XResproto.h
        x11/headers/extensions/presentproto.h x11/headers/extensions/presenttokens.h
        x11/headers/extensions/saver.h x11/headers/extensions/xfixesproto.h
        x11/headers/extensions/xfixeswire.h x11/headers/extensions/randrproto.h
        x11/headers/extensions/composite.h x11/headers/extensions/compositeproto.h
        x11/headers/extensions/damagewire.h x11/headers/extensions/damageproto.h
        x11/headers/extensions/bigreqsproto.h x11/headers/extensions/dpmsproto.h
        x11/headers/extensions/XKMformat.h x11/headers/extensions/XKM.h
        x11/headers/extensions/syncproto.h x11/headers/extensions/xcmiscproto.h
        x11/headers/extensions/XShm.h x11/headers/extensions/Xext.h x11/headers/extensions/extutil.h
        x11/headers/extensions/Xge.h x11/headers/extensions/XKBstr.h
        x11/headers/extensions/XKBgeom.h x11/headers/extensions/XKBfile.h
        x11/headers/extensions/shape.h x11/headers/extensions/XInput.h
        x11/headers/extensions/XInput2.h x11/headers/extensions/Xfixes.h
        x11/headers/extensions/XTest.h x11/headers/extensions/record.h
        x11/headers/extensions/recordconst.h x11/headers/extensions/recordproto.h
        x11/headers/extensions/Xcomposite.h x11/headers/extensions/Xrender.h


        x11/headers/Xtrans/Xtrans.h x11/headers/Xtrans/Xtransint.h
        x11/headers/Xtrans/Xtranslcl_xserv.c x11/headers/Xtrans/Xtranssock_xserv.c
        x11/headers/Xtrans/Xtrans.c x11/headers/Xtrans/Xtranslcl.c x11/headers/Xtrans/Xtranssock.c
        x11/headers/Xtrans/Xtransutil.c x11/headers/Xtrans/transport.c
        x11/headers/Xtrans/Xtrans_xserv.c
        x11/headers/Xtrans/Xtrans_ice.c x11/headers/Xtrans/Xtranslcl_ice.c
        x11/headers/Xtrans/Xtranssock_ice.c

        x11/headers/Xtrans/bits/stropts.h x11/headers/Xtrans/bits/types.h
        x11/headers/Xtrans/bits/wordsize.h x11/headers/Xtrans/bits/typesizes.h


        x11/headers/fonts/fontstruct.h x11/headers/fonts/libxfont2.h x11/headers/fonts/font.h
        x11/headers/fonts/fsmasks.h x11/headers/fonts/fontproto.h x11/headers/fonts/fontutil.h
        x11/headers/fonts/FSproto.h x11/headers/fonts/FS x11/headers/fonts/fontmisc.h
        x11/headers/fonts/fontxlfd.h x11/headers/fonts/fntfilst.h x11/headers/fonts/fntfil.h
        x11/headers/fonts/bitmap.h x11/headers/fonts/fntfilio.h x11/headers/fonts/bufio.h
        x11/headers/fonts/bdfint.h x11/headers/fonts/pcf.h


        x11/xlc/xlcint.h x11/xlc/xlcPubI.h x11/xlc/xlcPublic.h x11/xlc/XlcGeneric.h x11/xlc/Ximint.h
        x11/xlc/XimintL.h x11/xlc/XimProto.h x11/xlc/XimImSw.h x11/xlc/XimThai.h x11/xlc/XimTrInt.h
        x11/xlc/XimTrX.h x11/xlc/XimTrans.h

        x11/xlc/lcWrap.c x11/xlc/lcInit.c x11/xlc/lcUtil.c x11/xlc/lcUTF8Load.c x11/xlc/lcGeneric.c
        x11/xlc/lcUTF8.c x11/xlc/lcPubWrap.c x11/xlc/lcDB.c x11/xlc/lcCT.c x11/xlc/lcCharSet.c
        x11/xlc/lcGenConv.c x11/xlc/lcDefConv.c x11/xlc/lcPublic.c x11/xlc/lcFile.c x11/xlc/lcConv.c
        x11/xlc/lcRM.c x11/xlc/lcPrTxt.c x11/xlc/lcTxtPr.c x11/xlc/lcStd.c x11/xlc/IMWrap.c
        x11/xlc/ICWrap.c x11/xlc/XDefaultIMIF.c x11/xlc/imKStoUCS.c x11/xlc/XDefaultOMIF.c
        x11/xlc/xim_trans.c x11/xlc/mbWrap.c x11/xlc/wcWrap.c

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


        x11/xext/dpmsproc.h x11/xext/syncsrv.h x11/xext/geext.h x11/xext/geint.h
        x11/xext/panoramiXsrv.h x11/xext/panoramiX.h x11/xext/panoramiXh.h x11/xext/sleepuntil.h
        x11/xext/shmint.h x11/xext/xace.h x11/xext/xacestr.h x11/xext/syncsdk.h x11/xext/hashtable.h

        x11/xext/bigreq.c x11/xext/geext.c x11/xext/dpms.c x11/xext/panoramiX.c x11/xext/xtest.c
        x11/xext/panoramiXSwap.c x11/xext/panoramiXprocs.c x11/xext/sleepuntil.c x11/xext/shape.c
        x11/xext/shm.c x11/xext/xace.c x11/xext/sync.c x11/xext/xcmisc.c x11/xext/hashtable.c


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
        x11/GetFProp.c x11/imConv.c x11/FreePix.c x11/GetImage.c x11/CrPixmap.c x11/VisUtil.c
        x11/ImUtil.c x11/CrCmap.c x11/CopyArea.c x11/PutImage.c x11/InitExt.c x11/QuExt.c
        x11/UngrabKbd.c x11/FreeCols.c x11/SetPMask.c x11/DrLine.c x11/SetLStyle.c x11/UngrabPtr.c
        x11/globals.c x11/SetWMProto.c x11/CrBFData.c x11/DrLines.c x11/FillPoly.c x11/Clear.c
        x11/ClearArea.c x11/Depths.c x11/ChkIfEv.c x11/SetNrmHint.c x11/PropAlloc.c x11/TrCoords.c
        x11/DelProp.c x11/SetHints.c x11/SetTxtProp.c x11/GrPointer.c x11/UngrabBut.c
        x11/UngrabKey.c x11/MapSubs.c x11/PeekEvent.c x11/Pending.c x11/Region.c x11/FilterEv.c
        x11/Iconify.c x11/StName.c x11/DrSegs.c x11/QuPntr.c x11/MapRaised.c x11/PmapBord.c
        x11/UndefCurs.c x11/DefCursor.c x11/Misc.c x11/SetSOwner.c x11/Withdraw.c x11/WMProps.c
        x11/WMGeom.c x11/GetDflt.c x11/GetHColor.c x11/AllPlanes.c x11/AllCells.c x11/QuColor.c
        x11/GetColor.c x11/CrPFBData.c x11/RdBitF.c x11/Border.c x11/FreeCurs.c x11/Cursor.c
        x11/LookupCol.c x11/ParseCmd.c x11/SetCRects.c x11/DrPoints.c x11/DrRect.c x11/DrRects.c
        x11/DrArc.c x11/FillArc.c x11/FillArcs.c x11/CopyPlane.c x11/SetClOrig.c x11/SetTSOrig.c
        x11/DrPoint.c x11/SetClMask.c x11/SetTile.c x11/SetStip.c x11/SetDashes.c x11/SetBack.c
        x11/RecolorC.c x11/QuColors.c x11/CrCursor.c x11/CrGlCur.c x11/ParseCol.c x11/WarpPtr.c
        x11/FillRcts.c x11/PeekIfEv.c x11/StBytes.c x11/RotProp.c x11/DrArcs.c


        xcb/xcb.h xcb/xproto.h xcb/xcbext.h xcb/xcbint.h xcb/xcb_windefs.h xcb/bigreq.h
        xcb/xc_misc.h

        xcb/xcb_conn.c xcb/xcb_in.c xcb/xcb_list.c xcb/xproto.c xcb/xcb_out.c xcb/xcb_ext.c
        xcb/xcb_util.c xcb/bigreq.c xcb/xcb_auth.c xcb/xcb_xid.c xcb/xc_misc.c


        x11/libXext/XShm.c x11/libXext/extutil.c x11/libXext/Xge.c x11/libXext/globals.c
        x11/libXext/XShape.c


        im/imInt.c im/imImSw.c im/imInsClbk.c im/imThaiFlt.c im/imThaiIc.c im/imThaiIm.c
        im/imLcIm.c im/imDefIm.c im/imTransR.c im/imDefLkup.c im/imDefIc.c im/imRm.c
        im/imRmAttr.c im/imCallbk.c im/imDefFlt.c im/imLcFlt.c im/imLcLkup.c im/imLcGIc.c
        im/imLcIc.c im/imEvToWire.c im/imExten.c im/imDispch.c im/imLcPrs.c im/imLcSIc.c
        im/imTrans.c im/imTrX.c


        om/omDefault.c om/omGeneric.c om/omImText.c om/omText.c om/omTextEsc.c om/omTextExt.c
        om/omTextPer.c om/omXChar.c


        impl/libc-symbols.h impl/smmintrin.h impl/open_memstream.h impl/errno.h impl/stropts.h
        impl/sys/shm.h impl/sys/filio.h
        impl/mblen.c impl/open_memstream.c impl/fattach.c impl/isastream.c
        impl/shmem.c

        glibc/shlib-compat.h glibc/abi-versions.h glibc/hp-timing.h glibc/list_t.h glibc/atomic.h
        glibc/atomic-machine.h glibc/stackinfo.h glibc/endian.h glibc/unwind-generic.h
        glibc/dl-dtv.h glibc/adaptive_spin_count.h

        glibc/arm/stackinfo.h glibc/arm/dl-sysdep.h

        glibc/string/endian.h


        ipc/bits/ipc.h ipc/bits/ipctypes.h ipc/bits/predefs.h ipc/bits/types.h ipc/bits/typesizes.h
        ipc/bits/wordsize.h

        ipc/sys/cdefs.h ipc/sys/ipc.h

        ipc/gnu/stubs.h ipc/gnu/stubs-32.h ipc/gnu/stubs-64.h

        ipc/features.h


        libsha1/libsha1.h libsha1/sha1.c


        zlib/zlib.h zlib/zconf.h zlib/inflate.h zlib/inffast.h zlib/zutil.h zlib/inftrees.h
        zlib/inffixed.h zlib/deflate.h zlib/crc32.h zlib/trees.h zlib/gzguts.h
        zlib/inflate.c zlib/inffast.c zlib/zutil.c zlib/inftrees.c zlib/adler32.c zlib/compress.c
        zlib/deflate.c zlib/crc32.c zlib/gencrc32.c zlib/genfixed.c zlib/gentrees.c zlib/infback.c
        zlib/trees.c zlib/uncompr.c zlib/gzclose.c zlib/gzlib.c zlib/gzread.c zlib/gzwrite.c


        x11/mi/mi.h x11/mi/mipointer.h x11/mi/misync.h x11/mi/misyncstr.h x11/mi/mivalidate.h
        x11/mi/migc.h x11/mi/miinitext.h x11/mi/micmap.h x11/mi/mipointrst.h x11/mi/miline.h
        x11/mi/micoord.h x11/mi/mizerarc.h x11/mi/mipoly.h x11/mi/miscanfill.h x11/mi/mifillarc.h
        x11/mi/mifpoly.h x11/mi/mistruct.h x11/mi/miwideline.h x11/mi/misprite.h

        x11/mi/migc.c x11/mi/miinitext.c x11/mi/micmap.c x11/mi/mipointer.c x11/mi/mieq.c
        x11/mi/miscrinit.c x11/mi/misync.c x11/mi/micopy.c x11/mi/mivaltree.c x11/mi/miexpose.c
        x11/mi/miwindow.c x11/mi/mizerarc.c x11/mi/mipolyrect.c x11/mi/mipoly.c x11/mi/mifillarc.c
        x11/mi/mipolytext.c x11/mi/miarc.c x11/mi/midash.c x11/mi/miwideline.c x11/mi/mipolyseg.c
        x11/mi/mizerclip.c x11/mi/mizerline.c x11/mi/midispcur.c x11/mi/misprite.c


        x11/glx/glxext.h x11/glx/indirect_table.h x11/glx/indirect_util.h x11/glx/indirect_reqsize.h
        x11/glx/indirect_dispatch.h x11/glx/singlesize.h
        x11/glx/glxscreens.h x11/glx/extension_string.h x11/glx/glxdricommon.h
        x11/glx/glxserver.h x11/glx/unpack.h x11/glx/glxutil.h x11/glx/indirect_size.h
        x11/glx/glxbyteorder.h x11/glx/glxdrawable.h x11/glx/glxcontext.h
        x11/glx/indirect_dispatch_swap.h x11/glx/glwindows.h x11/glx/vndservervendor.h
        x11/glx/vndserver.h x11/glx/indirect_size_get.h

        x11/glx/glxext.c x11/glx/indirect_table.c x11/glx/indirect_util.c x11/glx/indirect_reqsize.c
        x11/glx/indirect_dispatch.c x11/glx/singlesize.c
        x11/glx/glxscreens.c x11/glx/extension_string.c x11/glx/glxdricommon.c
        x11/glx/glxcmds.c
        x11/glx/indirect_dispatch_swap.c x11/glx/rensize.c x11/glx/swap_interval.c
        x11/glx/glxcmdsswap.c x11/glx/singlepix.c x11/glx/renderpixswap.c x11/glx/render2swap.c
        x11/glx/clientinfo.c x11/glx/createcontext.c x11/glx/singlepixswap.c
        x11/glx/indirect_program.c x11/glx/renderpix.c x11/glx/render2.c x11/glx/single2.c
        x11/glx/indirect_texture_compression.c x11/glx/single2swap.c x11/glx/xfont.c
        x11/glx/vndext.c x11/glx/vndservervendor.c x11/glx/vndservermapping.c x11/glx/vndcmds.c
        x11/glx/vnd_dispatch_stubs.c x11/glx/indirect_size_get.c
        x11/glx/glxdriswrast.c

        x11/glx/generated_gl_shim.ic x11/glx/generated_wgl_wrappers.ic


        GL/glxproto.h GL/glxmd.h GL/glxtokens.h
        GL/gl.h GL/glext.h GL/wglext.h GL/glx.h GL/glxext.h

        GL/internal/dri_interface.h


        windows/windows.h windows/windows2.h windows/guidDef.h windows/types.h windows/windef.h
        windows/winsock2.h windows/ws2tcpip.h windows/ws2ipdef.h windows/in6addr.h
        windows/basetyps.h windows/winnt.h windows/basetsd.h windows/pshpack2.h windows/poppack.h
        windows/pshpack8.h windows/pshpack4.h

        windows/winnt.rh


        pixman/pixman.h pixman/pixman-version.h pixman/pixman-private.h pixman/pixman-compiler.h
        pixman/pixman-combine32.h pixman/pixman-inlines.h pixman/dither/blue-noise-64x64.h
        pixman/pixman-accessor.h pixman/pixman-edge-imp.h

        pixman/pixman.c pixman/pixman-matrix.c pixman/pixman-region.c pixman/pixman-region16.c
        pixman/pixman-region32.c pixman/pixman-image.c pixman/pixman-utils.c pixman/pixman-arm.c
        pixman/pixman-implementation.c pixman/pixman-ppc.c pixman/pixman-mips.c pixman/pixman-x86.c
        pixman/pixman-general.c pixman/pixman-fast-path.c pixman/pixman-noop.c
        pixman/pixman-solid-fill.c pixman/pixman-combine32.c pixman/pixman-bits-image.c
        pixman/pixman-radial-gradient.c pixman/pixman-conical-gradient.c
        pixman/pixman-combine-float.c pixman/pixman-linear-gradient.c pixman/pixman-access.c
        pixman/pixman-gradient-walker.c pixman/pixman-glyph.c pixman/pixman-trap.c
        pixman/pixman-edge.c pixman/pixman-edge-accessors.c


        x11/dix/main.c x11/dix/dixutils.c x11/dix/privates.c x11/dix/resource.c x11/dix/grabs.c
        x11/dix/devices.c x11/dix/dispatch.c x11/dix/cursor.c x11/dix/swaprep.c x11/dix/swapreq.c
        x11/dix/events.c x11/dix/events.c x11/dix/eventconvert.c x11/dix/globals.c x11/dix/atom.c
        x11/dix/colormap.c x11/dix/dixfonts.c x11/dix/enterleave.c x11/dix/extension.c x11/dix/gc.c
        x11/dix/gestures.c x11/dix/getevents.c x11/dix/initatoms.c x11/dix/inpututils.c
        x11/dix/pixmap.c x11/dix/property.c x11/dix/ptrveloc.c x11/dix/region.c x11/dix/registry.c
        x11/dix/selection.c x11/dix/stubmain.c x11/dix/tables.c x11/dix/touch.c x11/dix/window.c
        x11/dix/glyphcurs.c


        x11/xi/xiquerydevice.h x11/xi/xiproperty.h x11/xi/xichangehierarchy.h x11/xi/allowev.h
        x11/xi/xiallowev.h x11/xi/chgdctl.h x11/xi/chgfctl.h x11/xi/chgkbd.h x11/xi/chgprop.h
        x11/xi/chgptr.h x11/xi/closedev.h x11/xi/devbell.h x11/xi/getbmap.h x11/xi/getdctl.h
        x11/xi/getfctl.h x11/xi/getfocus.h x11/xi/getkmap.h x11/xi/getmmap.h x11/xi/getprop.h
        x11/xi/getselev.h x11/xi/getvers.h x11/xi/grabdev.h x11/xi/grabdevb.h x11/xi/grabdevk.h
        x11/xi/xigrabdev.h x11/xi/gtmotion.h x11/xi/listdev.h x11/xi/opendev.h x11/xi/queryst.h
        x11/xi/selectev.h x11/xi/sendexev.h x11/xi/chgkmap.h x11/xi/setbmap.h x11/xi/setdval.h
        x11/xi/setfocus.h x11/xi/setmmap.h x11/xi/setmode.h x11/xi/ungrdev.h x11/xi/ungrdevb.h
        x11/xi/ungrdevk.h x11/xi/xiselectev.h x11/xi/xipassivegrab.h x11/xi/xisetdevfocus.h
        x11/xi/xiwarppointer.h x11/xi/xichangecursor.h x11/xi/xigetclientpointer.h
        x11/xi/xiquerypointer.h x11/xi/xiqueryversion.h x11/xi/xisetclientpointer.h
        x11/xi/xibarriers.h

        x11/xi/extinit.c x11/xi/allowev.c x11/xi/xiallowev.c x11/xi/chgdctl.c x11/xi/chgfctl.c
        x11/xi/chgkbd.c x11/xi/chgprop.c x11/xi/chgptr.c x11/xi/closedev.c x11/xi/devbell.c
        x11/xi/getbmap.c x11/xi/getdctl.c x11/xi/getfctl.c x11/xi/getfocus.c x11/xi/getkmap.c
        x11/xi/getmmap.c x11/xi/getprop.c x11/xi/getselev.c x11/xi/getvers.c x11/xi/grabdev.c
        x11/xi/grabdevb.c x11/xi/grabdevk.c x11/xi/xigrabdev.c x11/xi/gtmotion.c x11/xi/listdev.c
        x11/xi/opendev.c x11/xi/queryst.c x11/xi/selectev.c x11/xi/sendexev.c x11/xi/chgkmap.c
        x11/xi/setbmap.c x11/xi/setdval.c x11/xi/setfocus.c x11/xi/setmmap.c x11/xi/setmode.c
        x11/xi/ungrdev.c x11/xi/ungrdevb.c x11/xi/ungrdevk.c x11/xi/xiselectev.c
        x11/xi/xipassivegrab.c x11/xi/xisetdevfocus.c x11/xi/xiwarppointer.c x11/xi/xichangecursor.c
        x11/xi/xigetclientpointer.c x11/xi/xiquerypointer.c x11/xi/xiqueryversion.c
        x11/xi/xisetclientpointer.c x11/xi/xibarriers.c x11/xi/xichangehierarchy.c
        x11/xi/xiproperty.c x11/xi/xiquerydevice.c x11/xi/exevents.c x11/xi/stubs.c


        x11/xfixes/xfixesint.h x11/xfixes/xfixes.c x11/xfixes/xfixes.h x11/xfixes/select.c
        x11/xfixes/saveset.c x11/xfixes/region.c x11/xfixes/disconnect.c x11/xfixes/cursor.c


        x11/randr/randrstr.h x11/randr/rrtransform.h
        x11/randr/rrtransform.c x11/randr/randr.c x11/randr/rrcrtc.c x11/randr/rrscreen.c
        x11/randr/rrdispatch.c x11/randr/rrsdispatch.c x11/randr/rrinfo.c x11/randr/rrmode.c
        x11/randr/rrmonitor.c x11/randr/rroutput.c x11/randr/rrpointer.c x11/randr/rrproperty.c
        x11/randr/rrprovider.c x11/randr/rrproviderproperty.c x11/randr/rrxinerama.c
        x11/randr/rrlease.c


        x11/composite/compint.h x11/composite/compositeext.h x11/composite/compalloc.c
        x11/composite/compext.c x11/composite/compinit.c x11/composite/compoverlay.c
        x11/composite/compwindow.c


        x11/damageext/damageext.c x11/damageext/damageext.h x11/damageext/damageextint.h


        x11/miext/damage/damage.h x11/miext/damage/damage.c x11/miext/damage/damagestr.h


        x11/miext/rootless/rootless.h x11/miext/rootless/rootlessConfig.h
        x11/miext/rootless/rootlessCommon.h x11/miext/rootless/rootlessWindow.h
        x11/miext/rootless/rootlessCommon.c x11/miext/rootless/rootlessGC.c
        x11/miext/rootless/rootlessScreen.c x11/miext/rootless/rootlessValTree.c
        x11/miext/rootless/rootlessWindow.c


        x11/miext/shadow/shadow.h x11/miext/shadow/shadow.c


        x11/fb/fb.h x11/fb/fbrop.h x11/fb/wfbrename.h x11/fb/fbbits.h x11/fb/fbpict.h

        x11/fb/fbwindow.c x11/fb/fbcopy.c x11/fb/fbfill.c x11/fb/fbgetsp.c x11/fb/fbimage.c
        x11/fb/fbpush.c x11/fb/fbsetsp.c x11/fb/fballpriv.c x11/fb/fbblt.c x11/fb/fbbltone.c
        x11/fb/fbgc.c x11/fb/fbsolid.c x11/fb/fbglyph.c x11/fb/fbutil.c x11/fb/fbfillsp.c
        x11/fb/fbbits.c x11/fb/fbline.c x11/fb/fbpoint.c x11/fb/fbarc.c x11/fb/fbfillrect.c
        x11/fb/fbseg.c x11/fb/fbscreen.c x11/fb/fbcmap_mi.c x11/fb/fbpict.c x11/fb/fbtrap.c
        x11/fb/fbpixmap.c


        x11/xkb/xkbgeom.h x11/xkb/xkb.h x11/xkb/XKBlibint.h x11/xkb/xkbDflts.h

        x11/xkb/XKBAlloc.c x11/xkb/XKBGAlloc.c x11/xkb/XKBMAlloc.c x11/xkb/XKBMisc.c x11/xkb/xkb.c
        x11/xkb/xkbAccessX.c x11/xkb/ddxPrivate.c x11/xkb/xkbEvents.c x11/xkb/xkbInit.c
        x11/xkb/xkbLEDs.c x11/xkb/xkbPrKeyEv.c x11/xkb/xkbSwap.c x11/xkb/xkbUtils.c
        x11/xkb/xkbfmisc.c x11/xkb/xkbfmisc.c x11/xkb/xkbout.c x11/xkb/xkbtext.c x11/xkb/xkmread.c
        x11/xkb/ddxBeep.c x11/xkb/ddxCtrls.c x11/xkb/ddxVT.c x11/xkb/ddxKillSrv.c x11/xkb/ddxLEDs.c
        x11/xkb/ddxLoad.c x11/xkb/xkbActions.c x11/xkb/maprules.c x11/xkb/XKB2.c x11/xkb/XKBGeom.c
        x11/xkb/XKBNames.c x11/xkb/XKBCompat.c x11/xkb/XKBCtrls.c x11/xkb/XKBleds2.c
        x11/xkb/XkbGetMap.c x11/xkb/XKBUse.c x11/xkb/XKBRdBuf.c x11/xkb/XKBAlloc2.c
        x11/xkb/XKBMAlloc2.c x11/xkb/XKBGAlloc2.c x11/xkb/XKBSetMap.c x11/xkb/XKBSetGeom.c


        x11/xfont/replace.h x11/xfont/libxfontint.h x11/xfont/snfstr.h x11/xfont/builtin.h
        x11/xfont/private.c x11/xfont/fontutil.c x11/xfont/miscutil.c x11/xfont/reallocarray.c
        x11/xfont/patcache.c x11/xfont/libxfontstubs.c x11/xfont/fontnames.c x11/xfont/fontaccel.c
        x11/xfont/fontxlfd.c x11/xfont/format.c x11/xfont/atom.c x11/xfont/register.c
        x11/xfont/fontfile.c x11/xfont/bitmapfunc.c x11/xfont/fpe.c x11/xfont/fontdir.c
        x11/xfont/fontscale.c x11/xfont/dirfile.c x11/xfont/bitsource.c x11/xfont/render.c
        x11/xfont/dir.c x11/xfont/renderers.c x11/xfont/fonts.c x11/xfont/file.c x11/xfont/pcfread.c
        x11/xfont/defaults.c x11/xfont/bufio.c x11/xfont/bitmap.c x11/xfont/utilbitmap.c
        x11/xfont/gunzip.c


        x11/render/mipict.c x11/render/mipict.h x11/render/animcur.c x11/render/filter.c
        x11/render/glyph.c x11/render/glyphstr.h x11/render/matrix.c x11/render/miindex.c
        x11/render/mitrap.c x11/render/mitri.c x11/render/mirect.c x11/render/picture.h
        x11/render/picture.c x11/render/picturestr.h x11/render/render.c


        x11/hw/vfb/InitInput.c x11/hw/vfb/InitOutput.c


        x11/xcms/Xcmsint.h


        x11/libxkbfile/XKBfileInt.h

        x11/libxkbfile/xkbatom.c x11/libxkbfile/xkbmisc.c x11/libxkbfile/xkberrs.c
        x11/libxkbfile/srvmisc.c x11/libxkbfile/xkmout.c x11/libxkbfile/cout.c
        x11/libxkbfile/xkbout.c x11/libxkbfile/xkbtext.c x11/libxkbfile/xkmread.c


        os/xsha1.c os/connection.c os/osdep.h os/ospoll.c os/ospoll.h os/xserver_poll.c os/io.c
        os/access.c os/client.c os/xprintf.c os/inputthread.c os/utils.c os/osinit.c
        os/busfault.c os/WaitFor.c os/auth.c os/backtrace.c os/log.c os/mitauth.c os/oscolor.c
        os/reallocarray.c os/rpcauth.c os/strcasecmp.c os/strcasestr.c os/strlcat.c os/strlcpy.c
        os/strndup.c os/timingsafe_memcmp.c os/xdmauth.c os/xdmcp.c os/xstrans.c


#        mesa/main/dd.h mesa/main/menums.h mesa/main/formats.h mesa/main/macros.h
#        mesa/main/mesa_private.h mesa/main/glheader.h mesa/main/errors.h mesa/main/enums.h
#        mesa/main/context.h mesa/main/glformats.h mesa/main/texcompress.h mesa/main/debug_output.h
#        mesa/main/mtypes.h mesa/main/texcompress_fxt1.h mesa/main/texstore.h mesa/main/version.h
#        mesa/main/hash.h mesa/main/image.h mesa/main/colormac.h mesa/main/config.h
#        mesa/main/mipmap.h mesa/main/texcompress_s3tc_tmp.h mesa/main/texcompress_s3tc.h
#        mesa/main/format_unpack.h mesa/main/texcompress_rgtc.h mesa/main/draw_validate.h
#        mesa/main/texcompress_etc.h mesa/main/texcompress_bptc.h mesa/main/texcompress_bptc_tmp.h
#        mesa/main/texcompress_etc_tmp.h mesa/main/texobj.h mesa/main/teximage.h
#        mesa/main/samplerobj.h mesa/main/texturebindless.h mesa/main/shaderimage.h
#        mesa/main/bufferobj.h mesa/main/externalobjects.h mesa/main/texstorage.h
#        mesa/main/textureview.h mesa/main/transformfeedback.h mesa/main/varray.h mesa/main/enable.h
#        mesa/main/arrayobj.h mesa/main/state.h mesa/main/get.h mesa/main/dispatch.h mesa/main/draw.h
#        mesa/main/blend.h mesa/main/extensions.h mesa/main/spirv_extensions.h mesa/main/stencil.h
#        mesa/main/framebuffer.h mesa/main/fbobject.h mesa/main/buffers.h mesa/main/multisample.h
#        mesa/main/renderbuffer.h mesa/main/clip.h mesa/main/light.h mesa/main/pixel.h
#        mesa/main/pbo.h mesa/main/pixelstore.h mesa/main/pixeltransfer.h mesa/main/format_pack.h
#        mesa/main/texcompress_cpal.h mesa/main/shaderobj.h mesa/main/shaderapi.h
#        mesa/main/uniforms.h mesa/main/glspirv.h mesa/main/extensions_table.h mesa/main/accum.h
#        mesa/main/condrender.h mesa/main/queryobj.h mesa/main/api_exec.h mesa/main/attrib.h
#        mesa/main/clear.h mesa/main/depth.h mesa/main/fog.h mesa/main/hint.h mesa/main/lines.h
#        mesa/main/matrix.h mesa/main/points.h mesa/main/polygon.h mesa/main/pack.h
#        mesa/main/format_utils.h mesa/main/shared.h mesa/main/atifragshader.h mesa/main/dlist.h
#        mesa/main/syncobj.h mesa/main/scissor.h mesa/main/texenv.h mesa/main/texgen.h
#        mesa/main/texparam.h mesa/main/viewport.h mesa/main/bbox.h mesa/main/conservativeraster.h
#        mesa/main/cpuinfo.h mesa/main/debug.h mesa/main/readpix.h mesa/main/eval.h
#        mesa/main/feedback.h mesa/main/glthread.h mesa/main/glthread_marshal.h mesa/main/git_sha1.h
#        mesa/main/performance_monitor.h mesa/main/performance_query.h mesa/main/pipelineobj.h
#        mesa/main/rastpos.h mesa/main/remap.h mesa/main/remap_helper.h mesa/main/marshal_generated.h
#        mesa/main/api_arrayelt.h mesa/main/arbprogram.h mesa/main/vtxfmt.h mesa/main/format_info.h
#        mesa/main/get_hash.h mesa/main/ffvertex_prog.h mesa/main/texenvprogram.h
#        mesa/main/program_binary.h mesa/main/texstate.h mesa/main/sse_minmax.h mesa/main/blit.h
#        mesa/main/colortab.h mesa/main/compute.h mesa/main/convolve.h mesa/main/copyimage.h
#        mesa/main/drawpix.h mesa/main/drawtex.h mesa/main/es1_conversion.h mesa/main/genmipmap.h
#        mesa/main/histogram.h mesa/main/objectlabel.h mesa/main/objectpurge.h
#        mesa/main/texgetimage.h mesa/main/program_resource.h mesa/main/barrier.h
#        mesa/main/formatquery.h mesa/main/vdpau.h mesa/main/querymatrix.h mesa/main/texformat.h
#        mesa/main/execmem.h mesa/main/texcompress_astc.h
#
#        mesa/main/formats.c mesa/main/errors.c mesa/main/context.c mesa/main/glformats.c
#        mesa/main/texcompress.c mesa/main/debug_output.c mesa/main/texcompress_fxt1.c
#        mesa/main/texstore.c mesa/main/version.c mesa/main/hash.c mesa/main/image.c
#        mesa/main/mipmap.c mesa/main/texcompress_s3tc.c mesa/main/texcompress_rgtc.c
#        mesa/main/draw_validate.c mesa/main/texcompress_etc.c mesa/main/texcompress_bptc.c
#        mesa/main/texobj.c mesa/main/teximage.c mesa/main/samplerobj.c mesa/main/texturebindless.c
#        mesa/main/shaderimage.c mesa/main/bufferobj.c mesa/main/externalobjects.c mesa/main/draw.c
#        mesa/main/texstorage.c mesa/main/textureview.c mesa/main/transformfeedback.c
#        mesa/main/varray.c mesa/main/enable.c mesa/main/arrayobj.c mesa/main/state.c mesa/main/get.c
#        mesa/main/blend.c mesa/main/extensions.c mesa/main/spirv_extensions.c mesa/main/stencil.c
#        mesa/main/framebuffer.c mesa/main/fbobject.c mesa/main/buffers.c mesa/main/multisample.c
#        mesa/main/renderbuffer.c mesa/main/clip.c mesa/main/light.c mesa/main/pixel.c
#        mesa/main/pbo.c mesa/main/pixelstore.c mesa/main/pixeltransfer.c
#        mesa/main/texcompress_cpal.c mesa/main/shaderobj.c mesa/main/shaderapi.c
#        mesa/main/uniforms.c mesa/main/glspirv.c mesa/main/extensions_table.c mesa/main/accum.c
#        mesa/main/condrender.c mesa/main/queryobj.c mesa/main/attrib.c mesa/main/clear.c
#        mesa/main/depth.c mesa/main/fog.c mesa/main/hint.c mesa/main/lines.c mesa/main/matrix.c
#        mesa/main/points.c mesa/main/polygon.c mesa/main/pack.c mesa/main/format_utils.c
#        mesa/main/shared.c mesa/main/atifragshader.c mesa/main/dlist.c mesa/main/syncobj.c
#        mesa/main/scissor.c mesa/main/texenv.c mesa/main/texgen.c mesa/main/texparam.c
#        mesa/main/viewport.c mesa/main/bbox.c mesa/main/conservativeraster.c mesa/main/cpuinfo.c
#        mesa/main/debug.c mesa/main/readpix.c mesa/main/eval.c mesa/main/feedback.c
#        mesa/main/glthread.c mesa/main/performance_monitor.c mesa/main/performance_query.c
#        mesa/main/pipelineobj.c mesa/main/rastpos.c mesa/main/remap.c
#        mesa/main/marshal_generated0.c mesa/main/marshal_generated1.c mesa/main/marshal_generated2.c
#        mesa/main/marshal_generated3.c mesa/main/marshal_generated4.c mesa/main/marshal_generated5.c
#        mesa/main/marshal_generated6.c mesa/main/marshal_generated7.c
#        mesa/main/api_arrayelt.c mesa/main/arbprogram.c mesa/main/vtxfmt.c mesa/main/ffvertex_prog.c
#        mesa/main/program_binary.c mesa/main/enums.c mesa/main/glthread_varray.c
#        mesa/main/glthread_shaderobj.c mesa/main/glthread_get.c mesa/main/glthread_draw.c
#        mesa/main/glthread_bufferobj.c mesa/main/getstring.c mesa/main/shader_query.cpp
#        mesa/main/uniform_query.cpp mesa/main/texstate.c mesa/main/sse_minmax.c mesa/main/api_exec.c
#        mesa/main/format_fallback.c mesa/main/format_pack.c mesa/main/blit.c mesa/main/colortab.c
#        mesa/main/compute.c mesa/main/convolve.c mesa/main/copyimage.c mesa/main/drawpix.c
#        mesa/main/drawtex.c mesa/main/es1_conversion.c mesa/main/genmipmap.c mesa/main/histogram.c
#        mesa/main/objectlabel.c mesa/main/objectpurge.c mesa/main/texgetimage.c
#        mesa/main/program_resource.c mesa/main/barrier.c mesa/main/formatquery.c mesa/main/vdpau.c
#        mesa/main/robustness.c mesa/main/ff_fragment_shader.cpp mesa/main/querymatrix.c
#        mesa/main/texformat.c mesa/main/execmem.c mesa/main/texcompress_astc.cpp
#
#        mesa/util/macros.h mesa/util/u_math.h mesa/util/u_cpu_detect.h mesa/util/u_thread.h
#        mesa/util/detect_os.h mesa/util/u_endian.h mesa/util/bitscan.h mesa/util/u_debug.h
#        mesa/util/os_misc.h mesa/util/os_file.h mesa/util/u_string.h mesa/util/rounding.h
#        mesa/util/compiler.h mesa/util/hash_table.h mesa/util/u_memory.h mesa/util/os_memory.h
#        mesa/util/os_memory_stdc.h mesa/util/os_memory_aligned.h mesa/util/fast_urem_by_const.h
#        mesa/util/xxhash.h mesa/util/ralloc.h mesa/util/list.h mesa/util/u_idalloc.h
#        mesa/util/simple_mtx.h mesa/util/futex.h mesa/util/format_srgb.h mesa/util/rgtc.h
#        mesa/util/texcompress_rgtc_tmp.h mesa/util/half_float.h mesa/util/softfloat.h
#        mesa/util/format_rgb9e5.h mesa/util/format_r11g11b10f.h mesa/util/debug.h
#        mesa/util/u_atomic.h mesa/util/set.h mesa/util/bitset.h mesa/util/blob.h mesa/util/driconf.h
#        mesa/util/disk_cache.h mesa/util/mesa-sha1.h mesa/util/build_id.h mesa/util/disk_cache_os.h
#        mesa/util/u_queue.h mesa/util/os_time.h mesa/util/fossilize_db.h mesa/util/u_process.h
#        mesa/util/crc32.h mesa/util/rand_xor.h mesa/util/compress.h mesa/util/enum_operators.h
#        mesa/util/strtod.h mesa/util/u_dynarray.h mesa/util/simple_list.h mesa/util/strndup.h
#        mesa/util/string_buffer.h mesa/util/double.h mesa/util/bigmath.h mesa/util/u_vector.h
#        mesa/util/memstream.h mesa/util/u_printf.h mesa/util/u_inlines.h mesa/util/u_box.h
#        mesa/util/u_debug_refcnt.h mesa/util/u_debug_describe.h mesa/util/u_debug_stack.h
#        mesa/util/u_debug_symbol.h mesa/util/u_hash_table.h mesa/util/u_pointer.h
#        mesa/util/dbghelp.h mesa/util/u_draw_quad.h mesa/util/u_draw.h mesa/util/u_framebuffer.h
#        mesa/util/u_vbuf.h mesa/util/u_dump.h mesa/util/u_screen.h mesa/util/u_upload_mgr.h
#        mesa/util/u_sampler.h mesa/util/u_simple_shaders.h mesa/util/u_prim.h mesa/util/u_bitmask.h
#        mesa/util/u_mm.h mesa/util/xmlconfig.h mesa/util/u_driconf.h mesa/util/u_tests.h
#        mesa/util/u_surface.h mesa/util/u_pack_color.h mesa/util/u_tile.h mesa/util/u_memset.h
#        mesa/util/u_rect.h mesa/util/u_dl.h mesa/util/u_file.h mesa/util/u_transfer.h
#        mesa/util/u_helpers.h mesa/util/u_threaded_context.h mesa/util/u_range.h mesa/util/slab.h
#        mesa/util/log.h mesa/util/u_log.h mesa/util/u_network.h mesa/util/u_gen_mipmap.h
#
#        mesa/util/u_math.c mesa/util/u_cpu_detect.c mesa/util/bitscan.c mesa/util/u_debug.c
#        mesa/util/os_misc.c mesa/util/os_file.c mesa/util/hash_table.c mesa/util/ralloc.c
#        mesa/util/u_idalloc.c mesa/util/rgtc.c mesa/util/half_float.c mesa/util/softfloat.c
#        mesa/util/debug.c mesa/util/u_atomic.c mesa/util/set.c mesa/util/blob.c
#        mesa/util/disk_cache.c mesa/util/mesa-sha1.c mesa/util/build_id.c mesa/util/disk_cache_os.c
#        mesa/util/u_queue.c mesa/util/os_time.c mesa/util/fossilize_db.c mesa/util/u_process.c
#        mesa/util/crc32.c mesa/util/rand_xor.c mesa/util/compress.c mesa/util/strtod.c
#        mesa/util/string_buffer.c mesa/util/double.c mesa/util/u_vector.c mesa/util/format_srgb.c
#        mesa/util/memstream.c mesa/util/u_printf.cpp mesa/util/u_debug_refcnt.c
#        mesa/util/u_debug_describe.c mesa/util/u_debug_stack.c mesa/util/u_debug_symbol.c
#        mesa/util/u_hash_table.c mesa/util/u_draw_quad.c mesa/util/u_draw.c
#        mesa/util/u_framebuffer.c mesa/util/u_vbuf.c mesa/util/u_screen.c mesa/util/u_upload_mgr.c
#        mesa/util/u_sampler.c mesa/util/u_simple_shaders.c mesa/util/u_prim.c mesa/util/u_bitmask.c
#        mesa/util/u_mm.c mesa/util/xmlconfig.c mesa/util/u_driconf.c mesa/util/u_tests.c
#        mesa/util/u_surface.c mesa/util/u_tile.c mesa/util/u_dl.c mesa/util/u_transfer.c
#        mesa/util/u_helpers.c mesa/util/u_threaded_context.c mesa/util/slab.c mesa/util/log.c
#        mesa/util/u_log.c mesa/util/u_network.c mesa/util/u_gen_mipmap.c
#
#        mesa/util/format/u_format_fxt1.h mesa/util/format/u_format.h
#        mesa/util/format/u_format_s3tc.h mesa/util/format/u_format_pack.h
#        mesa/util/format/u_format_bptc.h mesa/util/format/u_format_etc.h
#        mesa/util/format/u_format_latc.h mesa/util/format/u_format_other.h
#        mesa/util/format/u_format_rgtc.h mesa/util/format/u_format_tests.h
#        mesa/util/format/u_format_yuv.h mesa/util/format/u_format_zs.h
#        mesa/util/format/format_utils.h
#        mesa/util/format/u_format_fxt1.c mesa/util/format/u_format.c
#        mesa/util/format/u_format_s3tc.c mesa/util/format/u_format_pack.c
#        mesa/util/format/u_format_bptc.c mesa/util/format/u_format_etc.c
#        mesa/util/format/u_format_latc.c mesa/util/format/u_format_other.c
#        mesa/util/format/u_format_rgtc.c mesa/util/format/u_format_tests.c
#        mesa/util/format/u_format_yuv.c mesa/util/format/u_format_zs.c
#
#        mesa/util/sha1/sha1.h
#        mesa/util/sha1/sha1.c
#
#
#        mesa/include/c11_compat.h mesa/include/c99_compat.h mesa/include/no_extern_c.h
#        mesa/include/c99_math.h mesa/include/c99_alloca.h mesa/include/vulkan_core.h
#
#        mesa/include/pipe/p_config.h mesa/include/pipe/p_format.h mesa/include/pipe/p_compiler.h
#        mesa/include/pipe/p_state.h mesa/include/pipe/p_defines.h mesa/include/pipe/p_screen.h
#        mesa/include/pipe/p_video_enums.h mesa/include/pipe/p_context.h
#        mesa/include/pipe/p_shader_tokens.h mesa/include/pipe/p_video_codec.h
#        mesa/include/pipe/p_video_state.h
#
#        mesa/include/c11/threads.h mesa/include/c11/threads_win32.h mesa/include/c11/threads_posix.h
#
#
#        mesa/math/m_matrix.h mesa/math/m_eval.h mesa/math/m_vector_asm.h mesa/math/m_vector.h
#        mesa/math/m_xform.h mesa/math/m_xform_tmp.h mesa/math/m_clip_tmp.h mesa/math/m_norm_tmp.h
#        mesa/math/m_dotprod_tmp.h mesa/math/m_copy_tmp.h mesa/math/m_translate.h
#        mesa/math/m_trans_tmp.h
#
#        mesa/math/m_matrix.c mesa/math/m_eval.c mesa/math/m_vector.c mesa/math/m_xform.c
#        mesa/math/m_translate.c
#
#
#        mesa/program/prog_instruction.h mesa/program/prog_parameter.h mesa/program/prog_statevars.h
#        mesa/program/program.h mesa/program/prog_cache.h mesa/program/ir_to_mesa.h
#        mesa/program/symbol_table.h mesa/program/arbprogparse.h mesa/program/programopt.h
#        mesa/program/prog_optimize.h mesa/program/prog_print.h mesa/program/program_parser.h
#        mesa/program/program_parse.h mesa/program/prog_parameter_layout.h
#        mesa/program/prog_execute.h mesa/program/prog_noise.h mesa/program/prog_to_nir.h
#
#        mesa/program/prog_instruction.c mesa/program/prog_parameter.c mesa/program/prog_statevars.c
#        mesa/program/program.c mesa/program/prog_cache.c mesa/program/ir_to_mesa.cpp
#        mesa/program/symbol_table.c mesa/program/arbprogparse.c mesa/program/programopt.c
#        mesa/program/prog_optimize.c mesa/program/prog_print.c mesa/program/program_parse.c
#        mesa/program/prog_parameter_layout.c mesa/program/program_lexer.c
#        mesa/program/program_parse_extra.c mesa/program/prog_opt_constant_fold.c
#        mesa/program/prog_execute.c mesa/program/prog_noise.c mesa/program/prog_to_nir.c
#
#
#        mesa/compiler/shader_enums.h mesa/compiler/glsl_types.h mesa/compiler/shader_info.h
#        mesa/compiler/nir_types.h mesa/compiler/builtin_type_macros.h
#
#        mesa/compiler/shader_enums.c mesa/compiler/glsl_types.cpp mesa/compiler/nir_types.cpp
#
#
#        mesa/compiler/glsl/string_to_uint_map.h mesa/compiler/glsl/glsl_parser_extras.h
#        mesa/compiler/glsl/glsl_symbol_table.h mesa/compiler/glsl/ir_uniform.h
#        mesa/compiler/glsl/ast.h mesa/compiler/glsl/list.h mesa/compiler/glsl/ir.h
#        mesa/compiler/glsl/ir_visitor.h mesa/compiler/glsl/ir_hierarchical_visitor.h
#        mesa/compiler/glsl/glsl_parser.h mesa/compiler/glsl/ir_optimization.h
#        mesa/compiler/glsl/loop_analysis.h mesa/compiler/glsl/builtin_functions.h
#        mesa/compiler/glsl/ir_builder.h mesa/compiler/glsl/ir_expression_operation.h
#        mesa/compiler/glsl/serialize.h mesa/compiler/glsl/program.h
#        mesa/compiler/glsl/ir_expression_flattening.h mesa/compiler/glsl/ir_rvalue_visitor.h
#        mesa/compiler/glsl/linker.h mesa/compiler/glsl/shader_cache.h
#        mesa/compiler/glsl/linker_util.h mesa/compiler/glsl/link_varyings.h
#        mesa/compiler/glsl/ir_expression_operation_strings.h
#        mesa/compiler/glsl/link_uniform_block_active_visitor.h
#        mesa/compiler/glsl/ir_array_refcount.h mesa/compiler/glsl/lower_buffer_access.h
#        mesa/compiler/glsl/builtin_int64.h mesa/compiler/glsl/ir_expression_operation_constant.h
#        mesa/compiler/glsl/ir_function_inlining.h mesa/compiler/glsl/ir_variable_refcount.h
#        mesa/compiler/glsl/ir_print_visitor.h mesa/compiler/glsl/ir_basic_block.h
#        mesa/compiler/glsl/opt_add_neg_to_sub.h mesa/compiler/glsl/gl_nir.h
#
#        mesa/compiler/glsl/string_to_uint_map.cpp mesa/compiler/glsl/glsl_parser_extras.cpp
#        mesa/compiler/glsl/glsl_symbol_table.cpp mesa/compiler/glsl/ir.cpp
#        mesa/compiler/glsl/ir_hierarchical_visitor.cpp mesa/compiler/glsl/glsl_parser.cpp
#        mesa/compiler/glsl/loop_analysis.cpp mesa/compiler/glsl/builtin_functions.cpp
#        mesa/compiler/glsl/ir_builder.cpp mesa/compiler/glsl/serialize.cpp
#        mesa/compiler/glsl/ir_expression_flattening.cpp mesa/compiler/glsl/ir_rvalue_visitor.cpp
#        mesa/compiler/glsl/linker.cpp mesa/compiler/glsl/shader_cache.cpp
#        mesa/compiler/glsl/linker_util.cpp mesa/compiler/glsl/link_varyings.cpp
#        mesa/compiler/glsl/link_uniform_blocks.cpp mesa/compiler/glsl/link_atomics.cpp
#        mesa/compiler/glsl/link_uniform_block_active_visitor.cpp
#        mesa/compiler/glsl/link_uniform_initializers.cpp mesa/compiler/glsl/link_functions.cpp
#        mesa/compiler/glsl/link_interface_blocks.cpp mesa/compiler/glsl/link_uniforms.cpp
#        mesa/compiler/glsl/ir_array_refcount.cpp mesa/compiler/glsl/lower_vertex_id.cpp
#        mesa/compiler/glsl/lower_vector_insert.cpp mesa/compiler/glsl/lower_vector_derefs.cpp
#        mesa/compiler/glsl/lower_vector.cpp mesa/compiler/glsl/lower_vec_index_to_swizzle.cpp
#        mesa/compiler/glsl/lower_vec_index_to_cond_assign.cpp
#        mesa/compiler/glsl/lower_variable_index_to_cond_assign.cpp
#        mesa/compiler/glsl/lower_ubo_reference.cpp mesa/compiler/glsl/lower_texture_projection.cpp
#        mesa/compiler/glsl/lower_buffer_access.cpp mesa/compiler/glsl/lower_builtins.cpp
#        mesa/compiler/glsl/lower_blend_equation_advanced.cpp mesa/compiler/glsl/lower_cs_derived.cpp
#        mesa/compiler/glsl/lower_const_arrays_to_uniforms.cpp mesa/compiler/glsl/lower_discard.cpp
#        mesa/compiler/glsl/lower_discard_flow.cpp mesa/compiler/glsl/lower_distance.cpp
#        mesa/compiler/glsl/lower_if_to_cond_assign.cpp mesa/compiler/glsl/lower_instructions.cpp
#        mesa/compiler/glsl/lower_int64.cpp mesa/compiler/glsl/lower_jumps.cpp
#        mesa/compiler/glsl/lower_mat_op_to_vec.cpp mesa/compiler/glsl/lower_offset_array.cpp
#        mesa/compiler/glsl/lower_named_interface_blocks.cpp
#        mesa/compiler/glsl/lower_output_reads.cpp mesa/compiler/glsl/lower_packed_varyings.cpp
#        mesa/compiler/glsl/lower_packing_builtins.cpp mesa/compiler/glsl/lower_precision.cpp
#        mesa/compiler/glsl/lower_shared_reference.cpp mesa/compiler/glsl/lower_subroutine.cpp
#        mesa/compiler/glsl/lower_tess_level.cpp mesa/compiler/glsl/generate_ir.cpp
#        mesa/compiler/glsl/ast_type.cpp mesa/compiler/glsl/ast_array_index.cpp
#        mesa/compiler/glsl/ast_expr.cpp mesa/compiler/glsl/ast_function.cpp
#        mesa/compiler/glsl/ast_to_hir.cpp mesa/compiler/glsl/ir_clone.cpp
#        mesa/compiler/glsl/ir_constant_expression.cpp mesa/compiler/glsl/ir_hv_accept.cpp
#        mesa/compiler/glsl/ir_function.cpp mesa/compiler/glsl/ir_function_can_inline.cpp
#        mesa/compiler/glsl/ir_function_detect_recursion.cpp mesa/compiler/glsl/ir_equals.cpp
#        mesa/compiler/glsl/hir_field_selection.cpp mesa/compiler/glsl/builtin_variables.cpp
#        mesa/compiler/glsl/opt_function_inlining.cpp mesa/compiler/glsl/opt_dead_code.cpp
#        mesa/compiler/glsl/ir_variable_refcount.cpp mesa/compiler/glsl/opt_dead_builtin_varyings.cpp
#        mesa/compiler/glsl/opt_dead_builtin_variables.cpp mesa/compiler/glsl/ir_print_visitor.cpp
#        mesa/compiler/glsl/opt_dead_code_local.cpp mesa/compiler/glsl/opt_dead_functions.cpp
#        mesa/compiler/glsl/ir_basic_block.cpp mesa/compiler/glsl/opt_algebraic.cpp
#        mesa/compiler/glsl/opt_array_splitting.cpp mesa/compiler/glsl/opt_conditional_discard.cpp
#        mesa/compiler/glsl/opt_constant_folding.cpp mesa/compiler/glsl/opt_constant_propagation.cpp
#        mesa/compiler/glsl/opt_constant_variable.cpp mesa/compiler/glsl/opt_if_simplification.cpp
#        mesa/compiler/glsl/opt_copy_propagation_elements.cpp mesa/compiler/glsl/opt_minmax.cpp
#        mesa/compiler/glsl/opt_flatten_nested_if_blocks.cpp mesa/compiler/glsl/opt_flip_matrices.cpp
#        mesa/compiler/glsl/opt_rebalance_tree.cpp mesa/compiler/glsl/opt_redundant_jumps.cpp
#        mesa/compiler/glsl/opt_structure_splitting.cpp mesa/compiler/glsl/opt_swizzle.cpp
#        mesa/compiler/glsl/opt_tree_grafting.cpp mesa/compiler/glsl/opt_vectorize.cpp
#        mesa/compiler/glsl/builtin_types.cpp mesa/compiler/glsl/glsl_lexer.cpp
#        mesa/compiler/glsl/ir_validate.cpp mesa/compiler/glsl/loop_unroll.cpp
#        mesa/compiler/glsl/propagate_invariance.cpp mesa/compiler/glsl/lower_xfb_varying.cpp
#        mesa/compiler/glsl/ir_set_program_inouts.cpp
#
#        mesa/compiler/glsl/glcpp/glcpp.h mesa/compiler/glsl/glcpp/glcpp-parse.h
#        mesa/compiler/glsl/glcpp/pp.c mesa/compiler/glsl/glcpp/glcpp.c
#        mesa/compiler/glsl/glcpp/glcpp-parse.c mesa/compiler/glsl/glcpp/glcpp-lex.c
#
#
#        mesa/compiler/spirv/nir_spirv.h mesa/compiler/spirv/vtn_private.h
#        mesa/compiler/spirv/spirv.h mesa/compiler/spirv/vtn_generator_ids.h
#        mesa/compiler/spirv/GLSL.ext.AMD.h mesa/compiler/spirv/GLSL.std.450.h
#        mesa/compiler/spirv/OpenCL.std.h
#
#        mesa/compiler/spirv/gl_spirv.c mesa/compiler/spirv/spirv_info.c
#        mesa/compiler/spirv/spirv_to_nir.c mesa/compiler/spirv/vtn_variables.c
#        mesa/compiler/spirv/vtn_amd.c mesa/compiler/spirv/vtn_glsl450.c
#        mesa/compiler/spirv/vtn_alu.c mesa/compiler/spirv/vtn_opencl.c
#        mesa/compiler/spirv/vtn_subgroup.c mesa/compiler/spirv/vtn_cfg.c
#        mesa/compiler/spirv/vtn_gather_types.c
#
#
#        mesa/compiler/nir/nir.h mesa/compiler/nir/nir_builder.h mesa/compiler/nir/nir_control_flow.h
#        mesa/compiler/nir/nir_control_flow_private.h mesa/compiler/nir/nir_opcodes.h
#        mesa/compiler/nir/nir_intrinsics.h mesa/compiler/nir/nir_intrinsics_indices.h
#        mesa/compiler/nir/nir_inline_helpers.h mesa/compiler/nir/nir_builder_opcodes.h
#        mesa/compiler/nir/nir_loop_analyze.h mesa/compiler/nir/nir_constant_expressions.h
#        mesa/compiler/nir/nir_instr_set.h mesa/compiler/nir/nir_vla.h
#        mesa/compiler/nir/nir_worklist.h mesa/compiler/nir/nir_serialize.h
#        mesa/compiler/nir/nir_deref.h mesa/compiler/nir/nir_builtin_builder.h
#        mesa/compiler/nir/nir_phi_builder.h mesa/compiler/nir/nir_to_tgsi.h
#        mesa/compiler/nir/nir_format_convert.h
#
#        mesa/compiler/nir/nir.c mesa/compiler/nir/nir_control_flow.c mesa/compiler/nir/nir_opcodes.c
#        mesa/compiler/nir/nir_intrinsics.c mesa/compiler/nir/nir_metadata.c
#        mesa/compiler/nir/nir_loop_analyze.c mesa/compiler/nir/nir_constant_expressions.c
#        mesa/compiler/nir/nir_dominance.c mesa/compiler/nir/nir_instr_set.c
#        mesa/compiler/nir/nir_lower_int64.c mesa/compiler/nir/nir_lower_double_ops.c
#        mesa/compiler/nir/nir_live_ssa_defs_impl.c mesa/compiler/nir/nir_worklist.c
#        mesa/compiler/nir/nir_divergence_analysis.c mesa/compiler/nir/nir_inline_functions.c
#        mesa/compiler/nir/nir_clone.c mesa/compiler/nir/nir_print.c mesa/compiler/nir/nir_validate.c
#        mesa/compiler/nir/nir_lower_variable_initializers.c mesa/compiler/nir/nir_serialize.c
#        mesa/compiler/nir/nir_split_per_member_structs.c mesa/compiler/nir/nir_deref.c
#        mesa/compiler/nir/nir_lower_io.c mesa/compiler/nir/nir_builtin_builder.c
#        mesa/compiler/nir/nir_opt_dce.c mesa/compiler/nir/nir_remove_dead_variables.c
#        mesa/compiler/nir/nir_lower_goto_ifs.c mesa/compiler/nir/nir_lower_frexp.c
#        mesa/compiler/nir/nir_split_var_copies.c mesa/compiler/nir/nir_opt_copy_propagate.c
#        mesa/compiler/nir/nir_lower_returns.c mesa/compiler/nir/nir_repair_ssa.c
#        mesa/compiler/nir/nir_phi_builder.c mesa/compiler/nir/nir_lower_regs_to_ssa.c
#        mesa/compiler/nir/nir_from_ssa.c mesa/compiler/nir/nir_to_tgsi.c
#
#
#        mesa/vbo/vbo.h mesa/vbo/vbo_attrib.h mesa/vbo/vbo_init_tmp.h mesa/vbo/vbo_util.h
#        mesa/vbo/vbo_save.h mesa/vbo/vbo_exec.h mesa/vbo/vbo_private.h mesa/vbo/vbo_attrib_tmp.h
#        mesa/vbo/vbo_noop.h
#
#        mesa/vbo/vbo_save.c mesa/vbo/vbo_exec.c mesa/vbo/vbo_save_api.c mesa/vbo/vbo_noop.c
#        mesa/vbo/vbo_exec_api.c mesa/vbo/vbo_save_draw.c mesa/vbo/vbo_save_loopback.c
#        mesa/vbo/vbo_exec_draw.c mesa/vbo/vbo_exec_eval.c mesa/vbo/vbo_context.c
#        mesa/vbo/vbo_get_minmax_index_mapped.c
#
#
#        mesa/tnl/tnl.h mesa/tnl/t_context.h mesa/tnl/t_pipeline.h mesa/tnl/t_vp_build.h
#        mesa/tnl/t_vertex.h mesa/tnl/t_rebase.h mesa/tnl/t_vb_cliptmp.h mesa/tnl/t_vb_rendertmp.h
#        mesa/tnl/t_vb_lighttmp.h mesa/tnl/t_split.h
#
#        mesa/tnl/t_context.c mesa/tnl/t_pipeline.c mesa/tnl/t_vp_build.c mesa/tnl/t_vertex.c
#        mesa/tnl/t_draw.c mesa/tnl/t_rebase.c mesa/tnl/t_vb_program.c mesa/tnl/t_vertex_generic.c
#        mesa/tnl/t_vb_render.c mesa/tnl/t_vertex_sse.c mesa/tnl/t_vb_light.c mesa/tnl/t_vb_texgen.c
#        mesa/tnl/t_vb_texmat.c mesa/tnl/t_vb_vertex.c mesa/tnl/t_split.c mesa/tnl/t_vb_fog.c
#        mesa/tnl/t_vb_points.c mesa/tnl/t_vb_normals.c mesa/tnl/t_split_copy.c
#        mesa/tnl/t_split_inplace.c
#
#
#        mesa/mapi/u_current.h mesa/mapi/table.h mesa/mapi/stub.h mesa/mapi/entry.h
#        mesa/mapi/mapi_tmp.h mesa/mapi/glapi_mapi_tmp.h mesa/mapi/shared_glapi_mapi_tmp.h
#
#        mesa/mapi/u_current.c mesa/mapi/table.c mesa/mapi/stub.c mesa/mapi/entry.c
#        mesa/mapi/mapi_glapi.c
#
#        mesa/mapi/glapi/glapi.h mesa/mapi/glapi/glapitable.h mesa/mapi/glapi/glapioffsets.h
#        mesa/mapi/glapi/glapiremap_table.h mesa/mapi/glapi/glapidispatch.h
#        mesa/mapi/glapi/glapi_priv.h mesa/mapi/glapi/glapitemp.h mesa/mapi/glapi/glprocs.h
#
#        mesa/mapi/glapi/glapi.c mesa/mapi/glapi/glapi_getproc.c mesa/mapi/glapi/glapi_dispatch.c
#        mesa/mapi/glapi/glapi_entrypoint.c mesa/mapi/glapi/glapi_nop.c
#
#
#        mesa/swrast/s_context.h mesa/swrast/swrast.h mesa/swrast/s_chan.h mesa/swrast/s_span.h
#        mesa/swrast/s_fragprog.h mesa/swrast/s_blend.h mesa/swrast/s_lines.h mesa/swrast/s_points.h
#        mesa/swrast/s_feedback.h mesa/swrast/s_triangle.h mesa/swrast/s_aaline.h
#        mesa/swrast/s_aalinetemp.h mesa/swrast/s_aatriangle.h mesa/swrast/s_aatritemp.h
#        mesa/swrast/s_texfilter.h mesa/swrast/s_texfetch.h mesa/swrast/s_texfetch_tmp.h
#        mesa/swrast/s_atifragshader.h mesa/swrast/s_alpha.h mesa/swrast/s_depth.h
#        mesa/swrast/s_fog.h mesa/swrast/s_logic.h mesa/swrast/s_masking.h mesa/swrast/s_stencil.h
#        mesa/swrast/s_texcombine.h mesa/swrast/s_linetemp.h mesa/swrast/s_tritemp.h
#        mesa/swrast/s_renderbuffer.h mesa/swrast/s_zoom.h
#
#        mesa/swrast/s_context.c mesa/swrast/s_span.c mesa/swrast/s_fragprog.c mesa/swrast/s_blend.c
#        mesa/swrast/s_lines.c mesa/swrast/s_points.c mesa/swrast/s_feedback.c
#        mesa/swrast/s_triangle.c mesa/swrast/s_aaline.c mesa/swrast/s_aatriangle.c
#        mesa/swrast/s_texfilter.c mesa/swrast/s_texfetch.c mesa/swrast/s_atifragshader.c
#        mesa/swrast/s_alpha.c mesa/swrast/s_depth.c mesa/swrast/s_fog.c mesa/swrast/s_logic.c
#        mesa/swrast/s_masking.c mesa/swrast/s_stencil.c mesa/swrast/s_texcombine.c
#        mesa/swrast/s_renderbuffer.c mesa/swrast/s_drawpix.c mesa/swrast/s_zoom.c
#        mesa/swrast/s_clear.c mesa/swrast/s_bitmap.c mesa/swrast/s_copypix.c mesa/swrast/s_blit.c
#        mesa/swrast/s_texrender.c mesa/swrast/s_texture.c
#
#
#        mesa/swrast_setup/swrast_setup.h mesa/swrast_setup/ss_context.h mesa/swrast_setup/ss_vb.h
#        mesa/swrast_setup/ss_triangle.h mesa/swrast_setup/ss_tritmp.h
#
#        mesa/swrast_setup/ss_context.c mesa/swrast_setup/ss_triangle.c
#
#
#        mesa/drivers/common/driverfuncs.h mesa/drivers/common/driverfuncs.c
#        mesa/drivers/common/meta.h mesa/drivers/common/meta.c mesa/drivers/common/meta_blit.c
#        mesa/drivers/common/meta_generate_mipmap.c
#
#
#        mesa/drivers/x11/glxapi.h mesa/drivers/x11/glxheader.h mesa/drivers/x11/xfonts.h
#        mesa/drivers/x11/xmesaP.h mesa/drivers/x11/xmesa.h mesa/drivers/x11/xmesa_x.h
#
#        mesa/drivers/x11/glxapi.c mesa/drivers/x11/fakeglx.c mesa/drivers/x11/xfonts.c
#        mesa/drivers/x11/xm_api.c mesa/drivers/x11/xm_buffer.c mesa/drivers/x11/xm_dd.c
#        mesa/drivers/x11/xm_tri.c mesa/drivers/x11/xm_line.c
#
#
#        mesa/gallium/frontend/api.h mesa/gallium/frontend/xlibsw_api.h
#        mesa/gallium/frontend/sw_winsys.h mesa/gallium/frontend/winsys_handle.h
#        mesa/gallium/frontend/drm_driver.h mesa/gallium/frontend/opencl_interop.h
#        mesa/gallium/frontend/vdpau_dmabuf.h mesa/gallium/frontend/vdpau_interop.h
#        mesa/gallium/frontend/vdpau_funcs.h
#
#
#        mesa/gallium/frontends/xm_api.h mesa/gallium/frontends/xm_st.h
#        mesa/gallium/frontends/xm_public.h
#        mesa/gallium/frontends/xm_api.c mesa/gallium/frontends/xm_st.c
#
#        mesa/gallium/frontends/dri/dri_util.h mesa/gallium/frontends/dri/dri_util.c
#        mesa/gallium/frontends/dri/utils.h mesa/gallium/frontends/dri/utils.c
#        mesa/gallium/frontends/dri/dri_context.h mesa/gallium/frontends/dri/dri_context.c
#        mesa/gallium/frontends/dri/dri_drawable.h mesa/gallium/frontends/dri/dri_drawable.c
#        mesa/gallium/frontends/dri/dri_helpers.h mesa/gallium/frontends/dri/dri_helpers.c
#
#
#        mesa/gallium/hud/hud_context.h mesa/gallium/hud/hud_private.h mesa/gallium/hud/font.h
#
#        mesa/gallium/hud/hud_context.c mesa/gallium/hud/font.c mesa/gallium/hud/hud_driver_query.c
#        mesa/gallium/hud/hud_cpu.c mesa/gallium/hud/hud_fps.c
#
#
#        mesa/gallium/tgsi/tgsi_parse.h mesa/gallium/tgsi/tgsi_text.h mesa/gallium/tgsi/tgsi_dump.h
#        mesa/gallium/tgsi/tgsi_strings.h mesa/gallium/tgsi/tgsi_ureg.h mesa/gallium/tgsi/tgsi_info.h
#        mesa/gallium/tgsi/tgsi_info_opcodes.h mesa/gallium/tgsi/tgsi_iterate.h
#        mesa/gallium/tgsi/tgsi_build.h mesa/gallium/tgsi/tgsi_sanity.h mesa/gallium/tgsi/tgsi_util.h
#        mesa/gallium/tgsi/tgsi_exec.h mesa/gallium/tgsi/tgsi_from_mesa.h
#        mesa/gallium/tgsi/tgsi_opcode_tmp.h mesa/gallium/tgsi/tgsi_scan.h
#        mesa/gallium/tgsi/tgsi_emulate.h mesa/gallium/tgsi/tgsi_transform.h
#
#        mesa/gallium/tgsi/tgsi_parse.c mesa/gallium/tgsi/tgsi_text.c mesa/gallium/tgsi/tgsi_dump.c
#        mesa/gallium/tgsi/tgsi_strings.c mesa/gallium/tgsi/tgsi_ureg.c mesa/gallium/tgsi/tgsi_info.c
#        mesa/gallium/tgsi/tgsi_iterate.c mesa/gallium/tgsi/tgsi_build.c
#        mesa/gallium/tgsi/tgsi_sanity.c mesa/gallium/tgsi/tgsi_util.c mesa/gallium/tgsi/tgsi_exec.c
#        mesa/gallium/tgsi/tgsi_from_mesa.c mesa/gallium/tgsi/tgsi_scan.c
#        mesa/gallium/tgsi/tgsi_emulate.c mesa/gallium/tgsi/tgsi_transform.c
#
#
#        mesa/gallium/cso_cache/cso_context.h mesa/gallium/cso_cache/cso_context.c
#        mesa/gallium/cso_cache/cso_cache.h mesa/gallium/cso_cache/cso_cache.c
#        mesa/gallium/cso_cache/cso_hash.h mesa/gallium/cso_cache/cso_hash.c
#
#
#        mesa/gallium/translate/translate_cache.h mesa/gallium/translate/translate_cache.c
#        mesa/gallium/translate/translate.h mesa/gallium/translate/translate.c
#        mesa/gallium/translate/translate_generic.c mesa/gallium/translate/translate_sse.c
#
#
#        mesa/gallium/targets/dri/dri_screen.h
#
#        mesa/gallium/targets/dri/target.c mesa/gallium/targets/dri/dri_screen.c
#
#
#        mesa/gallium/target-helpers/drm_helper.h mesa/gallium/target-helpers/drm_helper_public.h
#        mesa/gallium/target-helpers/sw_helper.h mesa/gallium/target-helpers/sw_helper_public.h
#        mesa/gallium/target-helpers/inline_debug_helper.h
#
#
#        mesa/gallium/pipe-loader/pipe_loader.h mesa/gallium/pipe-loader/pipe_loader.c
#        mesa/gallium/pipe-loader/pipe_loader_priv.h
#
#
#        mesa/gallium/postprocess/filters.h mesa/gallium/postprocess/postprocess.h
#
#
#        mesa/gallium/driver_noop/noop_public.h mesa/gallium/driver_noop/noop_pipe.c
#        mesa/gallium/driver_noop/noop_state.c
#
#
#        mesa/gallium/driver_ddebug/dd_public.h mesa/gallium/driver_ddebug/dd_pipe.h
#        mesa/gallium/driver_ddebug/dd_util.h mesa/gallium/driver_ddebug/dd_context.c
#        mesa/gallium/driver_ddebug/dd_draw.c mesa/gallium/driver_ddebug/dd_screen.c
#
#
#        mesa/gallium/driver_trace/tr_context.h mesa/gallium/driver_trace/tr_context.c
#        mesa/gallium/driver_trace/tr_screen.h mesa/gallium/driver_trace/tr_screen.c
#        mesa/gallium/driver_trace/tr_dump.h mesa/gallium/driver_trace/tr_dump.c
#        mesa/gallium/driver_trace/tr_texture.h mesa/gallium/driver_trace/tr_texture.c
#        mesa/gallium/driver_trace/tr_public.h mesa/gallium/driver_trace/tr_dump_defines.h
#        mesa/gallium/driver_trace/tr_dump_state.h mesa/gallium/driver_trace/tr_dump_state.c
#
#
#        mesa/gallium/driver_rbug/rbug_public.h mesa/gallium/driver_rbug/rbug_core.c
#        mesa/gallium/driver_rbug/rbug_context.h mesa/gallium/driver_rbug/rbug_context.c
#        mesa/gallium/driver_rbug/rbug_screen.h mesa/gallium/driver_rbug/rbug_screen.c
#        mesa/gallium/driver_rbug/rbug_objects.h mesa/gallium/driver_rbug/rbug_objects.c
#
#
#        mesa/gallium/rbug/rbug.h mesa/gallium/rbug/rbug_context.h mesa/gallium/rbug/rbug_context.c
#        mesa/gallium/rbug/rbug_connection.h mesa/gallium/rbug/rbug_connection.c
#        mesa/gallium/rbug/rbug_core.h mesa/gallium/rbug/rbug_core.c mesa/gallium/rbug/rbug_proto.h
#        mesa/gallium/rbug/rbug_internal.h mesa/gallium/rbug/rbug_demarshal.c
#        mesa/gallium/rbug/rbug_shader.h mesa/gallium/rbug/rbug_shader.c
#        mesa/gallium/rbug/rbug_texture.h mesa/gallium/rbug/rbug_texture.c
#
#
#        mesa/os/os_thread.h mesa/os/os_process.h mesa/os/os_mman.h mesa/os/os_process.c
#
#
#        mesa/state_tracker/st_gl_api.h mesa/state_tracker/st_context.h mesa/state_tracker/st_atom.h
#        mesa/state_tracker/st_atom_list.h mesa/state_tracker/st_program.h
#        mesa/state_tracker/st_texture.h mesa/state_tracker/st_glsl_to_tgsi.h
#        mesa/state_tracker/st_format.h mesa/state_tracker/st_cb_texture.h
#        mesa/state_tracker/st_cb_fbo.h mesa/state_tracker/st_cb_program.h
#        mesa/state_tracker/st_glsl_to_tgsi_temprename.h mesa/state_tracker/st_cb_flush.h
#        mesa/state_tracker/st_glsl_to_tgsi_array_merge.h mesa/state_tracker/st_manager.h
#        mesa/state_tracker/st_glsl_to_tgsi_private.h mesa/state_tracker/st_debug.h
#        mesa/state_tracker/st_util.h mesa/state_tracker/st_glsl_to_ir.h
#        mesa/state_tracker/st_atifs_to_nir.h mesa/state_tracker/st_extensions.h
#        mesa/state_tracker/st_cb_bitmap.h mesa/state_tracker/st_sampler_view.h
#        mesa/state_tracker/st_cb_drawpixels.h mesa/state_tracker/st_draw.h
#        mesa/state_tracker/st_cb_xformfb.h mesa/state_tracker/st_atom_constbuf.h
#        mesa/state_tracker/st_tgsi_lower_yuv.h mesa/state_tracker/st_shader_cache.h
#        mesa/state_tracker/st_nir.h mesa/state_tracker/st_scissor.h mesa/state_tracker/st_pbo.h
#        mesa/state_tracker/st_cb_readpixels.h mesa/state_tracker/st_cb_clear.h
#        mesa/state_tracker/st_cb_drawtex.h mesa/state_tracker/st_cb_eglimage.h
#        mesa/state_tracker/st_cb_feedback.h mesa/state_tracker/st_cb_perfquery.h
#        mesa/state_tracker/st_gen_mipmap.h mesa/state_tracker/st_vdpau.h
#
#        mesa/state_tracker/st_context.c mesa/state_tracker/st_atom.c mesa/state_tracker/st_program.c
#        mesa/state_tracker/st_texture.c mesa/state_tracker/st_glsl_to_tgsi.cpp
#        mesa/state_tracker/st_format.c mesa/state_tracker/st_cb_texture.c
#        mesa/state_tracker/st_cb_fbo.c mesa/state_tracker/st_cb_program.c
#        mesa/state_tracker/st_glsl_to_tgsi_temprename.cpp mesa/state_tracker/st_cb_flush.c
#        mesa/state_tracker/st_glsl_to_tgsi_array_merge.cpp mesa/state_tracker/st_manager.c
#        mesa/state_tracker/st_glsl_to_tgsi_private.cpp mesa/state_tracker/st_debug.c
#        mesa/state_tracker/st_glsl_to_ir.cpp mesa/state_tracker/st_atifs_to_nir.c
#        mesa/state_tracker/st_extensions.c  mesa/state_tracker/st_cb_bitmap_shader.c
#        mesa/state_tracker/st_cb_bitmap.c mesa/state_tracker/st_sampler_view.c
#        mesa/state_tracker/st_cb_drawpixels.c mesa/state_tracker/st_cb_drawpixels_shader.c
#        mesa/state_tracker/st_draw.c mesa/state_tracker/st_draw_feedback.c
#        mesa/state_tracker/st_cb_xformfb.c mesa/state_tracker/st_atom_constbuf.c
#        mesa/state_tracker/st_tgsi_lower_yuv.c mesa/state_tracker/st_shader_cache.c
#        mesa/state_tracker/st_nir_builtins.c mesa/state_tracker/st_nir_lower_builtin.c
#        mesa/state_tracker/st_nir_lower_tex_src_plane.c mesa/state_tracker/st_scissor.c
#        mesa/state_tracker/st_cb_readpixels.c mesa/state_tracker/st_pbo.c
#        mesa/state_tracker/st_pbo_compute.c mesa/state_tracker/st_cb_clear.c
#        mesa/state_tracker/st_cb_drawtex.c mesa/state_tracker/st_cb_eglimage.c
#        mesa/state_tracker/st_cb_feedback.c mesa/state_tracker/st_cb_perfquery.c
#        mesa/state_tracker/st_gen_mipmap.c mesa/state_tracker/st_vdpau.c
#
#
#        mesa/gallium/draw/draw_context.h mesa/gallium/draw/draw_private.h
#        mesa/gallium/draw/draw_pipe.h
#
#
#        mesa/include/drm-uapi/drm_fourcc.h mesa/include/drm-uapi/drm.h


#        virgl/config.h
#        virgl/vrend_iov.h virgl/venus_hw.h virgl/virgl_context.h virgl/virglrenderer_hw.h
#        virgl/virgl_hw.h virgl/virgl_protocol.h virgl/virgl_resource.h virgl/virgl_util.h
#        virgl/vrend_blitter.h virgl/vrend_debug.h virgl/vrend_renderer.h virgl/vrend_shader.h
#        virgl/vrend_strbuf.h virgl/vrend_tweaks.h virgl/vrend_winsys_gbm.h virgl/vrend_winsys.h
#        virgl/vrend_winsys_glx.h virgl/vrend_object.h virgl/virglrenderer.h
#
#        virgl/iov.c virgl/virgl_context.c virgl/virgl_resource.c virgl/virgl_util.c
#        virgl/vrend_blitter.c virgl/vrend_debug.c virgl/vrend_renderer.c virgl/vrend_shader.c
#        virgl/vrend_tweaks.c virgl/vrend_winsys_gbm.c virgl/vrend_winsys.c virgl/vrend_winsys_glx.c
#        virgl/vrend_object.c virgl/virglrenderer.c
#
#
#        virgl/mesa/pipe/p_compiler.h virgl/mesa/pipe/p_config.h
#
#
#        virgl/mesa/util/os_misc.h virgl/mesa/util/detect_os.h virgl/mesa/util/u_endian.h
#        virgl/mesa/util/macros.h virgl/mesa/util/os_file.h virgl/mesa/util/u_debug.h
#        virgl/mesa/util/u_string.h virgl/mesa/util/u_memory.h virgl/mesa/util/u_thread.h
#        virgl/mesa/util/os_memory.h virgl/mesa/util/os_memory_aligned.h
#        virgl/mesa/util/os_memory_stdc.h virgl/mesa/util/xxhash.h virgl/mesa/util/compiler.h
#        virgl/mesa/util/fast_urem_by_const.h virgl/mesa/util/list.h virgl/mesa/util/u_atomic.h
#        virgl/mesa/util/bitscan.h virgl/mesa/util/hash_table.h virgl/mesa/util/ralloc.h
#        virgl/mesa/util/u_cpu_detect.h virgl/mesa/util/u_math.h
#
#        virgl/mesa/util/os_misc.c virgl/mesa/util/os_file.c virgl/mesa/util/u_debug.c
#        virgl/mesa/util/bitscan.c virgl/mesa/util/hash_table.c virgl/mesa/util/ralloc.c
#        virgl/mesa/util/u_cpu_detect.c virgl/mesa/util/u_math.c
#
#
#        virgl/mesa/compat/c11_compat.h virgl/mesa/compat/no_extern_c.h
#        virgl/mesa/compat/c99_compat.h virgl/mesa/compat/c99_math.h
#
#        virgl/mesa/compat/c11/threads.h virgl/mesa/compat/c11/threads_posix.h
#        virgl/mesa/compat/c11/threads_win32.h
#
#
#        virgl/gallium/auxiliary/util/u_hash_table.h virgl/gallium/auxiliary/util/u_pointer.h
#        virgl/gallium/auxiliary/util/u_format.h virgl/gallium/auxiliary/util/u_format_s3tc.h
#        virgl/gallium/auxiliary/util/u_texture.h virgl/gallium/auxiliary/util/u_inlines.h
#        virgl/gallium/auxiliary/util/u_debug_refcnt.h virgl/gallium/auxiliary/util/u_dual_blend.h
#        virgl/gallium/auxiliary/util/u_double_list.h virgl/gallium/auxiliary/util/u_debug_describe.h
#        virgl/gallium/auxiliary/util/u_prim.h virgl/gallium/auxiliary/util/u_half.h
#
#        virgl/gallium/auxiliary/util/u_hash_table.c virgl/gallium/auxiliary/util/u_format.c
#        virgl/gallium/auxiliary/util/u_texture.c virgl/gallium/auxiliary/util/u_debug_describe.c
#
#
#        virgl/gallium/auxiliary/cso_cache/cso_cache.h virgl/gallium/auxiliary/cso_cache/cso_hash.h
#
#        virgl/gallium/auxiliary/cso_cache/cso_cache.c virgl/gallium/auxiliary/cso_cache/cso_hash.c
#
#
#        virgl/gallium/auxiliary/tgsi/tgsi_info.h virgl/gallium/auxiliary/tgsi/tgsi_iterate.h
#        virgl/gallium/auxiliary/tgsi/tgsi_parse.h virgl/gallium/auxiliary/tgsi/tgsi_scan.h
#        virgl/gallium/auxiliary/tgsi/tgsi_util.h virgl/gallium/auxiliary/tgsi/tgsi_build.h
#        virgl/gallium/auxiliary/tgsi/tgsi_dump.h virgl/gallium/auxiliary/tgsi/tgsi_sanity.h
#        virgl/gallium/auxiliary/tgsi/tgsi_strings.h virgl/gallium/auxiliary/tgsi/tgsi_text.h
#
#        virgl/gallium/auxiliary/tgsi/tgsi_info.c virgl/gallium/auxiliary/tgsi/tgsi_iterate.c
#        virgl/gallium/auxiliary/tgsi/tgsi_parse.c virgl/gallium/auxiliary/tgsi/tgsi_scan.c
#        virgl/gallium/auxiliary/tgsi/tgsi_util.c virgl/gallium/auxiliary/tgsi/tgsi_build.c
#        virgl/gallium/auxiliary/tgsi/tgsi_dump.c virgl/gallium/auxiliary/tgsi/tgsi_sanity.c
#        virgl/gallium/auxiliary/tgsi/tgsi_strings.c virgl/gallium/auxiliary/tgsi/tgsi_text.c
#
#
#        virgl/gallium/include/pipe/p_defines.h virgl/gallium/include/pipe/p_shader_tokens.h
#        virgl/gallium/include/pipe/p_format.h virgl/gallium/include/pipe/p_state.h


#        libepoxy/common.h libepoxy/gl.h libepoxy/gl_generated.h
#        libepoxy/dispatch_common.h
#        libepoxy/dispatch_common.c


        gl4es/gl/arbconverter.c gl4es/gl/arbgenerator.c gl4es/gl/arbhelper.c gl4es/gl/arbparser.c
        gl4es/gl/array.c gl4es/gl/blit.c gl4es/gl/blend.c gl4es/gl/buffers.c gl4es/gl/build_info.c
        gl4es/gl/debug.c gl4es/gl/decompress.c gl4es/gl/depth.c gl4es/gl/directstate.c
        gl4es/gl/drawing.c gl4es/gl/enable.c gl4es/gl/envvars.c gl4es/gl/eval.c gl4es/gl/face.c
        gl4es/gl/fog.c gl4es/gl/fpe.c gl4es/gl/fpe_cache.c gl4es/gl/fpe_shader.c
        gl4es/gl/framebuffers.c gl4es/gl/gl_lookup.c gl4es/gl/getter.c gl4es/gl/gl4es.c
        gl4es/gl/glstate.c gl4es/gl/hint.c gl4es/gl/init.c gl4es/gl/light.c gl4es/gl/line.c
        gl4es/gl/list.c gl4es/gl/listdraw.c gl4es/gl/listrl.c gl4es/gl/loader.c gl4es/gl/logs.c
        gl4es/gl/matrix.c gl4es/gl/matvec.c gl4es/gl/oldprogram.c gl4es/gl/pixel.c gl4es/gl/planes.c
        gl4es/gl/pointsprite.c gl4es/gl/preproc.c gl4es/gl/program.c gl4es/gl/queries.c
        gl4es/gl/raster.c gl4es/gl/render.c gl4es/gl/shader.c gl4es/gl/shaderconv.c
        gl4es/gl/shader_hacks.c gl4es/gl/stack.c gl4es/gl/stencil.c gl4es/gl/string_utils.c
        gl4es/gl/stubs.c gl4es/gl/texenv.c gl4es/gl/texgen.c gl4es/gl/texture.c
        gl4es/gl/texture_compressed.c gl4es/gl/texture_params.c gl4es/gl/texture_read.c
        gl4es/gl/texture_3d.c gl4es/gl/uniform.c gl4es/gl/vertexattrib.c gl4es/gl/wrap/gl4eswraps.c
        gl4es/gl/wrap/gles.c gl4es/gl/wrap/glstub.c gl4es/gl/math/matheval.c

        gl4es/version.h gl4es/gl/arbconverter.h gl4es/gl/arbgenerator.h gl4es/gl/arbhelper.h
        gl4es/gl/arbparser.h gl4es/gl/array.h gl4es/gl/blend.h gl4es/gl/blit.h gl4es/gl/buffers.h
        gl4es/gl/build_info.h gl4es/gl/const.h gl4es/gl/debug.h gl4es/gl/decompress.h
        gl4es/gl/depth.h gl4es/gl/directstate.h gl4es/gl/envvars.h gl4es/gl/eval.h gl4es/gl/face.h
        gl4es/gl/fog.h gl4es/gl/fpe.h gl4es/gl/fpe_shader.h gl4es/gl/framebuffers.h
        gl4es/gl/gl_lookup.h gl4es/gl/gles.h gl4es/gl/gl4es.h gl4es/gl/glstate.h gl4es/gl/hint.h
        gl4es/gl/init.h gl4es/gl/light.h gl4es/gl/line.h gl4es/gl/list.h gl4es/gl/loader.h
        gl4es/gl/logs.h gl4es/gl/matrix.h gl4es/gl/matvec.h gl4es/gl/oldprogram.h gl4es/gl/pixel.h
        gl4es/gl/planes.h gl4es/gl/pointsprite.h gl4es/gl/preproc.h gl4es/gl/program.h
        gl4es/gl/queries.h gl4es/gl/raster.h gl4es/gl/render.h gl4es/gl/shader.h
        gl4es/gl/shaderconv.h gl4es/gl/shader_hacks.h gl4es/gl/stack.h gl4es/gl/state.h
        gl4es/gl/stencil.h gl4es/gl/stb_dxt_104.h gl4es/gl/string_utils.h gl4es/gl/texenv.h
        gl4es/gl/texgen.h gl4es/gl/uniform.h gl4es/gl/texture.h gl4es/gl/vertexattrib.h
        gl4es/gl/math/eval.h gl4es/gl/wrap/gl4es.h gl4es/gl/wrap/gles.h gl4es/gl/wrap/stub.h


        gl4es/glx/hardext.c gl4es/glx/gbm.c gl4es/glx/glx.c gl4es/glx/lookup.c gl4es/glx/rpi.c
        gl4es/glx/streaming.c gl4es/glx/utils.c

        gl4es/glx/glx_gbm.h gl4es/glx/gbmfunc.h gl4es/glx/drmfunc.h gl4es/glx/glx.h
        gl4es/glx/hardext.h gl4es/glx/rpi.h gl4es/glx/streaming.h gl4es/glx/utils.h

        gl4es/include/khash.h gl4es/include/gl4eshint.h


        dri/dri_interface.h dri/dri_private.h dri/errors.h
        dri/dri.c dri/dri_backend.c dri/errors.c

        dri/st/st.h
        dri/st/st.c


        cutils/properties.h

        cutils/cutils_stub.cpp


        zstd/zstd.h zstd/zstd_errors.h


        zstd/common/error_private.h zstd/common/zstd_deps.h zstd/common/zstd_internal.h
        zstd/common/compiler.h zstd/common/mem.h zstd/common/debug.h zstd/common/fse.h
        zstd/common/huf.h zstd/common/xxhash.h zstd/common/zstd_trace.h zstd/common/bitstream.h
        zstd/common/cpu.h

        zstd/common/error_private.c zstd/common/zstd_common.c zstd/common/debug.c
        zstd/common/xxhash.c zstd/common/entropy_common.c zstd/common/fse_decompress.c


        zstd/compress/zstd_compress_internal.h zstd/compress/zstd_cwksp.h zstd/compress/hist.h
        zstd/compress/zstd_compress_sequences.h zstd/compress/zstd_compress_literals.h
        zstd/compress/zstd_fast.h zstd/compress/zstd_double_fast.h zstd/compress/zstd_lazy.h
        zstd/compress/zstd_opt.h zstd/compress/zstd_ldm.h zstd/compress/zstd_ldm_geartab.h
        zstd/compress/zstd_compress_superblock.h

        zstd/compress/zstd_compress.c zstd/compress/zstd_compress_sequences.c zstd/compress/hist.c
        zstd/compress/zstd_compress_literals.c zstd/compress/zstd_fast.c zstd/compress/zstd_lazy.c
        zstd/compress/zstd_double_fast.c zstd/compress/zstd_opt.c zstd/compress/zstd_ldm.c
        zstd/compress/zstd_compress_superblock.c zstd/compress/huf_compress.c
        zstd/compress/fse_compress.c


        zstd/decompress/zstd_ddict.h zstd/decompress/zstd_decompress_block.h
        zstd/decompress/zstd_decompress_internal.h

        zstd/decompress/huf_decompress.c zstd/decompress/zstd_ddict.c
        zstd/decompress/zstd_decompress.c zstd/decompress/zstd_decompress_block.c


        xkbcomp/xkbcomp.h xkbcomp/utils.h xkbcomp/xkbpath.h xkbcomp/parseutils.h xkbcomp/misc.h
        xkbcomp/keycodes.h xkbcomp/alias.h xkbcomp/expr.h xkbcomp/vmod.h xkbcomp/action.h
        xkbcomp/indicators.h xkbcomp/compat.h
        xkbcomp/tokens.h

        xkbcomp/xkbcomp.c xkbcomp/utils.c xkbcomp/xkbpath.c xkbcomp/parseutils.c xkbcomp/misc.c
        xkbcomp/keycodes.c xkbcomp/alias.c xkbcomp/expr.c xkbcomp/vmod.c xkbcomp/action.c
        xkbcomp/indicators.c xkbcomp/compat.c
        xkbcomp/xkbscan.c xkbcomp/listing.c xkbcomp/keytypes.c xkbcomp/keymap.c xkbcomp/geometry.c
        xkbcomp/symbols.c xkbcomp/xkbparse.c


        x11/Xmu/Converters.h x11/Xmu/Drawing.h x11/Xmu/CharSet.h x11/Xmu/SysUtil.h x11/Xmu/CurUtil.h
        x11/Xmu/CvtCache.h x11/Xmu/DisplayQue.h x11/Xmu/CloseHook.h x11/Xmu/Misc.h x11/Xmu/Atoms.h
        x11/Xmu/Xmu.h x11/Xmu/Error.h x11/Xmu/StdSel.h x11/Xmu/Editres.h x11/Xmu/EditresP.h
        x11/Xmu/ExtAgent.h

        x11/Xmu/StrToBS.c x11/Xmu/Lower.c x11/Xmu/GrayPixmap.c x11/Xmu/StrToCurs.c
        x11/Xmu/CursorName.c x11/Xmu/LocBitmap.c x11/Xmu/CvtCache.c x11/Xmu/DisplayQue.c
        x11/Xmu/CloseHook.c x11/Xmu/RdBitF.c x11/Xmu/Atoms.c x11/Xmu/CvtStdSel.c
        x11/Xmu/EditresCom.c x11/Xmu/ExtAgent.c x11/Xmu/Clip.c x11/Xmu/StrToOrnt.c
        x11/Xmu/StrToBmap.c x11/Xmu/StrToWidg.c x11/Xmu/ShapeWidg.c x11/Xmu/StrToShap.c
        x11/Xmu/StrToJust.c x11/Xmu/GetHost.c x11/Xmu/DrRndRect.c


        x11/libXt/Intrinsic.h x11/libXt/Core.h x11/libXt/Composite.h x11/libXt/Constraint.h
        x11/libXt/Object.h x11/libXt/RectObj.h x11/libXt/IntrinsicP.h x11/libXt/CoreP.h
        x11/libXt/CompositeP.h x11/libXt/ConstrainP.h x11/libXt/ObjectP.h x11/libXt/RectObjP.h
        x11/libXt/StringDefs.h x11/libXt/Shell.h x11/libXt/IntrinsicI.h x11/libXt/ConvertI.h
        x11/libXt/TranslateI.h x11/libXt/CallbackI.h x11/libXt/EventI.h x11/libXt/PassivGraI.h
        x11/libXt/HookObjI.h x11/libXt/ThreadsI.h x11/libXt/InitialI.h x11/libXt/ResourceI.h
        x11/libXt/ShellP.h x11/libXt/VendorP.h x11/libXt/Vendor.h x11/libXt/VarargsI.h
        x11/libXt/CreateI.h x11/libXt/ResConfigP.h x11/libXt/ShellI.h x11/libXt/SelectionI.h

        x11/libXt/StringDefs.c x11/libXt/Composite.c x11/libXt/Callback.c x11/libXt/Constraint.c
        x11/libXt/Convert.c x11/libXt/Core.c x11/libXt/Event.c x11/libXt/HookObj.c
        x11/libXt/Initialize.c x11/libXt/Intrinsic.c x11/libXt/Object.c x11/libXt/PassivGrab.c
        x11/libXt/RectObj.c x11/libXt/Create.c x11/libXt/ResConfig.c x11/libXt/Resources.c
        x11/libXt/Shell.c x11/libXt/Threads.c x11/libXt/Varargs.c x11/libXt/Vendor.c
        x11/libXt/GetValues.c x11/libXt/SetValues.c x11/libXt/TMaction.c x11/libXt/TMgrab.c
        x11/libXt/TMkey.c x11/libXt/TMparse.c x11/libXt/TMprint.c x11/libXt/TMstate.c
        x11/libXt/GCManager.c x11/libXt/Destroy.c x11/libXt/ClickTime.c x11/libXt/Display.c
        x11/libXt/Geometry.c x11/libXt/Alloc.c x11/libXt/Hooks.c x11/libXt/Error.c
        x11/libXt/NextEvent.c x11/libXt/Popup.c x11/libXt/Selection.c x11/libXt/Manage.c
        x11/libXt/GetResList.c x11/libXt/VarCreate.c x11/libXt/EventUtil.c x11/libXt/Keyboard.c
        x11/libXt/Converters.c x11/libXt/Pointer.c x11/libXt/SetSens.c x11/libXt/ArgList.c
        x11/libXt/sharedlib.c


        x11/Xaw/XawInit.h x11/Xaw/Simple.h x11/Xaw/SimpleP.h x11/Xaw/Cardinals.h x11/Xaw/Private.h
        x11/Xaw/Tip.h x11/Xaw/TipP.h x11/Xaw/Form.h x11/Xaw/FormP.h x11/Xaw/Label.h x11/Xaw/LabelP.h
        x11/Xaw/Command.h x11/Xaw/CommandP.h x11/Xaw/Toggle.h x11/Xaw/ToggleP.h x11/Xaw/XawImP.h
        x11/Xaw/Text.h x11/Xaw/TextP.h x11/Xaw/MultiSrc.h x11/Xaw/MultiSrcP.h x11/Xaw/TextSrc.h
        x11/Xaw/TextSrcP.h x11/Xaw/XawI18n.h x11/Xaw/TextSink.h x11/Xaw/TextSinkP.h
        x11/Xaw/MultiSink.h x11/Xaw/MultiSinkP.h x11/Xaw/VendorEP.h x11/Xaw/Scrollbar.h
        x11/Xaw/ScrollbarP.h x11/Xaw/AsciiSrc.h x11/Xaw/AsciiSrcP.h x11/Xaw/AsciiSink.h
        x11/Xaw/AsciiSinkP.h x11/Xaw/AsciiText.h x11/Xaw/AsciiTextP.h

        x11/Xaw/Simple.c x11/Xaw/Tip.c x11/Xaw/XawInit.c x11/Xaw/Converters.c x11/Xaw/Pixmap.c
        x11/Xaw/Actions.c x11/Xaw/DisplayList.c x11/Xaw/Form.c x11/Xaw/Label.c x11/Xaw/Command.c
        x11/Xaw/Toggle.c x11/Xaw/XawIm.c x11/Xaw/Text.c x11/Xaw/MultiSrc.c x11/Xaw/TextSrc.c
        x11/Xaw/XawI18n.c x11/Xaw/TextSink.c x11/Xaw/MultiSink.c x11/Xaw/Vendor.c
        x11/Xaw/Scrollbar.c x11/Xaw/AsciiSrc.c x11/Xaw/AsciiSink.c x11/Xaw/OS.c x11/Xaw/TextTr.c
        x11/Xaw/TextAction.c x11/Xaw/TextPop.c x11/Xaw/AsciiText.c


        x11/SM/SM.h x11/SM/SMlib.h x11/SM/SMproto.h x11/SM/SMlibint.h

        x11/SM/sm_client.c x11/SM/sm_error.c x11/SM/sm_genid.c x11/SM/sm_manager.c x11/SM/sm_misc.c
        x11/SM/sm_process.c


        x11/ICE/ICE.h x11/ICE/ICElib.h x11/ICE/ICEproto.h x11/ICE/ICEmsg.h x11/ICE/ICEconn.h
        x11/ICE/ICEutil.h x11/ICE/ICElibint.h x11/ICE/globals.h

        x11/ICE/accept.c x11/ICE/authutil.c x11/ICE/connect.c x11/ICE/error.c x11/ICE/getauth.c
        x11/ICE/iceauth.c x11/ICE/icetrans.c x11/ICE/listen.c x11/ICE/listenwk.c x11/ICE/locking.c
        x11/ICE/misc.c x11/ICE/ping.c x11/ICE/process.c x11/ICE/protosetup.c x11/ICE/register.c
        x11/ICE/replywait.c x11/ICE/setauth.c x11/ICE/shutdown.c x11/ICE/watch.c


        x11/libxpm/xpm.h x11/libxpm/XpmI.h

        x11/libxpm/RdFToP.c x11/libxpm/CrPFrI.c x11/libxpm/RdFToI.c x11/libxpm/parse.c
        x11/libxpm/Info.c x11/libxpm/Image.c x11/libxpm/Attrib.c x11/libxpm/create.c
        x11/libxpm/hashtab.c x11/libxpm/data.c x11/libxpm/misc.c


        x11/libxfixes/Xfixesint.h
        x11/libxfixes/Cursor.c x11/libxfixes/Disconnect.c x11/libxfixes/Region.c
        x11/libxfixes/SaveSet.c x11/libxfixes/Selection.c x11/libxfixes/Xfixes.c


        x11/libxi/XIint.h
        x11/libxi/XAllowDv.c x11/libxi/XChDProp.c x11/libxi/XChgDCtl.c x11/libxi/XChgFCtl.c
        x11/libxi/XChgKMap.c x11/libxi/XChgKbd.c x11/libxi/XChgPnt.c x11/libxi/XChgProp.c
        x11/libxi/XCloseDev.c x11/libxi/XDelDProp.c x11/libxi/XDevBell.c x11/libxi/XExtInt.c
        x11/libxi/XExtToWire.c x11/libxi/XGMotion.c x11/libxi/XGetBMap.c
        x11/libxi/XGetCPtr.c x11/libxi/XGetDCtl.c x11/libxi/XGetDProp.c x11/libxi/XGetFCtl.c
        x11/libxi/XGetFCtl.c x11/libxi/XGetKMap.c x11/libxi/XGetMMap.c x11/libxi/XGetProp.c
        x11/libxi/XGetVers.c x11/libxi/XGrDvBut.c x11/libxi/XGrDvKey.c x11/libxi/XGrabDev.c
        x11/libxi/XGtFocus.c x11/libxi/XGtSelect.c x11/libxi/XIAllowEvents.c x11/libxi/XIBarrier.c
        x11/libxi/XIDefineCursor.c x11/libxi/XIGetDevFocus.c x11/libxi/XIGrabDevice.c
        x11/libxi/XIHierarchy.c x11/libxi/XIPassiveGrab.c x11/libxi/XIProperties.c
        x11/libxi/XIQueryDevice.c x11/libxi/XIQueryPointer.c x11/libxi/XIQueryVersion.c
        x11/libxi/XISelEv.c x11/libxi/XISetCPtr.c x11/libxi/XISetDevFocus.c
        x11/libxi/XIWarpPointer.c x11/libxi/XListDProp.c x11/libxi/XListDev.c x11/libxi/XOpenDev.c
        x11/libxi/XQueryDv.c x11/libxi/XSelect.c x11/libxi/XSetBMap.c x11/libxi/XSetDVal.c
        x11/libxi/XSetMMap.c x11/libxi/XSetMode.c x11/libxi/XSndExEv.c x11/libxi/XStFocus.c
        x11/libxi/XUngrDev.c x11/libxi/XUngrDvB.c x11/libxi/XUngrDvK.c


        x11/libXtst/XTest.c x11/libXtst/XRecord.c

        x11/libxcomposite/xcompositeint.h x11/libxcomposite/Xcomposite.c


        x11/libxrender/Xrenderint.h

        x11/libxrender/AddTrap.c x11/libxrender/Color.c x11/libxrender/Composite.c
        x11/libxrender/Cursor.c x11/libxrender/FillRect.c x11/libxrender/FillRects.c
        x11/libxrender/Filter.c x11/libxrender/Glyph.c x11/libxrender/Picture.c
        x11/libxrender/Poly.c x11/libxrender/Trap.c x11/libxrender/Tri.c x11/libxrender/Xrender.c



        xclock/Clock.h xclock/ClockP.h
        xclock/Clock.c xclock/xclock.c
        xclock/clock.bit xclock/clmask.bit

        xeyes/Eyes.h xeyes/EyesP.h xeyes/transform.h
        xeyes/Eyes.c xeyes/transform.c xeyes/xeyes.c
        xeyes/eyes.bit xeyes/eyesmask.bit

        xcalc/xcalc.h
        xcalc/xcalc.c xcalc/math.c xcalc/actions.c



        main_wm.cpp main_wm.h main_wm_c.c main_wm_c.h
        window_manager.cpp window_manager.h util.cpp util.h)
find_library(log-lib log)
target_link_libraries(xserver
        -ljnigraphics
        -landroid
        ${log-lib})