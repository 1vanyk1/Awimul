package com.vantacom.aarm.libraries.jna;

import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.win32.W32APIOptions;

@SuppressWarnings("serial")
public interface Kernel32 extends Library, WinNT, Wincon {
    Kernel32 INSTANCE = Native.loadLibrary("/data/user/0/com.vantacom.aarm/files/armeabi-v7a/lib/wine/kernel32.dll.so", Kernel32.class, W32APIOptions.UNICODE_OPTIONS);
    int LOAD_LIBRARY_AS_DATAFILE = 0x2;
    boolean ReadFile(HANDLE hFile, byte[] lpBuffer, int nNumberOfBytesToRead,
                     IntByReference lpNumberOfBytesRead, WinBase.OVERLAPPED lpOverlapped);

    Pointer LocalFree(Pointer hMem);

    Pointer GlobalFree(Pointer hGlobal);

    HMODULE GetModuleHandle(String name);

    void GetSystemTime(SYSTEMTIME lpSystemTime);

    boolean SetSystemTime(SYSTEMTIME lpSystemTime);

    void GetLocalTime(WinBase.SYSTEMTIME lpSystemTime);

    boolean SetLocalTime(SYSTEMTIME lpSystemTime);

    boolean GetSystemTimes(WinBase.FILETIME lpIdleTime, WinBase.FILETIME lpKernelTime, WinBase.FILETIME lpUserTime);

    int GetTickCount();

    long GetTickCount64();

    int GetCurrentThreadId();

    HANDLE GetCurrentThread();

    int GetCurrentProcessId();

    HANDLE GetCurrentProcess();

    int GetProcessId(HANDLE process);

    int GetProcessVersion(int processId);

    boolean GetProcessAffinityMask(HANDLE hProcess, ULONG_PTRByReference lpProcessAffinityMask,
                                   ULONG_PTRByReference lpSystemAffinityMask);

    boolean SetProcessAffinityMask(HANDLE hProcess, ULONG_PTR dwProcessAffinityMask);

    boolean GetExitCodeProcess(HANDLE hProcess, IntByReference lpExitCode);

    boolean TerminateProcess(HANDLE hProcess, int uExitCode);

    int GetLastError();

    void SetLastError(int dwErrCode);

    int GetDriveType(String lpRootPathName);

    int FormatMessage(int dwFlags, Pointer lpSource, int dwMessageId,
                      int dwLanguageId, PointerByReference lpBuffer, int nSize,
                      Pointer va_list);

    HANDLE CreateFile(String lpFileName, int dwDesiredAccess, int dwShareMode,
                      WinBase.SECURITY_ATTRIBUTES lpSecurityAttributes,
                      int dwCreationDisposition, int dwFlagsAndAttributes,
                      HANDLE hTemplateFile);

    boolean CopyFile(String lpExistingFileName, String lpNewFileName,
                     boolean bFailIfExists);

    boolean MoveFile(String lpExistingFileName, String lpNewFileName);

    boolean MoveFileEx(String lpExistingFileName, String lpNewFileName,
                       DWORD dwFlags);

    boolean CreateDirectory(String lpPathName,
                            WinBase.SECURITY_ATTRIBUTES lpSecurityAttributes);

    HANDLE CreateIoCompletionPort(HANDLE FileHandle,
                                  HANDLE ExistingCompletionPort, Pointer CompletionKey,
                                  int NumberOfConcurrentThreads);

    boolean GetQueuedCompletionStatus(HANDLE CompletionPort,
                                      IntByReference lpNumberOfBytes,
                                      ULONG_PTRByReference lpCompletionKey,
                                      PointerByReference lpOverlapped, int dwMilliseconds);

    boolean PostQueuedCompletionStatus(HANDLE CompletionPort,
                                       int dwNumberOfBytesTransferred, Pointer dwCompletionKey,
                                       WinBase.OVERLAPPED lpOverlapped);

    int WaitForSingleObject(HANDLE hHandle, int dwMilliseconds);

    int WaitForMultipleObjects(int nCount, HANDLE[] hHandle, boolean bWaitAll,
                               int dwMilliseconds);

