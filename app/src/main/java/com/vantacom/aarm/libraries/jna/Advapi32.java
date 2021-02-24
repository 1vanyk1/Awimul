package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.LongByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.ptr.ShortByReference;
import com.sun.jna.win32.W32APIOptions;
import com.vantacom.aarm.libraries.jna.WinBase.FE_EXPORT_FUNC;
import com.vantacom.aarm.libraries.jna.WinBase.FE_IMPORT_FUNC;
import com.vantacom.aarm.libraries.jna.WinBase.PROCESS_INFORMATION;
import com.vantacom.aarm.libraries.jna.WinBase.SECURITY_ATTRIBUTES;
import com.vantacom.aarm.libraries.jna.WinBase.STARTUPINFO;
import com.vantacom.aarm.libraries.jna.WinDef.BOOLByReference;
import com.vantacom.aarm.libraries.jna.WinDef.DWORDByReference;
import com.vantacom.aarm.libraries.jna.WinNT.ACL;
import com.vantacom.aarm.libraries.jna.WinNT.GENERIC_MAPPING;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLEByReference;
import com.vantacom.aarm.libraries.jna.WinNT.LUID;
import com.vantacom.aarm.libraries.jna.WinNT.PACLByReference;
import com.vantacom.aarm.libraries.jna.WinNT.PRIVILEGE_SET;
import com.vantacom.aarm.libraries.jna.WinNT.PSID;
import com.vantacom.aarm.libraries.jna.WinNT.PSIDByReference;
import com.vantacom.aarm.libraries.jna.WinNT.SECURITY_DESCRIPTOR;
import com.vantacom.aarm.libraries.jna.WinNT.SECURITY_DESCRIPTOR_RELATIVE;
import com.vantacom.aarm.libraries.jna.WinReg.HKEY;
import com.vantacom.aarm.libraries.jna.WinReg.HKEYByReference;
import com.vantacom.aarm.libraries.jna.Winsvc.HandlerEx;
import com.vantacom.aarm.libraries.jna.Winsvc.SC_HANDLE;
import com.vantacom.aarm.libraries.jna.Winsvc.SERVICE_STATUS;
import com.vantacom.aarm.libraries.jna.Winsvc.SERVICE_STATUS_HANDLE;


public interface Advapi32 extends Library {
    Advapi32 INSTANCE = Native.loadLibrary("/data/user/0/com.vantacom.aarm/files/armeabi-v7a/lib/wine/advapi32.dll.so", Advapi32.class, W32APIOptions.DEFAULT_OPTIONS);

    int MAX_KEY_LENGTH = 255;
    int MAX_VALUE_NAME = 16383;

    int RRF_RT_ANY = 0x0000ffff;
    int RRF_RT_DWORD = 0x00000018;
    int RRF_RT_QWORD = 0x00000048;
    int RRF_RT_REG_BINARY = 0x00000008;
    int RRF_RT_REG_DWORD = 0x00000010;
    int RRF_RT_REG_EXPAND_SZ = 0x00000004;
    int RRF_RT_REG_MULTI_SZ = 0x00000020;
    int RRF_RT_REG_NONE = 0x00000001;
    int RRF_RT_REG_QWORD = 0x00000040;
    int RRF_RT_REG_SZ = 0x00000002;
    int LOGON_WITH_PROFILE = 0x00000001;
    int LOGON_NETCREDENTIALS_ONLY = 0x00000002;

    boolean GetUserNameW(char[] buffer, IntByReference len);


    boolean LookupAccountName(String lpSystemName, String lpAccountName,
                              PSID Sid, IntByReference cbSid, char[] ReferencedDomainName,
                              IntByReference cchReferencedDomainName, PointerByReference peUse);

    boolean LookupAccountSid(String lpSystemName, PSID Sid,
                             char[] lpName, IntByReference cchName, char[] ReferencedDomainName,
                             IntByReference cchReferencedDomainName, PointerByReference peUse);

    boolean ConvertSidToStringSid(PSID Sid, PointerByReference StringSid);

    boolean ConvertStringSidToSid(String StringSid, PSIDByReference Sid);

    int GetLengthSid(PSID pSid);

    boolean IsValidSid(PSID pSid);

