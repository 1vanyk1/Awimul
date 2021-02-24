package com.vantacom.aarm.libraries.jna;

import com.sun.jna.ptr.IntByReference;

import java.io.ByteArrayOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

import com.sun.jna.ptr.LongByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.win32.W32APITypeMapper;
import com.vantacom.aarm.libraries.jna.WinBase.FE_EXPORT_FUNC;
import com.vantacom.aarm.libraries.jna.WinBase.FE_IMPORT_FUNC;
import com.vantacom.aarm.libraries.jna.WinBase.FILETIME;
import com.vantacom.aarm.libraries.jna.WinDef.BOOLByReference;
import com.vantacom.aarm.libraries.jna.WinDef.DWORD;
import com.vantacom.aarm.libraries.jna.WinDef.DWORDByReference;
import com.vantacom.aarm.libraries.jna.WinDef.ULONG;
import com.vantacom.aarm.libraries.jna.WinDef.ULONGByReference;
import com.vantacom.aarm.libraries.jna.WinNT.ACCESS_ACEStructure;
import com.vantacom.aarm.libraries.jna.WinNT.ACCESS_ALLOWED_ACE;
import com.vantacom.aarm.libraries.jna.WinNT.ACE_HEADER;
import com.vantacom.aarm.libraries.jna.WinNT.ACL;
import com.vantacom.aarm.libraries.jna.WinNT.EVENTLOGRECORD;
import com.vantacom.aarm.libraries.jna.WinNT.GENERIC_MAPPING;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLEByReference;
import com.vantacom.aarm.libraries.jna.WinNT.PRIVILEGE_SET;
import com.vantacom.aarm.libraries.jna.WinNT.PSID;
import com.vantacom.aarm.libraries.jna.WinNT.PSIDByReference;
import com.vantacom.aarm.libraries.jna.WinNT.SECURITY_DESCRIPTOR_RELATIVE;
import com.vantacom.aarm.libraries.jna.WinNT.SECURITY_IMPERSONATION_LEVEL;
import com.vantacom.aarm.libraries.jna.WinNT.SID_NAME_USE;
import com.vantacom.aarm.libraries.jna.WinNT.TOKEN_TYPE;
import com.vantacom.aarm.libraries.jna.WinReg.HKEY;
import com.vantacom.aarm.libraries.jna.WinReg.HKEYByReference;

import static com.vantacom.aarm.libraries.jna.WinBase.CREATE_FOR_DIR;
import static com.vantacom.aarm.libraries.jna.WinBase.CREATE_FOR_IMPORT;
import static com.vantacom.aarm.libraries.jna.WinNT.DACL_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.FILE_ALL_ACCESS;
import static com.vantacom.aarm.libraries.jna.WinNT.FILE_GENERIC_EXECUTE;
import static com.vantacom.aarm.libraries.jna.WinNT.FILE_GENERIC_READ;
import static com.vantacom.aarm.libraries.jna.WinNT.FILE_GENERIC_WRITE;
import static com.vantacom.aarm.libraries.jna.WinNT.GENERIC_EXECUTE;
import static com.vantacom.aarm.libraries.jna.WinNT.GENERIC_READ;
import static com.vantacom.aarm.libraries.jna.WinNT.GENERIC_WRITE;
import static com.vantacom.aarm.libraries.jna.WinNT.GROUP_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.OWNER_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.PROTECTED_DACL_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.PROTECTED_SACL_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.SACL_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.SE_DACL_PROTECTED;
import static com.vantacom.aarm.libraries.jna.WinNT.SE_SACL_PROTECTED;
import static com.vantacom.aarm.libraries.jna.WinNT.STANDARD_RIGHTS_READ;
import static com.vantacom.aarm.libraries.jna.WinNT.TOKEN_ADJUST_PRIVILEGES;
import static com.vantacom.aarm.libraries.jna.WinNT.TOKEN_DUPLICATE;
import static com.vantacom.aarm.libraries.jna.WinNT.TOKEN_IMPERSONATE;
import static com.vantacom.aarm.libraries.jna.WinNT.TOKEN_QUERY;
import static com.vantacom.aarm.libraries.jna.WinNT.SID_AND_ATTRIBUTES;
import static com.vantacom.aarm.libraries.jna.WinNT.UNPROTECTED_DACL_SECURITY_INFORMATION;
import static com.vantacom.aarm.libraries.jna.WinNT.UNPROTECTED_SACL_SECURITY_INFORMATION;

public abstract class Advapi32Util {
    public static class Account {
        public String name;
        public String domain;
        public byte[] sid;
        public String sidString;
        public int accountType;
        public String fqn;
    }

    public static String getUserName() {
        char[] buffer = new char[128];
        IntByReference len = new IntByReference(buffer.length);
        boolean result = Advapi32.INSTANCE.GetUserNameW(buffer, len);

        if (!result) {
            switch (Kernel32.INSTANCE.GetLastError()) {
                case W32Errors.ERROR_INSUFFICIENT_BUFFER:
                    buffer = new char[len.getValue()];
                    break;

                default:
                    throw new Win32Exception(Native.getLastError());
            }

            result = Advapi32.INSTANCE.GetUserNameW(buffer, len);
        }

        if (!result) {
            throw new Win32Exception(Native.getLastError());
        }

        return Native.toString(buffer);
    }

    public static Account getAccountByName(String accountName) {
        return getAccountByName(null, accountName);
    }