    boolean DuplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle,
                            HANDLE hTargetProcessHandle, HANDLEByReference lpTargetHandle,
                            int dwDesiredAccess, boolean bInheritHandle, int dwOptions);

    boolean CloseHandle(HANDLE hObject);

    public boolean ReadDirectoryChangesW(HANDLE directory,
                                         WinNT.FILE_NOTIFY_INFORMATION info, int length,
                                         boolean watchSubtree, int notifyFilter,
                                         IntByReference bytesReturned, WinBase.OVERLAPPED overlapped,
                                         OVERLAPPED_COMPLETION_ROUTINE completionRoutine);

    int GetShortPathName(String lpszLongPath, char[] lpdzShortPath,
                         int cchBuffer);

    Pointer LocalAlloc(int /* UINT */uFlags, int /* SIZE_T */uBytes);

    boolean WriteFile(HANDLE hFile, byte[] lpBuffer, int nNumberOfBytesToWrite,
                      IntByReference lpNumberOfBytesWritten,
                      WinBase.OVERLAPPED lpOverlapped);

    boolean FlushFileBuffers(HANDLE hFile);

    HANDLE CreateEvent(WinBase.SECURITY_ATTRIBUTES lpEventAttributes,
                       boolean bManualReset, boolean bInitialState, String lpName);

    HANDLE OpenEvent(int dwDesiredAccess, boolean bInheritHandle, String lpName);

    boolean SetEvent(HANDLE hEvent);

    boolean ResetEvent(HANDLE hEvent);

    boolean PulseEvent(HANDLE hEvent);

    HANDLE CreateFileMapping(HANDLE hFile,
                             WinBase.SECURITY_ATTRIBUTES lpAttributes, int flProtect,
                             int dwMaximumSizeHigh, int dwMaximumSizeLow, String lpName);

    Pointer MapViewOfFile(HANDLE hFileMappingObject, int dwDesiredAccess,
                          int dwFileOffsetHigh, int dwFileOffsetLow, int dwNumberOfBytesToMap);

    boolean UnmapViewOfFile(Pointer lpBaseAddress);

    public boolean GetComputerName(char[] buffer, IntByReference lpnSize);

    boolean GetComputerNameEx(int nameType, char[] buffer, IntByReference lpnSize);

    HANDLE OpenThread(int dwDesiredAccess, boolean bInheritHandle,
                      int dwThreadId);

    boolean CreateProcess(String lpApplicationName, String lpCommandLine,
                          WinBase.SECURITY_ATTRIBUTES lpProcessAttributes,
                          WinBase.SECURITY_ATTRIBUTES lpThreadAttributes,
                          boolean bInheritHandles, DWORD dwCreationFlags,
                          Pointer lpEnvironment, String lpCurrentDirectory,
                          WinBase.STARTUPINFO lpStartupInfo,
                          WinBase.PROCESS_INFORMATION lpProcessInformation);

    boolean CreateProcessW(String lpApplicationName, char[] lpCommandLine,
                           WinBase.SECURITY_ATTRIBUTES lpProcessAttributes,
                           WinBase.SECURITY_ATTRIBUTES lpThreadAttributes,
                           boolean bInheritHandles, DWORD dwCreationFlags,
                           Pointer lpEnvironment, String lpCurrentDirectory,
                           WinBase.STARTUPINFO lpStartupInfo,
                           WinBase.PROCESS_INFORMATION lpProcessInformation);

    HANDLE OpenProcess(int fdwAccess, boolean fInherit, int IDProcess);

    boolean QueryFullProcessImageName(HANDLE hProcess, int dwFlags, char[] lpExeName, IntByReference lpdwSize);

    DWORD GetTempPath(DWORD nBufferLength, char[] buffer);

    DWORD GetVersion();

    boolean GetVersionEx(OSVERSIONINFO lpVersionInfo);

    boolean GetVersionEx(OSVERSIONINFOEX lpVersionInfo);

    boolean VerifyVersionInfoW(OSVERSIONINFOEX lpVersionInformation, int dwTypeMask, long dwlConditionMask);

    long VerSetConditionMask(long conditionMask, int typeMask, byte condition);

    void GetSystemInfo(SYSTEM_INFO lpSystemInfo);

    void GetNativeSystemInfo(SYSTEM_INFO lpSystemInfo);

    boolean IsWow64Process(HANDLE hProcess, IntByReference Wow64Process);

    boolean GetLogicalProcessorInformation(Pointer buffer,
                                           DWORDByReference returnLength);

    boolean GetLogicalProcessorInformationEx(int relationshipType, Pointer buffer, DWORDByReference returnedLength);

    boolean GlobalMemoryStatusEx(MEMORYSTATUSEX lpBuffer);

    boolean GetFileInformationByHandleEx(HANDLE hFile, int FileInformationClass, Pointer lpFileInformation, DWORD dwBufferSize);

    boolean SetFileInformationByHandle(HANDLE hFile, int FileInformationClass, Pointer lpFileInformation, DWORD dwBufferSize);

    boolean GetFileTime(HANDLE hFile, WinBase.FILETIME lpCreationTime,
                        WinBase.FILETIME lpLastAccessTime, WinBase.FILETIME lpLastWriteTime);

    int SetFileTime(HANDLE hFile, WinBase.FILETIME lpCreationTime,
                    WinBase.FILETIME lpLastAccessTime, WinBase.FILETIME lpLastWriteTime);

    boolean SetFileAttributes(String lpFileName, DWORD dwFileAttributes);

    DWORD GetLogicalDriveStrings(DWORD nBufferLength, char[] lpBuffer);

    boolean GetDiskFreeSpace(String lpRootPathName,
                             DWORDByReference lpSectorsPerCluster,
                             DWORDByReference lpBytesPerSector,
                             DWORDByReference lpNumberOfFreeClusters,
                             DWORDByReference lpTotalNumberOfClusters);

    boolean GetDiskFreeSpaceEx(String lpDirectoryName,
                               LARGE_INTEGER lpFreeBytesAvailable,
                               LARGE_INTEGER lpTotalNumberOfBytes,
                               LARGE_INTEGER lpTotalNumberOfFreeBytes);

    public boolean DeleteFile(String filename);

    public boolean CreatePipe(HANDLEByReference hReadPipe,
                              HANDLEByReference hWritePipe,
                              WinBase.SECURITY_ATTRIBUTES lpPipeAttributes, int nSize);

    public boolean CallNamedPipe(String lpNamedPipeName,
                                 byte[] lpInBuffer, int nInBufferSize,
                                 byte[] lpOutBuffer, int nOutBufferSize,
                                 IntByReference lpBytesRead, int nTimeOut);

    public boolean ConnectNamedPipe(HANDLE hNamedPipe, OVERLAPPED lpOverlapped);

    int MAX_PIPE_NAME_LENGTH=256;

    public HANDLE CreateNamedPipe(String lpName, int dwOpenMode, int dwPipeMode, int nMaxInstances,
                                  int nOutBufferSize, int nInBufferSize, int nDefaultTimeOut,
                                  SECURITY_ATTRIBUTES lpSecurityAttributes);

    public boolean DisconnectNamedPipe(HANDLE hNamedPipe);

    public boolean GetNamedPipeClientComputerName(HANDLE Pipe, char[] ClientComputerName, int ClientComputerNameLength);

    public boolean GetNamedPipeClientProcessId(HANDLE Pipe, ULONGByReference ClientProcessId);

    public boolean GetNamedPipeClientSessionId(HANDLE Pipe, ULONGByReference ClientSessionId);

    public boolean GetNamedPipeHandleState(HANDLE hNamedPipe, IntByReference lpState,
                                           IntByReference lpCurInstances, IntByReference lpMaxCollectionCount,
                                           IntByReference lpCollectDataTimeout, char[] lpUserName, int nMaxUserNameSize);

    public boolean GetNamedPipeInfo(HANDLE hNamedPipe, IntByReference lpFlags,
                                    IntByReference lpOutBufferSize, IntByReference lpInBufferSize,
                                    IntByReference lpMaxInstances);

    public boolean GetNamedPipeServerProcessId(HANDLE Pipe, ULONGByReference ServerProcessId);

    public boolean GetNamedPipeServerSessionId(HANDLE Pipe, ULONGByReference ServerSessionId);

    public boolean PeekNamedPipe(HANDLE hNamedPipe, byte[] lpBuffer, int nBufferSize,
                                 IntByReference lpBytesRead,IntByReference lpTotalBytesAvail, IntByReference lpBytesLeftThisMessage);

    public boolean SetNamedPipeHandleState(HANDLE hNamedPipe, IntByReference lpMode,
                                           IntByReference lpMaxCollectionCount, IntByReference lpCollectDataTimeout);

    public boolean TransactNamedPipe(HANDLE hNamedPipe,
                                     byte[] lpInBuffer, int nInBufferSize,
                                     byte[] lpOutBuffer, int nOutBufferSize,
                                     IntByReference lpBytesRead, OVERLAPPED lpOverlapped);

    public boolean WaitNamedPipe(String lpNamedPipeName, int nTimeOut);

    boolean SetHandleInformation(HANDLE hObject, int dwMask, int dwFlags);

    public int GetFileAttributes(String lpFileName);

    public int GetFileType(HANDLE hFile);

    boolean DeviceIoControl(HANDLE hDevice, int dwIoControlCode,
                            Pointer lpInBuffer, int nInBufferSize, Pointer lpOutBuffer,
                            int nOutBufferSize, IntByReference lpBytesReturned,
                            Pointer lpOverlapped);

    HANDLE CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);

    boolean Process32First(HANDLE hSnapshot, Tlhelp32.PROCESSENTRY32 lppe);

    boolean Process32Next(HANDLE hSnapshot, Tlhelp32.PROCESSENTRY32 lppe);

    boolean Thread32First(HANDLE hSnapshot, Tlhelp32.THREADENTRY32 lpte);

    boolean Thread32Next(HANDLE hSnapshot, Tlhelp32.THREADENTRY32 lpte);

    boolean SetEnvironmentVariable(String lpName, String lpValue);

    int GetEnvironmentVariable(String lpName, char[] lpBuffer, int nSize);

    Pointer GetEnvironmentStrings();

    boolean FreeEnvironmentStrings(Pointer lpszEnvironmentBlock);

    LCID GetSystemDefaultLCID();

    LCID GetUserDefaultLCID();

    int GetPrivateProfileInt(String appName, String keyName, int defaultValue,
                             String fileName);

    DWORD GetPrivateProfileString(String lpAppName, String lpKeyName,
                                  String lpDefault, char[] lpReturnedString, DWORD nSize,
                                  String lpFileName);

    boolean WritePrivateProfileString(String lpAppName, String lpKeyName,
                                      String lpString, String lpFileName);

    DWORD GetPrivateProfileSection(String lpAppName, char[] lpReturnedString,
                                   DWORD nSize, String lpFileName);

    DWORD GetPrivateProfileSectionNames(char[] lpszReturnBuffer, DWORD nSize,
                                        String lpFileName);

    boolean WritePrivateProfileSection(String lpAppName, String lpString,
                                       String lpFileName);

    boolean FileTimeToLocalFileTime(FILETIME lpFileTime,
                                    FILETIME lpLocalFileTime);

    boolean SystemTimeToTzSpecificLocalTime(TIME_ZONE_INFORMATION lpTimeZone,
                                            SYSTEMTIME lpUniversalTime, SYSTEMTIME lpLocalTime);

    boolean SystemTimeToFileTime(SYSTEMTIME lpSystemTime, FILETIME lpFileTime);

    boolean FileTimeToSystemTime(FILETIME lpFileTime, SYSTEMTIME lpSystemTime);

    @Deprecated
    HANDLE CreateRemoteThread(HANDLE hProcess, WinBase.SECURITY_ATTRIBUTES lpThreadAttributes, int dwStackSize, FOREIGN_THREAD_START_ROUTINE lpStartAddress, Pointer lpParameter, DWORD dwCreationFlags, Pointer lpThreadId);

    HANDLE CreateRemoteThread(HANDLE hProcess, SECURITY_ATTRIBUTES lpThreadAttributes, int dwStackSize, Pointer lpStartAddress, Pointer lpParameter, int dwCreationFlags, DWORDByReference lpThreadId);

    boolean WriteProcessMemory(HANDLE hProcess, Pointer lpBaseAddress, Pointer lpBuffer, int nSize, IntByReference lpNumberOfBytesWritten);

    boolean ReadProcessMemory(HANDLE hProcess, Pointer lpBaseAddress, Pointer lpBuffer, int nSize, IntByReference lpNumberOfBytesRead);

    SIZE_T VirtualQueryEx(HANDLE hProcess, Pointer lpAddress, MEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);

    boolean DefineDosDevice(int dwFlags, String lpDeviceName, String lpTargetPath);

    int QueryDosDevice(String lpDeviceName, char[] lpTargetPath, int ucchMax);

    HANDLE FindFirstFile(String lpFileName, Pointer lpFindFileData);

    HANDLE FindFirstFileEx(String lpFileName, int fInfoLevelId, Pointer lpFindFileData, int fSearchOp, Pointer lpSearchFilter, DWORD dwAdditionalFlags);

    boolean FindNextFile(HANDLE hFindFile, Pointer lpFindFileData);

    boolean FindClose(HANDLE hFindFile);

    HANDLE FindFirstVolumeMountPoint(String lpszRootPathName, char[] lpszVolumeMountPoint, int cchBufferLength);

    boolean FindNextVolumeMountPoint(HANDLE hFindVolumeMountPoint, char[] lpszVolumeMountPoint, int cchBufferLength);

    boolean FindVolumeMountPointClose(HANDLE hFindVolumeMountPoint);

    boolean GetVolumeNameForVolumeMountPoint(String lpszVolumeMountPoint, char[] lpszVolumeName, int cchBufferLength);

    boolean SetVolumeLabel(String lpRootPathName, String lpVolumeName);

    boolean SetVolumeMountPoint(String lpszVolumeMountPoint, String lpszVolumeName);

    boolean DeleteVolumeMountPoint(String lpszVolumeMountPoint);

    boolean GetVolumeInformation(String lpRootPathName,
                                 char[] lpVolumeNameBuffer, int nVolumeNameSize,
                                 IntByReference lpVolumeSerialNumber,
                                 IntByReference lpMaximumComponentLength,
                                 IntByReference lpFileSystemFlags,
                                 char[] lpFileSystemNameBuffer, int nFileSystemNameSize);

    boolean GetVolumePathName(String lpszFileName, char[] lpszVolumePathName, int cchBufferLength);

    boolean GetVolumePathNamesForVolumeName(String lpszVolumeName,
                                            char[] lpszVolumePathNames, int cchBufferLength,
                                            IntByReference lpcchReturnLength);

    HANDLE FindFirstVolume(char[] lpszVolumeName, int cchBufferLength);

    boolean FindNextVolume(HANDLE hFindVolume, char[] lpszVolumeName, int cchBufferLength);

    boolean FindVolumeClose(HANDLE hFindVolume);

    boolean GetCommState(HANDLE hFile, WinBase.DCB lpDCB);

    boolean GetCommTimeouts(HANDLE hFile, WinBase.COMMTIMEOUTS lpCommTimeouts);

    boolean SetCommState(HANDLE hFile, WinBase.DCB lpDCB);

    boolean SetCommTimeouts(HANDLE hFile, WinBase.COMMTIMEOUTS lpCommTimeouts);

    boolean ProcessIdToSessionId(int dwProcessId, IntByReference pSessionId);

    HMODULE LoadLibraryEx(String lpFileName, HANDLE hFile, int flags);

    HRSRC FindResource(HMODULE hModule, Pointer name, Pointer type);

    HANDLE LoadResource(HMODULE hModule, HRSRC hResource);

    Pointer LockResource(HANDLE hResource);

    int SizeofResource(HMODULE hModule, HANDLE hResource);

    boolean FreeLibrary(HMODULE module);

    boolean EnumResourceTypes(HMODULE hModule, WinBase.EnumResTypeProc proc, Pointer lParam);

    boolean EnumResourceNames(HMODULE hModule, Pointer type, WinBase.EnumResNameProc proc, Pointer lParam);

    boolean Module32FirstW(HANDLE hSnapshot, Tlhelp32.MODULEENTRY32W lpme);

    boolean Module32NextW(HANDLE hSnapshot, Tlhelp32.MODULEENTRY32W lpme);

    int SetErrorMode(int umode);

    Pointer GetProcAddress(HMODULE hmodule, int ordinal) throws LastErrorException;

    int SetThreadExecutionState(int esFlags);

    int ExpandEnvironmentStrings(String lpSrc, Pointer lpDst, int nSize);

    boolean GetProcessTimes(HANDLE hProcess, FILETIME lpCreationTime, FILETIME lpExitTime, FILETIME lpKernelTime,
                            FILETIME lpUserTime);

    boolean GetProcessIoCounters(HANDLE hProcess, WinNT.IO_COUNTERS lpIoCounters);

    HANDLE CreateMutex(SECURITY_ATTRIBUTES lpMutexAttributes,
                       boolean bInitialOwner,
                       String lpName);

    HANDLE OpenMutex(int dwDesiredAccess,
                     boolean bInheritHandle,
                     String lpName);

    boolean ReleaseMutex(HANDLE handle);

    void ExitProcess(int exitCode);

    Pointer VirtualAllocEx(HANDLE hProcess, Pointer lpAddress, SIZE_T dwSize,
                           int flAllocationType, int flProtect);

    boolean GetExitCodeThread(HANDLE hThread, IntByReference exitCode);

    boolean VirtualFreeEx( HANDLE hProcess, Pointer lpAddress, SIZE_T dwSize, int dwFreeType);

    HRESULT RegisterApplicationRestart(char[] pwzCommandline, int dwFlags);

    HRESULT UnregisterApplicationRestart();

    HRESULT GetApplicationRestartSettings(HANDLE hProcess, char[] pwzCommandline, IntByReference pcchSize, IntByReference pdwFlags);
}
