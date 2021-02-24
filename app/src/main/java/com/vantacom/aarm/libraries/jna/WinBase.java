package com.vantacom.aarm.libraries.jna;

import java.text.DateFormat;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.sun.jna.Callback;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.Union;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.win32.StdCallLibrary.StdCallCallback;
import com.sun.jna.win32.W32APITypeMapper;

public interface WinBase extends WinDef, BaseTSD {
    WinNT.HANDLE INVALID_HANDLE_VALUE =
            new WinNT.HANDLE(Pointer.createConstant(Native.POINTER_SIZE == 8
                    ? -1 : 0xFFFFFFFFL));

    int WAIT_FAILED = 0xFFFFFFFF;
    int WAIT_OBJECT_0 = ((NTStatus.STATUS_WAIT_0 ) + 0 );
    int WAIT_ABANDONED = ((NTStatus.STATUS_ABANDONED_WAIT_0 ) + 0 );
    int WAIT_ABANDONED_0 = ((NTStatus.STATUS_ABANDONED_WAIT_0 ) + 0 );
    int MAX_COMPUTERNAME_LENGTH = Platform.isMac() ? 15 : 31;
    int LOGON32_LOGON_INTERACTIVE = 2;
    int LOGON32_LOGON_NETWORK = 3;
    int LOGON32_LOGON_BATCH = 4;
    int LOGON32_LOGON_SERVICE = 5;
    int LOGON32_LOGON_UNLOCK = 7;
    int LOGON32_LOGON_NETWORK_CLEARTEXT = 8;
    int LOGON32_LOGON_NEW_CREDENTIALS = 9;
    int LOGON32_PROVIDER_DEFAULT = 0;
    int LOGON32_PROVIDER_WINNT35 = 1;
    int LOGON32_PROVIDER_WINNT40 = 2;
    int LOGON32_PROVIDER_WINNT50 = 3;
    int HANDLE_FLAG_INHERIT = 1;
    int HANDLE_FLAG_PROTECT_FROM_CLOSE = 2;

    int STARTF_USESHOWWINDOW = 0x001;
    int STARTF_USESIZE = 0x002;
    int STARTF_USEPOSITION = 0x004;
    int STARTF_USECOUNTCHARS = 0x008;
    int STARTF_USEFILLATTRIBUTE = 0x010;
    int STARTF_RUNFULLSCREEN = 0x020;
    int STARTF_FORCEONFEEDBACK = 0x040;
    int STARTF_FORCEOFFFEEDBACK = 0x080;
    int STARTF_USESTDHANDLES = 0x100;

    int DEBUG_PROCESS = 0x00000001;
    int DEBUG_ONLY_THIS_PROCESS = 0x00000002;
    int CREATE_SUSPENDED = 0x00000004;
    int DETACHED_PROCESS = 0x00000008;
    int CREATE_NEW_CONSOLE = 0x00000010;
    int CREATE_NEW_PROCESS_GROUP = 0x00000200;
    int CREATE_UNICODE_ENVIRONMENT = 0x00000400;
    int CREATE_SEPARATE_WOW_VDM = 0x00000800;
    int CREATE_SHARED_WOW_VDM = 0x00001000;
    int CREATE_FORCEDOS = 0x00002000;
    int INHERIT_PARENT_AFFINITY = 0x00010000;
    int CREATE_PROTECTED_PROCESS = 0x00040000;
    int EXTENDED_STARTUPINFO_PRESENT = 0x00080000;
    int CREATE_BREAKAWAY_FROM_JOB = 0x01000000;
    int CREATE_PRESERVE_CODE_AUTHZ_LEVEL = 0x02000000;
    int CREATE_DEFAULT_ERROR_MODE = 0x04000000;
    int CREATE_NO_WINDOW = 0x08000000;

    /* File encryption status */
    int FILE_ENCRYPTABLE = 0;
    int FILE_IS_ENCRYPTED = 1;
    int FILE_SYSTEM_ATTR = 2;
    int FILE_ROOT_DIR = 3;
    int FILE_SYSTEM_DIR = 4;
    int FILE_UNKNOWN = 5;
    int FILE_SYSTEM_NOT_SUPPORT = 6;
    int FILE_USER_DISALLOWED = 7;
    int FILE_READ_ONLY = 8;
    int FILE_DIR_DISALOWED = 9;

    int CREATE_FOR_IMPORT = 1;
    int CREATE_FOR_DIR = 2;
    int OVERWRITE_HIDDEN = 4;

    int INVALID_FILE_SIZE           = 0xFFFFFFFF;
    int INVALID_SET_FILE_POINTER    = 0xFFFFFFFF;
    int INVALID_FILE_ATTRIBUTES     = 0xFFFFFFFF;

    int STILL_ACTIVE = WinNT.STATUS_PENDING;

