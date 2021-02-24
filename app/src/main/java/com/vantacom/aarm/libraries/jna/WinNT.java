package com.vantacom.aarm.libraries.jna;

import com.sun.jna.FromNativeContext;
import com.sun.jna.IntegerType;
import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import com.sun.jna.Structure;
import com.sun.jna.Union;
import com.sun.jna.ptr.ByReference;
import com.sun.jna.win32.StdCallLibrary.StdCallCallback;

import java.util.Arrays;
import java.util.List;


@SuppressWarnings("serial")
public interface WinNT extends WinError, WinDef, WinBase, BaseTSD {
    int MINCHAR     = 0x80;
    int MAXCHAR     = 0x7f;
    int MINSHORT    = 0x8000;
    int MAXSHORT    = 0x7fff;
    int MINLONG     = 0x80000000;
    int MAXLONG     = 0x7fffffff;
    int MAXBYTE     = 0xff;
    int MAXWORD     = 0xffff;
    int MAXDWORD    = 0xffffffff;

    int DELETE = 0x00010000;
    int READ_CONTROL = 0x00020000;
    int WRITE_DAC = 0x00040000;
    int WRITE_OWNER = 0x00080000;
    int SYNCHRONIZE = 0x00100000;

    int STANDARD_RIGHTS_REQUIRED = 0x000F0000;
    int STANDARD_RIGHTS_READ = READ_CONTROL;
    int STANDARD_RIGHTS_WRITE = READ_CONTROL;
    int STANDARD_RIGHTS_EXECUTE = READ_CONTROL;
    int STANDARD_RIGHTS_ALL = 0x001F0000;

    int SPECIFIC_RIGHTS_ALL = 0x0000FFFF;

    int MUTANT_QUERY_STATE = 0x0001;
    int MUTANT_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | MUTANT_QUERY_STATE;

    int TOKEN_ASSIGN_PRIMARY = 0x0001;

    int TOKEN_DUPLICATE = 0x0002;

    int TOKEN_IMPERSONATE = 0x0004;

    int TOKEN_QUERY = 0x0008;

    int TOKEN_QUERY_SOURCE = 0x0010;

    int TOKEN_ADJUST_PRIVILEGES = 0x0020;

    int TOKEN_ADJUST_GROUPS = 0x0040;

    int TOKEN_ADJUST_DEFAULT = 0x0080;

    int TOKEN_ADJUST_SESSIONID = 0x0100;

    int TOKEN_ALL_ACCESS_P = STANDARD_RIGHTS_REQUIRED | TOKEN_ASSIGN_PRIMARY
            | TOKEN_DUPLICATE | TOKEN_IMPERSONATE | TOKEN_QUERY
            | TOKEN_QUERY_SOURCE | TOKEN_ADJUST_PRIVILEGES
            | TOKEN_ADJUST_GROUPS | TOKEN_ADJUST_DEFAULT;

    int TOKEN_ALL_ACCESS = TOKEN_ALL_ACCESS_P | TOKEN_ADJUST_SESSIONID;

    int TOKEN_READ = STANDARD_RIGHTS_READ | TOKEN_QUERY;

    int TOKEN_WRITE = STANDARD_RIGHTS_WRITE | TOKEN_ADJUST_PRIVILEGES
            | TOKEN_ADJUST_GROUPS | TOKEN_ADJUST_DEFAULT;

    int TOKEN_EXECUTE = STANDARD_RIGHTS_EXECUTE;

    int THREAD_TERMINATE = 0x0001;
    int THREAD_SUSPEND_RESUME = 0x0002;
    int THREAD_GET_CONTEXT = 0x0008;
    int THREAD_SET_CONTEXT = 0x0010;
    int THREAD_QUERY_INFORMATION = 0x0040;
    int THREAD_SET_INFORMATION = 0x0020;
    int THREAD_SET_THREAD_TOKEN = 0x0080;
    int THREAD_IMPERSONATE = 0x0100;
    int THREAD_DIRECT_IMPERSONATION = 0x0200;
    int THREAD_SET_LIMITED_INFORMATION = 0x0400;
    int THREAD_QUERY_LIMITED_INFORMATION = 0x0800;
    int THREAD_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3FF;

    int LTP_PC_SMT = 0x1;

    public abstract class SECURITY_IMPERSONATION_LEVEL {
        public static final int SecurityAnonymous = 0;
        public static final int SecurityIdentification = 1;
        public static final int SecurityImpersonation = 2;
        public static final int SecurityDelegation = 3;
    }

    public abstract class TOKEN_INFORMATION_CLASS {
        public static final int TokenUser = 1;
        public static final int TokenGroups = 2;
        public static final int TokenPrivileges = 3;
        public static final int TokenOwner = 4;
        public static final int TokenPrimaryGroup = 5;
        public static final int TokenDefaultDacl = 6;
        public static final int TokenSource = 7;
        public static final int TokenType = 8;
        public static final int TokenImpersonationLevel = 9;
        public static final int TokenStatistics = 10;
        public static final int TokenRestrictedSids = 11;
        public static final int TokenSessionId = 12;
        public static final int TokenGroupsAndPrivileges = 13;
        public static final int TokenSessionReference = 14;
        public static final int TokenSandBoxInert = 15;
        public static final int TokenAuditPolicy = 16;
        public static final int TokenOrigin = 17;
        public static final int TokenElevationType = 18;
        public static final int TokenLinkedToken = 19;
        public static final int TokenElevation = 20;
        public static final int TokenHasRestrictions = 21;
        public static final int TokenAccessInformation = 22;
        public static final int TokenVirtualizationAllowed = 23;
        public static final int TokenVirtualizationEnabled = 24;
        public static final int TokenIntegrityLevel = 25;
        public static final int TokenUIAccess = 26;
        public static final int TokenMandatoryPolicy = 27;
        public static final int TokenLogonSid = 28;
    }

    public abstract class TOKEN_TYPE {
        public static final int TokenPrimary = 1;
        public static final int TokenImpersonation = 2;
    }

