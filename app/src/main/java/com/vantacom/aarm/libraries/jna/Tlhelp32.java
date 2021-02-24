package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public interface Tlhelp32 {
    WinDef.DWORD TH32CS_SNAPHEAPLIST = new WinDef.DWORD(0x00000001);
    WinDef.DWORD TH32CS_SNAPPROCESS  = new WinDef.DWORD(0x00000002);
    WinDef.DWORD TH32CS_SNAPTHREAD   = new WinDef.DWORD(0x00000004);
    WinDef.DWORD TH32CS_SNAPMODULE   = new WinDef.DWORD(0x00000008);
    WinDef.DWORD TH32CS_SNAPMODULE32 = new WinDef.DWORD(0x00000010);
    WinDef.DWORD TH32CS_SNAPALL      = new WinDef.DWORD((TH32CS_SNAPHEAPLIST.intValue() |
            TH32CS_SNAPPROCESS.intValue() | TH32CS_SNAPTHREAD.intValue() | TH32CS_SNAPMODULE.intValue()));

    WinDef.DWORD TH32CS_INHERIT      = new WinDef.DWORD(0x80000000);

    int MAX_MODULE_NAME32 = 255;

    @Structure.FieldOrder({"dwSize", "cntUsage", "th32ProcessID", "th32DefaultHeapID",
            "th32ModuleID", "cntThreads", "th32ParentProcessID", "pcPriClassBase",
            "dwFlags", "szExeFile"})
    public static class PROCESSENTRY32 extends Structure {
        public static class ByReference extends PROCESSENTRY32 implements Structure.ByReference {
            public ByReference() {
            }

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public WinDef.DWORD dwSize;
        public WinDef.DWORD cntUsage;
        public WinDef.DWORD th32ProcessID;
        public BaseTSD.ULONG_PTR th32DefaultHeapID;
        public WinDef.DWORD th32ModuleID;
        public WinDef.DWORD cntThreads;
        public WinDef.DWORD th32ParentProcessID;
        public WinDef.LONG pcPriClassBase;
        public WinDef.DWORD dwFlags;
        public char[] szExeFile = new char[WinDef.MAX_PATH];

        public PROCESSENTRY32() {
            dwSize = new WinDef.DWORD(size());
        }

        public PROCESSENTRY32(Pointer memory) {
            super(memory);
            read();
        }
    }

    @Structure.FieldOrder({ "dwSize", "cntUsage", "th32ThreadID", "th32OwnerProcessID", "tpBasePri", "tpDeltaPri", "dwFlags" })
    public static class THREADENTRY32 extends Structure {
        public static class ByReference extends THREADENTRY32 implements Structure.ByReference {
            public ByReference() {
            }

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public int dwSize;
        public int cntUsage;
        public int th32ThreadID;
        public int th32OwnerProcessID;
        public NativeLong tpBasePri;
        public NativeLong tpDeltaPri;
        public int dwFlags;

        public THREADENTRY32() {
            dwSize = size();
        }

        public THREADENTRY32(Pointer memory) {
            super(memory);
            read();
        }
    }

    @Structure.FieldOrder({"dwSize", "th32ModuleID", "th32ProcessID", "GlblcntUsage",
            "ProccntUsage", "modBaseAddr", "modBaseSize", "hModule",
            "szModule", "szExePath"})
    public class MODULEENTRY32W extends Structure {
        public static class ByReference extends MODULEENTRY32W implements Structure.ByReference {
            public ByReference() {
            }

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public WinDef.DWORD dwSize;
        public WinDef.DWORD th32ModuleID;
        public WinDef.DWORD th32ProcessID;
        public WinDef.DWORD GlblcntUsage;
        public WinDef.DWORD ProccntUsage;
        public Pointer modBaseAddr;
        public WinDef.DWORD modBaseSize;
        public WinDef.HMODULE hModule;
        public char[] szModule = new char[MAX_MODULE_NAME32 + 1];
        public char[] szExePath = new char[Kernel32.MAX_PATH];

        public MODULEENTRY32W() {
            dwSize = new WinDef.DWORD(size());
        }

        public MODULEENTRY32W(Pointer memory) {
            super(memory);
            read();
        }

        public String szModule() {
            return Native.toString(this.szModule);
        }

        public String szExePath() {
            return Native.toString(this.szExePath);
        }
    }
}