    boolean EqualSid(PSID pSid1, PSID pSid2);

    boolean IsWellKnownSid(PSID pSid, int wellKnownSidType);

    boolean CreateWellKnownSid(int wellKnownSidType, PSID domainSid,
                               PSID pSid, IntByReference cbSid);

    boolean InitializeSecurityDescriptor(SECURITY_DESCRIPTOR pSecurityDescriptor, int dwRevision);

    boolean GetSecurityDescriptorControl(SECURITY_DESCRIPTOR pSecurityDescriptor, ShortByReference pControl, IntByReference lpdwRevision);

    boolean SetSecurityDescriptorControl(SECURITY_DESCRIPTOR pSecurityDescriptor, short ControlBitsOfInterest, short ControlBitsToSet);

    boolean GetSecurityDescriptorOwner(SECURITY_DESCRIPTOR pSecurityDescriptor, PSIDByReference pOwner, BOOLByReference lpbOwnerDefaulted);

    boolean SetSecurityDescriptorOwner(SECURITY_DESCRIPTOR pSecurityDescriptor, PSID pOwner, boolean bOwnerDefaulted);

    boolean GetSecurityDescriptorGroup(SECURITY_DESCRIPTOR pSecurityDescriptor, PSIDByReference pGroup, BOOLByReference lpbGroupDefaulted);

    boolean SetSecurityDescriptorGroup(SECURITY_DESCRIPTOR pSecurityDescriptor, PSID pGroup, boolean bGroupDefaulted);

    boolean GetSecurityDescriptorDacl(SECURITY_DESCRIPTOR pSecurityDescriptor, BOOLByReference bDaclPresent, PACLByReference pDacl, BOOLByReference bDaclDefaulted);

    boolean SetSecurityDescriptorDacl(SECURITY_DESCRIPTOR pSecurityDescriptor, boolean bDaclPresent, ACL pDacl, boolean bDaclDefaulted);

    boolean InitializeAcl(ACL pAcl, int nAclLength, int dwAclRevision);

    boolean AddAce(ACL pAcl, int dwAceRevision, int dwStartingAceIndex, Pointer pAceList, int nAceListLength);

    boolean AddAccessAllowedAce(ACL pAcl, int dwAceRevision, int AccessMask, PSID pSid);

    boolean AddAccessAllowedAceEx(ACL pAcl, int dwAceRevision, int AceFlags, int AccessMask, PSID pSid);

    boolean GetAce(ACL pAcl, int dwAceIndex, PointerByReference pAce);

    boolean LogonUser(String lpszUsername, String lpszDomain,
                      String lpszPassword, int logonType, int logonProvider,
                      HANDLEByReference phToken);

    boolean OpenThreadToken(HANDLE ThreadHandle, int DesiredAccess,
                            boolean OpenAsSelf, HANDLEByReference TokenHandle);

    boolean SetThreadToken(HANDLEByReference ThreadHandle, HANDLE TokenHandle);

    boolean OpenProcessToken(HANDLE ProcessHandle, int DesiredAccess,
                             HANDLEByReference TokenHandle);

    boolean DuplicateToken(HANDLE ExistingTokenHandle,
                           int ImpersonationLevel, HANDLEByReference DuplicateTokenHandle);

    boolean DuplicateTokenEx(HANDLE hExistingToken, int dwDesiredAccess,
                             SECURITY_ATTRIBUTES lpTokenAttributes,
                             int ImpersonationLevel, int TokenType, HANDLEByReference phNewToken);

    boolean GetTokenInformation(HANDLE tokenHandle,
                                int tokenInformationClass, Structure tokenInformation,
                                int tokenInformationLength, IntByReference returnLength);

    boolean ImpersonateLoggedOnUser(HANDLE hToken);

    boolean ImpersonateSelf(int ImpersonationLevel);

    boolean RevertToSelf();

    int RegOpenKeyEx(HKEY hKey, String lpSubKey, int ulOptions,
                     int samDesired, HKEYByReference phkResult);

    int RegConnectRegistry(String lpMachineName, HKEY hKey, HKEYByReference phkResult);

    int RegQueryValueEx(HKEY hKey, String lpValueName, int lpReserved,
                        IntByReference lpType, char[] lpData, IntByReference lpcbData);

