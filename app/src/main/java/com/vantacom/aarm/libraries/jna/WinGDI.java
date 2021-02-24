package com.vantacom.aarm.libraries.jna;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.Structure.FieldOrder;
import com.vantacom.aarm.libraries.jna.WinDef.HBITMAP;
import com.vantacom.aarm.libraries.jna.WinDef.RECT;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;

public interface WinGDI {
    int RDH_RECTANGLES = 1;

    @FieldOrder({"dwSize", "iType", "nCount", "nRgnSize", "rcBound"})
    class RGNDATAHEADER extends Structure {
        public int dwSize = size();
        public int iType = RDH_RECTANGLES;
        public int nCount;
        public int nRgnSize;
        public RECT rcBound;
    }

    @FieldOrder({"rdh", "Buffer"})
    class RGNDATA extends Structure {
        public RGNDATAHEADER rdh;
        public byte[] Buffer;

        public RGNDATA() {
            this(1);
        }
        public RGNDATA(int bufferSize) {
            Buffer = new byte[bufferSize];
            allocateMemory();
        }
    }

    HANDLE HGDI_ERROR = new HANDLE(Pointer.createConstant(0xFFFFFFFF));

    int RGN_AND = 1;
    int RGN_OR = 2;
    int RGN_XOR = 3;
    int RGN_DIFF = 4;
    int RGN_COPY = 5;

    int ERROR = 0;
    int NULLREGION = 1;
    int SIMPLEREGION = 2;
    int COMPLEXREGION = 3;

    int ALTERNATE = 1;
    int WINDING = 2;

    int BI_RGB = 0;
    int BI_RLE8 = 1;
    int BI_RLE4 = 2;
    int BI_BITFIELDS = 3;
    int BI_JPEG = 4;
    int BI_PNG = 5;

    int PFD_TYPE_RGBA = 0;
    int PFD_TYPE_COLORINDEX = 1;

    int PFD_MAIN_PLANE = 0;
    int PFD_OVERLAY_PLANE = 1;
    int PFD_UNDERLAY_PLANE = (-1);

    int PFD_DOUBLEBUFFER = 0x00000001;
    int PFD_STEREO = 0x00000002;
    int PFD_DRAW_TO_WINDOW = 0x00000004;
    int PFD_DRAW_TO_BITMAP = 0x00000008;
    int PFD_SUPPORT_GDI = 0x00000010;
    int PFD_SUPPORT_OPENGL = 0x00000020;
    int PFD_GENERIC_FORMAT = 0x00000040;
    int PFD_NEED_PALETTE = 0x00000080;
    int PFD_NEED_SYSTEM_PALETTE = 0x00000100;
    int PFD_SWAP_EXCHANGE = 0x00000200;
    int PFD_SWAP_COPY = 0x00000400;
    int PFD_SWAP_LAYER_BUFFERS = 0x00000800;
    int PFD_GENERIC_ACCELERATED = 0x00001000;
    int PFD_SUPPORT_DIRECTDRAW = 0x00002000;

    @FieldOrder({"biSize", "biWidth", "biHeight", "biPlanes", "biBitCount",
            "biCompression", "biSizeImage", "biXPelsPerMeter", "biYPelsPerMeter",
            "biClrUsed", "biClrImportant"})
    class BITMAPINFOHEADER extends Structure {
        public int biSize = size();
        public int biWidth;
        public int biHeight;
        public short biPlanes;
        public short biBitCount;
        public int biCompression;
        public int biSizeImage;
        public int biXPelsPerMeter;
        public int biYPelsPerMeter;
        public int biClrUsed;
        public int biClrImportant;
    }

    @FieldOrder({"rgbBlue", "rgbGreen", "rgbRed", "rgbReserved"})
    class RGBQUAD extends Structure {
        public byte rgbBlue;
        public byte rgbGreen;
        public byte rgbRed;
        public byte rgbReserved = 0;
    }

    @FieldOrder({"bmiHeader", "bmiColors"})
    class BITMAPINFO extends Structure {

        public BITMAPINFOHEADER bmiHeader = new BITMAPINFOHEADER();
        public RGBQUAD[] bmiColors = new RGBQUAD[1];
        public BITMAPINFO() {
            this(1);
        }
        public BITMAPINFO(int size) {
            bmiColors = new RGBQUAD[size];
        }
    }

    @FieldOrder({"fIcon", "xHotspot", "yHotspot", "hbmMask", "hbmColor"})
    class ICONINFO extends Structure {
        public boolean fIcon;
        public int xHotspot;
        public int yHotspot;
        public HBITMAP hbmMask;
        public HBITMAP hbmColor;
    }

    @FieldOrder({"bmType", "bmWidth", "bmHeight", "bmWidthBytes", "bmPlanes", "bmBitsPixel", "bmBits"})
    class BITMAP extends Structure {
        public NativeLong bmType;
        public NativeLong bmWidth;
        public NativeLong bmHeight;
        public NativeLong bmWidthBytes;
        public short bmPlanes;
        public short bmBitsPixel;
        public Pointer bmBits;
    }

    @FieldOrder({"dsBm", "dsBmih", "dsBitfields", "dshSection", "dsOffset"})
    class DIBSECTION extends Structure {
        public BITMAP           dsBm;
        public BITMAPINFOHEADER dsBmih;
        public int[]            dsBitfields = new int[3];
        public HANDLE           dshSection;
        public int              dsOffset;
    }

    int DIB_RGB_COLORS = 0;
    int DIB_PAL_COLORS = 1;

    @FieldOrder({"nSize", "nVersion", "dwFlags", "iPixelType",
            "cColorBits", "cRedBits", "cRedShift", "cGreenBits", "cGreenShift", "cBlueBits", "cBlueShift", "cAlphaBits", "cAlphaShift",
            "cAccumBits", "cAccumRedBits", "cAccumGreenBits", "cAccumBlueBits", "cAccumAlphaBits",
            "cDepthBits", "cStencilBits", "cAuxBuffers", "iLayerType", "bReserved", "dwLayerMask", "dwVisibleMask", "dwDamageMask"})
    class PIXELFORMATDESCRIPTOR extends Structure {
        public PIXELFORMATDESCRIPTOR() {
            nSize = (short) size();
        }

        public PIXELFORMATDESCRIPTOR(Pointer memory) {
            super(memory);
            read();
        }

        public static class ByReference extends PIXELFORMATDESCRIPTOR implements Structure.ByReference {
        }

        public short  nSize;
        public short  nVersion;
        public int dwFlags;
        public byte  iPixelType;
        public byte  cColorBits;
        public byte  cRedBits;
        public byte  cRedShift;
        public byte  cGreenBits;
        public byte  cGreenShift;
        public byte  cBlueBits;
        public byte  cBlueShift;
        public byte  cAlphaBits;
        public byte  cAlphaShift;
        public byte  cAccumBits;
        public byte  cAccumRedBits;
        public byte  cAccumGreenBits;
        public byte  cAccumBlueBits;
        public byte  cAccumAlphaBits;
        public byte  cDepthBits;
        public byte  cStencilBits;
        public byte  cAuxBuffers;
        public byte  iLayerType;
        public byte  bReserved;
        public int dwLayerMask;
        public int dwVisibleMask;
        public int dwDamageMask;
    }
}