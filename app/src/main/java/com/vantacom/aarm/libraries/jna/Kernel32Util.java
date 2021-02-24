package com.vantacom.aarm.libraries.jna;

import java.io.File;
import java.io.FileNotFoundException;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import com.sun.jna.LastErrorException;
import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.win32.W32APITypeMapper;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLEByReference;
import com.vantacom.aarm.libraries.jna.WinNT.HRESULT;
import com.vantacom.aarm.libraries.jna.WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX;

public abstract class Kernel32Util implements WinDef {
    public static String getComputerName() {
        char buffer[] = new char[WinBase.MAX_COMPUTERNAME_LENGTH + 1];
        IntByReference lpnSize = new IntByReference(buffer.length);
        if (!Kernel32.INSTANCE.GetComputerName(buffer, lpnSize)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return Native.toString(buffer);
    }

    public static void freeLocalMemory(Pointer ptr) {
        Pointer res = Kernel32.INSTANCE.LocalFree(ptr);
        if (res != null) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static void freeGlobalMemory(Pointer ptr) {
        Pointer res = Kernel32.INSTANCE.GlobalFree(ptr);
        if (res != null) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static void closeHandleRefs(HANDLEByReference... refs) {
        Win32Exception err = null;
        for (HANDLEByReference r : refs) {
            try {
                closeHandleRef(r);
            } catch(Win32Exception e) {
                if (err == null) {
                    err = e;
                } else {
                    err.addSuppressedReflected(e);
                }
            }
        }

        if (err != null) {
            throw err;
        }
    }

    public static void closeHandleRef(HANDLEByReference ref) {
        closeHandle((ref == null) ? null : ref.getValue());
    }

    public static void closeHandles(HANDLE... handles) {
        Win32Exception err = null;
        for (HANDLE h : handles) {
            try {
                closeHandle(h);
            } catch(Win32Exception e) {
                if (err == null) {
                    err = e;
                } else {
                    err.addSuppressedReflected(e);
                }
            }
        }

        if (err != null) {
            throw err;
        }
    }

    public static void closeHandle(HANDLE h) {
        if (h == null) {
            return;
        }

        if (!Kernel32.INSTANCE.CloseHandle(h)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static String formatMessage(int code) {
        PointerByReference buffer = new PointerByReference();
        int nLen = Kernel32.INSTANCE.FormatMessage(
                WinBase.FORMAT_MESSAGE_ALLOCATE_BUFFER
                        | WinBase.FORMAT_MESSAGE_FROM_SYSTEM
                        | WinBase.FORMAT_MESSAGE_IGNORE_INSERTS,
                null,
                code,
                0,
                buffer, 0, null);
        if (nLen == 0) {
            throw new LastErrorException(Native.getLastError());
        }

        Pointer ptr = buffer.getValue();
        try {
            String s = ptr.getWideString(0);
            return s.trim();
        } finally {
            freeLocalMemory(ptr);
        }
    }

    public static String formatMessage(HRESULT code) {
        return formatMessage(code.intValue());
    }

    public static String formatMessageFromLastErrorCode(int code) {
        return formatMessage(W32Errors.HRESULT_FROM_WIN32(code));
    }

    public static String getLastErrorMessage() {
        return Kernel32Util.formatMessageFromLastErrorCode(Kernel32.INSTANCE
                .GetLastError());
    }

    public static String getTempPath() {
        DWORD nBufferLength = new DWORD(WinDef.MAX_PATH);
        char[] buffer = new char[nBufferLength.intValue()];
        if (Kernel32.INSTANCE.GetTempPath(nBufferLength, buffer).intValue() == 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return Native.toString(buffer);
    }

    public static void deleteFile(String filename) {
        if (!Kernel32.INSTANCE.DeleteFile(filename)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static List<String> getLogicalDriveStrings() {
        DWORD dwSize = Kernel32.INSTANCE.GetLogicalDriveStrings(new DWORD(0), null);
        if (dwSize.intValue() <= 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        char buf[] = new char[dwSize.intValue()];
        dwSize = Kernel32.INSTANCE.GetLogicalDriveStrings(dwSize, buf);
        int bufSize = dwSize.intValue();
        if (bufSize <= 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        return Native.toStringList(buf, 0, bufSize);
    }

    public static int getFileAttributes(String fileName) {
        int fileAttributes = Kernel32.INSTANCE.GetFileAttributes(fileName);
        if (fileAttributes == WinBase.INVALID_FILE_ATTRIBUTES) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return fileAttributes;
    }

    public static int getFileType(String fileName) throws FileNotFoundException {
        File f = new File(fileName);
        if (!f.exists()) {
            throw new FileNotFoundException(fileName);
        }

        HANDLE hFile = null;
        Win32Exception err = null;
        try {
            hFile = Kernel32.INSTANCE.CreateFile(fileName, WinNT.GENERIC_READ,
                    WinNT.FILE_SHARE_READ, new WinBase.SECURITY_ATTRIBUTES(),
                    WinNT.OPEN_EXISTING, WinNT.FILE_ATTRIBUTE_NORMAL,
                    new HANDLEByReference().getValue());

            if (WinBase.INVALID_HANDLE_VALUE.equals(hFile)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }

            int type = Kernel32.INSTANCE.GetFileType(hFile);
            switch (type) {
                case WinNT.FILE_TYPE_UNKNOWN:
                    int rc = Kernel32.INSTANCE.GetLastError();
                    switch (rc) {
                        case WinError.NO_ERROR:
                            break;
                        default:
                            throw new Win32Exception(rc);
                    }
                default:
                    return type;
            }
        } catch(Win32Exception e) {
            err = e;
            throw err;
        } finally {
            try {
                closeHandle(hFile);
            } catch(Win32Exception e) {
                if (err == null) {
                    err = e;
                } else {
                    err.addSuppressedReflected(e);
                }
            }

            if (err != null) {
                throw err;
            }
        }
    }

    public static int getDriveType(String rootName) {
        return Kernel32.INSTANCE.GetDriveType(rootName);
    }

    public static String getEnvironmentVariable(String name) {
        int size = Kernel32.INSTANCE.GetEnvironmentVariable(name, null, 0);
        if (size == 0) {
            return null;
        } else if (size < 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        char[] buffer = new char[size];
        size = Kernel32.INSTANCE.GetEnvironmentVariable(name, buffer,
                buffer.length);
        if (size <= 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return Native.toString(buffer);
    }

    public static Map<String,String> getEnvironmentVariables() {
        Pointer lpszEnvironmentBlock=Kernel32.INSTANCE.GetEnvironmentStrings();
        if (lpszEnvironmentBlock == null) {
            throw new LastErrorException(Kernel32.INSTANCE.GetLastError());
        }

        try {
            return getEnvironmentVariables(lpszEnvironmentBlock, 0L);
        } finally {
            if (!Kernel32.INSTANCE.FreeEnvironmentStrings(lpszEnvironmentBlock)) {
                throw new LastErrorException(Kernel32.INSTANCE.GetLastError());
            }
        }
    }

    public static Map<String,String> getEnvironmentVariables(Pointer lpszEnvironmentBlock, long offset) {
        if (lpszEnvironmentBlock == null) {
            return null;
        }

        Map<String,String>  vars=new TreeMap<String,String>();
        boolean             asWideChars=isWideCharEnvironmentStringBlock(lpszEnvironmentBlock, offset);
        long                stepFactor=asWideChars ? 2L : 1L;
        for (long    curOffset=offset; ; ) {
            String  nvp=readEnvironmentStringBlockEntry(lpszEnvironmentBlock, curOffset, asWideChars);
            int     len=nvp.length();
            if (len == 0) {
                break;
            }

            int pos=nvp.indexOf('=');
            if (pos < 0) {
                throw new IllegalArgumentException("Missing variable value separator in " + nvp);
            }

            String  name=nvp.substring(0, pos), value=nvp.substring(pos + 1);
            vars.put(name, value);

            curOffset += (len + 1) * stepFactor;
        }

        return vars;
    }

    public static String readEnvironmentStringBlockEntry(Pointer lpszEnvironmentBlock, long offset, boolean asWideChars) {
        long endOffset=findEnvironmentStringBlockEntryEnd(lpszEnvironmentBlock, offset, asWideChars);
        int  dataLen=(int) (endOffset - offset);
        if (dataLen == 0) {
            return "";
        }

        int         charsLen=asWideChars ? (dataLen / 2) : dataLen;
        char[]      chars=new char[charsLen];
        long        curOffset=offset, stepSize=asWideChars ? 2L : 1L;
        ByteOrder   byteOrder=ByteOrder.nativeOrder();
        for (int index=0; index < chars.length; index++, curOffset += stepSize) {
            byte b=lpszEnvironmentBlock.getByte(curOffset);
            if (asWideChars) {
                byte x=lpszEnvironmentBlock.getByte(curOffset + 1L);
                if (ByteOrder.LITTLE_ENDIAN.equals(byteOrder)) {
                    chars[index] = (char) (((x << Byte.SIZE) & 0xFF00) | (b & 0x00FF));
                } else {    // unlikely, but handle it
                    chars[index] = (char) (((b << Byte.SIZE) & 0xFF00) | (x & 0x00FF));
                }
            } else {
                chars[index] = (char) (b & 0x00FF);
            }
        }

        return new String(chars);
    }

    public static long findEnvironmentStringBlockEntryEnd(Pointer lpszEnvironmentBlock, long offset, boolean asWideChars) {
        for (long curOffset=offset, stepSize=asWideChars ? 2L : 1L; ; curOffset += stepSize) {
            byte b=lpszEnvironmentBlock.getByte(curOffset);
            if (b == 0) {
                return curOffset;
            }
        }
    }

    public static boolean isWideCharEnvironmentStringBlock(Pointer lpszEnvironmentBlock, long offset) {
        byte        b0=lpszEnvironmentBlock.getByte(offset);
        byte        b1=lpszEnvironmentBlock.getByte(offset + 1L);
        ByteOrder   byteOrder=ByteOrder.nativeOrder();
        if (ByteOrder.LITTLE_ENDIAN.equals(byteOrder)) {
            return isWideCharEnvironmentStringBlock(b1);
        } else {
            return isWideCharEnvironmentStringBlock(b0);
        }
    }

    private static boolean isWideCharEnvironmentStringBlock(byte charsetIndicator) {
        if (charsetIndicator != 0) {
            return false;
        } else {
            return true;
        }
    }

    public static final int getPrivateProfileInt(final String appName,
                                                 final String keyName, final int defaultValue, final String fileName) {
        return Kernel32.INSTANCE.GetPrivateProfileInt(appName, keyName,
                defaultValue, fileName);
    }

    public static final String getPrivateProfileString(final String lpAppName,
                                                       final String lpKeyName, final String lpDefault,
                                                       final String lpFileName) {
        final char buffer[] = new char[1024];
        Kernel32.INSTANCE.GetPrivateProfileString(lpAppName, lpKeyName,
                lpDefault, buffer, new DWORD(buffer.length), lpFileName);
        return Native.toString(buffer);
    }

    public static final void writePrivateProfileString(final String appName,
                                                       final String keyName, final String string, final String fileName) {
        if (!Kernel32.INSTANCE.WritePrivateProfileString(appName, keyName,
                string, fileName))
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
    }

    public static final WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION[] getLogicalProcessorInformation() {
        int sizePerStruct = new WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION()
                .size();
        WinDef.DWORDByReference bufferSize = new WinDef.DWORDByReference(
                new WinDef.DWORD(sizePerStruct));
        Memory memory;
        while (true) {
            memory = new Memory(bufferSize.getValue().intValue());
            if (!Kernel32.INSTANCE.GetLogicalProcessorInformation(memory,
                    bufferSize)) {
                int err = Kernel32.INSTANCE.GetLastError();
                if (err != WinError.ERROR_INSUFFICIENT_BUFFER)
                    throw new Win32Exception(err);
            } else {
                break;
            }
        }
        WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION firstInformation = new WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION(
                memory);
        int returnedStructCount = bufferSize.getValue().intValue()
                / sizePerStruct;
        return (WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION[]) firstInformation
                .toArray(new WinNT.SYSTEM_LOGICAL_PROCESSOR_INFORMATION[returnedStructCount]);
    }

    public static final SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX[] getLogicalProcessorInformationEx(
            int relationshipType) {
        WinDef.DWORDByReference bufferSize = new WinDef.DWORDByReference(new WinDef.DWORD(1));
        Memory memory;
        while (true) {
            memory = new Memory(bufferSize.getValue().intValue());
            if (!Kernel32.INSTANCE.GetLogicalProcessorInformationEx(relationshipType, memory, bufferSize)) {
                int err = Kernel32.INSTANCE.GetLastError();
                if (err != WinError.ERROR_INSUFFICIENT_BUFFER)
                    throw new Win32Exception(err);
            } else {
                break;
            }
        }
        List<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX> procInfoList = new ArrayList<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>();
        int offset = 0;
        while (offset < bufferSize.getValue().intValue()) {
            SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX information = SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX
                    .fromPointer(memory.share(offset));
            procInfoList.add(information);
            offset += information.size;
        }
        return procInfoList.toArray(new SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX[0]);
    }

    public static final String[] getPrivateProfileSection(final String appName, final String fileName) {
        final char buffer[] = new char[32768]; // Maximum section size according to MSDN (http://msdn.microsoft.com/en-us/library/windows/desktop/ms724348(v=vs.85).aspx)
        if (Kernel32.INSTANCE.GetPrivateProfileSection(appName, buffer, new DWORD(buffer.length), fileName).intValue() == 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return new String(buffer).split("\0");
    }

    public static final String[] getPrivateProfileSectionNames(final String fileName) {
        final char buffer[] = new char[65536]; // Maximum INI file size according to MSDN (http://support.microsoft.com/kb/78346)
        if (Kernel32.INSTANCE.GetPrivateProfileSectionNames(buffer, new DWORD(buffer.length), fileName).intValue() == 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return new String(buffer).split("\0");
    }

    public static final void writePrivateProfileSection(final String appName, final String[] strings, final String fileName) {
        final StringBuilder buffer = new StringBuilder();
        for (final String string : strings)
            buffer.append(string).append('\0');
        buffer.append('\0');
        if (! Kernel32.INSTANCE.WritePrivateProfileSection(appName, buffer.toString(), fileName)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static final List<String> queryDosDevice(String lpszDeviceName, int maxTargetSize) {
        char[] lpTargetPath = new char[maxTargetSize];
        int dwSize = Kernel32.INSTANCE.QueryDosDevice(lpszDeviceName, lpTargetPath, lpTargetPath.length);
        if (dwSize == 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        return Native.toStringList(lpTargetPath, 0, dwSize);
    }

    public static final List<String> getVolumePathNamesForVolumeName(String lpszVolumeName) {
        char[] lpszVolumePathNames = new char[WinDef.MAX_PATH + 1];
        IntByReference lpcchReturnLength = new IntByReference();

        if (!Kernel32.INSTANCE.GetVolumePathNamesForVolumeName(lpszVolumeName, lpszVolumePathNames, lpszVolumePathNames.length, lpcchReturnLength)) {
            int hr = Kernel32.INSTANCE.GetLastError();
            if (hr != WinError.ERROR_MORE_DATA) {
                throw new Win32Exception(hr);
            }
            int required = lpcchReturnLength.getValue();
            lpszVolumePathNames = new char[required];
            if (!Kernel32.INSTANCE.GetVolumePathNamesForVolumeName(lpszVolumeName, lpszVolumePathNames, lpszVolumePathNames.length, lpcchReturnLength)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }
        }

        int bufSize = lpcchReturnLength.getValue();
        return Native.toStringList(lpszVolumePathNames, 0, bufSize);
    }

    public static final String VOLUME_GUID_PATH_PREFIX = "\\\\?\\Volume{";
    public static final String VOLUME_GUID_PATH_SUFFIX = "}\\";

    public static final String extractVolumeGUID(String volumeGUIDPath) {
        if ((volumeGUIDPath == null)
                || (volumeGUIDPath.length() <= (VOLUME_GUID_PATH_PREFIX.length() + VOLUME_GUID_PATH_SUFFIX.length()))
                || (!volumeGUIDPath.startsWith(VOLUME_GUID_PATH_PREFIX))
                || (!volumeGUIDPath.endsWith(VOLUME_GUID_PATH_SUFFIX))) {
            throw new IllegalArgumentException("Bad volume GUID path format: " + volumeGUIDPath);
        }

        return volumeGUIDPath.substring(VOLUME_GUID_PATH_PREFIX.length(), volumeGUIDPath.length() - VOLUME_GUID_PATH_SUFFIX.length());
    }

    public static final String QueryFullProcessImageName(int pid, int dwFlags) {
        HANDLE hProcess = null;
        Win32Exception we = null;

        try {
            hProcess = Kernel32.INSTANCE.OpenProcess(WinNT.PROCESS_QUERY_INFORMATION | WinNT.PROCESS_VM_READ, false, pid);
            if (hProcess == null) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }
            return QueryFullProcessImageName(hProcess, dwFlags);
        } catch (Win32Exception e) {
            we = e;
            throw we;
        } finally {
            try {
                closeHandle(hProcess);
            } catch (Win32Exception e) {
                if (we == null) {
                    we = e;
                } else {
                    we.addSuppressed(e);
                }
            }
            if (we != null) {
                throw we;
            }
        }
    }

    public static final String QueryFullProcessImageName(HANDLE hProcess, int dwFlags) {
        int size = WinDef.MAX_PATH; // Start with MAX_PATH, then increment with 1024 each iteration
        IntByReference lpdwSize = new IntByReference();
        do {
            char[] lpExeName = new char[size];
            lpdwSize.setValue(size);
            if (Kernel32.INSTANCE.QueryFullProcessImageName(hProcess, dwFlags, lpExeName, lpdwSize)) {
                return new String(lpExeName, 0, lpdwSize.getValue());
            }
            size += 1024;
        } while (Kernel32.INSTANCE.GetLastError() == Kernel32.ERROR_INSUFFICIENT_BUFFER);
        throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
    }

    public static byte[] getResource(String path, String type, String name) {
        HMODULE target = Kernel32.INSTANCE.LoadLibraryEx(path, null, Kernel32.LOAD_LIBRARY_AS_DATAFILE);

        if (target == null) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        Win32Exception err = null;
        Pointer start = null;
        int length = 0;
        byte[] results = null;
        try {
            Pointer t = null;
            try {
                t = new Pointer(Long.parseLong(type));
            } catch (NumberFormatException e) {
                t = new Memory(Native.WCHAR_SIZE * (type.length() + 1));
                t.setWideString(0, type);
            }
            Pointer n = null;
            try {
                n = new Pointer(Long.parseLong(name));
            } catch (NumberFormatException e) {
                n = new Memory(Native.WCHAR_SIZE * (name.length() + 1));
                n.setWideString(0, name);
            }
            HRSRC hrsrc = Kernel32.INSTANCE.FindResource(target, n, t);
            if (hrsrc == null) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }
            HANDLE loaded = Kernel32.INSTANCE.LoadResource(target, hrsrc);
            if (loaded == null) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }
            length = Kernel32.INSTANCE.SizeofResource(target, hrsrc);
            if (length == 0) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }
            start = Kernel32.INSTANCE.LockResource(loaded);
            if (start == null) {
                throw new IllegalStateException("LockResource returned null.");
            }
            results = start.getByteArray(0, length);
        } catch (Win32Exception we) {
            err = we;
        } finally {
            if (target != null) {
                if (!Kernel32.INSTANCE.FreeLibrary(target)) {
                    Win32Exception we = new Win32Exception(Kernel32.INSTANCE.GetLastError());
                    if (err != null) {
                        we.addSuppressedReflected(err);
                    }
                    throw we;
                }
            }
        }

        if (err != null) {
            throw err;
        }

        return results;
    }

    public static Map<String, List<String>> getResourceNames(String path) {
        HMODULE target = Kernel32.INSTANCE.LoadLibraryEx(path, null, Kernel32.LOAD_LIBRARY_AS_DATAFILE);

        if (target == null) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        final List<String> types = new ArrayList<String>();
        final Map<String, List<String>> result = new LinkedHashMap<String, List<String>>();

        WinBase.EnumResTypeProc ertp = new WinBase.EnumResTypeProc() {

            @Override
            public boolean invoke(HMODULE module, Pointer type, Pointer lParam) {
                if (Pointer.nativeValue(type) <= 65535) {
                    types.add(Pointer.nativeValue(type) + "");
                } else {
                    types.add(type.getWideString(0));
                }
                return true;
            }
        };

        WinBase.EnumResNameProc ernp = new WinBase.EnumResNameProc() {
            @Override
            public boolean invoke(HMODULE module, Pointer type, Pointer name, Pointer lParam) {
                String typeName = "";

                if (Pointer.nativeValue(type) <= 65535) {
                    typeName = Pointer.nativeValue(type) + "";
                } else {
                    typeName = type.getWideString(0);
                }

                if (Pointer.nativeValue(name) < 65535) {
                    result.get(typeName).add(Pointer.nativeValue(name) + "");
                } else {
                    result.get(typeName).add(name.getWideString(0));
                }

                return true;
            }
        };

        Win32Exception err = null;
        try {
            if (!Kernel32.INSTANCE.EnumResourceTypes(target, ertp, null)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }

            for (final String typeName : types) {
                result.put(typeName, new ArrayList<String>());
                Pointer pointer = null;
                try {
                    pointer = new Pointer(Long.parseLong(typeName));
                } catch (NumberFormatException e) {
                    pointer = new Memory(Native.WCHAR_SIZE * (typeName.length() + 1));
                    pointer.setWideString(0, typeName);
                }

                boolean callResult = Kernel32.INSTANCE.EnumResourceNames(target, pointer, ernp, null);

                if (!callResult) {
                    throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                }
            }
        } catch (Win32Exception e) {
            err = e;
        } finally {
            if (target != null) {
                if (!Kernel32.INSTANCE.FreeLibrary(target)) {
                    Win32Exception we = new Win32Exception(Kernel32.INSTANCE.GetLastError());
                    if (err != null) {
                        we.addSuppressedReflected(err);
                    }
                    throw we;
                }
            }
        }

        if (err != null) {
            throw err;
        }
        return result;
    }

    public static List<Tlhelp32.MODULEENTRY32W> getModules(int processID) {
        HANDLE snapshot = Kernel32.INSTANCE.CreateToolhelp32Snapshot(Tlhelp32.TH32CS_SNAPMODULE, new DWORD(processID));
        if (snapshot == null) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        Win32Exception we = null;
        try {
            Tlhelp32.MODULEENTRY32W first = new Tlhelp32.MODULEENTRY32W();

            if (!Kernel32.INSTANCE.Module32FirstW(snapshot, first)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }

            List<Tlhelp32.MODULEENTRY32W> modules = new ArrayList<Tlhelp32.MODULEENTRY32W>();
            modules.add(first);

            Tlhelp32.MODULEENTRY32W next = new Tlhelp32.MODULEENTRY32W();
            while (Kernel32.INSTANCE.Module32NextW(snapshot, next)) {
                modules.add(next);
                next = new Tlhelp32.MODULEENTRY32W();
            }

            int lastError = Kernel32.INSTANCE.GetLastError();
            if (lastError != W32Errors.ERROR_SUCCESS && lastError != W32Errors.ERROR_NO_MORE_FILES) {
                throw new Win32Exception(lastError);
            }

            return modules;
        } catch (Win32Exception e) {
            we = e;
            throw we;
        } finally {
            try {
                closeHandle(snapshot);
            } catch(Win32Exception e) {
                if (we == null) {
                    we = e;
                } else {
                    we.addSuppressedReflected(e);
                }
            }

            if (we != null) {
                throw we;
            }
        }
    }

    public static String expandEnvironmentStrings(String input) {
        if(input == null) {
            return "";
        }

        int resultChars = Kernel32.INSTANCE.ExpandEnvironmentStrings(input, null, 0);

        if(resultChars == 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        Memory resultMemory;
        if( W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE ) {
            resultMemory = new Memory(resultChars * Native.WCHAR_SIZE);
        } else {
            resultMemory = new Memory(resultChars + 1);
        }
        resultChars = Kernel32.INSTANCE.ExpandEnvironmentStrings(input, resultMemory, resultChars);

        if(resultChars == 0) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        if( W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE ) {
            return resultMemory.getWideString(0);
        } else {
            return resultMemory.getString(0);
        }
    }
}