    public static class LUID_AND_ATTRIBUTES extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Luid", "Attributes");
        }

        public LUID Luid;
        public DWORD Attributes;

        public LUID_AND_ATTRIBUTES() {
            super();
        }

        public LUID_AND_ATTRIBUTES(LUID luid, DWORD attributes) {
            this.Luid = luid;
            this.Attributes = attributes;
        }
    }

    public static class SID_AND_ATTRIBUTES extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Sid", "Attributes");
        }

        public PSID.ByReference Sid;
        public int Attributes;

        public SID_AND_ATTRIBUTES() {
            super();
        }

        public SID_AND_ATTRIBUTES(Pointer memory) {
            super(memory);
        }
    }

    public static class TOKEN_OWNER extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Owner");
        }

        public PSID.ByReference Owner;

        public TOKEN_OWNER() {
            super();
        }

        public TOKEN_OWNER(int size) {
            super(new Memory(size));
        }

        public TOKEN_OWNER(Pointer memory) {
            super(memory);
            read();
        }
    }

    public static class PSID extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("sid");
        }

        public static class ByReference extends PSID implements Structure.ByReference {}

        public Pointer sid;

        public PSID() {
            super();
        }

        public PSID(byte[] data) {
            super(new Memory(data.length));
            getPointer().write(0, data, 0, data.length);
            read();
        }

        public PSID(int size) {
            super(new Memory(size));
        }

        public PSID(Pointer memory) {
            super(memory);
            read();
        }

        public byte[] getBytes() {
            int len = Advapi32.INSTANCE.GetLengthSid(this);
            return getPointer().getByteArray(0, len);
        }

        public String getSidString() {
            return Advapi32Util.convertSidToStringSid(this);
        }
    }

    public static class PSIDByReference extends ByReference {
        public PSIDByReference() {
            this(null);
        }

        public PSIDByReference(PSID h) {
            super(Native.POINTER_SIZE);
            setValue(h);
        }

        public void setValue(PSID h) {
            getPointer().setPointer(0, h != null ? h.getPointer() : null);
        }

        public PSID getValue() {
            Pointer p = getPointer().getPointer(0);
            if (p == null) {
                return null;
            }
            else {
                return new PSID(p);
            }
        }
    }

    public static class TOKEN_USER extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("User");
        }

        public SID_AND_ATTRIBUTES User;

        public TOKEN_USER() {
            super();
        }

        public TOKEN_USER(Pointer memory) {
            super(memory);
            read();
        }

        public TOKEN_USER(int size) {
            super(new Memory(size));
        }
    }

    public static class TOKEN_PRIMARY_GROUP extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("PrimaryGroup");
        }

        public PSID.ByReference PrimaryGroup;

        public TOKEN_PRIMARY_GROUP() {
            super();
        }

        public TOKEN_PRIMARY_GROUP(Pointer memory) {
            super(memory);
            read();
        }

        public TOKEN_PRIMARY_GROUP(int size) {
            super(new Memory(size));
        }
    }

    public static class TOKEN_GROUPS extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("GroupCount", "Group0");
        }

        public int GroupCount;
        public SID_AND_ATTRIBUTES Group0;

        public TOKEN_GROUPS() {
            super();
        }

        public TOKEN_GROUPS(Pointer memory) {
            super(memory);
            read();
        }

        public TOKEN_GROUPS(int size) {
            super(new Memory(size));
        }

        public SID_AND_ATTRIBUTES[] getGroups() {
            return (SID_AND_ATTRIBUTES[]) Group0.toArray(GroupCount);
        }
    }

    public static class PRIVILEGE_SET extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("PrivilegeCount", "Control", "Privileges");
        }

        public DWORD PrivilegeCount;
        public DWORD Control;
        public LUID_AND_ATTRIBUTES Privileges[];

        public PRIVILEGE_SET() {
            this(0);
        }

        public PRIVILEGE_SET(int nbOfPrivileges) {
            PrivilegeCount = new DWORD(nbOfPrivileges);
            if(nbOfPrivileges > 0) {
                Privileges = new LUID_AND_ATTRIBUTES[nbOfPrivileges];
            }
        }

        public PRIVILEGE_SET(Pointer p) {
            super(p);
            final int count = p.getInt(0);
            PrivilegeCount = new DWORD(count);
            if(count > 0) {
                Privileges = new LUID_AND_ATTRIBUTES[count];
            }
            read();
        }
    }

    public static class TOKEN_PRIVILEGES extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("PrivilegeCount", "Privileges");
        }

        public DWORD PrivilegeCount;

        public LUID_AND_ATTRIBUTES Privileges[];

        public TOKEN_PRIVILEGES() {
            this(0);
        }

        public TOKEN_PRIVILEGES(int nbOfPrivileges) {
            PrivilegeCount = new DWORD(nbOfPrivileges);
            Privileges = new LUID_AND_ATTRIBUTES[nbOfPrivileges];
        }

        public TOKEN_PRIVILEGES(Pointer p) {
            super(p);
            int count = p.getInt(0);
            PrivilegeCount = new DWORD(count);
            Privileges = new LUID_AND_ATTRIBUTES[count];
            read();
        }
    }

    public abstract class SID_NAME_USE {
        public static final int SidTypeUser = 1;
        public static final int SidTypeGroup = 2;
        public static final int SidTypeDomain = 3;
        public static final int SidTypeAlias = 4;
        public static final int SidTypeWellKnownGroup = 5;
        public static final int SidTypeDeletedAccount = 6;
        public static final int SidTypeInvalid = 7;
        public static final int SidTypeUnknown = 8;
        public static final int SidTypeComputer = 9;
        public static final int SidTypeLabel = 10;
    }

    /* File access rights */
    int FILE_READ_DATA = 0x00000001;
    int FILE_LIST_DIRECTORY = 0x00000001;
    int FILE_WRITE_DATA = 0x00000002;
    int FILE_ADD_FILE = 0x00000002;
    int FILE_APPEND_DATA = 0x00000004;
    int FILE_ADD_SUBDIRECTORY = 0x00000004;
    int FILE_CREATE_PIPE_INSTANCE = 0x00000004;
    int FILE_READ_EA = 0x00000008;
    int FILE_WRITE_EA = 0x00000010;
    int FILE_EXECUTE = 0x00000020;
    int FILE_TRAVERSE = 0x00000020;
    int FILE_DELETE_CHILD = 0x00000040;
    int FILE_READ_ATTRIBUTES = 0x00000080;
    int FILE_WRITE_ATTRIBUTES = 0x00000100;

    int FILE_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x000001FF;

    int FILE_GENERIC_READ = STANDARD_RIGHTS_READ | SYNCHRONIZE | FILE_READ_DATA
            | FILE_READ_ATTRIBUTES | FILE_READ_EA;

    int FILE_GENERIC_WRITE = STANDARD_RIGHTS_WRITE | SYNCHRONIZE
            | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA
            | FILE_APPEND_DATA;

    int FILE_GENERIC_EXECUTE = STANDARD_RIGHTS_EXECUTE | SYNCHRONIZE
            | FILE_READ_ATTRIBUTES | FILE_EXECUTE;

    int CREATE_NEW = 1;
    int CREATE_ALWAYS = 2;
    int OPEN_EXISTING = 3;
    int OPEN_ALWAYS = 4;
    int TRUNCATE_EXISTING = 5;

    int FILE_FLAG_WRITE_THROUGH = 0x80000000;
    int FILE_FLAG_OVERLAPPED = 0x40000000;
    int FILE_FLAG_NO_BUFFERING = 0x20000000;
    int FILE_FLAG_RANDOM_ACCESS = 0x10000000;
    int FILE_FLAG_SEQUENTIAL_SCAN = 0x08000000;
    int FILE_FLAG_DELETE_ON_CLOSE = 0x04000000;
    int FILE_FLAG_BACKUP_SEMANTICS = 0x02000000;
    int FILE_FLAG_POSIX_SEMANTICS = 0x01000000;
    int FILE_FLAG_OPEN_REPARSE_POINT = 0x00200000;
    int FILE_FLAG_OPEN_NO_RECALL = 0x00100000;

    int GENERIC_READ = 0x80000000;
    int GENERIC_WRITE = 0x40000000;
    int GENERIC_EXECUTE = 0x20000000;
    int GENERIC_ALL = 0x10000000;

    int ACCESS_SYSTEM_SECURITY             = 0x01000000;
    int PAGE_GUARD                         = 0x100;
    int PAGE_NOACCESS                      = 0x01;
    int PAGE_READONLY                      = 0x02;
    int PAGE_READWRITE                     = 0x04;
    int PAGE_WRITECOPY = 0x08;
    int PAGE_EXECUTE                       = 0x10;
    int PAGE_EXECUTE_READ                  = 0x20;
    int PAGE_EXECUTE_READWRITE             = 0x40;

    int SECTION_QUERY = 0x0001;
    int SECTION_MAP_WRITE = 0x0002;
    int SECTION_MAP_READ = 0x0004;
    int SECTION_MAP_EXECUTE = 0x0008;
    int SECTION_EXTEND_SIZE = 0x0010;

    int FILE_SHARE_READ = 0x00000001;
    int FILE_SHARE_WRITE = 0x00000002;
    int FILE_SHARE_DELETE = 0x00000004;
    int FILE_TYPE_CHAR = 0x0002;
    int FILE_TYPE_DISK = 0x0001;
    int FILE_TYPE_PIPE = 0x0003;
    int FILE_TYPE_REMOTE = 0x8000;
    int FILE_TYPE_UNKNOWN = 0x0000;
    int FILE_ATTRIBUTE_READONLY = 0x00000001;
    int FILE_ATTRIBUTE_HIDDEN = 0x00000002;
    int FILE_ATTRIBUTE_SYSTEM = 0x00000004;
    int FILE_ATTRIBUTE_DIRECTORY = 0x00000010;
    int FILE_ATTRIBUTE_ARCHIVE = 0x00000020;
    int FILE_ATTRIBUTE_DEVICE = 0x00000040;
    int FILE_ATTRIBUTE_NORMAL = 0x00000080;
    int FILE_ATTRIBUTE_TEMPORARY = 0x00000100;
    int FILE_ATTRIBUTE_SPARSE_FILE = 0x00000200;
    int FILE_ATTRIBUTE_REPARSE_POINT = 0x00000400;
    int FILE_ATTRIBUTE_COMPRESSED = 0x00000800;
    int FILE_ATTRIBUTE_OFFLINE = 0x00001000;
    int FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 0x00002000;
    int FILE_ATTRIBUTE_ENCRYPTED = 0x00004000;
    int FILE_ATTRIBUTE_VIRTUAL = 0x00010000;
    int FILE_NOTIFY_CHANGE_FILE_NAME = 0x00000001;
    int FILE_NOTIFY_CHANGE_DIR_NAME = 0x00000002;
    int FILE_NOTIFY_CHANGE_NAME = 0x00000003;
    int FILE_NOTIFY_CHANGE_ATTRIBUTES = 0x00000004;
    int FILE_NOTIFY_CHANGE_SIZE = 0x00000008;
    int FILE_NOTIFY_CHANGE_LAST_WRITE = 0x00000010;
    int FILE_NOTIFY_CHANGE_LAST_ACCESS = 0x00000020;
    int FILE_NOTIFY_CHANGE_CREATION = 0x00000040;
    int FILE_NOTIFY_CHANGE_SECURITY = 0x00000100;
    int FILE_ACTION_ADDED = 0x00000001;
    int FILE_ACTION_REMOVED = 0x00000002;
    int FILE_ACTION_MODIFIED = 0x00000003;
    int FILE_ACTION_RENAMED_OLD_NAME = 0x00000004;
    int FILE_ACTION_RENAMED_NEW_NAME = 0x00000005;
    int FILE_CASE_SENSITIVE_SEARCH = 0x00000001;
    int FILE_CASE_PRESERVED_NAMES = 0x00000002;
    int FILE_UNICODE_ON_DISK = 0x00000004;
    int FILE_PERSISTENT_ACLS = 0x00000008;
    int FILE_FILE_COMPRESSION = 0x00000010;
    int FILE_VOLUME_QUOTAS = 0x00000020;
    int FILE_SUPPORTS_SPARSE_FILES = 0x00000040;
    int FILE_SUPPORTS_REPARSE_POINTS = 0x00000080;
    int FILE_SUPPORTS_REMOTE_STORAGE = 0x00000100;
    int FILE_VOLUME_IS_COMPRESSED = 0x00008000;
    int FILE_SUPPORTS_OBJECT_IDS = 0x00010000;
    int FILE_SUPPORTS_ENCRYPTION = 0x00020000;
    int FILE_NAMED_STREAMS = 0x00040000;
    int FILE_READ_ONLY_VOLUME = 0x00080000;
    int FILE_SEQUENTIAL_WRITE_ONCE = 0x00100000;
    int FILE_SUPPORTS_TRANSACTIONS = 0x00200000;
    int FILE_SUPPORTS_HARD_LINKS = 0x00400000;
    int FILE_SUPPORTS_EXTENDED_ATTRIBUTES = 0x00800000;
    int FILE_SUPPORTS_OPEN_BY_FILE_ID = 0x01000000;
    int FILE_SUPPORTS_USN_JOURNAL = 0x02000000;

    // Reparse point tags
    int IO_REPARSE_TAG_MOUNT_POINT              = 0xA0000003;
    int IO_REPARSE_TAG_HSM                      = 0xC0000004;
    int IO_REPARSE_TAG_HSM2                     = 0x80000006;
    int IO_REPARSE_TAG_SIS                      = 0x80000007;
    int IO_REPARSE_TAG_WIM                      = 0x80000008;
    int IO_REPARSE_TAG_CSV                      = 0x80000009;
    int IO_REPARSE_TAG_DFS                      = 0x8000000A;
    int IO_REPARSE_TAG_SYMLINK                  = 0xA000000C;
    int IO_REPARSE_TAG_DFSR                     = 0x80000012;

    int DDD_RAW_TARGET_PATH = 0x00000001;
    int DDD_REMOVE_DEFINITION = 0x00000002;
    int DDD_EXACT_MATCH_ON_REMOVE = 0x00000004;
    int DDD_NO_BROADCAST_SYSTEM = 0x00000008;

    int COMPRESSION_FORMAT_NONE          = 0x0000;
    int COMPRESSION_FORMAT_DEFAULT       = 0x0001;
    int COMPRESSION_FORMAT_LZNT1         = 0x0002;
    int COMPRESSION_FORMAT_XPRESS        = 0x0003;
    int COMPRESSION_FORMAT_XPRESS_HUFF   = 0x0004;
    int COMPRESSION_ENGINE_STANDARD      = 0x0000;
    int COMPRESSION_ENGINE_MAXIMUM       = 0x0100;
    int COMPRESSION_ENGINE_HIBER         = 0x0200;

    public static class FILE_NOTIFY_INFORMATION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("NextEntryOffset", "Action", "FileNameLength", "FileName");
        }

        public int NextEntryOffset;
        public int Action;
        public int FileNameLength;
        public char[] FileName = new char[1];

        private FILE_NOTIFY_INFORMATION() {
            super();
        }

        public FILE_NOTIFY_INFORMATION(int size) {
            if (size < size()) {
                throw new IllegalArgumentException("Size must greater than "
                        + size() + ", requested " + size);
            }
            allocateMemory(size);
        }

        public String getFilename() {
            return new String(FileName, 0, FileNameLength / 2);
        }

        @Override
        public void read() {
            // avoid reading filename until we know how long it is
            FileName = new char[0];
            super.read();
            FileName = getPointer().getCharArray(12, FileNameLength / 2);
        }

        public FILE_NOTIFY_INFORMATION next() {
            if (NextEntryOffset == 0) {
                return null;
            }
            FILE_NOTIFY_INFORMATION next = new FILE_NOTIFY_INFORMATION();
            next.useMemory(getPointer(), NextEntryOffset);
            next.read();
            return next;
        }
    }

    int KEY_QUERY_VALUE = 0x0001;
    int KEY_SET_VALUE = 0x0002;
    int KEY_CREATE_SUB_KEY = 0x0004;
    int KEY_ENUMERATE_SUB_KEYS = 0x0008;
    int KEY_NOTIFY = 0x0010;
    int KEY_CREATE_LINK = 0x0020;
    int KEY_WOW64_32KEY = 0x0200;
    int KEY_WOW64_64KEY = 0x0100;
    int KEY_WOW64_RES = 0x0300;

    int KEY_READ = STANDARD_RIGHTS_READ | KEY_QUERY_VALUE
            | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY & (~SYNCHRONIZE);

    int KEY_WRITE = STANDARD_RIGHTS_WRITE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY
            & (~SYNCHRONIZE);

    int KEY_EXECUTE = KEY_READ & (~SYNCHRONIZE);

    int KEY_ALL_ACCESS = ((STANDARD_RIGHTS_ALL | KEY_QUERY_VALUE | KEY_SET_VALUE
            | KEY_CREATE_SUB_KEY | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY
            | KEY_CREATE_LINK) & (~SYNCHRONIZE));

    int REG_OPTION_RESERVED = 0x00000000;
    int REG_OPTION_NON_VOLATILE = 0x00000000;
    int REG_OPTION_VOLATILE = 0x00000001;
    int REG_OPTION_CREATE_LINK = 0x00000002;
    int REG_OPTION_BACKUP_RESTORE = 0x00000004;
    int REG_OPTION_OPEN_LINK = 0x00000008;

    int REG_LEGAL_OPTION = REG_OPTION_RESERVED | REG_OPTION_NON_VOLATILE
            | REG_OPTION_VOLATILE | REG_OPTION_CREATE_LINK
            | REG_OPTION_BACKUP_RESTORE | REG_OPTION_OPEN_LINK;

    int REG_CREATED_NEW_KEY = 0x00000001;
    int REG_OPENED_EXISTING_KEY = 0x00000002;
    int REG_STANDARD_FORMAT = 1;
    int REG_LATEST_FORMAT = 2;
    int REG_NO_COMPRESSION = 4;
    int REG_WHOLE_HIVE_VOLATILE = 0x00000001;
    int REG_REFRESH_HIVE = 0x00000002;
    int REG_NO_LAZY_FLUSH = 0x00000004;
    int REG_FORCE_RESTORE = 0x00000008;
    int REG_APP_HIVE = 0x00000010;
    int REG_PROCESS_PRIVATE = 0x00000020;
    int REG_START_JOURNAL = 0x00000040;
    int REG_HIVE_EXACT_FILE_GROWTH = 0x00000080;
    int REG_HIVE_NO_RM = 0x00000100;
    int REG_HIVE_SINGLE_LOG = 0x00000200;
    int REG_FORCE_UNLOAD = 1;

    int REG_NOTIFY_CHANGE_NAME = 0x00000001;
    int REG_NOTIFY_CHANGE_ATTRIBUTES = 0x00000002;
    int REG_NOTIFY_CHANGE_LAST_SET = 0x00000004;
    int REG_NOTIFY_CHANGE_SECURITY = 0x00000008;

    int REG_LEGAL_CHANGE_FILTER = REG_NOTIFY_CHANGE_NAME
            | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET
            | REG_NOTIFY_CHANGE_SECURITY;

    int REG_NONE = 0;
    int REG_SZ = 1;
    int REG_EXPAND_SZ = 2;
    int REG_BINARY = 3;
    int REG_DWORD = 4;
    int REG_DWORD_LITTLE_ENDIAN = 4;
    int REG_DWORD_BIG_ENDIAN = 5;
    int REG_LINK = 6;
    int REG_MULTI_SZ = 7;
    int REG_RESOURCE_LIST = 8;
    int REG_FULL_RESOURCE_DESCRIPTOR = 9;
    int REG_RESOURCE_REQUIREMENTS_LIST = 10;
    int REG_QWORD = 11;
    int REG_QWORD_LITTLE_ENDIAN = 11;

    public static class LUID extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("LowPart", "HighPart");
        }

        public int LowPart;
        public int HighPart;
    }

    public static class LARGE_INTEGER extends Structure implements Comparable<LARGE_INTEGER> {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("u");
        }

        public static class ByReference extends LARGE_INTEGER implements
                Structure.ByReference {
        }

        public static class LowHigh extends Structure {
            @Override
            protected List<String> getFieldOrder() {
                return Arrays.asList("LowPart", "HighPart");
            }

            public DWORD LowPart;
            public DWORD HighPart;

            public LowHigh() {
                super();
            }

            public LowHigh(long value) {
                this(new DWORD(value & 0xFFFFFFFFL),  new DWORD((value >> 32) & 0xFFFFFFFFL));
            }

            public LowHigh(DWORD low, DWORD high) {
                LowPart = low;
                HighPart = high;
            }

            public long longValue() {
                long loValue = LowPart.longValue();
                long hiValue = HighPart.longValue();
                return ((hiValue << 32) & 0xFFFFFFFF00000000L) | (loValue & 0xFFFFFFFFL);
            }

            @Override
            public String toString() {
                if ((LowPart == null) || (HighPart == null)) {
                    return "null";
                } else {
                    return Long.toString(longValue());
                }
            }
        }

        public static class UNION extends Union {
            public LowHigh lh;
            public long value;

            public UNION() {
                super();
            }

            public UNION(long value) {
                this.value = value;
                this.lh = new LowHigh(value);
            }

            @Override
            public void read() {
                readField("lh");
                readField("value");
            }

            public long longValue() {
                return value;
            }

            @Override
            public String toString() {
                return Long.toString(longValue());
            }
        }

        public UNION u;

        public LARGE_INTEGER() {
            super();
        }

        public LARGE_INTEGER(long value) {
            this.u = new UNION(value);
        }

        public DWORD getLow() {
            return u.lh.LowPart;
        }

        public DWORD getHigh() {
            return u.lh.HighPart;
        }

        public long getValue() {
            return u.value;
        }

        @Override
        public int compareTo(LARGE_INTEGER other) {
            return compare(this, other);
        }

        @Override
        public String toString() {
            return (u == null) ? "null" : Long.toString(getValue());
        }

        public static int compare(LARGE_INTEGER v1, LARGE_INTEGER v2) {
            if (v1 == v2) {
                return 0;
            } else if (v1 == null) {
                return 1;
            } else if (v2 == null) {
                return (-1);
            } else {
                return IntegerType.compare(v1.getValue(), v2.getValue());
            }
        }

        public static int compare(LARGE_INTEGER v1, long v2) {
            if (v1 == null) {
                return 1;
            } else {
                return IntegerType.compare(v1.getValue(), v2);
            }
        }
    }

    public static class HANDLE extends PointerType {
        private boolean immutable;

        public HANDLE() {}

        public HANDLE(Pointer p) {
            setPointer(p);
            immutable = true;
        }

        @Override
        public Object fromNative(Object nativeValue, FromNativeContext context) {
            Object o = super.fromNative(nativeValue, context);
            if (WinBase.INVALID_HANDLE_VALUE.equals(o)) {
                return WinBase.INVALID_HANDLE_VALUE;
            }
            return o;
        }

        @Override
        public void setPointer(Pointer p) {
            if (immutable) {
                throw new UnsupportedOperationException("immutable reference");
            }

            super.setPointer(p);
        }

        @Override
        public String toString() {
            return String.valueOf(getPointer());
        }
    }

    public static class HANDLEByReference extends ByReference {
        public HANDLEByReference() {
            this(null);
        }

        public HANDLEByReference(HANDLE h) {
            super(Native.POINTER_SIZE);
            setValue(h);
        }

        public void setValue(HANDLE h) {
            getPointer().setPointer(0, h != null ? h.getPointer() : null);
        }

        public HANDLE getValue() {
            Pointer p = getPointer().getPointer(0);
            if (p == null) {
                return null;
            }
            if (WinBase.INVALID_HANDLE_VALUE.getPointer().equals(p)) {
                return WinBase.INVALID_HANDLE_VALUE;
            }
            HANDLE h = new HANDLE();
            h.setPointer(p);
            return h;
        }
    }

    class HRESULT extends NativeLong {
        public HRESULT() {
        }

        public HRESULT(int value) {
            super(value);
        }
    }

    public abstract class WELL_KNOWN_SID_TYPE {
        public static final int WinNullSid = 0;
        public static final int WinWorldSid = 1;
        public static final int WinLocalSid = 2;
        public static final int WinCreatorOwnerSid = 3;
        public static final int WinCreatorGroupSid = 4;
        public static final int WinCreatorOwnerServerSid = 5;
        public static final int WinCreatorGroupServerSid = 6;
        public static final int WinNtAuthoritySid = 7;
        public static final int WinDialupSid = 8;
        public static final int WinNetworkSid = 9;
        public static final int WinBatchSid = 10;
        public static final int WinInteractiveSid = 11;
        public static final int WinServiceSid = 12;
        public static final int WinAnonymousSid = 13;
        public static final int WinProxySid = 14;
        public static final int WinEnterpriseControllersSid = 15;
        public static final int WinSelfSid = 16;
        public static final int WinAuthenticatedUserSid = 17;
        public static final int WinRestrictedCodeSid = 18;
        public static final int WinTerminalServerSid = 19;
        public static final int WinRemoteLogonIdSid = 20;
        public static final int WinLogonIdsSid = 21;
        public static final int WinLocalSystemSid = 22;
        public static final int WinLocalServiceSid = 23;
        public static final int WinNetworkServiceSid = 24;
        public static final int WinBuiltinDomainSid = 25;
        public static final int WinBuiltinAdministratorsSid = 26;
        public static final int WinBuiltinUsersSid = 27;
        public static final int WinBuiltinGuestsSid = 28;
        public static final int WinBuiltinPowerUsersSid = 29;
        public static final int WinBuiltinAccountOperatorsSid = 30;
        public static final int WinBuiltinSystemOperatorsSid = 31;
        public static final int WinBuiltinPrintOperatorsSid = 32;
        public static final int WinBuiltinBackupOperatorsSid = 33;
        public static final int WinBuiltinReplicatorSid = 34;
        public static final int WinBuiltinPreWindows2000CompatibleAccessSid = 35;
        public static final int WinBuiltinRemoteDesktopUsersSid = 36;
        public static final int WinBuiltinNetworkConfigurationOperatorsSid = 37;
        public static final int WinAccountAdministratorSid = 38;
        public static final int WinAccountGuestSid = 39;
        public static final int WinAccountKrbtgtSid = 40;
        public static final int WinAccountDomainAdminsSid = 41;
        public static final int WinAccountDomainUsersSid = 42;
        public static final int WinAccountDomainGuestsSid = 43;
        public static final int WinAccountComputersSid = 44;
        public static final int WinAccountControllersSid = 45;
        public static final int WinAccountCertAdminsSid = 46;
        public static final int WinAccountSchemaAdminsSid = 47;
        public static final int WinAccountEnterpriseAdminsSid = 48;
        public static final int WinAccountPolicyAdminsSid = 49;
        public static final int WinAccountRasAndIasServersSid = 50;
        public static final int WinNTLMAuthenticationSid = 51;
        public static final int WinDigestAuthenticationSid = 52;
        public static final int WinSChannelAuthenticationSid = 53;
        public static final int WinThisOrganizationSid = 54;
        public static final int WinOtherOrganizationSid = 55;
        public static final int WinBuiltinIncomingForestTrustBuildersSid = 56;
        public static final int WinBuiltinPerfMonitoringUsersSid = 57;
        public static final int WinBuiltinPerfLoggingUsersSid = 58;
        public static final int WinBuiltinAuthorizationAccessSid = 59;
        public static final int WinBuiltinTerminalServerLicenseServersSid = 60;
        public static final int WinBuiltinDCOMUsersSid = 61;
        public static final int WinBuiltinIUsersSid = 62;
        public static final int WinIUserSid = 63;
        public static final int WinBuiltinCryptoOperatorsSid = 64;
        public static final int WinUntrustedLabelSid = 65;
        public static final int WinLowLabelSid = 66;
        public static final int WinMediumLabelSid = 67;
        public static final int WinHighLabelSid = 68;
        public static final int WinSystemLabelSid = 69;
        public static final int WinWriteRestrictedCodeSid = 70;
        public static final int WinCreatorOwnerRightsSid = 71;
        public static final int WinCacheablePrincipalsGroupSid = 72;
        public static final int WinNonCacheablePrincipalsGroupSid = 73;
        public static final int WinEnterpriseReadonlyControllersSid = 74;
        public static final int WinAccountReadonlyControllersSid = 75;
        public static final int WinBuiltinEventLogReadersGroup = 76;
    }

    int SID_REVISION = 1;
    int SID_MAX_SUB_AUTHORITIES = 15;
    int SID_RECOMMENDED_SUB_AUTHORITIES = 1;
    int SECURITY_MAX_SID_SIZE = 68;

    public static class OSVERSIONINFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwOSVersionInfoSize", "dwMajorVersion", "dwMinorVersion", "dwBuildNumber", "dwPlatformId", "szCSDVersion");
        }

        public DWORD dwOSVersionInfoSize;
        public DWORD dwMajorVersion;
        public DWORD dwMinorVersion;
        public DWORD dwBuildNumber;
        public DWORD dwPlatformId;
        public char szCSDVersion[];

        public OSVERSIONINFO() {
            szCSDVersion = new char[128];
            dwOSVersionInfoSize = new DWORD(size());
        }

        public OSVERSIONINFO(Pointer memory) {
            super(memory);
            read();
        }
    }

    public static class OSVERSIONINFOEX extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwOSVersionInfoSize",
                    "dwMajorVersion", "dwMinorVersion", "dwBuildNumber",
                    "dwPlatformId",
                    "szCSDVersion",
                    "wServicePackMajor", "wServicePackMinor",
                    "wSuiteMask", "wProductType", "wReserved");
        }

        public DWORD dwOSVersionInfoSize;
        public DWORD dwMajorVersion;
        public DWORD dwMinorVersion;
        public DWORD dwBuildNumber;
        public DWORD dwPlatformId;
        public char szCSDVersion[];
        public WORD wServicePackMajor;
        public WORD wServicePackMinor;
        public WORD wSuiteMask;
        public byte wProductType;
        public byte wReserved;

        public OSVERSIONINFOEX() {
            szCSDVersion = new char[128];
            dwOSVersionInfoSize = new DWORD(size());
        }

        public OSVERSIONINFOEX(Pointer memory) {
            super(memory);
            read();
        }

        public int getMajor() {
            return dwMajorVersion.intValue();
        }

        public int getMinor() {
            return dwMinorVersion.intValue();
        }

        public int getBuildNumber() {
            return dwBuildNumber.intValue();
        }

        public int getPlatformId() {
            return dwPlatformId.intValue();
        }

        public String getServicePack() {
            return Native.toString(szCSDVersion);
        }

        public int getSuiteMask() {
            return wSuiteMask.intValue();
        }

        public byte getProductType() {
            return wProductType;
        }
    }

    int VER_EQUAL = 1;
    int VER_GREATER = 2;
    int VER_GREATER_EQUAL = 3;
    int VER_LESS = 4;
    int VER_LESS_EQUAL = 5;
    int VER_AND = 6;
    int VER_OR = 7;

    int VER_CONDITION_MASK = 7;
    int VER_NUM_BITS_PER_CONDITION_MASK = 3;

    int VER_MINORVERSION = 0x0000001;
    int VER_MAJORVERSION = 0x0000002;
    int VER_BUILDNUMBER = 0x0000004;
    int VER_PLATFORMID = 0x0000008;
    int VER_SERVICEPACKMINOR = 0x0000010;
    int VER_SERVICEPACKMAJOR = 0x0000020;
    int VER_SUITENAME = 0x0000040;
    int VER_PRODUCT_TYPE = 0x0000080;

    int VER_NT_WORKSTATION = 0x0000001;
    int VER_NT_DOMAIN_CONTROLLER = 0x0000002;
    int VER_NT_SERVER = 0x0000003;

    int VER_PLATFORM_WIN32s = 0;
    int VER_PLATFORM_WIN32_WINDOWS = 1;
    int VER_PLATFORM_WIN32_NT = 2;

    short WIN32_WINNT_NT4 = 0x0400; // Windows NT 4.0
    short WIN32_WINNT_WIN2K = 0x0500; // Windows 2000
    short WIN32_WINNT_WINXP = 0x0501; // Windows XP
    short WIN32_WINNT_WS03 = 0x0502; // Windows Server 2003
    short WIN32_WINNT_WIN6 = 0x0600; // Windows Vista
    short WIN32_WINNT_VISTA = 0x0600; // Windows Vista
    short WIN32_WINNT_WS08 = 0x0600; // Windows Server 2008
    short WIN32_WINNT_LONGHORN = 0x0600; // Windows Vista
    short WIN32_WINNT_WIN7 = 0x0601; // Windows 7
    short WIN32_WINNT_WIN8 = 0x0602; // Windows 8
    short WIN32_WINNT_WINBLUE = 0x0603; // Windows 8.1
    short WIN32_WINNT_WINTHRESHOLD = 0x0A00; // Windows 10
    short WIN32_WINNT_WIN10 = 0x0A00; // Windows 10
    int EVENTLOG_SEQUENTIAL_READ = 0x0001;
    int EVENTLOG_SEEK_READ = 0x0002;
    int EVENTLOG_FORWARDS_READ = 0x0004;
    int EVENTLOG_BACKWARDS_READ = 0x0008;
    int EVENTLOG_SUCCESS = 0x0000;
    int EVENTLOG_ERROR_TYPE = 0x0001;
    int EVENTLOG_WARNING_TYPE = 0x0002;
    int EVENTLOG_INFORMATION_TYPE = 0x0004;
    int EVENTLOG_AUDIT_SUCCESS = 0x0008;
    int EVENTLOG_AUDIT_FAILURE = 0x0010;

    public static class EVENTLOGRECORD extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Length", "Reserved", "RecordNumber", "TimeGenerated", "TimeWritten",
                    "EventID", "EventType", "NumStrings", "EventCategory", "ReservedFlags",
                    "ClosingRecordNumber", "StringOffset", "UserSidLength", "UserSidOffset",
                    "DataLength", "DataOffset");
        }

        public DWORD Length;
        public DWORD Reserved;
        public DWORD RecordNumber;
        public DWORD TimeGenerated;
        public DWORD TimeWritten;
        public DWORD EventID;
        public WORD EventType;
        public WORD NumStrings;
        public WORD EventCategory;
        public WORD ReservedFlags;
        public DWORD ClosingRecordNumber;
        public DWORD StringOffset;
        public DWORD UserSidLength;
        public DWORD UserSidOffset;
        public DWORD DataLength;
        public DWORD DataOffset;

        public EVENTLOGRECORD() {
            super();
        }

        public EVENTLOGRECORD(Pointer p) {
            super(p);
            read();
        }
    }

    int SERVICE_KERNEL_DRIVER = 0x00000001;
    int SERVICE_FILE_SYSTEM_DRIVER = 0x00000002;
    int SERVICE_ADAPTER = 0x00000004;
    int SERVICE_RECOGNIZER_DRIVER = 0x00000008;
    int SERVICE_DRIVER = SERVICE_KERNEL_DRIVER | SERVICE_FILE_SYSTEM_DRIVER
            | SERVICE_RECOGNIZER_DRIVER;
    int SERVICE_WIN32_OWN_PROCESS = 0x00000010;
    int SERVICE_WIN32_SHARE_PROCESS = 0x00000020;
    int SERVICE_WIN32 = SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS;
    int SERVICE_INTERACTIVE_PROCESS = 0x00000100;
    int SERVICE_TYPE_ALL = SERVICE_WIN32 | SERVICE_ADAPTER | SERVICE_DRIVER
            | SERVICE_INTERACTIVE_PROCESS;

    int SERVICE_BOOT_START   = 0x00000000;
    int SERVICE_SYSTEM_START = 0x00000001;
    int SERVICE_AUTO_START   = 0x00000002;
    int SERVICE_DEMAND_START = 0x00000003;
    int SERVICE_DISABLED     = 0x00000004;

    //
    // Error control type
    //
    int SERVICE_ERROR_IGNORE   = 0x00000000;
    int SERVICE_ERROR_NORMAL   = 0x00000001;
    int SERVICE_ERROR_SEVERE   = 0x00000002;
    int SERVICE_ERROR_CRITICAL = 0x00000003;

    int STATUS_PENDING = 0x00000103;

    // Privilege Constants
    String SE_CREATE_TOKEN_NAME = "SeCreateTokenPrivilege";
    String SE_ASSIGNPRIMARYTOKEN_NAME = "SeAssignPrimaryTokenPrivilege";
    String SE_LOCK_MEMORY_NAME = "SeLockMemoryPrivilege";
    String SE_INCREASE_QUOTA_NAME = "SeIncreaseQuotaPrivilege";
    String SE_UNSOLICITED_INPUT_NAME = "SeUnsolicitedInputPrivilege";
    String SE_MACHINE_ACCOUNT_NAME = "SeMachineAccountPrivilege";
    String SE_TCB_NAME = "SeTcbPrivilege";
    String SE_SECURITY_NAME = "SeSecurityPrivilege";
    String SE_TAKE_OWNERSHIP_NAME = "SeTakeOwnershipPrivilege";
    String SE_LOAD_DRIVER_NAME = "SeLoadDriverPrivilege";
    String SE_SYSTEM_PROFILE_NAME = "SeSystemProfilePrivilege";
    String SE_SYSTEMTIME_NAME = "SeSystemtimePrivilege";
    String SE_PROF_SINGLE_PROCESS_NAME = "SeProfileSingleProcessPrivilege";
    String SE_INC_BASE_PRIORITY_NAME = "SeIncreaseBasePriorityPrivilege";
    String SE_CREATE_PAGEFILE_NAME = "SeCreatePagefilePrivilege";
    String SE_CREATE_PERMANENT_NAME = "SeCreatePermanentPrivilege";
    String SE_BACKUP_NAME = "SeBackupPrivilege";
    String SE_RESTORE_NAME = "SeRestorePrivilege";
    String SE_SHUTDOWN_NAME = "SeShutdownPrivilege";
    String SE_DEBUG_NAME = "SeDebugPrivilege";
    String SE_AUDIT_NAME = "SeAuditPrivilege";
    String SE_SYSTEM_ENVIRONMENT_NAME = "SeSystemEnvironmentPrivilege";
    String SE_CHANGE_NOTIFY_NAME = "SeChangeNotifyPrivilege";
    String SE_REMOTE_SHUTDOWN_NAME = "SeRemoteShutdownPrivilege";
    String SE_UNDOCK_NAME = "SeUndockPrivilege";
    String SE_SYNC_AGENT_NAME = "SeSyncAgentPrivilege";
    String SE_ENABLE_DELEGATION_NAME = "SeEnableDelegationPrivilege";
    String SE_MANAGE_VOLUME_NAME = "SeManageVolumePrivilege";
    String SE_IMPERSONATE_NAME = "SeImpersonatePrivilege";
    String SE_CREATE_GLOBAL_NAME = "SeCreateGlobalPrivilege";

    int SE_PRIVILEGE_ENABLED_BY_DEFAULT = 0x00000001;
    int SE_PRIVILEGE_ENABLED = 0x00000002;
    int SE_PRIVILEGE_REMOVED = 0X00000004;
    int SE_PRIVILEGE_USED_FOR_ACCESS = 0x80000000;
    int PROCESS_CREATE_PROCESS = 0x0080;
    int PROCESS_CREATE_THREAD = 0x0002;
    int PROCESS_DUP_HANDLE = 0x0040;

    int PROCESS_ALL_ACCESS = WinNT.PROCESS_CREATE_PROCESS
            | WinNT.PROCESS_CREATE_THREAD
            | WinNT.PROCESS_DUP_HANDLE
            | WinNT.PROCESS_QUERY_INFORMATION
            | WinNT.PROCESS_QUERY_LIMITED_INFORMATION
            | WinNT.PROCESS_SET_INFORMATION
            | WinNT.PROCESS_SET_QUOTA
            | WinNT.PROCESS_SUSPEND_RESUME
            | WinNT.PROCESS_SYNCHRONIZE
            | WinNT.PROCESS_TERMINATE
            | WinNT.PROCESS_VM_OPERATION
            | WinNT.PROCESS_VM_READ
            | WinNT.PROCESS_VM_WRITE
            | WinNT.DELETE
            | WinNT.READ_CONTROL
            | WinNT.WRITE_DAC
            | WinNT.WRITE_OWNER
            | WinNT.SYNCHRONIZE;

    int PROCESS_QUERY_INFORMATION = 0x0400;
    int PROCESS_QUERY_LIMITED_INFORMATION = 0x1000;
    int PROCESS_SET_INFORMATION = 0x0200;
    int PROCESS_SET_QUOTA = 0x0100;
    int PROCESS_SUSPEND_RESUME = 0x0800;
    int PROCESS_TERMINATE = 0x00000001;
    int PROCESS_NAME_NATIVE = 0x00000001;
    int PROCESS_VM_OPERATION = 0x0008;
    int PROCESS_VM_READ = 0x0010;
    int PROCESS_VM_WRITE = 0x0020;
    int PROCESS_SYNCHRONIZE = 0x00100000;

    int OWNER_SECURITY_INFORMATION = 0x00000001;
    int GROUP_SECURITY_INFORMATION = 0x00000002;
    int DACL_SECURITY_INFORMATION = 0x00000004;
    int SACL_SECURITY_INFORMATION = 0x00000008;
    int LABEL_SECURITY_INFORMATION = 0x00000010;
    int PROTECTED_DACL_SECURITY_INFORMATION = 0x80000000;
    int PROTECTED_SACL_SECURITY_INFORMATION = 0x40000000;
    int UNPROTECTED_DACL_SECURITY_INFORMATION = 0x20000000;
    int UNPROTECTED_SACL_SECURITY_INFORMATION = 0x10000000;

    /* Security control bits */
    int SE_OWNER_DEFAULTED          = 0x00000001;
    int SE_GROUP_DEFAULTED          = 0x00000002;
    int SE_DACL_PRESENT             = 0x00000004;
    int SE_DACL_DEFAULTED           = 0x00000008;
    int SE_SACL_PRESENT             = 0x00000010;
    int SE_SACL_DEFAULTED           = 0x00000020;
    int SE_DACL_AUTO_INHERIT_REQ    = 0x00000100;
    int SE_SACL_AUTO_INHERIT_REQ    = 0x00000200;
    int SE_DACL_AUTO_INHERITED      = 0x00000400;
    int SE_SACL_AUTO_INHERITED      = 0x00000800;
    int SE_DACL_PROTECTED           = 0x00001000;
    int SE_SACL_PROTECTED           = 0x00002000;
    int SE_RM_CONTROL_VALID         = 0x00004000;
    int SE_SELF_RELATIVE            = 0x00008000;

    int SECURITY_DESCRIPTOR_REVISION = 0x00000001;

    public static class SECURITY_DESCRIPTOR extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("data");
        }

        public static class ByReference extends SECURITY_DESCRIPTOR implements
                Structure.ByReference {
        }

        public byte[] data;

        public SECURITY_DESCRIPTOR() {
            super();
        }

        public SECURITY_DESCRIPTOR(byte[] data) {
            super();
            this.data = data;
            useMemory(new Memory(data.length));
        }

        public SECURITY_DESCRIPTOR(int size) {
            super();
            useMemory(new Memory(size));
            data = new byte[size];
        }

        public SECURITY_DESCRIPTOR(Pointer memory) {
            super(memory);
            read();
        }
    }

    int ACL_REVISION        = 2;
    int ACL_REVISION_DS     = 4;
    int ACL_REVISION1       = 1;
    int ACL_REVISION2       = 2;
    int ACL_REVISION3       = 3;
    int ACL_REVISION4       = 4;
    int MIN_ACL_REVISION    = ACL_REVISION2;
    int MAX_ACL_REVISION    = ACL_REVISION4;

    public static class ACL extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("AclRevision", "Sbz1", "AclSize", "AceCount", "Sbz2");
        }

        public static int MAX_ACL_SIZE = 64 * 1024;

        public byte AclRevision;
        public byte Sbz1;
        public short AclSize;
        public short AceCount;
        public short Sbz2;

        public ACL() {
            super();
        }

        public ACL(int size) {
            super();
            useMemory(new Memory(size));
        }

        public ACL(Pointer pointer) {
            super(pointer);
            read();
        }

        public ACE_HEADER[] getACEs() {
            ACE_HEADER[] ACEs = new ACE_HEADER[AceCount];
            final Pointer pointer = this.getPointer();
            int offset = size();
            for (int i = 0; i < AceCount; i++) {
                final Pointer share = pointer.share(offset);
                final byte aceType = share.getByte(0); // ACE_HEADER.AceType
                switch (aceType) {
                    case ACCESS_ALLOWED_ACE_TYPE:
                        ACEs[i] = new ACCESS_ALLOWED_ACE(share);
                        break;
                    case ACCESS_DENIED_ACE_TYPE:
                        ACEs[i] = new ACCESS_DENIED_ACE(share);
                        break;
                    default:
                        ACEs[i] = new ACE_HEADER(share);
                        break;
                }
                offset += ACEs[i].AceSize;
            }
            return ACEs;
        }
    }

    public static class PACLByReference extends ByReference {
        public PACLByReference() {
            this(null);
        }

        public PACLByReference(ACL h) {
            super(Native.POINTER_SIZE);
            setValue(h);
        }

        public void setValue(ACL h) {
            getPointer().setPointer(0, h != null ? h.getPointer() : null);
        }

        public ACL getValue() {
            Pointer p = getPointer().getPointer(0);
            if (p == null) {
                return null;
            }
            else {
                return new ACL(p);
            }
        }
    }

    public static class SECURITY_DESCRIPTOR_RELATIVE extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Revision", "Sbz1", "Control", "Owner", "Group", "Sacl", "Dacl");
        }

        public static class ByReference extends SECURITY_DESCRIPTOR_RELATIVE
                implements Structure.ByReference {
        }

        public byte Revision;
        public byte Sbz1;
        public short Control;
        public int Owner;
        public int Group;
        public int Sacl;
        public int Dacl;

        private PSID OWNER;
        private PSID GROUP;
        private ACL SACL;
        private ACL DACL;

        public SECURITY_DESCRIPTOR_RELATIVE() {
            super();
        }

        public SECURITY_DESCRIPTOR_RELATIVE(byte[] data) {
            super(new Memory(data.length));
            getPointer().write(0, data, 0, data.length);
            setMembers();
        }

        public SECURITY_DESCRIPTOR_RELATIVE(int length) {
            super(new Memory(length));
        }

        public SECURITY_DESCRIPTOR_RELATIVE(Pointer p) {
            super(p);
            setMembers();
        }

        public PSID getOwner() {
            return OWNER;
        }

        public PSID getGroup() {
            return GROUP;
        }

        public ACL getDiscretionaryACL() {
            return DACL;
        }

        public ACL getSystemACL() {
            return SACL;
        }

        private final void setMembers() {
            read();
            if (Dacl != 0) {
                DACL = new ACL(getPointer().share(Dacl));
            }
            if (Sacl != 0) {
                SACL = new ACL(getPointer().share(Sacl));
            }
            if (Group != 0) {
                GROUP = new PSID(getPointer().share(Group));
            }
            if (Owner != 0) {
                OWNER = new PSID(getPointer().share(Owner));
            }
        }
    }

    public static class ACE_HEADER extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("AceType", "AceFlags", "AceSize");
        }

        public byte AceType;
        public byte AceFlags;
        public short AceSize;

        public ACE_HEADER() {
            super();
        }

        public ACE_HEADER(Pointer p) {
            super(p);
            read();
        }

        public ACE_HEADER(byte AceType, byte AceFlags, short AceSize) {
            super();
            this.AceType = AceType;
            this.AceFlags = AceFlags;
            this.AceSize = AceSize;
            write();
        }
    }

    public static abstract class ACCESS_ACEStructure extends ACE_HEADER {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Mask", "SidStart");
        }

        public int Mask;
        public byte[] SidStart = new byte[4];

        PSID psid;

        public ACCESS_ACEStructure() {
            super();
        }

        public ACCESS_ACEStructure(int Mask, byte AceType, byte AceFlags, PSID psid) {
            super();
            this.calculateSize(true);
            this.AceType = AceType;
            this.AceFlags = AceFlags;
            this.AceSize = (short) (super.fieldOffset("SidStart") + psid.getBytes().length);
            this.psid = psid;
            this.Mask = Mask;
            this.SidStart = psid.getPointer().getByteArray(0, SidStart.length);
            this.allocateMemory(AceSize);
            write();
        }

        public ACCESS_ACEStructure(Pointer p) {
            super(p);
            read();
        }

        public String getSidString() {
            return Advapi32Util.convertSidToStringSid(psid);
        }

        public PSID getSID() {
            return psid;
        }

        @Override
        public void write() {
            super.write();
            int offsetOfSID = super.fieldOffset("SidStart");
            int sizeOfSID = super.AceSize - super.fieldOffset("SidStart");
            if(psid != null) {
                byte[] psidWrite = psid.getBytes();
                assert psidWrite.length <= sizeOfSID;
                getPointer().write(offsetOfSID, psidWrite, 0, sizeOfSID);
            }
        }

        @Override
        public void read() {
            if(SidStart == null) {
                SidStart = new byte[4];
            }
            super.read();
            int offsetOfSID = super.fieldOffset("SidStart");
            int sizeOfSID = super.AceSize - super.fieldOffset("SidStart");
            if(sizeOfSID > 0) {
                psid = new PSID(getPointer().getByteArray(offsetOfSID, sizeOfSID));
            } else {
                psid = new PSID();
            }
        }
    }

    public static class ACCESS_ALLOWED_ACE extends ACCESS_ACEStructure {
        public ACCESS_ALLOWED_ACE() {
            super();
        }

        public ACCESS_ALLOWED_ACE(Pointer p) {
            super(p);
        }

        public ACCESS_ALLOWED_ACE(int Mask, byte AceFlags, PSID psid) {
            super(Mask, ACCESS_ALLOWED_ACE_TYPE, AceFlags, psid);
        }
    }

    public static class ACCESS_DENIED_ACE extends ACCESS_ACEStructure {
        public ACCESS_DENIED_ACE() {
            super();
        }

        public ACCESS_DENIED_ACE(Pointer p) {
            super(p);
        }

        public ACCESS_DENIED_ACE(int Mask, byte AceFlags, PSID psid) {
            super(Mask, ACCESS_DENIED_ACE_TYPE, AceFlags, psid);
        }
    }

    byte ACCESS_ALLOWED_ACE_TYPE = 0x00;
    byte ACCESS_DENIED_ACE_TYPE = 0x01;
    byte SYSTEM_AUDIT_ACE_TYPE = 0x02;
    byte SYSTEM_ALARM_ACE_TYPE = 0x03;
    byte ACCESS_ALLOWED_COMPOUND_ACE_TYPE = 0x04;
    byte ACCESS_ALLOWED_OBJECT_ACE_TYPE = 0x05;
    byte ACCESS_DENIED_OBJECT_ACE_TYPE = 0x06;
    byte SYSTEM_AUDIT_OBJECT_ACE_TYPE = 0x07;
    byte SYSTEM_ALARM_OBJECT_ACE_TYPE = 0x08;
    byte ACCESS_ALLOWED_CALLBACK_ACE_TYPE = 0x09;
    byte ACCESS_DENIED_CALLBACK_ACE_TYPE = 0x0A;
    byte ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE = 0x0B;
    byte ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE = 0x0C;
    byte SYSTEM_AUDIT_CALLBACK_ACE_TYPE = 0x0D;
    byte SYSTEM_ALARM_CALLBACK_ACE_TYPE = 0x0E;
    byte SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE = 0x0F;
    byte SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE = 0x10;
    byte SYSTEM_MANDATORY_LABEL_ACE_TYPE = 0x11;

    /* ACE inherit flags */
    byte OBJECT_INHERIT_ACE = 0x01;
    byte CONTAINER_INHERIT_ACE = 0x02;
    byte NO_PROPAGATE_INHERIT_ACE = 0x04;
    byte INHERIT_ONLY_ACE = 0x08;
    byte INHERITED_ACE = 0x10;
    byte VALID_INHERIT_FLAGS = 0x1F;

    interface OVERLAPPED_COMPLETION_ROUTINE extends StdCallCallback {
        void callback(int errorCode, int nBytesTransferred,
                      WinBase.OVERLAPPED overlapped);
    }

    public static class GENERIC_MAPPING extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("genericRead", "genericWrite", "genericExecute", "genericAll");
        }

        public static class ByReference extends GENERIC_MAPPING implements Structure.ByReference {}

        public DWORD genericRead;
        public DWORD genericWrite;
        public DWORD genericExecute;
        public DWORD genericAll;
    }

    public static class SYSTEM_LOGICAL_PROCESSOR_INFORMATION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("processorMask", "relationship", "payload");
        }

        public ULONG_PTR processorMask;
        public int relationship;
        public AnonymousUnionPayload payload;

        public SYSTEM_LOGICAL_PROCESSOR_INFORMATION() {
            super();
        }

        public SYSTEM_LOGICAL_PROCESSOR_INFORMATION(Pointer memory) {
            super(memory);
            read();
        }

        public static class AnonymousUnionPayload extends Union {
            public AnonymousStructProcessorCore processorCore;
            public AnonymousStructNumaNode numaNode;
            public CACHE_DESCRIPTOR cache;
            public ULONGLONG[] reserved = new ULONGLONG[2];
        }

        public static class AnonymousStructProcessorCore extends Structure {
            @Override
            protected List<String> getFieldOrder() {
                return Arrays.asList("flags");
            }

            public BYTE flags;
        }

        public static class AnonymousStructNumaNode extends Structure {
            @Override
            protected List<String> getFieldOrder() {
                return Arrays.asList("nodeNumber");
            }

            public DWORD nodeNumber;
        }
    }

    public abstract class SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("relationship", "size");
        }

        public int relationship;

        public int size;

        public SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX() {
            super();
        }

        protected SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX(Pointer memory) {
            super(memory);
        }

        public static SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX fromPointer(Pointer memory) {
            int relationship = memory.getInt(0);
            SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX result;
            switch (relationship) {
                case LOGICAL_PROCESSOR_RELATIONSHIP.RelationProcessorCore:
                case LOGICAL_PROCESSOR_RELATIONSHIP.RelationProcessorPackage:
                    result = new PROCESSOR_RELATIONSHIP(memory);
                    break;
                case LOGICAL_PROCESSOR_RELATIONSHIP.RelationNumaNode:
                    result = new NUMA_NODE_RELATIONSHIP(memory);
                    break;
                case LOGICAL_PROCESSOR_RELATIONSHIP.RelationCache:
                    result = new CACHE_RELATIONSHIP(memory);
                    break;
                case LOGICAL_PROCESSOR_RELATIONSHIP.RelationGroup:
                    result = new GROUP_RELATIONSHIP(memory);
                    break;
                default:
                    throw new IllegalStateException("Unmapped relationship: " + relationship);
            }
            result.read();
            return result;
        }
    }

    public static class PROCESSOR_RELATIONSHIP extends SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("flags", "efficiencyClass", "reserved", "groupCount", "groupMask");
        }

        public byte flags;
        public byte efficiencyClass;
        public byte[] reserved = new byte[20];
        public short groupCount;
        public GROUP_AFFINITY[] groupMask = new GROUP_AFFINITY[1];

        public PROCESSOR_RELATIONSHIP() {
        }

        public PROCESSOR_RELATIONSHIP(Pointer memory) {
            super(memory);
        }

        @Override
        public void read() {
            readField("groupCount");
            groupMask = new GROUP_AFFINITY[groupCount];
            super.read();
        }
    }

    public static class NUMA_NODE_RELATIONSHIP extends SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("nodeNumber", "reserved", "groupMask");
        }

        public int nodeNumber;
        public byte[] reserved = new byte[20];
        public GROUP_AFFINITY groupMask;

        public NUMA_NODE_RELATIONSHIP() {
        }

        public NUMA_NODE_RELATIONSHIP(Pointer memory) {
            super(memory);
        }
    }

    public static class CACHE_RELATIONSHIP extends SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("level", "associativity", "lineSize", "cacheSize", "type", "reserved", "groupMask");
        }

        public byte level;
        public byte associativity;
        public short lineSize;
        public int cacheSize;
        public int /* PROCESSOR_CACHE_TYPE */ type;
        public byte[] reserved = new byte[20];
        public GROUP_AFFINITY groupMask;

        public CACHE_RELATIONSHIP() {
        }

        public CACHE_RELATIONSHIP(Pointer memory) {
            super(memory);
        }
    }

    public static class GROUP_RELATIONSHIP extends SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("maximumGroupCount", "activeGroupCount", "reserved", "groupInfo");
        }

        public short maximumGroupCount;
        public short activeGroupCount;
        public byte[] reserved = new byte[20];
        public PROCESSOR_GROUP_INFO[] groupInfo = new PROCESSOR_GROUP_INFO[1];

        public GROUP_RELATIONSHIP() {
        }

        public GROUP_RELATIONSHIP(Pointer memory) {
            super(memory);
        }

        @Override
        public void read() {
            readField("activeGroupCount");
            groupInfo = new PROCESSOR_GROUP_INFO[activeGroupCount];
            super.read();
        }
    }

    public static class GROUP_AFFINITY extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("mask", "group", "reserved");
        }

        public ULONG_PTR mask;
        public short group;
        public short[] reserved = new short[3];

        public GROUP_AFFINITY(Pointer memory) {
            super(memory);
        }

        public GROUP_AFFINITY() {
            super();
        }
    }

    public static class PROCESSOR_GROUP_INFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("maximumProcessorCount", "activeProcessorCount", "reserved", "activeProcessorMask");
        }

        public byte maximumProcessorCount;
        public byte activeProcessorCount;
        public byte[] reserved = new byte[38];
        public ULONG_PTR /* KAFFINITY */ activeProcessorMask;

        public PROCESSOR_GROUP_INFO(Pointer memory) {
            super(memory);
        }

        public PROCESSOR_GROUP_INFO() {
            super();
        }
    }

    public interface LOGICAL_PROCESSOR_RELATIONSHIP {
        int RelationProcessorCore = 0;
        int RelationNumaNode = 1;
        int RelationCache = 2;
        int RelationProcessorPackage = 3;
        int RelationGroup = 4;
        int RelationAll = 0xFFFF;
    }

    byte CACHE_FULLY_ASSOCIATIVE = (byte)0xFF;

    public static class CACHE_DESCRIPTOR extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("level", "associativity", "lineSize", "size", "type");
        }

        public BYTE level;
        public BYTE associativity;
        public WORD lineSize;
        public DWORD size;
        public int /* PROCESSOR_CACHE_TYPE */ type;
    }

    public static abstract class PROCESSOR_CACHE_TYPE {
        public static int CacheUnified = 0;
        public static int CacheInstruction = 1;
        public static int CacheData = 2;
        public static int CacheTrace = 3;
    }

    public interface POWER_ACTION {
        int PowerActionNone = 0;
        int PowerActionReserved = 1;
        int PowerActionSleep = 2;
        int PowerActionHibernate = 3;
        int PowerActionShutdown = 4;
        int PowerActionShutdownReset = 5;
        int PowerActionShutdownOff = 6;
        int PowerActionWarmEject = 7;
        int PowerActionDisplayOff = 8;
    }

    public interface SYSTEM_POWER_STATE {
        int PowerSystemUnspecified = 0;
        int PowerSystemWorking = 1;
        int PowerSystemSleeping1 = 2;
        int PowerSystemSleeping2 = 3;
        int PowerSystemSleeping3 = 4;
        int PowerSystemHibernate = 5;
        int PowerSystemShutdown = 6;
        int PowerSystemMaximum = 7;
    }

    class SYSTEM_BATTERY_STATE extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("AcOnLine", "BatteryPresent", "Charging", "Discharging", "Spare1", "Tag", "MaxCapacity",
                    "RemainingCapacity", "Rate", "EstimatedTime", "DefaultAlert1", "DefaultAlert2");
        }

        public byte AcOnLine;
        public byte BatteryPresent;
        public byte Charging;
        public byte Discharging;
        public byte[] Spare1 = new byte[3];
        public byte Tag;
        public int MaxCapacity;
        public int RemainingCapacity;
        public int Rate;
        public int EstimatedTime;
        public int DefaultAlert1;
        public int DefaultAlert2;

        public SYSTEM_BATTERY_STATE(Pointer p) {
            super(p);
            read();
        }

        public SYSTEM_BATTERY_STATE() {
            super();
        }
    }

    class BATTERY_REPORTING_SCALE extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Granularity", "Capacity");
        }

        public int Granularity;
        public int Capacity;
    }

    class PROCESSOR_POWER_INFORMATION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Number", "MaxMhz", "CurrentMhz", "MhzLimit", "MaxIdleState", "CurrentIdleState");
        }

        public int Number;
        public int MaxMhz;
        public int CurrentMhz;
        public int MhzLimit;
        public int MaxIdleState;
        public int CurrentIdleState;

        public PROCESSOR_POWER_INFORMATION(Pointer p) {
            super(p);
            read();
        }

        public PROCESSOR_POWER_INFORMATION() {
            super();
        }
    }

    class SYSTEM_POWER_INFORMATION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("MaxIdlenessAllowed", "Idleness", "TimeRemaining", "CoolingMode");
        }

        public int MaxIdlenessAllowed;
        public int Idleness;
        public int TimeRemaining;
        public byte CoolingMode;

        public SYSTEM_POWER_INFORMATION(Pointer p) {
            super(p);
            read();
        }

        public SYSTEM_POWER_INFORMATION() {
            super();
        }
    }

    class POWER_ACTION_POLICY extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Action", "Flags", "EventCode");
        }

        public int /* POWER_ACTION */ Action;
        public int Flags;
        public int EventCode;
    }

    class SYSTEM_POWER_LEVEL extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Enable", "Spare", "BatteryLevel", "PowerPolicy", "MinSystemState");
        }

        public byte Enable;
        public byte[] Spare = new byte[3];
        public int BatteryLevel;
        public POWER_ACTION_POLICY PowerPolicy;
        public int /* SYSTEM_POWER_STATE */ MinSystemState;
    }

    int NUM_DISCHARGE_POLICIES = 4;

    class SYSTEM_POWER_POLICY extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Revision", "PowerButton", "SleepButton", "LidClose", "LidOpenWake", "Reserved", "Idle",
                    "IdleTimeout", "IdleSensitivity", "DynamicThrottle", "Spare2", "MinSleep", "MaxSleep",
                    "ReducedLatencySleep", "WinLogonFlags", "Spare3", "DozeS4Timeout", "BroadcastCapacityResolution",
                    "DischargePolicy", "VideoTimeout", "VideoDimDisplay", "VideoReserved", "SpindownTimeout",
                    "OptimizeForPower", "FanThrottleTolerance", "ForcedThrottle", "MinThrottle", "OverThrottled");
        }

        public int Revision;
        public POWER_ACTION_POLICY PowerButton;
        public POWER_ACTION_POLICY SleepButton;
        public POWER_ACTION_POLICY LidClose;
        public int /* SYSTEM_POWER_STATE */ LidOpenWake;
        public int Reserved;
        public POWER_ACTION_POLICY Idle;
        public int IdleTimeout;
        public byte IdleSensitivity;
        public byte DynamicThrottle;
        public byte[] Spare2 = new byte[2];
        public int /* SYSTEM_POWER_STATE */ MinSleep;
        public int /* SYSTEM_POWER_STATE */ MaxSleep;
        public int /* SYSTEM_POWER_STATE */ ReducedLatencySleep;
        public int WinLogonFlags;
        public int Spare3;
        public int DozeS4Timeout;
        public int BroadcastCapacityResolution;
        public SYSTEM_POWER_LEVEL[] DischargePolicy = new SYSTEM_POWER_LEVEL[NUM_DISCHARGE_POLICIES];
        public int VideoTimeout;
        public byte VideoDimDisplay;
        public int[] VideoReserved = new int[3];
        public int SpindownTimeout;
        public byte OptimizeForPower;
        public byte FanThrottleTolerance;
        public byte ForcedThrottle;
        public byte MinThrottle;
        public POWER_ACTION_POLICY OverThrottled;

        public SYSTEM_POWER_POLICY(Pointer p) {
            super(p);
            read();
        }

        public SYSTEM_POWER_POLICY() {
            super();
        }
    }

    class SYSTEM_POWER_CAPABILITIES extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("PowerButtonPresent", "SleepButtonPresent", "LidPresent", "SystemS1", "SystemS2", "SystemS3",
                    "SystemS4", "SystemS5", "HiberFilePresent", "FullWake", "VideoDimPresent", "ApmPresent", "UpsPresent",
                    "ThermalControl", "ProcessorThrottle", "ProcessorMinThrottle", "ProcessorMaxThrottle", "FastSystemS4",
                    "Hiberboot", "WakeAlarmPresent", "AoAc", "DiskSpinDown", "HiberFileType", "AoAcConnectivitySupported",
                    "spare3", "SystemBatteriesPresent", "BatteriesAreShortTerm", "BatteryScale", "AcOnLineWake", "SoftLidWake",
                    "RtcWake", "MinDeviceWakeState", "DefaultLowLatencyWake");
        }

        public byte PowerButtonPresent;
        public byte SleepButtonPresent;
        public byte LidPresent;
        public byte SystemS1;
        public byte SystemS2;
        public byte SystemS3;
        public byte SystemS4; // hibernate
        public byte SystemS5; // off
        public byte HiberFilePresent;
        public byte FullWake;
        public byte VideoDimPresent;
        public byte ApmPresent;
        public byte UpsPresent;

        public byte ThermalControl;
        public byte ProcessorThrottle;
        public byte ProcessorMinThrottle;

        public byte ProcessorMaxThrottle;
        public byte FastSystemS4;
        public byte Hiberboot;
        public byte WakeAlarmPresent;
        public byte AoAc;
        public byte DiskSpinDown;
        public byte HiberFileType;
        public byte AoAcConnectivitySupported;
        public byte[] spare3 = new byte[6];
        public byte SystemBatteriesPresent;
        public byte BatteriesAreShortTerm;
        public BATTERY_REPORTING_SCALE[] BatteryScale = new BATTERY_REPORTING_SCALE[3];
        public int /* SYSTEM_POWER_STATE */ AcOnLineWake;
        public int /* SYSTEM_POWER_STATE */ SoftLidWake;
        public int /* SYSTEM_POWER_STATE */ RtcWake;
        public int /* SYSTEM_POWER_STATE */ MinDeviceWakeState;
        public int /* SYSTEM_POWER_STATE */ DefaultLowLatencyWake;

        public SYSTEM_POWER_CAPABILITIES(Pointer p) {
            super(p);
            read();
        }

        public SYSTEM_POWER_CAPABILITIES() {
            super();
        }
    }

    int MEM_COMMIT = 0x1000;
    int MEM_FREE = 0x10000;
    int MEM_RESERVE = 0x2000;
    int MEM_IMAGE = 0x1000000;
    int MEM_MAPPED = 0x40000;
    int MEM_PRIVATE = 0x20000;
    int MEM_RESET = 0x00080000;
    int MEM_RESET_UNDO = 0x1000000;
    int MEM_LARGE_PAGES = 0x20000000;
    int MEM_PHYSICAL = 0x00400000;
    int MEM_TOP_DOWN = 0x00100000;
    int MEM_COALESCE_PLACEHOLDERS = 0x00000001;
    int MEM_PRESERVE_PLACEHOLDER = 0x00000002;
    int MEM_DECOMMIT = 0x4000;
    int MEM_RELEASE = 0x8000;

    public static class MEMORY_BASIC_INFORMATION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("baseAddress", "allocationBase", "allocationProtect",
                    "regionSize", "state", "protect", "type");
        }

        public Pointer baseAddress;
        public Pointer allocationBase;
        public DWORD allocationProtect;
        public SIZE_T regionSize;
        public DWORD state;
        public DWORD protect;
        public DWORD type;
    }

    public class SECURITY_QUALITY_OF_SERVICE extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Length", "ImpersonationLevel", "ContextTrackingMode", "EffectiveOnly");
        }

        public int Length;
        public int ImpersonationLevel;
        public byte ContextTrackingMode;
        public byte EffectiveOnly;

        @Override
        public void write() {
            this.Length = size();
            super.write();
        }
    }

    byte SECURITY_DYNAMIC_TRACKING = (byte) 1;
    byte SECURITY_STATIC_TRACKING = (byte) 0;
    byte BOOLEAN_TRUE = (byte) 1;
    byte BOOLEAN_FALSE = (byte) 0;

    public static final int LANG_NEUTRAL                    = 0x00;
    public static final int LANG_INVARIANT                  = 0x7f;

    public static final int LANG_AFRIKAANS                  = 0x36;
    public static final int LANG_ALBANIAN                   = 0x1c;
    public static final int LANG_ARABIC                     = 0x01;
    public static final int LANG_ARMENIAN                   = 0x2b;
    public static final int LANG_ASSAMESE                   = 0x4d;
    public static final int LANG_AZERI                      = 0x2c;
    public static final int LANG_BASQUE                     = 0x2d;
    public static final int LANG_BELARUSIAN                 = 0x23;
    public static final int LANG_BENGALI                    = 0x45;
    public static final int LANG_BULGARIAN                  = 0x02;
    public static final int LANG_CATALAN                    = 0x03;
    public static final int LANG_CHINESE                    = 0x04;
    public static final int LANG_CROATIAN                   = 0x1a;
    public static final int LANG_CZECH                      = 0x05;
    public static final int LANG_DANISH                     = 0x06;
    public static final int LANG_DIVEHI                     = 0x65;
    public static final int LANG_DUTCH                      = 0x13;
    public static final int LANG_ENGLISH                    = 0x09;
    public static final int LANG_ESTONIAN                   = 0x25;
    public static final int LANG_FAEROESE                   = 0x38;
    public static final int LANG_FARSI                      = 0x29;
    public static final int LANG_FINNISH                    = 0x0b;
    public static final int LANG_FRENCH                     = 0x0c;
    public static final int LANG_GALICIAN                   = 0x56;
    public static final int LANG_GEORGIAN                   = 0x37;
    public static final int LANG_GERMAN                     = 0x07;
    public static final int LANG_GREEK                      = 0x08;
    public static final int LANG_GUJARATI                   = 0x47;
    public static final int LANG_HEBREW                     = 0x0d;
    public static final int LANG_HINDI                      = 0x39;
    public static final int LANG_HUNGARIAN                  = 0x0e;
    public static final int LANG_ICELANDIC                  = 0x0f;
    public static final int LANG_INDONESIAN                 = 0x21;
    public static final int LANG_ITALIAN                    = 0x10;
    public static final int LANG_JAPANESE                   = 0x11;
    public static final int LANG_KANNADA                    = 0x4b;
    public static final int LANG_KASHMIRI                   = 0x60;
    public static final int LANG_KAZAK                      = 0x3f;
    public static final int LANG_KONKANI                    = 0x57;
    public static final int LANG_KOREAN                     = 0x12;
    public static final int LANG_KYRGYZ                     = 0x40;
    public static final int LANG_LATVIAN                    = 0x26;
    public static final int LANG_LITHUANIAN                 = 0x27;
    public static final int LANG_MACEDONIAN                 = 0x2f;
    public static final int LANG_MALAY                      = 0x3e;
    public static final int LANG_MALAYALAM                  = 0x4c;
    public static final int LANG_MANIPURI                   = 0x58;
    public static final int LANG_MARATHI                    = 0x4e;
    public static final int LANG_MONGOLIAN                  = 0x50;
    public static final int LANG_NEPALI                     = 0x61;
    public static final int LANG_NORWEGIAN                  = 0x14;
    public static final int LANG_ORIYA                      = 0x48;
    public static final int LANG_POLISH                     = 0x15;
    public static final int LANG_PORTUGUESE                 = 0x16;
    public static final int LANG_PUNJABI                    = 0x46;
    public static final int LANG_ROMANIAN                   = 0x18;
    public static final int LANG_RUSSIAN                    = 0x19;
    public static final int LANG_SANSKRIT                   = 0x4f;
    public static final int LANG_SERBIAN                    = 0x1a;
    public static final int LANG_SINDHI                     = 0x59;
    public static final int LANG_SLOVAK                     = 0x1b;
    public static final int LANG_SLOVENIAN                  = 0x24;
    public static final int LANG_SPANISH                    = 0x0a;
    public static final int LANG_SWAHILI                    = 0x41;
    public static final int LANG_SWEDISH                    = 0x1d;
    public static final int LANG_SYRIAC                     = 0x5a;
    public static final int LANG_TAMIL                      = 0x49;
    public static final int LANG_TATAR                      = 0x44;
    public static final int LANG_TELUGU                     = 0x4a;
    public static final int LANG_THAI                       = 0x1e;
    public static final int LANG_TURKISH                    = 0x1f;
    public static final int LANG_UKRAINIAN                  = 0x22;
    public static final int LANG_URDU                       = 0x20;
    public static final int LANG_UZBEK                      = 0x43;
    public static final int LANG_VIETNAMESE                 = 0x2a;

    public static final int SUBLANG_NEUTRAL                 = 0x00;    // language neutral
    public static final int SUBLANG_DEFAULT                 = 0x01;    // user default
    public static final int SUBLANG_SYS_DEFAULT             = 0x02;    // system default

    public static final int SUBLANG_ARABIC_SAUDI_ARABIA     = 0x01;    // Arabic (Saudi Arabia)
    public static final int SUBLANG_ARABIC_IRAQ             = 0x02;    // Arabic (Iraq)
    public static final int SUBLANG_ARABIC_EGYPT            = 0x03;    // Arabic (Egypt)
    public static final int SUBLANG_ARABIC_LIBYA            = 0x04;    // Arabic (Libya)
    public static final int SUBLANG_ARABIC_ALGERIA          = 0x05;    // Arabic (Algeria)
    public static final int SUBLANG_ARABIC_MOROCCO          = 0x06;    // Arabic (Morocco)
    public static final int SUBLANG_ARABIC_TUNISIA          = 0x07;    // Arabic (Tunisia)
    public static final int SUBLANG_ARABIC_OMAN             = 0x08;    // Arabic (Oman)
    public static final int SUBLANG_ARABIC_YEMEN            = 0x09;    // Arabic (Yemen)
    public static final int SUBLANG_ARABIC_SYRIA            = 0x0a;    // Arabic (Syria)
    public static final int SUBLANG_ARABIC_JORDAN           = 0x0b;    // Arabic (Jordan)
    public static final int SUBLANG_ARABIC_LEBANON          = 0x0c;    // Arabic (Lebanon)
    public static final int SUBLANG_ARABIC_KUWAIT           = 0x0d;    // Arabic (Kuwait)
    public static final int SUBLANG_ARABIC_UAE              = 0x0e;    // Arabic (U.A.E)
    public static final int SUBLANG_ARABIC_BAHRAIN          = 0x0f;    // Arabic (Bahrain)
    public static final int SUBLANG_ARABIC_QATAR            = 0x10;    // Arabic (Qatar)
    public static final int SUBLANG_AZERI_LATIN             = 0x01;    // Azeri (Latin)
    public static final int SUBLANG_AZERI_CYRILLIC          = 0x02;    // Azeri (Cyrillic)
    public static final int SUBLANG_CHINESE_TRADITIONAL     = 0x01;    // Chinese (Taiwan)
    public static final int SUBLANG_CHINESE_SIMPLIFIED      = 0x02;    // Chinese (PR China)
    public static final int SUBLANG_CHINESE_HONGKONG        = 0x03;    // Chinese (Hong Kong S.A.R., P.R.C.)
    public static final int SUBLANG_CHINESE_SINGAPORE       = 0x04;    // Chinese (Singapore)
    public static final int SUBLANG_CHINESE_MACAU           = 0x05;    // Chinese (Macau S.A.R.)
    public static final int SUBLANG_DUTCH                   = 0x01;    // Dutch
    public static final int SUBLANG_DUTCH_BELGIAN           = 0x02;    // Dutch (Belgian)
    public static final int SUBLANG_ENGLISH_US              = 0x01;    // English (USA)
    public static final int SUBLANG_ENGLISH_UK              = 0x02;    // English (UK)
    public static final int SUBLANG_ENGLISH_AUS             = 0x03;    // English (Australian)
    public static final int SUBLANG_ENGLISH_CAN             = 0x04;    // English (Canadian)
    public static final int SUBLANG_ENGLISH_NZ              = 0x05;    // English (New Zealand)
    public static final int SUBLANG_ENGLISH_EIRE            = 0x06;    // English (Irish)
    public static final int SUBLANG_ENGLISH_SOUTH_AFRICA    = 0x07;    // English (South Africa)
    public static final int SUBLANG_ENGLISH_JAMAICA         = 0x08;    // English (Jamaica)
    public static final int SUBLANG_ENGLISH_CARIBBEAN       = 0x09;    // English (Caribbean)
    public static final int SUBLANG_ENGLISH_BELIZE          = 0x0a;    // English (Belize)
    public static final int SUBLANG_ENGLISH_TRINIDAD        = 0x0b;    // English (Trinidad)
    public static final int SUBLANG_ENGLISH_ZIMBABWE        = 0x0c;    // English (Zimbabwe)
    public static final int SUBLANG_ENGLISH_PHILIPPINES     = 0x0d;    // English (Philippines)
    public static final int SUBLANG_FRENCH                  = 0x01;    // French
    public static final int SUBLANG_FRENCH_BELGIAN          = 0x02;    // French (Belgian)
    public static final int SUBLANG_FRENCH_CANADIAN         = 0x03;    // French (Canadian)
    public static final int SUBLANG_FRENCH_SWISS            = 0x04;    // French (Swiss)
    public static final int SUBLANG_FRENCH_LUXEMBOURG       = 0x05;    // French (Luxembourg)
    public static final int SUBLANG_FRENCH_MONACO           = 0x06;    // French (Monaco)
    public static final int SUBLANG_GERMAN                  = 0x01;    // German
    public static final int SUBLANG_GERMAN_SWISS            = 0x02;    // German (Swiss)
    public static final int SUBLANG_GERMAN_AUSTRIAN         = 0x03;    // German (Austrian)
    public static final int SUBLANG_GERMAN_LUXEMBOURG       = 0x04;    // German (Luxembourg)
    public static final int SUBLANG_GERMAN_LIECHTENSTEIN    = 0x05;    // German (Liechtenstein)
    public static final int SUBLANG_ITALIAN                 = 0x01;    // Italian
    public static final int SUBLANG_ITALIAN_SWISS           = 0x02;    // Italian (Swiss)
    public static final int SUBLANG_KASHMIRI_SASIA          = 0x02;    // Kashmiri (South Asia)
    public static final int SUBLANG_KASHMIRI_INDIA          = 0x02;    // For app compatibility only
    public static final int SUBLANG_KOREAN                  = 0x01;    // Korean (Extended Wansung)
    public static final int SUBLANG_LITHUANIAN              = 0x01;    // Lithuanian
    public static final int SUBLANG_MALAY_MALAYSIA          = 0x01;    // Malay (Malaysia)
    public static final int SUBLANG_MALAY_BRUNEI_DARUSSALAM = 0x02;    // Malay (Brunei Darussalam)
    public static final int SUBLANG_NEPALI_INDIA            = 0x02;    // Nepali (India)
    public static final int SUBLANG_NORWEGIAN_BOKMAL        = 0x01;    // Norwegian (Bokmal)
    public static final int SUBLANG_NORWEGIAN_NYNORSK       = 0x02;    // Norwegian (Nynorsk)
    public static final int SUBLANG_PORTUGUESE              = 0x02;    // Portuguese
    public static final int SUBLANG_PORTUGUESE_BRAZILIAN    = 0x01;    // Portuguese (Brazilian)
    public static final int SUBLANG_SERBIAN_LATIN           = 0x02;    // Serbian (Latin)
    public static final int SUBLANG_SERBIAN_CYRILLIC        = 0x03;    // Serbian (Cyrillic)
    public static final int SUBLANG_SPANISH                 = 0x01;    // Spanish (Castilian)
    public static final int SUBLANG_SPANISH_MEXICAN         = 0x02;    // Spanish (Mexican)
    public static final int SUBLANG_SPANISH_MODERN          = 0x03;    // Spanish (Spain)
    public static final int SUBLANG_SPANISH_GUATEMALA       = 0x04;    // Spanish (Guatemala)
    public static final int SUBLANG_SPANISH_COSTA_RICA      = 0x05;    // Spanish (Costa Rica)
    public static final int SUBLANG_SPANISH_PANAMA          = 0x06;    // Spanish (Panama)
    public static final int SUBLANG_SPANISH_DOMINICAN_REPUBLIC = 0x07; // Spanish (Dominican Republic)
    public static final int SUBLANG_SPANISH_VENEZUELA       = 0x08;    // Spanish (Venezuela)
    public static final int SUBLANG_SPANISH_COLOMBIA        = 0x09;    // Spanish (Colombia)
    public static final int SUBLANG_SPANISH_PERU            = 0x0a;    // Spanish (Peru)
    public static final int SUBLANG_SPANISH_ARGENTINA       = 0x0b;    // Spanish (Argentina)
    public static final int SUBLANG_SPANISH_ECUADOR         = 0x0c;    // Spanish (Ecuador)
    public static final int SUBLANG_SPANISH_CHILE           = 0x0d;    // Spanish (Chile)
    public static final int SUBLANG_SPANISH_URUGUAY         = 0x0e;    // Spanish (Uruguay)
    public static final int SUBLANG_SPANISH_PARAGUAY        = 0x0f;    // Spanish (Paraguay)
    public static final int SUBLANG_SPANISH_BOLIVIA         = 0x10;    // Spanish (Bolivia)
    public static final int SUBLANG_SPANISH_EL_SALVADOR     = 0x11;    // Spanish (El Salvador)
    public static final int SUBLANG_SPANISH_HONDURAS        = 0x12;    // Spanish (Honduras)
    public static final int SUBLANG_SPANISH_NICARAGUA       = 0x13;    // Spanish (Nicaragua)
    public static final int SUBLANG_SPANISH_PUERTO_RICO     = 0x14;    // Spanish (Puerto Rico)
    public static final int SUBLANG_SWEDISH                 = 0x01;    // Swedish
    public static final int SUBLANG_SWEDISH_FINLAND         = 0x02;    // Swedish (Finland)
    public static final int SUBLANG_URDU_PAKISTAN           = 0x01;    // Urdu (Pakistan)
    public static final int SUBLANG_URDU_INDIA              = 0x02;    // Urdu (India)
    public static final int SUBLANG_UZBEK_LATIN             = 0x01;    // Uzbek (Latin)
    public static final int SUBLANG_UZBEK_CYRILLIC          = 0x02;    // Uzbek (Cyrillic)

    public static final int SORT_DEFAULT                    = 0x0;     // sorting default

    public static final int SORT_JAPANESE_XJIS              = 0x0;     // Japanese XJIS order
    public static final int SORT_JAPANESE_UNICODE           = 0x1;     // Japanese Unicode order

    public static final int SORT_CHINESE_BIG5               = 0x0;     // Chinese BIG5 order
    public static final int SORT_CHINESE_PRCP               = 0x0;     // PRC Chinese Phonetic order
    public static final int SORT_CHINESE_UNICODE            = 0x1;     // Chinese Unicode order
    public static final int SORT_CHINESE_PRC                = 0x2;     // PRC Chinese Stroke Count order
    public static final int SORT_CHINESE_BOPOMOFO           = 0x3;     // Traditional Chinese Bopomofo order

    public static final int SORT_KOREAN_KSC                 = 0x0;
    public static final int SORT_KOREAN_UNICODE             = 0x1;

    public static final int SORT_GERMAN_PHONE_BOOK          = 0x1;

    public static final int SORT_HUNGARIAN_DEFAULT          = 0x0;
    public static final int SORT_HUNGARIAN_TECHNICAL        = 0x1;

    public static final int SORT_GEORGIAN_TRADITIONAL       = 0x0;
    public static final int SORT_GEORGIAN_MODERN            = 0x1;

    public static final int NLS_VALID_LOCALE_MASK = 0x000fffff;

    public static final class LocaleMacros {
        private static final int _MAKELCID(int lgid, int srtid) {
            return (srtid << 16) | lgid;
        }

        public static final LCID MAKELCID(int lgid, int srtid) {
            return new LCID(_MAKELCID(lgid, srtid));
        }

        public static final LCID MAKESORTLCID(int lgid, int srtid, int ver) {
            return new LCID(_MAKELCID(lgid, srtid) | (ver << 20));
        }

        public static final int LANGIDFROMLCID(LCID lcid) {
            return lcid.intValue() & 0xFFFF;
        }

        public static final int SORTIDFROMLCID(LCID lcid) {
            return (lcid.intValue() >>> 16) & 0xf;
        }

        public static final int SORTVERSIONFROMLCID(LCID lcid) {
            return (lcid.intValue() >>> 20) & 0xf;
        }

        public static final int MAKELANGID(int p, int s) {
            return (s << 10) | (p & 0xFFFF);
        }

        public static final int PRIMARYLANGID(int lgid) {
            return lgid & 0x3ff;
        }

        public static final int SUBLANGID(int lgid) {
            return (lgid  & 0xFFFF) >>> 10;
        }
    }

    public static final int  LANG_SYSTEM_DEFAULT   = LocaleMacros.MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);
    public static final int  LANG_USER_DEFAULT     = LocaleMacros.MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

    public static final LCID LOCALE_SYSTEM_DEFAULT = LocaleMacros.MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT);
    public static final LCID LOCALE_USER_DEFAULT   = LocaleMacros.MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT);

    public static final LCID LOCALE_NEUTRAL        = LocaleMacros.MAKELCID(LocaleMacros.MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT);

    public static final LCID LOCALE_INVARIANT      = LocaleMacros.MAKELCID(LocaleMacros.MAKELANGID(LANG_INVARIANT, SUBLANG_NEUTRAL), SORT_DEFAULT);

    public static class IO_COUNTERS extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("ReadOperationCount", "WriteOperationCount",
                    "OtherOperationCount", "ReadTransferCount", "WriteTransferCount",
                    "OtherTransferCount");
        }

        public long ReadOperationCount;
        public long WriteOperationCount;
        public long OtherOperationCount;
        public long ReadTransferCount;
        public long WriteTransferCount;
        public long OtherTransferCount;

        public IO_COUNTERS() {
            super();
        }

        public IO_COUNTERS(Pointer memory) {
            super(memory);
        }
    }

    public int EVENT_MODIFY_STATE = 0x0002;
    public int EVENT_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3);
}