    int FileBasicInfo                   = 0;
    int FileStandardInfo                = 1;
    int FileNameInfo                    = 2;
    int FileRenameInfo                  = 3;
    int FileDispositionInfo             = 4;
    int FileAllocationInfo              = 5;
    int FileEndOfFileInfo               = 6;
    int FileStreamInfo                  = 7;
    int FileCompressionInfo             = 8;
    int FileAttributeTagInfo            = 9;
    int FileIdBothDirectoryInfo         = 10; // 0xA
    int FileIdBothDirectoryRestartInfo  = 11; // 0xB
    int FileIoPriorityHintInfo          = 12; // 0xC
    int FileRemoteProtocolInfo          = 13; // 0xD
    int FileFullDirectoryInfo           = 14; // 0xE
    int FileFullDirectoryRestartInfo    = 15; // 0xF
    int FileStorageInfo                 = 16; // 0x10
    int FileAlignmentInfo               = 17; // 0x11
    int FileIdInfo                      = 18; // 0x12
    int FileIdExtdDirectoryInfo         = 19; // 0x13
    int FileIdExtdDirectoryRestartInfo  = 20; // 0x14

    public static class FILE_BASIC_INFO extends Structure {
        public static class ByReference extends FILE_BASIC_INFO implements Structure.ByReference {
            public ByReference() {
            }

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("CreationTime", "LastAccessTime", "LastWriteTime", "ChangeTime", "FileAttributes");
        }

        public WinNT.LARGE_INTEGER CreationTime;
        public WinNT.LARGE_INTEGER LastAccessTime;
        public WinNT.LARGE_INTEGER LastWriteTime;
        public WinNT.LARGE_INTEGER ChangeTime;
        public int FileAttributes;

        public static int sizeOf()
        {
            return Native.getNativeSize(FILE_BASIC_INFO.class, null);
        }

        public FILE_BASIC_INFO() {
            super();
        }

        public FILE_BASIC_INFO(Pointer memory) {
            super(memory);
            read();
            this.CreationTime = new WinNT.LARGE_INTEGER(this.CreationTime.getValue());
            this.LastAccessTime = new WinNT.LARGE_INTEGER(this.LastAccessTime.getValue());
            this.LastWriteTime = new WinNT.LARGE_INTEGER(this.LastWriteTime.getValue());
            this.ChangeTime = new WinNT.LARGE_INTEGER(this.ChangeTime.getValue());
        }

        public FILE_BASIC_INFO(FILETIME CreationTime,
                               FILETIME LastAccessTime,
                               FILETIME LastWriteTime,
                               FILETIME ChangeTime,
                               int FileAttributes) {
            this.CreationTime = new WinNT.LARGE_INTEGER(CreationTime.toTime());
            this.LastAccessTime = new WinNT.LARGE_INTEGER(LastAccessTime.toTime());
            this.LastWriteTime = new WinNT.LARGE_INTEGER(LastWriteTime.toTime());
            this.ChangeTime = new WinNT.LARGE_INTEGER(ChangeTime.toTime());
            this.FileAttributes = FileAttributes;
            write();
        }

        public FILE_BASIC_INFO(WinNT.LARGE_INTEGER CreationTime,
                               WinNT.LARGE_INTEGER LastAccessTime,
                               WinNT.LARGE_INTEGER LastWriteTime,
                               WinNT.LARGE_INTEGER ChangeTime,
                               int FileAttributes) {
            this.CreationTime = CreationTime;
            this.LastAccessTime = LastAccessTime;
            this.LastWriteTime = LastWriteTime;
            this.ChangeTime = ChangeTime;
            this.FileAttributes = FileAttributes;
            write();
        }
    }

    public static class FILE_STANDARD_INFO extends Structure {
        public static class ByReference extends FILE_STANDARD_INFO implements Structure.ByReference {
            public ByReference() {
            }

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("AllocationSize", "EndOfFile", "NumberOfLinks", "DeletePending", "Directory");
        }

        public WinNT.LARGE_INTEGER AllocationSize;
        public WinNT.LARGE_INTEGER EndOfFile;
        public int NumberOfLinks;
        public boolean DeletePending;
        public boolean Directory;

        public static int sizeOf()
        {
            return Native.getNativeSize(FILE_STANDARD_INFO.class, null);
        }

        public FILE_STANDARD_INFO() {
            super();
        }

        public FILE_STANDARD_INFO(Pointer memory) {
            super(memory);
            read();
        }

        public FILE_STANDARD_INFO(WinNT.LARGE_INTEGER AllocationSize,
                                  WinNT.LARGE_INTEGER EndOfFile,
                                  int NumberOfLinks,
                                  boolean DeletePending,
                                  boolean Directory) {
            this.AllocationSize = AllocationSize;
            this.EndOfFile = EndOfFile;
            this.NumberOfLinks = NumberOfLinks;
            this.DeletePending = DeletePending;
            this.Directory = Directory;
            write();
        }
    }

    public static class FILE_DISPOSITION_INFO extends Structure {
        public static class ByReference extends FILE_DISPOSITION_INFO  implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("DeleteFile");
        }