    int RegQueryValueEx(HKEY hKey, String lpValueName, int lpReserved,
                        IntByReference lpType, byte[] lpData, IntByReference lpcbData);

    int RegQueryValueEx(HKEY hKey, String lpValueName, int lpReserved,
                        IntByReference lpType, IntByReference lpData,
                        IntByReference lpcbData);

    int RegQueryValueEx(HKEY hKey, String lpValueName, int lpReserved,
                        IntByReference lpType, LongByReference lpData,
                        IntByReference lpcbData);

    int RegQueryValueEx(HKEY hKey, String lpValueName, int lpReserved,
                        IntByReference lpType, Pointer lpData, IntByReference lpcbData);

    int RegCloseKey(HKEY hKey);

    int RegDeleteValue(HKEY hKey, String lpValueName);

    int RegSetValueEx(HKEY hKey, String lpValueName, int Reserved,
                      int dwType, Pointer lpData, int cbData);

    int RegSetValueEx(HKEY hKey, String lpValueName, int Reserved,
                      int dwType, char[] lpData, int cbData);

    int RegSetValueEx(HKEY hKey, String lpValueName, int Reserved,
                      int dwType, byte[] lpData, int cbData);

    int RegCreateKeyEx(HKEY hKey, String lpSubKey, int Reserved,
                       String lpClass, int dwOptions, int samDesired,
                       SECURITY_ATTRIBUTES lpSecurityAttributes,
                       HKEYByReference phkResult, IntByReference lpdwDisposition);

    int RegDeleteKey(HKEY hKey, String name);

    int RegEnumKeyEx(HKEY hKey, int dwIndex, char[] lpName,
                     IntByReference lpcName, IntByReference reserved, char[] lpClass,
                     IntByReference lpcClass, WinBase.FILETIME lpftLastWriteTime);

    int RegEnumValue(HKEY hKey, int dwIndex, char[] lpValueName,
                     IntByReference lpcchValueName, IntByReference reserved,
                     IntByReference lpType, Pointer lpData, IntByReference lpcbData);

    int RegEnumValue(HKEY hKey, int dwIndex, char[] lpValueName,
                     IntByReference lpcchValueName, IntByReference reserved,
                     IntByReference lpType, byte[] lpData, IntByReference lpcbData);

    int RegQueryInfoKey(HKEY hKey, char[] lpClass,
                        IntByReference lpcClass, IntByReference lpReserved,
                        IntByReference lpcSubKeys, IntByReference lpcMaxSubKeyLen,
                        IntByReference lpcMaxClassLen, IntByReference lpcValues,
                        IntByReference lpcMaxValueNameLen, IntByReference lpcMaxValueLen,
                        IntByReference lpcbSecurityDescriptor,
                        WinBase.FILETIME lpftLastWriteTime);

    int RegGetValue(HKEY hkey, String lpSubKey, String lpValue,
                    int dwFlags, IntByReference pdwType, Pointer pvData,
                    IntByReference pcbData);

    int RegGetValue(HKEY hkey, String lpSubKey, String lpValue,
                    int dwFlags, IntByReference pdwType, byte[] pvData,
                    IntByReference pcbData);

    HANDLE RegisterEventSource(String lpUNCServerName, String lpSourceName);

    boolean DeregisterEventSource(HANDLE hEventLog);

    HANDLE OpenEventLog(String lpUNCServerName, String lpSourceName);

    boolean CloseEventLog(HANDLE hEventLog);

    boolean GetNumberOfEventLogRecords(HANDLE hEventLog, IntByReference NumberOfRecords);

    boolean ReportEvent(HANDLE hEventLog, int wType, int wCategory,
                        int dwEventID, PSID lpUserSid, int wNumStrings, int dwDataSize,
                        String[] lpStrings, Pointer lpRawData);

    boolean ClearEventLog(HANDLE hEventLog, String lpBackupFileName);

    boolean BackupEventLog(HANDLE hEventLog, String lpBackupFileName);

    HANDLE OpenBackupEventLog(String lpUNCServerName, String lpFileName);