    public static Account getAccountByName(String systemName, String accountName) {
        IntByReference pSid = new IntByReference(0);
        IntByReference cchDomainName = new IntByReference(0);
        PointerByReference peUse = new PointerByReference();

        if (Advapi32.INSTANCE.LookupAccountName(systemName, accountName, null,
                pSid, null, cchDomainName, peUse)) {
            throw new RuntimeException(
                    "LookupAccountNameW was expected to fail with ERROR_INSUFFICIENT_BUFFER");
        }

        int rc = Kernel32.INSTANCE.GetLastError();
        if (pSid.getValue() == 0 || rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }

        Memory sidMemory = new Memory(pSid.getValue());
        PSID result = new PSID(sidMemory);
        char[] referencedDomainName = new char[cchDomainName.getValue() + 1];

        if (!Advapi32.INSTANCE.LookupAccountName(systemName, accountName,
                result, pSid, referencedDomainName, cchDomainName, peUse)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        Account account = new Account();
        account.accountType = peUse.getPointer().getInt(0);

        String[] accountNamePartsBs = accountName.split("\\\\", 2);
        String[] accountNamePartsAt = accountName.split("@", 2);

        if (accountNamePartsBs.length == 2) {
            account.name = accountNamePartsBs[1];
        } else if (accountNamePartsAt.length == 2) {
            account.name = accountNamePartsAt[0];
        } else {
            account.name = accountName;
        }

        if (cchDomainName.getValue() > 0) {
            account.domain = Native.toString(referencedDomainName);
            account.fqn = account.domain + "\\" + account.name;
        } else {
            account.fqn = account.name;
        }

        account.sid = result.getBytes();
        account.sidString = convertSidToStringSid(new PSID(account.sid));
        return account;
    }

    public static Account getAccountBySid(PSID sid) {
        return getAccountBySid(null, sid);
    }

    public static Account getAccountBySid(String systemName, PSID sid) {
        IntByReference cchName = new IntByReference();
        IntByReference cchDomainName = new IntByReference();
        PointerByReference peUse = new PointerByReference();

        if (Advapi32.INSTANCE.LookupAccountSid(systemName, sid, null, cchName, null,
                cchDomainName, peUse)) {
            throw new RuntimeException(
                    "LookupAccountSidW was expected to fail with ERROR_INSUFFICIENT_BUFFER");
        }

        int rc = Kernel32.INSTANCE.GetLastError();
        if (cchName.getValue() == 0
                || rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }

        char[] domainName = new char[cchDomainName.getValue()];
        char[] name = new char[cchName.getValue()];

        if (!Advapi32.INSTANCE.LookupAccountSid(systemName, sid, name, cchName,
                domainName, cchDomainName, peUse)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        Account account = new Account();
        account.accountType = peUse.getPointer().getInt(0);
        account.name = Native.toString(name);

        if (cchDomainName.getValue() > 0) {
            account.domain = Native.toString(domainName);
            account.fqn = account.domain + "\\" + account.name;
        } else {
            account.fqn = account.name;
        }

        account.sid = sid.getBytes();
        account.sidString = convertSidToStringSid(sid);
        return account;
    }

    public static String convertSidToStringSid(PSID sid) {
        PointerByReference stringSid = new PointerByReference();
        if (!Advapi32.INSTANCE.ConvertSidToStringSid(sid, stringSid)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        Pointer ptr = stringSid.getValue();
        try {
            return ptr.getWideString(0);
        } finally {
            Kernel32Util.freeLocalMemory(ptr);
        }
    }

    public static byte[] convertStringSidToSid(String sidString) {
        PSIDByReference pSID = new PSIDByReference();
        if (!Advapi32.INSTANCE.ConvertStringSidToSid(sidString, pSID)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        PSID value = pSID.getValue();
        try {
            return value.getBytes();
        } finally {
            Kernel32Util.freeLocalMemory(value.getPointer());
        }
    }

    public static boolean isWellKnownSid(String sidString, int wellKnownSidType) {
        PSIDByReference pSID = new PSIDByReference();
        if (!Advapi32.INSTANCE.ConvertStringSidToSid(sidString, pSID)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        PSID value = pSID.getValue();
        try {
            return Advapi32.INSTANCE.IsWellKnownSid(value, wellKnownSidType);
        } finally {
            Kernel32Util.freeLocalMemory(value.getPointer());
        }
    }

    public static boolean isWellKnownSid(byte[] sidBytes, int wellKnownSidType) {
        PSID pSID = new PSID(sidBytes);
        return Advapi32.INSTANCE.IsWellKnownSid(pSID, wellKnownSidType);
    }

    public static int alignOnDWORD(int cbAcl) {
        return (cbAcl + (DWORD.SIZE - 1)) & 0xfffffffc;
    }

    public static int getAceSize(int sidLength) {
        return Native.getNativeSize(ACCESS_ALLOWED_ACE.class, null)
                + sidLength
                - DWORD.SIZE;
    }

    public static Account getAccountBySid(String sidString) {
        return getAccountBySid(null, sidString);
    }

    public static Account getAccountBySid(String systemName, String sidString) {
        return getAccountBySid(systemName, new PSID(convertStringSidToSid(sidString)));
    }

    public static Account[] getTokenGroups(HANDLE hToken) {
        // get token group information size
        IntByReference tokenInformationLength = new IntByReference();
        if (Advapi32.INSTANCE.GetTokenInformation(hToken,
                WinNT.TOKEN_INFORMATION_CLASS.TokenGroups, null, 0,
                tokenInformationLength)) {
            throw new RuntimeException(
                    "Expected GetTokenInformation to fail with ERROR_INSUFFICIENT_BUFFER");
        }
        int rc = Kernel32.INSTANCE.GetLastError();
        if (rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        WinNT.TOKEN_GROUPS groups = new WinNT.TOKEN_GROUPS(
                tokenInformationLength.getValue());
        if (!Advapi32.INSTANCE.GetTokenInformation(hToken,
                WinNT.TOKEN_INFORMATION_CLASS.TokenGroups, groups,
                tokenInformationLength.getValue(), tokenInformationLength)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        ArrayList<Account> userGroups = new ArrayList<Account>();
        for (SID_AND_ATTRIBUTES sidAndAttribute : groups.getGroups()) {
            Account group;
            try {
                group = Advapi32Util.getAccountBySid(sidAndAttribute.Sid);
            } catch (Exception e) {
                group = new Account();
                group.sid = sidAndAttribute.Sid.getBytes();
                group.sidString = Advapi32Util
                        .convertSidToStringSid(sidAndAttribute.Sid);
                group.name = group.sidString;
                group.fqn = group.sidString;
                group.accountType = SID_NAME_USE.SidTypeGroup;
            }
            userGroups.add(group);
        }
        return userGroups.toArray(new Account[0]);
    }

    public static Account getTokenPrimaryGroup(HANDLE hToken) {
        IntByReference tokenInformationLength = new IntByReference();
        if (Advapi32.INSTANCE.GetTokenInformation(hToken, WinNT.TOKEN_INFORMATION_CLASS.TokenPrimaryGroup, null, 0,
                tokenInformationLength)) {
            throw new RuntimeException("Expected GetTokenInformation to fail with ERROR_INSUFFICIENT_BUFFER");
        }
        int rc = Kernel32.INSTANCE.GetLastError();
        if (rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        WinNT.TOKEN_PRIMARY_GROUP primaryGroup = new WinNT.TOKEN_PRIMARY_GROUP(tokenInformationLength.getValue());
        if (!Advapi32.INSTANCE.GetTokenInformation(hToken, WinNT.TOKEN_INFORMATION_CLASS.TokenPrimaryGroup,
                primaryGroup, tokenInformationLength.getValue(), tokenInformationLength)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        Account group;
        try {
            group = Advapi32Util.getAccountBySid(primaryGroup.PrimaryGroup);
        } catch (Exception e) {
            group = new Account();
            group.sid = primaryGroup.PrimaryGroup.getBytes();
            group.sidString = Advapi32Util.convertSidToStringSid(primaryGroup.PrimaryGroup);
            group.name = group.sidString;
            group.fqn = group.sidString;
            group.accountType = SID_NAME_USE.SidTypeGroup;
        }
        return group;
    }

    public static Account getTokenAccount(HANDLE hToken) {
        IntByReference tokenInformationLength = new IntByReference();
        if (Advapi32.INSTANCE.GetTokenInformation(hToken,
                WinNT.TOKEN_INFORMATION_CLASS.TokenUser, null, 0,
                tokenInformationLength)) {
            throw new RuntimeException(
                    "Expected GetTokenInformation to fail with ERROR_INSUFFICIENT_BUFFER");
        }
        int rc = Kernel32.INSTANCE.GetLastError();
        if (rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        // get token user information
        WinNT.TOKEN_USER user = new WinNT.TOKEN_USER(
                tokenInformationLength.getValue());
        if (!Advapi32.INSTANCE.GetTokenInformation(hToken,
                WinNT.TOKEN_INFORMATION_CLASS.TokenUser, user,
                tokenInformationLength.getValue(), tokenInformationLength)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return getAccountBySid(user.User.Sid);
    }

    public static Account[] getCurrentUserGroups() {
        HANDLEByReference phToken = new HANDLEByReference();
        Win32Exception err = null;
        try {
            // open thread or process token
            HANDLE threadHandle = Kernel32.INSTANCE.GetCurrentThread();
            if (!Advapi32.INSTANCE.OpenThreadToken(threadHandle,
                    TOKEN_DUPLICATE | TOKEN_QUERY, true, phToken)) {
                int rc = Kernel32.INSTANCE.GetLastError();
                if (rc != W32Errors.ERROR_NO_TOKEN) {
                    throw new Win32Exception(rc);
                }

                HANDLE processHandle = Kernel32.INSTANCE.GetCurrentProcess();
                if (!Advapi32.INSTANCE.OpenProcessToken(processHandle,
                        TOKEN_DUPLICATE | TOKEN_QUERY, phToken)) {
                    throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                }
            }

            return getTokenGroups(phToken.getValue());
        } catch(Win32Exception e) {
            err = e;
            throw err;
        } finally {
            HANDLE hToken = phToken.getValue();
            if (!WinBase.INVALID_HANDLE_VALUE.equals(hToken)) {
                try {
                    Kernel32Util.closeHandle(hToken);
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
    }

    public static boolean registryKeyExists(HKEY root, String key) {
        return registryKeyExists(root, key, 0);
    }

    public static boolean registryKeyExists(HKEY root, String key, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        switch (rc) {
            case W32Errors.ERROR_SUCCESS:
                Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
                return true;
            case W32Errors.ERROR_FILE_NOT_FOUND:
                return false;
            default:
                throw new Win32Exception(rc);
        }
    }

    public static boolean registryValueExists(HKEY root, String key, String value) {
        return registryValueExists(root, key, value, 0);
    }

    public static boolean registryValueExists(HKEY root, String key,
                                              String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        switch (rc) {
            case W32Errors.ERROR_SUCCESS:
                break;
            case W32Errors.ERROR_FILE_NOT_FOUND:
                return false;
            default:
                throw new Win32Exception(rc);
        }
        try {
            IntByReference lpcbData = new IntByReference();
            IntByReference lpType = new IntByReference();
            rc = Advapi32.INSTANCE.RegQueryValueEx(phkKey.getValue(), value, 0,
                    lpType, (Pointer) null, lpcbData);
            switch (rc) {
                case W32Errors.ERROR_SUCCESS:
                case W32Errors.ERROR_MORE_DATA:
                case W32Errors.ERROR_INSUFFICIENT_BUFFER:
                    return true;
                case W32Errors.ERROR_FILE_NOT_FOUND:
                    return false;
                default:
                    throw new Win32Exception(rc);
            }
        } finally {
            if (phkKey.getValue() != WinBase.INVALID_HANDLE_VALUE) {
                rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
                if (rc != W32Errors.ERROR_SUCCESS) {
                    throw new Win32Exception(rc);
                }
            }
        }
    }

    public static String registryGetStringValue(HKEY root, String key,
                                                String value) {
        return registryGetStringValue(root, key, value, 0);
    }

    public static String registryGetStringValue(HKEY root, String key,
                                                String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra ,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetStringValue(phkKey.getValue(), value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static String registryGetStringValue(HKEY hKey, String value) {
        IntByReference lpcbData = new IntByReference();
        IntByReference lpType = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, (Pointer) null, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() != WinNT.REG_SZ
                && lpType.getValue() != WinNT.REG_EXPAND_SZ) {
            throw new RuntimeException("Unexpected registry type "
                    + lpType.getValue()
                    + ", expected REG_SZ or REG_EXPAND_SZ");
        }
        if (lpcbData.getValue() == 0) {
            return "";
        }
        Memory mem = new Memory(lpcbData.getValue() + Native.WCHAR_SIZE);
        mem.clear();
        rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, mem, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
            return mem.getWideString(0);
        } else {
            return mem.getString(0);
        }
    }

    public static String registryGetExpandableStringValue(HKEY root,
                                                          String key, String value) {
        return registryGetExpandableStringValue(root, key, value, 0);
    }

    public static String registryGetExpandableStringValue(HKEY root,
                                                          String key, String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetExpandableStringValue(phkKey.getValue(), value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static String registryGetExpandableStringValue(HKEY hKey, String value) {
        IntByReference lpcbData = new IntByReference();
        IntByReference lpType = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, (char[]) null, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() != WinNT.REG_EXPAND_SZ) {
            throw new RuntimeException("Unexpected registry type "
                    + lpType.getValue() + ", expected REG_SZ");
        }
        if (lpcbData.getValue() == 0) {
            return "";
        }
        Memory mem = new Memory(lpcbData.getValue() + Native.WCHAR_SIZE);
        mem.clear();
        rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, mem, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
            return mem.getWideString(0);
        } else {
            return mem.getString(0);
        }
    }

    public static String[] registryGetStringArray(HKEY root, String key,
                                                  String value) {
        return registryGetStringArray(root, key, value, 0);
    }

    public static String[] registryGetStringArray(HKEY root, String key,
                                                  String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetStringArray(phkKey.getValue(), value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static String[] registryGetStringArray(HKEY hKey, String value) {
        IntByReference lpcbData = new IntByReference();
        IntByReference lpType = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, (char[]) null, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() != WinNT.REG_MULTI_SZ) {
            throw new RuntimeException("Unexpected registry type "
                    + lpType.getValue() + ", expected REG_SZ");
        }
        Memory data = new Memory(lpcbData.getValue() + 2 * Native.WCHAR_SIZE);
        data.clear();
        rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, data, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        ArrayList<String> result = new ArrayList<String>();
        int offset = 0;
        while (offset < data.size()) {
            String s;
            if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
                s = data.getWideString(offset);
                offset += s.length() * Native.WCHAR_SIZE;
                offset += Native.WCHAR_SIZE;
            } else {
                s = data.getString(offset);
                offset += s.length();
                offset += 1;
            }

            if (s.length() == 0) {
                break;
            } else {
                result.add(s);
            }
        }
        return result.toArray(new String[0]);
    }

    public static byte[] registryGetBinaryValue(HKEY root, String key,
                                                String value) {
        return registryGetBinaryValue(root, key, value, 0);
    }

    public static byte[] registryGetBinaryValue(HKEY root, String key,
                                                String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetBinaryValue(phkKey.getValue(), value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static byte[] registryGetBinaryValue(HKEY hKey, String value) {
        IntByReference lpcbData = new IntByReference();
        IntByReference lpType = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, (Pointer) null, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() != WinNT.REG_BINARY) {
            throw new RuntimeException("Unexpected registry type "
                    + lpType.getValue() + ", expected REG_BINARY");
        }
        byte[] data = new byte[lpcbData.getValue()];
        rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, data, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        return data;
    }

    public static int registryGetIntValue(HKEY root, String key, String value) {
        return registryGetIntValue(root, key, value, 0);
    }

    public static int registryGetIntValue(HKEY root, String key, String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetIntValue(phkKey.getValue(), value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static int registryGetIntValue(HKEY hKey, String value) {
        IntByReference lpcbData = new IntByReference();
        IntByReference lpType = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, (char[]) null, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() != WinNT.REG_DWORD) {
            throw new RuntimeException("Unexpected registry type "
                    + lpType.getValue() + ", expected REG_DWORD");
        }
        IntByReference data = new IntByReference();
        rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, data, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        return data.getValue();
    }

    public static long registryGetLongValue(HKEY root, String key, String value) {
        return registryGetLongValue(root, key, value, 0);
    }

    public static long registryGetLongValue(HKEY root, String key, String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, key, 0, WinNT.KEY_READ | samDesiredExtra,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetLongValue(phkKey.getValue(), value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static long registryGetLongValue(HKEY hKey, String value) {
        IntByReference lpcbData = new IntByReference();
        IntByReference lpType = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, (char[]) null, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() != WinNT.REG_QWORD) {
            throw new RuntimeException("Unexpected registry type "
                    + lpType.getValue() + ", expected REG_QWORD");
        }
        LongByReference data = new LongByReference();
        rc = Advapi32.INSTANCE.RegQueryValueEx(hKey, value, 0,
                lpType, data, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        return data.getValue();
    }

    public static Object registryGetValue(HKEY hkKey, String subKey,
                                          String lpValueName) {
        Object result = null;
        IntByReference lpType = new IntByReference();
        IntByReference lpcbData = new IntByReference();
        int rc = Advapi32.INSTANCE.RegGetValue(hkKey, subKey, lpValueName,
                Advapi32.RRF_RT_ANY, lpType, (Pointer) null, lpcbData);
        if (lpType.getValue() == WinNT.REG_NONE)
            return null;
        if (rc != W32Errors.ERROR_SUCCESS
                && rc != W32Errors.ERROR_INSUFFICIENT_BUFFER) {
            throw new Win32Exception(rc);
        }
        Memory byteData = new Memory(lpcbData.getValue() + Native.WCHAR_SIZE);
        byteData.clear();
        rc = Advapi32.INSTANCE.RegGetValue(hkKey, subKey, lpValueName,
                Advapi32.RRF_RT_ANY, lpType, byteData, lpcbData);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        if (lpType.getValue() == WinNT.REG_DWORD) {
            result = byteData.getInt(0);
        } else if (lpType.getValue() == WinNT.REG_QWORD) {
            result = byteData.getLong(0);
        } else if (lpType.getValue() == WinNT.REG_BINARY) {
            result = byteData.getByteArray(0, lpcbData.getValue());
        } else if ((lpType.getValue() == WinNT.REG_SZ)
                || (lpType.getValue() == WinNT.REG_EXPAND_SZ)) {
            if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
                result = byteData.getWideString(0);
            } else {
                result = byteData.getString(0);
            }
        }

        return result;
    }

    public static boolean registryCreateKey(HKEY hKey, String keyName) {
        return registryCreateKey(hKey, keyName, 0);
    }

    public static boolean registryCreateKey(HKEY hKey, String keyName, int samDesiredExtra) {
        HKEYByReference phkResult = new HKEYByReference();
        IntByReference lpdwDisposition = new IntByReference();
        int rc = Advapi32.INSTANCE.RegCreateKeyEx(hKey, keyName, 0, null,
                WinNT.REG_OPTION_NON_VOLATILE, WinNT.KEY_READ | samDesiredExtra, null, phkResult,
                lpdwDisposition);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        rc = Advapi32.INSTANCE.RegCloseKey(phkResult.getValue());
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        return WinNT.REG_CREATED_NEW_KEY == lpdwDisposition.getValue();
    }

    public static boolean registryCreateKey(HKEY root, String parentPath,
                                            String keyName) {
        return registryCreateKey(root, parentPath, keyName, 0);
    }

    public static boolean registryCreateKey(HKEY root, String parentPath,
                                            String keyName, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, parentPath, 0,
                WinNT.KEY_CREATE_SUB_KEY | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryCreateKey(phkKey.getValue(), keyName);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registrySetIntValue(HKEY hKey, String name, int value) {
        byte[] data = new byte[4];
        data[0] = (byte) (value & 0xff);
        data[1] = (byte) ((value >> 8) & 0xff);
        data[2] = (byte) ((value >> 16) & 0xff);
        data[3] = (byte) ((value >> 24) & 0xff);
        int rc = Advapi32.INSTANCE.RegSetValueEx(hKey, name, 0,
                WinNT.REG_DWORD, data, 4);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registrySetIntValue(HKEY root, String keyPath,
                                           String name, int value) {
        registrySetIntValue(root, keyPath, name, value, 0);
    }

    public static void registrySetIntValue(HKEY root, String keyPath,
                                           String name, int value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registrySetIntValue(phkKey.getValue(), name, value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registrySetLongValue(HKEY hKey, String name, long value) {
        byte[] data = new byte[8];
        data[0] = (byte) (value & 0xff);
        data[1] = (byte) ((value >> 8) & 0xff);
        data[2] = (byte) ((value >> 16) & 0xff);
        data[3] = (byte) ((value >> 24) & 0xff);
        data[4] = (byte) ((value >> 32) & 0xff);
        data[5] = (byte) ((value >> 40) & 0xff);
        data[6] = (byte) ((value >> 48) & 0xff);
        data[7] = (byte) ((value >> 56) & 0xff);
        int rc = Advapi32.INSTANCE.RegSetValueEx(hKey, name, 0,
                WinNT.REG_QWORD, data, 8);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registrySetLongValue(HKEY root, String keyPath,
                                            String name, long value) {
        registrySetLongValue(root, keyPath, name, value, 0);
    }

    public static void registrySetLongValue(HKEY root, String keyPath,
                                            String name, long value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registrySetLongValue(phkKey.getValue(), name, value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registrySetStringValue(HKEY hKey, String name,
                                              String value) {
        if (value == null) {
            value = "";
        }
        Memory data;
        if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
            data = new Memory((value.length() + 1) * Native.WCHAR_SIZE);
            data.setWideString(0, value);
        } else {
            data = new Memory((value.length() + 1));
            data.setString(0, value);
        }
        int rc = Advapi32.INSTANCE.RegSetValueEx(hKey, name, 0, WinNT.REG_SZ,
                data, (int) data.size());
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registrySetStringValue(HKEY root, String keyPath,
                                              String name, String value) {
        registrySetStringValue(root, keyPath, name, value, 0);
    }

    public static void registrySetStringValue(HKEY root, String keyPath,
                                              String name, String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registrySetStringValue(phkKey.getValue(), name, value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registrySetExpandableStringValue(HKEY hKey, String name,
                                                        String value) {
        Memory data;
        if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
            data = new Memory((value.length() + 1) * Native.WCHAR_SIZE);
            data.setWideString(0, value);
        } else {
            data = new Memory((value.length() + 1));
            data.setString(0, value);
        }
        int rc = Advapi32.INSTANCE.RegSetValueEx(hKey, name, 0,
                WinNT.REG_EXPAND_SZ, data, (int) data.size());
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registrySetExpandableStringValue(HKEY root,
                                                        String keyPath, String name, String value) {
        registrySetExpandableStringValue(root, keyPath, name, value, 0);
    }

    public static void registrySetExpandableStringValue(HKEY root,
                                                        String keyPath, String name, String value, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registrySetExpandableStringValue(phkKey.getValue(), name, value);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registrySetStringArray(HKEY hKey, String name,
                                              String[] arr) {

        int charwidth = W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE ? Native.WCHAR_SIZE : 1;

        int size = 0;
        for (String s : arr) {
            size += s.length() * charwidth;
            size += charwidth;
        }
        size += charwidth;

        int offset = 0;
        Memory data = new Memory(size);
        data.clear();
        for (String s : arr) {
            if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
                data.setWideString(offset, s);
            } else {
                data.setString(offset, s);
            }
            offset += s.length() * charwidth;
            offset += charwidth;
        }

        int rc = Advapi32.INSTANCE.RegSetValueEx(hKey, name, 0,
                WinNT.REG_MULTI_SZ, data, size);

        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registrySetStringArray(HKEY root, String keyPath,
                                              String name, String[] arr) {
        registrySetStringArray(root, keyPath, name, arr, 0);
    }

    public static void registrySetStringArray(HKEY root, String keyPath,
                                              String name, String[] arr, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registrySetStringArray(phkKey.getValue(), name, arr);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registrySetBinaryValue(HKEY hKey, String name,
                                              byte[] data) {
        int rc = Advapi32.INSTANCE.RegSetValueEx(hKey, name, 0,
                WinNT.REG_BINARY, data, data.length);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registrySetBinaryValue(HKEY root, String keyPath,
                                              String name, byte[] data) {
        registrySetBinaryValue(root, keyPath, name, data, 0);
    }

    public static void registrySetBinaryValue(HKEY root, String keyPath,
                                              String name, byte[] data, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registrySetBinaryValue(phkKey.getValue(), name, data);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registryDeleteKey(HKEY hKey, String keyName) {
        int rc = Advapi32.INSTANCE.RegDeleteKey(hKey, keyName);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registryDeleteKey(HKEY root, String keyPath,
                                         String keyName) {
        registryDeleteKey(root, keyPath, keyName, 0);
    }

    public static void registryDeleteKey(HKEY root, String keyPath,
                                         String keyName, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registryDeleteKey(phkKey.getValue(), keyName);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static void registryDeleteValue(HKEY hKey, String valueName) {
        int rc = Advapi32.INSTANCE.RegDeleteValue(hKey, valueName);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static void registryDeleteValue(HKEY root, String keyPath,
                                           String valueName) {
        registryDeleteValue(root, keyPath, valueName, 0);
    }

    public static void registryDeleteValue(HKEY root, String keyPath,
                                           String valueName, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | WinNT.KEY_WRITE | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            registryDeleteValue(phkKey.getValue(), valueName);
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static String[] registryGetKeys(HKEY hKey) {
        IntByReference lpcSubKeys = new IntByReference();
        IntByReference lpcMaxSubKeyLen = new IntByReference();
        int rc = Advapi32.INSTANCE
                .RegQueryInfoKey(hKey, null, null, null, lpcSubKeys,
                        lpcMaxSubKeyLen, null, null, null, null, null, null);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        ArrayList<String> keys = new ArrayList<String>(lpcSubKeys.getValue());
        char[] name = new char[lpcMaxSubKeyLen.getValue() + 1];
        for (int i = 0; i < lpcSubKeys.getValue(); i++) {
            IntByReference lpcchValueName = new IntByReference(
                    lpcMaxSubKeyLen.getValue() + 1);
            rc = Advapi32.INSTANCE.RegEnumKeyEx(hKey, i, name, lpcchValueName,
                    null, null, null, null);
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
            keys.add(Native.toString(name));
        }
        return keys.toArray(new String[0]);
    }

    public static String[] registryGetKeys(HKEY root, String keyPath) {
        return registryGetKeys(root, keyPath, 0);
    }

    public static String[] registryGetKeys(HKEY root, String keyPath, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetKeys(phkKey.getValue());
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static HKEYByReference registryGetKey(HKEY root, String keyPath,
                                                 int samDesired) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0, samDesired,
                phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }

        return phkKey;
    }

    public static void registryCloseKey(HKEY hKey) {
        int rc = Advapi32.INSTANCE.RegCloseKey(hKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
    }

    public static TreeMap<String, Object> registryGetValues(HKEY hKey) {
        IntByReference lpcValues = new IntByReference();
        IntByReference lpcMaxValueNameLen = new IntByReference();
        IntByReference lpcMaxValueLen = new IntByReference();
        int rc = Advapi32.INSTANCE.RegQueryInfoKey(hKey, null, null, null,
                null, null, null, lpcValues, lpcMaxValueNameLen,
                lpcMaxValueLen, null, null);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        TreeMap<String, Object> keyValues = new TreeMap<String, Object>();
        char[] name = new char[lpcMaxValueNameLen.getValue() + 1];
        Memory byteData = new Memory(lpcMaxValueLen.getValue() + 2 * Native.WCHAR_SIZE);
        for (int i = 0; i < lpcValues.getValue(); i++) {
            byteData.clear();
            IntByReference lpcchValueName = new IntByReference(
                    lpcMaxValueNameLen.getValue() + 1);
            IntByReference lpcbData = new IntByReference(
                    lpcMaxValueLen.getValue());
            IntByReference lpType = new IntByReference();
            rc = Advapi32.INSTANCE.RegEnumValue(hKey, i, name, lpcchValueName,
                    null, lpType, byteData, lpcbData);
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }

            String nameString = Native.toString(name);

            if (lpcbData.getValue() == 0) {
                switch (lpType.getValue()) {
                    case WinNT.REG_BINARY: {
                        keyValues.put(nameString, new byte[0]);
                        break;
                    }
                    case WinNT.REG_SZ:
                    case WinNT.REG_EXPAND_SZ: {
                        keyValues.put(nameString, new char[0]);
                        break;
                    }
                    case WinNT.REG_MULTI_SZ: {
                        keyValues.put(nameString, new String[0]);
                        break;
                    }
                    case WinNT.REG_NONE: {
                        keyValues.put(nameString, null);
                        break;
                    }
                    default:
                        throw new RuntimeException("Unsupported empty type: "
                                + lpType.getValue());
                }
                continue;
            }

            switch (lpType.getValue()) {
                case WinNT.REG_QWORD: {
                    keyValues.put(nameString, byteData.getLong(0));
                    break;
                }
                case WinNT.REG_DWORD: {
                    keyValues.put(nameString, byteData.getInt(0));
                    break;
                }
                case WinNT.REG_SZ:
                case WinNT.REG_EXPAND_SZ: {
                    if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
                        keyValues.put(nameString, byteData.getWideString(0));
                    } else {
                        keyValues.put(nameString, byteData.getString(0));
                    }
                    break;
                }
                case WinNT.REG_BINARY: {
                    keyValues.put(nameString,
                            byteData.getByteArray(0, lpcbData.getValue()));
                    break;
                }
                case WinNT.REG_MULTI_SZ: {
                    ArrayList<String> result = new ArrayList<String>();
                    int offset = 0;
                    while (offset < byteData.size()) {
                        String s;
                        if (W32APITypeMapper.DEFAULT == W32APITypeMapper.UNICODE) {
                            s = byteData.getWideString(offset);
                            offset += s.length() * Native.WCHAR_SIZE;
                            offset += Native.WCHAR_SIZE;
                        } else {
                            s = byteData.getString(offset);
                            offset += s.length();
                            offset += 1;
                        }

                        if (s.length() == 0) {
                            break;
                        } else {
                            result.add(s);
                        }
                    }
                    keyValues.put(nameString, result.toArray(new String[0]));
                    break;
                }
                default:
                    throw new RuntimeException("Unsupported type: "
                            + lpType.getValue());
            }
        }
        return keyValues;
    }

    public static TreeMap<String, Object> registryGetValues(HKEY root,
                                                            String keyPath) {
        return registryGetValues(root, keyPath, 0);
    }

    public static TreeMap<String, Object> registryGetValues(HKEY root,
                                                            String keyPath, int samDesiredExtra) {
        HKEYByReference phkKey = new HKEYByReference();
        int rc = Advapi32.INSTANCE.RegOpenKeyEx(root, keyPath, 0,
                WinNT.KEY_READ | samDesiredExtra, phkKey);
        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }
        try {
            return registryGetValues(phkKey.getValue());
        } finally {
            rc = Advapi32.INSTANCE.RegCloseKey(phkKey.getValue());
            if (rc != W32Errors.ERROR_SUCCESS) {
                throw new Win32Exception(rc);
            }
        }
    }

    public static InfoKey registryQueryInfoKey(HKEY hKey,
                                               int lpcbSecurityDescriptor) {

        InfoKey infoKey = new InfoKey(hKey, lpcbSecurityDescriptor);
        int rc = Advapi32.INSTANCE.RegQueryInfoKey(hKey, infoKey.lpClass,
                infoKey.lpcClass, null, infoKey.lpcSubKeys,
                infoKey.lpcMaxSubKeyLen, infoKey.lpcMaxClassLen,
                infoKey.lpcValues, infoKey.lpcMaxValueNameLen,
                infoKey.lpcMaxValueLen, infoKey.lpcbSecurityDescriptor,
                infoKey.lpftLastWriteTime);

        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }

        return infoKey;
    }

    public static class InfoKey {
        public HKEY hKey;
        public char[] lpClass = new char[WinNT.MAX_PATH];
        public IntByReference lpcClass = new IntByReference(WinNT.MAX_PATH);
        public IntByReference lpcSubKeys = new IntByReference();
        public IntByReference lpcMaxSubKeyLen = new IntByReference();
        public IntByReference lpcMaxClassLen = new IntByReference();
        public IntByReference lpcValues = new IntByReference();
        public IntByReference lpcMaxValueNameLen = new IntByReference();
        public IntByReference lpcMaxValueLen = new IntByReference();
        public IntByReference lpcbSecurityDescriptor = new IntByReference();
        public FILETIME lpftLastWriteTime = new FILETIME();

        public InfoKey() {
        }

        public InfoKey(HKEY hKey, int securityDescriptor) {
            this.hKey = hKey;
            this.lpcbSecurityDescriptor = new IntByReference(securityDescriptor);
        }
    }

    public static EnumKey registryRegEnumKey(HKEY hKey, int dwIndex) {
        EnumKey enumKey = new EnumKey(hKey, dwIndex);
        int rc = Advapi32.INSTANCE.RegEnumKeyEx(hKey, enumKey.dwIndex,
                enumKey.lpName, enumKey.lpcName, null, enumKey.lpClass,
                enumKey.lpcbClass, enumKey.lpftLastWriteTime);

        if (rc != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(rc);
        }

        return enumKey;
    }

    public static class EnumKey {
        public HKEY hKey;
        public int dwIndex = 0;
        public char[] lpName = new char[Advapi32.MAX_KEY_LENGTH];
        public IntByReference lpcName = new IntByReference(
                Advapi32.MAX_KEY_LENGTH);
        public char[] lpClass = new char[Advapi32.MAX_KEY_LENGTH];
        public IntByReference lpcbClass = new IntByReference(
                Advapi32.MAX_KEY_LENGTH);
        public FILETIME lpftLastWriteTime = new FILETIME();

        public EnumKey() {
        }

        public EnumKey(HKEY hKey, int dwIndex) {
            this.hKey = hKey;
            this.dwIndex = dwIndex;
        }
    }

    public static String getEnvironmentBlock(Map<String, String> environment) {
        StringBuilder out = new StringBuilder(environment.size() * 32);
        for (Entry<String, String> entry : environment.entrySet()) {
            String    key=entry.getKey(), value=entry.getValue();
            if (value != null) {
                out.append(key).append("=").append(value).append('\0');
            }
        }
        return out.append('\0').toString();
    }

    public static enum EventLogType {
        Error, Warning, Informational, AuditSuccess, AuditFailure
    }

    public static class EventLogRecord {
        private EVENTLOGRECORD _record;
        private String _source;
        private byte[] _data;
        private String[] _strings;

        public EVENTLOGRECORD getRecord() {
            return _record;
        }

        public int getInstanceId() {
            return _record.EventID.intValue();
        }

        @Deprecated
        public int getEventId() {
            return _record.EventID.intValue();
        }

        public String getSource() {
            return _source;
        }

        public int getStatusCode() {
            return _record.EventID.intValue() & 0xFFFF;
        }

        public int getRecordNumber() {
            return _record.RecordNumber.intValue();
        }

        public int getLength() {
            return _record.Length.intValue();
        }

        public String[] getStrings() {
            return _strings;
        }

        public EventLogType getType() {
            switch (_record.EventType.intValue()) {
                case WinNT.EVENTLOG_SUCCESS:
                case WinNT.EVENTLOG_INFORMATION_TYPE:
                    return EventLogType.Informational;
                case WinNT.EVENTLOG_AUDIT_FAILURE:
                    return EventLogType.AuditFailure;
                case WinNT.EVENTLOG_AUDIT_SUCCESS:
                    return EventLogType.AuditSuccess;
                case WinNT.EVENTLOG_ERROR_TYPE:
                    return EventLogType.Error;
                case WinNT.EVENTLOG_WARNING_TYPE:
                    return EventLogType.Warning;
                default:
                    throw new RuntimeException("Invalid type: "
                            + _record.EventType.intValue());
            }
        }

        public byte[] getData() {
            return _data;
        }

        public EventLogRecord(Pointer pevlr) {
            _record = new EVENTLOGRECORD(pevlr);
            _source = pevlr.getWideString(_record.size());
            // data
            if (_record.DataLength.intValue() > 0) {
                _data = pevlr.getByteArray(_record.DataOffset.intValue(),
                        _record.DataLength.intValue());
            }
            // strings
            if (_record.NumStrings.intValue() > 0) {
                ArrayList<String> strings = new ArrayList<String>();
                int count = _record.NumStrings.intValue();
                long offset = _record.StringOffset.intValue();
                while (count > 0) {
                    String s = pevlr.getWideString(offset);
                    strings.add(s);
                    offset += s.length() * Native.WCHAR_SIZE;
                    offset += Native.WCHAR_SIZE;
                    count--;
                }
                _strings = strings.toArray(new String[0]);
            }
        }
    }

    public static class EventLogIterator implements Iterable<EventLogRecord>,
            Iterator<EventLogRecord> {

        private HANDLE _h;
        private Memory _buffer = new Memory(1024 * 64);
        private boolean _done = false;
        private int _dwRead = 0;
        private Pointer _pevlr = null;
        private int _flags;

        public EventLogIterator(String sourceName) {
            this(null, sourceName, WinNT.EVENTLOG_FORWARDS_READ);
        }

        public EventLogIterator(String serverName, String sourceName, int flags) {
            _flags = flags;
            _h = Advapi32.INSTANCE.OpenEventLog(serverName, sourceName);
            if (_h == null) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }
        }

        private boolean read() {
            if (_done || _dwRead > 0) {
                return false;
            }

            IntByReference pnBytesRead = new IntByReference();
            IntByReference pnMinNumberOfBytesNeeded = new IntByReference();

            if (!Advapi32.INSTANCE
                    .ReadEventLog(_h, WinNT.EVENTLOG_SEQUENTIAL_READ | _flags,
                            0, _buffer, (int) _buffer.size(), pnBytesRead,
                            pnMinNumberOfBytesNeeded)) {

                int rc = Kernel32.INSTANCE.GetLastError();

                if (rc == W32Errors.ERROR_INSUFFICIENT_BUFFER) {
                    _buffer = new Memory(pnMinNumberOfBytesNeeded.getValue());

                    if (!Advapi32.INSTANCE.ReadEventLog(_h,
                            WinNT.EVENTLOG_SEQUENTIAL_READ | _flags, 0,
                            _buffer, (int) _buffer.size(), pnBytesRead,
                            pnMinNumberOfBytesNeeded)) {
                        throw new Win32Exception(
                                Kernel32.INSTANCE.GetLastError());
                    }
                } else {
                    close();
                    if (rc != W32Errors.ERROR_HANDLE_EOF) {
                        throw new Win32Exception(rc);
                    }
                    return false;
                }
            }

            _dwRead = pnBytesRead.getValue();
            _pevlr = _buffer;
            return true;
        }

        public void close() {
            _done = true;
            if (_h != null) {
                if (!Advapi32.INSTANCE.CloseEventLog(_h)) {
                    throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                }
                _h = null;
            }
        }

        @Override
        public Iterator<EventLogRecord> iterator() {
            return this;
        }

        @Override
        public boolean hasNext() {
            read();
            return !_done;
        }

        @Override
        public EventLogRecord next() {
            read();
            EventLogRecord record = new EventLogRecord(_pevlr);
            _dwRead -= record.getLength();
            _pevlr = _pevlr.share(record.getLength());
            return record;
        }

        @Override
        public void remove() {
        }
    }

    public static ACE_HEADER[] getFileSecurity(String fileName,
                                               boolean compact) {
        int infoType = DACL_SECURITY_INFORMATION;
        int nLength = 1024;
        boolean repeat;
        Memory memory;

        do {
            repeat = false;
            memory = new Memory(nLength);
            IntByReference lpnSize = new IntByReference();
            boolean succeded = Advapi32.INSTANCE.GetFileSecurity(
                    fileName, infoType, memory, nLength, lpnSize);

            if (!succeded) {
                int lastError = Kernel32.INSTANCE.GetLastError();
                memory.clear();
                if (W32Errors.ERROR_INSUFFICIENT_BUFFER != lastError) {
                    throw new Win32Exception(lastError);
                }
            }
            int lengthNeeded = lpnSize.getValue();
            if (nLength < lengthNeeded) {
                repeat = true;
                nLength = lengthNeeded;
                memory.clear();
            }
        } while (repeat);

        SECURITY_DESCRIPTOR_RELATIVE sdr = new SECURITY_DESCRIPTOR_RELATIVE(
                memory);
        ACL dacl = sdr.getDiscretionaryACL();
        ACE_HEADER[] aceStructures = dacl.getACEs();

        if (compact) {
            List<ACE_HEADER> result = new ArrayList<ACE_HEADER>();
            Map<String, ACCESS_ACEStructure> aceMap = new HashMap<String, ACCESS_ACEStructure>();
            for (ACE_HEADER aceStructure : aceStructures) {
                if (aceStructure instanceof ACCESS_ACEStructure) {
                    ACCESS_ACEStructure accessACEStructure = (ACCESS_ACEStructure) aceStructure;
                    boolean inherted = ((aceStructure.AceFlags & WinNT.VALID_INHERIT_FLAGS) != 0);
                    String key = accessACEStructure.getSidString() + "/" + inherted + "/"
                            + aceStructure.getClass().getName();
                    ACCESS_ACEStructure aceStructure2 = aceMap.get(key);
                    if (aceStructure2 != null) {
                        int accessMask = aceStructure2.Mask;
                        accessMask = accessMask | accessACEStructure.Mask;
                        aceStructure2.Mask = accessMask;
                    } else {
                        aceMap.put(key, accessACEStructure);
                        result.add(aceStructure2);
                    }
                } else {
                    result.add(aceStructure);
                }
            }
            return result.toArray(new ACE_HEADER[0]);
        }

        return aceStructures;
    }

    public static enum AccessCheckPermission {
        READ(GENERIC_READ),
        WRITE(GENERIC_WRITE),
        EXECUTE(GENERIC_EXECUTE);

        final int code;

        AccessCheckPermission(int code) {
            this.code = code;
        }

        public int getCode() {
            return code;
        }
    }


    private static Memory getSecurityDescriptorForFile(final String absoluteFilePath) {
        final int infoType = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                DACL_SECURITY_INFORMATION;

        final IntByReference lpnSize = new IntByReference();
        boolean succeeded = Advapi32.INSTANCE.GetFileSecurity(
                absoluteFilePath,
                infoType,
                null,
                0, lpnSize);

        if (!succeeded) {
            final int lastError = Kernel32.INSTANCE.GetLastError();
            if (W32Errors.ERROR_INSUFFICIENT_BUFFER != lastError) {
                throw new Win32Exception(lastError);
            }
        }

        final int nLength = lpnSize.getValue();
        final Memory securityDescriptorMemoryPointer = new Memory(nLength);
        succeeded = Advapi32.INSTANCE.GetFileSecurity(
                absoluteFilePath, infoType, securityDescriptorMemoryPointer, nLength, lpnSize);

        if (!succeeded) {
            securityDescriptorMemoryPointer.clear();
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        return securityDescriptorMemoryPointer;
    }

    public static Memory getSecurityDescriptorForObject(final String absoluteObjectPath, int objectType, boolean getSACL) {

        int infoType = OWNER_SECURITY_INFORMATION
                | GROUP_SECURITY_INFORMATION
                | DACL_SECURITY_INFORMATION
                | (getSACL ? SACL_SECURITY_INFORMATION : 0);

        PointerByReference ppSecurityDescriptor = new PointerByReference();

        int lastError = Advapi32.INSTANCE.GetNamedSecurityInfo(
                absoluteObjectPath,
                objectType,
                infoType,
                null,
                null,
                null,
                null,
                ppSecurityDescriptor);

        if (lastError != 0) {
            throw new Win32Exception(lastError);
        }

        int nLength = Advapi32.INSTANCE.GetSecurityDescriptorLength(ppSecurityDescriptor.getValue());
        Memory memory = new Memory(nLength);
        Pointer secValue = ppSecurityDescriptor.getValue();
        try {
            byte[] data = secValue.getByteArray(0, nLength);
            memory.write(0, data, 0, nLength);
            return memory;
        } finally {
            Kernel32Util.freeLocalMemory(secValue);
        }
    }

    public static void setSecurityDescriptorForObject(final String absoluteObjectPath,
                                                      int objectType,
                                                      SECURITY_DESCRIPTOR_RELATIVE securityDescriptor,
                                                      boolean setOwner,
                                                      boolean setGroup,
                                                      boolean setDACL,
                                                      boolean setSACL,
                                                      boolean setDACLProtectedStatus,
                                                      boolean setSACLProtectedStatus) {

        final PSID psidOwner = securityDescriptor.getOwner();
        final PSID psidGroup = securityDescriptor.getGroup();
        final ACL dacl = securityDescriptor.getDiscretionaryACL();
        final ACL sacl = securityDescriptor.getSystemACL();

        int infoType = 0;
        if (setOwner) {
            if (psidOwner == null)
                throw new IllegalArgumentException("SECURITY_DESCRIPTOR_RELATIVE does not contain owner");
            if (!Advapi32.INSTANCE.IsValidSid(psidOwner))
                throw new IllegalArgumentException("Owner PSID is invalid");
            infoType |= OWNER_SECURITY_INFORMATION;
        }

        if (setGroup) {
            if (psidGroup == null)
                throw new IllegalArgumentException("SECURITY_DESCRIPTOR_RELATIVE does not contain group");
            if (!Advapi32.INSTANCE.IsValidSid(psidGroup))
                throw new IllegalArgumentException("Group PSID is invalid");
            infoType |= GROUP_SECURITY_INFORMATION;
        }

        if (setDACL) {
            if (dacl == null)
                throw new IllegalArgumentException("SECURITY_DESCRIPTOR_RELATIVE does not contain DACL");
            if (!Advapi32.INSTANCE.IsValidAcl(dacl.getPointer()))
                throw new IllegalArgumentException("DACL is invalid");
            infoType |= DACL_SECURITY_INFORMATION;
        }

        if (setSACL) {
            if (sacl == null)
                throw new IllegalArgumentException("SECURITY_DESCRIPTOR_RELATIVE does not contain SACL");
            if (!Advapi32.INSTANCE.IsValidAcl(sacl.getPointer()))
                throw new IllegalArgumentException("SACL is invalid");
            infoType |= SACL_SECURITY_INFORMATION;
        }
        if (setDACLProtectedStatus) {
            if ((securityDescriptor.Control & SE_DACL_PROTECTED) != 0) {
                infoType |= PROTECTED_DACL_SECURITY_INFORMATION;
            }
            else if ((securityDescriptor.Control & SE_DACL_PROTECTED) == 0) {
                infoType |= UNPROTECTED_DACL_SECURITY_INFORMATION;
            }
        }
        if (setSACLProtectedStatus) {
            if ((securityDescriptor.Control & SE_SACL_PROTECTED) != 0) {
                infoType |= PROTECTED_SACL_SECURITY_INFORMATION;
            } else if ((securityDescriptor.Control & SE_SACL_PROTECTED) == 0) {
                infoType |= UNPROTECTED_SACL_SECURITY_INFORMATION;
            }
        }
        int lastError = Advapi32.INSTANCE.SetNamedSecurityInfo(
                absoluteObjectPath,
                objectType,
                infoType,
                setOwner ? psidOwner.getPointer() : null,
                setGroup ? psidGroup.getPointer() : null,
                setDACL ? dacl.getPointer() : null,
                setSACL ? sacl.getPointer() : null);
        if (lastError != 0) {
            throw new Win32Exception(lastError);
        }
    }

    public static boolean accessCheck(File file, AccessCheckPermission permissionToCheck) {
        Memory securityDescriptorMemoryPointer = getSecurityDescriptorForFile(file.getAbsolutePath().replace('/', '\\'));

        HANDLEByReference openedAccessToken = new HANDLEByReference();
        HANDLEByReference duplicatedToken = new HANDLEByReference();
        Win32Exception err = null;
        try {
            int desireAccess = TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE | STANDARD_RIGHTS_READ;
            HANDLE hProcess = Kernel32.INSTANCE.GetCurrentProcess();
            if (!Advapi32.INSTANCE.OpenProcessToken(hProcess, desireAccess, openedAccessToken)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }

            if (!Advapi32.INSTANCE.DuplicateToken(openedAccessToken.getValue(), SECURITY_IMPERSONATION_LEVEL.SecurityImpersonation, duplicatedToken)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }

            GENERIC_MAPPING mapping = new GENERIC_MAPPING();
            mapping.genericRead = new DWORD(FILE_GENERIC_READ);
            mapping.genericWrite = new DWORD(FILE_GENERIC_WRITE);
            mapping.genericExecute = new DWORD(FILE_GENERIC_EXECUTE);
            mapping.genericAll = new DWORD(FILE_ALL_ACCESS);

            DWORDByReference rights = new DWORDByReference(new DWORD(permissionToCheck.getCode()));
            Advapi32.INSTANCE.MapGenericMask(rights, mapping);

            PRIVILEGE_SET privileges = new PRIVILEGE_SET(1);
            privileges.PrivilegeCount = new DWORD(0);
            DWORDByReference privilegeLength = new DWORDByReference(new DWORD(privileges.size()));

            DWORDByReference grantedAccess = new DWORDByReference();
            BOOLByReference result = new BOOLByReference();
            if (!Advapi32.INSTANCE.AccessCheck(securityDescriptorMemoryPointer,
                    duplicatedToken.getValue(),
                    rights.getValue(),
                    mapping,
                    privileges, privilegeLength, grantedAccess, result)) {
                throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
            }

            return result.getValue().booleanValue();
        } catch (Win32Exception e) {
            err = e;
            throw err;
        } finally {
            try {
                Kernel32Util.closeHandleRefs(openedAccessToken, duplicatedToken);
            } catch (Win32Exception e) {
                if (err == null) {
                    err = e;
                } else {
                    err.addSuppressedReflected(e);
                }
            }

            if (securityDescriptorMemoryPointer != null) {
                securityDescriptorMemoryPointer.clear();
            }

            if (err != null) {
                throw err;
            }
        }
    }

    public static SECURITY_DESCRIPTOR_RELATIVE getFileSecurityDescriptor(File file, boolean getSACL)
    {
        SECURITY_DESCRIPTOR_RELATIVE sdr;
        Memory securityDesc = getSecurityDescriptorForObject(file.getAbsolutePath().replaceAll("/", "\\"), AccCtrl.SE_OBJECT_TYPE.SE_FILE_OBJECT, getSACL);
        sdr = new SECURITY_DESCRIPTOR_RELATIVE(securityDesc);
        return sdr;
    }

    public static void setFileSecurityDescriptor(
            File file,
            SECURITY_DESCRIPTOR_RELATIVE securityDescriptor,
            boolean setOwner,
            boolean setGroup,
            boolean setDACL,
            boolean setSACL,
            boolean setDACLProtectedStatus,
            boolean setSACLProtectedStatus)
    {
        setSecurityDescriptorForObject(file.getAbsolutePath().replaceAll("/", "\\"), AccCtrl.SE_OBJECT_TYPE.SE_FILE_OBJECT, securityDescriptor, setOwner, setGroup, setDACL, setSACL, setDACLProtectedStatus, setSACLProtectedStatus);
    }

    public static void encryptFile(File file) {
        String lpFileName = file.getAbsolutePath();
        if (!Advapi32.INSTANCE.EncryptFile(lpFileName)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static void decryptFile(File file) {
        String lpFileName = file.getAbsolutePath();
        if (!Advapi32.INSTANCE.DecryptFile(lpFileName, new DWORD(0))) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
    }

    public static int fileEncryptionStatus(File file) {
        DWORDByReference status = new DWORDByReference();
        String lpFileName = file.getAbsolutePath();
        if (!Advapi32.INSTANCE.FileEncryptionStatus(lpFileName, status)) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        return status.getValue().intValue();
    }

    public static void disableEncryption(File directory, boolean disable) {
        String dirPath = directory.getAbsolutePath();
        if (!Advapi32.INSTANCE.EncryptionDisable(dirPath, disable)) {
            throw new Win32Exception(Native.getLastError());
        }
    }

    public static void backupEncryptedFile(File src, File destDir) {
        if (!destDir.isDirectory()) {
            throw new IllegalArgumentException("destDir must be a directory.");
        }

        ULONG readFlag = new ULONG(0);
        ULONG writeFlag = new ULONG(CREATE_FOR_IMPORT);

        if (src.isDirectory()) {
            writeFlag.setValue(CREATE_FOR_IMPORT | CREATE_FOR_DIR);
        }

        String srcFileName = src.getAbsolutePath();
        PointerByReference pvContext = new PointerByReference();
        if (Advapi32.INSTANCE.OpenEncryptedFileRaw(srcFileName, readFlag,
                pvContext) != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        FE_EXPORT_FUNC pfExportCallback = new FE_EXPORT_FUNC() {
            @Override
            public DWORD callback(Pointer pbData, Pointer pvCallbackContext,
                                  ULONG ulLength) {
                byte[] arr = pbData.getByteArray(0, ulLength.intValue());
                try {
                    outputStream.write(arr);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
                return new DWORD(W32Errors.ERROR_SUCCESS);
            }
        };

        if (Advapi32.INSTANCE.ReadEncryptedFileRaw(pfExportCallback, null,
                pvContext.getValue()) != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        try {
            outputStream.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        Advapi32.INSTANCE.CloseEncryptedFileRaw(pvContext.getValue());

        String destFileName = destDir.getAbsolutePath() + File.separator
                + src.getName();
        pvContext = new PointerByReference();
        if (Advapi32.INSTANCE.OpenEncryptedFileRaw(destFileName, writeFlag,
                pvContext) != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }

        final IntByReference elementsReadWrapper = new IntByReference(0);
        FE_IMPORT_FUNC pfImportCallback = new FE_IMPORT_FUNC() {
            @Override
            public DWORD callback(Pointer pbData, Pointer pvCallbackContext,
                                  ULONGByReference ulLength) {
                int elementsRead = elementsReadWrapper.getValue();
                int remainingElements = outputStream.size() - elementsRead;
                int length = Math.min(remainingElements, ulLength.getValue().intValue());
                pbData.write(0, outputStream.toByteArray(), elementsRead,
                        length);
                elementsReadWrapper.setValue(elementsRead + length);
                ulLength.setValue(new ULONG(length));
                return new DWORD(W32Errors.ERROR_SUCCESS);
            }
        };

        if (Advapi32.INSTANCE.WriteEncryptedFileRaw(pfImportCallback, null,
                pvContext.getValue()) != W32Errors.ERROR_SUCCESS) {
            throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
        }
        Advapi32.INSTANCE.CloseEncryptedFileRaw(pvContext.getValue());
    }

    public static class Privilege implements Closeable {
        private boolean currentlyImpersonating = false;
        private boolean privilegesEnabled = false;
        private final WinNT.LUID[] pLuids;

        public Privilege(String... privileges) throws IllegalArgumentException, Win32Exception {
            pLuids = new WinNT.LUID[privileges.length];
            int i = 0;
            for (String p : privileges) {
                pLuids[i] = new WinNT.LUID();
                if (!Advapi32.INSTANCE.LookupPrivilegeValue(null, p, pLuids[i])) {
                    throw new IllegalArgumentException("Failed to find privilege \"" + privileges[i] + "\" - " + Kernel32.INSTANCE.GetLastError());
                }
                i++;
            }
        }

        public void close() {
            this.disable();
        }

        public Privilege enable() throws Win32Exception {
            if (privilegesEnabled)
                return this;

            final HANDLEByReference phThreadToken = new HANDLEByReference();

            try {
                phThreadToken.setValue(getThreadToken());
                WinNT.TOKEN_PRIVILEGES tp = new WinNT.TOKEN_PRIVILEGES(pLuids.length);
                for (int i = 0; i < pLuids.length; i++) {
                    tp.Privileges[i] = new WinNT.LUID_AND_ATTRIBUTES(pLuids[i], new DWORD(WinNT.SE_PRIVILEGE_ENABLED));
                }
                if (!Advapi32.INSTANCE.AdjustTokenPrivileges(phThreadToken.getValue(), false, tp, 0, null, null)) {
                    throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                }
                privilegesEnabled = true;
            }
            catch (Win32Exception ex) {
                if (currentlyImpersonating) {
                    Advapi32.INSTANCE.SetThreadToken(null, null);
                    currentlyImpersonating = false;
                }
                else {
                    if (privilegesEnabled) {
                        WinNT.TOKEN_PRIVILEGES tp = new WinNT.TOKEN_PRIVILEGES(pLuids.length);
                        for (int i = 0; i < pLuids.length; i++) {
                            tp.Privileges[i] = new WinNT.LUID_AND_ATTRIBUTES(pLuids[i], new DWORD(0));
                        }
                        Advapi32.INSTANCE.AdjustTokenPrivileges(phThreadToken.getValue(), false, tp, 0, null, null);
                        privilegesEnabled = false;
                    }
                }
                throw ex;
            }
            finally {
                if ((phThreadToken.getValue() != WinBase.INVALID_HANDLE_VALUE)
                        && (phThreadToken.getValue() != null)) {
                    Kernel32.INSTANCE.CloseHandle(phThreadToken.getValue());
                    phThreadToken.setValue(null);
                }
            }
            return this;
        }

        public void disable() throws Win32Exception {
            final HANDLEByReference phThreadToken = new HANDLEByReference();

            try {
                phThreadToken.setValue(getThreadToken());
                if (currentlyImpersonating) {
                    Advapi32.INSTANCE.SetThreadToken(null, null);
                }
                else
                {
                    if (privilegesEnabled) {
                        WinNT.TOKEN_PRIVILEGES tp = new WinNT.TOKEN_PRIVILEGES(pLuids.length);
                        for (int i = 0; i < pLuids.length; i++) {
                            tp.Privileges[i] = new WinNT.LUID_AND_ATTRIBUTES(pLuids[i], new DWORD(0));
                        }
                        Advapi32.INSTANCE.AdjustTokenPrivileges(phThreadToken.getValue(), false, tp, 0, null, null);
                        privilegesEnabled = false;
                    }
                }
            }
            finally {
                if ((phThreadToken.getValue() != WinBase.INVALID_HANDLE_VALUE)
                        && (phThreadToken.getValue() != null)) {
                    Kernel32.INSTANCE.CloseHandle(phThreadToken.getValue());
                    phThreadToken.setValue(null);
                }
            }
        }

        private HANDLE getThreadToken() throws Win32Exception {
            final HANDLEByReference phThreadToken = new HANDLEByReference();
            final HANDLEByReference phProcessToken = new HANDLEByReference();

            try {
                if (!Advapi32.INSTANCE.OpenThreadToken(Kernel32.INSTANCE.GetCurrentThread(),
                        TOKEN_ADJUST_PRIVILEGES,
                        false,
                        phThreadToken)) {
                    int lastError = Kernel32.INSTANCE.GetLastError();
                    if (W32Errors.ERROR_NO_TOKEN != lastError) {
                        throw new Win32Exception(lastError);
                    }

                    if (!Advapi32.INSTANCE.OpenProcessToken(Kernel32.INSTANCE.GetCurrentProcess(), TOKEN_DUPLICATE, phProcessToken)) {
                        throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                    }

                    if (!Advapi32.INSTANCE.DuplicateTokenEx(phProcessToken.getValue(),
                            TOKEN_ADJUST_PRIVILEGES | TOKEN_IMPERSONATE,
                            null,
                            SECURITY_IMPERSONATION_LEVEL.SecurityImpersonation,
                            TOKEN_TYPE.TokenImpersonation,
                            phThreadToken)) {
                        throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                    }

                    if (!Advapi32.INSTANCE.SetThreadToken(null, phThreadToken.getValue())) {
                        throw new Win32Exception(Kernel32.INSTANCE.GetLastError());
                    }
                    currentlyImpersonating = true;
                }
            }
            catch (Win32Exception ex) {
                if ((phThreadToken.getValue() != WinBase.INVALID_HANDLE_VALUE)
                        && (phThreadToken.getValue() != null)) {
                    Kernel32.INSTANCE.CloseHandle(phThreadToken.getValue());
                    phThreadToken.setValue(null);
                }
                throw ex;
            }
            finally
            {
                if ((phProcessToken.getValue() != WinBase.INVALID_HANDLE_VALUE)
                        && (phProcessToken.getValue() != null)) {
                    Kernel32.INSTANCE.CloseHandle(phProcessToken.getValue());
                    phProcessToken.setValue(null);
                }
            }

            return phThreadToken.getValue();
        }
    }
}