        public boolean DeleteFile;

        public static int sizeOf()
        {
            return Native.getNativeSize(FILE_DISPOSITION_INFO.class, null);
        }

        public FILE_DISPOSITION_INFO () {
            super();
        }

        public FILE_DISPOSITION_INFO (Pointer memory) {
            super(memory);
            read();
        }

        public FILE_DISPOSITION_INFO (boolean DeleteFile) {
            this.DeleteFile = DeleteFile;
            write();
        }
    }

    public static class FILE_COMPRESSION_INFO extends Structure {
        public static class ByReference extends FILE_COMPRESSION_INFO implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("CompressedFileSize", "CompressionFormat", "CompressionUnitShift", "ChunkShift", "ClusterShift", "Reserved");
        }

        public WinNT.LARGE_INTEGER CompressedFileSize;
        public short CompressionFormat;
        public byte CompressionUnitShift;
        public byte ChunkShift;
        public byte ClusterShift;
        public byte[] Reserved = new byte[3];

        public static int sizeOf()
        {
            return Native.getNativeSize(FILE_COMPRESSION_INFO.class, null);
        }

        public FILE_COMPRESSION_INFO() {
            super(W32APITypeMapper.DEFAULT);
        }

        public FILE_COMPRESSION_INFO(Pointer memory) {
            super(memory, Structure.ALIGN_DEFAULT, W32APITypeMapper.DEFAULT);
            read();
        }

        public FILE_COMPRESSION_INFO(WinNT.LARGE_INTEGER CompressedFileSize,
                                     short CompressionFormat,
                                     byte CompressionUnitShift,
                                     byte ChunkShift,
                                     byte ClusterShift) {
            this.CompressedFileSize = CompressedFileSize;
            this.CompressionFormat = CompressionFormat;
            this.CompressionUnitShift = CompressionUnitShift;
            this.ChunkShift = ChunkShift;
            this.ClusterShift = ClusterShift;
            this.Reserved = new byte[3];
            write();
        }
    }

    public static class FILE_ATTRIBUTE_TAG_INFO extends Structure {
        public static class ByReference extends FILE_ATTRIBUTE_TAG_INFO implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("FileAttributes", "ReparseTag");
        }

        public int FileAttributes;
        public int ReparseTag;

        public static int sizeOf()
        {
            return Native.getNativeSize(FILE_ATTRIBUTE_TAG_INFO.class, null);
        }

        public FILE_ATTRIBUTE_TAG_INFO() {
            super();
        }

        public FILE_ATTRIBUTE_TAG_INFO(Pointer memory) {
            super(memory);
            read();
        }

        public FILE_ATTRIBUTE_TAG_INFO(int FileAttributes,
                                       int ReparseTag) {
            this.FileAttributes = FileAttributes;
            this.ReparseTag = ReparseTag;
            write();
        }
    }

    public static class FILE_ID_INFO extends Structure {
        public static class ByReference extends FILE_ID_INFO implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("VolumeSerialNumber", "FileId");
        }

        public static class FILE_ID_128 extends Structure {
            public BYTE[] Identifier = new BYTE[16];

            public FILE_ID_128() {
                super();
            }

            public FILE_ID_128(Pointer memory) {
                super(memory);
                read();
            }

            public FILE_ID_128(BYTE[] Identifier) {
                this.Identifier = Identifier;
                write();
            }

            @Override
            protected List<String> getFieldOrder() {
                return Arrays.asList("Identifier");
            }
        }

        public long VolumeSerialNumber;
        public FILE_ID_128 FileId;

        public static int sizeOf()
        {
            return Native.getNativeSize(FILE_ID_INFO.class, null);
        }

        public FILE_ID_INFO() {
            super();
        }

        public FILE_ID_INFO(Pointer memory) {
            super(memory);
            read();
        }

        public FILE_ID_INFO(long VolumeSerialNumber,
                            FILE_ID_128 FileId) {
            this.VolumeSerialNumber = VolumeSerialNumber;
            this.FileId = FileId;
            write();
        }
    }

    int FindExInfoStandard = 0;
    int FindExInfoBasic = 1;
    int FindExInfoMaxInfoLevel = 2;
    int FindExSearchNameMatch = 0;
    int FindExSearchLimitToDirectories = 1;
    int FindExSearchLimitToDevices = 2;

    public static class WIN32_FIND_DATA extends Structure {
        public static class ByReference extends WIN32_FIND_DATA implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwFileAttributes", "ftCreationTime", "ftLastAccessTime", "ftLastWriteTime", "nFileSizeHigh", "nFileSizeLow", "dwReserved0", "dwReserved1", "cFileName", "cAlternateFileName");
        }

        public int dwFileAttributes;
        public FILETIME ftCreationTime;
        public FILETIME ftLastAccessTime;
        public FILETIME ftLastWriteTime;
        public int nFileSizeHigh;
        public int nFileSizeLow;
        public int dwReserved0;
        public int dwReserved1;
        public char[] cFileName = new char[MAX_PATH];
        public char[] cAlternateFileName = new char[14];

        public static int sizeOf() {
            return Native.getNativeSize(WIN32_FIND_DATA.class, null);
        }

        public WIN32_FIND_DATA() {
            super(W32APITypeMapper.DEFAULT);
        }

        public WIN32_FIND_DATA(Pointer memory) {
            super(memory, Structure.ALIGN_DEFAULT, W32APITypeMapper.DEFAULT);
            read();
        }

        public WIN32_FIND_DATA(int dwFileAttributes,
                               FILETIME ftCreationTime,
                               FILETIME ftLastAccessTime,
                               FILETIME ftLastWriteTime,
                               int nFileSizeHigh,
                               int nFileSizeLow,
                               int dwReserved0,
                               int dwReserved1,
                               char[] cFileName,
                               char[] cAlternateFileName) {
            this.dwFileAttributes = dwFileAttributes;
            this.ftCreationTime = ftCreationTime;
            this.ftLastAccessTime = ftLastAccessTime;
            this.ftLastWriteTime = ftLastWriteTime;
            this.nFileSizeHigh = nFileSizeHigh;
            this.nFileSizeLow = nFileSizeLow;
            this.dwReserved0 = dwReserved0;
            this.cFileName = cFileName;
            this.cAlternateFileName = cAlternateFileName;
            write();
        }

        public String getFileName() {
            return Native.toString(cFileName);
        }

        public String getAlternateFileName() {
            return Native.toString(cAlternateFileName);
        }
    }

    public static class FILETIME extends Structure {
        public int dwLowDateTime;
        public int dwHighDateTime;

        public static class ByReference extends FILETIME implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwLowDateTime", "dwHighDateTime");
        }

        public FILETIME(Date date) {
            long rawValue = dateToFileTime(date);
            dwHighDateTime = (int)(rawValue >> 32 & 0xffffffffL);
            dwLowDateTime = (int)(rawValue & 0xffffffffL);
        }

        public FILETIME(WinNT.LARGE_INTEGER ft) {
            dwHighDateTime = ft.getHigh().intValue();
            dwLowDateTime = ft.getLow().intValue();
        }

        public FILETIME() {
        }

        public FILETIME(Pointer memory) {
            super(memory);
            read();
        }

        private static final long EPOCH_DIFF = 11644473600000L;

        public static Date filetimeToDate(final int high, final int low) {
            final long filetime = (long) high << 32 | low & 0xffffffffL;
            final long ms_since_16010101 = filetime / (1000 * 10);
            final long ms_since_19700101 = ms_since_16010101 - EPOCH_DIFF;
            return new Date(ms_since_19700101);
        }

        public static long dateToFileTime(final Date date) {
            final long ms_since_19700101 = date.getTime();
            final long ms_since_16010101 = ms_since_19700101 + EPOCH_DIFF;
            return ms_since_16010101 * 1000 * 10;
        }

        public Date toDate() {
            return filetimeToDate(dwHighDateTime, dwLowDateTime);
        }

        public long toTime() {
            return toDate().getTime();
        }

        public DWORDLONG toDWordLong() {
            return new DWORDLONG((long) dwHighDateTime << 32 | dwLowDateTime & 0xffffffffL);
        }

        @Override
        public String toString() {
            return super.toString() + ": " + toDate().toString(); //$NON-NLS-1$
        }
    }

    int  LMEM_FIXED = 0x0000;
    int  LMEM_MOVEABLE = 0x0002;
    int  LMEM_NOCOMPACT = 0x0010;
    int  LMEM_NODISCARD = 0x0020;
    int  LMEM_ZEROINIT = 0x0040;
    int  LMEM_MODIFY = 0x0080;
    int  LMEM_DISCARDABLE = 0x0F00;
    int  LMEM_VALID_FLAGS = 0x0F72;
    int  LMEM_INVALID_HANDLE = 0x8000;

    int  LHND = (LMEM_MOVEABLE | LMEM_ZEROINIT);
    int  LPTR = (LMEM_FIXED | LMEM_ZEROINIT);

    int  LMEM_DISCARDED = 0x4000;
    int  LMEM_LOCKCOUNT = 0x00FF;

    public static class SYSTEMTIME extends Structure {
        public short wYear;
        public short wMonth;
        public short wDayOfWeek;
        public short wDay;
        public short wHour;
        public short wMinute;
        public short wSecond;
        public short wMilliseconds;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("wYear", "wMonth", "wDayOfWeek", "wDay", "wHour", "wMinute", "wSecond", "wMilliseconds");
        }

        public SYSTEMTIME() {
            super();
        }

        public SYSTEMTIME(Date date) {
            this(date.getTime());
        }

        public SYSTEMTIME(long timestamp) {
            Calendar cal = Calendar.getInstance();
            cal.setTimeInMillis(timestamp);
            fromCalendar(cal);
        }

        public SYSTEMTIME(Calendar cal) {
            fromCalendar(cal);
        }

        public void fromCalendar(Calendar cal) {
            wYear = (short) cal.get(Calendar.YEAR);
            wMonth = (short) (1 + cal.get(Calendar.MONTH) - Calendar.JANUARY);  // 1 = January
            wDay = (short) cal.get(Calendar.DAY_OF_MONTH);
            wHour = (short) cal.get(Calendar.HOUR_OF_DAY);
            wMinute = (short) cal.get(Calendar.MINUTE);
            wSecond = (short) cal.get(Calendar.SECOND);
            wMilliseconds = (short) cal.get(Calendar.MILLISECOND);
            wDayOfWeek = (short) (cal.get(Calendar.DAY_OF_WEEK) - Calendar.SUNDAY); // 0 = Sunday
        }

        public Calendar toCalendar() {
            Calendar cal = Calendar.getInstance();
            cal.set(Calendar.YEAR, wYear);
            cal.set(Calendar.MONTH, Calendar.JANUARY + (wMonth - 1));
            cal.set(Calendar.DAY_OF_MONTH, wDay);
            cal.set(Calendar.HOUR_OF_DAY, wHour);
            cal.set(Calendar.MINUTE, wMinute);
            cal.set(Calendar.SECOND, wSecond);
            cal.set(Calendar.MILLISECOND, wMilliseconds);
            return cal;
        }

        @Override
        public String toString() {
            if ((wYear == 0) && (wMonth == 0) && (wDay == 0)
                    && (wHour == 0) && (wMinute == 0) && (wSecond == 0)
                    && (wMilliseconds == 0)) {
                return super.toString();
            }

            DateFormat dtf = DateFormat.getDateTimeInstance();
            Calendar cal = toCalendar();
            return dtf.format(cal.getTime());
        }
    }

    public static class TIME_ZONE_INFORMATION extends Structure {
        public LONG       Bias;
        public String      StandardName;
        public SYSTEMTIME StandardDate;
        public LONG       StandardBias;
        public String      DaylightName;
        public SYSTEMTIME DaylightDate;
        public LONG       DaylightBias;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Bias", "StandardName", "StandardDate", "StandardBias", "DaylightName", "DaylightDate", "DaylightBias");
        }

        public TIME_ZONE_INFORMATION() {
            super(W32APITypeMapper.DEFAULT);
        }
    }

    int FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x00000100;
    int FORMAT_MESSAGE_IGNORE_INSERTS  = 0x00000200;
    int FORMAT_MESSAGE_FROM_STRING     = 0x00000400;
    int FORMAT_MESSAGE_FROM_HMODULE    = 0x00000800;
    int FORMAT_MESSAGE_FROM_SYSTEM     = 0x00001000;
    int FORMAT_MESSAGE_ARGUMENT_ARRAY  = 0x00002000;

    int DRIVE_UNKNOWN = 0;
    int DRIVE_NO_ROOT_DIR = 1;
    int DRIVE_REMOVABLE = 2;
    int DRIVE_FIXED = 3;
    int DRIVE_REMOTE = 4;
    int DRIVE_CDROM = 5;
    int DRIVE_RAMDISK = 6;

    public static class OVERLAPPED extends Structure {
        public ULONG_PTR Internal;
        public ULONG_PTR InternalHigh;
        public int Offset;
        public int OffsetHigh;
        public WinNT.HANDLE hEvent;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Internal", "InternalHigh", "Offset", "OffsetHigh", "hEvent");
        }
    }

    int INFINITE = 0xFFFFFFFF;

    public static class SYSTEM_INFO extends Structure {
        public static class PI extends Structure {
            public static class ByReference extends PI implements Structure.ByReference {}

            public WORD wProcessorArchitecture;
            public WORD wReserved;

            @Override
            protected List<String> getFieldOrder() {
                return Arrays.asList("wProcessorArchitecture", "wReserved");
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("processorArchitecture", "dwPageSize", "lpMinimumApplicationAddress", "lpMaximumApplicationAddress", "dwActiveProcessorMask", "dwNumberOfProcessors", "dwProcessorType", "dwAllocationGranularity", "wProcessorLevel", "wProcessorRevision");
        }

        public static class UNION extends Union {

            public static class ByReference extends UNION implements Structure.ByReference {

            }

            public DWORD dwOemID;
            public PI pi;

            @Override
            public void read() {
                setType("dwOemID");
                super.read();
                setType("pi");
                super.read();
            }
        }

        public UNION processorArchitecture;
        public DWORD dwPageSize;
        public Pointer lpMinimumApplicationAddress;
        public Pointer lpMaximumApplicationAddress;
        public DWORD_PTR dwActiveProcessorMask;
        public DWORD dwNumberOfProcessors;
        public DWORD dwProcessorType;
        public DWORD dwAllocationGranularity;
        public WORD wProcessorLevel;
        public WORD wProcessorRevision;
    }

    public static class MEMORYSTATUSEX extends Structure {
        public DWORD dwLength;
        public DWORD dwMemoryLoad;
        public DWORDLONG ullTotalPhys;
        public DWORDLONG ullAvailPhys;
        public DWORDLONG ullTotalPageFile;
        public DWORDLONG ullAvailPageFile;
        public DWORDLONG ullTotalVirtual;
        public DWORDLONG ullAvailVirtual;
        public DWORDLONG ullAvailExtendedVirtual;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwLength", "dwMemoryLoad", "ullTotalPhys", "ullAvailPhys", "ullTotalPageFile", "ullAvailPageFile", "ullTotalVirtual", "ullAvailVirtual", "ullAvailExtendedVirtual");
        }

        public MEMORYSTATUSEX() {
            dwLength = new DWORD(size());
        }
    }

    public static class SECURITY_ATTRIBUTES extends Structure {
        public DWORD dwLength;
        public Pointer lpSecurityDescriptor;
        public boolean bInheritHandle;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwLength", "lpSecurityDescriptor", "bInheritHandle");
        }

        public SECURITY_ATTRIBUTES() {
            dwLength = new DWORD(size());
        }
    }

    public static class STARTUPINFO extends Structure {
        public DWORD cb;
        public String lpReserved;
        public String lpDesktop;
        public String lpTitle;
        public DWORD dwX;
        public DWORD dwY;
        public DWORD dwXSize;
        public DWORD dwYSize;
        public DWORD dwXCountChars;
        public DWORD dwYCountChars;
        public DWORD dwFillAttribute;
        public int dwFlags;
        public WORD wShowWindow;
        public WORD cbReserved2;
        public ByteByReference lpReserved2;
        public WinNT.HANDLE hStdInput;
        public WinNT.HANDLE hStdOutput;
        public WinNT.HANDLE hStdError;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cb", "lpReserved", "lpDesktop", "lpTitle", "dwX", "dwY", "dwXSize", "dwYSize", "dwXCountChars", "dwYCountChars", "dwFillAttribute", "dwFlags", "wShowWindow", "cbReserved2", "lpReserved2", "hStdInput", "hStdOutput", "hStdError");
        }

        public STARTUPINFO() {
            super(W32APITypeMapper.DEFAULT);
            cb = new DWORD(size());
        }
    }

    public static class PROCESS_INFORMATION extends Structure {
        public WinNT.HANDLE hProcess;
        public WinNT.HANDLE hThread;
        public DWORD dwProcessId;
        public DWORD dwThreadId;

        public static class ByReference extends PROCESS_INFORMATION implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("hProcess", "hThread", "dwProcessId", "dwThreadId");
        }

        public PROCESS_INFORMATION() {
        }

        public PROCESS_INFORMATION(Pointer memory) {
            super(memory);
            read();
        }
    }

    int MOVEFILE_COPY_ALLOWED = 0x2;
    int MOVEFILE_CREATE_HARDLINK = 0x10;
    int MOVEFILE_DELAY_UNTIL_REBOOT = 0x4;
    int MOVEFILE_FAIL_IF_NOT_TRACKABLE = 0x20;
    int MOVEFILE_REPLACE_EXISTING = 0x1;
    int MOVEFILE_WRITE_THROUGH = 0x8;

    public interface THREAD_START_ROUTINE extends StdCallCallback{
        public DWORD apply( LPVOID lpParameter );
    }

    public class FOREIGN_THREAD_START_ROUTINE extends Structure {
        public LPVOID foreignLocation;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("foreignLocation");
        }
    }

    public static interface COMPUTER_NAME_FORMAT {
        int ComputerNameNetBIOS = 0;
        int ComputerNameDnsHostname = 1;
        int ComputerNameDnsDomain = 2;
        int ComputerNameDnsFullyQualified = 3;
        int ComputerNamePhysicalNetBIOS = 4;
        int ComputerNamePhysicalDnsHostname = 5;
        int ComputerNamePhysicalDnsDomain = 6;
        int ComputerNamePhysicalDnsFullyQualified = 7;
        int ComputerNameMax = 8;
    }

    public interface FE_EXPORT_FUNC extends StdCallCallback {
        public DWORD callback(Pointer pbData, Pointer pvCallbackContext,
                              ULONG ulLength);
    }

    public interface FE_IMPORT_FUNC extends StdCallCallback {
        public DWORD callback(Pointer pbData, Pointer pvCallbackContext,
                              ULONGByReference ulLength);
    }

    int PIPE_CLIENT_END=0x00000000;
    int PIPE_SERVER_END=0x00000001;

    int PIPE_ACCESS_DUPLEX=0x00000003;
    int PIPE_ACCESS_INBOUND=0x00000001;
    int PIPE_ACCESS_OUTBOUND=0x00000002;

    int PIPE_TYPE_BYTE=0x00000000;
    int PIPE_TYPE_MESSAGE=0x00000004;

    int PIPE_READMODE_BYTE=0x00000000;
    int PIPE_READMODE_MESSAGE=0x00000002;

    int PIPE_WAIT=0x00000000;
    int PIPE_NOWAIT=0x00000001;

    int PIPE_ACCEPT_REMOTE_CLIENTS=0x00000000;
    int PIPE_REJECT_REMOTE_CLIENTS=0x00000008;

    int PIPE_UNLIMITED_INSTANCES=255;
    int NMPWAIT_USE_DEFAULT_WAIT=0x00000000;
    int NMPWAIT_NOWAIT=0x00000001;
    int NMPWAIT_WAIT_FOREVER=0xffffffff;

    public static class COMMTIMEOUTS extends Structure {
        public DWORD ReadIntervalTimeout;
        public DWORD ReadTotalTimeoutMultiplier;
        public DWORD ReadTotalTimeoutConstant;
        public DWORD WriteTotalTimeoutMultiplier;
        public DWORD WriteTotalTimeoutConstant;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("ReadIntervalTimeout", "ReadTotalTimeoutMultiplier",
                    "ReadTotalTimeoutConstant", "WriteTotalTimeoutMultiplier",
                    "WriteTotalTimeoutConstant");
        }
    }

    public static class DCB extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("DCBlength", "BaudRate", "controllBits", "wReserved", "XonLim",
                    "XoffLim", "ByteSize", "Parity", "StopBits", "XonChar", "XoffChar",
                    "ErrorChar", "EofChar", "EvtChar", "wReserved1");
        }

        public static class DCBControllBits extends DWORD {
            private static final long serialVersionUID = 8574966619718078579L;

            @Override
            public String toString() {
                final StringBuilder stringBuilder = new StringBuilder();
                stringBuilder.append('<');
                stringBuilder.append("fBinary:1=");
                stringBuilder.append(getfBinary() ? '1' : '0');
                stringBuilder.append(", fParity:1=");
                stringBuilder.append(getfParity() ? '1' : '0');
                stringBuilder.append(", fOutxCtsFlow:1=");
                stringBuilder.append(getfOutxCtsFlow() ? '1' : '0');
                stringBuilder.append(", fOutxDsrFlow:1=");
                stringBuilder.append(getfOutxDsrFlow() ? '1' : '0');
                stringBuilder.append(", fDtrControl:2=");
                stringBuilder.append(getfDtrControl());
                stringBuilder.append(", fDsrSensitivity:1=");
                stringBuilder.append(getfDsrSensitivity() ? '1' : '0');
                stringBuilder.append(", fTXContinueOnXoff:1=");
                stringBuilder.append(getfTXContinueOnXoff() ? '1' : '0');
                stringBuilder.append(", fOutX:1=");
                stringBuilder.append(getfOutX() ? '1' : '0');
                stringBuilder.append(", fInX:1=");
                stringBuilder.append(getfInX() ? '1' : '0');
                stringBuilder.append(", fErrorChar:1=");
                stringBuilder.append(getfErrorChar() ? '1' : '0');
                stringBuilder.append(", fNull:1=");
                stringBuilder.append(getfNull() ? '1' : '0');
                stringBuilder.append(", fRtsControl:2=");
                stringBuilder.append(getfRtsControl());
                stringBuilder.append(", fAbortOnError:1=");
                stringBuilder.append(getfAbortOnError() ? '1' : '0');
                stringBuilder.append(", fDummy2:17=");
                stringBuilder.append(getfDummy2());
                stringBuilder.append('>');
                return stringBuilder.toString();
            }

            public boolean getfAbortOnError() {
                return (this.intValue() & (0x01 << 14)) != 0x00;
            }

            public boolean getfBinary() {
                return (this.intValue() & 0x01) != 0x00;
            }

            public boolean getfDsrSensitivity() {
                return (this.intValue() & (0x01 << 6)) != 0x00;
            }

            public int getfDtrControl() {
                return (this.intValue() >>> 4) & 0x03;
            }

            public boolean getfErrorChar() {
                return (this.intValue() & (0x01 << 10)) != 0x00;
            }

            public boolean getfInX() {
                return (this.intValue() & (0x01 << 9)) != 0x00;
            }

            public boolean getfNull() {
                return (this.intValue() & (0x01 << 11)) != 0x00;
            }

            public boolean getfOutX() {
                return (this.intValue() & (0x01 << 8)) != 0x00;
            }

            public boolean getfOutxCtsFlow() {
                return (this.intValue() & (0x01 << 2)) != 0x00;
            }

            public boolean getfOutxDsrFlow() {
                return (this.intValue() & (0x01 << 3)) != 0x00;
            }

            public boolean getfParity() {
                return (this.intValue() & (0x01 << 1)) != 0x00;
            }

            public int getfRtsControl() {
                return (this.intValue() >>> 12) & 0x03;
            }

            public int getfDummy2() {
                return (this.intValue()>>>15) & 0x1FFFF;
            }

            public boolean getfTXContinueOnXoff() {
                return (this.intValue() & (0x01 << 7)) != 0x00;
            }

            public void setfAbortOnError(boolean fAbortOnError) {
                int tmp = leftShiftMask(fAbortOnError ? 1 : 0, (byte)14, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfBinary(boolean fBinary) {
                int tmp = leftShiftMask(fBinary ? 1 : 0, (byte)0, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfDsrSensitivity(boolean fDsrSensitivity) {
                int tmp = leftShiftMask(fDsrSensitivity ? 1 : 0, (byte)6, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfDtrControl(int fOutxDsrFlow) {
                int tmp = leftShiftMask(fOutxDsrFlow, (byte)4, 0x03, this.intValue());
                this.setValue(tmp);
            }

            public void setfErrorChar(boolean fErrorChar) {
                int tmp = leftShiftMask(fErrorChar ? 1 : 0, (byte)10, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfInX(boolean fInX) {
                int tmp = leftShiftMask(fInX ? 1 : 0, (byte)9, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfNull(boolean fNull) {
                int tmp = leftShiftMask(fNull ? 1 : 0, (byte)11, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfOutX(boolean fOutX) {
                int tmp = leftShiftMask(fOutX ? 1 : 0, (byte)8, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfOutxCtsFlow(boolean fOutxCtsFlow) {
                int tmp = leftShiftMask(fOutxCtsFlow ? 1 : 0, (byte)2, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfOutxDsrFlow(boolean fOutxDsrFlow) {
                int tmp = leftShiftMask(fOutxDsrFlow ? 1 : 0, (byte)3, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfParity(boolean fParity) {
                int tmp = leftShiftMask(fParity ? 1 : 0, (byte)1, 0x01, this.intValue());
                this.setValue(tmp);
            }

            public void setfRtsControl(int fRtsControl) {
                int tmp = leftShiftMask(fRtsControl, (byte)12, 0x03, this.intValue());
                this.setValue(tmp);
            }

            public void setfTXContinueOnXoff(boolean fTXContinueOnXoff) {
                int tmp = leftShiftMask(fTXContinueOnXoff ? 1 : 0, (byte)7, 0x01, this.intValue());
                this.setValue(tmp);
            }


            private static  int leftShiftMask(int valuetoset, byte shift, int mask, int storage) {
                int tmp = storage;
                tmp &= ~(mask << shift);
                tmp |= ((valuetoset & mask) << shift);
                return tmp;
            }
        }
        public DWORD DCBlength;
        public DWORD BaudRate;
        public DCBControllBits controllBits;
        public WORD wReserved;
        public WORD XonLim;
        public WORD XoffLim;
        public BYTE ByteSize;
        public BYTE Parity;
        public BYTE StopBits;
        public char XonChar;
        public char XoffChar;
        public char ErrorChar;
        public char EofChar;
        public char EvtChar;
        public WORD wReserved1;

        public DCB() {
            DCBlength = new DWORD(size());
        }
    }

    int NOPARITY = 0;
    int ODDPARITY = 1;
    int EVENPARITY = 2;
    int MARKPARITY = 3;
    int SPACEPARITY = 4;
    int ONESTOPBIT = 0;
    int ONE5STOPBITS = 1;
    int TWOSTOPBITS = 2;
    int CBR_110 = 110;
    int CBR_300 = 300;
    int CBR_600 = 600;
    int CBR_1200 = 1200;
    int CBR_2400 = 2400;
    int CBR_4800 = 4800;
    int CBR_9600 = 9600;
    int CBR_14400 = 14400;
    int CBR_19200 = 19200;
    int CBR_38400 = 38400;
    int CBR_56000 = 56000;
    int CBR_128000 = 128000;
    int CBR_256000 = 256000;
    int DTR_CONTROL_DISABLE = 0;
    int DTR_CONTROL_ENABLE = 1;
    int DTR_CONTROL_HANDSHAKE = 2;
    int RTS_CONTROL_DISABLE = 0;
    int RTS_CONTROL_ENABLE = 1;
    int RTS_CONTROL_HANDSHAKE = 2;
    int RTS_CONTROL_TOGGLE = 3;

    interface EnumResTypeProc extends Callback {
        boolean invoke(HMODULE module, Pointer type, Pointer lParam);
    }

    interface EnumResNameProc extends Callback {
        boolean invoke(HMODULE module, Pointer type, Pointer name, Pointer lParam);
    }

    int ES_AWAYMODE_REQUIRED = 0x00000040;
    int ES_CONTINUOUS = 0x80000000;
    int ES_DISPLAY_REQUIRED = 0x00000002;
    int ES_SYSTEM_REQUIRED = 0x00000001;
    int ES_USER_PRESENT = 0x00000004;
    int MUTEX_MODIFY_STATE = WinNT.MUTANT_QUERY_STATE;
    int MUTEX_ALL_ACCESS = WinNT.MUTANT_ALL_ACCESS;
}