    boolean ReadEventLog(HANDLE hEventLog, int dwReadFlags,
                         int dwRecordOffset, Pointer lpBuffer, int nNumberOfBytesToRead,
                         IntByReference pnBytesRead, IntByReference pnMinNumberOfBytesNeeded);

    boolean GetOldestEventLogRecord(HANDLE hEventLog, IntByReference OldestRecord);

    public boolean ChangeServiceConfig2(SC_HANDLE hService, int dwInfoLevel,
                                        Winsvc.ChangeServiceConfig2Info lpInfo);

    public boolean QueryServiceConfig2(SC_HANDLE hService, int dwInfoLevel,
                                       Pointer lpBuffer, int cbBufSize, IntByReference pcbBytesNeeded);

    boolean QueryServiceStatusEx(SC_HANDLE hService, int InfoLevel,
                                 Winsvc.SERVICE_STATUS_PROCESS lpBuffer, int cbBufSize,
                                 IntByReference pcbBytesNeeded);

    boolean QueryServiceStatus(SC_HANDLE hService, SERVICE_STATUS lpServiceStatus);

    boolean ControlService(SC_HANDLE hService, int dwControl, SERVICE_STATUS lpServiceStatus);

    boolean StartService(SC_HANDLE hService, int dwNumServiceArgs, String[] lpServiceArgVectors);

    boolean CloseServiceHandle(SC_HANDLE hSCObject);

    SC_HANDLE OpenService(SC_HANDLE hSCManager, String lpServiceName, int dwDesiredAccess);

    SC_HANDLE OpenSCManager(String lpMachineName, String lpDatabaseName, int dwDesiredAccess);

    boolean EnumDependentServices(SC_HANDLE hService, int dwServiceState,
                                  Pointer lpService, int cbBufSize, IntByReference pcbBytesNeeded,
                                  IntByReference lpServicesReturned);

    boolean EnumServicesStatusEx(SC_HANDLE hSCManager, int InfoLevel,
                                 int dwServiceType, int dwServiceState, Pointer lpServices,
                                 int cbBufSize, IntByReference pcbBytesNeeded,
                                 IntByReference lpServicesReturned, IntByReference lpResumeHandle,
                                 String pszGroupName);

    boolean CreateProcessAsUser(HANDLE hToken, String lpApplicationName,
                                String lpCommandLine, SECURITY_ATTRIBUTES lpProcessAttributes,
                                SECURITY_ATTRIBUTES lpThreadAttributes, boolean bInheritHandles,
                                int dwCreationFlags, String lpEnvironment,
                                String lpCurrentDirectory, STARTUPINFO lpStartupInfo,
                                PROCESS_INFORMATION lpProcessInformation);

    boolean AdjustTokenPrivileges(HANDLE TokenHandle,
                                  boolean DisableAllPrivileges, WinNT.TOKEN_PRIVILEGES NewState,
                                  int BufferLength, WinNT.TOKEN_PRIVILEGES PreviousState,
                                  IntByReference ReturnLength);

    boolean LookupPrivilegeName(String lpSystemName, LUID lpLuid,
                                char[] lpName, IntByReference cchName);

    boolean LookupPrivilegeValue(String lpSystemName, String lpName, LUID lpLuid);

    boolean GetFileSecurity(String lpFileName,
                            int RequestedInformation, Pointer pointer, int nLength,
                            IntByReference lpnLengthNeeded);

    boolean SetFileSecurity(String lpFileName, int SecurityInformation, Pointer pSecurityDescriptor);

    int GetSecurityInfo(HANDLE handle,
                        int ObjectType,
                        int SecurityInfo,
                        PointerByReference ppsidOwner,
                        PointerByReference ppsidGroup,
                        PointerByReference ppDacl,
                        PointerByReference ppSacl,
                        PointerByReference ppSecurityDescriptor);

    int SetSecurityInfo(HANDLE handle,
                        int ObjectType,
                        int SecurityInfo,
                        Pointer ppsidOwner,
                        Pointer ppsidGroup,
                        Pointer ppDacl,
                        Pointer ppSacl);

    int GetNamedSecurityInfo(
            String pObjectName,
            int ObjectType,
            int SecurityInfo,
            PointerByReference ppsidOwner,
            PointerByReference ppsidGroup,
            PointerByReference ppDacl,
            PointerByReference ppSacl,
            PointerByReference ppSecurityDescriptor);

    int SetNamedSecurityInfo(
            String pObjectName,
            int ObjectType,
            int SecurityInfo,
            Pointer ppsidOwner,
            Pointer ppsidGroup,
            Pointer ppDacl,
            Pointer ppSacl);

    int GetSecurityDescriptorLength(Pointer ppSecurityDescriptor);

    boolean IsValidSecurityDescriptor(Pointer ppSecurityDescriptor);

    boolean MakeSelfRelativeSD(SECURITY_DESCRIPTOR pAbsoluteSD,
                               SECURITY_DESCRIPTOR_RELATIVE pSelfRelativeSD,
                               IntByReference lpdwBufferLength);

    boolean MakeAbsoluteSD(SECURITY_DESCRIPTOR_RELATIVE pSelfRelativeSD,
                           SECURITY_DESCRIPTOR pAbsoluteSD,
                           IntByReference lpdwAbsoluteSDSize,
                           ACL pDacl,
                           IntByReference lpdwDaclSize,
                           ACL pSacl,
                           IntByReference lpdwSaclSize,
                           PSID pOwner,
                           IntByReference lpdwOwnerSize,
                           PSID pPrimaryGroup,
                           IntByReference lpdwPrimaryGroupSize);

    boolean IsValidAcl(Pointer pAcl);

    void MapGenericMask(DWORDByReference AccessMask, GENERIC_MAPPING GenericMapping);

    boolean AccessCheck(Pointer pSecurityDescriptor,
                        HANDLE ClientToken, WinDef.DWORD DesiredAccess,
                        GENERIC_MAPPING GenericMapping,
                        PRIVILEGE_SET PrivilegeSet,
                        DWORDByReference PrivilegeSetLength,
                        DWORDByReference GrantedAccess, BOOLByReference AccessStatus);

    boolean EncryptFile(String lpFileName);

    boolean DecryptFile(String lpFileName, WinDef.DWORD dwReserved);

    boolean FileEncryptionStatus(String lpFileName, DWORDByReference lpStatus);

    boolean EncryptionDisable(String DirPath, boolean Disable);

    int OpenEncryptedFileRaw(String lpFileName, WinDef.ULONG ulFlags, PointerByReference pvContext);

    int ReadEncryptedFileRaw(FE_EXPORT_FUNC pfExportCallback,
                             Pointer pvCallbackContext, Pointer pvContext);

    int WriteEncryptedFileRaw(FE_IMPORT_FUNC pfImportCallback,
                              Pointer pvCallbackContext, Pointer pvContext);

    void CloseEncryptedFileRaw(Pointer pvContext);

    boolean CreateProcessWithLogonW(String lpUsername, String lpDomain, String lpPassword, int dwLogonFlags,
                                    String lpApplicationName, String lpCommandLine, int dwCreationFlags, Pointer lpEnvironment,
                                    String lpCurrentDirectory, STARTUPINFO lpStartupInfo, PROCESS_INFORMATION lpProcessInfo);

    public boolean StartServiceCtrlDispatcher(Winsvc.SERVICE_TABLE_ENTRY[] lpServiceTable);

    public SERVICE_STATUS_HANDLE RegisterServiceCtrlHandler(String lpServiceName,
                                                                   Handler lpHandlerProc);

    public SERVICE_STATUS_HANDLE RegisterServiceCtrlHandlerEx(String lpServiceName,
                                                                     HandlerEx lpHandlerProc, Pointer lpContext);

    public boolean SetServiceStatus(SERVICE_STATUS_HANDLE hServiceStatus,
                                    SERVICE_STATUS lpServiceStatus);

    public SC_HANDLE CreateService(SC_HANDLE hSCManager, String lpServiceName,
                                   String lpDisplayName, int dwDesiredAccess, int dwServiceType,
                                   int dwStartType, int dwErrorControl, String lpBinaryPathName,
                                   String lpLoadOrderGroup, IntByReference lpdwTagId,
                                   String lpDependencies, String lpServiceStartName, String lpPassword);

    public boolean DeleteService(SC_HANDLE hService);
}