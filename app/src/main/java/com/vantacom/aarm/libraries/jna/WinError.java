
package com.vantacom.aarm.libraries.jna;

public interface WinError {

    short FACILITY_WINRM = 51;
    short FACILITY_WINDOWSUPDATE = 36;
    short FACILITY_WINDOWS_DEFENDER = 80;
    short FACILITY_WINDOWS_CE = 24;
    short FACILITY_WINDOWS = 8;
    short FACILITY_URT = 19;
    short FACILITY_UMI = 22;
    short FACILITY_TPM_SOFTWARE = 41;
    short FACILITY_TPM_SERVICES = 40;
    short FACILITY_SXS = 23;
    short FACILITY_STORAGE = 3;
    short FACILITY_STATE_MANAGEMENT = 34;
    short FACILITY_SSPI = 9;
    short FACILITY_SCARD = 16;
    short FACILITY_SHELL = 39;
    short FACILITY_SETUPAPI = 15;
    short FACILITY_SECURITY = 9;
    short FACILITY_RPC = 1;
    short FACILITY_PLA = 48;
    short FACILITY_WIN32 = 7;
    short FACILITY_CONTROL = 10;
    short FACILITY_NULL = 0;
    short FACILITY_NDIS = 52;
    short FACILITY_METADIRECTORY = 35;
    short FACILITY_MSMQ = 14;
    short FACILITY_MEDIASERVER = 13;
    short FACILITY_INTERNET = 12;
    short FACILITY_ITF = 4;
    short FACILITY_USERMODE_HYPERVISOR = 53;
    short FACILITY_HTTP = 25;
    short FACILITY_GRAPHICS = 38;
    short FACILITY_FWP = 50;
    short FACILITY_FVE = 49;
    short FACILITY_USERMODE_FILTER_MANAGER = 31;
    short FACILITY_DPLAY = 21;
    short FACILITY_DISPATCH = 2;
    short FACILITY_DIRECTORYSERVICE = 37;
    short FACILITY_CONFIGURATION = 33;
    short FACILITY_COMPLUS = 17;
    short FACILITY_USERMODE_COMMONLOG = 26;
    short FACILITY_CMI = 54;
    short FACILITY_CERT = 11;
    short FACILITY_BACKGROUNDCOPY = 32;
    short FACILITY_ACS = 20;
    short FACILITY_AAF = 18;

    int ERROR_SUCCESS = 0;

    int NO_ERROR = 0; // dderror
    int SEC_E_OK = 0;

    int ERROR_INVALID_FUNCTION = 1;
    int ERROR_FILE_NOT_FOUND = 2;
    int ERROR_PATH_NOT_FOUND = 3;
    int ERROR_TOO_MANY_OPEN_FILES = 4;
    int ERROR_ACCESS_DENIED = 5;
    int ERROR_INVALID_HANDLE = 6;
    int ERROR_ARENA_TRASHED = 7;
    int ERROR_NOT_ENOUGH_MEMORY = 8;
    int ERROR_INVALID_BLOCK = 9;
    int ERROR_BAD_ENVIRONMENT = 10;
    int ERROR_BAD_FORMAT = 11;
    int ERROR_INVALID_ACCESS = 12;
    int ERROR_INVALID_DATA = 13;
    int ERROR_OUTOFMEMORY = 14;
    int ERROR_INVALID_DRIVE = 15;
    int ERROR_CURRENT_DIRECTORY = 16;
    int ERROR_NOT_SAME_DEVICE = 17;
    int ERROR_NO_MORE_FILES = 18;
    int ERROR_WRITE_PROTECT = 19;
    int ERROR_BAD_UNIT = 20;
    int ERROR_NOT_READY = 21;
    int ERROR_BAD_COMMAND = 22;
    int ERROR_CRC = 23;
    int ERROR_BAD_LENGTH = 24;
    int ERROR_SEEK = 25;
    int ERROR_NOT_DOS_DISK = 26;
    int ERROR_SECTOR_NOT_FOUND = 27;
    int ERROR_OUT_OF_PAPER = 28;
    int ERROR_WRITE_FAULT = 29;
    int ERROR_READ_FAULT = 30;
    int ERROR_GEN_FAILURE = 31;
    int ERROR_SHARING_VIOLATION = 32;
    int ERROR_LOCK_VIOLATION = 33;
    int ERROR_WRONG_DISK = 34;
    int ERROR_SHARING_BUFFER_EXCEEDED = 36;
    int ERROR_HANDLE_EOF = 38;
    int ERROR_HANDLE_DISK_FULL = 39;
    int ERROR_NOT_SUPPORTED = 50;
    int ERROR_REM_NOT_LIST = 51;
    int ERROR_DUP_NAME = 52;
    int ERROR_BAD_NETPATH = 53;
    int ERROR_NETWORK_BUSY = 54;
    int ERROR_DEV_NOT_EXIST = 55;
    int ERROR_TOO_MANY_CMDS = 56;
    int ERROR_ADAP_HDW_ERR = 57;
    int ERROR_BAD_NET_RESP = 58;
    int ERROR_UNEXP_NET_ERR = 59;
    int ERROR_BAD_REM_ADAP = 60;
    int ERROR_PRINTQ_FULL = 61;
    int ERROR_NO_SPOOL_SPACE = 62;
    int ERROR_PRINT_CANCELLED = 63;
    int ERROR_NETNAME_DELETED = 64;
    int ERROR_NETWORK_ACCESS_DENIED = 65;
    int ERROR_BAD_DEV_TYPE = 66;
    int ERROR_BAD_NET_NAME = 67;
    int ERROR_TOO_MANY_NAMES = 68;
    int ERROR_TOO_MANY_SESS = 69;
    int ERROR_SHARING_PAUSED = 70;
    int ERROR_REQ_NOT_ACCEP = 71;
    int ERROR_REDIR_PAUSED = 72;
    int ERROR_FILE_EXISTS = 80;
    int ERROR_CANNOT_MAKE = 82;
    int ERROR_FAIL_I24 = 83;
    int ERROR_OUT_OF_STRUCTURES = 84;
    int ERROR_ALREADY_ASSIGNED = 85;
    int ERROR_INVALID_PASSWORD = 86;
    int ERROR_INVALID_PARAMETER = 87;
    int ERROR_NET_WRITE_FAULT = 88;
    int ERROR_NO_PROC_SLOTS = 89;
    int ERROR_TOO_MANY_SEMAPHORES = 100;
    int ERROR_EXCL_SEM_ALREADY_OWNED = 101;
    int ERROR_SEM_IS_SET = 102;
    int ERROR_TOO_MANY_SEM_REQUESTS = 103;
    int ERROR_INVALID_AT_INTERRUPT_TIME = 104;
    int ERROR_SEM_OWNER_DIED = 105;
    int ERROR_SEM_USER_LIMIT = 106;
    int ERROR_DISK_CHANGE = 107;
    int ERROR_DRIVE_LOCKED = 108;
    int ERROR_BROKEN_PIPE = 109;
    int ERROR_OPEN_FAILED = 110;
    int ERROR_BUFFER_OVERFLOW = 111;
    int ERROR_DISK_FULL = 112;
    int ERROR_NO_MORE_SEARCH_HANDLES = 113;
    int ERROR_INVALID_TARGET_HANDLE = 114;
    int ERROR_INVALID_CATEGORY = 117;
    int ERROR_INVALID_VERIFY_SWITCH = 118;
    int ERROR_BAD_DRIVER_LEVEL = 119;

    //
    // MessageId: ERROR_CALL_NOT_IMPLEMENTED
    //
    // MessageText:
    //
    // This function is not supported on this system.
    //
    int ERROR_CALL_NOT_IMPLEMENTED = 120;

    //
    // MessageId: ERROR_SEM_TIMEOUT
    //
    // MessageText:
    //
    // The semaphore timeout period has expired.
    //
    int ERROR_SEM_TIMEOUT = 121;

    //
    // MessageId: ERROR_INSUFFICIENT_BUFFER
    //
    // MessageText:
    //
    // The data area passed to a system call is too small.
    //
    int ERROR_INSUFFICIENT_BUFFER = 122; // dderror

    //
    // MessageId: ERROR_INVALID_NAME
    //
    // MessageText:
    //
    // The filename, directory name, or volume label syntax is incorrect.
    //
    int ERROR_INVALID_NAME = 123; // dderror

    //
    // MessageId: ERROR_INVALID_LEVEL
    //
    // MessageText:
    //
    // The system call level is not correct.
    //
    int ERROR_INVALID_LEVEL = 124;

    //
    // MessageId: ERROR_NO_VOLUME_LABEL
    //
    // MessageText:
    //
    // The disk has no volume label.
    //
    int ERROR_NO_VOLUME_LABEL = 125;

    //
    // MessageId: ERROR_MOD_NOT_FOUND
    //
    // MessageText:
    //
    // The specified module could not be found.
    //
    int ERROR_MOD_NOT_FOUND = 126;

    //
    // MessageId: ERROR_PROC_NOT_FOUND
    //
    // MessageText:
    //
    // The specified procedure could not be found.
    //
    int ERROR_PROC_NOT_FOUND = 127;

    //
    // MessageId: ERROR_WAIT_NO_CHILDREN
    //
    // MessageText:
    //
    // There are no child processes to wait for.
    //
    int ERROR_WAIT_NO_CHILDREN = 128;

    //
    // MessageId: ERROR_CHILD_NOT_COMPLETE
    //
    // MessageText:
    //
    // The %1 application cannot be run in Win32 mode.
    //
    int ERROR_CHILD_NOT_COMPLETE = 129;

    //
    // MessageId: ERROR_DIRECT_ACCESS_HANDLE
    //
    // MessageText:
    //
    // Attempt to use a file handle to an open disk partition for an operation
    // other than raw disk I/O.
    //
    int ERROR_DIRECT_ACCESS_HANDLE = 130;

    //
    // MessageId: ERROR_NEGATIVE_SEEK
    //
    // MessageText:
    //
    // An attempt was made to move the file pointer before the beginning of the
    // file.
    //
    int ERROR_NEGATIVE_SEEK = 131;

    //
    // MessageId: ERROR_SEEK_ON_DEVICE
    //
    // MessageText:
    //
    // The file pointer cannot be set on the specified device or file.
    //
    int ERROR_SEEK_ON_DEVICE = 132;

    //
    // MessageId: ERROR_IS_JOIN_TARGET
    //
    // MessageText:
    //
    // A JOIN or SUBST command cannot be used for a drive that contains
    // previously joined drives.
    //
    int ERROR_IS_JOIN_TARGET = 133;

    //
    // MessageId: ERROR_IS_JOINED
    //
    // MessageText:
    //
    // An attempt was made to use a JOIN or SUBST command on a drive that has
    // already been joined.
    //
    int ERROR_IS_JOINED = 134;

    //
    // MessageId: ERROR_IS_SUBSTED
    //
    // MessageText:
    //
    // An attempt was made to use a JOIN or SUBST command on a drive that has
    // already been substituted.
    //
    int ERROR_IS_SUBSTED = 135;

    //
    // MessageId: ERROR_NOT_JOINED
    //
    // MessageText:
    //
    // The system tried to delete the JOIN of a drive that is not joined.
    //
    int ERROR_NOT_JOINED = 136;

    //
    // MessageId: ERROR_NOT_SUBSTED
    //
    // MessageText:
    //
    // The system tried to delete the substitution of a drive that is not
    // substituted.
    //
    int ERROR_NOT_SUBSTED = 137;

    //
    // MessageId: ERROR_JOIN_TO_JOIN
    //
    // MessageText:
    //
    // The system tried to join a drive to a directory on a joined drive.
    //
    int ERROR_JOIN_TO_JOIN = 138;

    //
    // MessageId: ERROR_SUBST_TO_SUBST
    //
    // MessageText:
    //
    // The system tried to substitute a drive to a directory on a substituted
    // drive.
    //
    int ERROR_SUBST_TO_SUBST = 139;

    //
    // MessageId: ERROR_JOIN_TO_SUBST
    //
    // MessageText:
    //
    // The system tried to join a drive to a directory on a substituted drive.
    //
    int ERROR_JOIN_TO_SUBST = 140;

    //
    // MessageId: ERROR_SUBST_TO_JOIN
    //
    // MessageText:
    //
    // The system tried to SUBST a drive to a directory on a joined drive.
    //
    int ERROR_SUBST_TO_JOIN = 141;

    //
    // MessageId: ERROR_BUSY_DRIVE
    //
    // MessageText:
    //
    // The system cannot perform a JOIN or SUBST at this time.
    //
    int ERROR_BUSY_DRIVE = 142;

    //
    // MessageId: ERROR_SAME_DRIVE
    //
    // MessageText:
    //
    // The system cannot join or substitute a drive to or for a directory on the
    // same drive.
    //
    int ERROR_SAME_DRIVE = 143;

    //
    // MessageId: ERROR_DIR_NOT_ROOT
    //
    // MessageText:
    //
    // The directory is not a subdirectory of the root directory.
    //
    int ERROR_DIR_NOT_ROOT = 144;

    //
    // MessageId: ERROR_DIR_NOT_EMPTY
    //
    // MessageText:
    //
    // The directory is not empty.
    //
    int ERROR_DIR_NOT_EMPTY = 145;

    //
    // MessageId: ERROR_IS_SUBST_PATH
    //
    // MessageText:
    //
    // The path specified is being used in a substitute.
    //
    int ERROR_IS_SUBST_PATH = 146;

    //
    // MessageId: ERROR_IS_JOIN_PATH
    //
    // MessageText:
    //
    // Not enough resources are available to process this command.
    //
    int ERROR_IS_JOIN_PATH = 147;

    //
    // MessageId: ERROR_PATH_BUSY
    //
    // MessageText:
    //
    // The path specified cannot be used at this time.
    //
    int ERROR_PATH_BUSY = 148;

    //
    // MessageId: ERROR_IS_SUBST_TARGET
    //
    // MessageText:
    //
    // An attempt was made to join or substitute a drive for which a directory
    // on the drive is the target of a previous substitute.
    //
    int ERROR_IS_SUBST_TARGET = 149;

    //
    // MessageId: ERROR_SYSTEM_TRACE
    //
    // MessageText:
    //
    // System trace information was not specified in your CONFIG.SYS file, or
    // tracing is disallowed.
    //
    int ERROR_SYSTEM_TRACE = 150;

    //
    // MessageId: ERROR_INVALID_EVENT_COUNT
    //
    // MessageText:
    //
    // The number of specified semaphore events for DosMuxSemWait is not
    // correct.
    //
    int ERROR_INVALID_EVENT_COUNT = 151;

    //
    // MessageId: ERROR_TOO_MANY_MUXWAITERS
    //
    // MessageText:
    //
    // DosMuxSemWait did not execute; too many semaphores are already set.
    //
    int ERROR_TOO_MANY_MUXWAITERS = 152;

    //
    // MessageId: ERROR_INVALID_LIST_FORMAT
    //
    // MessageText:
    //
    // The DosMuxSemWait list is not correct.
    //
    int ERROR_INVALID_LIST_FORMAT = 153;

    //
    // MessageId: ERROR_LABEL_TOO_LONG
    //
    // MessageText:
    //
    // The volume label you entered exceeds the label character limit of the
    // target file system.
    //
    int ERROR_LABEL_TOO_LONG = 154;

    //
    // MessageId: ERROR_TOO_MANY_TCBS
    //
    // MessageText:
    //
    // Cannot create another thread.
    //
    int ERROR_TOO_MANY_TCBS = 155;

    //
    // MessageId: ERROR_SIGNAL_REFUSED
    //
    // MessageText:
    //
    // The recipient process has refused the signal.
    //
    int ERROR_SIGNAL_REFUSED = 156;

    //
    // MessageId: ERROR_DISCARDED
    //
    // MessageText:
    //
    // The segment is already discarded and cannot be locked.
    //
    int ERROR_DISCARDED = 157;

    //
    // MessageId: ERROR_NOT_LOCKED
    //
    // MessageText:
    //
    // The segment is already unlocked.
    //
    int ERROR_NOT_LOCKED = 158;

    //
    // MessageId: ERROR_BAD_THREADID_ADDR
    //
    // MessageText:
    //
    // The address for the thread ID is not correct.
    //
    int ERROR_BAD_THREADID_ADDR = 159;

    //
    // MessageId: ERROR_BAD_ARGUMENTS
    //
    // MessageText:
    //
    // One or more arguments are not correct.
    //
    int ERROR_BAD_ARGUMENTS = 160;

    //
    // MessageId: ERROR_BAD_PATHNAME
    //
    // MessageText:
    //
    // The specified path is invalid.
    //
    int ERROR_BAD_PATHNAME = 161;

    //
    // MessageId: ERROR_SIGNAL_PENDING
    //
    // MessageText:
    //
    // A signal is already pending.
    //
    int ERROR_SIGNAL_PENDING = 162;

    //
    // MessageId: ERROR_MAX_THRDS_REACHED
    //
    // MessageText:
    //
    // No more threads can be created in the system.
    //
    int ERROR_MAX_THRDS_REACHED = 164;

    //
    // MessageId: ERROR_LOCK_FAILED
    //
    // MessageText:
    //
    // Unable to lock a region of a file.
    //
    int ERROR_LOCK_FAILED = 167;

    //
    // MessageId: ERROR_BUSY
    //
    // MessageText:
    //
    // The requested resource is in use.
    //
    int ERROR_BUSY = 170; // dderror

    //
    // MessageId: ERROR_CANCEL_VIOLATION
    //
    // MessageText:
    //
    // A lock request was not outstanding for the supplied cancel region.
    //
    int ERROR_CANCEL_VIOLATION = 173;

    //
    // MessageId: ERROR_ATOMIC_LOCKS_NOT_SUPPORTED
    //
    // MessageText:
    //
    // The file system does not support atomic changes to the lock type.
    //
    int ERROR_ATOMIC_LOCKS_NOT_SUPPORTED = 174;

    //
    // MessageId: ERROR_INVALID_SEGMENT_NUMBER
    //
    // MessageText:
    //
    // The system detected a segment number that was not correct.
    //
    int ERROR_INVALID_SEGMENT_NUMBER = 180;

    //
    // MessageId: ERROR_INVALID_ORDINAL
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INVALID_ORDINAL = 182;

    //
    // MessageId: ERROR_ALREADY_EXISTS
    //
    // MessageText:
    //
    // Cannot create a file when that file already exists.
    //
    int ERROR_ALREADY_EXISTS = 183;

    //
    // MessageId: ERROR_INVALID_FLAG_NUMBER
    //
    // MessageText:
    //
    // The flag passed is not correct.
    //
    int ERROR_INVALID_FLAG_NUMBER = 186;

    //
    // MessageId: ERROR_SEM_NOT_FOUND
    //
    // MessageText:
    //
    // The specified system semaphore name was not found.
    //
    int ERROR_SEM_NOT_FOUND = 187;

    //
    // MessageId: ERROR_INVALID_STARTING_CODESEG
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INVALID_STARTING_CODESEG = 188;

    //
    // MessageId: ERROR_INVALID_STACKSEG
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INVALID_STACKSEG = 189;

    //
    // MessageId: ERROR_INVALID_MODULETYPE
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INVALID_MODULETYPE = 190;

    //
    // MessageId: ERROR_INVALID_EXE_SIGNATURE
    //
    // MessageText:
    //
    // Cannot run %1 in Win32 mode.
    //
    int ERROR_INVALID_EXE_SIGNATURE = 191;

    //
    // MessageId: ERROR_EXE_MARKED_INVALID
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_EXE_MARKED_INVALID = 192;

    //
    // MessageId: ERROR_BAD_EXE_FORMAT
    //
    // MessageText:
    //
    // %1 is not a valid Win32 application.
    //
    int ERROR_BAD_EXE_FORMAT = 193;

    //
    // MessageId: ERROR_ITERATED_DATA_EXCEEDS_64k
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_ITERATED_DATA_EXCEEDS_64k = 194;

    //
    // MessageId: ERROR_INVALID_MINALLOCSIZE
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INVALID_MINALLOCSIZE = 195;

    //
    // MessageId: ERROR_DYNLINK_FROM_INVALID_RING
    //
    // MessageText:
    //
    // The operating system cannot run this application program.
    //
    int ERROR_DYNLINK_FROM_INVALID_RING = 196;

    //
    // MessageId: ERROR_IOPL_NOT_ENABLED
    //
    // MessageText:
    //
    // The operating system is not presently configured to run this application.
    //
    int ERROR_IOPL_NOT_ENABLED = 197;

    //
    // MessageId: ERROR_INVALID_SEGDPL
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INVALID_SEGDPL = 198;

    //
    // MessageId: ERROR_AUTODATASEG_EXCEEDS_64k
    //
    // MessageText:
    //
    // The operating system cannot run this application program.
    //
    int ERROR_AUTODATASEG_EXCEEDS_64k = 199;

    //
    // MessageId: ERROR_RING2SEG_MUST_BE_MOVABLE
    //
    // MessageText:
    //
    // The code segment cannot be greater than or equal to=64K.
    //
    int ERROR_RING2SEG_MUST_BE_MOVABLE = 200;

    //
    // MessageId: ERROR_RELOC_CHAIN_XEEDS_SEGLIM
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_RELOC_CHAIN_XEEDS_SEGLIM = 201;

    //
    // MessageId: ERROR_INFLOOP_IN_RELOC_CHAIN
    //
    // MessageText:
    //
    // The operating system cannot run %1.
    //
    int ERROR_INFLOOP_IN_RELOC_CHAIN = 202;

    //
    // MessageId: ERROR_ENVVAR_NOT_FOUND
    //
    // MessageText:
    //
    // The system could not find the environment option that was entered.
    //
    int ERROR_ENVVAR_NOT_FOUND = 203;

    //
    // MessageId: ERROR_NO_SIGNAL_SENT
    //
    // MessageText:
    //
    // No process in the command subtree has a signal handler.
    //
    int ERROR_NO_SIGNAL_SENT = 205;

    //
    // MessageId: ERROR_FILENAME_EXCED_RANGE
    //
    // MessageText:
    //
    // The filename or extension is too long.
    //
    int ERROR_FILENAME_EXCED_RANGE = 206;

    //
    // MessageId: ERROR_RING2_STACK_IN_USE
    //
    // MessageText:
    //
    // The ring=2 stack is in use.
    //
    int ERROR_RING2_STACK_IN_USE = 207;

    //
    // MessageId: ERROR_META_EXPANSION_TOO_LONG
    //
    // MessageText:
    //
    // The global filename characters, * or ?, are entered incorrectly or too
    // many global filename characters are specified.
    //
    int ERROR_META_EXPANSION_TOO_LONG = 208;

    //
    // MessageId: ERROR_INVALID_SIGNAL_NUMBER
    //
    // MessageText:
    //
    // The signal being posted is not correct.
    //
    int ERROR_INVALID_SIGNAL_NUMBER = 209;

    //
    // MessageId: ERROR_THREAD_1_INACTIVE
    //
    // MessageText:
    //
    // The signal handler cannot be set.
    //
    int ERROR_THREAD_1_INACTIVE = 210;

    //
    // MessageId: ERROR_LOCKED
    //
    // MessageText:
    //
    // The segment is locked and cannot be reallocated.
    //
    int ERROR_LOCKED = 212;

    //
    // MessageId: ERROR_TOO_MANY_MODULES
    //
    // MessageText:
    //
    // Too many dynamic-link modules are attached to this program or
    // dynamic-link module.
    //
    int ERROR_TOO_MANY_MODULES = 214;

    //
    // MessageId: ERROR_NESTING_NOT_ALLOWED
    //
    // MessageText:
    //
    // Cannot nest calls to LoadModule.
    //
    int ERROR_NESTING_NOT_ALLOWED = 215;

    //
    // MessageId: ERROR_EXE_MACHINE_TYPE_MISMATCH
    //
    // MessageText:
    //
    // This version of %1 is not compatible with the version of Windows you're
    // running. Check your computer's system information to see whether you need
    // a x86 (32-bit) or x64 (64-bit) version of the program, and then contact
    // the software publisher.
    //
    int ERROR_EXE_MACHINE_TYPE_MISMATCH = 216;

    //
    // MessageId: ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY
    //
    // MessageText:
    //
    // The image file %1 is signed, unable to modify.
    //
    int ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY = 217;

    //
    // MessageId: ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY
    //
    // MessageText:
    //
    // The image file %1 is strong signed, unable to modify.
    //
    int ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY = 218;

    //
    // MessageId: ERROR_FILE_CHECKED_OUT
    //
    // MessageText:
    //
    // This file is checked out or locked for editing by another user.
    //
    int ERROR_FILE_CHECKED_OUT = 220;

    //
    // MessageId: ERROR_CHECKOUT_REQUIRED
    //
    // MessageText:
    //
    // The file must be checked out before saving changes.
    //
    int ERROR_CHECKOUT_REQUIRED = 221;

    //
    // MessageId: ERROR_BAD_FILE_TYPE
    //
    // MessageText:
    //
    // The file type being saved or retrieved has been blocked.
    //
    int ERROR_BAD_FILE_TYPE = 222;

    //
    // MessageId: ERROR_FILE_TOO_LARGE
    //
    // MessageText:
    //
    // The file size exceeds the limit allowed and cannot be saved.
    //
    int ERROR_FILE_TOO_LARGE = 223;

    //
    // MessageId: ERROR_FORMS_AUTH_REQUIRED
    //
    // MessageText:
    //
    // Access Denied. Before opening files in this location, you must first
    // browse to the web site and select the option to login automatically.
    //
    int ERROR_FORMS_AUTH_REQUIRED = 224;

    //
    // MessageId: ERROR_VIRUS_INFECTED
    //
    // MessageText:
    //
    // Operation did not complete successfully because the file contains a
    // virus.
    //
    int ERROR_VIRUS_INFECTED = 225;

    //
    // MessageId: ERROR_VIRUS_DELETED
    //
    // MessageText:
    //
    // This file contains a virus and cannot be opened. Due to the nature of
    // this virus, the file has been removed from this location.
    //
    int ERROR_VIRUS_DELETED = 226;

    //
    // MessageId: ERROR_PIPE_LOCAL
    //
    // MessageText:
    //
    // The pipe is local.
    //
    int ERROR_PIPE_LOCAL = 229;

    //
    // MessageId: ERROR_BAD_PIPE
    //
    // MessageText:
    //
    // The pipe state is invalid.
    //
    int ERROR_BAD_PIPE = 230;

    //
    // MessageId: ERROR_PIPE_BUSY
    //
    // MessageText:
    //
    // All pipe instances are busy.
    //
    int ERROR_PIPE_BUSY = 231;

    //
    // MessageId: ERROR_NO_DATA
    //
    // MessageText:
    //
    // The pipe is being closed.
    //
    int ERROR_NO_DATA = 232;

    //
    // MessageId: ERROR_PIPE_NOT_CONNECTED
    //
    // MessageText:
    //
    // No process is on the other end of the pipe.
    //
    int ERROR_PIPE_NOT_CONNECTED = 233;

    //
    // MessageId: ERROR_MORE_DATA
    //
    // MessageText:
    //
    // More data is available.
    //
    int ERROR_MORE_DATA = 234; // dderror

    //
    // MessageId: ERROR_VC_DISCONNECTED
    //
    // MessageText:
    //
    // The session was canceled.
    //
    int ERROR_VC_DISCONNECTED = 240;

    //
    // MessageId: ERROR_INVALID_EA_NAME
    //
    // MessageText:
    //
    // The specified extended attribute name was invalid.
    //
    int ERROR_INVALID_EA_NAME = 254;

    //
    // MessageId: ERROR_EA_LIST_INCONSISTENT
    //
    // MessageText:
    //
    // The extended attributes are inconsistent.
    //
    int ERROR_EA_LIST_INCONSISTENT = 255;

    //
    // MessageId: WAIT_TIMEOUT
    //
    // MessageText:
    //
    // The wait operation timed out.
    //
    int WAIT_TIMEOUT = 258; // dderror

    //
    // MessageId: ERROR_NO_MORE_ITEMS
    //
    // MessageText:
    //
    // No more data is available.
    //
    int ERROR_NO_MORE_ITEMS = 259;

    //
    // MessageId: ERROR_CANNOT_COPY
    //
    // MessageText:
    //
    // The copy functions cannot be used.
    //
    int ERROR_CANNOT_COPY = 266;

    //
    // MessageId: ERROR_DIRECTORY
    //
    // MessageText:
    //
    // The directory name is invalid.
    //
    int ERROR_DIRECTORY = 267;

    //
    // MessageId: ERROR_EAS_DIDNT_FIT
    //
    // MessageText:
    //
    // The extended attributes did not fit in the buffer.
    //
    int ERROR_EAS_DIDNT_FIT = 275;

    //
    // MessageId: ERROR_EA_FILE_CORRUPT
    //
    // MessageText:
    //
    // The extended attribute file on the mounted file system is corrupt.
    //
    int ERROR_EA_FILE_CORRUPT = 276;

    //
    // MessageId: ERROR_EA_TABLE_FULL
    //
    // MessageText:
    //
    // The extended attribute table file is full.
    //
    int ERROR_EA_TABLE_FULL = 277;

    //
    // MessageId: ERROR_INVALID_EA_HANDLE
    //
    // MessageText:
    //
    // The specified extended attribute handle is invalid.
    //
    int ERROR_INVALID_EA_HANDLE = 278;

    //
    // MessageId: ERROR_EAS_NOT_SUPPORTED
    //
    // MessageText:
    //
    // The mounted file system does not support extended attributes.
    //
    int ERROR_EAS_NOT_SUPPORTED = 282;

    //
    // MessageId: ERROR_NOT_OWNER
    //
    // MessageText:
    //
    // Attempt to release mutex not owned by caller.
    //
    int ERROR_NOT_OWNER = 288;

    //
    // MessageId: ERROR_TOO_MANY_POSTS
    //
    // MessageText:
    //
    // Too many posts were made to a semaphore.
    //
    int ERROR_TOO_MANY_POSTS = 298;

    //
    // MessageId: ERROR_PARTIAL_COPY
    //
    // MessageText:
    //
    // Only part of a ReadProcessMemory or WriteProcessMemory request was
    // completed.
    //
    int ERROR_PARTIAL_COPY = 299;

    //
    // MessageId: ERROR_OPLOCK_NOT_GRANTED
    //
    // MessageText:
    //
    // The oplock request is denied.
    //
    int ERROR_OPLOCK_NOT_GRANTED = 300;

    //
    // MessageId: ERROR_INVALID_OPLOCK_PROTOCOL
    //
    // MessageText:
    //
    // An invalid oplock acknowledgment was received by the system.
    //
    int ERROR_INVALID_OPLOCK_PROTOCOL = 301;

    //
    // MessageId: ERROR_DISK_TOO_FRAGMENTED
    //
    // MessageText:
    //
    // The volume is too fragmented to complete this operation.
    //
    int ERROR_DISK_TOO_FRAGMENTED = 302;

    //
    // MessageId: ERROR_DELETE_PENDING
    //
    // MessageText:
    //
    // The file cannot be opened because it is in the process of being deleted.
    //
    int ERROR_DELETE_PENDING = 303;

    //
    // MessageId: ERROR_MR_MID_NOT_FOUND
    //
    // MessageText:
    //
    // The system cannot find message text for message number 0x%1 in the
    // message file for %2.
    //
    int ERROR_MR_MID_NOT_FOUND = 317;

    //
    // MessageId: ERROR_SCOPE_NOT_FOUND
    //
    // MessageText:
    //
    // The scope specified was not found.
    //
    int ERROR_SCOPE_NOT_FOUND = 318;

    //
    // MessageId: ERROR_FAIL_NOACTION_REBOOT
    //
    // MessageText:
    //
    // No action was taken as a system reboot is required.
    //
    int ERROR_FAIL_NOACTION_REBOOT = 350;

    //
    // MessageId: ERROR_FAIL_SHUTDOWN
    //
    // MessageText:
    //
    // The shutdown operation failed.
    //
    int ERROR_FAIL_SHUTDOWN = 351;

    //
    // MessageId: ERROR_FAIL_RESTART
    //
    // MessageText:
    //
    // The restart operation failed.
    //
    int ERROR_FAIL_RESTART = 352;

    //
    // MessageId: ERROR_MAX_SESSIONS_REACHED
    //
    // MessageText:
    //
    // The maximum number of sessions has been reached.
    //
    int ERROR_MAX_SESSIONS_REACHED = 353;

    //
    // MessageId: ERROR_THREAD_MODE_ALREADY_BACKGROUND
    //
    // MessageText:
    //
    // The thread is already in background processing mode.
    //
    int ERROR_THREAD_MODE_ALREADY_BACKGROUND = 400;

    //
    // MessageId: ERROR_THREAD_MODE_NOT_BACKGROUND
    //
    // MessageText:
    //
    // The thread is not in background processing mode.
    //
    int ERROR_THREAD_MODE_NOT_BACKGROUND = 401;

    //
    // MessageId: ERROR_PROCESS_MODE_ALREADY_BACKGROUND
    //
    // MessageText:
    //
    // The process is already in background processing mode.
    //
    int ERROR_PROCESS_MODE_ALREADY_BACKGROUND = 402;

    //
    // MessageId: ERROR_PROCESS_MODE_NOT_BACKGROUND
    //
    // MessageText:
    //
    // The process is not in background processing mode.
    //
    int ERROR_PROCESS_MODE_NOT_BACKGROUND = 403;

    //
    // MessageId: ERROR_INVALID_ADDRESS
    //
    // MessageText:
    //
    // Attempt to access invalid address.
    //
    int ERROR_INVALID_ADDRESS = 487;

    //
    // MessageId: ERROR_USER_PROFILE_LOAD
    //
    // MessageText:
    //
    // User profile cannot be loaded.
    //
    int ERROR_USER_PROFILE_LOAD = 500;

    //
    // MessageId: ERROR_ARITHMETIC_OVERFLOW
    //
    // MessageText:
    //
    // Arithmetic result exceeded=32 bits.
    //
    int ERROR_ARITHMETIC_OVERFLOW = 534;

    //
    // MessageId: ERROR_PIPE_CONNECTED
    //
    // MessageText:
    //
    // There is a process on other end of the pipe.
    //
    int ERROR_PIPE_CONNECTED = 535;

    //
    // MessageId: ERROR_PIPE_LISTENING
    //
    // MessageText:
    //
    // Waiting for a process to open the other end of the pipe.
    //
    int ERROR_PIPE_LISTENING = 536;

    //
    // MessageId: ERROR_VERIFIER_STOP
    //
    // MessageText:
    //
    // Application verifier has found an error in the current process.
    //
    int ERROR_VERIFIER_STOP = 537;

    //
    // MessageId: ERROR_ABIOS_ERROR
    //
    // MessageText:
    //
    // An error occurred in the ABIOS subsystem.
    //
    int ERROR_ABIOS_ERROR = 538;

    //
    // MessageId: ERROR_WX86_WARNING
    //
    // MessageText:
    //
    // A warning occurred in the WX86 subsystem.
    //
    int ERROR_WX86_WARNING = 539;

    //
    // MessageId: ERROR_WX86_ERROR
    //
    // MessageText:
    //
    // An error occurred in the WX86 subsystem.
    //
    int ERROR_WX86_ERROR = 540;

    //
    // MessageId: ERROR_TIMER_NOT_CANCELED
    //
    // MessageText:
    //
    // An attempt was made to cancel or set a timer that has an associated APC
    // and the subject thread is not the thread that originally set the timer
    // with an associated APC routine.
    //
    int ERROR_TIMER_NOT_CANCELED = 541;

    //
    // MessageId: ERROR_UNWIND
    //
    // MessageText:
    //
    // Unwind exception code.
    //
    int ERROR_UNWIND = 542;

    //
    // MessageId: ERROR_BAD_STACK
    //
    // MessageText:
    //
    // An invalid or unaligned stack was encountered during an unwind operation.
    //
    int ERROR_BAD_STACK = 543;

    //
    // MessageId: ERROR_INVALID_UNWIND_TARGET
    //
    // MessageText:
    //
    // An invalid unwind target was encountered during an unwind operation.
    //
    int ERROR_INVALID_UNWIND_TARGET = 544;

    //
    // MessageId: ERROR_INVALID_PORT_ATTRIBUTES
    //
    // MessageText:
    //
    // Invalid Object Attributes specified to NtCreatePort or invalid Port
    // Attributes specified to NtConnectPort
    //
    int ERROR_INVALID_PORT_ATTRIBUTES = 545;

    //
    // MessageId: ERROR_PORT_MESSAGE_TOO_LONG
    //
    // MessageText:
    //
    // Length of message passed to NtRequestPort or NtRequestWaitReplyPort was
    // longer than the maximum message allowed by the port.
    //
    int ERROR_PORT_MESSAGE_TOO_LONG = 546;

    //
    // MessageId: ERROR_INVALID_QUOTA_LOWER
    //
    // MessageText:
    //
    // An attempt was made to lower a quota limit below the current usage.
    //
    int ERROR_INVALID_QUOTA_LOWER = 547;

    //
    // MessageId: ERROR_DEVICE_ALREADY_ATTACHED
    //
    // MessageText:
    //
    // An attempt was made to attach to a device that was already attached to
    // another device.
    //
    int ERROR_DEVICE_ALREADY_ATTACHED = 548;

    //
    // MessageId: ERROR_INSTRUCTION_MISALIGNMENT
    //
    // MessageText:
    //
    // An attempt was made to execute an instruction at an unaligned address and
    // the host system does not support unaligned instruction references.
    //
    int ERROR_INSTRUCTION_MISALIGNMENT = 549;

    //
    // MessageId: ERROR_PROFILING_NOT_STARTED
    //
    // MessageText:
    //
    // Profiling not started.
    //
    int ERROR_PROFILING_NOT_STARTED = 550;

    //
    // MessageId: ERROR_PROFILING_NOT_STOPPED
    //
    // MessageText:
    //
    // Profiling not stopped.
    //
    int ERROR_PROFILING_NOT_STOPPED = 551;

    //
    // MessageId: ERROR_COULD_NOT_INTERPRET
    //
    // MessageText:
    //
    // The passed ACL did not contain the minimum required information.
    //
    int ERROR_COULD_NOT_INTERPRET = 552;

    //
    // MessageId: ERROR_PROFILING_AT_LIMIT
    //
    // MessageText:
    //
    // The number of active profiling objects is at the maximum and no more may
    // be started.
    //
    int ERROR_PROFILING_AT_LIMIT = 553;

    //
    // MessageId: ERROR_CANT_WAIT
    //
    // MessageText:
    //
    // Used to indicate that an operation cannot continue without blocking for
    // I/O.
    //
    int ERROR_CANT_WAIT = 554;

    //
    // MessageId: ERROR_CANT_TERMINATE_SELF
    //
    // MessageText:
    //
    // Indicates that a thread attempted to terminate itself by default (called
    // NtTerminateThread with NUL; and it was the last thread in the current
    // process.
    //
    int ERROR_CANT_TERMINATE_SELF = 555;

    //
    // MessageId: ERROR_UNEXPECTED_MM_CREATE_ERR
    //
    // MessageText:
    //
    // If an MM error is returned which is not defined in the standard FsRtl
    // filter, it is converted to one of the following errors which is
    // guaranteed to be in the filter.
    // In this case information is lost, however, the filter correctly handles
    // the exception.
    //
    int ERROR_UNEXPECTED_MM_CREATE_ERR = 556;

    //
    // MessageId: ERROR_UNEXPECTED_MM_MAP_ERROR
    //
    // MessageText:
    //
    // If an MM error is returned which is not defined in the standard FsRtl
    // filter, it is converted to one of the following errors which is
    // guaranteed to be in the filter.
    // In this case information is lost, however, the filter correctly handles
    // the exception.
    //
    int ERROR_UNEXPECTED_MM_MAP_ERROR = 557;

    //
    // MessageId: ERROR_UNEXPECTED_MM_EXTEND_ERR
    //
    // MessageText:
    //
    // If an MM error is returned which is not defined in the standard FsRtl
    // filter, it is converted to one of the following errors which is
    // guaranteed to be in the filter.
    // In this case information is lost, however, the filter correctly handles
    // the exception.
    //
    int ERROR_UNEXPECTED_MM_EXTEND_ERR = 558;

    //
    // MessageId: ERROR_BAD_FUNCTION_TABLE
    //
    // MessageText:
    //
    // A malformed function table was encountered during an unwind operation.
    //
    int ERROR_BAD_FUNCTION_TABLE = 559;

    //
    // MessageId: ERROR_NO_GUID_TRANSLATION
    //
    // MessageText:
    //
    // Indicates that an attempt was made to assign protection to a file system
    // file or directory and one of the SIDs in the security descriptor could
    // not be translated into a GUID that could be stored by the file system.
    // This causes the protection attempt to fail, which may cause a file
    // creation attempt to fail.
    //
    int ERROR_NO_GUID_TRANSLATION = 560;

    //
    // MessageId: ERROR_INVALID_LDT_SIZE
    //
    // MessageText:
    //
    // Indicates that an attempt was made to grow an LDT by setting its size, or
    // that the size was not an even number of selectors.
    //
    int ERROR_INVALID_LDT_SIZE = 561;

    //
    // MessageId: ERROR_INVALID_LDT_OFFSET
    //
    // MessageText:
    //
    // Indicates that the starting value for the LDT information was not an
    // integral multiple of the selector size.
    //
    int ERROR_INVALID_LDT_OFFSET = 563;

    //
    // MessageId: ERROR_INVALID_LDT_DESCRIPTOR
    //
    // MessageText:
    //
    // Indicates that the user supplied an invalid descriptor when trying to set
    // up Ldt descriptors.
    //
    int ERROR_INVALID_LDT_DESCRIPTOR = 564;

    //
    // MessageId: ERROR_TOO_MANY_THREADS
    //
    // MessageText:
    //
    // Indicates a process has too many threads to perform the requested action.
    // For example, assignment of a primary token may only be performed when a
    // process has zero or one threads.
    //
    int ERROR_TOO_MANY_THREADS = 565;

    //
    // MessageId: ERROR_THREAD_NOT_IN_PROCESS
    //
    // MessageText:
    //
    // An attempt was made to operate on a thread within a specific process, but
    // the thread specified is not in the process specified.
    //
    int ERROR_THREAD_NOT_IN_PROCESS = 566;

    //
    // MessageId: ERROR_PAGEFILE_QUOTA_EXCEEDED
    //
    // MessageText:
    //
    // Page file quota was exceeded.
    //
    int ERROR_PAGEFILE_QUOTA_EXCEEDED = 567;

    //
    // MessageId: ERROR_LOGON_SERVER_CONFLICT
    //
    // MessageText:
    //
    // The Netlogon service cannot start because another Netlogon service
    // running in the domain conflicts with the specified role.
    //
    int ERROR_LOGON_SERVER_CONFLICT = 568;

    //
    // MessageId: ERROR_SYNCHRONIZATION_REQUIRED
    //
    // MessageText:
    //
    // The SAM database on a Windows Server is significantly out of
    // synchronization with the copy on the Domain Controller. A complete
    // synchronization is required.
    //
    int ERROR_SYNCHRONIZATION_REQUIRED = 569;

    //
    // MessageId: ERROR_NET_OPEN_FAILED
    //
    // MessageText:
    //
    // The NtCreateFile API failed. This error should never be returned to an
    // application, it is a place holder for the Windows Lan Manager Redirector
    // to use in its internal error mapping routines.
    //
    int ERROR_NET_OPEN_FAILED = 570;

    //
    // MessageId: ERROR_IO_PRIVILEGE_FAILED
    //
    // MessageText:
    //
    // {Privilege Failed}
    // The I/O permissions for the process could not be changed.
    //
    int ERROR_IO_PRIVILEGE_FAILED = 571;

    //
    // MessageId: ERROR_CONTROL_C_EXIT
    //
    // MessageText:
    //
    // {Application Exit by CTRL+C}
    // The application terminated as a result of a CTRL+C.
    //
    int ERROR_CONTROL_C_EXIT = 572; // winnt

    //
    // MessageId: ERROR_MISSING_SYSTEMFILE
    //
    // MessageText:
    //
    // {Missing System File}
    // The required system file %hs is bad or missing.
    //
    int ERROR_MISSING_SYSTEMFILE = 573;

    //
    // MessageId: ERROR_UNHANDLED_EXCEPTION
    //
    // MessageText:
    //
    // {Application Error}
    // The exception %s (0x%08;x) occurred in the application at location
    // 0x%08;x.
    //
    int ERROR_UNHANDLED_EXCEPTION = 574;

    //
    // MessageId: ERROR_APP_INIT_FAILURE
    //
    // MessageText:
    //
    // {Application Error}
    // The application failed to initialize properly (0x%lx). Click OK to
    // terminate the application.
    //
    int ERROR_APP_INIT_FAILURE = 575;

    //
    // MessageId: ERROR_PAGEFILE_CREATE_FAILED
    //
    // MessageText:
    //
    // {Unable to Create Paging File}
    // The creation of the paging file %hs failed (%lx). The requested size was
    // %ld.
    //
    int ERROR_PAGEFILE_CREATE_FAILED = 576;

    //
    // MessageId: ERROR_INVALID_IMAGE_HASH
    //
    // MessageText:
    //
    // Windows cannot verify the digital signature for this file. A recent
    // hardware or software change might have installed a file that is signed
    // incorrectly or damaged, or that might be malicious software from an
    // unknown source.
    //
    int ERROR_INVALID_IMAGE_HASH = 577;

    //
    // MessageId: ERROR_NO_PAGEFILE
    //
    // MessageText:
    //
    // {No Paging File Specified}
    // No paging file was specified in the system configuration.
    //
    int ERROR_NO_PAGEFILE = 578;

    //
    // MessageId: ERROR_ILLEGAL_FLOAT_CONTEXT
    //
    // MessageText:
    //
    // {EXCEPTION}
    // A real-mode application issued a floating-point instruction and
    // floating-point hardware is not present.
    //
    int ERROR_ILLEGAL_FLOAT_CONTEXT = 579;

    //
    // MessageId: ERROR_NO_EVENT_PAIR
    //
    // MessageText:
    //
    // An event pair synchronization operation was performed using the thread
    // specific client/server event pair object, but no event pair object was
    // associated with the thread.
    //
    int ERROR_NO_EVENT_PAIR = 580;

    //
    // MessageId: ERROR_DOMAIN_CTRLR_CONFIG_ERROR
    //
    // MessageText:
    //
    // A Windows Server has an incorrect configuration.
    //
    int ERROR_DOMAIN_CTRLR_CONFIG_ERROR = 581;

    //
    // MessageId: ERROR_ILLEGAL_CHARACTER
    //
    // MessageText:
    //
    // An illegal character was encountered. For a multi-byte character set this
    // includes a lead byte without a succeeding trail byte. For the Unicode
    // character set this includes the characters 0xFFFF and 0xFFFE.
    //
    int ERROR_ILLEGAL_CHARACTER = 582;

    //
    // MessageId: ERROR_UNDEFINED_CHARACTER
    //
    // MessageText:
    //
    // The Unicode character is not defined in the Unicode character set
    // installed on the system.
    //
    int ERROR_UNDEFINED_CHARACTER = 583;

    //
    // MessageId: ERROR_FLOPPY_VOLUME
    //
    // MessageText:
    //
    // The paging file cannot be created on a floppy diskette.
    //
    int ERROR_FLOPPY_VOLUME = 584;

    //
    // MessageId: ERROR_BIOS_FAILED_TO_CONNECT_INTERRUPT
    //
    // MessageText:
    //
    // The system BIOS failed to connect a system interrupt to the device or bus
    // for which the device is connected.
    //
    int ERROR_BIOS_FAILED_TO_CONNECT_INTERRUPT = 585;

    //
    // MessageId: ERROR_BACKUP_CONTROLLER
    //
    // MessageText:
    //
    // This operation is only allowed for the Primary Domain Controller of the
    // domain.
    //
    int ERROR_BACKUP_CONTROLLER = 586;

    //
    // MessageId: ERROR_MUTANT_LIMIT_EXCEEDED
    //
    // MessageText:
    //
    // An attempt was made to acquire a mutant such that its maximum count would
    // have been exceeded.
    //
    int ERROR_MUTANT_LIMIT_EXCEEDED = 587;

    //
    // MessageId: ERROR_FS_DRIVER_REQUIRED
    //
    // MessageText:
    //
    // A volume has been accessed for which a file system driver is required
    // that has not yet been loaded.
    //
    int ERROR_FS_DRIVER_REQUIRED = 588;

    //
    // MessageId: ERROR_CANNOT_LOAD_REGISTRY_FILE
    //
    // MessageText:
    //
    // {Registry File Failure}
    // The registry cannot load the hive (file):
    // %hs
    // or its log or alternate.
    // It is corrupt, absent, or not writable.
    //
    int ERROR_CANNOT_LOAD_REGISTRY_FILE = 589;

    //
    // MessageId: ERROR_DEBUG_ATTACH_FAILED
    //
    // MessageText:
    //
    // {Unexpected Failure in DebugActiveProcess}
    // An unexpected failure occurred while processing a DebugActiveProcess API
    // request. You may choose OK to terminate the process, or Cancel to ignore
    // the error.
    //
    int ERROR_DEBUG_ATTACH_FAILED = 590;

    //
    // MessageId: ERROR_SYSTEM_PROCESS_TERMINATED
    //
    // MessageText:
    //
    // {Fatal System Error}
    // The %hs system process terminated unexpectedly with a status of 0x%08x
    // (0x%08x 0x%08x).
    // The system has been shut down.
    //
    int ERROR_SYSTEM_PROCESS_TERMINATED = 591;

    //
    // MessageId: ERROR_DATA_NOT_ACCEPTED
    //
    // MessageText:
    //
    // {Data Not Accepted}
    // The TDI client could not handle the data received during an indication.
    //
    int ERROR_DATA_NOT_ACCEPTED = 592;

    //
    // MessageId: ERROR_VDM_HARD_ERROR
    //
    // MessageText:
    //
    // NTVDM encountered a hard error.
    //
    int ERROR_VDM_HARD_ERROR = 593;

    //
    // MessageId: ERROR_DRIVER_CANCEL_TIMEOUT
    //
    // MessageText:
    //
    // {Cancel Timeout}
    // The driver %hs failed to complete a cancelled I/O request in the allotted
    // time.
    //
    int ERROR_DRIVER_CANCEL_TIMEOUT = 594;

    //
    // MessageId: ERROR_REPLY_MESSAGE_MISMATCH
    //
    // MessageText:
    //
    // {Reply Message Mismatch}
    // An attempt was made to reply to an LPC message, but the thread specified
    // by the client ID in the message was not waiting on that message.
    //
    int ERROR_REPLY_MESSAGE_MISMATCH = 595;

    //
    // MessageId: ERROR_LOST_WRITEBEHIND_DATA
    //
    // MessageText:
    //
    // {Delayed Write Failed}
    // Windows was unable to save all the data for the file %hs. The data has
    // been lost.
    // This error may be caused by a failure of your computer hardware or
    // network connection. Please try to save this file elsewhere.
    //
    int ERROR_LOST_WRITEBEHIND_DATA = 596;

    //
    // MessageId: ERROR_CLIENT_SERVER_PARAMETERS_INVALID
    //
    // MessageText:
    //
    // The parameter(s) passed to the server in the client/server shared memory
    // window were invalid. Too much data may have been put in the shared memory
    // window.
    //
    int ERROR_CLIENT_SERVER_PARAMETERS_INVALID = 597;

    //
    // MessageId: ERROR_NOT_TINY_STREAM
    //
    // MessageText:
    //
    // The stream is not a tiny stream.
    //
    int ERROR_NOT_TINY_STREAM = 598;

    //
    // MessageId: ERROR_STACK_OVERFLOW_READ
    //
    // MessageText:
    //
    // The request must be handled by the stack overflow code.
    //
    int ERROR_STACK_OVERFLOW_READ = 599;

    //
    // MessageId: ERROR_CONVERT_TO_LARGE
    //
    // MessageText:
    //
    // Internal OFS status codes indicating how an allocation operation is
    // handled. Either it is retried after the containing onode is moved or the
    // extent stream is converted to a large stream.
    //
    int ERROR_CONVERT_TO_LARGE = 600;

    //
    // MessageId: ERROR_FOUND_OUT_OF_SCOPE
    //
    // MessageText:
    //
    // The attempt to find the object found an object matching by ID on the
    // volume but it is out of the scope of the handle used for the operation.
    //
    int ERROR_FOUND_OUT_OF_SCOPE = 601;

    //
    // MessageId: ERROR_ALLOCATE_BUCKET
    //
    // MessageText:
    //
    // The bucket array must be grown. Retry transaction after doing so.
    //
    int ERROR_ALLOCATE_BUCKET = 602;

    //
    // MessageId: ERROR_MARSHALL_OVERFLOW
    //
    // MessageText:
    //
    // The user/kernel marshalling buffer has overflowed.
    //
    int ERROR_MARSHALL_OVERFLOW = 603;

    //
    // MessageId: ERROR_INVALID_VARIANT
    //
    // MessageText:
    //
    // The supplied variant structure contains invalid data.
    //
    int ERROR_INVALID_VARIANT = 604;

    //
    // MessageId: ERROR_BAD_COMPRESSION_BUFFER
    //
    // MessageText:
    //
    // The specified buffer contains ill-formed data.
    //
    int ERROR_BAD_COMPRESSION_BUFFER = 605;

    //
    // MessageId: ERROR_AUDIT_FAILED
    //
    // MessageText:
    //
    // {Audit Failed}
    // An attempt to generate a security audit failed.
    //
    int ERROR_AUDIT_FAILED = 606;

    //
    // MessageId: ERROR_TIMER_RESOLUTION_NOT_SET
    //
    // MessageText:
    //
    // The timer resolution was not previously set by the current process.
    //
    int ERROR_TIMER_RESOLUTION_NOT_SET = 607;

    //
    // MessageId: ERROR_INSUFFICIENT_LOGON_INFO
    //
    // MessageText:
    //
    // There is insufficient account information to log you on.
    //
    int ERROR_INSUFFICIENT_LOGON_INFO = 608;

    //
    // MessageId: ERROR_BAD_DLL_ENTRYPOINT
    //
    // MessageText:
    //
    // {Invalid DLL Entrypoint}
    // The dynamic link library %hs is not written correctly. The stack pointer
    // has been left in an inconsistent state.
    // The entrypoint should be declared as WINAPI or STDCALL. Select YES to
    // fail the DLL load. Select NO to continue execution. Selecting NO may
    // cause the application to operate incorrectly.
    //
    int ERROR_BAD_DLL_ENTRYPOINT = 609;

    //
    // MessageId: ERROR_BAD_SERVICE_ENTRYPOINT
    //
    // MessageText:
    //
    // {Invalid Service Callback Entrypoint}
    // The %hs service is not written correctly. The stack pointer has been left
    // in an inconsistent state.
    // The callback entrypoint should be declared as WINAPI or STDCALL.
    // Selecting OK will cause the service to continue operation. However, the
    // service process may operate incorrectly.
    //
    int ERROR_BAD_SERVICE_ENTRYPOINT = 610;

    //
    // MessageId: ERROR_IP_ADDRESS_CONFLICT1
    //
    // MessageText:
    //
    // There is an IP address conflict with another system on the network
    //
    int ERROR_IP_ADDRESS_CONFLICT1 = 611;

    //
    // MessageId: ERROR_IP_ADDRESS_CONFLICT2
    //
    // MessageText:
    //
    // There is an IP address conflict with another system on the network
    //
    int ERROR_IP_ADDRESS_CONFLICT2 = 612;

    //
    // MessageId: ERROR_REGISTRY_QUOTA_LIMIT
    //
    // MessageText:
    //
    // {Low On Registry Space}
    // The system has reached the maximum size allowed for the system part of
    // the registry. Additional storage requests will be ignored.
    //
    int ERROR_REGISTRY_QUOTA_LIMIT = 613;

    //
    // MessageId: ERROR_NO_CALLBACK_ACTIVE
    //
    // MessageText:
    //
    // A callback return system service cannot be executed when no callback is
    // active.
    //
    int ERROR_NO_CALLBACK_ACTIVE = 614;

    //
    // MessageId: ERROR_PWD_TOO_SHORT
    //
    // MessageText:
    //
    // The password provided is too short to meet the policy of your user
    // account.
    // Please choose a longer password.
    //
    int ERROR_PWD_TOO_SHORT = 615;

    //
    // MessageId: ERROR_PWD_TOO_RECENT
    //
    // MessageText:
    //
    // The policy of your user account does not allow you to change passwords
    // too frequently.
    // This is done to prevent users from changing back to a familiar, but
    // potentially discovered, password.
    // If you feel your password has been compromised then please contact your
    // administrator immediately to have a new one assigned.
    //
    int ERROR_PWD_TOO_RECENT = 616;

    //
    // MessageId: ERROR_PWD_HISTORY_CONFLICT
    //
    // MessageText:
    //
    // You have attempted to change your password to one that you have used in
    // the past.
    // The policy of your user account does not allow this. Please select a
    // password that you have not previously used.
    //
    int ERROR_PWD_HISTORY_CONFLICT = 617;

    //
    // MessageId: ERROR_UNSUPPORTED_COMPRESSION
    //
    // MessageText:
    //
    // The specified compression format is unsupported.
    //
    int ERROR_UNSUPPORTED_COMPRESSION = 618;

    //
    // MessageId: ERROR_INVALID_HW_PROFILE
    //
    // MessageText:
    //
    // The specified hardware profile configuration is invalid.
    //
    int ERROR_INVALID_HW_PROFILE = 619;

    //
    // MessageId: ERROR_INVALID_PLUGPLAY_DEVICE_PATH
    //
    // MessageText:
    //
    // The specified Plug and Play registry device path is invalid.
    //
    int ERROR_INVALID_PLUGPLAY_DEVICE_PATH = 620;

    //
    // MessageId: ERROR_QUOTA_LIST_INCONSISTENT
    //
    // MessageText:
    //
    // The specified quota list is internally inconsistent with its descriptor.
    //
    int ERROR_QUOTA_LIST_INCONSISTENT = 621;

    //
    // MessageId: ERROR_EVALUATION_EXPIRATION
    //
    // MessageText:
    //
    // {Windows Evaluation Notification}
    // The evaluation period for this installation of Windows has expired. This
    // system will shutdown in=1 hour. To restore access to this installation of
    // Windows, please upgrade this installation using a licensed distribution
    // of this product.
    //
    int ERROR_EVALUATION_EXPIRATION = 622;

    //
    // MessageId: ERROR_ILLEGAL_DLL_RELOCATION
    //
    // MessageText:
    //
    // {Illegal System DLL Relocation}
    // The system DLL %hs was relocated in memory. The application will not run
    // properly.
    // The relocation occurred because the DLL %hs occupied an address range
    // reserved for Windows system DLLs. The vendor supplying the DLL should be
    // contacted for a new DLL.
    //
    int ERROR_ILLEGAL_DLL_RELOCATION = 623;

    //
    // MessageId: ERROR_DLL_INIT_FAILED_LOGOFF
    //
    // MessageText:
    //
    // {DLL Initialization Failed}
    // The application failed to initialize because the window station is
    // shutting down.
    //
    int ERROR_DLL_INIT_FAILED_LOGOFF = 624;

    //
    // MessageId: ERROR_VALIDATE_CONTINUE
    //
    // MessageText:
    //
    // The validation process needs to continue on to the next step.
    //
    int ERROR_VALIDATE_CONTINUE = 625;

    //
    // MessageId: ERROR_NO_MORE_MATCHES
    //
    // MessageText:
    //
    // There are no more matches for the current index enumeration.
    //
    int ERROR_NO_MORE_MATCHES = 626;

    //
    // MessageId: ERROR_RANGE_LIST_CONFLICT
    //
    // MessageText:
    //
    // The range could not be added to the range list because of a conflict.
    //
    int ERROR_RANGE_LIST_CONFLICT = 627;

    //
    // MessageId: ERROR_SERVER_SID_MISMATCH
    //
    // MessageText:
    //
    // The server process is running under a SID different than that required by
    // client.
    //
    int ERROR_SERVER_SID_MISMATCH = 628;

    //
    // MessageId: ERROR_CANT_ENABLE_DENY_ONLY
    //
    // MessageText:
    //
    // A group marked use for deny only cannot be enabled.
    //
    int ERROR_CANT_ENABLE_DENY_ONLY = 629;

    //
    // MessageId: ERROR_FLOAT_MULTIPLE_FAULTS
    //
    // MessageText:
    //
    // {EXCEPTION}
    // Multiple floating point faults.
    //
    int ERROR_FLOAT_MULTIPLE_FAULTS = 630; // winnt

    //
    // MessageId: ERROR_FLOAT_MULTIPLE_TRAPS
    //
    // MessageText:
    //
    // {EXCEPTION}
    // Multiple floating point traps.
    //
    int ERROR_FLOAT_MULTIPLE_TRAPS = 631; // winnt

    //
    // MessageId: ERROR_NOINTERFACE
    //
    // MessageText:
    //
    // The requested interface is not supported.
    //
    int ERROR_NOINTERFACE = 632;

    //
    // MessageId: ERROR_DRIVER_FAILED_SLEEP
    //
    // MessageText:
    //
    // {System Standby Failed}
    // The driver %hs does not support standby mode. Updating this driver may
    // allow the system to go to standby mode.
    //
    int ERROR_DRIVER_FAILED_SLEEP = 633;

    //
    // MessageId: ERROR_CORRUPT_SYSTEM_FILE
    //
    // MessageText:
    //
    // The system file %1 has become corrupt and has been replaced.
    //
    int ERROR_CORRUPT_SYSTEM_FILE = 634;

    //
    // MessageId: ERROR_COMMITMENT_MINIMUM
    //
    // MessageText:
    //
    // {Virtual Memory Minimum Too Low}
    // Your system is low on virtual memory. Windows is increasing the size of
    // your virtual memory paging file.
    // During this process, memory requests for some applications may be denied.
    // For more information, see Help.
    //
    int ERROR_COMMITMENT_MINIMUM = 635;

    //
    // MessageId: ERROR_PNP_RESTART_ENUMERATION
    //
    // MessageText:
    //
    // A device was removed so enumeration must be restarted.
    //
    int ERROR_PNP_RESTART_ENUMERATION = 636;

    //
    // MessageId: ERROR_SYSTEM_IMAGE_BAD_SIGNATURE
    //
    // MessageText:
    //
    // {Fatal System Error}
    // The system image %s is not properly signed.
    // The file has been replaced with the signed file.
    // The system has been shut down.
    //
    int ERROR_SYSTEM_IMAGE_BAD_SIGNATURE = 637;

    //
    // MessageId: ERROR_PNP_REBOOT_REQUIRED
    //
    // MessageText:
    //
    // Device will not start without a reboot.
    //
    int ERROR_PNP_REBOOT_REQUIRED = 638;

    //
    // MessageId: ERROR_INSUFFICIENT_POWER
    //
    // MessageText:
    //
    // There is not enough power to complete the requested operation.
    //
    int ERROR_INSUFFICIENT_POWER = 639;

    //
    // MessageId: ERROR_MULTIPLE_FAULT_VIOLATION
    //
    // MessageText:
    //
    // ERROR_MULTIPLE_FAULT_VIOLATION
    //
    int ERROR_MULTIPLE_FAULT_VIOLATION = 640;

    //
    // MessageId: ERROR_SYSTEM_SHUTDOWN
    //
    // MessageText:
    //
    // The system is in the process of shutting down.
    //
    int ERROR_SYSTEM_SHUTDOWN = 641;

    //
    // MessageId: ERROR_PORT_NOT_SET
    //
    // MessageText:
    //
    // An attempt to remove a processes DebugPort was made, but a port was not
    // already associated with the process.
    //
    int ERROR_PORT_NOT_SET = 642;

    //
    // MessageId: ERROR_DS_VERSION_CHECK_FAILURE
    //
    // MessageText:
    //
    // This version of Windows is not compatible with the behavior version of
    // directory forest, domain or domain controller.
    //
    int ERROR_DS_VERSION_CHECK_FAILURE = 643;

    //
    // MessageId: ERROR_RANGE_NOT_FOUND
    //
    // MessageText:
    //
    // The specified range could not be found in the range list.
    //
    int ERROR_RANGE_NOT_FOUND = 644;

    //
    // MessageId: ERROR_NOT_SAFE_MODE_DRIVER
    //
    // MessageText:
    //
    // The driver was not loaded because the system is booting into safe mode.
    //
    int ERROR_NOT_SAFE_MODE_DRIVER = 646;

    //
    // MessageId: ERROR_FAILED_DRIVER_ENTRY
    //
    // MessageText:
    //
    // The driver was not loaded because it failed it's initialization call.
    //
    int ERROR_FAILED_DRIVER_ENTRY = 647;

    //
    // MessageId: ERROR_DEVICE_ENUMERATION_ERROR
    //
    // MessageText:
    //
    // The "%hs" encountered an error while applying power or reading the device
    // configuration.
    // This may be caused by a failure of your hardware or by a poor connection.
    //
    int ERROR_DEVICE_ENUMERATION_ERROR = 648;

    //
    // MessageId: ERROR_MOUNT_POINT_NOT_RESOLVED
    //
    // MessageText:
    //
    // The create operation failed because the name contained at least one mount
    // point which resolves to a volume to which the specified device object is
    // not attached.
    //
    int ERROR_MOUNT_POINT_NOT_RESOLVED = 649;

    //
    // MessageId: ERROR_INVALID_DEVICE_OBJECT_PARAMETER
    //
    // MessageText:
    //
    // The device object parameter is either not a valid device object or is not
    // attached to the volume specified by the file name.
    //
    int ERROR_INVALID_DEVICE_OBJECT_PARAMETER = 650;

    //
    // MessageId: ERROR_MCA_OCCURED
    //
    // MessageText:
    //
    // A Machine Check Error has occurred. Please check the system eventlog for
    // additional information.
    //
    int ERROR_MCA_OCCURED = 651;

    //
    // MessageId: ERROR_DRIVER_DATABASE_ERROR
    //
    // MessageText:
    //
    // There was error [%2] processing the driver database.
    //
    int ERROR_DRIVER_DATABASE_ERROR = 652;

    //
    // MessageId: ERROR_SYSTEM_HIVE_TOO_LARGE
    //
    // MessageText:
    //
    // System hive size has exceeded its limit.
    //
    int ERROR_SYSTEM_HIVE_TOO_LARGE = 653;

    //
    // MessageId: ERROR_DRIVER_FAILED_PRIOR_UNLOAD
    //
    // MessageText:
    //
    // The driver could not be loaded because a previous version of the driver
    // is still in memory.
    //
    int ERROR_DRIVER_FAILED_PRIOR_UNLOAD = 654;

    //
    // MessageId: ERROR_VOLSNAP_PREPARE_HIBERNATE
    //
    // MessageText:
    //
    // {Volume Shadow Copy Service}
    // Please wait while the Volume Shadow Copy Service prepares volume %hs for
    // hibernation.
    //
    int ERROR_VOLSNAP_PREPARE_HIBERNATE = 655;

    //
    // MessageId: ERROR_HIBERNATION_FAILURE
    //
    // MessageText:
    //
    // The system has failed to hibernate (The error code is %hs). Hibernation
    // will be disabled until the system is restarted.
    //
    int ERROR_HIBERNATION_FAILURE = 656;

    //
    // MessageId: ERROR_FILE_SYSTEM_LIMITATION
    //
    // MessageText:
    //
    // The requested operation could not be completed due to a file system
    // limitation
    //
    int ERROR_FILE_SYSTEM_LIMITATION = 665;

    //
    // MessageId: ERROR_ASSERTION_FAILURE
    //
    // MessageText:
    //
    // An assertion failure has occurred.
    //
    int ERROR_ASSERTION_FAILURE = 668;

    //
    // MessageId: ERROR_ACPI_ERROR
    //
    // MessageText:
    //
    // An error occurred in the ACPI subsystem.
    //
    int ERROR_ACPI_ERROR = 669;

    //
    // MessageId: ERROR_WOW_ASSERTION
    //
    // MessageText:
    //
    // WOW Assertion Error.
    //
    int ERROR_WOW_ASSERTION = 670;

    //
    // MessageId: ERROR_PNP_BAD_MPS_TABLE
    //
    // MessageText:
    //
    // A device is missing in the system BIOS MPS table. This device will not be
    // used.
    // Please contact your system vendor for system BIOS update.
    //
    int ERROR_PNP_BAD_MPS_TABLE = 671;

    //
    // MessageId: ERROR_PNP_TRANSLATION_FAILED
    //
    // MessageText:
    //
    // A translator failed to translate resources.
    //
    int ERROR_PNP_TRANSLATION_FAILED = 672;

    //
    // MessageId: ERROR_PNP_IRQ_TRANSLATION_FAILED
    //
    // MessageText:
    //
    // A IRQ translator failed to translate resources.
    //
    int ERROR_PNP_IRQ_TRANSLATION_FAILED = 673;

    //
    // MessageId: ERROR_PNP_INVALID_ID
    //
    // MessageText:
    //
    // Driver %2 returned invalid ID for a child device (%3).
    //
    int ERROR_PNP_INVALID_ID = 674;

    //
    // MessageId: ERROR_WAKE_SYSTEM_DEBUGGER
    //
    // MessageText:
    //
    // {Kernel Debugger Awakened}
    // the system debugger was awakened by an interrupt.
    //
    int ERROR_WAKE_SYSTEM_DEBUGGER = 675;

    //
    // MessageId: ERROR_HANDLES_CLOSED
    //
    // MessageText:
    //
    // {Handles Closed}
    // Handles to objects have been automatically closed as a result of the
    // requested operation.
    //
    int ERROR_HANDLES_CLOSED = 676;

    //
    // MessageId: ERROR_EXTRANEOUS_INFORMATION
    //
    // MessageText:
    //
    // {Too Much Information}
    // The specified access control list (AC; contained more information than
    // was expected.
    //
    int ERROR_EXTRANEOUS_INFORMATION = 677;

    //
    // MessageId: ERROR_RXACT_COMMIT_NECESSARY
    //
    // MessageText:
    //
    // This warning level status indicates that the transaction state already
    // exists for the registry sub-tree, but that a transaction commit was
    // previously aborted.
    // The commit has NOT been completed, but has not been rolled back either
    // (so it may still be committed if desired).
    //
    int ERROR_RXACT_COMMIT_NECESSARY = 678;

    //
    // MessageId: ERROR_MEDIA_CHECK
    //
    // MessageText:
    //
    // {Media Changed}
    // The media may have changed.
    //
    int ERROR_MEDIA_CHECK = 679;

    //
    // MessageId: ERROR_GUID_SUBSTITUTION_MADE
    //
    // MessageText:
    //
    // {GUID Substitution}
    // During the translation of a global identifier (GUID) to a Windows
    // security ID (SID), no administratively-defined GUID prefix was found.
    // A substitute prefix was used, which will not compromise system security.
    // However, this may provide a more restrictive access than intended.
    //
    int ERROR_GUID_SUBSTITUTION_MADE = 680;

    //
    // MessageId: ERROR_STOPPED_ON_SYMLINK
    //
    // MessageText:
    //
    // The create operation stopped after reaching a symbolic link
    //
    int ERROR_STOPPED_ON_SYMLINK = 681;

    //
    // MessageId: ERROR_LONGJUMP
    //
    // MessageText:
    //
    // A long jump has been executed.
    //
    int ERROR_LONGJUMP = 682;

    //
    // MessageId: ERROR_PLUGPLAY_QUERY_VETOED
    //
    // MessageText:
    //
    // The Plug and Play query operation was not successful.
    //
    int ERROR_PLUGPLAY_QUERY_VETOED = 683;

    //
    // MessageId: ERROR_UNWIND_CONSOLIDATE
    //
    // MessageText:
    //
    // A frame consolidation has been executed.
    //
    int ERROR_UNWIND_CONSOLIDATE = 684;

    //
    // MessageId: ERROR_REGISTRY_HIVE_RECOVERED
    //
    // MessageText:
    //
    // {Registry Hive Recovered}
    // Registry hive (file):
    // %hs
    // was corrupted and it has been recovered. Some data might have been lost.
    //
    int ERROR_REGISTRY_HIVE_RECOVERED = 685;

    //
    // MessageId: ERROR_DLL_MIGHT_BE_INSECURE
    //
    // MessageText:
    //
    // The application is attempting to run executable code from the module %hs.
    // This may be insecure. An alternative, %hs, is available. Should the
    // application use the secure module %hs?
    //
    int ERROR_DLL_MIGHT_BE_INSECURE = 686;

    //
    // MessageId: ERROR_DLL_MIGHT_BE_INCOMPATIBLE
    //
    // MessageText:
    //
    // The application is loading executable code from the module %hs. This is
    // secure, but may be incompatible with previous releases of the operating
    // system. An alternative, %hs, is available. Should the application use the
    // secure module %hs?
    //
    int ERROR_DLL_MIGHT_BE_INCOMPATIBLE = 687;

    //
    // MessageId: ERROR_DBG_EXCEPTION_NOT_HANDLED
    //
    // MessageText:
    //
    // Debugger did not handle the exception.
    //
    int ERROR_DBG_EXCEPTION_NOT_HANDLED = 688; // winnt

    //
    // MessageId: ERROR_DBG_REPLY_LATER
    //
    // MessageText:
    //
    // Debugger will reply later.
    //
    int ERROR_DBG_REPLY_LATER = 689;

    //
    // MessageId: ERROR_DBG_UNABLE_TO_PROVIDE_HANDLE
    //
    // MessageText:
    //
    // Debugger cannot provide handle.
    //
    int ERROR_DBG_UNABLE_TO_PROVIDE_HANDLE = 690;

    //
    // MessageId: ERROR_DBG_TERMINATE_THREAD
    //
    // MessageText:
    //
    // Debugger terminated thread.
    //
    int ERROR_DBG_TERMINATE_THREAD = 691; // winnt

    //
    // MessageId: ERROR_DBG_TERMINATE_PROCESS
    //
    // MessageText:
    //
    // Debugger terminated process.
    //
    int ERROR_DBG_TERMINATE_PROCESS = 692; // winnt

    //
    // MessageId: ERROR_DBG_CONTROL_C
    //
    // MessageText:
    //
    // Debugger got control C.
    //
    int ERROR_DBG_CONTROL_C = 693; // winnt

    //
    // MessageId: ERROR_DBG_PRINTEXCEPTION_C
    //
    // MessageText:
    //
    // Debugger printed exception on control C.
    //
    int ERROR_DBG_PRINTEXCEPTION_C = 694;

    //
    // MessageId: ERROR_DBG_RIPEXCEPTION
    //
    // MessageText:
    //
    // Debugger received RIP exception.
    //
    int ERROR_DBG_RIPEXCEPTION = 695;

    //
    // MessageId: ERROR_DBG_CONTROL_BREAK
    //
    // MessageText:
    //
    // Debugger received control break.
    //
    int ERROR_DBG_CONTROL_BREAK = 696; // winnt

    //
    // MessageId: ERROR_DBG_COMMAND_EXCEPTION
    //
    // MessageText:
    //
    // Debugger command communication exception.
    //
    int ERROR_DBG_COMMAND_EXCEPTION = 697; // winnt

    //
    // MessageId: ERROR_OBJECT_NAME_EXISTS
    //
    // MessageText:
    //
    // {Object Exists}
    // An attempt was made to create an object and the object name already
    // existed.
    //
    int ERROR_OBJECT_NAME_EXISTS = 698;

    //
    // MessageId: ERROR_THREAD_WAS_SUSPENDED
    //
    // MessageText:
    //
    // {Thread Suspended}
    // A thread termination occurred while the thread was suspended. The thread
    // was resumed, and termination proceeded.
    //
    int ERROR_THREAD_WAS_SUSPENDED = 699;

    //
    // MessageId: ERROR_IMAGE_NOT_AT_BASE
    //
    // MessageText:
    //
    // {Image Relocated}
    // An image file could not be mapped at the address specified in the image
    // file. Local fixups must be performed on this image.
    //
    int ERROR_IMAGE_NOT_AT_BASE = 700;

    //
    // MessageId: ERROR_RXACT_STATE_CREATED
    //
    // MessageText:
    //
    // This informational level status indicates that a specified registry
    // sub-tree transaction state did not yet exist and had to be created.
    //
    int ERROR_RXACT_STATE_CREATED = 701;

    //
    // MessageId: ERROR_SEGMENT_NOTIFICATION
    //
    // MessageText:
    //
    // {Segment Load}
    // A virtual DOS machine (VDM) is loading, unloading, or moving an MS-DOS or
    // Win16 program segment image.
    // An exception is raised so a debugger can load, unload or track symbols
    // and breakpoints within these=16-bit segments.
    //
    int ERROR_SEGMENT_NOTIFICATION = 702; // winnt

    //
    // MessageId: ERROR_BAD_CURRENT_DIRECTORY
    //
    // MessageText:
    //
    // {Invalid Current Directory}
    // The process cannot switch to the startup current directory %hs.
    // Select OK to set current directory to %hs, or select CANCEL to exit.
    //
    int ERROR_BAD_CURRENT_DIRECTORY = 703;

    //
    // MessageId: ERROR_FT_READ_RECOVERY_FROM_BACKUP
    //
    // MessageText:
    //
    // {Redundant Read}
    // To satisfy a read request, the NT fault-tolerant file system successfully
    // read the requested data from a redundant copy.
    // This was done because the file system encountered a failure on a member
    // of the fault-tolerant volume, but was unable to reassign the failing area
    // of the device.
    //
    int ERROR_FT_READ_RECOVERY_FROM_BACKUP = 704;

    //
    // MessageId: ERROR_FT_WRITE_RECOVERY
    //
    // MessageText:
    //
    // {Redundant Write}
    // To satisfy a write request, the NT fault-tolerant file system
    // successfully wrote a redundant copy of the information.
    // This was done because the file system encountered a failure on a member
    // of the fault-tolerant volume, but was not able to reassign the failing
    // area of the device.
    //
    int ERROR_FT_WRITE_RECOVERY = 705;

    //
    // MessageId: ERROR_IMAGE_MACHINE_TYPE_MISMATCH
    //
    // MessageText:
    //
    // {Machine Type Mismatch}
    // The image file %hs is valid, but is for a machine type other than the
    // current machine. Select OK to continue, or CANCEL to fail the DLL load.
    //
    int ERROR_IMAGE_MACHINE_TYPE_MISMATCH = 706;

    //
    // MessageId: ERROR_RECEIVE_PARTIAL
    //
    // MessageText:
    //
    // {Partial Data Received}
    // The network transport returned partial data to its client. The remaining
    // data will be sent later.
    //
    int ERROR_RECEIVE_PARTIAL = 707;

    //
    // MessageId: ERROR_RECEIVE_EXPEDITED
    //
    // MessageText:
    //
    // {Expedited Data Received}
    // The network transport returned data to its client that was marked as
    // expedited by the remote system.
    //
    int ERROR_RECEIVE_EXPEDITED = 708;

    //
    // MessageId: ERROR_RECEIVE_PARTIAL_EXPEDITED
    //
    // MessageText:
    //
    // {Partial Expedited Data Received}
    // The network transport returned partial data to its client and this data
    // was marked as expedited by the remote system. The remaining data will be
    // sent later.
    //
    int ERROR_RECEIVE_PARTIAL_EXPEDITED = 709;

    //
    // MessageId: ERROR_EVENT_DONE
    //
    // MessageText:
    //
    // {TDI Event Done}
    // The TDI indication has completed successfully.
    //
    int ERROR_EVENT_DONE = 710;

    //
    // MessageId: ERROR_EVENT_PENDING
    //
    // MessageText:
    //
    // {TDI Event Pending}
    // The TDI indication has entered the pending state.
    //
    int ERROR_EVENT_PENDING = 711;

    //
    // MessageId: ERROR_CHECKING_FILE_SYSTEM
    //
    // MessageText:
    //
    // Checking file system on %wZ
    //
    int ERROR_CHECKING_FILE_SYSTEM = 712;

    //
    // MessageId: ERROR_FATAL_APP_EXIT
    //
    // MessageText:
    //
    // {Fatal Application Exit}
    // %hs
    //
    int ERROR_FATAL_APP_EXIT = 713;

    //
    // MessageId: ERROR_PREDEFINED_HANDLE
    //
    // MessageText:
    //
    // The specified registry key is referenced by a predefined handle.
    //
    int ERROR_PREDEFINED_HANDLE = 714;

    //
    // MessageId: ERROR_WAS_UNLOCKED
    //
    // MessageText:
    //
    // {Page Unlocked}
    // The page protection of a locked page was changed to 'No Access' and the
    // page was unlocked from memory and from the process.
    //
    int ERROR_WAS_UNLOCKED = 715;

    //
    // MessageId: ERROR_SERVICE_NOTIFICATION
    //
    // MessageText:
    //
    // %hs
    //
    int ERROR_SERVICE_NOTIFICATION = 716;

    //
    // MessageId: ERROR_WAS_LOCKED
    //
    // MessageText:
    //
    // {Page Locked}
    // One of the pages to lock was already locked.
    //
    int ERROR_WAS_LOCKED = 717;

    //
    // MessageId: ERROR_LOG_HARD_ERROR
    //
    // MessageText:
    //
    // Application popup: %1 : %2
    //
    int ERROR_LOG_HARD_ERROR = 718;

    //
    // MessageId: ERROR_ALREADY_WIN32
    //
    // MessageText:
    //
    // ERROR_ALREADY_WIN32
    //
    int ERROR_ALREADY_WIN32 = 719;

    //
    // MessageId: ERROR_IMAGE_MACHINE_TYPE_MISMATCH_EXE
    //
    // MessageText:
    //
    // {Machine Type Mismatch}
    // The image file %hs is valid, but is for a machine type other than the
    // current machine.
    //
    int ERROR_IMAGE_MACHINE_TYPE_MISMATCH_EXE = 720;

    //
    // MessageId: ERROR_NO_YIELD_PERFORMED
    //
    // MessageText:
    //
    // A yield execution was performed and no thread was available to run.
    //
    int ERROR_NO_YIELD_PERFORMED = 721;

    //
    // MessageId: ERROR_TIMER_RESUME_IGNORED
    //
    // MessageText:
    //
    // The resumable flag to a timer API was ignored.
    //
    int ERROR_TIMER_RESUME_IGNORED = 722;

    //
    // MessageId: ERROR_ARBITRATION_UNHANDLED
    //
    // MessageText:
    //
    // The arbiter has deferred arbitration of these resources to its parent
    //
    int ERROR_ARBITRATION_UNHANDLED = 723;

    //
    // MessageId: ERROR_CARDBUS_NOT_SUPPORTED
    //
    // MessageText:
    //
    // The inserted CardBus device cannot be started because of a configuration
    // error on "%hs".
    //
    int ERROR_CARDBUS_NOT_SUPPORTED = 724;

    //
    // MessageId: ERROR_MP_PROCESSOR_MISMATCH
    //
    // MessageText:
    //
    // The CPUs in this multiprocessor system are not all the same revision
    // level. To use all processors the operating system restricts itself to the
    // features of the least capable processor in the system. Should problems
    // occur with this system, contact the CPU manufacturer to see if this mix
    // of processors is supported.
    //
    int ERROR_MP_PROCESSOR_MISMATCH = 725;

    //
    // MessageId: ERROR_HIBERNATED
    //
    // MessageText:
    //
    // The system was put into hibernation.
    //
    int ERROR_HIBERNATED = 726;

    //
    // MessageId: ERROR_RESUME_HIBERNATION
    //
    // MessageText:
    //
    // The system was resumed from hibernation.
    //
    int ERROR_RESUME_HIBERNATION = 727;

    //
    // MessageId: ERROR_FIRMWARE_UPDATED
    //
    // MessageText:
    //
    // Windows has detected that the system firmware (BIOS) was updated
    // [previous firmware date =%2, current firmware date %3].
    //
    int ERROR_FIRMWARE_UPDATED = 728;

    //
    // MessageId: ERROR_DRIVERS_LEAKING_LOCKED_PAGES
    //
    // MessageText:
    //
    // A device driver is leaking locked I/O pages causing system degradation.
    // The system has automatically enabled tracking code in order to try and
    // catch the culprit.
    //
    int ERROR_DRIVERS_LEAKING_LOCKED_PAGES = 729;

    //
    // MessageId: ERROR_WAKE_SYSTEM
    //
    // MessageText:
    //
    // The system has awoken
    //
    int ERROR_WAKE_SYSTEM = 730;

    //
    // MessageId: ERROR_WAIT_1
    //
    // MessageText:
    //
    // ERROR_WAIT_1
    //
    int ERROR_WAIT_1 = 731;

    //
    // MessageId: ERROR_WAIT_2
    //
    // MessageText:
    //
    // ERROR_WAIT_2
    //
    int ERROR_WAIT_2 = 732;

    //
    // MessageId: ERROR_WAIT_3
    //
    // MessageText:
    //
    // ERROR_WAIT_3
    //
    int ERROR_WAIT_3 = 733;

    //
    // MessageId: ERROR_WAIT_63
    //
    // MessageText:
    //
    // ERROR_WAIT_63
    //
    int ERROR_WAIT_63 = 734;

    //
    // MessageId: ERROR_ABANDONED_WAIT_0
    //
    // MessageText:
    //
    // ERROR_ABANDONED_WAIT_0
    //
    int ERROR_ABANDONED_WAIT_0 = 735; // winnt

    //
    // MessageId: ERROR_ABANDONED_WAIT_63
    //
    // MessageText:
    //
    // ERROR_ABANDONED_WAIT_63
    //
    int ERROR_ABANDONED_WAIT_63 = 736;

    //
    // MessageId: ERROR_USER_APC
    //
    // MessageText:
    //
    // ERROR_USER_APC
    //
    int ERROR_USER_APC = 737; // winnt

    //
    // MessageId: ERROR_KERNEL_APC
    //
    // MessageText:
    //
    // ERROR_KERNEL_APC
    //
    int ERROR_KERNEL_APC = 738;

    //
    // MessageId: ERROR_ALERTED
    //
    // MessageText:
    //
    // ERROR_ALERTED
    //
    int ERROR_ALERTED = 739;

    //
    // MessageId: ERROR_ELEVATION_REQUIRED
    //
    // MessageText:
    //
    // The requested operation requires elevation.
    //
    int ERROR_ELEVATION_REQUIRED = 740;

    //
    // MessageId: ERROR_REPARSE
    //
    // MessageText:
    //
    // A reparse should be performed by the Object Manager since the name of the
    // file resulted in a symbolic link.
    //
    int ERROR_REPARSE = 741;

    //
    // MessageId: ERROR_OPLOCK_BREAK_IN_PROGRESS
    //
    // MessageText:
    //
    // An open/create operation completed while an oplock break is underway.
    //
    int ERROR_OPLOCK_BREAK_IN_PROGRESS = 742;

    //
    // MessageId: ERROR_VOLUME_MOUNTED
    //
    // MessageText:
    //
    // A new volume has been mounted by a file system.
    //
    int ERROR_VOLUME_MOUNTED = 743;

    //
    // MessageId: ERROR_RXACT_COMMITTED
    //
    // MessageText:
    //
    // This success level status indicates that the transaction state already
    // exists for the registry sub-tree, but that a transaction commit was
    // previously aborted.
    // The commit has now been completed.
    //
    int ERROR_RXACT_COMMITTED = 744;

    //
    // MessageId: ERROR_NOTIFY_CLEANUP
    //
    // MessageText:
    //
    // This indicates that a notify change request has been completed due to
    // closing the handle which made the notify change request.
    //
    int ERROR_NOTIFY_CLEANUP = 745;

    //
    // MessageId: ERROR_PRIMARY_TRANSPORT_CONNECT_FAILED
    //
    // MessageText:
    //
    // {Connect Failure on Primary Transport}
    // An attempt was made to connect to the remote server %hs on the primary
    // transport, but the connection failed.
    // The computer WAS able to connect on a secondary transport.
    //
    int ERROR_PRIMARY_TRANSPORT_CONNECT_FAILED = 746;

    //
    // MessageId: ERROR_PAGE_FAULT_TRANSITION
    //
    // MessageText:
    //
    // Page fault was a transition fault.
    //
    int ERROR_PAGE_FAULT_TRANSITION = 747;

    //
    // MessageId: ERROR_PAGE_FAULT_DEMAND_ZERO
    //
    // MessageText:
    //
    // Page fault was a demand zero fault.
    //
    int ERROR_PAGE_FAULT_DEMAND_ZERO = 748;

    //
    // MessageId: ERROR_PAGE_FAULT_COPY_ON_WRITE
    //
    // MessageText:
    //
    // Page fault was a demand zero fault.
    //
    int ERROR_PAGE_FAULT_COPY_ON_WRITE = 749;

    //
    // MessageId: ERROR_PAGE_FAULT_GUARD_PAGE
    //
    // MessageText:
    //
    // Page fault was a demand zero fault.
    //
    int ERROR_PAGE_FAULT_GUARD_PAGE = 750;

    //
    // MessageId: ERROR_PAGE_FAULT_PAGING_FILE
    //
    // MessageText:
    //
    // Page fault was satisfied by reading from a secondary storage device.
    //
    int ERROR_PAGE_FAULT_PAGING_FILE = 751;

    //
    // MessageId: ERROR_CACHE_PAGE_LOCKED
    //
    // MessageText:
    //
    // Cached page was locked during operation.
    //
    int ERROR_CACHE_PAGE_LOCKED = 752;

    //
    // MessageId: ERROR_CRASH_DUMP
    //
    // MessageText:
    //
    // Crash dump exists in paging file.
    //
    int ERROR_CRASH_DUMP = 753;

    //
    // MessageId: ERROR_BUFFER_ALL_ZEROS
    //
    // MessageText:
    //
    // Specified buffer contains all zeros.
    //
    int ERROR_BUFFER_ALL_ZEROS = 754;

    //
    // MessageId: ERROR_REPARSE_OBJECT
    //
    // MessageText:
    //
    // A reparse should be performed by the Object Manager since the name of the
    // file resulted in a symbolic link.
    //
    int ERROR_REPARSE_OBJECT = 755;

    //
    // MessageId: ERROR_RESOURCE_REQUIREMENTS_CHANGED
    //
    // MessageText:
    //
    // The device has succeeded a query-stop and its resource requirements have
    // changed.
    //
    int ERROR_RESOURCE_REQUIREMENTS_CHANGED = 756;

    //
    // MessageId: ERROR_TRANSLATION_COMPLETE
    //
    // MessageText:
    //
    // The translator has translated these resources into the global space and
    // no further translations should be performed.
    //
    int ERROR_TRANSLATION_COMPLETE = 757;

    //
    // MessageId: ERROR_NOTHING_TO_TERMINATE
    //
    // MessageText:
    //
    // A process being terminated has no threads to terminate.
    //
    int ERROR_NOTHING_TO_TERMINATE = 758;

    //
    // MessageId: ERROR_PROCESS_NOT_IN_JOB
    //
    // MessageText:
    //
    // The specified process is not part of a job.
    //
    int ERROR_PROCESS_NOT_IN_JOB = 759;

    //
    // MessageId: ERROR_PROCESS_IN_JOB
    //
    // MessageText:
    //
    // The specified process is part of a job.
    //
    int ERROR_PROCESS_IN_JOB = 760;

    //
    // MessageId: ERROR_VOLSNAP_HIBERNATE_READY
    //
    // MessageText:
    //
    // {Volume Shadow Copy Service}
    // The system is now ready for hibernation.
    //
    int ERROR_VOLSNAP_HIBERNATE_READY = 761;

    //
    // MessageId: ERROR_FSFILTER_OP_COMPLETED_SUCCESSFULLY
    //
    // MessageText:
    //
    // A file system or file system filter driver has successfully completed an
    // FsFilter operation.
    //
    int ERROR_FSFILTER_OP_COMPLETED_SUCCESSFULLY = 762;

    //
    // MessageId: ERROR_INTERRUPT_VECTOR_ALREADY_CONNECTED
    //
    // MessageText:
    //
    // The specified interrupt vector was already connected.
    //
    int ERROR_INTERRUPT_VECTOR_ALREADY_CONNECTED = 763;

    //
    // MessageId: ERROR_INTERRUPT_STILL_CONNECTED
    //
    // MessageText:
    //
    // The specified interrupt vector is still connected.
    //
    int ERROR_INTERRUPT_STILL_CONNECTED = 764;

    //
    // MessageId: ERROR_WAIT_FOR_OPLOCK
    //
    // MessageText:
    //
    // An operation is blocked waiting for an oplock.
    //
    int ERROR_WAIT_FOR_OPLOCK = 765;

    //
    // MessageId: ERROR_DBG_EXCEPTION_HANDLED
    //
    // MessageText:
    //
    // Debugger handled exception
    //
    int ERROR_DBG_EXCEPTION_HANDLED = 766; // winnt

    //
    // MessageId: ERROR_DBG_CONTINUE
    //
    // MessageText:
    //
    // Debugger continued
    //
    int ERROR_DBG_CONTINUE = 767; // winnt

    //
    // MessageId: ERROR_CALLBACK_POP_STACK
    //
    // MessageText:
    //
    // An exception occurred in a user mode callback and the kernel callback
    // frame should be removed.
    //
    int ERROR_CALLBACK_POP_STACK = 768;

    //
    // MessageId: ERROR_COMPRESSION_DISABLED
    //
    // MessageText:
    //
    // Compression is disabled for this volume.
    //
    int ERROR_COMPRESSION_DISABLED = 769;

    //
    // MessageId: ERROR_CANTFETCHBACKWARDS
    //
    // MessageText:
    //
    // The data provider cannot fetch backwards through a result set.
    //
    int ERROR_CANTFETCHBACKWARDS = 770;

    //
    // MessageId: ERROR_CANTSCROLLBACKWARDS
    //
    // MessageText:
    //
    // The data provider cannot scroll backwards through a result set.
    //
    int ERROR_CANTSCROLLBACKWARDS = 771;

    //
    // MessageId: ERROR_ROWSNOTRELEASED
    //
    // MessageText:
    //
    // The data provider requires that previously fetched data is released
    // before asking for more data.
    //
    int ERROR_ROWSNOTRELEASED = 772;

    //
    // MessageId: ERROR_BAD_ACCESSOR_FLAGS
    //
    // MessageText:
    //
    // The data provider was not able to intrepret the flags set for a column
    // binding in an accessor.
    //
    int ERROR_BAD_ACCESSOR_FLAGS = 773;

    //
    // MessageId: ERROR_ERRORS_ENCOUNTERED
    //
    // MessageText:
    //
    // One or more errors occurred while processing the request.
    //
    int ERROR_ERRORS_ENCOUNTERED = 774;

    //
    // MessageId: ERROR_NOT_CAPABLE
    //
    // MessageText:
    //
    // The implementation is not capable of performing the request.
    //
    int ERROR_NOT_CAPABLE = 775;

    //
    // MessageId: ERROR_REQUEST_OUT_OF_SEQUENCE
    //
    // MessageText:
    //
    // The client of a component requested an operation which is not valid given
    // the state of the component instance.
    //
    int ERROR_REQUEST_OUT_OF_SEQUENCE = 776;

    //
    // MessageId: ERROR_VERSION_PARSE_ERROR
    //
    // MessageText:
    //
    // A version number could not be parsed.
    //
    int ERROR_VERSION_PARSE_ERROR = 777;

    //
    // MessageId: ERROR_BADSTARTPOSITION
    //
    // MessageText:
    //
    // The iterator's start position is invalid.
    //
    int ERROR_BADSTARTPOSITION = 778;

    //
    // MessageId: ERROR_MEMORY_HARDWARE
    //
    // MessageText:
    //
    // The hardware has reported an uncorrectable memory error.
    //
    int ERROR_MEMORY_HARDWARE = 779;

    //
    // MessageId: ERROR_DISK_REPAIR_DISABLED
    //
    // MessageText:
    //
    // The attempted operation required self healing to be enabled.
    //
    int ERROR_DISK_REPAIR_DISABLED = 780;

    //
    // MessageId: ERROR_INSUFFICIENT_RESOURCE_FOR_SPECIFIED_SHARED_SECTION_SIZE
    //
    // MessageText:
    //
    // The Desktop heap encountered an error while allocating session memory.
    // There is more information in the system event log.
    //
    int ERROR_INSUFFICIENT_RESOURCE_FOR_SPECIFIED_SHARED_SECTION_SIZE = 781;

    //
    // MessageId: ERROR_SYSTEM_POWERSTATE_TRANSITION
    //
    // MessageText:
    //
    // The system powerstate is transitioning from %2 to %3.
    //
    int ERROR_SYSTEM_POWERSTATE_TRANSITION = 782;

    //
    // MessageId: ERROR_SYSTEM_POWERSTATE_COMPLEX_TRANSITION
    //
    // MessageText:
    //
    // The system powerstate is transitioning from %2 to %3 but could enter %4.
    //
    int ERROR_SYSTEM_POWERSTATE_COMPLEX_TRANSITION = 783;

    //
    // MessageId: ERROR_MCA_EXCEPTION
    //
    // MessageText:
    //
    // A thread is getting dispatched with MCA EXCEPTION because of MCA.
    //
    int ERROR_MCA_EXCEPTION = 784;

    //
    // MessageId: ERROR_ACCESS_AUDIT_BY_POLICY
    //
    // MessageText:
    //
    // Access to %1 is monitored by policy rule %2.
    //
    int ERROR_ACCESS_AUDIT_BY_POLICY = 785;

    //
    // MessageId: ERROR_ACCESS_DISABLED_NO_SAFER_UI_BY_POLICY
    //
    // MessageText:
    //
    // Access to %1 has been restricted by your Administrator by policy rule %2.
    //
    int ERROR_ACCESS_DISABLED_NO_SAFER_UI_BY_POLICY = 786;

    //
    // MessageId: ERROR_ABANDON_HIBERFILE
    //
    // MessageText:
    //
    // A valid hibernation file has been invalidated and should be abandoned.
    //
    int ERROR_ABANDON_HIBERFILE = 787;

    //
    // MessageId: ERROR_LOST_WRITEBEHIND_DATA_NETWORK_DISCONNECTED
    //
    // MessageText:
    //
    // {Delayed Write Failed}
    // Windows was unable to save all the data for the file %hs; the data has
    // been lost.
    // This error may be caused by network connectivity issues. Please try to
    // save this file elsewhere.
    //
    int ERROR_LOST_WRITEBEHIND_DATA_NETWORK_DISCONNECTED = 788;

    //
    // MessageId: ERROR_LOST_WRITEBEHIND_DATA_NETWORK_SERVER_ERROR
    //
    // MessageText:
    //
    // {Delayed Write Failed}
    // Windows was unable to save all the data for the file %hs; the data has
    // been lost.
    // This error was returned by the server on which the file exists. Please
    // try to save this file elsewhere.
    //
    int ERROR_LOST_WRITEBEHIND_DATA_NETWORK_SERVER_ERROR = 789;

    //
    // MessageId: ERROR_LOST_WRITEBEHIND_DATA_LOCAL_DISK_ERROR
    //
    // MessageText:
    //
    // {Delayed Write Failed}
    // Windows was unable to save all the data for the file %hs; the data has
    // been lost.
    // This error may be caused if the device has been removed or the media is
    // write-protected.
    //
    int ERROR_LOST_WRITEBEHIND_DATA_LOCAL_DISK_ERROR = 790;

    //
    // MessageId: ERROR_BAD_MCFG_TABLE
    //
    // MessageText:
    //
    // The resources required for this device conflict with the MCFG table.
    //
    int ERROR_BAD_MCFG_TABLE = 791;

    //
    // MessageId: ERROR_EA_ACCESS_DENIED
    //
    // MessageText:
    //
    // Access to the extended attribute was denied.
    //
    int ERROR_EA_ACCESS_DENIED = 994;

    //
    // MessageId: ERROR_OPERATION_ABORTED
    //
    // MessageText:
    //
    // The I/O operation has been aborted because of either a thread exit or an
    // application request.
    //
    int ERROR_OPERATION_ABORTED = 995;

    //
    // MessageId: ERROR_IO_INCOMPLETE
    //
    // MessageText:
    //
    // Overlapped I/O event is not in a signaled state.
    //
    int ERROR_IO_INCOMPLETE = 996;

    //
    // MessageId: ERROR_IO_PENDING
    //
    // MessageText:
    //
    // Overlapped I/O operation is in progress.
    //
    int ERROR_IO_PENDING = 997; // dderror

    //
    // MessageId: ERROR_NOACCESS
    //
    // MessageText:
    //
    // Invalid access to memory location.
    //
    int ERROR_NOACCESS = 998;

    //
    // MessageId: ERROR_SWAPERROR
    //
    // MessageText:
    //
    // Error performing inpage operation.
    //
    int ERROR_SWAPERROR = 999;

    //
    // MessageId: ERROR_STACK_OVERFLOW
    //
    // MessageText:
    //
    // Recursion too deep; the stack overflowed.
    //
    int ERROR_STACK_OVERFLOW = 1001;

    //
    // MessageId: ERROR_INVALID_MESSAGE
    //
    // MessageText:
    //
    // The window cannot act on the sent message.
    //
    int ERROR_INVALID_MESSAGE = 1002;

    //
    // MessageId: ERROR_CAN_NOT_COMPLETE
    //
    // MessageText:
    //
    // Cannot complete this function.
    //
    int ERROR_CAN_NOT_COMPLETE = 1003;

    //
    // MessageId: ERROR_INVALID_FLAGS
    //
    // MessageText:
    //
    // Invalid flags.
    //
    int ERROR_INVALID_FLAGS = 1004;

    //
    // MessageId: ERROR_UNRECOGNIZED_VOLUME
    //
    // MessageText:
    //
    // The volume does not contain a recognized file system.
    // Please make sure that all required file system drivers are loaded and
    // that the volume is not corrupted.
    //
    int ERROR_UNRECOGNIZED_VOLUME = 1005;

    //
    // MessageId: ERROR_FILE_INVALID
    //
    // MessageText:
    //
    // The volume for a file has been externally altered so that the opened file
    // is no longer valid.
    //
    int ERROR_FILE_INVALID = 1006;

    //
    // MessageId: ERROR_FULLSCREEN_MODE
    //
    // MessageText:
    //
    // The requested operation cannot be performed in full-screen mode.
    //
    int ERROR_FULLSCREEN_MODE = 1007;

    //
    // MessageId: ERROR_NO_TOKEN
    //
    // MessageText:
    //
    // An attempt was made to reference a token that does not exist.
    //
    int ERROR_NO_TOKEN = 1008;

    //
    // MessageId: ERROR_BADDB
    //
    // MessageText:
    //
    // The configuration registry database is corrupt.
    //
    int ERROR_BADDB = 1009;

    //
    // MessageId: ERROR_BADKEY
    //
    // MessageText:
    //
    // The configuration registry key is invalid.
    //
    int ERROR_BADKEY = 1010;

    //
    // MessageId: ERROR_CANTOPEN
    //
    // MessageText:
    //
    // The configuration registry key could not be opened.
    //
    int ERROR_CANTOPEN = 1011;

    //
    // MessageId: ERROR_CANTREAD
    //
    // MessageText:
    //
    // The configuration registry key could not be read.
    //
    int ERROR_CANTREAD = 1012;

    //
    // MessageId: ERROR_CANTWRITE
    //
    // MessageText:
    //
    // The configuration registry key could not be written.
    //
    int ERROR_CANTWRITE = 1013;

    //
    // MessageId: ERROR_REGISTRY_RECOVERED
    //
    // MessageText:
    //
    // One of the files in the registry database had to be recovered by use of a
    // log or alternate copy. The recovery was successful.
    //
    int ERROR_REGISTRY_RECOVERED = 1014;

    //
    // MessageId: ERROR_REGISTRY_CORRUPT
    //
    // MessageText:
    //
    // The registry is corrupted. The structure of one of the files containing
    // registry data is corrupted, or the system's memory image of the file is
    // corrupted, or the file could not be recovered because the alternate copy
    // or log was absent or corrupted.
    //
    int ERROR_REGISTRY_CORRUPT = 1015;

    //
    // MessageId: ERROR_REGISTRY_IO_FAILED
    //
    // MessageText:
    //
    // An I/O operation initiated by the registry failed unrecoverably. The
    // registry could not read in, or write out, or flush, one of the files that
    // contain the system's image of the registry.
    //
    int ERROR_REGISTRY_IO_FAILED = 1016;

    //
    // MessageId: ERROR_NOT_REGISTRY_FILE
    //
    // MessageText:
    //
    // The system has attempted to load or restore a file into the registry, but
    // the specified file is not in a registry file format.
    //
    int ERROR_NOT_REGISTRY_FILE = 1017;

    //
    // MessageId: ERROR_KEY_DELETED
    //
    // MessageText:
    //
    // Illegal operation attempted on a registry key that has been marked for
    // deletion.
    //
    int ERROR_KEY_DELETED = 1018;

    //
    // MessageId: ERROR_NO_LOG_SPACE
    //
    // MessageText:
    //
    // System could not allocate the required space in a registry log.
    //
    int ERROR_NO_LOG_SPACE = 1019;

    //
    // MessageId: ERROR_KEY_HAS_CHILDREN
    //
    // MessageText:
    //
    // Cannot create a symbolic link in a registry key that already has subkeys
    // or values.
    //
    int ERROR_KEY_HAS_CHILDREN = 1020;

    //
    // MessageId: ERROR_CHILD_MUST_BE_VOLATILE
    //
    // MessageText:
    //
    // Cannot create a stable subkey under a volatile parent key.
    //
    int ERROR_CHILD_MUST_BE_VOLATILE = 1021;

    //
    // MessageId: ERROR_NOTIFY_ENUM_DIR
    //
    // MessageText:
    //
    // A notify change request is being completed and the information is not
    // being returned in the caller's buffer. The caller now needs to enumerate
    // the files to find the changes.
    //
    int ERROR_NOTIFY_ENUM_DIR = 1022;

    //
    // MessageId: ERROR_DEPENDENT_SERVICES_RUNNING
    //
    // MessageText:
    //
    // A stop control has been sent to a service that other running services are
    // dependent on.
    //
    int ERROR_DEPENDENT_SERVICES_RUNNING = 1051;

    //
    // MessageId: ERROR_INVALID_SERVICE_CONTROL
    //
    // MessageText:
    //
    // The requested control is not valid for this service.
    //
    int ERROR_INVALID_SERVICE_CONTROL = 1052;

    //
    // MessageId: ERROR_SERVICE_REQUEST_TIMEOUT
    //
    // MessageText:
    //
    // The service did not respond to the start or control request in a timely
    // fashion.
    //
    int ERROR_SERVICE_REQUEST_TIMEOUT = 1053;

    //
    // MessageId: ERROR_SERVICE_NO_THREAD
    //
    // MessageText:
    //
    // A thread could not be created for the service.
    //
    int ERROR_SERVICE_NO_THREAD = 1054;

    //
    // MessageId: ERROR_SERVICE_DATABASE_LOCKED
    //
    // MessageText:
    //
    // The service database is locked.
    //
    int ERROR_SERVICE_DATABASE_LOCKED = 1055;

    //
    // MessageId: ERROR_SERVICE_ALREADY_RUNNING
    //
    // MessageText:
    //
    // An instance of the service is already running.
    //
    int ERROR_SERVICE_ALREADY_RUNNING = 1056;

    //
    // MessageId: ERROR_INVALID_SERVICE_ACCOUNT
    //
    // MessageText:
    //
    // The account name is invalid or does not exist, or the password is invalid
    // for the account name specified.
    //
    int ERROR_INVALID_SERVICE_ACCOUNT = 1057;

    //
    // MessageId: ERROR_SERVICE_DISABLED
    //
    // MessageText:
    //
    // The service cannot be started, either because it is disabled or because
    // it has no enabled devices associated with it.
    //
    int ERROR_SERVICE_DISABLED = 1058;

    //
    // MessageId: ERROR_CIRCULAR_DEPENDENCY
    //
    // MessageText:
    //
    // Circular service dependency was specified.
    //
    int ERROR_CIRCULAR_DEPENDENCY = 1059;

    //
    // MessageId: ERROR_SERVICE_DOES_NOT_EXIST
    //
    // MessageText:
    //
    // The specified service does not exist as an installed service.
    //
    int ERROR_SERVICE_DOES_NOT_EXIST = 1060;

    //
    // MessageId: ERROR_SERVICE_CANNOT_ACCEPT_CTRL
    //
    // MessageText:
    //
    // The service cannot accept control messages at this time.
    //
    int ERROR_SERVICE_CANNOT_ACCEPT_CTRL = 1061;

    //
    // MessageId: ERROR_SERVICE_NOT_ACTIVE
    //
    // MessageText:
    //
    // The service has not been started.
    //
    int ERROR_SERVICE_NOT_ACTIVE = 1062;

    //
    // MessageId: ERROR_FAILED_SERVICE_CONTROLLER_CONNECT
    //
    // MessageText:
    //
    // The service process could not connect to the service controller.
    //
    int ERROR_FAILED_SERVICE_CONTROLLER_CONNECT = 1063;

    //
    // MessageId: ERROR_EXCEPTION_IN_SERVICE
    //
    // MessageText:
    //
    // An exception occurred in the service when handling the control request.
    //
    int ERROR_EXCEPTION_IN_SERVICE = 1064;

    //
    // MessageId: ERROR_DATABASE_DOES_NOT_EXIST
    //
    // MessageText:
    //
    // The database specified does not exist.
    //
    int ERROR_DATABASE_DOES_NOT_EXIST = 1065;

    //
    // MessageId: ERROR_SERVICE_SPECIFIC_ERROR
    //
    // MessageText:
    //
    // The service has returned a service-specific error code.
    //
    int ERROR_SERVICE_SPECIFIC_ERROR = 1066;

    //
    // MessageId: ERROR_PROCESS_ABORTED
    //
    // MessageText:
    //
    // The process terminated unexpectedly.
    //
    int ERROR_PROCESS_ABORTED = 1067;

    //
    // MessageId: ERROR_SERVICE_DEPENDENCY_FAIL
    //
    // MessageText:
    //
    // The dependency service or group failed to start.
    //
    int ERROR_SERVICE_DEPENDENCY_FAIL = 1068;

    //
    // MessageId: ERROR_SERVICE_LOGON_FAILED
    //
    // MessageText:
    //
    // The service did not start due to a logon failure.
    //
    int ERROR_SERVICE_LOGON_FAILED = 1069;

    //
    // MessageId: ERROR_SERVICE_START_HANG
    //
    // MessageText:
    //
    // After starting, the service hung in a start-pending state.
    //
    int ERROR_SERVICE_START_HANG = 1070;

    //
    // MessageId: ERROR_INVALID_SERVICE_LOCK
    //
    // MessageText:
    //
    // The specified service database lock is invalid.
    //
    int ERROR_INVALID_SERVICE_LOCK = 1071;

    //
    // MessageId: ERROR_SERVICE_MARKED_FOR_DELETE
    //
    // MessageText:
    //
    // The specified service has been marked for deletion.
    //
    int ERROR_SERVICE_MARKED_FOR_DELETE = 1072;

    //
    // MessageId: ERROR_SERVICE_EXISTS
    //
    // MessageText:
    //
    // The specified service already exists.
    //
    int ERROR_SERVICE_EXISTS = 1073;

    //
    // MessageId: ERROR_ALREADY_RUNNING_LKG
    //
    // MessageText:
    //
    // The system is currently running with the last-known-good configuration.
    //
    int ERROR_ALREADY_RUNNING_LKG = 1074;

    //
    // MessageId: ERROR_SERVICE_DEPENDENCY_DELETED
    //
    // MessageText:
    //
    // The dependency service does not exist or has been marked for deletion.
    //
    int ERROR_SERVICE_DEPENDENCY_DELETED = 1075;

    //
    // MessageId: ERROR_BOOT_ALREADY_ACCEPTED
    //
    // MessageText:
    //
    // The current boot has already been accepted for use as the last-known-good
    // control set.
    //
    int ERROR_BOOT_ALREADY_ACCEPTED = 1076;

    //
    // MessageId: ERROR_SERVICE_NEVER_STARTED
    //
    // MessageText:
    //
    // No attempts to start the service have been made since the last boot.
    //
    int ERROR_SERVICE_NEVER_STARTED = 1077;

    //
    // MessageId: ERROR_DUPLICATE_SERVICE_NAME
    //
    // MessageText:
    //
    // The name is already in use as either a service name or a service display
    // name.
    //
    int ERROR_DUPLICATE_SERVICE_NAME = 1078;

    //
    // MessageId: ERROR_DIFFERENT_SERVICE_ACCOUNT
    //
    // MessageText:
    //
    // The account specified for this service is different from the account
    // specified for other services running in the same process.
    //
    int ERROR_DIFFERENT_SERVICE_ACCOUNT = 1079;

    //
    // MessageId: ERROR_CANNOT_DETECT_DRIVER_FAILURE
    //
    // MessageText:
    //
    // Failure actions can only be set for Win32 services, not for drivers.
    //
    int ERROR_CANNOT_DETECT_DRIVER_FAILURE = 1080;

    //
    // MessageId: ERROR_CANNOT_DETECT_PROCESS_ABORT
    //
    // MessageText:
    //
    // This service runs in the same process as the service control manager.
    // Therefore, the service control manager cannot take action if this
    // service's process terminates unexpectedly.
    //
    int ERROR_CANNOT_DETECT_PROCESS_ABORT = 1081;

    //
    // MessageId: ERROR_NO_RECOVERY_PROGRAM
    //
    // MessageText:
    //
    // No recovery program has been configured for this service.
    //
    int ERROR_NO_RECOVERY_PROGRAM = 1082;

    //
    // MessageId: ERROR_SERVICE_NOT_IN_EXE
    //
    // MessageText:
    //
    // The executable program that this service is configured to run in does not
    // implement the service.
    //
    int ERROR_SERVICE_NOT_IN_EXE = 1083;

    //
    // MessageId: ERROR_NOT_SAFEBOOT_SERVICE
    //
    // MessageText:
    //
    // This service cannot be started in Safe Mode
    //
    int ERROR_NOT_SAFEBOOT_SERVICE = 1084;

    //
    // MessageId: ERROR_END_OF_MEDIA
    //
    // MessageText:
    //
    // The physical end of the tape has been reached.
    //
    int ERROR_END_OF_MEDIA = 1100;

    //
    // MessageId: ERROR_FILEMARK_DETECTED
    //
    // MessageText:
    //
    // A tape access reached a filemark.
    //
    int ERROR_FILEMARK_DETECTED = 1101;

    //
    // MessageId: ERROR_BEGINNING_OF_MEDIA
    //
    // MessageText:
    //
    // The beginning of the tape or a partition was encountered.
    //
    int ERROR_BEGINNING_OF_MEDIA = 1102;

    //
    // MessageId: ERROR_SETMARK_DETECTED
    //
    // MessageText:
    //
    // A tape access reached the end of a set of files.
    //
    int ERROR_SETMARK_DETECTED = 1103;

    //
    // MessageId: ERROR_NO_DATA_DETECTED
    //
    // MessageText:
    //
    // No more data is on the tape.
    //
    int ERROR_NO_DATA_DETECTED = 1104;

    //
    // MessageId: ERROR_PARTITION_FAILURE
    //
    // MessageText:
    //
    // Tape could not be partitioned.
    //
    int ERROR_PARTITION_FAILURE = 1105;

    //
    // MessageId: ERROR_INVALID_BLOCK_LENGTH
    //
    // MessageText:
    //
    // When accessing a new tape of a multivolume partition, the current block
    // size is incorrect.
    //
    int ERROR_INVALID_BLOCK_LENGTH = 1106;

    //
    // MessageId: ERROR_DEVICE_NOT_PARTITIONED
    //
    // MessageText:
    //
    // Tape partition information could not be found when loading a tape.
    //
    int ERROR_DEVICE_NOT_PARTITIONED = 1107;

    //
    // MessageId: ERROR_UNABLE_TO_LOCK_MEDIA
    //
    // MessageText:
    //
    // Unable to lock the media eject mechanism.
    //
    int ERROR_UNABLE_TO_LOCK_MEDIA = 1108;

    //
    // MessageId: ERROR_UNABLE_TO_UNLOAD_MEDIA
    //
    // MessageText:
    //
    // Unable to unload the media.
    //
    int ERROR_UNABLE_TO_UNLOAD_MEDIA = 1109;

    //
    // MessageId: ERROR_MEDIA_CHANGED
    //
    // MessageText:
    //
    // The media in the drive may have changed.
    //
    int ERROR_MEDIA_CHANGED = 1110;

    //
    // MessageId: ERROR_BUS_RESET
    //
    // MessageText:
    //
    // The I/O bus was reset.
    //
    int ERROR_BUS_RESET = 1111;

    //
    // MessageId: ERROR_NO_MEDIA_IN_DRIVE
    //
    // MessageText:
    //
    // No media in drive.
    //
    int ERROR_NO_MEDIA_IN_DRIVE = 1112;

    //
    // MessageId: ERROR_NO_UNICODE_TRANSLATION
    //
    // MessageText:
    //
    // No mapping for the Unicode character exists in the target multi-byte code
    // page.
    //
    int ERROR_NO_UNICODE_TRANSLATION = 1113;

    //
    // MessageId: ERROR_DLL_INIT_FAILED
    //
    // MessageText:
    //
    // A dynamic link library (DL; initialization routine failed.
    //
    int ERROR_DLL_INIT_FAILED = 1114;

    //
    // MessageId: ERROR_SHUTDOWN_IN_PROGRESS
    //
    // MessageText:
    //
    // A system shutdown is in progress.
    //
    int ERROR_SHUTDOWN_IN_PROGRESS = 1115;

    //
    // MessageId: ERROR_NO_SHUTDOWN_IN_PROGRESS
    //
    // MessageText:
    //
    // Unable to abort the system shutdown because no shutdown was in progress.
    //
    int ERROR_NO_SHUTDOWN_IN_PROGRESS = 1116;

    //
    // MessageId: ERROR_IO_DEVICE
    //
    // MessageText:
    //
    // The request could not be performed because of an I/O device error.
    //
    int ERROR_IO_DEVICE = 1117;

    //
    // MessageId: ERROR_SERIAL_NO_DEVICE
    //
    // MessageText:
    //
    // No serial device was successfully initialized. The serial driver will
    // unload.
    //
    int ERROR_SERIAL_NO_DEVICE = 1118;

    //
    // MessageId: ERROR_IRQ_BUSY
    //
    // MessageText:
    //
    // Unable to open a device that was sharing an interrupt request (IRQ) with
    // other devices. At least one other device that uses that IRQ was already
    // opened.
    //
    int ERROR_IRQ_BUSY = 1119;

    //
    // MessageId: ERROR_MORE_WRITES
    //
    // MessageText:
    //
    // A serial I/O operation was completed by another write to the serial port.
    // (The IOCTL_SERIAL_XOFF_COUNTER reached zero.)
    //
    int ERROR_MORE_WRITES = 1120;

    //
    // MessageId: ERROR_COUNTER_TIMEOUT
    //
    // MessageText:
    //
    // A serial I/O operation completed because the timeout period expired.
    // (The IOCTL_SERIAL_XOFF_COUNTER did not reach zero.)
    //
    int ERROR_COUNTER_TIMEOUT = 1121;

    //
    // MessageId: ERROR_FLOPPY_ID_MARK_NOT_FOUND
    //
    // MessageText:
    //
    // No ID address mark was found on the floppy disk.
    //
    int ERROR_FLOPPY_ID_MARK_NOT_FOUND = 1122;

    //
    // MessageId: ERROR_FLOPPY_WRONG_CYLINDER
    //
    // MessageText:
    //
    // Mismatch between the floppy disk sector ID field and the floppy disk
    // controller track address.
    //
    int ERROR_FLOPPY_WRONG_CYLINDER = 1123;

    //
    // MessageId: ERROR_FLOPPY_UNKNOWN_ERROR
    //
    // MessageText:
    //
    // The floppy disk controller reported an error that is not recognized by
    // the floppy disk driver.
    //
    int ERROR_FLOPPY_UNKNOWN_ERROR = 1124;

    //
    // MessageId: ERROR_FLOPPY_BAD_REGISTERS
    //
    // MessageText:
    //
    // The floppy disk controller returned inconsistent results in its
    // registers.
    //
    int ERROR_FLOPPY_BAD_REGISTERS = 1125;

    //
    // MessageId: ERROR_DISK_RECALIBRATE_FAILED
    //
    // MessageText:
    //
    // While accessing the hard disk, a recalibrate operation failed, even after
    // retries.
    //
    int ERROR_DISK_RECALIBRATE_FAILED = 1126;

    //
    // MessageId: ERROR_DISK_OPERATION_FAILED
    //
    // MessageText:
    //
    // While accessing the hard disk, a disk operation failed even after
    // retries.
    //
    int ERROR_DISK_OPERATION_FAILED = 1127;

    //
    // MessageId: ERROR_DISK_RESET_FAILED
    //
    // MessageText:
    //
    // While accessing the hard disk, a disk controller reset was needed, but
    // even that failed.
    //
    int ERROR_DISK_RESET_FAILED = 1128;

    //
    // MessageId: ERROR_EOM_OVERFLOW
    //
    // MessageText:
    //
    // Physical end of tape encountered.
    //
    int ERROR_EOM_OVERFLOW = 1129;

    //
    // MessageId: ERROR_NOT_ENOUGH_SERVER_MEMORY
    //
    // MessageText:
    //
    // Not enough server storage is available to process this command.
    //
    int ERROR_NOT_ENOUGH_SERVER_MEMORY = 1130;

    //
    // MessageId: ERROR_POSSIBLE_DEADLOCK
    //
    // MessageText:
    //
    // A potential deadlock condition has been detected.
    //
    int ERROR_POSSIBLE_DEADLOCK = 1131;

    //
    // MessageId: ERROR_MAPPED_ALIGNMENT
    //
    // MessageText:
    //
    // The base address or the file offset specified does not have the proper
    // alignment.
    //
    int ERROR_MAPPED_ALIGNMENT = 1132;

    //
    // MessageId: ERROR_SET_POWER_STATE_VETOED
    //
    // MessageText:
    //
    // An attempt to change the system power state was vetoed by another
    // application or driver.
    //
    int ERROR_SET_POWER_STATE_VETOED = 1140;

    //
    // MessageId: ERROR_SET_POWER_STATE_FAILED
    //
    // MessageText:
    //
    // The system BIOS failed an attempt to change the system power state.
    //
    int ERROR_SET_POWER_STATE_FAILED = 1141;

    //
    // MessageId: ERROR_TOO_MANY_LINKS
    //
    // MessageText:
    //
    // An attempt was made to create more links on a file than the file system
    // supports.
    //
    int ERROR_TOO_MANY_LINKS = 1142;

    //
    // MessageId: ERROR_OLD_WIN_VERSION
    //
    // MessageText:
    //
    // The specified program requires a newer version of Windows.
    //
    int ERROR_OLD_WIN_VERSION = 1150;

    //
    // MessageId: ERROR_APP_WRONG_OS
    //
    // MessageText:
    //
    // The specified program is not a Windows or MS-DOS program.
    //
    int ERROR_APP_WRONG_OS = 1151;

    //
    // MessageId: ERROR_SINGLE_INSTANCE_APP
    //
    // MessageText:
    //
    // Cannot start more than one instance of the specified program.
    //
    int ERROR_SINGLE_INSTANCE_APP = 1152;

    //
    // MessageId: ERROR_RMODE_APP
    //
    // MessageText:
    //
    // The specified program was written for an earlier version of Windows.
    //
    int ERROR_RMODE_APP = 1153;

    //
    // MessageId: ERROR_INVALID_DLL
    //
    // MessageText:
    //
    // One of the library files needed to run this application is damaged.
    //
    int ERROR_INVALID_DLL = 1154;

    //
    // MessageId: ERROR_NO_ASSOCIATION
    //
    // MessageText:
    //
    // No application is associated with the specified file for this operation.
    //
    int ERROR_NO_ASSOCIATION = 1155;

    //
    // MessageId: ERROR_DDE_FAIL
    //
    // MessageText:
    //
    // An error occurred in sending the command to the application.
    //
    int ERROR_DDE_FAIL = 1156;

    //
    // MessageId: ERROR_DLL_NOT_FOUND
    //
    // MessageText:
    //
    // One of the library files needed to run this application cannot be found.
    //
    int ERROR_DLL_NOT_FOUND = 1157;

    //
    // MessageId: ERROR_NO_MORE_USER_HANDLES
    //
    // MessageText:
    //
    // The current process has used all of its system allowance of handles for
    // Window Manager objects.
    //
    int ERROR_NO_MORE_USER_HANDLES = 1158;

    //
    // MessageId: ERROR_MESSAGE_SYNC_ONLY
    //
    // MessageText:
    //
    // The message can be used only with synchronous operations.
    //
    int ERROR_MESSAGE_SYNC_ONLY = 1159;

    //
    // MessageId: ERROR_SOURCE_ELEMENT_EMPTY
    //
    // MessageText:
    //
    // The indicated source element has no media.
    //
    int ERROR_SOURCE_ELEMENT_EMPTY = 1160;

    //
    // MessageId: ERROR_DESTINATION_ELEMENT_FULL
    //
    // MessageText:
    //
    // The indicated destination element already contains media.
    //
    int ERROR_DESTINATION_ELEMENT_FULL = 1161;

    //
    // MessageId: ERROR_ILLEGAL_ELEMENT_ADDRESS
    //
    // MessageText:
    //
    // The indicated element does not exist.
    //
    int ERROR_ILLEGAL_ELEMENT_ADDRESS = 1162;

    //
    // MessageId: ERROR_MAGAZINE_NOT_PRESENT
    //
    // MessageText:
    //
    // The indicated element is part of a magazine that is not present.
    //
    int ERROR_MAGAZINE_NOT_PRESENT = 1163;

    //
    // MessageId: ERROR_DEVICE_REINITIALIZATION_NEEDED
    //
    // MessageText:
    //
    // The indicated device requires reinitialization due to hardware errors.
    //
    int ERROR_DEVICE_REINITIALIZATION_NEEDED = 1164; // dderror

    //
    // MessageId: ERROR_DEVICE_REQUIRES_CLEANING
    //
    // MessageText:
    //
    // The device has indicated that cleaning is required before further
    // operations are attempted.
    //
    int ERROR_DEVICE_REQUIRES_CLEANING = 1165;

    //
    // MessageId: ERROR_DEVICE_DOOR_OPEN
    //
    // MessageText:
    //
    // The device has indicated that its door is open.
    //
    int ERROR_DEVICE_DOOR_OPEN = 1166;

    //
    // MessageId: ERROR_DEVICE_NOT_CONNECTED
    //
    // MessageText:
    //
    // The device is not connected.
    //
    int ERROR_DEVICE_NOT_CONNECTED = 1167;

    //
    // MessageId: ERROR_NOT_FOUND
    //
    // MessageText:
    //
    // Element not found.
    //
    int ERROR_NOT_FOUND = 1168;

    //
    // MessageId: ERROR_NO_MATCH
    //
    // MessageText:
    //
    // There was no match for the specified key in the index.
    //
    int ERROR_NO_MATCH = 1169;

    //
    // MessageId: ERROR_SET_NOT_FOUND
    //
    // MessageText:
    //
    // The property set specified does not exist on the object.
    //
    int ERROR_SET_NOT_FOUND = 1170;

    //
    // MessageId: ERROR_POINT_NOT_FOUND
    //
    // MessageText:
    //
    // The point passed to GetMouseMovePoints is not in the buffer.
    //
    int ERROR_POINT_NOT_FOUND = 1171;

    //
    // MessageId: ERROR_NO_TRACKING_SERVICE
    //
    // MessageText:
    //
    // The tracking (workstation) service is not running.
    //
    int ERROR_NO_TRACKING_SERVICE = 1172;

    //
    // MessageId: ERROR_NO_VOLUME_ID
    //
    // MessageText:
    //
    // The Volume ID could not be found.
    //
    int ERROR_NO_VOLUME_ID = 1173;

    //
    // MessageId: ERROR_UNABLE_TO_REMOVE_REPLACED
    //
    // MessageText:
    //
    // Unable to remove the file to be replaced.
    //
    int ERROR_UNABLE_TO_REMOVE_REPLACED = 1175;

    //
    // MessageId: ERROR_UNABLE_TO_MOVE_REPLACEMENT
    //
    // MessageText:
    //
    // Unable to move the replacement file to the file to be replaced. The file
    // to be replaced has retained its original name.
    //
    int ERROR_UNABLE_TO_MOVE_REPLACEMENT = 1176;

    //
    // MessageId: ERROR_UNABLE_TO_MOVE_REPLACEMENT_2
    //
    // MessageText:
    //
    // Unable to move the replacement file to the file to be replaced. The file
    // to be replaced has been renamed using the backup name.
    //
    int ERROR_UNABLE_TO_MOVE_REPLACEMENT_2 = 1177;

    //
    // MessageId: ERROR_JOURNAL_DELETE_IN_PROGRESS
    //
    // MessageText:
    //
    // The volume change journal is being deleted.
    //
    int ERROR_JOURNAL_DELETE_IN_PROGRESS = 1178;

    //
    // MessageId: ERROR_JOURNAL_NOT_ACTIVE
    //
    // MessageText:
    //
    // The volume change journal is not active.
    //
    int ERROR_JOURNAL_NOT_ACTIVE = 1179;

    //
    // MessageId: ERROR_POTENTIAL_FILE_FOUND
    //
    // MessageText:
    //
    // A file was found, but it may not be the correct file.
    //
    int ERROR_POTENTIAL_FILE_FOUND = 1180;

    //
    // MessageId: ERROR_JOURNAL_ENTRY_DELETED
    //
    // MessageText:
    //
    // The journal entry has been deleted from the journal.
    //
    int ERROR_JOURNAL_ENTRY_DELETED = 1181;

    //
    // MessageId: ERROR_SHUTDOWN_IS_SCHEDULED
    //
    // MessageText:
    //
    // A system shutdown has already been scheduled.
    //
    int ERROR_SHUTDOWN_IS_SCHEDULED = 1190;

    //
    // MessageId: ERROR_SHUTDOWN_USERS_LOGGED_ON
    //
    // MessageText:
    //
    // The system shutdown cannot be initiated because there are other users
    // logged on to the computer.
    //
    int ERROR_SHUTDOWN_USERS_LOGGED_ON = 1191;
    int ERROR_BAD_DEVICE = 1200;
    int ERROR_CONNECTION_UNAVAIL = 1201;
    int ERROR_DEVICE_ALREADY_REMEMBERED = 1202;
    int ERROR_NO_NET_OR_BAD_PATH = 1203;
    int ERROR_BAD_PROVIDER = 1204;
    int ERROR_CANNOT_OPEN_PROFILE = 1205;
    int ERROR_BAD_PROFILE = 1206;
    int ERROR_NOT_CONTAINER = 1207;
    int ERROR_EXTENDED_ERROR = 1208;
    int ERROR_INVALID_GROUPNAME = 1209;
    int ERROR_INVALID_COMPUTERNAME = 1210;
    int ERROR_INVALID_EVENTNAME = 1211;
    int ERROR_INVALID_DOMAINNAME = 1212;
    int ERROR_INVALID_SERVICENAME = 1213;
    int ERROR_INVALID_NETNAME = 1214;
    int ERROR_INVALID_SHARENAME = 1215;
    int ERROR_INVALID_PASSWORDNAME = 1216;
    int ERROR_INVALID_MESSAGENAME = 1217;
    int ERROR_INVALID_MESSAGEDEST = 1218;
    int ERROR_SESSION_CREDENTIAL_CONFLICT = 1219;
    int ERROR_REMOTE_SESSION_LIMIT_EXCEEDED = 1220;
    int ERROR_DUP_DOMAINNAME = 1221;
    int ERROR_NO_NETWORK = 1222;
    int ERROR_CANCELLED = 1223;
    int ERROR_USER_MAPPED_FILE = 1224;
    int ERROR_CONNECTION_REFUSED = 1225;
    int ERROR_GRACEFUL_DISCONNECT = 1226;
    int ERROR_ADDRESS_ALREADY_ASSOCIATED = 1227;
    int ERROR_ADDRESS_NOT_ASSOCIATED = 1228;
    int ERROR_CONNECTION_INVALID = 1229;
    int ERROR_CONNECTION_ACTIVE = 1230;
    int ERROR_NETWORK_UNREACHABLE = 1231;
    int ERROR_HOST_UNREACHABLE = 1232;
    int ERROR_PROTOCOL_UNREACHABLE = 1233;
    int ERROR_PORT_UNREACHABLE = 1234;
    int ERROR_REQUEST_ABORTED = 1235;
    int ERROR_CONNECTION_ABORTED = 1236;
    int ERROR_RETRY = 1237;
    int ERROR_CONNECTION_COUNT_LIMIT = 1238;
    int ERROR_LOGIN_TIME_RESTRICTION = 1239;
    int ERROR_LOGIN_WKSTA_RESTRICTION = 1240;
    int ERROR_INCORRECT_ADDRESS = 1241;
    int ERROR_ALREADY_REGISTERED = 1242;
    int ERROR_SERVICE_NOT_FOUND = 1243;
    int ERROR_NOT_AUTHENTICATED = 1244;
    int ERROR_NOT_LOGGED_ON = 1245;
    int ERROR_CONTINUE = 1246;
    int ERROR_ALREADY_INITIALIZED = 1247;
    int ERROR_NO_MORE_DEVICES = 1248;
    int ERROR_NO_SUCH_SITE = 1249;
    int ERROR_DOMAIN_CONTROLLER_EXISTS = 1250;
    int ERROR_ONLY_IF_CONNECTED = 1251;
    int ERROR_OVERRIDE_NOCHANGES = 1252;
    int ERROR_BAD_USER_PROFILE = 1253;
    int ERROR_NOT_SUPPORTED_ON_SBS = 1254;
    int ERROR_SERVER_SHUTDOWN_IN_PROGRESS = 1255;
    int ERROR_HOST_DOWN = 1256;
    int ERROR_NON_ACCOUNT_SID = 1257;
    int ERROR_NON_DOMAIN_SID = 1258;
    int ERROR_APPHELP_BLOCK = 1259;
    int ERROR_ACCESS_DISABLED_BY_POLICY = 1260;
    int ERROR_REG_NAT_CONSUMPTION = 1261;
    int ERROR_CSCSHARE_OFFLINE = 1262;
    int ERROR_PKINIT_FAILURE = 1263;
    int ERROR_SMARTCARD_SUBSYSTEM_FAILURE = 1264;
    int ERROR_DOWNGRADE_DETECTED = 1265;
    int ERROR_MACHINE_LOCKED = 1271;
    int ERROR_CALLBACK_SUPPLIED_INVALID_DATA = 1273;
    int ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED = 1274;
    int ERROR_DRIVER_BLOCKED = 1275;
    int ERROR_INVALID_IMPORT_OF_NON_DLL = 1276;
    int ERROR_ACCESS_DISABLED_WEBBLADE = 1277;
    int ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER = 1278;
    int ERROR_RECOVERY_FAILURE = 1279;
    int ERROR_ALREADY_FIBER = 1280;
    int ERROR_ALREADY_THREAD = 1281;
    int ERROR_STACK_BUFFER_OVERRUN = 1282;
    int ERROR_PARAMETER_QUOTA_EXCEEDED = 1283;
    int ERROR_DEBUGGER_INACTIVE = 1284;
    int ERROR_DELAY_LOAD_FAILED = 1285;
    int ERROR_VDM_DISALLOWED = 1286;
    int ERROR_UNIDENTIFIED_ERROR = 1287;
    int ERROR_INVALID_CRUNTIME_PARAMETER = 1288;
    int ERROR_BEYOND_VDL = 1289;
    int ERROR_INCOMPATIBLE_SERVICE_SID_TYPE = 1290;
    int ERROR_DRIVER_PROCESS_TERMINATED = 1291;
    int ERROR_IMPLEMENTATION_LIMIT = 1292;
    int ERROR_PROCESS_IS_PROTECTED = 1293;
    int ERROR_SERVICE_NOTIFY_CLIENT_LAGGING = 1294;
    int ERROR_DISK_QUOTA_EXCEEDED = 1295;
    int ERROR_CONTENT_BLOCKED = 1296;
    int ERROR_INCOMPATIBLE_SERVICE_PRIVILEGE = 1297;
    int ERROR_INVALID_LABEL = 1299;
    int ERROR_NOT_ALL_ASSIGNED = 1300;
    int ERROR_SOME_NOT_MAPPED = 1301;
    int ERROR_NO_QUOTAS_FOR_ACCOUNT = 1302;
    int ERROR_LOCAL_USER_SESSION_KEY = 1303;
    int ERROR_NULL_LM_PASSWORD = 1304;
    int ERROR_UNKNOWN_REVISION = 1305;
    int ERROR_REVISION_MISMATCH = 1306;
    int ERROR_INVALID_OWNER = 1307;
    int ERROR_INVALID_PRIMARY_GROUP = 1308;
    int ERROR_NO_IMPERSONATION_TOKEN = 1309;
    int ERROR_CANT_DISABLE_MANDATORY = 1310;
    int ERROR_NO_LOGON_SERVERS = 1311;
    int ERROR_NO_SUCH_LOGON_SESSION = 1312;
    int ERROR_NO_SUCH_PRIVILEGE = 1313;
    int ERROR_PRIVILEGE_NOT_HELD = 1314;
    int ERROR_INVALID_ACCOUNT_NAME = 1315;
    int ERROR_USER_EXISTS = 1316;
    int ERROR_NO_SUCH_USER = 1317;
    int ERROR_GROUP_EXISTS = 1318;
    int ERROR_NO_SUCH_GROUP = 1319;
    int ERROR_MEMBER_IN_GROUP = 1320;
    int ERROR_MEMBER_NOT_IN_GROUP = 1321;
    int ERROR_LAST_ADMIN = 1322;
    int ERROR_WRONG_PASSWORD = 1323;
    int ERROR_ILL_FORMED_PASSWORD = 1324;
    int ERROR_PASSWORD_RESTRICTION = 1325;
    int ERROR_LOGON_FAILURE = 1326;
    int ERROR_ACCOUNT_RESTRICTION = 1327;
    int ERROR_INVALID_LOGON_HOURS = 1328;
    int ERROR_INVALID_WORKSTATION = 1329;
    int ERROR_PASSWORD_EXPIRED = 1330;
    int ERROR_ACCOUNT_DISABLED = 1331;
    int ERROR_NONE_MAPPED = 1332;
    int ERROR_TOO_MANY_LUIDS_REQUESTED = 1333;
    int ERROR_LUIDS_EXHAUSTED = 1334;
    int ERROR_INVALID_SUB_AUTHORITY = 1335;
    int ERROR_INVALID_ACL = 1336;
    int ERROR_INVALID_SID = 1337;
    int ERROR_INVALID_SECURITY_DESCR = 1338;
    int ERROR_BAD_INHERITANCE_ACL = 1340;
    int ERROR_SERVER_DISABLED = 1341;
    int ERROR_SERVER_NOT_DISABLED = 1342;
    int ERROR_INVALID_ID_AUTHORITY = 1343;
    int ERROR_ALLOTTED_SPACE_EXCEEDED = 1344;
    int ERROR_INVALID_GROUP_ATTRIBUTES = 1345;
    int ERROR_BAD_IMPERSONATION_LEVEL = 1346;
    int ERROR_CANT_OPEN_ANONYMOUS = 1347;
    int ERROR_BAD_VALIDATION_CLASS = 1348;
    int ERROR_BAD_TOKEN_TYPE = 1349;
    int ERROR_NO_SECURITY_ON_OBJECT = 1350;
    int ERROR_CANT_ACCESS_DOMAIN_INFO = 1351;
    int ERROR_INVALID_SERVER_STATE = 1352;
    int ERROR_INVALID_DOMAIN_STATE = 1353;
    int ERROR_INVALID_DOMAIN_ROLE = 1354;
    int ERROR_NO_SUCH_DOMAIN = 1355;
    int ERROR_DOMAIN_EXISTS = 1356;
    int ERROR_DOMAIN_LIMIT_EXCEEDED = 1357;
    int ERROR_INTERNAL_DB_CORRUPTION = 1358;
    int ERROR_INTERNAL_ERROR = 1359;
    int ERROR_GENERIC_NOT_MAPPED = 1360;
    int ERROR_BAD_DESCRIPTOR_FORMAT = 1361;
    int ERROR_NOT_LOGON_PROCESS = 1362;
    int ERROR_LOGON_SESSION_EXISTS = 1363;
    int ERROR_NO_SUCH_PACKAGE = 1364;
    int ERROR_BAD_LOGON_SESSION_STATE = 1365;
    int ERROR_LOGON_SESSION_COLLISION = 1366;
    int ERROR_INVALID_LOGON_TYPE = 1367;
    int ERROR_CANNOT_IMPERSONATE = 1368;
    int ERROR_RXACT_INVALID_STATE = 1369;
    int ERROR_RXACT_COMMIT_FAILURE = 1370;
    int ERROR_SPECIAL_ACCOUNT = 1371;
    int ERROR_SPECIAL_GROUP = 1372;
    int ERROR_SPECIAL_USER = 1373;
    int ERROR_MEMBERS_PRIMARY_GROUP = 1374;
    int ERROR_TOKEN_ALREADY_IN_USE = 1375;
    int ERROR_NO_SUCH_ALIAS = 1376;
    int ERROR_MEMBER_NOT_IN_ALIAS = 1377;
    int ERROR_MEMBER_IN_ALIAS = 1378;
    int ERROR_ALIAS_EXISTS = 1379;
    int ERROR_LOGON_NOT_GRANTED = 1380;
    int ERROR_TOO_MANY_SECRETS = 1381;
    int ERROR_SECRET_TOO_LONG = 1382;
    int ERROR_INTERNAL_DB_ERROR = 1383;
    int ERROR_TOO_MANY_CONTEXT_IDS = 1384;
    int ERROR_LOGON_TYPE_NOT_GRANTED = 1385;
    int ERROR_NT_CROSS_ENCRYPTION_REQUIRED = 1386;
    int ERROR_NO_SUCH_MEMBER = 1387;
    int ERROR_INVALID_MEMBER = 1388;
    int ERROR_TOO_MANY_SIDS = 1389;
    int ERROR_LM_CROSS_ENCRYPTION_REQUIRED = 1390;
    int ERROR_NO_INHERITANCE = 1391;
    int ERROR_FILE_CORRUPT = 1392;
    int ERROR_DISK_CORRUPT = 1393;
    int ERROR_NO_USER_SESSION_KEY = 1394;
    int ERROR_LICENSE_QUOTA_EXCEEDED = 1395;
    int ERROR_WRONG_TARGET_NAME = 1396;
    int ERROR_MUTUAL_AUTH_FAILED = 1397;
    int ERROR_TIME_SKEW = 1398;
    int ERROR_CURRENT_DOMAIN_NOT_ALLOWED = 1399;
    int ERROR_INVALID_WINDOW_HANDLE = 1400;
    int ERROR_INVALID_MENU_HANDLE = 1401;
    int ERROR_INVALID_CURSOR_HANDLE = 1402;
    int ERROR_INVALID_ACCEL_HANDLE = 1403;
    int ERROR_INVALID_HOOK_HANDLE = 1404;
    int ERROR_INVALID_DWP_HANDLE = 1405;
    int ERROR_TLW_WITH_WSCHILD = 1406;
    int ERROR_CANNOT_FIND_WND_CLASS = 1407;
    int ERROR_WINDOW_OF_OTHER_THREAD = 1408;
    int ERROR_HOTKEY_ALREADY_REGISTERED = 1409;
    int ERROR_CLASS_ALREADY_EXISTS = 1410;
    int ERROR_CLASS_DOES_NOT_EXIST = 1411;
    int ERROR_CLASS_HAS_WINDOWS = 1412;
    int ERROR_INVALID_INDEX = 1413;
    int ERROR_INVALID_ICON_HANDLE = 1414;
    int ERROR_PRIVATE_DIALOG_INDEX = 1415;
    int ERROR_LISTBOX_ID_NOT_FOUND = 1416;
    int ERROR_NO_WILDCARD_CHARACTERS = 1417;
    int ERROR_CLIPBOARD_NOT_OPEN = 1418;
    int ERROR_HOTKEY_NOT_REGISTERED = 1419;
    int ERROR_WINDOW_NOT_DIALOG = 1420;
    int ERROR_CONTROL_ID_NOT_FOUND = 1421;
    int ERROR_INVALID_COMBOBOX_MESSAGE = 1422;
    int ERROR_WINDOW_NOT_COMBOBOX = 1423;
    int ERROR_INVALID_EDIT_HEIGHT = 1424;
    int ERROR_DC_NOT_FOUND = 1425;
    int ERROR_INVALID_HOOK_FILTER = 1426;
    int ERROR_INVALID_FILTER_PROC = 1427;
    int ERROR_HOOK_NEEDS_HMOD = 1428;
    int ERROR_GLOBAL_ONLY_HOOK = 1429;
    int ERROR_JOURNAL_HOOK_SET = 1430;
    int ERROR_HOOK_NOT_INSTALLED = 1431;
    int ERROR_INVALID_LB_MESSAGE = 1432;
    int ERROR_SETCOUNT_ON_BAD_LB = 1433;
    int ERROR_LB_WITHOUT_TABSTOPS = 1434;
    int ERROR_DESTROY_OBJECT_OF_OTHER_THREAD = 1435;
    int ERROR_CHILD_WINDOW_MENU = 1436;
    int ERROR_NO_SYSTEM_MENU = 1437;
    int ERROR_INVALID_MSGBOX_STYLE = 1438;
    int ERROR_INVALID_SPI_VALUE = 1439;
    int ERROR_SCREEN_ALREADY_LOCKED = 1440;
    int ERROR_HWNDS_HAVE_DIFF_PARENT = 1441;
    int ERROR_NOT_CHILD_WINDOW = 1442;
    int ERROR_INVALID_GW_COMMAND = 1443;
    int ERROR_INVALID_THREAD_ID = 1444;
    int ERROR_NON_MDICHILD_WINDOW = 1445;
    int ERROR_POPUP_ALREADY_ACTIVE = 1446;
    int ERROR_NO_SCROLLBARS = 1447;
    int ERROR_INVALID_SCROLLBAR_RANGE = 1448;
    int ERROR_INVALID_SHOWWIN_COMMAND = 1449;
    int ERROR_NO_SYSTEM_RESOURCES = 1450;
    int ERROR_NONPAGED_SYSTEM_RESOURCES = 1451;
    int ERROR_PAGED_SYSTEM_RESOURCES = 1452;
    int ERROR_WORKING_SET_QUOTA = 1453;
    int ERROR_PAGEFILE_QUOTA = 1454;
    int ERROR_COMMITMENT_LIMIT = 1455;
    int ERROR_MENU_ITEM_NOT_FOUND = 1456;
    int ERROR_INVALID_KEYBOARD_HANDLE = 1457;
    int ERROR_HOOK_TYPE_NOT_ALLOWED = 1458;
    int ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION = 1459;
    int ERROR_TIMEOUT = 1460;
    int ERROR_INVALID_MONITOR_HANDLE = 1461;
    int ERROR_INCORRECT_SIZE = 1462;
    int ERROR_SYMLINK_CLASS_DISABLED = 1463;
    int ERROR_SYMLINK_NOT_SUPPORTED = 1464;
    int ERROR_XML_PARSE_ERROR = 1465;
    int ERROR_XMLDSIG_ERROR = 1466;
    int ERROR_RESTART_APPLICATION = 1467;
    int ERROR_WRONG_COMPARTMENT = 1468;
    int ERROR_AUTHIP_FAILURE = 1469;
    int ERROR_EVENTLOG_FILE_CORRUPT = 1500;
    int ERROR_EVENTLOG_CANT_START = 1501;
    int ERROR_LOG_FILE_FULL = 1502;
    int ERROR_EVENTLOG_FILE_CHANGED = 1503;
    int ERROR_INVALID_TASK_NAME = 1550;
    int ERROR_INVALID_TASK_INDEX = 1551;
    int ERROR_THREAD_ALREADY_IN_TASK = 1552;
    int ERROR_INSTALL_SERVICE_FAILURE = 1601;
    int ERROR_INSTALL_USEREXIT = 1602;
    int ERROR_INSTALL_FAILURE = 1603;
    int ERROR_INSTALL_SUSPEND = 1604;
    int ERROR_UNKNOWN_PRODUCT = 1605;
    int ERROR_UNKNOWN_FEATURE = 1606;
    int ERROR_UNKNOWN_COMPONENT = 1607;
    int ERROR_UNKNOWN_PROPERTY = 1608;
    int ERROR_INVALID_HANDLE_STATE = 1609;
    int ERROR_BAD_CONFIGURATION = 1610;
    int ERROR_INDEX_ABSENT = 1611;
    int ERROR_INSTALL_SOURCE_ABSENT = 1612;
    int ERROR_INSTALL_PACKAGE_VERSION = 1613;
    int ERROR_PRODUCT_UNINSTALLED = 1614;
    int ERROR_BAD_QUERY_SYNTAX = 1615;
    int ERROR_INVALID_FIELD = 1616;
    int ERROR_DEVICE_REMOVED = 1617;
    int ERROR_INSTALL_ALREADY_RUNNING = 1618;
    int ERROR_INSTALL_PACKAGE_OPEN_FAILED = 1619;
    int ERROR_INSTALL_PACKAGE_INVALID = 1620;
    int ERROR_INSTALL_UI_FAILURE = 1621;
    int ERROR_INSTALL_LOG_FAILURE = 1622;
    int ERROR_INSTALL_LANGUAGE_UNSUPPORTED = 1623;
    int ERROR_INSTALL_TRANSFORM_FAILURE = 1624;
    int ERROR_INSTALL_PACKAGE_REJECTED = 1625;
    int ERROR_FUNCTION_NOT_CALLED = 1626;
    int ERROR_FUNCTION_FAILED = 1627;
    int ERROR_INVALID_TABLE = 1628;
    int ERROR_DATATYPE_MISMATCH = 1629;
    int ERROR_UNSUPPORTED_TYPE = 1630;
    int ERROR_CREATE_FAILED = 1631;
    int ERROR_INSTALL_TEMP_UNWRITABLE = 1632;
    int ERROR_INSTALL_PLATFORM_UNSUPPORTED = 1633;
    int ERROR_INSTALL_NOTUSED = 1634;
    int ERROR_PATCH_PACKAGE_OPEN_FAILED = 1635;
    int ERROR_PATCH_PACKAGE_INVALID = 1636;
    int ERROR_PATCH_PACKAGE_UNSUPPORTED = 1637;
    int ERROR_PRODUCT_VERSION = 1638;
    int ERROR_INVALID_COMMAND_LINE = 1639;
    int ERROR_INSTALL_REMOTE_DISALLOWED = 1640;
    int ERROR_SUCCESS_REBOOT_INITIATED = 1641;
    int ERROR_PATCH_TARGET_NOT_FOUND = 1642;
    int ERROR_PATCH_PACKAGE_REJECTED = 1643;
    int ERROR_INSTALL_TRANSFORM_REJECTED = 1644;
    int ERROR_INSTALL_REMOTE_PROHIBITED = 1645;
    int ERROR_PATCH_REMOVAL_UNSUPPORTED = 1646;
    int ERROR_UNKNOWN_PATCH = 1647;
    int ERROR_PATCH_NO_SEQUENCE = 1648;
    int ERROR_PATCH_REMOVAL_DISALLOWED = 1649;
    int ERROR_INVALID_PATCH_XML = 1650;
    int ERROR_PATCH_MANAGED_ADVERTISED_PRODUCT = 1651;
    int ERROR_INSTALL_SERVICE_SAFEBOOT = 1652;
    int RPC_S_INVALID_STRING_BINDING = 1700;
    int RPC_S_WRONG_KIND_OF_BINDING = 1701;
    int RPC_S_INVALID_BINDING = 1702;
    int RPC_S_PROTSEQ_NOT_SUPPORTED = 1703;
    int RPC_S_INVALID_RPC_PROTSEQ = 1704;
    int RPC_S_INVALID_STRING_UUID = 1705;
    int RPC_S_INVALID_ENDPOINT_FORMAT = 1706;
    int RPC_S_INVALID_NET_ADDR = 1707;
    int RPC_S_NO_ENDPOINT_FOUND = 1708;
    int RPC_S_INVALID_TIMEOUT = 1709;
    int RPC_S_OBJECT_NOT_FOUND = 1710;
    int RPC_S_ALREADY_REGISTERED = 1711;
    int RPC_S_TYPE_ALREADY_REGISTERED = 1712;
    int RPC_S_ALREADY_LISTENING = 1713;
    int RPC_S_NO_PROTSEQS_REGISTERED = 1714;
    int RPC_S_NOT_LISTENING = 1715;
    int RPC_S_UNKNOWN_MGR_TYPE = 1716;
    int RPC_S_UNKNOWN_IF = 1717;
    int RPC_S_NO_BINDINGS = 1718;
    int RPC_S_NO_PROTSEQS = 1719;
    int RPC_S_CANT_CREATE_ENDPOINT = 1720;
    int RPC_S_OUT_OF_RESOURCES = 1721;
    int RPC_S_SERVER_UNAVAILABLE = 1722;
    int RPC_S_SERVER_TOO_BUSY = 1723;
    int RPC_S_INVALID_NETWORK_OPTIONS = 1724;
    int RPC_S_NO_CALL_ACTIVE = 1725;
    int RPC_S_CALL_FAILED = 1726;
    int RPC_S_CALL_FAILED_DNE = 1727;
    int RPC_S_PROTOCOL_ERROR = 1728;
    int RPC_S_PROXY_ACCESS_DENIED = 1729;
    int RPC_S_UNSUPPORTED_TRANS_SYN = 1730;
    int RPC_S_UNSUPPORTED_TYPE = 1732;
    int RPC_S_INVALID_TAG = 1733;
    int RPC_S_INVALID_BOUND = 1734;
    int RPC_S_NO_ENTRY_NAME = 1735;
    int RPC_S_INVALID_NAME_SYNTAX = 1736;
    int RPC_S_UNSUPPORTED_NAME_SYNTAX = 1737;
    int RPC_S_UUID_NO_ADDRESS = 1739;
    int RPC_S_DUPLICATE_ENDPOINT = 1740;
    int RPC_S_UNKNOWN_AUTHN_TYPE = 1741;
    int RPC_S_MAX_CALLS_TOO_SMALL = 1742;
    int RPC_S_STRING_TOO_LONG = 1743;
    int RPC_S_PROTSEQ_NOT_FOUND = 1744;
    int RPC_S_PROCNUM_OUT_OF_RANGE = 1745;
    int RPC_S_BINDING_HAS_NO_AUTH = 1746;
    int RPC_S_UNKNOWN_AUTHN_SERVICE = 1747;
    int RPC_S_UNKNOWN_AUTHN_LEVEL = 1748;
    int RPC_S_INVALID_AUTH_IDENTITY = 1749;
    int RPC_S_UNKNOWN_AUTHZ_SERVICE = 1750;
    int EPT_S_INVALID_ENTRY = 1751;
    int EPT_S_CANT_PERFORM_OP = 1752;
    int EPT_S_NOT_REGISTERED = 1753;
    int RPC_S_NOTHING_TO_EXPORT = 1754;
    int RPC_S_INCOMPLETE_NAME = 1755;
    int RPC_S_INVALID_VERS_OPTION = 1756;
    int RPC_S_NO_MORE_MEMBERS = 1757;
    int RPC_S_NOT_ALL_OBJS_UNEXPORTED = 1758;
    int RPC_S_INTERFACE_NOT_FOUND = 1759;
    int RPC_S_ENTRY_ALREADY_EXISTS = 1760;
    int RPC_S_ENTRY_NOT_FOUND = 1761;
    int RPC_S_NAME_SERVICE_UNAVAILABLE = 1762;
    int RPC_S_INVALID_NAF_ID = 1763;
    int RPC_S_CANNOT_SUPPORT = 1764;
    int RPC_S_NO_CONTEXT_AVAILABLE = 1765;
    int RPC_S_INTERNAL_ERROR = 1766;
    int RPC_S_ZERO_DIVIDE = 1767;
    int RPC_S_ADDRESS_ERROR = 1768;
    int RPC_S_FP_DIV_ZERO = 1769;
    int RPC_S_FP_UNDERFLOW = 1770;
    int RPC_S_FP_OVERFLOW = 1771;
    int RPC_X_NO_MORE_ENTRIES = 1772;
    int RPC_X_SS_CHAR_TRANS_OPEN_FAIL = 1773;
    int RPC_X_SS_CHAR_TRANS_SHORT_FILE = 1774;
    int RPC_X_SS_IN_NULL_CONTEXT = 1775;
    int RPC_X_SS_CONTEXT_DAMAGED = 1777;
    int RPC_X_SS_HANDLES_MISMATCH = 1778;
    int RPC_X_SS_CANNOT_GET_CALL_HANDLE = 1779;
    int RPC_X_NULL_REF_POINTER = 1780;
    int RPC_X_ENUM_VALUE_OUT_OF_RANGE = 1781;
    int RPC_X_BYTE_COUNT_TOO_SMALL = 1782;
    int RPC_X_BAD_STUB_DATA = 1783;
    int ERROR_INVALID_USER_BUFFER = 1784;
    int ERROR_UNRECOGNIZED_MEDIA = 1785;
    int ERROR_NO_TRUST_LSA_SECRET = 1786;
    int ERROR_NO_TRUST_SAM_ACCOUNT = 1787;
    int ERROR_TRUSTED_DOMAIN_FAILURE = 1788;
    int ERROR_TRUSTED_RELATIONSHIP_FAILURE = 1789;
    int ERROR_TRUST_FAILURE = 1790;
    int RPC_S_CALL_IN_PROGRESS = 1791;
    int ERROR_NETLOGON_NOT_STARTED = 1792;
    int ERROR_ACCOUNT_EXPIRED = 1793;
    int ERROR_REDIRECTOR_HAS_OPEN_HANDLES = 1794;
    int ERROR_PRINTER_DRIVER_ALREADY_INSTALLED = 1795;
    int ERROR_UNKNOWN_PORT = 1796;
    int ERROR_UNKNOWN_PRINTER_DRIVER = 1797;
    int ERROR_UNKNOWN_PRINTPROCESSOR = 1798;
    int ERROR_INVALID_SEPARATOR_FILE = 1799;
    int ERROR_INVALID_PRIORITY = 1800;
    int ERROR_INVALID_PRINTER_NAME = 1801;
    int ERROR_PRINTER_ALREADY_EXISTS = 1802;
    int ERROR_INVALID_PRINTER_COMMAND = 1803;
    int ERROR_INVALID_DATATYPE = 1804;
    int ERROR_INVALID_ENVIRONMENT = 1805;
    int RPC_S_NO_MORE_BINDINGS = 1806;
    int ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT = 1807;
    int ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT = 1808;
    int ERROR_NOLOGON_SERVER_TRUST_ACCOUNT = 1809;
    int ERROR_DOMAIN_TRUST_INCONSISTENT = 1810;
    int ERROR_SERVER_HAS_OPEN_HANDLES = 1811;
    int ERROR_RESOURCE_DATA_NOT_FOUND = 1812;
    int ERROR_RESOURCE_TYPE_NOT_FOUND = 1813;
    int ERROR_RESOURCE_NAME_NOT_FOUND = 1814;
    int ERROR_RESOURCE_LANG_NOT_FOUND = 1815;
    int ERROR_NOT_ENOUGH_QUOTA = 1816;
    int RPC_S_NO_INTERFACES = 1817;
    int RPC_S_CALL_CANCELLED = 1818;
    int RPC_S_BINDING_INCOMPLETE = 1819;
    int RPC_S_COMM_FAILURE = 1820;
    int RPC_S_UNSUPPORTED_AUTHN_LEVEL = 1821;
    int RPC_S_NO_PRINC_NAME = 1822;
    int RPC_S_NOT_RPC_ERROR = 1823;
    int RPC_S_UUID_LOCAL_ONLY = 1824;
    int RPC_S_SEC_PKG_ERROR = 1825;
    int RPC_S_NOT_CANCELLED = 1826;
    int RPC_X_INVALID_ES_ACTION = 1827;
    int RPC_X_WRONG_ES_VERSION = 1828;
    int RPC_X_WRONG_STUB_VERSION = 1829;
    int RPC_X_INVALID_PIPE_OBJECT = 1830;
    int RPC_X_WRONG_PIPE_ORDER = 1831;
    int RPC_X_WRONG_PIPE_VERSION = 1832;
    int RPC_S_GROUP_MEMBER_NOT_FOUND = 1898;
    int EPT_S_CANT_CREATE = 1899;
    int RPC_S_INVALID_OBJECT = 1900;
    int ERROR_INVALID_TIME = 1901;
    int ERROR_INVALID_FORM_NAME = 1902;
    int ERROR_INVALID_FORM_SIZE = 1903;
    int ERROR_ALREADY_WAITING = 1904;
    int ERROR_PRINTER_DELETED = 1905;
    int ERROR_INVALID_PRINTER_STATE = 1906;
    int ERROR_PASSWORD_MUST_CHANGE = 1907;
    int ERROR_DOMAIN_CONTROLLER_NOT_FOUND = 1908;
    int ERROR_ACCOUNT_LOCKED_OUT = 1909;
    int OR_INVALID_OXID = 1910;
    int OR_INVALID_OID = 1911;
    int OR_INVALID_SET = 1912;
    int RPC_S_SEND_INCOMPLETE = 1913;
    int RPC_S_INVALID_ASYNC_HANDLE = 1914;
    int RPC_S_INVALID_ASYNC_CALL = 1915;
    int RPC_X_PIPE_CLOSED = 1916;
    int RPC_X_PIPE_DISCIPLINE_ERROR = 1917;
    int RPC_X_PIPE_EMPTY = 1918;
    int ERROR_NO_SITENAME = 1919;
    int ERROR_CANT_ACCESS_FILE = 1920;
    int ERROR_CANT_RESOLVE_FILENAME = 1921;
    int RPC_S_ENTRY_TYPE_MISMATCH = 1922;
    int RPC_S_NOT_ALL_OBJS_EXPORTED = 1923;
    int RPC_S_INTERFACE_NOT_EXPORTED = 1924;
    int RPC_S_PROFILE_NOT_ADDED = 1925;
    int RPC_S_PRF_ELT_NOT_ADDED = 1926;
    int RPC_S_PRF_ELT_NOT_REMOVED = 1927;
    int RPC_S_GRP_ELT_NOT_ADDED = 1928;
    int RPC_S_GRP_ELT_NOT_REMOVED = 1929;
    int ERROR_KM_DRIVER_BLOCKED = 1930;
    int ERROR_CONTEXT_EXPIRED = 1931;
    int ERROR_PER_USER_TRUST_QUOTA_EXCEEDED = 1932;
    int ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED = 1933;
    int ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED = 1934;
    int ERROR_AUTHENTICATION_FIREWALL_FAILED = 1935;
    int ERROR_REMOTE_PRINT_CONNECTIONS_BLOCKED = 1936;
    int ERROR_NTLM_BLOCKED = 1937;
    int ERROR_INVALID_PIXEL_FORMAT = 2000;
    int ERROR_BAD_DRIVER = 2001;
    int ERROR_INVALID_WINDOW_STYLE = 2002;
    int ERROR_METAFILE_NOT_SUPPORTED = 2003;
    int ERROR_TRANSFORM_NOT_SUPPORTED = 2004;
    int ERROR_CLIPPING_NOT_SUPPORTED = 2005;
    int ERROR_INVALID_CMM = 2010;
    int ERROR_INVALID_PROFILE = 2011;
    int ERROR_TAG_NOT_FOUND = 2012;
    int ERROR_TAG_NOT_PRESENT = 2013;
    int ERROR_DUPLICATE_TAG = 2014;
    int ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE = 2015;
    int ERROR_PROFILE_NOT_FOUND = 2016;
    int ERROR_INVALID_COLORSPACE = 2017;
    int ERROR_ICM_NOT_ENABLED = 2018;
    int ERROR_DELETING_ICM_XFORM = 2019;
    int ERROR_INVALID_TRANSFORM = 2020;
    int ERROR_COLORSPACE_MISMATCH = 2021;
    int ERROR_INVALID_COLORINDEX = 2022;
    int ERROR_PROFILE_DOES_NOT_MATCH_DEVICE = 2023;
    int ERROR_CONNECTED_OTHER_PASSWORD = 2108;
    int ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT = 2109;
    int ERROR_BAD_USERNAME = 2202;
    int ERROR_NOT_CONNECTED = 2250;
    int ERROR_OPEN_FILES = 2401;
    int ERROR_ACTIVE_CONNECTIONS = 2402;
    int ERROR_DEVICE_IN_USE = 2404;
    int ERROR_UNKNOWN_PRINT_MONITOR = 3000;
    int ERROR_PRINTER_DRIVER_IN_USE = 3001;
    int ERROR_SPOOL_FILE_NOT_FOUND = 3002;
    int ERROR_SPL_NO_STARTDOC = 3003;
    int ERROR_SPL_NO_ADDJOB = 3004;
    int ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED = 3005;
    int ERROR_PRINT_MONITOR_ALREADY_INSTALLED = 3006;
    int ERROR_INVALID_PRINT_MONITOR = 3007;
    int ERROR_PRINT_MONITOR_IN_USE = 3008;
    int ERROR_PRINTER_HAS_JOBS_QUEUED = 3009;
    int ERROR_SUCCESS_REBOOT_REQUIRED = 3010;
    int ERROR_SUCCESS_RESTART_REQUIRED = 3011;
    int ERROR_PRINTER_NOT_FOUND = 3012;
    int ERROR_PRINTER_DRIVER_WARNED = 3013;
    int ERROR_PRINTER_DRIVER_BLOCKED = 3014;
    int ERROR_PRINTER_DRIVER_PACKAGE_IN_USE = 3015;
    int ERROR_CORE_DRIVER_PACKAGE_NOT_FOUND = 3016;
    int ERROR_FAIL_REBOOT_REQUIRED = 3017;
    int ERROR_FAIL_REBOOT_INITIATED = 3018;
    int ERROR_PRINTER_DRIVER_DOWNLOAD_NEEDED = 3019;
    int ERROR_PRINT_JOB_RESTART_REQUIRED = 3020;
    int ERROR_IO_REISSUE_AS_CACHED = 3950;
    int ERROR_WINS_INTERNAL = 4000;
    int ERROR_CAN_NOT_DEL_LOCAL_WINS = 4001;
    int ERROR_STATIC_INIT = 4002;
    int ERROR_INC_BACKUP = 4003;
    int ERROR_FULL_BACKUP = 4004;
    int ERROR_REC_NON_EXISTENT = 4005;
    int ERROR_RPL_NOT_ALLOWED = 4006;
    int ERROR_DHCP_ADDRESS_CONFLICT = 4100;
    int ERROR_WMI_GUID_NOT_FOUND = 4200;
    int ERROR_WMI_INSTANCE_NOT_FOUND = 4201;
    int ERROR_WMI_ITEMID_NOT_FOUND = 4202;
    int ERROR_WMI_TRY_AGAIN = 4203;
    int ERROR_WMI_DP_NOT_FOUND = 4204;
    int ERROR_WMI_UNRESOLVED_INSTANCE_REF = 4205;
    int ERROR_WMI_ALREADY_ENABLED = 4206;
    int ERROR_WMI_GUID_DISCONNECTED = 4207;
    int ERROR_WMI_SERVER_UNAVAILABLE = 4208;
    int ERROR_WMI_DP_FAILED = 4209;
    int ERROR_WMI_INVALID_MOF = 4210;
    int ERROR_WMI_INVALID_REGINFO = 4211;
    int ERROR_WMI_ALREADY_DISABLED = 4212;
    int ERROR_WMI_READ_ONLY = 4213;
    int ERROR_WMI_SET_FAILURE = 4214;
    int ERROR_INVALID_MEDIA = 4300;
    int ERROR_INVALID_LIBRARY = 4301;
    int ERROR_INVALID_MEDIA_POOL = 4302;
    int ERROR_DRIVE_MEDIA_MISMATCH = 4303;
    int ERROR_MEDIA_OFFLINE = 4304;
    int ERROR_LIBRARY_OFFLINE = 4305;
    int ERROR_EMPTY = 4306;
    int ERROR_NOT_EMPTY = 4307;
    int ERROR_MEDIA_UNAVAILABLE = 4308;
    int ERROR_RESOURCE_DISABLED = 4309;
    int ERROR_INVALID_CLEANER = 4310;
    int ERROR_UNABLE_TO_CLEAN = 4311;
    int ERROR_OBJECT_NOT_FOUND = 4312;
    int ERROR_DATABASE_FAILURE = 4313;
    int ERROR_DATABASE_FULL = 4314;
    int ERROR_MEDIA_INCOMPATIBLE = 4315;
    int ERROR_RESOURCE_NOT_PRESENT = 4316;
    int ERROR_INVALID_OPERATION = 4317;
    int ERROR_MEDIA_NOT_AVAILABLE = 4318;
    int ERROR_DEVICE_NOT_AVAILABLE = 4319;
    int ERROR_REQUEST_REFUSED = 4320;
    int ERROR_INVALID_DRIVE_OBJECT = 4321;
    int ERROR_LIBRARY_FULL = 4322;
    int ERROR_MEDIUM_NOT_ACCESSIBLE = 4323;
    int ERROR_UNABLE_TO_LOAD_MEDIUM = 4324;
    int ERROR_UNABLE_TO_INVENTORY_DRIVE = 4325;
    int ERROR_UNABLE_TO_INVENTORY_SLOT = 4326;
    int ERROR_UNABLE_TO_INVENTORY_TRANSPORT = 4327;
    int ERROR_TRANSPORT_FULL = 4328;
    int ERROR_CONTROLLING_IEPORT = 4329;
    int ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA = 4330;
    int ERROR_CLEANER_SLOT_SET = 4331;
    int ERROR_CLEANER_SLOT_NOT_SET = 4332;
    int ERROR_CLEANER_CARTRIDGE_SPENT = 4333;
    int ERROR_UNEXPECTED_OMID = 4334;
    int ERROR_CANT_DELETE_LAST_ITEM = 4335;
    int ERROR_MESSAGE_EXCEEDS_MAX_SIZE = 4336;
    int ERROR_VOLUME_CONTAINS_SYS_FILES = 4337;
    int ERROR_INDIGENOUS_TYPE = 4338;
    int ERROR_NO_SUPPORTING_DRIVES = 4339;
    int ERROR_CLEANER_CARTRIDGE_INSTALLED = 4340;
    int ERROR_IEPORT_FULL = 4341;
    int ERROR_FILE_OFFLINE = 4350;
    int ERROR_REMOTE_STORAGE_NOT_ACTIVE = 4351;
    int ERROR_REMOTE_STORAGE_MEDIA_ERROR = 4352;
    int ERROR_NOT_A_REPARSE_POINT = 4390;
    int ERROR_REPARSE_ATTRIBUTE_CONFLICT = 4391;
    int ERROR_INVALID_REPARSE_DATA = 4392;
    int ERROR_REPARSE_TAG_INVALID = 4393;
    int ERROR_REPARSE_TAG_MISMATCH = 4394;
    int ERROR_VOLUME_NOT_SIS_ENABLED = 4500;
    int ERROR_DEPENDENT_RESOURCE_EXISTS = 5001;
    int ERROR_DEPENDENCY_NOT_FOUND = 5002;
    int ERROR_DEPENDENCY_ALREADY_EXISTS = 5003;
    int ERROR_RESOURCE_NOT_ONLINE = 5004;
    int ERROR_HOST_NODE_NOT_AVAILABLE = 5005;
    int ERROR_RESOURCE_NOT_AVAILABLE = 5006;
    int ERROR_RESOURCE_NOT_FOUND = 5007;
    int ERROR_SHUTDOWN_CLUSTER = 5008;
    int ERROR_CANT_EVICT_ACTIVE_NODE = 5009;
    int ERROR_OBJECT_ALREADY_EXISTS = 5010;
    int ERROR_OBJECT_IN_LIST = 5011;
    int ERROR_GROUP_NOT_AVAILABLE = 5012;
    int ERROR_GROUP_NOT_FOUND = 5013;
    int ERROR_GROUP_NOT_ONLINE = 5014;
    int ERROR_HOST_NODE_NOT_RESOURCE_OWNER = 5015;
    int ERROR_HOST_NODE_NOT_GROUP_OWNER = 5016;
    int ERROR_RESMON_CREATE_FAILED = 5017;
    int ERROR_RESMON_ONLINE_FAILED = 5018;
    int ERROR_RESOURCE_ONLINE = 5019;
    int ERROR_QUORUM_RESOURCE = 5020;
    int ERROR_NOT_QUORUM_CAPABLE = 5021;
    int ERROR_CLUSTER_SHUTTING_DOWN = 5022;
    int ERROR_INVALID_STATE = 5023;
    int ERROR_RESOURCE_PROPERTIES_STORED = 5024;
    int ERROR_NOT_QUORUM_CLASS = 5025;
    int ERROR_CORE_RESOURCE = 5026;
    int ERROR_QUORUM_RESOURCE_ONLINE_FAILED = 5027;
    int ERROR_QUORUMLOG_OPEN_FAILED = 5028;
    int ERROR_CLUSTERLOG_CORRUPT = 5029;
    int ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE = 5030;
    int ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE = 5031;
    int ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND = 5032;
    int ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE = 5033;
    int ERROR_QUORUM_OWNER_ALIVE = 5034;
    int ERROR_NETWORK_NOT_AVAILABLE = 5035;
    int ERROR_NODE_NOT_AVAILABLE = 5036;
    int ERROR_ALL_NODES_NOT_AVAILABLE = 5037;
    int ERROR_RESOURCE_FAILED = 5038;
    int ERROR_CLUSTER_INVALID_NODE = 5039;
    int ERROR_CLUSTER_NODE_EXISTS = 5040;
    int ERROR_CLUSTER_JOIN_IN_PROGRESS = 5041;
    int ERROR_CLUSTER_NODE_NOT_FOUND = 5042;
    int ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND = 5043;
    int ERROR_CLUSTER_NETWORK_EXISTS = 5044;
    int ERROR_CLUSTER_NETWORK_NOT_FOUND = 5045;
    int ERROR_CLUSTER_NETINTERFACE_EXISTS = 5046;
    int ERROR_CLUSTER_NETINTERFACE_NOT_FOUND = 5047;
    int ERROR_CLUSTER_INVALID_REQUEST = 5048;
    int ERROR_CLUSTER_INVALID_NETWORK_PROVIDER = 5049;
    int ERROR_CLUSTER_NODE_DOWN = 5050;
    int ERROR_CLUSTER_NODE_UNREACHABLE = 5051;
    int ERROR_CLUSTER_NODE_NOT_MEMBER = 5052;
    int ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS = 5053;
    int ERROR_CLUSTER_INVALID_NETWORK = 5054;
    int ERROR_CLUSTER_NODE_UP = 5056;
    int ERROR_CLUSTER_IPADDR_IN_USE = 5057;
    int ERROR_CLUSTER_NODE_NOT_PAUSED = 5058;
    int ERROR_CLUSTER_NO_SECURITY_CONTEXT = 5059;
    int ERROR_CLUSTER_NETWORK_NOT_INTERNAL = 5060;
    int ERROR_CLUSTER_NODE_ALREADY_UP = 5061;
    int ERROR_CLUSTER_NODE_ALREADY_DOWN = 5062;
    int ERROR_CLUSTER_NETWORK_ALREADY_ONLINE = 5063;
    int ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE = 5064;
    int ERROR_CLUSTER_NODE_ALREADY_MEMBER = 5065;
    int ERROR_CLUSTER_LAST_INTERNAL_NETWORK = 5066;
    int ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS = 5067;
    int ERROR_INVALID_OPERATION_ON_QUORUM = 5068;
    int ERROR_DEPENDENCY_NOT_ALLOWED = 5069;
    int ERROR_CLUSTER_NODE_PAUSED = 5070;
    int ERROR_NODE_CANT_HOST_RESOURCE = 5071;
    int ERROR_CLUSTER_NODE_NOT_READY = 5072;
    int ERROR_CLUSTER_NODE_SHUTTING_DOWN = 5073;
    int ERROR_CLUSTER_JOIN_ABORTED = 5074;
    int ERROR_CLUSTER_INCOMPATIBLE_VERSIONS = 5075;
    int ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED = 5076;
    int ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED = 5077;
    int ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND = 5078;
    int ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED = 5079;
    int ERROR_CLUSTER_RESNAME_NOT_FOUND = 5080;
    int ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED = 5081;
    int ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST = 5082;
    int ERROR_CLUSTER_DATABASE_SEQMISMATCH = 5083;
    int ERROR_RESMON_INVALID_STATE = 5084;
    int ERROR_CLUSTER_GUM_NOT_LOCKER = 5085;
    int ERROR_QUORUM_DISK_NOT_FOUND = 5086;
    int ERROR_DATABASE_BACKUP_CORRUPT = 5087;
    int ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT = 5088;
    int ERROR_RESOURCE_PROPERTY_UNCHANGEABLE = 5089;
    int ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE = 5890;
    int ERROR_CLUSTER_QUORUMLOG_NOT_FOUND = 5891;
    int ERROR_CLUSTER_MEMBERSHIP_HALT = 5892;
    int ERROR_CLUSTER_INSTANCE_ID_MISMATCH = 5893;
    int ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP = 5894;
    int ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH = 5895;
    int ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP = 5896;
    int ERROR_CLUSTER_PARAMETER_MISMATCH = 5897;
    int ERROR_NODE_CANNOT_BE_CLUSTERED = 5898;
    int ERROR_CLUSTER_WRONG_OS_VERSION = 5899;
    int ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME = 5900;
    int ERROR_CLUSCFG_ALREADY_COMMITTED = 5901;
    int ERROR_CLUSCFG_ROLLBACK_FAILED = 5902;
    int ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT = 5903;
    int ERROR_CLUSTER_OLD_VERSION = 5904;
    int ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME = 5905;
    int ERROR_CLUSTER_NO_NET_ADAPTERS = 5906;
    int ERROR_CLUSTER_POISONED = 5907;
    int ERROR_CLUSTER_GROUP_MOVING = 5908;
    int ERROR_CLUSTER_RESOURCE_TYPE_BUSY = 5909;
    int ERROR_RESOURCE_CALL_TIMED_OUT = 5910;
    int ERROR_INVALID_CLUSTER_IPV6_ADDRESS = 5911;
    int ERROR_CLUSTER_INTERNAL_INVALID_FUNCTION = 5912;
    int ERROR_CLUSTER_PARAMETER_OUT_OF_BOUNDS = 5913;
    int ERROR_CLUSTER_PARTIAL_SEND = 5914;
    int ERROR_CLUSTER_REGISTRY_INVALID_FUNCTION = 5915;
    int ERROR_CLUSTER_INVALID_STRING_TERMINATION = 5916;
    int ERROR_CLUSTER_INVALID_STRING_FORMAT = 5917;
    int ERROR_CLUSTER_DATABASE_TRANSACTION_IN_PROGRESS = 5918;
    int ERROR_CLUSTER_DATABASE_TRANSACTION_NOT_IN_PROGRESS = 5919;
    int ERROR_CLUSTER_NULL_DATA = 5920;
    int ERROR_CLUSTER_PARTIAL_READ = 5921;
    int ERROR_CLUSTER_PARTIAL_WRITE = 5922;
    int ERROR_CLUSTER_CANT_DESERIALIZE_DATA = 5923;
    int ERROR_DEPENDENT_RESOURCE_PROPERTY_CONFLICT = 5924;
    int ERROR_CLUSTER_NO_QUORUM = 5925;
    int ERROR_CLUSTER_INVALID_IPV6_NETWORK = 5926;
    int ERROR_CLUSTER_INVALID_IPV6_TUNNEL_NETWORK = 5927;
    int ERROR_QUORUM_NOT_ALLOWED_IN_THIS_GROUP = 5928;
    int ERROR_DEPENDENCY_TREE_TOO_COMPLEX = 5929;
    int ERROR_EXCEPTION_IN_RESOURCE_CALL = 5930;
    int ERROR_CLUSTER_RHS_FAILED_INITIALIZATION = 5931;
    int ERROR_CLUSTER_NOT_INSTALLED = 5932;
    int ERROR_CLUSTER_RESOURCES_MUST_BE_ONLINE_ON_THE_SAME_NODE = 5933;
    int ERROR_ENCRYPTION_FAILED = 6000;
    int ERROR_DECRYPTION_FAILED = 6001;
    int ERROR_FILE_ENCRYPTED = 6002;
    int ERROR_NO_RECOVERY_POLICY = 6003;
    int ERROR_NO_EFS = 6004;
    int ERROR_WRONG_EFS = 6005;
    int ERROR_NO_USER_KEYS = 6006;
    int ERROR_FILE_NOT_ENCRYPTED = 6007;
    int ERROR_NOT_EXPORT_FORMAT = 6008;
    int ERROR_FILE_READ_ONLY = 6009;
    int ERROR_DIR_EFS_DISALLOWED = 6010;
    int ERROR_EFS_SERVER_NOT_TRUSTED = 6011;
    int ERROR_BAD_RECOVERY_POLICY = 6012;
    int ERROR_EFS_ALG_BLOB_TOO_BIG = 6013;
    int ERROR_VOLUME_NOT_SUPPORT_EFS = 6014;
    int ERROR_EFS_DISABLED = 6015;
    int ERROR_EFS_VERSION_NOT_SUPPORT = 6016;
    int ERROR_CS_ENCRYPTION_INVALID_SERVER_RESPONSE = 6017;
    int ERROR_CS_ENCRYPTION_UNSUPPORTED_SERVER = 6018;
    int ERROR_CS_ENCRYPTION_EXISTING_ENCRYPTED_FILE = 6019;
    int ERROR_CS_ENCRYPTION_NEW_ENCRYPTED_FILE = 6020;
    int ERROR_CS_ENCRYPTION_FILE_NOT_CSE = 6021;
    int ERROR_NO_BROWSER_SERVERS_FOUND = 6118;
    int SCHED_E_SERVICE_NOT_LOCALSYSTEM = 6200;
    int ERROR_LOG_SECTOR_INVALID = 6600;
    int ERROR_LOG_SECTOR_PARITY_INVALID = 6601;
    int ERROR_LOG_SECTOR_REMAPPED = 6602;
    int ERROR_LOG_BLOCK_INCOMPLETE = 6603;
    int ERROR_LOG_INVALID_RANGE = 6604;
    int ERROR_LOG_BLOCKS_EXHAUSTED = 6605;
    int ERROR_LOG_READ_CONTEXT_INVALID = 6606;
    int ERROR_LOG_RESTART_INVALID = 6607;
    int ERROR_LOG_BLOCK_VERSION = 6608;
    int ERROR_LOG_BLOCK_INVALID = 6609;
    int ERROR_LOG_READ_MODE_INVALID = 6610;
    int ERROR_LOG_NO_RESTART = 6611;
    int ERROR_LOG_METADATA_CORRUPT = 6612;
    int ERROR_LOG_METADATA_INVALID = 6613;
    int ERROR_LOG_METADATA_INCONSISTENT = 6614;
    int ERROR_LOG_RESERVATION_INVALID = 6615;
    int ERROR_LOG_CANT_DELETE = 6616;
    int ERROR_LOG_CONTAINER_LIMIT_EXCEEDED = 6617;
    int ERROR_LOG_START_OF_LOG = 6618;
    int ERROR_LOG_POLICY_ALREADY_INSTALLED = 6619;
    int ERROR_LOG_POLICY_NOT_INSTALLED = 6620;
    int ERROR_LOG_POLICY_INVALID = 6621;
    int ERROR_LOG_POLICY_CONFLICT = 6622;
    int ERROR_LOG_PINNED_ARCHIVE_TAIL = 6623;
    int ERROR_LOG_RECORD_NONEXISTENT = 6624;
    int ERROR_LOG_RECORDS_RESERVED_INVALID = 6625;
    int ERROR_LOG_SPACE_RESERVED_INVALID = 6626;
    int ERROR_LOG_TAIL_INVALID = 6627;
    int ERROR_LOG_FULL = 6628;
    int ERROR_COULD_NOT_RESIZE_LOG = 6629;
    int ERROR_LOG_MULTIPLEXED = 6630;
    int ERROR_LOG_DEDICATED = 6631;
    int ERROR_LOG_ARCHIVE_NOT_IN_PROGRESS = 6632;
    int ERROR_LOG_ARCHIVE_IN_PROGRESS = 6633;
    int ERROR_LOG_EPHEMERAL = 6634;
    int ERROR_LOG_NOT_ENOUGH_CONTAINERS = 6635;
    int ERROR_LOG_CLIENT_ALREADY_REGISTERED = 6636;
    int ERROR_LOG_CLIENT_NOT_REGISTERED = 6637;
    int ERROR_LOG_FULL_HANDLER_IN_PROGRESS = 6638;
    int ERROR_LOG_CONTAINER_READ_FAILED = 6639;
    int ERROR_LOG_CONTAINER_WRITE_FAILED = 6640;
    int ERROR_LOG_CONTAINER_OPEN_FAILED = 6641;
    int ERROR_LOG_CONTAINER_STATE_INVALID = 6642;
    int ERROR_LOG_STATE_INVALID = 6643;
    int ERROR_LOG_PINNED = 6644;
    int ERROR_LOG_METADATA_FLUSH_FAILED = 6645;
    int ERROR_LOG_INCONSISTENT_SECURITY = 6646;
    int ERROR_LOG_APPENDED_FLUSH_FAILED = 6647;
    int ERROR_LOG_PINNED_RESERVATION = 6648;
    int ERROR_INVALID_TRANSACTION = 6700;
    int ERROR_TRANSACTION_NOT_ACTIVE = 6701;
    int ERROR_TRANSACTION_REQUEST_NOT_VALID = 6702;
    int ERROR_TRANSACTION_NOT_REQUESTED = 6703;
    int ERROR_TRANSACTION_ALREADY_ABORTED = 6704;
    int ERROR_TRANSACTION_ALREADY_COMMITTED = 6705;
    int ERROR_TM_INITIALIZATION_FAILED = 6706;
    int ERROR_RESOURCEMANAGER_READ_ONLY = 6707;
    int ERROR_TRANSACTION_NOT_JOINED = 6708;
    int ERROR_TRANSACTION_SUPERIOR_EXISTS = 6709;
    int ERROR_CRM_PROTOCOL_ALREADY_EXISTS = 6710;
    int ERROR_TRANSACTION_PROPAGATION_FAILED = 6711;
    int ERROR_CRM_PROTOCOL_NOT_FOUND = 6712;
    int ERROR_TRANSACTION_INVALID_MARSHALL_BUFFER = 6713;
    int ERROR_CURRENT_TRANSACTION_NOT_VALID = 6714;
    int ERROR_TRANSACTION_NOT_FOUND = 6715;
    int ERROR_RESOURCEMANAGER_NOT_FOUND = 6716;
    int ERROR_ENLISTMENT_NOT_FOUND = 6717;
    int ERROR_TRANSACTIONMANAGER_NOT_FOUND = 6718;
    int ERROR_TRANSACTIONMANAGER_NOT_ONLINE = 6719;
    int ERROR_TRANSACTIONMANAGER_RECOVERY_NAME_COLLISION = 6720;
    int ERROR_TRANSACTION_NOT_ROOT = 6721;
    int ERROR_TRANSACTION_OBJECT_EXPIRED = 6722;
    int ERROR_TRANSACTION_RESPONSE_NOT_ENLISTED = 6723;
    int ERROR_TRANSACTION_RECORD_TOO_LONG = 6724;
    int ERROR_IMPLICIT_TRANSACTION_NOT_SUPPORTED = 6725;
    int ERROR_TRANSACTION_INTEGRITY_VIOLATED = 6726;
    int ERROR_TRANSACTIONAL_CONFLICT = 6800;
    int ERROR_RM_NOT_ACTIVE = 6801;
    int ERROR_RM_METADATA_CORRUPT = 6802;
    int ERROR_DIRECTORY_NOT_RM = 6803;
    int ERROR_TRANSACTIONS_UNSUPPORTED_REMOTE = 6805;
    int ERROR_LOG_RESIZE_INVALID_SIZE = 6806;
    int ERROR_OBJECT_NO_LONGER_EXISTS = 6807;
    int ERROR_STREAM_MINIVERSION_NOT_FOUND = 6808;
    int ERROR_STREAM_MINIVERSION_NOT_VALID = 6809;
    int ERROR_MINIVERSION_INACCESSIBLE_FROM_SPECIFIED_TRANSACTION = 6810;
    int ERROR_CANT_OPEN_MINIVERSION_WITH_MODIFY_INTENT = 6811;
    int ERROR_CANT_CREATE_MORE_STREAM_MINIVERSIONS = 6812;
    int ERROR_REMOTE_FILE_VERSION_MISMATCH = 6814;
    int ERROR_HANDLE_NO_LONGER_VALID = 6815;
    int ERROR_NO_TXF_METADATA = 6816;
    int ERROR_LOG_CORRUPTION_DETECTED = 6817;
    int ERROR_CANT_RECOVER_WITH_HANDLE_OPEN = 6818;
    int ERROR_RM_DISCONNECTED = 6819;
    int ERROR_ENLISTMENT_NOT_SUPERIOR = 6820;
    int ERROR_RECOVERY_NOT_NEEDED = 6821;
    int ERROR_RM_ALREADY_STARTED = 6822;
    int ERROR_FILE_IDENTITY_NOT_PERSISTENT = 6823;
    int ERROR_CANT_BREAK_TRANSACTIONAL_DEPENDENCY = 6824;
    int ERROR_CANT_CROSS_RM_BOUNDARY = 6825;
    int ERROR_TXF_DIR_NOT_EMPTY = 6826;
    int ERROR_INDOUBT_TRANSACTIONS_EXIST = 6827;
    int ERROR_TM_VOLATILE = 6828;
    int ERROR_ROLLBACK_TIMER_EXPIRED = 6829;
    int ERROR_TXF_ATTRIBUTE_CORRUPT = 6830;
    int ERROR_EFS_NOT_ALLOWED_IN_TRANSACTION = 6831;
    int ERROR_TRANSACTIONAL_OPEN_NOT_ALLOWED = 6832;
    int ERROR_LOG_GROWTH_FAILED = 6833;
    int ERROR_TRANSACTED_MAPPING_UNSUPPORTED_REMOTE = 6834;
    int ERROR_TXF_METADATA_ALREADY_PRESENT = 6835;
    int ERROR_TRANSACTION_SCOPE_CALLBACKS_NOT_SET = 6836;
    int ERROR_TRANSACTION_REQUIRED_PROMOTION = 6837;
    int ERROR_CANNOT_EXECUTE_FILE_IN_TRANSACTION = 6838;
    int ERROR_TRANSACTIONS_NOT_FROZEN = 6839;
    int ERROR_TRANSACTION_FREEZE_IN_PROGRESS = 6840;
    int ERROR_NOT_SNAPSHOT_VOLUME = 6841;
    int ERROR_NO_SAVEPOINT_WITH_OPEN_FILES = 6842;
    int ERROR_DATA_LOST_REPAIR = 6843;
    int ERROR_SPARSE_NOT_ALLOWED_IN_TRANSACTION = 6844;
    int ERROR_TM_IDENTITY_MISMATCH = 6845;
    int ERROR_FLOATED_SECTION = 6846;
    int ERROR_CANNOT_ACCEPT_TRANSACTED_WORK = 6847;
    int ERROR_CANNOT_ABORT_TRANSACTIONS = 6848;
    int ERROR_BAD_CLUSTERS = 6849;
    int ERROR_COMPRESSION_NOT_ALLOWED_IN_TRANSACTION = 6850;
    int ERROR_VOLUME_DIRTY = 6851;
    int ERROR_NO_LINK_TRACKING_IN_TRANSACTION = 6852;
    int ERROR_OPERATION_NOT_SUPPORTED_IN_TRANSACTION = 6853;
    int ERROR_CTX_WINSTATION_NAME_INVALID = 7001;
    int ERROR_CTX_INVALID_PD = 7002;
    int ERROR_CTX_PD_NOT_FOUND = 7003;
    int ERROR_CTX_WD_NOT_FOUND = 7004;
    int ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY = 7005;
    int ERROR_CTX_SERVICE_NAME_COLLISION = 7006;
    int ERROR_CTX_CLOSE_PENDING = 7007;
    int ERROR_CTX_NO_OUTBUF = 7008;
    int ERROR_CTX_MODEM_INF_NOT_FOUND = 7009;
    int ERROR_CTX_INVALID_MODEMNAME = 7010;
    int ERROR_CTX_MODEM_RESPONSE_ERROR = 7011;
    int ERROR_CTX_MODEM_RESPONSE_TIMEOUT = 7012;
    int ERROR_CTX_MODEM_RESPONSE_NO_CARRIER = 7013;
    int ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE = 7014;
    int ERROR_CTX_MODEM_RESPONSE_BUSY = 7015;
    int ERROR_CTX_MODEM_RESPONSE_VOICE = 7016;
    int ERROR_CTX_TD_ERROR = 7017;
    int ERROR_CTX_WINSTATION_NOT_FOUND = 7022;
    int ERROR_CTX_WINSTATION_ALREADY_EXISTS = 7023;
    int ERROR_CTX_WINSTATION_BUSY = 7024;
    int ERROR_CTX_BAD_VIDEO_MODE = 7025;
    int ERROR_CTX_GRAPHICS_INVALID = 7035;
    int ERROR_CTX_LOGON_DISABLED = 7037;
    int ERROR_CTX_NOT_CONSOLE = 7038;
    int ERROR_CTX_CLIENT_QUERY_TIMEOUT = 7040;
    int ERROR_CTX_CONSOLE_DISCONNECT = 7041;
    int ERROR_CTX_CONSOLE_CONNECT = 7042;
    int ERROR_CTX_SHADOW_DENIED = 7044;
    int ERROR_CTX_WINSTATION_ACCESS_DENIED = 7045;
    int ERROR_CTX_INVALID_WD = 7049;
    int ERROR_CTX_SHADOW_INVALID = 7050;
    int ERROR_CTX_SHADOW_DISABLED = 7051;
    int ERROR_CTX_CLIENT_LICENSE_IN_USE = 7052;
    int ERROR_CTX_CLIENT_LICENSE_NOT_SET = 7053;
    int ERROR_CTX_LICENSE_NOT_AVAILABLE = 7054;
    int ERROR_CTX_LICENSE_CLIENT_INVALID = 7055;
    int ERROR_CTX_LICENSE_EXPIRED = 7056;
    int ERROR_CTX_SHADOW_NOT_RUNNING = 7057;
    int ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE = 7058;
    int ERROR_ACTIVATION_COUNT_EXCEEDED = 7059;
    int ERROR_CTX_WINSTATIONS_DISABLED = 7060;
    int ERROR_CTX_ENCRYPTION_LEVEL_REQUIRED = 7061;
    int ERROR_CTX_SESSION_IN_USE = 7062;
    int ERROR_CTX_NO_FORCE_LOGOFF = 7063;
    int ERROR_CTX_ACCOUNT_RESTRICTION = 7064;
    int ERROR_RDP_PROTOCOL_ERROR = 7065;
    int ERROR_CTX_CDM_CONNECT = 7066;
    int ERROR_CTX_CDM_DISCONNECT = 7067;
    int ERROR_CTX_SECURITY_LAYER_ERROR = 7068;
    int ERROR_TS_INCOMPATIBLE_SESSIONS = 7069;
    int FRS_ERR_INVALID_API_SEQUENCE = 8001;
    int FRS_ERR_STARTING_SERVICE = 8002;
    int FRS_ERR_STOPPING_SERVICE = 8003;
    int FRS_ERR_INTERNAL_API = 8004;
    int FRS_ERR_INTERNAL = 8005;
    int FRS_ERR_SERVICE_COMM = 8006;
    int FRS_ERR_INSUFFICIENT_PRIV = 8007;
    int FRS_ERR_AUTHENTICATION = 8008;
    int FRS_ERR_PARENT_INSUFFICIENT_PRIV = 8009;
    int FRS_ERR_PARENT_AUTHENTICATION = 8010;
    int FRS_ERR_CHILD_TO_PARENT_COMM = 8011;
    int FRS_ERR_PARENT_TO_CHILD_COMM = 8012;
    int FRS_ERR_SYSVOL_POPULATE = 8013;
    int FRS_ERR_SYSVOL_POPULATE_TIMEOUT = 8014;
    int FRS_ERR_SYSVOL_IS_BUSY = 8015;
    int FRS_ERR_SYSVOL_DEMOTE = 8016;
    int FRS_ERR_INVALID_SERVICE_PARAMETER = 8017;
    int DS_S_SUCCESS = NO_ERROR;
    int ERROR_DS_NOT_INSTALLED = 8200;
    int ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY = 8201;
    int ERROR_DS_NO_ATTRIBUTE_OR_VALUE = 8202;
    int ERROR_DS_INVALID_ATTRIBUTE_SYNTAX = 8203;
    int ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED = 8204;
    int ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS = 8205;
    int ERROR_DS_BUSY = 8206;
    int ERROR_DS_UNAVAILABLE = 8207;
    int ERROR_DS_NO_RIDS_ALLOCATED = 8208;
    int ERROR_DS_NO_MORE_RIDS = 8209;
    int ERROR_DS_INCORRECT_ROLE_OWNER = 8210;
    int ERROR_DS_RIDMGR_INIT_ERROR = 8211;
    int ERROR_DS_OBJ_CLASS_VIOLATION = 8212;
    int ERROR_DS_CANT_ON_NON_LEAF = 8213;
    int ERROR_DS_CANT_ON_RDN = 8214;
    int ERROR_DS_CANT_MOD_OBJ_CLASS = 8215;
    int ERROR_DS_CROSS_DOM_MOVE_ERROR = 8216;
    int ERROR_DS_GC_NOT_AVAILABLE = 8217;
    int ERROR_SHARED_POLICY = 8218;
    int ERROR_POLICY_OBJECT_NOT_FOUND = 8219;
    int ERROR_POLICY_ONLY_IN_DS = 8220;
    int ERROR_PROMOTION_ACTIVE = 8221;
    int ERROR_NO_PROMOTION_ACTIVE = 8222;
    int ERROR_DS_OPERATIONS_ERROR = 8224;
    int ERROR_DS_PROTOCOL_ERROR = 8225;
    int ERROR_DS_TIMELIMIT_EXCEEDED = 8226;
    int ERROR_DS_SIZELIMIT_EXCEEDED = 8227;
    int ERROR_DS_ADMIN_LIMIT_EXCEEDED = 8228;
    int ERROR_DS_COMPARE_FALSE = 8229;
    int ERROR_DS_COMPARE_TRUE = 8230;
    int ERROR_DS_AUTH_METHOD_NOT_SUPPORTED = 8231;
    int ERROR_DS_STRONG_AUTH_REQUIRED = 8232;
    int ERROR_DS_INAPPROPRIATE_AUTH = 8233;
    int ERROR_DS_AUTH_UNKNOWN = 8234;
    int ERROR_DS_REFERRAL = 8235;
    int ERROR_DS_UNAVAILABLE_CRIT_EXTENSION = 8236;
    int ERROR_DS_CONFIDENTIALITY_REQUIRED = 8237;
    int ERROR_DS_INAPPROPRIATE_MATCHING = 8238;
    int ERROR_DS_CONSTRAINT_VIOLATION = 8239;
    int ERROR_DS_NO_SUCH_OBJECT = 8240;
    int ERROR_DS_ALIAS_PROBLEM = 8241;
    int ERROR_DS_INVALID_DN_SYNTAX = 8242;
    int ERROR_DS_IS_LEAF = 8243;
    int ERROR_DS_ALIAS_DEREF_PROBLEM = 8244;
    int ERROR_DS_UNWILLING_TO_PERFORM = 8245;
    int ERROR_DS_LOOP_DETECT = 8246;
    int ERROR_DS_NAMING_VIOLATION = 8247;
    int ERROR_DS_OBJECT_RESULTS_TOO_LARGE = 8248;
    int ERROR_DS_AFFECTS_MULTIPLE_DSAS = 8249;
    int ERROR_DS_SERVER_DOWN = 8250;
    int ERROR_DS_LOCAL_ERROR = 8251;
    int ERROR_DS_ENCODING_ERROR = 8252;
    int ERROR_DS_DECODING_ERROR = 8253;
    int ERROR_DS_FILTER_UNKNOWN = 8254;
    int ERROR_DS_PARAM_ERROR = 8255;
    int ERROR_DS_NOT_SUPPORTED = 8256;
    int ERROR_DS_NO_RESULTS_RETURNED = 8257;
    int ERROR_DS_CONTROL_NOT_FOUND = 8258;
    int ERROR_DS_CLIENT_LOOP = 8259;
    int ERROR_DS_REFERRAL_LIMIT_EXCEEDED = 8260;
    int ERROR_DS_SORT_CONTROL_MISSING = 8261;
    int ERROR_DS_OFFSET_RANGE_ERROR = 8262;
    int ERROR_DS_ROOT_MUST_BE_NC = 8301;
    int ERROR_DS_ADD_REPLICA_INHIBITED = 8302;
    int ERROR_DS_ATT_NOT_DEF_IN_SCHEMA = 8303;
    int ERROR_DS_MAX_OBJ_SIZE_EXCEEDED = 8304;
    int ERROR_DS_OBJ_STRING_NAME_EXISTS = 8305;
    int ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA = 8306;
    int ERROR_DS_RDN_DOESNT_MATCH_SCHEMA = 8307;
    int ERROR_DS_NO_REQUESTED_ATTS_FOUND = 8308;
    int ERROR_DS_USER_BUFFER_TO_SMALL = 8309;
    int ERROR_DS_ATT_IS_NOT_ON_OBJ = 8310;
    int ERROR_DS_ILLEGAL_MOD_OPERATION = 8311;
    int ERROR_DS_OBJ_TOO_LARGE = 8312;
    int ERROR_DS_BAD_INSTANCE_TYPE = 8313;
    int ERROR_DS_MASTERDSA_REQUIRED = 8314;
    int ERROR_DS_OBJECT_CLASS_REQUIRED = 8315;
    int ERROR_DS_MISSING_REQUIRED_ATT = 8316;
    int ERROR_DS_ATT_NOT_DEF_FOR_CLASS = 8317;
    int ERROR_DS_ATT_ALREADY_EXISTS = 8318;
    int ERROR_DS_CANT_ADD_ATT_VALUES = 8320;
    int ERROR_DS_SINGLE_VALUE_CONSTRAINT = 8321;
    int ERROR_DS_RANGE_CONSTRAINT = 8322;
    int ERROR_DS_ATT_VAL_ALREADY_EXISTS = 8323;
    int ERROR_DS_CANT_REM_MISSING_ATT = 8324;
    int ERROR_DS_CANT_REM_MISSING_ATT_VAL = 8325;
    int ERROR_DS_ROOT_CANT_BE_SUBREF = 8326;
    int ERROR_DS_NO_CHAINING = 8327;
    int ERROR_DS_NO_CHAINED_EVAL = 8328;
    int ERROR_DS_NO_PARENT_OBJECT = 8329;
    int ERROR_DS_PARENT_IS_AN_ALIAS = 8330;
    int ERROR_DS_CANT_MIX_MASTER_AND_REPS = 8331;
    int ERROR_DS_CHILDREN_EXIST = 8332;
    int ERROR_DS_OBJ_NOT_FOUND = 8333;
    int ERROR_DS_ALIASED_OBJ_MISSING = 8334;
    int ERROR_DS_BAD_NAME_SYNTAX = 8335;
    int ERROR_DS_ALIAS_POINTS_TO_ALIAS = 8336;
    int ERROR_DS_CANT_DEREF_ALIAS = 8337;
    int ERROR_DS_OUT_OF_SCOPE = 8338;
    int ERROR_DS_OBJECT_BEING_REMOVED = 8339;
    int ERROR_DS_CANT_DELETE_DSA_OBJ = 8340;
    int ERROR_DS_GENERIC_ERROR = 8341;
    int ERROR_DS_DSA_MUST_BE_INT_MASTER = 8342;
    int ERROR_DS_CLASS_NOT_DSA = 8343;
    int ERROR_DS_INSUFF_ACCESS_RIGHTS = 8344;
    int ERROR_DS_ILLEGAL_SUPERIOR = 8345;
    int ERROR_DS_ATTRIBUTE_OWNED_BY_SAM = 8346;
    int ERROR_DS_NAME_TOO_MANY_PARTS = 8347;
    int ERROR_DS_NAME_TOO_LONG = 8348;
    int ERROR_DS_NAME_VALUE_TOO_LONG = 8349;
    int ERROR_DS_NAME_UNPARSEABLE = 8350;
    int ERROR_DS_NAME_TYPE_UNKNOWN = 8351;
    int ERROR_DS_NOT_AN_OBJECT = 8352;
    int ERROR_DS_SEC_DESC_TOO_SHORT = 8353;
    int ERROR_DS_SEC_DESC_INVALID = 8354;
    int ERROR_DS_NO_DELETED_NAME = 8355;
    int ERROR_DS_SUBREF_MUST_HAVE_PARENT = 8356;
    int ERROR_DS_NCNAME_MUST_BE_NC = 8357;
    int ERROR_DS_CANT_ADD_SYSTEM_ONLY = 8358;
    int ERROR_DS_CLASS_MUST_BE_CONCRETE = 8359;
    int ERROR_DS_INVALID_DMD = 8360;
    int ERROR_DS_OBJ_GUID_EXISTS = 8361;
    int ERROR_DS_NOT_ON_BACKLINK = 8362;
    int ERROR_DS_NO_CROSSREF_FOR_NC = 8363;
    int ERROR_DS_SHUTTING_DOWN = 8364;
    int ERROR_DS_UNKNOWN_OPERATION = 8365;
    int ERROR_DS_INVALID_ROLE_OWNER = 8366;
    int ERROR_DS_COULDNT_CONTACT_FSMO = 8367;
    int ERROR_DS_CROSS_NC_DN_RENAME = 8368;
    int ERROR_DS_CANT_MOD_SYSTEM_ONLY = 8369;
    int ERROR_DS_REPLICATOR_ONLY = 8370;
    int ERROR_DS_OBJ_CLASS_NOT_DEFINED = 8371;
    int ERROR_DS_OBJ_CLASS_NOT_SUBCLASS = 8372;
    int ERROR_DS_NAME_REFERENCE_INVALID = 8373;
    int ERROR_DS_CROSS_REF_EXISTS = 8374;
    int ERROR_DS_CANT_DEL_MASTER_CROSSREF = 8375;
    int ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD = 8376;
    int ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX = 8377;
    int ERROR_DS_DUP_RDN = 8378;
    int ERROR_DS_DUP_OID = 8379;
    int ERROR_DS_DUP_MAPI_ID = 8380;
    int ERROR_DS_DUP_SCHEMA_ID_GUID = 8381;
    int ERROR_DS_DUP_LDAP_DISPLAY_NAME = 8382;
    int ERROR_DS_SEMANTIC_ATT_TEST = 8383;
    int ERROR_DS_SYNTAX_MISMATCH = 8384;
    int ERROR_DS_EXISTS_IN_MUST_HAVE = 8385;
    int ERROR_DS_EXISTS_IN_MAY_HAVE = 8386;
    int ERROR_DS_NONEXISTENT_MAY_HAVE = 8387;
    int ERROR_DS_NONEXISTENT_MUST_HAVE = 8388;
    int ERROR_DS_AUX_CLS_TEST_FAIL = 8389;
    int ERROR_DS_NONEXISTENT_POSS_SUP = 8390;
    int ERROR_DS_SUB_CLS_TEST_FAIL = 8391;
    int ERROR_DS_BAD_RDN_ATT_ID_SYNTAX = 8392;
    int ERROR_DS_EXISTS_IN_AUX_CLS = 8393;
    int ERROR_DS_EXISTS_IN_SUB_CLS = 8394;
    int ERROR_DS_EXISTS_IN_POSS_SUP = 8395;
    int ERROR_DS_RECALCSCHEMA_FAILED = 8396;
    int ERROR_DS_TREE_DELETE_NOT_FINISHED = 8397;
    int ERROR_DS_CANT_DELETE = 8398;
    int ERROR_DS_ATT_SCHEMA_REQ_ID = 8399;
    int ERROR_DS_BAD_ATT_SCHEMA_SYNTAX = 8400;
    int ERROR_DS_CANT_CACHE_ATT = 8401;
    int ERROR_DS_CANT_CACHE_CLASS = 8402;
    int ERROR_DS_CANT_REMOVE_ATT_CACHE = 8403;
    int ERROR_DS_CANT_REMOVE_CLASS_CACHE = 8404;
    int ERROR_DS_CANT_RETRIEVE_DN = 8405;
    int ERROR_DS_MISSING_SUPREF = 8406;
    int ERROR_DS_CANT_RETRIEVE_INSTANCE = 8407;
    int ERROR_DS_CODE_INCONSISTENCY = 8408;
    int ERROR_DS_DATABASE_ERROR = 8409;
    int ERROR_DS_GOVERNSID_MISSING = 8410;
    int ERROR_DS_MISSING_EXPECTED_ATT = 8411;
    int ERROR_DS_NCNAME_MISSING_CR_REF = 8412;
    int ERROR_DS_SECURITY_CHECKING_ERROR = 8413;
    int ERROR_DS_SCHEMA_NOT_LOADED = 8414;
    int ERROR_DS_SCHEMA_ALLOC_FAILED = 8415;
    int ERROR_DS_ATT_SCHEMA_REQ_SYNTAX = 8416;
    int ERROR_DS_GCVERIFY_ERROR = 8417;
    int ERROR_DS_DRA_SCHEMA_MISMATCH = 8418;
    int ERROR_DS_CANT_FIND_DSA_OBJ = 8419;
    int ERROR_DS_CANT_FIND_EXPECTED_NC = 8420;
    int ERROR_DS_CANT_FIND_NC_IN_CACHE = 8421;
    int ERROR_DS_CANT_RETRIEVE_CHILD = 8422;
    int ERROR_DS_SECURITY_ILLEGAL_MODIFY = 8423;
    int ERROR_DS_CANT_REPLACE_HIDDEN_REC = 8424;
    int ERROR_DS_BAD_HIERARCHY_FILE = 8425;
    int ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED = 8426;
    int ERROR_DS_CONFIG_PARAM_MISSING = 8427;
    int ERROR_DS_COUNTING_AB_INDICES_FAILED = 8428;
    int ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED = 8429;
    int ERROR_DS_INTERNAL_FAILURE = 8430;
    int ERROR_DS_UNKNOWN_ERROR = 8431;
    int ERROR_DS_ROOT_REQUIRES_CLASS_TOP = 8432;
    int ERROR_DS_REFUSING_FSMO_ROLES = 8433;
    int ERROR_DS_MISSING_FSMO_SETTINGS = 8434;
    int ERROR_DS_UNABLE_TO_SURRENDER_ROLES = 8435;
    int ERROR_DS_DRA_GENERIC = 8436;
    int ERROR_DS_DRA_INVALID_PARAMETER = 8437;
    int ERROR_DS_DRA_BUSY = 8438;
    int ERROR_DS_DRA_BAD_DN = 8439;
    int ERROR_DS_DRA_BAD_NC = 8440;
    int ERROR_DS_DRA_DN_EXISTS = 8441;
    int ERROR_DS_DRA_INTERNAL_ERROR = 8442;
    int ERROR_DS_DRA_INCONSISTENT_DIT = 8443;
    int ERROR_DS_DRA_CONNECTION_FAILED = 8444;
    int ERROR_DS_DRA_BAD_INSTANCE_TYPE = 8445;
    int ERROR_DS_DRA_OUT_OF_MEM = 8446;
    int ERROR_DS_DRA_MAIL_PROBLEM = 8447;
    int ERROR_DS_DRA_REF_ALREADY_EXISTS = 8448;
    int ERROR_DS_DRA_REF_NOT_FOUND = 8449;
    int ERROR_DS_DRA_OBJ_IS_REP_SOURCE = 8450;
    int ERROR_DS_DRA_DB_ERROR = 8451;
    int ERROR_DS_DRA_NO_REPLICA = 8452;
    int ERROR_DS_DRA_ACCESS_DENIED = 8453;
    int ERROR_DS_DRA_NOT_SUPPORTED = 8454;
    int ERROR_DS_DRA_RPC_CANCELLED = 8455;
    int ERROR_DS_DRA_SOURCE_DISABLED = 8456;
    int ERROR_DS_DRA_SINK_DISABLED = 8457;
    int ERROR_DS_DRA_NAME_COLLISION = 8458;
    int ERROR_DS_DRA_SOURCE_REINSTALLED = 8459;
    int ERROR_DS_DRA_MISSING_PARENT = 8460;
    int ERROR_DS_DRA_PREEMPTED = 8461;
    int ERROR_DS_DRA_ABANDON_SYNC = 8462;
    int ERROR_DS_DRA_SHUTDOWN = 8463;
    int ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET = 8464;
    int ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA = 8465;
    int ERROR_DS_DRA_EXTN_CONNECTION_FAILED = 8466;
    int ERROR_DS_INSTALL_SCHEMA_MISMATCH = 8467;
    int ERROR_DS_DUP_LINK_ID = 8468;
    int ERROR_DS_NAME_ERROR_RESOLVING = 8469;
    int ERROR_DS_NAME_ERROR_NOT_FOUND = 8470;
    int ERROR_DS_NAME_ERROR_NOT_UNIQUE = 8471;
    int ERROR_DS_NAME_ERROR_NO_MAPPING = 8472;
    int ERROR_DS_NAME_ERROR_DOMAIN_ONLY = 8473;
    int ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING = 8474;
    int ERROR_DS_CONSTRUCTED_ATT_MOD = 8475;
    int ERROR_DS_WRONG_OM_OBJ_CLASS = 8476;
    int ERROR_DS_DRA_REPL_PENDING = 8477;
    int ERROR_DS_DS_REQUIRED = 8478;
    int ERROR_DS_INVALID_LDAP_DISPLAY_NAME = 8479;
    int ERROR_DS_NON_BASE_SEARCH = 8480;
    int ERROR_DS_CANT_RETRIEVE_ATTS = 8481;
    int ERROR_DS_BACKLINK_WITHOUT_LINK = 8482;
    int ERROR_DS_EPOCH_MISMATCH = 8483;
    int ERROR_DS_SRC_NAME_MISMATCH = 8484;
    int ERROR_DS_SRC_AND_DST_NC_IDENTICAL = 8485;
    int ERROR_DS_DST_NC_MISMATCH = 8486;
    int ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC = 8487;
    int ERROR_DS_SRC_GUID_MISMATCH = 8488;
    int ERROR_DS_CANT_MOVE_DELETED_OBJECT = 8489;
    int ERROR_DS_PDC_OPERATION_IN_PROGRESS = 8490;
    int ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD = 8491;
    int ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION = 8492;
    int ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS = 8493;
    int ERROR_DS_NC_MUST_HAVE_NC_PARENT = 8494;
    int ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE = 8495;
    int ERROR_DS_DST_DOMAIN_NOT_NATIVE = 8496;
    int ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER = 8497;
    int ERROR_DS_CANT_MOVE_ACCOUNT_GROUP = 8498;
    int ERROR_DS_CANT_MOVE_RESOURCE_GROUP = 8499;
    int ERROR_DS_INVALID_SEARCH_FLAG = 8500;
    int ERROR_DS_NO_TREE_DELETE_ABOVE_NC = 8501;
    int ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE = 8502;
    int ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE = 8503;
    int ERROR_DS_SAM_INIT_FAILURE = 8504;
    int ERROR_DS_SENSITIVE_GROUP_VIOLATION = 8505;
    int ERROR_DS_CANT_MOD_PRIMARYGROUPID = 8506;
    int ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD = 8507;
    int ERROR_DS_NONSAFE_SCHEMA_CHANGE = 8508;
    int ERROR_DS_SCHEMA_UPDATE_DISALLOWED = 8509;
    int ERROR_DS_CANT_CREATE_UNDER_SCHEMA = 8510;
    int ERROR_DS_INSTALL_NO_SRC_SCH_VERSION = 8511;
    int ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE = 8512;
    int ERROR_DS_INVALID_GROUP_TYPE = 8513;
    int ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN = 8514;
    int ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN = 8515;
    int ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER = 8516;
    int ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER = 8517;
    int ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER = 8518;
    int ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER = 8519;
    int ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER = 8520;
    int ERROR_DS_HAVE_PRIMARY_MEMBERS = 8521;
    int ERROR_DS_STRING_SD_CONVERSION_FAILED = 8522;
    int ERROR_DS_NAMING_MASTER_GC = 8523;
    int ERROR_DS_DNS_LOOKUP_FAILURE = 8524;
    int ERROR_DS_COULDNT_UPDATE_SPNS = 8525;
    int ERROR_DS_CANT_RETRIEVE_SD = 8526;
    int ERROR_DS_KEY_NOT_UNIQUE = 8527;
    int ERROR_DS_WRONG_LINKED_ATT_SYNTAX = 8528;
    int ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD = 8529;
    int ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY = 8530;
    int ERROR_DS_CANT_START = 8531;
    int ERROR_DS_INIT_FAILURE = 8532;
    int ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION = 8533;
    int ERROR_DS_SOURCE_DOMAIN_IN_FOREST = 8534;
    int ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST = 8535;
    int ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED = 8536;
    int ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN = 8537;
    int ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER = 8538;
    int ERROR_DS_SRC_SID_EXISTS_IN_FOREST = 8539;
    int ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH = 8540;
    int ERROR_SAM_INIT_FAILURE = 8541;
    int ERROR_DS_DRA_SCHEMA_INFO_SHIP = 8542;
    int ERROR_DS_DRA_EARLIER_SCHEMA_CONFLICT = 8544;
    int ERROR_DS_DRA_OBJ_NC_MISMATCH = 8545;
    int ERROR_DS_NC_STILL_HAS_DSAS = 8546;
    int ERROR_DS_GC_REQUIRED = 8547;
    int ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY = 8548;
    int ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS = 8549;
    int ERROR_DS_CANT_ADD_TO_GC = 8550;
    int ERROR_DS_NO_CHECKPOINT_WITH_PDC = 8551;
    int ERROR_DS_SOURCE_AUDITING_NOT_ENABLED = 8552;
    int ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC = 8553;
    int ERROR_DS_INVALID_NAME_FOR_SPN = 8554;
    int ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS = 8555;
    int ERROR_DS_UNICODEPWD_NOT_IN_QUOTES = 8556;
    int ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED = 8557;
    int ERROR_DS_MUST_BE_RUN_ON_DST_DC = 8558;
    int ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER = 8559;
    int ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ = 8560;
    int ERROR_DS_INIT_FAILURE_CONSOLE = 8561;
    int ERROR_DS_SAM_INIT_FAILURE_CONSOLE = 8562;
    int ERROR_DS_FOREST_VERSION_TOO_HIGH = 8563;
    int ERROR_DS_DOMAIN_VERSION_TOO_HIGH = 8564;
    int ERROR_DS_FOREST_VERSION_TOO_LOW = 8565;
    int ERROR_DS_DOMAIN_VERSION_TOO_LOW = 8566;
    int ERROR_DS_INCOMPATIBLE_VERSION = 8567;
    int ERROR_DS_LOW_DSA_VERSION = 8568;
    int ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN = 8569;
    int ERROR_DS_NOT_SUPPORTED_SORT_ORDER = 8570;
    int ERROR_DS_NAME_NOT_UNIQUE = 8571;
    int ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4 = 8572;
    int ERROR_DS_OUT_OF_VERSION_STORE = 8573;
    int ERROR_DS_INCOMPATIBLE_CONTROLS_USED = 8574;
    int ERROR_DS_NO_REF_DOMAIN = 8575;
    int ERROR_DS_RESERVED_LINK_ID = 8576;
    int ERROR_DS_LINK_ID_NOT_AVAILABLE = 8577;
    int ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER = 8578;
    int ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE = 8579;
    int ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC = 8580;
    int ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG = 8581;
    int ERROR_DS_MODIFYDN_WRONG_GRANDPARENT = 8582;
    int ERROR_DS_NAME_ERROR_TRUST_REFERRAL = 8583;
    int ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER = 8584;
    int ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD = 8585;
    int ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2 = 8586;
    int ERROR_DS_THREAD_LIMIT_EXCEEDED = 8587;
    int ERROR_DS_NOT_CLOSEST = 8588;
    int ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF = 8589;
    int ERROR_DS_SINGLE_USER_MODE_FAILED = 8590;
    int ERROR_DS_NTDSCRIPT_SYNTAX_ERROR = 8591;
    int ERROR_DS_NTDSCRIPT_PROCESS_ERROR = 8592;
    int ERROR_DS_DIFFERENT_REPL_EPOCHS = 8593;
    int ERROR_DS_DRS_EXTENSIONS_CHANGED = 8594;
    int ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR = 8595;
    int ERROR_DS_NO_MSDS_INTID = 8596;
    int ERROR_DS_DUP_MSDS_INTID = 8597;
    int ERROR_DS_EXISTS_IN_RDNATTID = 8598;
    int ERROR_DS_AUTHORIZATION_FAILED = 8599;
    int ERROR_DS_INVALID_SCRIPT = 8600;
    int ERROR_DS_REMOTE_CROSSREF_OP_FAILED = 8601;
    int ERROR_DS_CROSS_REF_BUSY = 8602;
    int ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN = 8603;
    int ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC = 8604;
    int ERROR_DS_DUPLICATE_ID_FOUND = 8605;
    int ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT = 8606;
    int ERROR_DS_GROUP_CONVERSION_ERROR = 8607;
    int ERROR_DS_CANT_MOVE_APP_BASIC_GROUP = 8608;
    int ERROR_DS_CANT_MOVE_APP_QUERY_GROUP = 8609;
    int ERROR_DS_ROLE_NOT_VERIFIED = 8610;
    int ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL = 8611;
    int ERROR_DS_DOMAIN_RENAME_IN_PROGRESS = 8612;
    int ERROR_DS_EXISTING_AD_CHILD_NC = 8613;
    int ERROR_DS_REPL_LIFETIME_EXCEEDED = 8614;
    int ERROR_DS_DISALLOWED_IN_SYSTEM_CONTAINER = 8615;
    int ERROR_DS_LDAP_SEND_QUEUE_FULL = 8616;
    int ERROR_DS_DRA_OUT_SCHEDULE_WINDOW = 8617;
    int ERROR_DS_POLICY_NOT_KNOWN = 8618;
    int ERROR_NO_SITE_SETTINGS_OBJECT = 8619;
    int ERROR_NO_SECRETS = 8620;
    int ERROR_NO_WRITABLE_DC_FOUND = 8621;
    int ERROR_DS_NO_SERVER_OBJECT = 8622;
    int ERROR_DS_NO_NTDSA_OBJECT = 8623;
    int ERROR_DS_NON_ASQ_SEARCH = 8624;
    int ERROR_DS_AUDIT_FAILURE = 8625;
    int ERROR_DS_INVALID_SEARCH_FLAG_SUBTREE = 8626;
    int ERROR_DS_INVALID_SEARCH_FLAG_TUPLE = 8627;
    int ERROR_DS_HIERARCHY_TABLE_TOO_DEEP = 8628;
    int DNS_ERROR_RESPONSE_CODES_BASE = 9000;
    int DNS_ERROR_RCODE_NO_ERROR = NO_ERROR;
    int DNS_ERROR_MASK = 0x00002328;
    int DNS_ERROR_RCODE_FORMAT_ERROR = 9001;
    int DNS_ERROR_RCODE_SERVER_FAILURE = 9002;
    int DNS_ERROR_RCODE_NAME_ERROR = 9003;
    int DNS_ERROR_RCODE_NOT_IMPLEMENTED = 9004;
    int DNS_ERROR_RCODE_REFUSED = 9005;
    int DNS_ERROR_RCODE_YXDOMAIN = 9006;
    int DNS_ERROR_RCODE_YXRRSET = 9007;
    int DNS_ERROR_RCODE_NXRRSET = 9008;
    int DNS_ERROR_RCODE_NOTAUTH = 9009;
    int DNS_ERROR_RCODE_NOTZONE = 9010;
    int DNS_ERROR_RCODE_BADSIG = 9016;
    int DNS_ERROR_RCODE_BADKEY = 9017;
    int DNS_ERROR_RCODE_BADTIME = 9018;
    int DNS_ERROR_RCODE_LAST = DNS_ERROR_RCODE_BADTIME;
    int DNS_ERROR_PACKET_FMT_BASE = 9500;
    int DNS_INFO_NO_RECORDS = 9501;
    int DNS_ERROR_BAD_PACKET = 9502;
    int DNS_ERROR_NO_PACKET = 9503;
    int DNS_ERROR_RCODE = 9504;
    int DNS_ERROR_UNSECURE_PACKET = 9505;
    int DNS_STATUS_PACKET_UNSECURE = DNS_ERROR_UNSECURE_PACKET;
    int DNS_ERROR_NO_MEMORY = ERROR_OUTOFMEMORY;
    int DNS_ERROR_INVALID_NAME = ERROR_INVALID_NAME;
    int DNS_ERROR_INVALID_DATA = ERROR_INVALID_DATA;
    int DNS_ERROR_GENERAL_API_BASE = 9550;
    int DNS_ERROR_INVALID_TYPE = 9551;
    int DNS_ERROR_INVALID_IP_ADDRESS = 9552;
    int DNS_ERROR_INVALID_PROPERTY = 9553;
    int DNS_ERROR_TRY_AGAIN_LATER = 9554;
    int DNS_ERROR_NOT_UNIQUE = 9555;
    int DNS_ERROR_NON_RFC_NAME = 9556;
    int DNS_STATUS_FQDN = 9557;
    int DNS_STATUS_DOTTED_NAME = 9558;
    int DNS_STATUS_SINGLE_PART_NAME = 9559;
    int DNS_ERROR_INVALID_NAME_CHAR = 9560;
    int DNS_ERROR_NUMERIC_NAME = 9561;
    int DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER = 9562;
    int DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION = 9563;
    int DNS_ERROR_CANNOT_FIND_ROOT_HINTS = 9564;
    int DNS_ERROR_INCONSISTENT_ROOT_HINTS = 9565;
    int DNS_ERROR_DWORD_VALUE_TOO_SMALL = 9566;
    int DNS_ERROR_DWORD_VALUE_TOO_LARGE = 9567;
    int DNS_ERROR_BACKGROUND_LOADING = 9568;
    int DNS_ERROR_NOT_ALLOWED_ON_RODC = 9569;
    int DNS_ERROR_NOT_ALLOWED_UNDER_DNAME = 9570;
    int DNS_ERROR_ZONE_BASE = 9600;
    int DNS_ERROR_ZONE_DOES_NOT_EXIST = 9601;
    int DNS_ERROR_NO_ZONE_INFO = 9602;
    int DNS_ERROR_INVALID_ZONE_OPERATION = 9603;
    int DNS_ERROR_ZONE_CONFIGURATION_ERROR = 9604;
    int DNS_ERROR_ZONE_HAS_NO_SOA_RECORD = 9605;
    int DNS_ERROR_ZONE_HAS_NO_NS_RECORDS = 9606;
    int DNS_ERROR_ZONE_LOCKED = 9607;
    int DNS_ERROR_ZONE_CREATION_FAILED = 9608;
    int DNS_ERROR_ZONE_ALREADY_EXISTS = 9609;
    int DNS_ERROR_AUTOZONE_ALREADY_EXISTS = 9610;
    int DNS_ERROR_INVALID_ZONE_TYPE = 9611;
    int DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP = 9612;
    int DNS_ERROR_ZONE_NOT_SECONDARY = 9613;
    int DNS_ERROR_NEED_SECONDARY_ADDRESSES = 9614;
    int DNS_ERROR_WINS_INIT_FAILED = 9615;
    int DNS_ERROR_NEED_WINS_SERVERS = 9616;
    int DNS_ERROR_NBSTAT_INIT_FAILED = 9617;
    int DNS_ERROR_SOA_DELETE_INVALID = 9618;
    int DNS_ERROR_FORWARDER_ALREADY_EXISTS = 9619;
    int DNS_ERROR_ZONE_REQUIRES_MASTER_IP = 9620;
    int DNS_ERROR_ZONE_IS_SHUTDOWN = 9621;
    int DNS_ERROR_DATAFILE_BASE = 9650;
    int DNS_ERROR_PRIMARY_REQUIRES_DATAFILE = 9651;
    int DNS_ERROR_INVALID_DATAFILE_NAME = 9652;
    int DNS_ERROR_DATAFILE_OPEN_FAILURE = 9653;
    int DNS_ERROR_FILE_WRITEBACK_FAILED = 9654;
    int DNS_ERROR_DATAFILE_PARSING = 9655;
    int DNS_ERROR_DATABASE_BASE = 9700;
    int DNS_ERROR_RECORD_DOES_NOT_EXIST = 9701;
    int DNS_ERROR_RECORD_FORMAT = 9702;
    int DNS_ERROR_NODE_CREATION_FAILED = 9703;
    int DNS_ERROR_UNKNOWN_RECORD_TYPE = 9704;
    int DNS_ERROR_RECORD_TIMED_OUT = 9705;
    int DNS_ERROR_NAME_NOT_IN_ZONE = 9706;
    int DNS_ERROR_CNAME_LOOP = 9707;
    int DNS_ERROR_NODE_IS_CNAME = 9708;
    int DNS_ERROR_CNAME_COLLISION = 9709;
    int DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT = 9710;
    int DNS_ERROR_RECORD_ALREADY_EXISTS = 9711;
    int DNS_ERROR_SECONDARY_DATA = 9712;
    int DNS_ERROR_NO_CREATE_CACHE_DATA = 9713;
    int DNS_ERROR_NAME_DOES_NOT_EXIST = 9714;
    int DNS_WARNING_PTR_CREATE_FAILED = 9715;
    int DNS_WARNING_DOMAIN_UNDELETED = 9716;
    int DNS_ERROR_DS_UNAVAILABLE = 9717;
    int DNS_ERROR_DS_ZONE_ALREADY_EXISTS = 9718;
    int DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE = 9719;
    int DNS_ERROR_NODE_IS_DNAME = 9720;
    int DNS_ERROR_DNAME_COLLISION = 9721;
    int DNS_ERROR_ALIAS_LOOP = 9722;
    int DNS_ERROR_OPERATION_BASE = 9750;
    int DNS_INFO_AXFR_COMPLETE = 9751;
    int DNS_ERROR_AXFR = 9752;
    int DNS_INFO_ADDED_LOCAL_WINS = 9753;
    int DNS_ERROR_SECURE_BASE = 9800;
    int DNS_STATUS_CONTINUE_NEEDED = 9801;
    int DNS_ERROR_SETUP_BASE = 9850;
    int DNS_ERROR_NO_TCPIP = 9851;
    int DNS_ERROR_NO_DNS_SERVERS = 9852;
    int DNS_ERROR_DP_BASE = 9900;
    int DNS_ERROR_DP_DOES_NOT_EXIST = 9901;
    int DNS_ERROR_DP_ALREADY_EXISTS = 9902;
    int DNS_ERROR_DP_NOT_ENLISTED = 9903;
    int DNS_ERROR_DP_ALREADY_ENLISTED = 9904;
    int DNS_ERROR_DP_NOT_AVAILABLE = 9905;
    int DNS_ERROR_DP_FSMO_ERROR = 9906;
    int WSABASEERR = 10000;
    int WSAEINTR = 10004;
    int WSAEBADF = 10009;
    int WSAEACCES = 10013;
    int WSAEFAULT = 10014;
    int WSAEINVAL = 10022;
    int WSAEMFILE = 10024;
    int WSAEWOULDBLOCK = 10035;
    int WSAEINPROGRESS = 10036;
    int WSAEALREADY = 10037;
    int WSAENOTSOCK = 10038;
    int WSAEDESTADDRREQ = 10039;
    int WSAEMSGSIZE = 10040;
    int WSAEPROTOTYPE = 10041;
    int WSAENOPROTOOPT = 10042;
    int WSAEPROTONOSUPPORT = 10043;
    int WSAESOCKTNOSUPPORT = 10044;
    int WSAEOPNOTSUPP = 10045;
    int WSAEPFNOSUPPORT = 10046;
    int WSAEAFNOSUPPORT = 10047;
    int WSAEADDRINUSE = 10048;
    int WSAEADDRNOTAVAIL = 10049;
    int WSAENETDOWN = 10050;
    int WSAENETUNREACH = 10051;
    int WSAENETRESET = 10052;
    int WSAECONNABORTED = 10053;
    int WSAECONNRESET = 10054;
    int WSAENOBUFS = 10055;
    int WSAEISCONN = 10056;
    int WSAENOTCONN = 10057;
    int WSAESHUTDOWN = 10058;
    int WSAETOOMANYREFS = 10059;
    int WSAETIMEDOUT = 10060;
    int WSAECONNREFUSED = 10061;
    int WSAELOOP = 10062;
    int WSAENAMETOOLONG = 10063;
    int WSAEHOSTDOWN = 10064;
    int WSAEHOSTUNREACH = 10065;
    int WSAENOTEMPTY = 10066;
    int WSAEPROCLIM = 10067;
    int WSAEUSERS = 10068;
    int WSAEDQUOT = 10069;
    int WSAESTALE = 10070;
    int WSAEREMOTE = 10071;
    int WSASYSNOTREADY = 10091;
    int WSAVERNOTSUPPORTED = 10092;
    int WSANOTINITIALISED = 10093;
    int WSAEDISCON = 10101;
    int WSAENOMORE = 10102;
    int WSAECANCELLED = 10103;
    int WSAEINVALIDPROCTABLE = 10104;
    int WSAEINVALIDPROVIDER = 10105;
    int WSAEPROVIDERFAILEDINIT = 10106;
    int WSASYSCALLFAILURE = 10107;
    int WSASERVICE_NOT_FOUND = 10108;
    int WSATYPE_NOT_FOUND = 10109;
    int WSA_E_NO_MORE = 10110;
    int WSA_E_CANCELLED = 10111;
    int WSAEREFUSED = 10112;
    int WSAHOST_NOT_FOUND = 11001;
    int WSATRY_AGAIN = 11002;
    int WSANO_RECOVERY = 11003;
    int WSANO_DATA = 11004;
    int WSA_QOS_RECEIVERS = 11005;
    int WSA_QOS_SENDERS = 11006;
    int WSA_QOS_NO_SENDERS = 11007;
    int WSA_QOS_NO_RECEIVERS = 11008;
    int WSA_QOS_REQUEST_CONFIRMED = 11009;
    int WSA_QOS_ADMISSION_FAILURE = 11010;
    int WSA_QOS_POLICY_FAILURE = 11011;
    int WSA_QOS_BAD_STYLE = 11012;
    int WSA_QOS_BAD_OBJECT = 11013;
    int WSA_QOS_TRAFFIC_CTRL_ERROR = 11014;
    int WSA_QOS_GENERIC_ERROR = 11015;
    int WSA_QOS_ESERVICETYPE = 11016;
    int WSA_QOS_EFLOWSPEC = 11017;
    int WSA_QOS_EPROVSPECBUF = 11018;
    int WSA_QOS_EFILTERSTYLE = 11019;
    int WSA_QOS_EFILTERTYPE = 11020;
    int WSA_QOS_EFILTERCOUNT = 11021;
    int WSA_QOS_EOBJLENGTH = 11022;
    int WSA_QOS_EFLOWCOUNT = 11023;
    int WSA_QOS_EUNKOWNPSOBJ = 11024;
    int WSA_QOS_EPOLICYOBJ = 11025;
    int WSA_QOS_EFLOWDESC = 11026;
    int WSA_QOS_EPSFLOWSPEC = 11027;
    int WSA_QOS_EPSFILTERSPEC = 11028;
    int WSA_QOS_ESDMODEOBJ = 11029;
    int WSA_QOS_ESHAPERATEOBJ = 11030;
    int WSA_QOS_RESERVED_PETYPE = 11031;
    int ERROR_IPSEC_QM_POLICY_EXISTS = 13000;
    int ERROR_IPSEC_QM_POLICY_NOT_FOUND = 13001;
    int ERROR_IPSEC_QM_POLICY_IN_USE = 13002;
    int ERROR_IPSEC_MM_POLICY_EXISTS = 13003;
    int ERROR_IPSEC_MM_POLICY_NOT_FOUND = 13004;
    int ERROR_IPSEC_MM_POLICY_IN_USE = 13005;
    int ERROR_IPSEC_MM_FILTER_EXISTS = 13006;
    int ERROR_IPSEC_MM_FILTER_NOT_FOUND = 13007;
    int ERROR_IPSEC_TRANSPORT_FILTER_EXISTS = 13008;
    int ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND = 13009;
    int ERROR_IPSEC_MM_AUTH_EXISTS = 13010;
    int ERROR_IPSEC_MM_AUTH_NOT_FOUND = 13011;
    int ERROR_IPSEC_MM_AUTH_IN_USE = 13012;
    int ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND = 13013;
    int ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND = 13014;
    int ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND = 13015;
    int ERROR_IPSEC_TUNNEL_FILTER_EXISTS = 13016;
    int ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND = 13017;
    int ERROR_IPSEC_MM_FILTER_PENDING_DELETION = 13018;
    int ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION = 13019;
    int ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION = 13020;
    int ERROR_IPSEC_MM_POLICY_PENDING_DELETION = 13021;
    int ERROR_IPSEC_MM_AUTH_PENDING_DELETION = 13022;
    int ERROR_IPSEC_QM_POLICY_PENDING_DELETION = 13023;
    int WARNING_IPSEC_MM_POLICY_PRUNED = 13024;
    int WARNING_IPSEC_QM_POLICY_PRUNED = 13025;
    int ERROR_IPSEC_IKE_NEG_STATUS_BEGIN = 13800;
    int ERROR_IPSEC_IKE_AUTH_FAIL = 13801;
    int ERROR_IPSEC_IKE_ATTRIB_FAIL = 13802;
    int ERROR_IPSEC_IKE_NEGOTIATION_PENDING = 13803;
    int ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR = 13804;
    int ERROR_IPSEC_IKE_TIMED_OUT = 13805;
    int ERROR_IPSEC_IKE_NO_CERT = 13806;
    int ERROR_IPSEC_IKE_SA_DELETED = 13807;
    int ERROR_IPSEC_IKE_SA_REAPED = 13808;
    int ERROR_IPSEC_IKE_MM_ACQUIRE_DROP = 13809;
    int ERROR_IPSEC_IKE_QM_ACQUIRE_DROP = 13810;
    int ERROR_IPSEC_IKE_QUEUE_DROP_MM = 13811;
    int ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM = 13812;
    int ERROR_IPSEC_IKE_DROP_NO_RESPONSE = 13813;
    int ERROR_IPSEC_IKE_MM_DELAY_DROP = 13814;
    int ERROR_IPSEC_IKE_QM_DELAY_DROP = 13815;
    int ERROR_IPSEC_IKE_ERROR = 13816;
    int ERROR_IPSEC_IKE_CRL_FAILED = 13817;
    int ERROR_IPSEC_IKE_INVALID_KEY_USAGE = 13818;
    int ERROR_IPSEC_IKE_INVALID_CERT_TYPE = 13819;
    int ERROR_IPSEC_IKE_NO_PRIVATE_KEY = 13820;
    int ERROR_IPSEC_IKE_DH_FAIL = 13822;
    int ERROR_IPSEC_IKE_INVALID_HEADER = 13824;
    int ERROR_IPSEC_IKE_NO_POLICY = 13825;
    int ERROR_IPSEC_IKE_INVALID_SIGNATURE = 13826;
    int ERROR_IPSEC_IKE_KERBEROS_ERROR = 13827;
    int ERROR_IPSEC_IKE_NO_PUBLIC_KEY = 13828;
    int ERROR_IPSEC_IKE_PROCESS_ERR = 13829;
    int ERROR_IPSEC_IKE_PROCESS_ERR_SA = 13830;
    int ERROR_IPSEC_IKE_PROCESS_ERR_PROP = 13831;
    int ERROR_IPSEC_IKE_PROCESS_ERR_TRANS = 13832;
    int ERROR_IPSEC_IKE_PROCESS_ERR_KE = 13833;
    int ERROR_IPSEC_IKE_PROCESS_ERR_ID = 13834;
    int ERROR_IPSEC_IKE_PROCESS_ERR_CERT = 13835;
    int ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ = 13836;
    int ERROR_IPSEC_IKE_PROCESS_ERR_HASH = 13837;
    int ERROR_IPSEC_IKE_PROCESS_ERR_SIG = 13838;
    int ERROR_IPSEC_IKE_PROCESS_ERR_NONCE = 13839;
    int ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY = 13840;
    int ERROR_IPSEC_IKE_PROCESS_ERR_DELETE = 13841;
    int ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR = 13842;
    int ERROR_IPSEC_IKE_INVALID_PAYLOAD = 13843;
    int ERROR_IPSEC_IKE_LOAD_SOFT_SA = 13844;
    int ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN = 13845;
    int ERROR_IPSEC_IKE_INVALID_COOKIE = 13846;
    int ERROR_IPSEC_IKE_NO_PEER_CERT = 13847;
    int ERROR_IPSEC_IKE_PEER_CRL_FAILED = 13848;
    int ERROR_IPSEC_IKE_POLICY_CHANGE = 13849;
    int ERROR_IPSEC_IKE_NO_MM_POLICY = 13850;
    int ERROR_IPSEC_IKE_NOTCBPRIV = 13851;
    int ERROR_IPSEC_IKE_SECLOADFAIL = 13852;
    int ERROR_IPSEC_IKE_FAILSSPINIT = 13853;
    int ERROR_IPSEC_IKE_FAILQUERYSSP = 13854;
    int ERROR_IPSEC_IKE_SRVACQFAIL = 13855;
    int ERROR_IPSEC_IKE_SRVQUERYCRED = 13856;
    int ERROR_IPSEC_IKE_GETSPIFAIL = 13857;
    int ERROR_IPSEC_IKE_INVALID_FILTER = 13858;
    int ERROR_IPSEC_IKE_OUT_OF_MEMORY = 13859;
    int ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED = 13860;
    int ERROR_IPSEC_IKE_INVALID_POLICY = 13861;
    int ERROR_IPSEC_IKE_UNKNOWN_DOI = 13862;
    int ERROR_IPSEC_IKE_INVALID_SITUATION = 13863;
    int ERROR_IPSEC_IKE_DH_FAILURE = 13864;
    int ERROR_IPSEC_IKE_INVALID_GROUP = 13865;
    int ERROR_IPSEC_IKE_ENCRYPT = 13866;
    int ERROR_IPSEC_IKE_DECRYPT = 13867;
    int ERROR_IPSEC_IKE_POLICY_MATCH = 13868;
    int ERROR_IPSEC_IKE_UNSUPPORTED_ID = 13869;
    int ERROR_IPSEC_IKE_INVALID_HASH = 13870;
    int ERROR_IPSEC_IKE_INVALID_HASH_ALG = 13871;
    int ERROR_IPSEC_IKE_INVALID_HASH_SIZE = 13872;
    int ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG = 13873;
    int ERROR_IPSEC_IKE_INVALID_AUTH_ALG = 13874;
    int ERROR_IPSEC_IKE_INVALID_SIG = 13875;
    int ERROR_IPSEC_IKE_LOAD_FAILED = 13876;
    int ERROR_IPSEC_IKE_RPC_DELETE = 13877;
    int ERROR_IPSEC_IKE_BENIGN_REINIT = 13878;
    int ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY = 13879;
    int ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN = 13881;
    int ERROR_IPSEC_IKE_MM_LIMIT = 13882;
    int ERROR_IPSEC_IKE_NEGOTIATION_DISABLED = 13883;
    int ERROR_IPSEC_IKE_QM_LIMIT = 13884;
    int ERROR_IPSEC_IKE_MM_EXPIRED = 13885;
    int ERROR_IPSEC_IKE_PEER_MM_ASSUMED_INVALID = 13886;
    int ERROR_IPSEC_IKE_CERT_CHAIN_POLICY_MISMATCH = 13887;
    int ERROR_IPSEC_IKE_UNEXPECTED_MESSAGE_ID = 13888;
    int ERROR_IPSEC_IKE_INVALID_AUTH_PAYLOAD = 13889;
    int ERROR_IPSEC_IKE_DOS_COOKIE_SENT = 13890;
    int ERROR_IPSEC_IKE_SHUTTING_DOWN = 13891;
    int ERROR_IPSEC_IKE_CGA_AUTH_FAILED = 13892;
    int ERROR_IPSEC_IKE_PROCESS_ERR_NATOA = 13893;
    int ERROR_IPSEC_IKE_INVALID_MM_FOR_QM = 13894;
    int ERROR_IPSEC_IKE_QM_EXPIRED = 13895;
    int ERROR_IPSEC_IKE_TOO_MANY_FILTERS = 13896;
    int ERROR_IPSEC_IKE_NEG_STATUS_END = 13897;
    int ERROR_IPSEC_BAD_SPI = 13910;
    int ERROR_IPSEC_SA_LIFETIME_EXPIRED = 13911;
    int ERROR_IPSEC_WRONG_SA = 13912;
    int ERROR_IPSEC_REPLAY_CHECK_FAILED = 13913;
    int ERROR_IPSEC_INVALID_PACKET = 13914;
    int ERROR_IPSEC_INTEGRITY_CHECK_FAILED = 13915;
    int ERROR_IPSEC_CLEAR_TEXT_DROP = 13916;
    int ERROR_SXS_SECTION_NOT_FOUND = 14000;
    int ERROR_SXS_CANT_GEN_ACTCTX = 14001;
    int ERROR_SXS_INVALID_ACTCTXDATA_FORMAT = 14002;
    int ERROR_SXS_ASSEMBLY_NOT_FOUND = 14003;
    int ERROR_SXS_MANIFEST_FORMAT_ERROR = 14004;
    int ERROR_SXS_MANIFEST_PARSE_ERROR = 14005;
    int ERROR_SXS_ACTIVATION_CONTEXT_DISABLED = 14006;
    int ERROR_SXS_KEY_NOT_FOUND = 14007;
    int ERROR_SXS_VERSION_CONFLICT = 14008;
    int ERROR_SXS_WRONG_SECTION_TYPE = 14009;
    int ERROR_SXS_THREAD_QUERIES_DISABLED = 14010;
    int ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET = 14011;
    int ERROR_SXS_UNKNOWN_ENCODING_GROUP = 14012;
    int ERROR_SXS_UNKNOWN_ENCODING = 14013;
    int ERROR_SXS_INVALID_XML_NAMESPACE_URI = 14014;
    int ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED = 14015;
    int ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED = 14016;
    int ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE = 14017;
    int ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE = 14018;
    int ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE = 14019;
    int ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT = 14020;
    int ERROR_SXS_DUPLICATE_DLL_NAME = 14021;
    int ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME = 14022;
    int ERROR_SXS_DUPLICATE_CLSID = 14023;
    int ERROR_SXS_DUPLICATE_IID = 14024;
    int ERROR_SXS_DUPLICATE_TLBID = 14025;
    int ERROR_SXS_DUPLICATE_PROGID = 14026;
    int ERROR_SXS_DUPLICATE_ASSEMBLY_NAME = 14027;
    int ERROR_SXS_FILE_HASH_MISMATCH = 14028;
    int ERROR_SXS_POLICY_PARSE_ERROR = 14029;
    int ERROR_SXS_XML_E_MISSINGQUOTE = 14030;
    int ERROR_SXS_XML_E_COMMENTSYNTAX = 14031;
    int ERROR_SXS_XML_E_BADSTARTNAMECHAR = 14032;
    int ERROR_SXS_XML_E_BADNAMECHAR = 14033;
    int ERROR_SXS_XML_E_BADCHARINSTRING = 14034;
    int ERROR_SXS_XML_E_XMLDECLSYNTAX = 14035;
    int ERROR_SXS_XML_E_BADCHARDATA = 14036;
    int ERROR_SXS_XML_E_MISSINGWHITESPACE = 14037;
    int ERROR_SXS_XML_E_EXPECTINGTAGEND = 14038;
    int ERROR_SXS_XML_E_MISSINGSEMICOLON = 14039;
    int ERROR_SXS_XML_E_UNBALANCEDPAREN = 14040;
    int ERROR_SXS_XML_E_INTERNALERROR = 14041;
    int ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE = 14042;
    int ERROR_SXS_XML_E_INCOMPLETE_ENCODING = 14043;
    int ERROR_SXS_XML_E_MISSING_PAREN = 14044;
    int ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE = 14045;
    int ERROR_SXS_XML_E_MULTIPLE_COLONS = 14046;
    int ERROR_SXS_XML_E_INVALID_DECIMAL = 14047;
    int ERROR_SXS_XML_E_INVALID_HEXIDECIMAL = 14048;
    int ERROR_SXS_XML_E_INVALID_UNICODE = 14049;
    int ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK = 14050;
    int ERROR_SXS_XML_E_UNEXPECTEDENDTAG = 14051;
    int ERROR_SXS_XML_E_UNCLOSEDTAG = 14052;
    int ERROR_SXS_XML_E_DUPLICATEATTRIBUTE = 14053;
    int ERROR_SXS_XML_E_MULTIPLEROOTS = 14054;
    int ERROR_SXS_XML_E_INVALIDATROOTLEVEL = 14055;
    int ERROR_SXS_XML_E_BADXMLDECL = 14056;
    int ERROR_SXS_XML_E_MISSINGROOT = 14057;
    int ERROR_SXS_XML_E_UNEXPECTEDEOF = 14058;
    int ERROR_SXS_XML_E_BADPEREFINSUBSET = 14059;
    int ERROR_SXS_XML_E_UNCLOSEDSTARTTAG = 14060;
    int ERROR_SXS_XML_E_UNCLOSEDENDTAG = 14061;
    int ERROR_SXS_XML_E_UNCLOSEDSTRING = 14062;
    int ERROR_SXS_XML_E_UNCLOSEDCOMMENT = 14063;
    int ERROR_SXS_XML_E_UNCLOSEDDECL = 14064;
    int ERROR_SXS_XML_E_UNCLOSEDCDATA = 14065;
    int ERROR_SXS_XML_E_RESERVEDNAMESPACE = 14066;
    int ERROR_SXS_XML_E_INVALIDENCODING = 14067;
    int ERROR_SXS_XML_E_INVALIDSWITCH = 14068;
    int ERROR_SXS_XML_E_BADXMLCASE = 14069;
    int ERROR_SXS_XML_E_INVALID_STANDALONE = 14070;
    int ERROR_SXS_XML_E_UNEXPECTED_STANDALONE = 14071;
    int ERROR_SXS_XML_E_INVALID_VERSION = 14072;
    int ERROR_SXS_XML_E_MISSINGEQUALS = 14073;
    int ERROR_SXS_PROTECTION_RECOVERY_FAILED = 14074;
    int ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT = 14075;
    int ERROR_SXS_PROTECTION_CATALOG_NOT_VALID = 14076;
    int ERROR_SXS_UNTRANSLATABLE_HRESULT = 14077;
    int ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING = 14078;
    int ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE = 14079;
    int ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME = 14080;
    int ERROR_SXS_ASSEMBLY_MISSING = 14081;
    int ERROR_SXS_CORRUPT_ACTIVATION_STACK = 14082;
    int ERROR_SXS_CORRUPTION = 14083;
    int ERROR_SXS_EARLY_DEACTIVATION = 14084;
    int ERROR_SXS_INVALID_DEACTIVATION = 14085;
    int ERROR_SXS_MULTIPLE_DEACTIVATION = 14086;
    int ERROR_SXS_PROCESS_TERMINATION_REQUESTED = 14087;
    int ERROR_SXS_RELEASE_ACTIVATION_CONTEXT = 14088;
    int ERROR_SXS_SYSTEM_DEFAULT_ACTIVATION_CONTEXT_EMPTY = 14089;
    int ERROR_SXS_INVALID_IDENTITY_ATTRIBUTE_VALUE = 14090;
    int ERROR_SXS_INVALID_IDENTITY_ATTRIBUTE_NAME = 14091;
    int ERROR_SXS_IDENTITY_DUPLICATE_ATTRIBUTE = 14092;
    int ERROR_SXS_IDENTITY_PARSE_ERROR = 14093;
    int ERROR_MALFORMED_SUBSTITUTION_STRING = 14094;
    int ERROR_SXS_INCORRECT_PUBLIC_KEY_TOKEN = 14095;
    int ERROR_UNMAPPED_SUBSTITUTION_STRING = 14096;
    int ERROR_SXS_ASSEMBLY_NOT_LOCKED = 14097;
    int ERROR_SXS_COMPONENT_STORE_CORRUPT = 14098;
    int ERROR_ADVANCED_INSTALLER_FAILED = 14099;
    int ERROR_XML_ENCODING_MISMATCH = 14100;
    int ERROR_SXS_MANIFEST_IDENTITY_SAME_BUT_CONTENTS_DIFFERENT = 14101;
    int ERROR_SXS_IDENTITIES_DIFFERENT = 14102;
    int ERROR_SXS_ASSEMBLY_IS_NOT_A_DEPLOYMENT = 14103;
    int ERROR_SXS_FILE_NOT_PART_OF_ASSEMBLY = 14104;
    int ERROR_SXS_MANIFEST_TOO_BIG = 14105;
    int ERROR_SXS_SETTING_NOT_REGISTERED = 14106;
    int ERROR_SXS_TRANSACTION_CLOSURE_INCOMPLETE = 14107;
    int ERROR_SMI_PRIMITIVE_INSTALLER_FAILED = 14108;
    int ERROR_GENERIC_COMMAND_FAILED = 14109;
    int ERROR_SXS_FILE_HASH_MISSING = 14110;
    int ERROR_EVT_INVALID_CHANNEL_PATH = 15000;
    int ERROR_EVT_INVALID_QUERY = 15001;
    int ERROR_EVT_PUBLISHER_METADATA_NOT_FOUND = 15002;
    int ERROR_EVT_EVENT_TEMPLATE_NOT_FOUND = 15003;
    int ERROR_EVT_INVALID_PUBLISHER_NAME = 15004;
    int ERROR_EVT_INVALID_EVENT_DATA = 15005;
    int ERROR_EVT_CHANNEL_NOT_FOUND = 15007;
    int ERROR_EVT_MALFORMED_XML_TEXT = 15008;
    int ERROR_EVT_SUBSCRIPTION_TO_DIRECT_CHANNEL = 15009;
    int ERROR_EVT_CONFIGURATION_ERROR = 15010;
    int ERROR_EVT_QUERY_RESULT_STALE = 15011;
    int ERROR_EVT_QUERY_RESULT_INVALID_POSITION = 15012;
    int ERROR_EVT_NON_VALIDATING_MSXML = 15013;
    int ERROR_EVT_FILTER_ALREADYSCOPED = 15014;
    int ERROR_EVT_FILTER_NOTELTSET = 15015;
    int ERROR_EVT_FILTER_INVARG = 15016;
    int ERROR_EVT_FILTER_INVTEST = 15017;
    int ERROR_EVT_FILTER_INVTYPE = 15018;
    int ERROR_EVT_FILTER_PARSEERR = 15019;
    int ERROR_EVT_FILTER_UNSUPPORTEDOP = 15020;
    int ERROR_EVT_FILTER_UNEXPECTEDTOKEN = 15021;
    int ERROR_EVT_INVALID_OPERATION_OVER_ENABLED_DIRECT_CHANNEL = 15022;
    int ERROR_EVT_INVALID_CHANNEL_PROPERTY_VALUE = 15023;
    int ERROR_EVT_INVALID_PUBLISHER_PROPERTY_VALUE = 15024;
    int ERROR_EVT_CHANNEL_CANNOT_ACTIVATE = 15025;
    int ERROR_EVT_FILTER_TOO_COMPLEX = 15026;
    int ERROR_EVT_MESSAGE_NOT_FOUND = 15027;
    int ERROR_EVT_MESSAGE_ID_NOT_FOUND = 15028;
    int ERROR_EVT_UNRESOLVED_VALUE_INSERT = 15029;
    int ERROR_EVT_UNRESOLVED_PARAMETER_INSERT = 15030;
    int ERROR_EVT_MAX_INSERTS_REACHED = 15031;
    int ERROR_EVT_EVENT_DEFINITION_NOT_FOUND = 15032;
    int ERROR_EVT_MESSAGE_LOCALE_NOT_FOUND = 15033;
    int ERROR_EVT_VERSION_TOO_OLD = 15034;
    int ERROR_EVT_VERSION_TOO_NEW = 15035;
    int ERROR_EVT_CANNOT_OPEN_CHANNEL_OF_QUERY = 15036;
    int ERROR_EVT_PUBLISHER_DISABLED = 15037;
    int ERROR_EVT_FILTER_OUT_OF_RANGE = 15038;
    int ERROR_EC_SUBSCRIPTION_CANNOT_ACTIVATE = 15080;
    int ERROR_EC_LOG_DISABLED = 15081;
    int ERROR_EC_CIRCULAR_FORWARDING = 15082;
    int ERROR_EC_CREDSTORE_FULL = 15083;
    int ERROR_EC_CRED_NOT_FOUND = 15084;
    int ERROR_EC_NO_ACTIVE_CHANNEL = 15085;
    int ERROR_MUI_FILE_NOT_FOUND = 15100;
    int ERROR_MUI_INVALID_FILE = 15101;
    int ERROR_MUI_INVALID_RC_CONFIG = 15102;
    int ERROR_MUI_INVALID_LOCALE_NAME = 15103;
    int ERROR_MUI_INVALID_ULTIMATEFALLBACK_NAME = 15104;
    int ERROR_MUI_FILE_NOT_LOADED = 15105;
    int ERROR_RESOURCE_ENUM_USER_STOP = 15106;
    int ERROR_MUI_INTLSETTINGS_UILANG_NOT_INSTALLED = 15107;
    int ERROR_MUI_INTLSETTINGS_INVALID_LOCALE_NAME = 15108;
    int ERROR_MCA_INVALID_CAPABILITIES_STRING = 15200;
    int ERROR_MCA_INVALID_VCP_VERSION = 15201;
    int ERROR_MCA_MONITOR_VIOLATES_MCCS_SPECIFICATION = 15202;
    int ERROR_MCA_MCCS_VERSION_MISMATCH = 15203;
    int ERROR_MCA_UNSUPPORTED_MCCS_VERSION = 15204;
    int ERROR_MCA_INTERNAL_ERROR = 15205;
    int ERROR_MCA_INVALID_TECHNOLOGY_TYPE_RETURNED = 15206;
    int ERROR_MCA_UNSUPPORTED_COLOR_TEMPERATURE = 15207;
    int ERROR_AMBIGUOUS_SYSTEM_DEVICE = 15250;
    int ERROR_SYSTEM_DEVICE_NOT_FOUND = 15299;
    int SEVERITY_SUCCESS = 0;
    int SEVERITY_ERROR = 1;
    int FACILITY_NT_BIT = 0x10000000;
    int NOERROR = 0;
    int E_UNEXPECTED = 0x8000FFFF;
    int E_NOTIMPL = 0x80004001;
    int E_OUTOFMEMORY = 0x8007000E;
    int E_INVALIDARG = 0x80070057;
    int E_NOINTERFACE = 0x80004002;
    int E_POINTER = 0x80004003;
    int E_HANDLE = 0x80070006;
    int E_ABORT = 0x80004004;
    int E_FAIL = 0x80004005;
    int E_ACCESSDENIED = 0x80070005;
    int E_PENDING = 0x8000000A;
    int CO_E_INIT_TLS = 0x80004006;
    int CO_E_INIT_SHARED_ALLOCATOR = 0x80004007;
    int CO_E_INIT_MEMORY_ALLOCATOR = 0x80004008;
    int CO_E_INIT_CLASS_CACHE = 0x80004009;
    int CO_E_INIT_RPC_CHANNEL = 0x8000400A;
    int CO_E_INIT_TLS_SET_CHANNEL_CONTROL = 0x8000400B;
    int CO_E_INIT_TLS_CHANNEL_CONTROL = 0x8000400C;
    int CO_E_INIT_UNACCEPTED_USER_ALLOCATOR = 0x8000400D;
    int CO_E_INIT_SCM_MUTEX_EXISTS = 0x8000400E;
    int CO_E_INIT_SCM_FILE_MAPPING_EXISTS = 0x8000400F;
    int CO_E_INIT_SCM_MAP_VIEW_OF_FILE = 0x80004010;
    int CO_E_INIT_SCM_EXEC_FAILURE = 0x80004011;
    int CO_E_INIT_ONLY_SINGLE_THREADED = 0x80004012;
    int CO_E_CANT_REMOTE = 0x80004013;
    int CO_E_BAD_SERVER_NAME = 0x80004014;
    int CO_E_WRONG_SERVER_IDENTITY = 0x80004015;
    int CO_E_OLE1DDE_DISABLED = 0x80004016;
    int CO_E_RUNAS_SYNTAX = 0x80004017;
    int CO_E_CREATEPROCESS_FAILURE = 0x80004018;
    int CO_E_RUNAS_CREATEPROCESS_FAILURE = 0x80004019;
    int CO_E_RUNAS_LOGON_FAILURE = 0x8000401A;
    int CO_E_LAUNCH_PERMSSION_DENIED = 0x8000401B;
    int CO_E_START_SERVICE_FAILURE = 0x8000401C;
    int CO_E_REMOTE_COMMUNICATION_FAILURE = 0x8000401D;
    int CO_E_SERVER_START_TIMEOUT = 0x8000401E;
    int CO_E_CLSREG_INCONSISTENT = 0x8000401F;
    int CO_E_IIDREG_INCONSISTENT = 0x80004020;
    int CO_E_NOT_SUPPORTED = 0x80004021;
    int CO_E_RELOAD_DLL = 0x80004022;
    int CO_E_MSI_ERROR = 0x80004023;
    int CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT = 0x80004024;
    int CO_E_SERVER_PAUSED = 0x80004025;
    int CO_E_SERVER_NOT_PAUSED = 0x80004026;
    int CO_E_CLASS_DISABLED = 0x80004027;
    int CO_E_CLRNOTAVAILABLE = 0x80004028;
    int CO_E_ASYNC_WORK_REJECTED = 0x80004029;
    int CO_E_SERVER_INIT_TIMEOUT = 0x8000402A;
    int CO_E_NO_SECCTX_IN_ACTIVATE = 0x8000402B;
    int CO_E_TRACKER_CONFIG = 0x80004030;
    int CO_E_THREADPOOL_CONFIG = 0x80004031;
    int CO_E_SXS_CONFIG = 0x80004032;
    int CO_E_MALFORMED_SPN = 0x80004033;
    WinNT.HRESULT S_OK = new WinNT.HRESULT(0);
    WinNT.HRESULT S_FALSE = new WinNT.HRESULT(1);
    int OLE_E_FIRST = 0x80040000;
    int OLE_E_LAST = 0x800400FF;
    int OLE_S_FIRST = 0x00040000;
    int OLE_S_LAST = 0x000400FF;
    int OLE_E_OLEVERB = 0x80040000;
    int OLE_E_ADVF = 0x80040001;
    int OLE_E_ENUM_NOMORE = 0x80040002;
    int OLE_E_ADVISENOTSUPPORTED = 0x80040003;
    int OLE_E_NOCONNECTION = 0x80040004;
    int OLE_E_NOTRUNNING = 0x80040005;
    int OLE_E_NOCACHE = 0x80040006;
    int OLE_E_BLANK = 0x80040007;
    int OLE_E_CLASSDIFF = 0x80040008;
    int OLE_E_CANT_GETMONIKER = 0x80040009;
    int OLE_E_CANT_BINDTOSOURCE = 0x8004000A;
    int OLE_E_STATIC = 0x8004000B;
    int OLE_E_PROMPTSAVECANCELLED = 0x8004000C;
    int OLE_E_INVALIDRECT = 0x8004000D;
    int OLE_E_WRONGCOMPOBJ = 0x8004000E;
    int OLE_E_INVALIDHWND = 0x8004000F;
    int OLE_E_NOT_INPLACEACTIVE = 0x80040010;
    int OLE_E_CANTCONVERT = 0x80040011;
    int OLE_E_NOSTORAGE = 0x80040012;
    int DV_E_FORMATETC = 0x80040064;
    int DV_E_DVTARGETDEVICE = 0x80040065;
    int DV_E_STGMEDIUM = 0x80040066;
    int DV_E_STATDATA = 0x80040067;
    int DV_E_LINDEX = 0x80040068;
    int DV_E_TYMED = 0x80040069;
    int DV_E_CLIPFORMAT = 0x8004006A;
    int DV_E_DVASPECT = 0x8004006B;
    int DV_E_DVTARGETDEVICE_SIZE = 0x8004006C;
    int DV_E_NOIVIEWOBJECT = 0x8004006D;
    int DRAGDROP_E_FIRST = 0x80040100;
    int DRAGDROP_E_LAST = 0x8004010F;
    int DRAGDROP_S_FIRST = 0x00040100;
    int DRAGDROP_S_LAST = 0x0004010F;
    int DRAGDROP_E_NOTREGISTERED = 0x80040100;
    int DRAGDROP_E_ALREADYREGISTERED = 0x80040101;
    int DRAGDROP_E_INVALIDHWND = 0x80040102;
    int CLASSFACTORY_E_FIRST = 0x80040110;
    int CLASSFACTORY_E_LAST = 0x8004011F;
    int CLASSFACTORY_S_FIRST = 0x00040110;
    int CLASSFACTORY_S_LAST = 0x0004011F;
    int CLASS_E_NOAGGREGATION = 0x80040110;
    int CLASS_E_CLASSNOTAVAILABLE = 0x80040111;
    int CLASS_E_NOTLICENSED = 0x80040112;
    int MARSHAL_E_FIRST = 0x80040120;
    int MARSHAL_E_LAST = 0x8004012F;
    int MARSHAL_S_FIRST = 0x00040120;
    int MARSHAL_S_LAST = 0x0004012F;
    int DATA_E_FIRST = 0x80040130;
    int DATA_E_LAST = 0x8004013F;
    int DATA_S_FIRST = 0x00040130;
    int DATA_S_LAST = 0x0004013F;
    int VIEW_E_FIRST = 0x80040140;
    int VIEW_E_LAST = 0x8004014F;
    int VIEW_S_FIRST = 0x00040140;
    int VIEW_S_LAST = 0x0004014F;
    int VIEW_E_DRAW = 0x80040140;
    int REGDB_E_FIRST = 0x80040150;
    int REGDB_E_LAST = 0x8004015F;
    int REGDB_S_FIRST = 0x00040150;
    int REGDB_S_LAST = 0x0004015F;
    int REGDB_E_READREGDB = 0x80040150;
    int REGDB_E_WRITEREGDB = 0x80040151;
    int REGDB_E_KEYMISSING = 0x80040152;
    int REGDB_E_INVALIDVALUE = 0x80040153;
    int REGDB_E_CLASSNOTREG = 0x80040154;
    int REGDB_E_IIDNOTREG = 0x80040155;
    int REGDB_E_BADTHREADINGMODEL = 0x80040156;
    int CAT_E_FIRST = 0x80040160;
    int CAT_E_LAST = 0x80040161;
    int CAT_E_CATIDNOEXIST = 0x80040160;
    int CAT_E_NODESCRIPTION = 0x80040161;
    int CS_E_FIRST = 0x80040164;
    int CS_E_LAST = 0x8004016F;
    int CS_E_PACKAGE_NOTFOUND = 0x80040164;
    int CS_E_NOT_DELETABLE = 0x80040165;
    int CS_E_CLASS_NOTFOUND = 0x80040166;
    int CS_E_INVALID_VERSION = 0x80040167;
    int CS_E_NO_CLASSSTORE = 0x80040168;
    int CS_E_OBJECT_NOTFOUND = 0x80040169;
    int CS_E_OBJECT_ALREADY_EXISTS = 0x8004016A;
    int CS_E_INVALID_PATH = 0x8004016B;
    int CS_E_NETWORK_ERROR = 0x8004016C;
    int CS_E_ADMIN_LIMIT_EXCEEDED = 0x8004016D;
    int CS_E_SCHEMA_MISMATCH = 0x8004016E;
    int CS_E_INTERNAL_ERROR = 0x8004016F;
    int CACHE_E_FIRST = 0x80040170;
    int CACHE_E_LAST = 0x8004017F;
    int CACHE_S_FIRST = 0x00040170;
    int CACHE_S_LAST = 0x0004017F;
    int CACHE_E_NOCACHE_UPDATED = 0x80040170;
    int OLEOBJ_E_FIRST = 0x80040180;
    int OLEOBJ_E_LAST = 0x8004018F;
    int OLEOBJ_S_FIRST = 0x00040180;
    int OLEOBJ_S_LAST = 0x0004018F;
    int OLEOBJ_E_NOVERBS = 0x80040180;
    int OLEOBJ_E_INVALIDVERB = 0x80040181;
    int CLIENTSITE_E_FIRST = 0x80040190;
    int CLIENTSITE_E_LAST = 0x8004019F;
    int CLIENTSITE_S_FIRST = 0x00040190;
    int CLIENTSITE_S_LAST = 0x0004019F;
    int INPLACE_E_NOTUNDOABLE = 0x800401A0;
    int INPLACE_E_NOTOOLSPACE = 0x800401A1;
    int INPLACE_E_FIRST = 0x800401A0;
    int INPLACE_E_LAST = 0x800401AF;
    int INPLACE_S_FIRST = 0x000401A0;
    int INPLACE_S_LAST = 0x000401AF;
    int ENUM_E_FIRST = 0x800401B0;
    int ENUM_E_LAST = 0x800401BF;
    int ENUM_S_FIRST = 0x000401B0;
    int ENUM_S_LAST = 0x000401BF;
    int CONVERT10_E_FIRST = 0x800401C0;
    int CONVERT10_E_LAST = 0x800401CF;
    int CONVERT10_S_FIRST = 0x000401C0;
    int CONVERT10_S_LAST = 0x000401CF;
    int CONVERT10_E_OLESTREAM_GET = 0x800401C0;
    int CONVERT10_E_OLESTREAM_PUT = 0x800401C1;
    int CONVERT10_E_OLESTREAM_FMT = 0x800401C2;
    int CONVERT10_E_OLESTREAM_BITMAP_TO_DIB = 0x800401C3;
    int CONVERT10_E_STG_FMT = 0x800401C4;
    int CONVERT10_E_STG_NO_STD_STREAM = 0x800401C5;
    int CONVERT10_E_STG_DIB_TO_BITMAP = 0x800401C6;
    int CLIPBRD_E_FIRST = 0x800401D0;
    int CLIPBRD_E_LAST = 0x800401DF;
    int CLIPBRD_S_FIRST = 0x000401D0;
    int CLIPBRD_S_LAST = 0x000401DF;
    int CLIPBRD_E_CANT_OPEN = 0x800401D0;
    int CLIPBRD_E_CANT_EMPTY = 0x800401D1;
    int CLIPBRD_E_CANT_SET = 0x800401D2;
    int CLIPBRD_E_BAD_DATA = 0x800401D3;
    int CLIPBRD_E_CANT_CLOSE = 0x800401D4;
    int MK_E_FIRST = 0x800401E0;
    int MK_E_LAST = 0x800401EF;
    int MK_S_FIRST = 0x000401E0;
    int MK_S_LAST = 0x000401EF;
    int MK_E_CONNECTMANUALLY = 0x800401E0;
    int MK_E_EXCEEDEDDEADLINE = 0x800401E1;
    int MK_E_NEEDGENERIC = 0x800401E2;
    int MK_E_UNAVAILABLE = 0x800401E3;
    int MK_E_SYNTAX = 0x800401E4;
    int MK_E_NOOBJECT = 0x800401E5;
    int MK_E_INVALIDEXTENSION = 0x800401E6;
    int MK_E_INTERMEDIATEINTERFACENOTSUPPORTED = 0x800401E7;
    int MK_E_NOTBINDABLE = 0x800401E8;
    int MK_E_NOTBOUND = 0x800401E9;
    int MK_E_CANTOPENFILE = 0x800401EA;
    int MK_E_MUSTBOTHERUSER = 0x800401EB;
    int MK_E_NOINVERSE = 0x800401EC;
    int MK_E_NOSTORAGE = 0x800401ED;
    int MK_E_NOPREFIX = 0x800401EE;
    int MK_E_ENUMERATION_FAILED = 0x800401EF;
    int CO_E_FIRST = 0x800401F0;
    int CO_E_LAST = 0x800401FF;
    int CO_S_FIRST = 0x000401F0;
    int CO_S_LAST = 0x000401FF;
    int CO_E_NOTINITIALIZED = 0x800401F0;
    int CO_E_ALREADYINITIALIZED = 0x800401F1;
    int CO_E_CANTDETERMINECLASS = 0x800401F2;
    int CO_E_CLASSSTRING = 0x800401F3;
    int CO_E_IIDSTRING = 0x800401F4;
    int CO_E_APPNOTFOUND = 0x800401F5;
    int CO_E_APPSINGLEUSE = 0x800401F6;
    int CO_E_ERRORINAPP = 0x800401F7;
    int CO_E_DLLNOTFOUND = 0x800401F8;
    int CO_E_ERRORINDLL = 0x800401F9;
    int CO_E_WRONGOSFORAPP = 0x800401FA;
    int CO_E_OBJNOTREG = 0x800401FB;
    int CO_E_OBJISREG = 0x800401FC;
    int CO_E_OBJNOTCONNECTED = 0x800401FD;
    int CO_E_APPDIDNTREG = 0x800401FE;
    int CO_E_RELEASED = 0x800401FF;
    int EVENT_E_FIRST = 0x80040200;
    int EVENT_E_LAST = 0x8004021F;
    int EVENT_S_FIRST = 0x00040200;
    int EVENT_S_LAST = 0x0004021F;
    int EVENT_S_SOME_SUBSCRIBERS_FAILED = 0x00040200;
    int EVENT_E_ALL_SUBSCRIBERS_FAILED = 0x80040201;
    int EVENT_S_NOSUBSCRIBERS = 0x00040202;
    int EVENT_E_QUERYSYNTAX = 0x80040203;
    int EVENT_E_QUERYFIELD = 0x80040204;
    int EVENT_E_INTERNALEXCEPTION = 0x80040205;
    int EVENT_E_INTERNALERROR = 0x80040206;
    int EVENT_E_INVALID_PER_USER_SID = 0x80040207;
    int EVENT_E_USER_EXCEPTION = 0x80040208;
    int EVENT_E_TOO_MANY_METHODS = 0x80040209;
    int EVENT_E_MISSING_EVENTCLASS = 0x8004020A;
    int EVENT_E_NOT_ALL_REMOVED = 0x8004020B;
    int EVENT_E_COMPLUS_NOT_INSTALLED = 0x8004020C;
    int EVENT_E_CANT_MODIFY_OR_DELETE_UNCONFIGURED_OBJECT = 0x8004020D;
    int EVENT_E_CANT_MODIFY_OR_DELETE_CONFIGURED_OBJECT = 0x8004020E;
    int EVENT_E_INVALID_EVENT_CLASS_PARTITION = 0x8004020F;
    int EVENT_E_PER_USER_SID_NOT_LOGGED_ON = 0x80040210;
    int XACT_E_FIRST = 0x8004D000;
    int XACT_E_LAST = 0x8004D029;
    int XACT_S_FIRST = 0x0004D000;
    int XACT_S_LAST = 0x0004D010;
    int XACT_E_ALREADYOTHERSINGLEPHASE = 0x8004D000;
    int XACT_E_CANTRETAIN = 0x8004D001;
    int XACT_E_COMMITFAILED = 0x8004D002;
    int XACT_E_COMMITPREVENTED = 0x8004D003;
    int XACT_E_HEURISTICABORT = 0x8004D004;
    int XACT_E_HEURISTICCOMMIT = 0x8004D005;
    int XACT_E_HEURISTICDAMAGE = 0x8004D006;
    int XACT_E_HEURISTICDANGER = 0x8004D007;
    int XACT_E_ISOLATIONLEVEL = 0x8004D008;
    int XACT_E_NOASYNC = 0x8004D009;
    int XACT_E_NOENLIST = 0x8004D00A;
    int XACT_E_NOISORETAIN = 0x8004D00B;
    int XACT_E_NORESOURCE = 0x8004D00C;
    int XACT_E_NOTCURRENT = 0x8004D00D;
    int XACT_E_NOTRANSACTION = 0x8004D00E;
    int XACT_E_NOTSUPPORTED = 0x8004D00F;
    int XACT_E_UNKNOWNRMGRID = 0x8004D010;
    int XACT_E_WRONGSTATE = 0x8004D011;
    int XACT_E_WRONGUOW = 0x8004D012;
    int XACT_E_XTIONEXISTS = 0x8004D013;
    int XACT_E_NOIMPORTOBJECT = 0x8004D014;
    int XACT_E_INVALIDCOOKIE = 0x8004D015;
    int XACT_E_INDOUBT = 0x8004D016;
    int XACT_E_NOTIMEOUT = 0x8004D017;
    int XACT_E_ALREADYINPROGRESS = 0x8004D018;
    int XACT_E_ABORTED = 0x8004D019;
    int XACT_E_LOGFULL = 0x8004D01A;
    int XACT_E_TMNOTAVAILABLE = 0x8004D01B;
    int XACT_E_CONNECTION_DOWN = 0x8004D01C;
    int XACT_E_CONNECTION_DENIED = 0x8004D01D;
    int XACT_E_REENLISTTIMEOUT = 0x8004D01E;
    int XACT_E_TIP_CONNECT_FAILED = 0x8004D01F;
    int XACT_E_TIP_PROTOCOL_ERROR = 0x8004D020;
    int XACT_E_TIP_PULL_FAILED = 0x8004D021;
    int XACT_E_DEST_TMNOTAVAILABLE = 0x8004D022;
    int XACT_E_TIP_DISABLED = 0x8004D023;
    int XACT_E_NETWORK_TX_DISABLED = 0x8004D024;
    int XACT_E_PARTNER_NETWORK_TX_DISABLED = 0x8004D025;
    int XACT_E_XA_TX_DISABLED = 0x8004D026;
    int XACT_E_UNABLE_TO_READ_DTC_CONFIG = 0x8004D027;
    int XACT_E_UNABLE_TO_LOAD_DTC_PROXY = 0x8004D028;
    int XACT_E_ABORTING = 0x8004D029;
    int XACT_E_CLERKNOTFOUND = 0x8004D080;
    int XACT_E_CLERKEXISTS = 0x8004D081;
    int XACT_E_RECOVERYINPROGRESS = 0x8004D082;
    int XACT_E_TRANSACTIONCLOSED = 0x8004D083;
    int XACT_E_INVALIDLSN = 0x8004D084;
    int XACT_E_REPLAYREQUEST = 0x8004D085;
    int XACT_S_ASYNC = 0x0004D000;
    int XACT_S_DEFECT = 0x0004D001;
    int XACT_S_READONLY = 0x0004D002;
    int XACT_S_SOMENORETAIN = 0x0004D003;
    int XACT_S_OKINFORM = 0x0004D004;
    int XACT_S_MADECHANGESCONTENT = 0x0004D005;
    int XACT_S_MADECHANGESINFORM = 0x0004D006;
    int XACT_S_ALLNORETAIN = 0x0004D007;
    int XACT_S_ABORTING = 0x0004D008;
    int XACT_S_SINGLEPHASE = 0x0004D009;
    int XACT_S_LOCALLY_OK = 0x0004D00A;
    int XACT_S_LASTRESOURCEMANAGER = 0x0004D010;
    int CONTEXT_E_FIRST = 0x8004E000;
    int CONTEXT_E_LAST = 0x8004E02F;
    int CONTEXT_S_FIRST = 0x0004E000;
    int CONTEXT_S_LAST = 0x0004E02F;
    int CONTEXT_E_ABORTED = 0x8004E002;
    int CONTEXT_E_ABORTING = 0x8004E003;
    int CONTEXT_E_NOCONTEXT = 0x8004E004;
    int CONTEXT_E_WOULD_DEADLOCK = 0x8004E005;
    int CONTEXT_E_SYNCH_TIMEOUT = 0x8004E006;
    int CONTEXT_E_OLDREF = 0x8004E007;
    int CONTEXT_E_ROLENOTFOUND = 0x8004E00C;
    int CONTEXT_E_TMNOTAVAILABLE = 0x8004E00F;
    int CO_E_ACTIVATIONFAILED = 0x8004E021;
    int CO_E_ACTIVATIONFAILED_EVENTLOGGED = 0x8004E022;
    int CO_E_ACTIVATIONFAILED_CATALOGERROR = 0x8004E023;
    int CO_E_ACTIVATIONFAILED_TIMEOUT = 0x8004E024;
    int CO_E_INITIALIZATIONFAILED = 0x8004E025;
    int CONTEXT_E_NOJIT = 0x8004E026;
    int CONTEXT_E_NOTRANSACTION = 0x8004E027;
    int CO_E_THREADINGMODEL_CHANGED = 0x8004E028;
    int CO_E_NOIISINTRINSICS = 0x8004E029;
    int CO_E_NOCOOKIES = 0x8004E02A;
    int CO_E_DBERROR = 0x8004E02B;
    int CO_E_NOTPOOLED = 0x8004E02C;
    int CO_E_NOTCONSTRUCTED = 0x8004E02D;
    int CO_E_NOSYNCHRONIZATION = 0x8004E02E;
    int CO_E_ISOLEVELMISMATCH = 0x8004E02F;
    int CO_E_CALL_OUT_OF_TX_SCOPE_NOT_ALLOWED = 0x8004E030;
    int CO_E_EXIT_TRANSACTION_SCOPE_NOT_CALLED = 0x8004E031;
    int OLE_S_USEREG = 0x00040000;
    int OLE_S_STATIC = 0x00040001;
    int OLE_S_MAC_CLIPFORMAT = 0x00040002;
    int DRAGDROP_S_DROP = 0x00040100;
    int DRAGDROP_S_CANCEL = 0x00040101;
    int DRAGDROP_S_USEDEFAULTCURSORS = 0x00040102;
    int DATA_S_SAMEFORMATETC = 0x00040130;
    int VIEW_S_ALREADY_FROZEN = 0x00040140;
    int CACHE_S_FORMATETC_NOTSUPPORTED = 0x00040170;
    int CACHE_S_SAMECACHE = 0x00040171;
    int CACHE_S_SOMECACHES_NOTUPDATED = 0x00040172;
    int OLEOBJ_S_INVALIDVERB = 0x00040180;
    int OLEOBJ_S_CANNOT_DOVERB_NOW = 0x00040181;
    int OLEOBJ_S_INVALIDHWND = 0x00040182;
    int INPLACE_S_TRUNCATED = 0x000401A0;
    int CONVERT10_S_NO_PRESENTATION = 0x000401C0;
    int MK_S_REDUCED_TO_SELF = 0x000401E2;
    int MK_S_ME = 0x000401E4;
    int MK_S_HIM = 0x000401E5;
    int MK_S_US = 0x000401E6;
    int MK_S_MONIKERALREADYREGISTERED = 0x000401E7;
    int SCHED_S_TASK_READY = 0x00041300;
    int SCHED_S_TASK_RUNNING = 0x00041301;
    int SCHED_S_TASK_DISABLED = 0x00041302;
    int SCHED_S_TASK_HAS_NOT_RUN = 0x00041303;
    int SCHED_S_TASK_NO_MORE_RUNS = 0x00041304;
    int SCHED_S_TASK_NOT_SCHEDULED = 0x00041305;
    int SCHED_S_TASK_TERMINATED = 0x00041306;
    int SCHED_S_TASK_NO_VALID_TRIGGERS = 0x00041307;
    int SCHED_S_EVENT_TRIGGER = 0x00041308;
    int SCHED_E_TRIGGER_NOT_FOUND = 0x80041309;
    int SCHED_E_TASK_NOT_READY = 0x8004130A;
    int SCHED_E_TASK_NOT_RUNNING = 0x8004130B;
    int SCHED_E_SERVICE_NOT_INSTALLED = 0x8004130C;
    int SCHED_E_CANNOT_OPEN_TASK = 0x8004130D;
    int SCHED_E_INVALID_TASK = 0x8004130E;
    int SCHED_E_ACCOUNT_INFORMATION_NOT_SET = 0x8004130F;
    int SCHED_E_ACCOUNT_NAME_NOT_FOUND = 0x80041310;
    int SCHED_E_ACCOUNT_DBASE_CORRUPT = 0x80041311;
    int SCHED_E_NO_SECURITY_SERVICES = 0x80041312;
    int SCHED_E_UNKNOWN_OBJECT_VERSION = 0x80041313;
    int SCHED_E_UNSUPPORTED_ACCOUNT_OPTION = 0x80041314;
    int SCHED_E_SERVICE_NOT_RUNNING = 0x80041315;
    int SCHED_E_UNEXPECTEDNODE = 0x80041316;
    int SCHED_E_NAMESPACE = 0x80041317;
    int SCHED_E_INVALIDVALUE = 0x80041318;
    int SCHED_E_MISSINGNODE = 0x80041319;
    int SCHED_E_MALFORMEDXML = 0x8004131A;
    int SCHED_S_SOME_TRIGGERS_FAILED = 0x0004131B;
    int SCHED_S_BATCH_LOGON_PROBLEM = 0x0004131C;
    int SCHED_E_TOO_MANY_NODES = 0x8004131D;
    int SCHED_E_PAST_END_BOUNDARY = 0x8004131E;
    int SCHED_E_ALREADY_RUNNING = 0x8004131F;
    int SCHED_E_USER_NOT_LOGGED_ON = 0x80041320;
    int SCHED_E_INVALID_TASK_HASH = 0x80041321;
    int SCHED_E_SERVICE_NOT_AVAILABLE = 0x80041322;
    int SCHED_E_SERVICE_TOO_BUSY = 0x80041323;
    int SCHED_E_TASK_ATTEMPTED = 0x80041324;
    int SCHED_S_TASK_QUEUED = 0x00041325;
    int SCHED_E_TASK_DISABLED = 0x80041326;
    int SCHED_E_TASK_NOT_V1_COMPAT = 0x80041327;
    int SCHED_E_START_ON_DEMAND = 0x80041328;
    int CO_E_CLASS_CREATE_FAILED = 0x80080001;
    int CO_E_SCM_ERROR = 0x80080002;
    int CO_E_SCM_RPC_FAILURE = 0x80080003;
    int CO_E_BAD_PATH = 0x80080004;
    int CO_E_SERVER_EXEC_FAILURE = 0x80080005;
    int CO_E_OBJSRV_RPC_FAILURE = 0x80080006;
    int MK_E_NO_NORMALIZED = 0x80080007;
    int CO_E_SERVER_STOPPING = 0x80080008;
    int MEM_E_INVALID_ROOT = 0x80080009;
    int MEM_E_INVALID_LINK = 0x80080010;
    int MEM_E_INVALID_SIZE = 0x80080011;
    int CO_S_NOTALLINTERFACES = 0x00080012;
    int CO_S_MACHINENAMENOTFOUND = 0x00080013;
    int CO_E_MISSING_DISPLAYNAME = 0x80080015;
    int CO_E_RUNAS_VALUE_MUST_BE_AAA = 0x80080016;
    int CO_E_ELEVATION_DISABLED = 0x80080017;
    int DISP_E_UNKNOWNINTERFACE = 0x80020001;
    int DISP_E_MEMBERNOTFOUND = 0x80020003;
    int DISP_E_PARAMNOTFOUND = 0x80020004;
    int DISP_E_TYPEMISMATCH = 0x80020005;
    int DISP_E_UNKNOWNNAME = 0x80020006;
    int DISP_E_NONAMEDARGS = 0x80020007;
    int DISP_E_BADVARTYPE = 0x80020008;
    int DISP_E_EXCEPTION = 0x80020009;
    int DISP_E_OVERFLOW = 0x8002000A;
    int DISP_E_BADINDEX = 0x8002000B;
    int DISP_E_UNKNOWNLCID = 0x8002000C;
    int DISP_E_ARRAYISLOCKED = 0x8002000D;
    int DISP_E_BADPARAMCOUNT = 0x8002000E;
    int DISP_E_PARAMNOTOPTIONAL = 0x8002000F;
    int DISP_E_BADCALLEE = 0x80020010;
    int DISP_E_NOTACOLLECTION = 0x80020011;
    int DISP_E_DIVBYZERO = 0x80020012;
    int DISP_E_BUFFERTOOSMALL = 0x80020013;
    int TYPE_E_BUFFERTOOSMALL = 0x80028016;
    int TYPE_E_FIELDNOTFOUND = 0x80028017;
    int TYPE_E_INVDATAREAD = 0x80028018;
    int TYPE_E_UNSUPFORMAT = 0x80028019;
    int TYPE_E_REGISTRYACCESS = 0x8002801C;
    int TYPE_E_LIBNOTREGISTERED = 0x8002801D;
    int TYPE_E_UNDEFINEDTYPE = 0x80028027;
    int TYPE_E_QUALIFIEDNAMEDISALLOWED = 0x80028028;
    int TYPE_E_INVALIDSTATE = 0x80028029;
    int TYPE_E_WRONGTYPEKIND = 0x8002802A;
    int TYPE_E_ELEMENTNOTFOUND = 0x8002802B;
    int TYPE_E_AMBIGUOUSNAME = 0x8002802C;
    int TYPE_E_NAMECONFLICT = 0x8002802D;
    int TYPE_E_UNKNOWNLCID = 0x8002802E;
    int TYPE_E_DLLFUNCTIONNOTFOUND = 0x8002802F;
    int TYPE_E_BADMODULEKIND = 0x800288BD;
    int TYPE_E_SIZETOOBIG = 0x800288C5;
    int TYPE_E_DUPLICATEID = 0x800288C6;
    int TYPE_E_INVALIDID = 0x800288CF;
    int TYPE_E_TYPEMISMATCH = 0x80028CA0;
    int TYPE_E_OUTOFBOUNDS = 0x80028CA1;
    int TYPE_E_IOERROR = 0x80028CA2;
    int TYPE_E_CANTCREATETMPFILE = 0x80028CA3;
    int TYPE_E_CANTLOADLIBRARY = 0x80029C4A;
    int TYPE_E_INCONSISTENTPROPFUNCS = 0x80029C83;
    int TYPE_E_CIRCULARTYPE = 0x80029C84;
    int STG_E_INVALIDFUNCTION = 0x80030001;
    int STG_E_FILENOTFOUND = 0x80030002;
    int STG_E_PATHNOTFOUND = 0x80030003;
    int STG_E_TOOMANYOPENFILES = 0x80030004;
    int STG_E_ACCESSDENIED = 0x80030005;
    int STG_E_INVALIDHANDLE = 0x80030006;
    int STG_E_INSUFFICIENTMEMORY = 0x80030008;
    int STG_E_INVALIDPOINTER = 0x80030009;
    int STG_E_NOMOREFILES = 0x80030012;
    int STG_E_DISKISWRITEPROTECTED = 0x80030013;
    int STG_E_SEEKERROR = 0x80030019;
    int STG_E_WRITEFAULT = 0x8003001D;
    int STG_E_READFAULT = 0x8003001E;
    int STG_E_SHAREVIOLATION = 0x80030020;
    int STG_E_LOCKVIOLATION = 0x80030021;
    int STG_E_FILEALREADYEXISTS = 0x80030050;
    int STG_E_INVALIDPARAMETER = 0x80030057;
    int STG_E_MEDIUMFULL = 0x80030070;
    int STG_E_PROPSETMISMATCHED = 0x800300F0;
    int STG_E_ABNORMALAPIEXIT = 0x800300FA;
    int STG_E_INVALIDHEADER = 0x800300FB;
    int STG_E_INVALIDNAME = 0x800300FC;
    int STG_E_UNKNOWN = 0x800300FD;
    int STG_E_UNIMPLEMENTEDFUNCTION = 0x800300FE;
    int STG_E_INVALIDFLAG = 0x800300FF;
    int STG_E_INUSE = 0x80030100;
    int STG_E_NOTCURRENT = 0x80030101;
    int STG_E_REVERTED = 0x80030102;
    int STG_E_CANTSAVE = 0x80030103;
    int STG_E_OLDFORMAT = 0x80030104;
    int STG_E_OLDDLL = 0x80030105;
    int STG_E_SHAREREQUIRED = 0x80030106;
    int STG_E_NOTFILEBASEDSTORAGE = 0x80030107;
    int STG_E_EXTANTMARSHALLINGS = 0x80030108;
    int STG_E_DOCFILECORRUPT = 0x80030109;
    int STG_E_BADBASEADDRESS = 0x80030110;
    int STG_E_DOCFILETOOLARGE = 0x80030111;
    int STG_E_NOTSIMPLEFORMAT = 0x80030112;
    int STG_E_INCOMPLETE = 0x80030201;
    int STG_E_TERMINATED = 0x80030202;
    int STG_S_CONVERTED = 0x00030200;
    int STG_S_BLOCK = 0x00030201;
    int STG_S_RETRYNOW = 0x00030202;
    int STG_S_MONITORING = 0x00030203;
    int STG_S_MULTIPLEOPENS = 0x00030204;
    int STG_S_CONSOLIDATIONFAILED = 0x00030205;
    int STG_S_CANNOTCONSOLIDATE = 0x00030206;
    int STG_E_STATUS_COPY_PROTECTION_FAILURE = 0x80030305;
    int STG_E_CSS_AUTHENTICATION_FAILURE = 0x80030306;
    int STG_E_CSS_KEY_NOT_PRESENT = 0x80030307;
    int STG_E_CSS_KEY_NOT_ESTABLISHED = 0x80030308;
    int STG_E_CSS_SCRAMBLED_SECTOR = 0x80030309;
    int STG_E_CSS_REGION_MISMATCH = 0x8003030A;
    int STG_E_RESETS_EXHAUSTED = 0x8003030B;
    int RPC_E_CALL_REJECTED = 0x80010001;
    int RPC_E_CALL_CANCELED = 0x80010002;
    int RPC_E_CANTPOST_INSENDCALL = 0x80010003;
    int RPC_E_CANTCALLOUT_INASYNCCALL = 0x80010004;
    int RPC_E_CANTCALLOUT_INEXTERNALCALL = 0x80010005;
    int RPC_E_CONNECTION_TERMINATED = 0x80010006;
    int RPC_E_SERVER_DIED = 0x80010007;
    int RPC_E_CLIENT_DIED = 0x80010008;
    int RPC_E_INVALID_DATAPACKET = 0x80010009;
    int RPC_E_CANTTRANSMIT_CALL = 0x8001000A;
    int RPC_E_CLIENT_CANTMARSHAL_DATA = 0x8001000B;
    int RPC_E_CLIENT_CANTUNMARSHAL_DATA = 0x8001000C;
    int RPC_E_SERVER_CANTMARSHAL_DATA = 0x8001000D;
    int RPC_E_SERVER_CANTUNMARSHAL_DATA = 0x8001000E;
    int RPC_E_INVALID_DATA = 0x8001000F;
    int RPC_E_INVALID_PARAMETER = 0x80010010;
    int RPC_E_CANTCALLOUT_AGAIN = 0x80010011;
    int RPC_E_SERVER_DIED_DNE = 0x80010012;
    int RPC_E_SYS_CALL_FAILED = 0x80010100;
    int RPC_E_OUT_OF_RESOURCES = 0x80010101;
    int RPC_E_ATTEMPTED_MULTITHREAD = 0x80010102;
    int RPC_E_NOT_REGISTERED = 0x80010103;
    int RPC_E_FAULT = 0x80010104;
    int RPC_E_SERVERFAULT = 0x80010105;
    int RPC_E_CHANGED_MODE = 0x80010106;
    int RPC_E_INVALIDMETHOD = 0x80010107;
    int RPC_E_DISCONNECTED = 0x80010108;
    int RPC_E_RETRY = 0x80010109;
    int RPC_E_SERVERCALL_RETRYLATER = 0x8001010A;
    int RPC_E_SERVERCALL_REJECTED = 0x8001010B;
    int RPC_E_INVALID_CALLDATA = 0x8001010C;
    int RPC_E_CANTCALLOUT_ININPUTSYNCCALL = 0x8001010D;
    int RPC_E_WRONG_THREAD = 0x8001010E;
    int RPC_E_THREAD_NOT_INIT = 0x8001010F;
    int RPC_E_VERSION_MISMATCH = 0x80010110;
    int RPC_E_INVALID_HEADER = 0x80010111;
    int RPC_E_INVALID_EXTENSION = 0x80010112;
    int RPC_E_INVALID_IPID = 0x80010113;
    int RPC_E_INVALID_OBJECT = 0x80010114;
    int RPC_S_CALLPENDING = 0x80010115;
    int RPC_S_WAITONTIMER = 0x80010116;
    int RPC_E_CALL_COMPLETE = 0x80010117;
    int RPC_E_UNSECURE_CALL = 0x80010118;
    int RPC_E_TOO_LATE = 0x80010119;
    int RPC_E_NO_GOOD_SECURITY_PACKAGES = 0x8001011A;
    int RPC_E_ACCESS_DENIED = 0x8001011B;
    int RPC_E_REMOTE_DISABLED = 0x8001011C;
    int RPC_E_INVALID_OBJREF = 0x8001011D;
    int RPC_E_NO_CONTEXT = 0x8001011E;
    int RPC_E_TIMEOUT = 0x8001011F;
    int RPC_E_NO_SYNC = 0x80010120;
    int RPC_E_FULLSIC_REQUIRED = 0x80010121;
    int RPC_E_INVALID_STD_NAME = 0x80010122;
    int CO_E_FAILEDTOIMPERSONATE = 0x80010123;
    int CO_E_FAILEDTOGETSECCTX = 0x80010124;
    int CO_E_FAILEDTOOPENTHREADTOKEN = 0x80010125;
    int CO_E_FAILEDTOGETTOKENINFO = 0x80010126;
    int CO_E_TRUSTEEDOESNTMATCHCLIENT = 0x80010127;
    int CO_E_FAILEDTOQUERYCLIENTBLANKET = 0x80010128;
    int CO_E_FAILEDTOSETDACL = 0x80010129;
    int CO_E_ACCESSCHECKFAILED = 0x8001012A;
    int CO_E_NETACCESSAPIFAILED = 0x8001012B;
    int CO_E_WRONGTRUSTEENAMESYNTAX = 0x8001012C;
    int CO_E_INVALIDSID = 0x8001012D;
    int CO_E_CONVERSIONFAILED = 0x8001012E;
    int CO_E_NOMATCHINGSIDFOUND = 0x8001012F;
    int CO_E_LOOKUPACCSIDFAILED = 0x80010130;
    int CO_E_NOMATCHINGNAMEFOUND = 0x80010131;
    int CO_E_LOOKUPACCNAMEFAILED = 0x80010132;
    int CO_E_SETSERLHNDLFAILED = 0x80010133;
    int CO_E_FAILEDTOGETWINDIR = 0x80010134;
    int CO_E_PATHTOOLONG = 0x80010135;
    int CO_E_FAILEDTOGENUUID = 0x80010136;
    int CO_E_FAILEDTOCREATEFILE = 0x80010137;
    int CO_E_FAILEDTOCLOSEHANDLE = 0x80010138;
    int CO_E_EXCEEDSYSACLLIMIT = 0x80010139;
    int CO_E_ACESINWRONGORDER = 0x8001013A;
    int CO_E_INCOMPATIBLESTREAMVERSION = 0x8001013B;
    int CO_E_FAILEDTOOPENPROCESSTOKEN = 0x8001013C;
    int CO_E_DECODEFAILED = 0x8001013D;
    int CO_E_ACNOTINITIALIZED = 0x8001013F;
    int CO_E_CANCEL_DISABLED = 0x80010140;
    int RPC_E_UNEXPECTED = 0x8001FFFF;
    int ERROR_AUDITING_DISABLED = 0xC0090001;
    int ERROR_ALL_SIDS_FILTERED = 0xC0090002;
    int ERROR_BIZRULES_NOT_ENABLED = 0xC0090003;
    int NTE_BAD_UID = 0x80090001;
    int NTE_BAD_HASH = 0x80090002;
    int NTE_BAD_KEY = 0x80090003;
    int NTE_BAD_LEN = 0x80090004;
    int NTE_BAD_DATA = 0x80090005;
    int NTE_BAD_SIGNATURE = 0x80090006;
    int NTE_BAD_VER = 0x80090007;
    int NTE_BAD_ALGID = 0x80090008;
    int NTE_BAD_FLAGS = 0x80090009;
    int NTE_BAD_TYPE = 0x8009000A;
    int NTE_BAD_KEY_STATE = 0x8009000B;
    int NTE_BAD_HASH_STATE = 0x8009000C;
    int NTE_NO_KEY = 0x8009000D;
    int NTE_NO_MEMORY = 0x8009000E;
    int NTE_EXISTS = 0x8009000F;
    int NTE_PERM = 0x80090010;
    int NTE_NOT_FOUND = 0x80090011;
    int NTE_DOUBLE_ENCRYPT = 0x80090012;
    int NTE_BAD_PROVIDER = 0x80090013;
    int NTE_BAD_PROV_TYPE = 0x80090014;
    int NTE_BAD_PUBLIC_KEY = 0x80090015;
    int NTE_BAD_KEYSET = 0x80090016;
    int NTE_PROV_TYPE_NOT_DEF = 0x80090017;
    int NTE_PROV_TYPE_ENTRY_BAD = 0x80090018;
    int NTE_KEYSET_NOT_DEF = 0x80090019;
    int NTE_KEYSET_ENTRY_BAD = 0x8009001A;
    int NTE_PROV_TYPE_NO_MATCH = 0x8009001B;
    int NTE_SIGNATURE_FILE_BAD = 0x8009001C;
    int NTE_PROVIDER_DLL_FAIL = 0x8009001D;
    int NTE_PROV_DLL_NOT_FOUND = 0x8009001E;
    int NTE_BAD_KEYSET_PARAM = 0x8009001F;
    int NTE_FAIL = 0x80090020;
    int NTE_SYS_ERR = 0x80090021;
    int NTE_SILENT_CONTEXT = 0x80090022;
    int NTE_TOKEN_KEYSET_STORAGE_FULL = 0x80090023;
    int NTE_TEMPORARY_PROFILE = 0x80090024;
    int NTE_FIXEDPARAMETER = 0x80090025;
    int NTE_INVALID_HANDLE = 0x80090026;
    int NTE_INVALID_PARAMETER = 0x80090027;
    int NTE_BUFFER_TOO_SMALL = 0x80090028;
    int NTE_NOT_SUPPORTED = 0x80090029;
    int NTE_NO_MORE_ITEMS = 0x8009002A;
    int NTE_BUFFERS_OVERLAP = 0x8009002B;
    int NTE_DECRYPTION_FAILURE = 0x8009002C;
    int NTE_INTERNAL_ERROR = 0x8009002D;
    int NTE_UI_REQUIRED = 0x8009002E;
    int NTE_HMAC_NOT_SUPPORTED = 0x8009002F;
    int SEC_E_INSUFFICIENT_MEMORY = 0x80090300;
    int SEC_E_INVALID_HANDLE = 0x80090301;
    int SEC_E_UNSUPPORTED_FUNCTION = 0x80090302;
    int SEC_E_TARGET_UNKNOWN = 0x80090303;
    int SEC_E_INTERNAL_ERROR = 0x80090304;
    int SEC_E_SECPKG_NOT_FOUND = 0x80090305;
    int SEC_E_NOT_OWNER = 0x80090306;
    int SEC_E_CANNOT_INSTALL = 0x80090307;
    int SEC_E_INVALID_TOKEN = 0x80090308;
    int SEC_E_CANNOT_PACK = 0x80090309;
    int SEC_E_QOP_NOT_SUPPORTED = 0x8009030A;
    int SEC_E_NO_IMPERSONATION = 0x8009030B;
    int SEC_E_LOGON_DENIED = 0x8009030C;
    int SEC_E_UNKNOWN_CREDENTIALS = 0x8009030D;
    int SEC_E_NO_CREDENTIALS = 0x8009030E;
    int SEC_E_MESSAGE_ALTERED = 0x8009030F;
    int SEC_E_OUT_OF_SEQUENCE = 0x80090310;
    int SEC_E_NO_AUTHENTICATING_AUTHORITY = 0x80090311;
    int SEC_I_CONTINUE_NEEDED = 0x00090312;
    int SEC_I_COMPLETE_NEEDED = 0x00090313;
    int SEC_I_COMPLETE_AND_CONTINUE = 0x00090314;
    int SEC_I_LOCAL_LOGON = 0x00090315;
    int SEC_E_BAD_PKGID = 0x80090316;
    int SEC_E_CONTEXT_EXPIRED = 0x80090317;
    int SEC_I_CONTEXT_EXPIRED = 0x00090317;
    int SEC_E_INCOMPLETE_MESSAGE = 0x80090318;
    int SEC_E_INCOMPLETE_CREDENTIALS = 0x80090320;
    int SEC_E_BUFFER_TOO_SMALL = 0x80090321;
    int SEC_I_INCOMPLETE_CREDENTIALS = 0x00090320;
    int SEC_I_RENEGOTIATE = 0x00090321;
    int SEC_E_WRONG_PRINCIPAL = 0x80090322;
    int SEC_I_NO_LSA_CONTEXT = 0x00090323;
    int SEC_E_TIME_SKEW = 0x80090324;
    int SEC_E_UNTRUSTED_ROOT = 0x80090325;
    int SEC_E_ILLEGAL_MESSAGE = 0x80090326;
    int SEC_E_CERT_UNKNOWN = 0x80090327;
    int SEC_E_CERT_EXPIRED = 0x80090328;
    int SEC_E_ENCRYPT_FAILURE = 0x80090329;
    int SEC_E_DECRYPT_FAILURE = 0x80090330;
    int SEC_E_ALGORITHM_MISMATCH = 0x80090331;
    int SEC_E_SECURITY_QOS_FAILED = 0x80090332;
    int SEC_E_UNFINISHED_CONTEXT_DELETED = 0x80090333;
    int SEC_E_NO_TGT_REPLY = 0x80090334;
    int SEC_E_NO_IP_ADDRESSES = 0x80090335;
    int SEC_E_WRONG_CREDENTIAL_HANDLE = 0x80090336;
    int SEC_E_CRYPTO_SYSTEM_INVALID = 0x80090337;
    int SEC_E_MAX_REFERRALS_EXCEEDED = 0x80090338;
    int SEC_E_MUST_BE_KDC = 0x80090339;
    int SEC_E_STRONG_CRYPTO_NOT_SUPPORTED = 0x8009033A;
    int SEC_E_TOO_MANY_PRINCIPALS = 0x8009033B;
    int SEC_E_NO_PA_DATA = 0x8009033C;
    int SEC_E_PKINIT_NAME_MISMATCH = 0x8009033D;
    int SEC_E_SMARTCARD_LOGON_REQUIRED = 0x8009033E;
    int SEC_E_SHUTDOWN_IN_PROGRESS = 0x8009033F;
    int SEC_E_KDC_INVALID_REQUEST = 0x80090340;
    int SEC_E_KDC_UNABLE_TO_REFER = 0x80090341;
    int SEC_E_KDC_UNKNOWN_ETYPE = 0x80090342;
    int SEC_E_UNSUPPORTED_PREAUTH = 0x80090343;
    int SEC_E_DELEGATION_REQUIRED = 0x80090345;
    int SEC_E_BAD_BINDINGS = 0x80090346;
    int SEC_E_MULTIPLE_ACCOUNTS = 0x80090347;
    int SEC_E_NO_KERB_KEY = 0x80090348;
    int SEC_E_CERT_WRONG_USAGE = 0x80090349;
    int SEC_E_DOWNGRADE_DETECTED = 0x80090350;
    int SEC_E_SMARTCARD_CERT_REVOKED = 0x80090351;
    int SEC_E_ISSUING_CA_UNTRUSTED = 0x80090352;
    int SEC_E_REVOCATION_OFFLINE_C = 0x80090353;
    int SEC_E_PKINIT_CLIENT_FAILURE = 0x80090354;
    int SEC_E_SMARTCARD_CERT_EXPIRED = 0x80090355;
    int SEC_E_NO_S4U_PROT_SUPPORT = 0x80090356;
    int SEC_E_CROSSREALM_DELEGATION_FAILURE = 0x80090357;
    int SEC_E_REVOCATION_OFFLINE_KDC = 0x80090358;
    int SEC_E_ISSUING_CA_UNTRUSTED_KDC = 0x80090359;
    int SEC_E_KDC_CERT_EXPIRED = 0x8009035A;
    int SEC_E_KDC_CERT_REVOKED = 0x8009035B;
    int SEC_I_SIGNATURE_NEEDED = 0x0009035C;
    int SEC_E_INVALID_PARAMETER = 0x8009035D;
    int SEC_E_DELEGATION_POLICY = 0x8009035E;
    int SEC_E_POLICY_NLTM_ONLY = 0x8009035F;
    int SEC_I_NO_RENEGOTIATION = 0x00090360;
    int SEC_E_NO_SPM = SEC_E_INTERNAL_ERROR;
    int SEC_E_NOT_SUPPORTED = SEC_E_UNSUPPORTED_FUNCTION;
    int CRYPT_E_MSG_ERROR = 0x80091001;
    int CRYPT_E_UNKNOWN_ALGO = 0x80091002;
    int CRYPT_E_OID_FORMAT = 0x80091003;
    int CRYPT_E_INVALID_MSG_TYPE = 0x80091004;
    int CRYPT_E_UNEXPECTED_ENCODING = 0x80091005;
    int CRYPT_E_AUTH_ATTR_MISSING = 0x80091006;
    int CRYPT_E_HASH_VALUE = 0x80091007;
    int CRYPT_E_INVALID_INDEX = 0x80091008;
    int CRYPT_E_ALREADY_DECRYPTED = 0x80091009;
    int CRYPT_E_NOT_DECRYPTED = 0x8009100A;
    int CRYPT_E_RECIPIENT_NOT_FOUND = 0x8009100B;
    int CRYPT_E_CONTROL_TYPE = 0x8009100C;
    int CRYPT_E_ISSUER_SERIALNUMBER = 0x8009100D;
    int CRYPT_E_SIGNER_NOT_FOUND = 0x8009100E;
    int CRYPT_E_ATTRIBUTES_MISSING = 0x8009100F;
    int CRYPT_E_STREAM_MSG_NOT_READY = 0x80091010;
    int CRYPT_E_STREAM_INSUFFICIENT_DATA = 0x80091011;
    int CRYPT_I_NEW_PROTECTION_REQUIRED = 0x00091012;
    int CRYPT_E_BAD_LEN = 0x80092001;
    int CRYPT_E_BAD_ENCODE = 0x80092002;
    int CRYPT_E_FILE_ERROR = 0x80092003;
    int CRYPT_E_NOT_FOUND = 0x80092004;
    int CRYPT_E_EXISTS = 0x80092005;
    int CRYPT_E_NO_PROVIDER = 0x80092006;
    int CRYPT_E_SELF_SIGNED = 0x80092007;
    int CRYPT_E_DELETED_PREV = 0x80092008;
    int CRYPT_E_NO_MATCH = 0x80092009;
    int CRYPT_E_UNEXPECTED_MSG_TYPE = 0x8009200A;
    int CRYPT_E_NO_KEY_PROPERTY = 0x8009200B;
    int CRYPT_E_NO_DECRYPT_CERT = 0x8009200C;
    int CRYPT_E_BAD_MSG = 0x8009200D;
    int CRYPT_E_NO_SIGNER = 0x8009200E;
    int CRYPT_E_PENDING_CLOSE = 0x8009200F;
    int CRYPT_E_REVOKED = 0x80092010;
    int CRYPT_E_NO_REVOCATION_DLL = 0x80092011;
    int CRYPT_E_NO_REVOCATION_CHECK = 0x80092012;
    int CRYPT_E_REVOCATION_OFFLINE = 0x80092013;
    int CRYPT_E_NOT_IN_REVOCATION_DATABASE = 0x80092014;
    int CRYPT_E_INVALID_NUMERIC_STRING = 0x80092020;
    int CRYPT_E_INVALID_PRINTABLE_STRING = 0x80092021;
    int CRYPT_E_INVALID_IA5_STRING = 0x80092022;
    int CRYPT_E_INVALID_X500_STRING = 0x80092023;
    int CRYPT_E_NOT_CHAR_STRING = 0x80092024;
    int CRYPT_E_FILERESIZED = 0x80092025;
    int CRYPT_E_SECURITY_SETTINGS = 0x80092026;
    int CRYPT_E_NO_VERIFY_USAGE_DLL = 0x80092027;
    int CRYPT_E_NO_VERIFY_USAGE_CHECK = 0x80092028;
    int CRYPT_E_VERIFY_USAGE_OFFLINE = 0x80092029;
    int CRYPT_E_NOT_IN_CTL = 0x8009202A;
    int CRYPT_E_NO_TRUSTED_SIGNER = 0x8009202B;
    int CRYPT_E_MISSING_PUBKEY_PARA = 0x8009202C;
    int CRYPT_E_OSS_ERROR = 0x80093000;
    int OSS_MORE_BUF = 0x80093001;
    int OSS_NEGATIVE_UINTEGER = 0x80093002;
    int OSS_PDU_RANGE = 0x80093003;
    int OSS_MORE_INPUT = 0x80093004;
    int OSS_DATA_ERROR = 0x80093005;
    int OSS_BAD_ARG = 0x80093006;
    int OSS_BAD_VERSION = 0x80093007;
    int OSS_OUT_MEMORY = 0x80093008;
    int OSS_PDU_MISMATCH = 0x80093009;
    int OSS_LIMITED = 0x8009300A;
    int OSS_BAD_PTR = 0x8009300B;
    int OSS_BAD_TIME = 0x8009300C;
    int OSS_INDEFINITE_NOT_SUPPORTED = 0x8009300D;
    int OSS_MEM_ERROR = 0x8009300E;
    int OSS_BAD_TABLE = 0x8009300F;
    int OSS_TOO_LONG = 0x80093010;
    int OSS_CONSTRAINT_VIOLATED = 0x80093011;
    int OSS_FATAL_ERROR = 0x80093012;
    int OSS_ACCESS_SERIALIZATION_ERROR = 0x80093013;
    int OSS_NULL_TBL = 0x80093014;
    int OSS_NULL_FCN = 0x80093015;
    int OSS_BAD_ENCRULES = 0x80093016;
    int OSS_UNAVAIL_ENCRULES = 0x80093017;
    int OSS_CANT_OPEN_TRACE_WINDOW = 0x80093018;
    int OSS_UNIMPLEMENTED = 0x80093019;
    int OSS_OID_DLL_NOT_LINKED = 0x8009301A;
    int OSS_CANT_OPEN_TRACE_FILE = 0x8009301B;
    int OSS_TRACE_FILE_ALREADY_OPEN = 0x8009301C;
    int OSS_TABLE_MISMATCH = 0x8009301D;
    int OSS_TYPE_NOT_SUPPORTED = 0x8009301E;
    int OSS_REAL_DLL_NOT_LINKED = 0x8009301F;
    int OSS_REAL_CODE_NOT_LINKED = 0x80093020;
    int OSS_OUT_OF_RANGE = 0x80093021;
    int OSS_COPIER_DLL_NOT_LINKED = 0x80093022;
    int OSS_CONSTRAINT_DLL_NOT_LINKED = 0x80093023;
    int OSS_COMPARATOR_DLL_NOT_LINKED = 0x80093024;
    int OSS_COMPARATOR_CODE_NOT_LINKED = 0x80093025;
    int OSS_MEM_MGR_DLL_NOT_LINKED = 0x80093026;
    int OSS_PDV_DLL_NOT_LINKED = 0x80093027;
    int OSS_PDV_CODE_NOT_LINKED = 0x80093028;
    int OSS_API_DLL_NOT_LINKED = 0x80093029;
    int OSS_BERDER_DLL_NOT_LINKED = 0x8009302A;
    int OSS_PER_DLL_NOT_LINKED = 0x8009302B;
    int OSS_OPEN_TYPE_ERROR = 0x8009302C;
    int OSS_MUTEX_NOT_CREATED = 0x8009302D;
    int OSS_CANT_CLOSE_TRACE_FILE = 0x8009302E;
    int CRYPT_E_ASN1_ERROR = 0x80093100;
    int CRYPT_E_ASN1_INTERNAL = 0x80093101;
    int CRYPT_E_ASN1_EOD = 0x80093102;
    int CRYPT_E_ASN1_CORRUPT = 0x80093103;
    int CRYPT_E_ASN1_LARGE = 0x80093104;
    int CRYPT_E_ASN1_CONSTRAINT = 0x80093105;
    int CRYPT_E_ASN1_MEMORY = 0x80093106;
    int CRYPT_E_ASN1_OVERFLOW = 0x80093107;
    int CRYPT_E_ASN1_BADPDU = 0x80093108;
    int CRYPT_E_ASN1_BADARGS = 0x80093109;
    int CRYPT_E_ASN1_BADREAL = 0x8009310A;
    int CRYPT_E_ASN1_BADTAG = 0x8009310B;
    int CRYPT_E_ASN1_CHOICE = 0x8009310C;
    int CRYPT_E_ASN1_RULE = 0x8009310D;
    int CRYPT_E_ASN1_UTF8 = 0x8009310E;
    int CRYPT_E_ASN1_PDU_TYPE = 0x80093133;
    int CRYPT_E_ASN1_NYI = 0x80093134;
    int CRYPT_E_ASN1_EXTENDED = 0x80093201;
    int CRYPT_E_ASN1_NOEOD = 0x80093202;
    int CERTSRV_E_BAD_REQUESTSUBJECT = 0x80094001;
    int CERTSRV_E_NO_REQUEST = 0x80094002;
    int CERTSRV_E_BAD_REQUESTSTATUS = 0x80094003;
    int CERTSRV_E_PROPERTY_EMPTY = 0x80094004;
    int CERTSRV_E_INVALID_CA_CERTIFICATE = 0x80094005;
    int CERTSRV_E_SERVER_SUSPENDED = 0x80094006;
    int CERTSRV_E_ENCODING_LENGTH = 0x80094007;
    int CERTSRV_E_ROLECONFLICT = 0x80094008;
    int CERTSRV_E_RESTRICTEDOFFICER = 0x80094009;
    int CERTSRV_E_KEY_ARCHIVAL_NOT_CONFIGURED = 0x8009400A;
    int CERTSRV_E_NO_VALID_KRA = 0x8009400B;
    int CERTSRV_E_BAD_REQUEST_KEY_ARCHIVAL = 0x8009400C;
    int CERTSRV_E_NO_CAADMIN_DEFINED = 0x8009400D;
    int CERTSRV_E_BAD_RENEWAL_CERT_ATTRIBUTE = 0x8009400E;
    int CERTSRV_E_NO_DB_SESSIONS = 0x8009400F;
    int CERTSRV_E_ALIGNMENT_FAULT = 0x80094010;
    int CERTSRV_E_ENROLL_DENIED = 0x80094011;
    int CERTSRV_E_TEMPLATE_DENIED = 0x80094012;
    int CERTSRV_E_DOWNLEVEL_DC_SSL_OR_UPGRADE = 0x80094013;
    int CERTSRV_E_UNSUPPORTED_CERT_TYPE = 0x80094800;
    int CERTSRV_E_NO_CERT_TYPE = 0x80094801;
    int CERTSRV_E_TEMPLATE_CONFLICT = 0x80094802;
    int CERTSRV_E_SUBJECT_ALT_NAME_REQUIRED = 0x80094803;
    int CERTSRV_E_ARCHIVED_KEY_REQUIRED = 0x80094804;
    int CERTSRV_E_SMIME_REQUIRED = 0x80094805;
    int CERTSRV_E_BAD_RENEWAL_SUBJECT = 0x80094806;
    int CERTSRV_E_BAD_TEMPLATE_VERSION = 0x80094807;
    int CERTSRV_E_TEMPLATE_POLICY_REQUIRED = 0x80094808;
    int CERTSRV_E_SIGNATURE_POLICY_REQUIRED = 0x80094809;
    int CERTSRV_E_SIGNATURE_COUNT = 0x8009480A;
    int CERTSRV_E_SIGNATURE_REJECTED = 0x8009480B;
    int CERTSRV_E_ISSUANCE_POLICY_REQUIRED = 0x8009480C;
    int CERTSRV_E_SUBJECT_UPN_REQUIRED = 0x8009480D;
    int CERTSRV_E_SUBJECT_DIRECTORY_GUID_REQUIRED = 0x8009480E;
    int CERTSRV_E_SUBJECT_DNS_REQUIRED = 0x8009480F;
    int CERTSRV_E_ARCHIVED_KEY_UNEXPECTED = 0x80094810;
    int CERTSRV_E_KEY_LENGTH = 0x80094811;
    int CERTSRV_E_SUBJECT_EMAIL_REQUIRED = 0x80094812;
    int CERTSRV_E_UNKNOWN_CERT_TYPE = 0x80094813;
    int CERTSRV_E_CERT_TYPE_OVERLAP = 0x80094814;
    int CERTSRV_E_TOO_MANY_SIGNATURES = 0x80094815;
    int XENROLL_E_KEY_NOT_EXPORTABLE = 0x80095000;
    int XENROLL_E_CANNOT_ADD_ROOT_CERT = 0x80095001;
    int XENROLL_E_RESPONSE_KA_HASH_NOT_FOUND = 0x80095002;
    int XENROLL_E_RESPONSE_UNEXPECTED_KA_HASH = 0x80095003;
    int XENROLL_E_RESPONSE_KA_HASH_MISMATCH = 0x80095004;
    int XENROLL_E_KEYSPEC_SMIME_MISMATCH = 0x80095005;
    int TRUST_E_SYSTEM_ERROR = 0x80096001;
    int TRUST_E_NO_SIGNER_CERT = 0x80096002;
    int TRUST_E_COUNTER_SIGNER = 0x80096003;
    int TRUST_E_CERT_SIGNATURE = 0x80096004;
    int TRUST_E_TIME_STAMP = 0x80096005;
    int TRUST_E_BAD_DIGEST = 0x80096010;
    int TRUST_E_BASIC_CONSTRAINTS = 0x80096019;
    int TRUST_E_FINANCIAL_CRITERIA = 0x8009601E;
    int MSSIPOTF_E_OUTOFMEMRANGE = 0x80097001;
    int MSSIPOTF_E_CANTGETOBJECT = 0x80097002;
    int MSSIPOTF_E_NOHEADTABLE = 0x80097003;
    int MSSIPOTF_E_BAD_MAGICNUMBER = 0x80097004;
    int MSSIPOTF_E_BAD_OFFSET_TABLE = 0x80097005;
    int MSSIPOTF_E_TABLE_TAGORDER = 0x80097006;
    int MSSIPOTF_E_TABLE_LONGWORD = 0x80097007;
    int MSSIPOTF_E_BAD_FIRST_TABLE_PLACEMENT = 0x80097008;
    int MSSIPOTF_E_TABLES_OVERLAP = 0x80097009;
    int MSSIPOTF_E_TABLE_PADBYTES = 0x8009700A;
    int MSSIPOTF_E_FILETOOSMALL = 0x8009700B;
    int MSSIPOTF_E_TABLE_CHECKSUM = 0x8009700C;
    int MSSIPOTF_E_FILE_CHECKSUM = 0x8009700D;
    int MSSIPOTF_E_FAILED_POLICY = 0x80097010;
    int MSSIPOTF_E_FAILED_HINTS_CHECK = 0x80097011;
    int MSSIPOTF_E_NOT_OPENTYPE = 0x80097012;
    int MSSIPOTF_E_FILE = 0x80097013;
    int MSSIPOTF_E_CRYPT = 0x80097014;
    int MSSIPOTF_E_BADVERSION = 0x80097015;
    int MSSIPOTF_E_DSIG_STRUCTURE = 0x80097016;
    int MSSIPOTF_E_PCONST_CHECK = 0x80097017;
    int MSSIPOTF_E_STRUCTURE = 0x80097018;
    int ERROR_CRED_REQUIRES_CONFIRMATION = 0x80097019;
    int NTE_OP_OK = 0;
    int TRUST_E_PROVIDER_UNKNOWN = 0x800B0001;
    int TRUST_E_ACTION_UNKNOWN = 0x800B0002;
    int TRUST_E_SUBJECT_FORM_UNKNOWN = 0x800B0003;
    int TRUST_E_SUBJECT_NOT_TRUSTED = 0x800B0004;
    int DIGSIG_E_ENCODE = 0x800B0005;
    int DIGSIG_E_DECODE = 0x800B0006;
    int DIGSIG_E_EXTENSIBILITY = 0x800B0007;
    int DIGSIG_E_CRYPTO = 0x800B0008;
    int PERSIST_E_SIZEDEFINITE = 0x800B0009;
    int PERSIST_E_SIZEINDEFINITE = 0x800B000A;
    int PERSIST_E_NOTSELFSIZING = 0x800B000B;
    int TRUST_E_NOSIGNATURE = 0x800B0100;
    int CERT_E_EXPIRED = 0x800B0101;
    int CERT_E_VALIDITYPERIODNESTING = 0x800B0102;
    int CERT_E_ROLE = 0x800B0103;
    int CERT_E_PATHLENCONST = 0x800B0104;
    int CERT_E_CRITICAL = 0x800B0105;
    int CERT_E_PURPOSE = 0x800B0106;
    int CERT_E_ISSUERCHAINING = 0x800B0107;
    int CERT_E_MALFORMED = 0x800B0108;
    int CERT_E_UNTRUSTEDROOT = 0x800B0109;
    int CERT_E_CHAINING = 0x800B010A;
    int TRUST_E_FAIL = 0x800B010B;
    int CERT_E_REVOKED = 0x800B010C;
    int CERT_E_UNTRUSTEDTESTROOT = 0x800B010D;
    int CERT_E_REVOCATION_FAILURE = 0x800B010E;
    int CERT_E_CN_NO_MATCH = 0x800B010F;
    int CERT_E_WRONG_USAGE = 0x800B0110;
    int TRUST_E_EXPLICIT_DISTRUST = 0x800B0111;
    int CERT_E_UNTRUSTEDCA = 0x800B0112;
    int CERT_E_INVALID_POLICY = 0x800B0113;
    int CERT_E_INVALID_NAME = 0x800B0114;
    int SPAPI_E_EXPECTED_SECTION_NAME = 0x800F0000;
    int SPAPI_E_BAD_SECTION_NAME_LINE = 0x800F0001;
    int SPAPI_E_SECTION_NAME_TOO_LONG = 0x800F0002;
    int SPAPI_E_GENERAL_SYNTAX = 0x800F0003;
    int SPAPI_E_WRONG_INF_STYLE = 0x800F0100;
    int SPAPI_E_SECTION_NOT_FOUND = 0x800F0101;
    int SPAPI_E_LINE_NOT_FOUND = 0x800F0102;
    int SPAPI_E_NO_BACKUP = 0x800F0103;
    int SPAPI_E_NO_ASSOCIATED_CLASS = 0x800F0200;
    int SPAPI_E_CLASS_MISMATCH = 0x800F0201;
    int SPAPI_E_DUPLICATE_FOUND = 0x800F0202;
    int SPAPI_E_NO_DRIVER_SELECTED = 0x800F0203;
    int SPAPI_E_KEY_DOES_NOT_EXIST = 0x800F0204;
    int SPAPI_E_INVALID_DEVINST_NAME = 0x800F0205;
    int SPAPI_E_INVALID_CLASS = 0x800F0206;
    int SPAPI_E_DEVINST_ALREADY_EXISTS = 0x800F0207;
    int SPAPI_E_DEVINFO_NOT_REGISTERED = 0x800F0208;
    int SPAPI_E_INVALID_REG_PROPERTY = 0x800F0209;
    int SPAPI_E_NO_INF = 0x800F020A;
    int SPAPI_E_NO_SUCH_DEVINST = 0x800F020B;
    int SPAPI_E_CANT_LOAD_CLASS_ICON = 0x800F020C;
    int SPAPI_E_INVALID_CLASS_INSTALLER = 0x800F020D;
    int SPAPI_E_DI_DO_DEFAULT = 0x800F020E;
    int SPAPI_E_DI_NOFILECOPY = 0x800F020F;
    int SPAPI_E_INVALID_HWPROFILE = 0x800F0210;
    int SPAPI_E_NO_DEVICE_SELECTED = 0x800F0211;
    int SPAPI_E_DEVINFO_LIST_LOCKED = 0x800F0212;
    int SPAPI_E_DEVINFO_DATA_LOCKED = 0x800F0213;
    int SPAPI_E_DI_BAD_PATH = 0x800F0214;
    int SPAPI_E_NO_CLASSINSTALL_PARAMS = 0x800F0215;
    int SPAPI_E_FILEQUEUE_LOCKED = 0x800F0216;
    int SPAPI_E_BAD_SERVICE_INSTALLSECT = 0x800F0217;
    int SPAPI_E_NO_CLASS_DRIVER_LIST = 0x800F0218;
    int SPAPI_E_NO_ASSOCIATED_SERVICE = 0x800F0219;
    int SPAPI_E_NO_DEFAULT_DEVICE_INTERFACE = 0x800F021A;
    int SPAPI_E_DEVICE_INTERFACE_ACTIVE = 0x800F021B;
    int SPAPI_E_DEVICE_INTERFACE_REMOVED = 0x800F021C;
    int SPAPI_E_BAD_INTERFACE_INSTALLSECT = 0x800F021D;
    int SPAPI_E_NO_SUCH_INTERFACE_CLASS = 0x800F021E;
    int SPAPI_E_INVALID_REFERENCE_STRING = 0x800F021F;
    int SPAPI_E_INVALID_MACHINENAME = 0x800F0220;
    int SPAPI_E_REMOTE_COMM_FAILURE = 0x800F0221;
    int SPAPI_E_MACHINE_UNAVAILABLE = 0x800F0222;
    int SPAPI_E_NO_CONFIGMGR_SERVICES = 0x800F0223;
    int SPAPI_E_INVALID_PROPPAGE_PROVIDER = 0x800F0224;
    int SPAPI_E_NO_SUCH_DEVICE_INTERFACE = 0x800F0225;
    int SPAPI_E_DI_POSTPROCESSING_REQUIRED = 0x800F0226;
    int SPAPI_E_INVALID_COINSTALLER = 0x800F0227;
    int SPAPI_E_NO_COMPAT_DRIVERS = 0x800F0228;
    int SPAPI_E_NO_DEVICE_ICON = 0x800F0229;
    int SPAPI_E_INVALID_INF_LOGCONFIG = 0x800F022A;
    int SPAPI_E_DI_DONT_INSTALL = 0x800F022B;
    int SPAPI_E_INVALID_FILTER_DRIVER = 0x800F022C;
    int SPAPI_E_NON_WINDOWS_NT_DRIVER = 0x800F022D;
    int SPAPI_E_NON_WINDOWS_DRIVER = 0x800F022E;
    int SPAPI_E_NO_CATALOG_FOR_OEM_INF = 0x800F022F;
    int SPAPI_E_DEVINSTALL_QUEUE_NONNATIVE = 0x800F0230;
    int SPAPI_E_NOT_DISABLEABLE = 0x800F0231;
    int SPAPI_E_CANT_REMOVE_DEVINST = 0x800F0232;
    int SPAPI_E_INVALID_TARGET = 0x800F0233;
    int SPAPI_E_DRIVER_NONNATIVE = 0x800F0234;
    int SPAPI_E_IN_WOW64 = 0x800F0235;
    int SPAPI_E_SET_SYSTEM_RESTORE_POINT = 0x800F0236;
    int SPAPI_E_INCORRECTLY_COPIED_INF = 0x800F0237;
    int SPAPI_E_SCE_DISABLED = 0x800F0238;
    int SPAPI_E_UNKNOWN_EXCEPTION = 0x800F0239;
    int SPAPI_E_PNP_REGISTRY_ERROR = 0x800F023A;
    int SPAPI_E_REMOTE_REQUEST_UNSUPPORTED = 0x800F023B;
    int SPAPI_E_NOT_AN_INSTALLED_OEM_INF = 0x800F023C;
    int SPAPI_E_INF_IN_USE_BY_DEVICES = 0x800F023D;
    int SPAPI_E_DI_FUNCTION_OBSOLETE = 0x800F023E;
    int SPAPI_E_NO_AUTHENTICODE_CATALOG = 0x800F023F;
    int SPAPI_E_AUTHENTICODE_DISALLOWED = 0x800F0240;
    int SPAPI_E_AUTHENTICODE_TRUSTED_PUBLISHER = 0x800F0241;
    int SPAPI_E_AUTHENTICODE_TRUST_NOT_ESTABLISHED = 0x800F0242;
    int SPAPI_E_AUTHENTICODE_PUBLISHER_NOT_TRUSTED = 0x800F0243;
    int SPAPI_E_SIGNATURE_OSATTRIBUTE_MISMATCH = 0x800F0244;
    int SPAPI_E_ONLY_VALIDATE_VIA_AUTHENTICODE = 0x800F0245;
    int SPAPI_E_DEVICE_INSTALLER_NOT_READY = 0x800F0246;
    int SPAPI_E_DRIVER_STORE_ADD_FAILED = 0x800F0247;
    int SPAPI_E_DEVICE_INSTALL_BLOCKED = 0x800F0248;
    int SPAPI_E_DRIVER_INSTALL_BLOCKED = 0x800F0249;
    int SPAPI_E_WRONG_INF_TYPE = 0x800F024A;
    int SPAPI_E_FILE_HASH_NOT_IN_CATALOG = 0x800F024B;
    int SPAPI_E_DRIVER_STORE_DELETE_FAILED = 0x800F024C;
    int SPAPI_E_UNRECOVERABLE_STACK_OVERFLOW = 0x800F0300;
    int SPAPI_E_ERROR_NOT_INSTALLED = 0x800F1000;
    int SCARD_S_SUCCESS = NO_ERROR;
    int SCARD_F_INTERNAL_ERROR = 0x80100001;
    int SCARD_E_CANCELLED = 0x80100002;
    int SCARD_E_INVALID_HANDLE = 0x80100003;
    int SCARD_E_INVALID_PARAMETER = 0x80100004;
    int SCARD_E_INVALID_TARGET = 0x80100005;
    int SCARD_E_NO_MEMORY = 0x80100006;
    int SCARD_F_WAITED_TOO_LONG = 0x80100007;
    int SCARD_E_INSUFFICIENT_BUFFER = 0x80100008;
    int SCARD_E_UNKNOWN_READER = 0x80100009;
    int SCARD_E_TIMEOUT = 0x8010000A;
    int SCARD_E_SHARING_VIOLATION = 0x8010000B;
    int SCARD_E_NO_SMARTCARD = 0x8010000C;
    int SCARD_E_UNKNOWN_CARD = 0x8010000D;
    int SCARD_E_CANT_DISPOSE = 0x8010000E;
    int SCARD_E_PROTO_MISMATCH = 0x8010000F;
    int SCARD_E_NOT_READY = 0x80100010;
    int SCARD_E_INVALID_VALUE = 0x80100011;
    int SCARD_E_SYSTEM_CANCELLED = 0x80100012;
    int SCARD_F_COMM_ERROR = 0x80100013;
    int SCARD_F_UNKNOWN_ERROR = 0x80100014;
    int SCARD_E_INVALID_ATR = 0x80100015;
    int SCARD_E_NOT_TRANSACTED = 0x80100016;
    int SCARD_E_READER_UNAVAILABLE = 0x80100017;
    int SCARD_P_SHUTDOWN = 0x80100018;
    int SCARD_E_PCI_TOO_SMALL = 0x80100019;
    int SCARD_E_READER_UNSUPPORTED = 0x8010001A;
    int SCARD_E_DUPLICATE_READER = 0x8010001B;
    int SCARD_E_CARD_UNSUPPORTED = 0x8010001C;
    int SCARD_E_NO_SERVICE = 0x8010001D;
    int SCARD_E_SERVICE_STOPPED = 0x8010001E;
    int SCARD_E_UNEXPECTED = 0x8010001F;
    int SCARD_E_ICC_INSTALLATION = 0x80100020;
    int SCARD_E_ICC_CREATEORDER = 0x80100021;
    int SCARD_E_UNSUPPORTED_FEATURE = 0x80100022;
    int SCARD_E_DIR_NOT_FOUND = 0x80100023;
    int SCARD_E_FILE_NOT_FOUND = 0x80100024;
    int SCARD_E_NO_DIR = 0x80100025;
    int SCARD_E_NO_FILE = 0x80100026;
    int SCARD_E_NO_ACCESS = 0x80100027;
    int SCARD_E_WRITE_TOO_MANY = 0x80100028;
    int SCARD_E_BAD_SEEK = 0x80100029;
    int SCARD_E_INVALID_CHV = 0x8010002A;
    int SCARD_E_UNKNOWN_RES_MNG = 0x8010002B;
    int SCARD_E_NO_SUCH_CERTIFICATE = 0x8010002C;
    int SCARD_E_CERTIFICATE_UNAVAILABLE = 0x8010002D;
    int SCARD_E_NO_READERS_AVAILABLE = 0x8010002E;
    int SCARD_E_COMM_DATA_LOST = 0x8010002F;
    int SCARD_E_NO_KEY_CONTAINER = 0x80100030;
    int SCARD_E_SERVER_TOO_BUSY = 0x80100031;
    int SCARD_W_UNSUPPORTED_CARD = 0x80100065;
    int SCARD_W_UNRESPONSIVE_CARD = 0x80100066;
    int SCARD_W_UNPOWERED_CARD = 0x80100067;
    int SCARD_W_RESET_CARD = 0x80100068;
    int SCARD_W_REMOVED_CARD = 0x80100069;
    int SCARD_W_SECURITY_VIOLATION = 0x8010006A;
    int SCARD_W_WRONG_CHV = 0x8010006B;
    int SCARD_W_CHV_BLOCKED = 0x8010006C;
    int SCARD_W_EOF = 0x8010006D;
    int SCARD_W_CANCELLED_BY_USER = 0x8010006E;
    int SCARD_W_CARD_NOT_AUTHENTICATED = 0x8010006F;
    int SCARD_W_CACHE_ITEM_NOT_FOUND = 0x80100070;
    int SCARD_W_CACHE_ITEM_STALE = 0x80100071;
    int SCARD_W_CACHE_ITEM_TOO_BIG = 0x80100072;
    int COMADMIN_E_OBJECTERRORS = 0x80110401;
    int COMADMIN_E_OBJECTINVALID = 0x80110402;
    int COMADMIN_E_KEYMISSING = 0x80110403;
    int COMADMIN_E_ALREADYINSTALLED = 0x80110404;
    int COMADMIN_E_APP_FILE_WRITEFAIL = 0x80110407;
    int COMADMIN_E_APP_FILE_READFAIL = 0x80110408;
    int COMADMIN_E_APP_FILE_VERSION = 0x80110409;
    int COMADMIN_E_BADPATH = 0x8011040A;
    int COMADMIN_E_APPLICATIONEXISTS = 0x8011040B;
    int COMADMIN_E_ROLEEXISTS = 0x8011040C;
    int COMADMIN_E_CANTCOPYFILE = 0x8011040D;
    int COMADMIN_E_NOUSER = 0x8011040F;
    int COMADMIN_E_INVALIDUSERIDS = 0x80110410;
    int COMADMIN_E_NOREGISTRYCLSID = 0x80110411;
    int COMADMIN_E_BADREGISTRYPROGID = 0x80110412;
    int COMADMIN_E_AUTHENTICATIONLEVEL = 0x80110413;
    int COMADMIN_E_USERPASSWDNOTVALID = 0x80110414;
    int COMADMIN_E_CLSIDORIIDMISMATCH = 0x80110418;
    int COMADMIN_E_REMOTEINTERFACE = 0x80110419;
    int COMADMIN_E_DLLREGISTERSERVER = 0x8011041A;
    int COMADMIN_E_NOSERVERSHARE = 0x8011041B;
    int COMADMIN_E_DLLLOADFAILED = 0x8011041D;
    int COMADMIN_E_BADREGISTRYLIBID = 0x8011041E;
    int COMADMIN_E_APPDIRNOTFOUND = 0x8011041F;
    int COMADMIN_E_REGISTRARFAILED = 0x80110423;
    int COMADMIN_E_COMPFILE_DOESNOTEXIST = 0x80110424;
    int COMADMIN_E_COMPFILE_LOADDLLFAIL = 0x80110425;
    int COMADMIN_E_COMPFILE_GETCLASSOBJ = 0x80110426;
    int COMADMIN_E_COMPFILE_CLASSNOTAVAIL = 0x80110427;
    int COMADMIN_E_COMPFILE_BADTLB = 0x80110428;
    int COMADMIN_E_COMPFILE_NOTINSTALLABLE = 0x80110429;
    int COMADMIN_E_NOTCHANGEABLE = 0x8011042A;
    int COMADMIN_E_NOTDELETEABLE = 0x8011042B;
    int COMADMIN_E_SESSION = 0x8011042C;
    int COMADMIN_E_COMP_MOVE_LOCKED = 0x8011042D;
    int COMADMIN_E_COMP_MOVE_BAD_DEST = 0x8011042E;
    int COMADMIN_E_REGISTERTLB = 0x80110430;
    int COMADMIN_E_SYSTEMAPP = 0x80110433;
    int COMADMIN_E_COMPFILE_NOREGISTRAR = 0x80110434;
    int COMADMIN_E_COREQCOMPINSTALLED = 0x80110435;
    int COMADMIN_E_SERVICENOTINSTALLED = 0x80110436;
    int COMADMIN_E_PROPERTYSAVEFAILED = 0x80110437;
    int COMADMIN_E_OBJECTEXISTS = 0x80110438;
    int COMADMIN_E_COMPONENTEXISTS = 0x80110439;
    int COMADMIN_E_REGFILE_CORRUPT = 0x8011043B;
    int COMADMIN_E_PROPERTY_OVERFLOW = 0x8011043C;
    int COMADMIN_E_NOTINREGISTRY = 0x8011043E;
    int COMADMIN_E_OBJECTNOTPOOLABLE = 0x8011043F;
    int COMADMIN_E_APPLID_MATCHES_CLSID = 0x80110446;
    int COMADMIN_E_ROLE_DOES_NOT_EXIST = 0x80110447;
    int COMADMIN_E_START_APP_NEEDS_COMPONENTS = 0x80110448;
    int COMADMIN_E_REQUIRES_DIFFERENT_PLATFORM = 0x80110449;
    int COMADMIN_E_CAN_NOT_EXPORT_APP_PROXY = 0x8011044A;
    int COMADMIN_E_CAN_NOT_START_APP = 0x8011044B;
    int COMADMIN_E_CAN_NOT_EXPORT_SYS_APP = 0x8011044C;
    int COMADMIN_E_CANT_SUBSCRIBE_TO_COMPONENT = 0x8011044D;
    int COMADMIN_E_EVENTCLASS_CANT_BE_SUBSCRIBER = 0x8011044E;
    int COMADMIN_E_LIB_APP_PROXY_INCOMPATIBLE = 0x8011044F;
    int COMADMIN_E_BASE_PARTITION_ONLY = 0x80110450;
    int COMADMIN_E_START_APP_DISABLED = 0x80110451;
    int COMADMIN_E_CAT_DUPLICATE_PARTITION_NAME = 0x80110457;
    int COMADMIN_E_CAT_INVALID_PARTITION_NAME = 0x80110458;
    int COMADMIN_E_CAT_PARTITION_IN_USE = 0x80110459;
    int COMADMIN_E_FILE_PARTITION_DUPLICATE_FILES = 0x8011045A;
    int COMADMIN_E_CAT_IMPORTED_COMPONENTS_NOT_ALLOWED = 0x8011045B;
    int COMADMIN_E_AMBIGUOUS_APPLICATION_NAME = 0x8011045C;
    int COMADMIN_E_AMBIGUOUS_PARTITION_NAME = 0x8011045D;
    int COMADMIN_E_REGDB_NOTINITIALIZED = 0x80110472;
    int COMADMIN_E_REGDB_NOTOPEN = 0x80110473;
    int COMADMIN_E_REGDB_SYSTEMERR = 0x80110474;
    int COMADMIN_E_REGDB_ALREADYRUNNING = 0x80110475;
    int COMADMIN_E_MIG_VERSIONNOTSUPPORTED = 0x80110480;
    int COMADMIN_E_MIG_SCHEMANOTFOUND = 0x80110481;
    int COMADMIN_E_CAT_BITNESSMISMATCH = 0x80110482;
    int COMADMIN_E_CAT_UNACCEPTABLEBITNESS = 0x80110483;
    int COMADMIN_E_CAT_WRONGAPPBITNESS = 0x80110484;
    int COMADMIN_E_CAT_PAUSE_RESUME_NOT_SUPPORTED = 0x80110485;
    int COMADMIN_E_CAT_SERVERFAULT = 0x80110486;
    int COMQC_E_APPLICATION_NOT_QUEUED = 0x80110600;
    int COMQC_E_NO_QUEUEABLE_INTERFACES = 0x80110601;
    int COMQC_E_QUEUING_SERVICE_NOT_AVAILABLE = 0x80110602;
    int COMQC_E_NO_IPERSISTSTREAM = 0x80110603;
    int COMQC_E_BAD_MESSAGE = 0x80110604;
    int COMQC_E_UNAUTHENTICATED = 0x80110605;
    int COMQC_E_UNTRUSTED_ENQUEUER = 0x80110606;
    int MSDTC_E_DUPLICATE_RESOURCE = 0x80110701;
    int COMADMIN_E_OBJECT_PARENT_MISSING = 0x80110808;
    int COMADMIN_E_OBJECT_DOES_NOT_EXIST = 0x80110809;
    int COMADMIN_E_APP_NOT_RUNNING = 0x8011080A;
    int COMADMIN_E_INVALID_PARTITION = 0x8011080B;
    int COMADMIN_E_SVCAPP_NOT_POOLABLE_OR_RECYCLABLE = 0x8011080D;
    int COMADMIN_E_USER_IN_SET = 0x8011080E;
    int COMADMIN_E_CANTRECYCLELIBRARYAPPS = 0x8011080F;
    int COMADMIN_E_CANTRECYCLESERVICEAPPS = 0x80110811;
    int COMADMIN_E_PROCESSALREADYRECYCLED = 0x80110812;
    int COMADMIN_E_PAUSEDPROCESSMAYNOTBERECYCLED = 0x80110813;
    int COMADMIN_E_CANTMAKEINPROCSERVICE = 0x80110814;
    int COMADMIN_E_PROGIDINUSEBYCLSID = 0x80110815;
    int COMADMIN_E_DEFAULT_PARTITION_NOT_IN_SET = 0x80110816;
    int COMADMIN_E_RECYCLEDPROCESSMAYNOTBEPAUSED = 0x80110817;
    int COMADMIN_E_PARTITION_ACCESSDENIED = 0x80110818;
    int COMADMIN_E_PARTITION_MSI_ONLY = 0x80110819;
    int COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_1_0_FORMAT = 0x8011081A;
    int COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_NONBASE_PARTITIONS = 0x8011081B;
    int COMADMIN_E_COMP_MOVE_SOURCE = 0x8011081C;
    int COMADMIN_E_COMP_MOVE_DEST = 0x8011081D;
    int COMADMIN_E_COMP_MOVE_PRIVATE = 0x8011081E;
    int COMADMIN_E_BASEPARTITION_REQUIRED_IN_SET = 0x8011081F;
    int COMADMIN_E_CANNOT_ALIAS_EVENTCLASS = 0x80110820;
    int COMADMIN_E_PRIVATE_ACCESSDENIED = 0x80110821;
    int COMADMIN_E_SAFERINVALID = 0x80110822;
    int COMADMIN_E_REGISTRY_ACCESSDENIED = 0x80110823;
    int COMADMIN_E_PARTITIONS_DISABLED = 0x80110824;
    int ERROR_FLT_IO_COMPLETE = 0x001F0001;
    int ERROR_FLT_NO_HANDLER_DEFINED = 0x801F0001;
    int ERROR_FLT_CONTEXT_ALREADY_DEFINED = 0x801F0002;
    int ERROR_FLT_INVALID_ASYNCHRONOUS_REQUEST = 0x801F0003;
    int ERROR_FLT_DISALLOW_FAST_IO = 0x801F0004;
    int ERROR_FLT_INVALID_NAME_REQUEST = 0x801F0005;
    int ERROR_FLT_NOT_SAFE_TO_POST_OPERATION = 0x801F0006;
    int ERROR_FLT_NOT_INITIALIZED = 0x801F0007;
    int ERROR_FLT_FILTER_NOT_READY = 0x801F0008;
    int ERROR_FLT_POST_OPERATION_CLEANUP = 0x801F0009;
    int ERROR_FLT_INTERNAL_ERROR = 0x801F000A;
    int ERROR_FLT_DELETING_OBJECT = 0x801F000B;
    int ERROR_FLT_MUST_BE_NONPAGED_POOL = 0x801F000C;
    int ERROR_FLT_DUPLICATE_ENTRY = 0x801F000D;
    int ERROR_FLT_CBDQ_DISABLED = 0x801F000E;
    int ERROR_FLT_DO_NOT_ATTACH = 0x801F000F;
    int ERROR_FLT_DO_NOT_DETACH = 0x801F0010;
    int ERROR_FLT_INSTANCE_ALTITUDE_COLLISION = 0x801F0011;
    int ERROR_FLT_INSTANCE_NAME_COLLISION = 0x801F0012;
    int ERROR_FLT_FILTER_NOT_FOUND = 0x801F0013;
    int ERROR_FLT_VOLUME_NOT_FOUND = 0x801F0014;
    int ERROR_FLT_INSTANCE_NOT_FOUND = 0x801F0015;
    int ERROR_FLT_CONTEXT_ALLOCATION_NOT_FOUND = 0x801F0016;
    int ERROR_FLT_INVALID_CONTEXT_REGISTRATION = 0x801F0017;
    int ERROR_FLT_NAME_CACHE_MISS = 0x801F0018;
    int ERROR_FLT_NO_DEVICE_OBJECT = 0x801F0019;
    int ERROR_FLT_VOLUME_ALREADY_MOUNTED = 0x801F001A;
    int ERROR_FLT_ALREADY_ENLISTED = 0x801F001B;
    int ERROR_FLT_CONTEXT_ALREADY_LINKED = 0x801F001C;
    int ERROR_FLT_NO_WAITER_FOR_REPLY = 0x801F0020;
    int ERROR_HUNG_DISPLAY_DRIVER_THREAD = 0x80260001;
    int DWM_E_COMPOSITIONDISABLED = 0x80263001;
    int DWM_E_REMOTING_NOT_SUPPORTED = 0x80263002;
    int DWM_E_NO_REDIRECTION_SURFACE_AVAILABLE = 0x80263003;
    int DWM_E_NOT_QUEUING_PRESENTS = 0x80263004;
    int ERROR_MONITOR_NO_DESCRIPTOR = 0x80261001;
    int ERROR_MONITOR_UNKNOWN_DESCRIPTOR_FORMAT = 0x80261002;
    int ERROR_MONITOR_INVALID_DESCRIPTOR_CHECKSUM = 0xC0261003;
    int ERROR_MONITOR_INVALID_STANDARD_TIMING_BLOCK = 0xC0261004;
    int ERROR_MONITOR_WMI_DATABLOCK_REGISTRATION_FAILED = 0xC0261005;
    int ERROR_MONITOR_INVALID_SERIAL_NUMBER_MONDSC_BLOCK = 0xC0261006;
    int ERROR_MONITOR_INVALID_USER_FRIENDLY_MONDSC_BLOCK = 0xC0261007;
    int ERROR_MONITOR_NO_MORE_DESCRIPTOR_DATA = 0xC0261008;
    int ERROR_MONITOR_INVALID_DETAILED_TIMING_BLOCK = 0xC0261009;
    int ERROR_GRAPHICS_NOT_EXCLUSIVE_MODE_OWNER = 0xC0262000;
    int ERROR_GRAPHICS_INSUFFICIENT_DMA_BUFFER = 0xC0262001;
    int ERROR_GRAPHICS_INVALID_DISPLAY_ADAPTER = 0xC0262002;
    int ERROR_GRAPHICS_ADAPTER_WAS_RESET = 0xC0262003;
    int ERROR_GRAPHICS_INVALID_DRIVER_MODEL = 0xC0262004;
    int ERROR_GRAPHICS_PRESENT_MODE_CHANGED = 0xC0262005;
    int ERROR_GRAPHICS_PRESENT_OCCLUDED = 0xC0262006;
    int ERROR_GRAPHICS_PRESENT_DENIED = 0xC0262007;
    int ERROR_GRAPHICS_CANNOTCOLORCONVERT = 0xC0262008;
    int ERROR_GRAPHICS_DRIVER_MISMATCH = 0xC0262009;
    int ERROR_GRAPHICS_PARTIAL_DATA_POPULATED = 0x4026200A;
    int ERROR_GRAPHICS_NO_VIDEO_MEMORY = 0xC0262100;
    int ERROR_GRAPHICS_CANT_LOCK_MEMORY = 0xC0262101;
    int ERROR_GRAPHICS_ALLOCATION_BUSY = 0xC0262102;
    int ERROR_GRAPHICS_TOO_MANY_REFERENCES = 0xC0262103;
    int ERROR_GRAPHICS_TRY_AGAIN_LATER = 0xC0262104;
    int ERROR_GRAPHICS_TRY_AGAIN_NOW = 0xC0262105;
    int ERROR_GRAPHICS_ALLOCATION_INVALID = 0xC0262106;
    int ERROR_GRAPHICS_UNSWIZZLING_APERTURE_UNAVAILABLE = 0xC0262107;
    int ERROR_GRAPHICS_UNSWIZZLING_APERTURE_UNSUPPORTED = 0xC0262108;
    int ERROR_GRAPHICS_CANT_EVICT_PINNED_ALLOCATION = 0xC0262109;
    int ERROR_GRAPHICS_INVALID_ALLOCATION_USAGE = 0xC0262110;
    int ERROR_GRAPHICS_CANT_RENDER_LOCKED_ALLOCATION = 0xC0262111;
    int ERROR_GRAPHICS_ALLOCATION_CLOSED = 0xC0262112;
    int ERROR_GRAPHICS_INVALID_ALLOCATION_INSTANCE = 0xC0262113;
    int ERROR_GRAPHICS_INVALID_ALLOCATION_HANDLE = 0xC0262114;
    int ERROR_GRAPHICS_WRONG_ALLOCATION_DEVICE = 0xC0262115;
    int ERROR_GRAPHICS_ALLOCATION_CONTENT_LOST = 0xC0262116;
    int ERROR_GRAPHICS_GPU_EXCEPTION_ON_DEVICE = 0xC0262200;
    int ERROR_GRAPHICS_INVALID_VIDPN_TOPOLOGY = 0xC0262300;
    int ERROR_GRAPHICS_VIDPN_TOPOLOGY_NOT_SUPPORTED = 0xC0262301;
    int ERROR_GRAPHICS_VIDPN_TOPOLOGY_CURRENTLY_NOT_SUPPORTED = 0xC0262302;
    int ERROR_GRAPHICS_INVALID_VIDPN = 0xC0262303;
    int ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE = 0xC0262304;
    int ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_TARGET = 0xC0262305;
    int ERROR_GRAPHICS_VIDPN_MODALITY_NOT_SUPPORTED = 0xC0262306;
    int ERROR_GRAPHICS_MODE_NOT_PINNED = 0x00262307;
    int ERROR_GRAPHICS_INVALID_VIDPN_SOURCEMODESET = 0xC0262308;
    int ERROR_GRAPHICS_INVALID_VIDPN_TARGETMODESET = 0xC0262309;
    int ERROR_GRAPHICS_INVALID_FREQUENCY = 0xC026230A;
    int ERROR_GRAPHICS_INVALID_ACTIVE_REGION = 0xC026230B;
    int ERROR_GRAPHICS_INVALID_TOTAL_REGION = 0xC026230C;
    int ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE_MODE = 0xC0262310;
    int ERROR_GRAPHICS_INVALID_VIDEO_PRESENT_TARGET_MODE = 0xC0262311;
    int ERROR_GRAPHICS_PINNED_MODE_MUST_REMAIN_IN_SET = 0xC0262312;
    int ERROR_GRAPHICS_PATH_ALREADY_IN_TOPOLOGY = 0xC0262313;
    int ERROR_GRAPHICS_MODE_ALREADY_IN_MODESET = 0xC0262314;
    int ERROR_GRAPHICS_INVALID_VIDEOPRESENTSOURCESET = 0xC0262315;
    int ERROR_GRAPHICS_INVALID_VIDEOPRESENTTARGETSET = 0xC0262316;
    int ERROR_GRAPHICS_SOURCE_ALREADY_IN_SET = 0xC0262317;
    int ERROR_GRAPHICS_TARGET_ALREADY_IN_SET = 0xC0262318;
    int ERROR_GRAPHICS_INVALID_VIDPN_PRESENT_PATH = 0xC0262319;
    int ERROR_GRAPHICS_NO_RECOMMENDED_VIDPN_TOPOLOGY = 0xC026231A;
    int ERROR_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGESET = 0xC026231B;
    int ERROR_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGE = 0xC026231C;
    int ERROR_GRAPHICS_FREQUENCYRANGE_NOT_IN_SET = 0xC026231D;
    int ERROR_GRAPHICS_NO_PREFERRED_MODE = 0x0026231E;
    int ERROR_GRAPHICS_FREQUENCYRANGE_ALREADY_IN_SET = 0xC026231F;
    int ERROR_GRAPHICS_STALE_MODESET = 0xC0262320;
    int ERROR_GRAPHICS_INVALID_MONITOR_SOURCEMODESET = 0xC0262321;
    int ERROR_GRAPHICS_INVALID_MONITOR_SOURCE_MODE = 0xC0262322;
    int ERROR_GRAPHICS_NO_RECOMMENDED_FUNCTIONAL_VIDPN = 0xC0262323;
    int ERROR_GRAPHICS_MODE_ID_MUST_BE_UNIQUE = 0xC0262324;
    int ERROR_GRAPHICS_EMPTY_ADAPTER_MONITOR_MODE_SUPPORT_INTERSECTION = 0xC0262325;
    int ERROR_GRAPHICS_VIDEO_PRESENT_TARGETS_LESS_THAN_SOURCES = 0xC0262326;
    int ERROR_GRAPHICS_PATH_NOT_IN_TOPOLOGY = 0xC0262327;
    int ERROR_GRAPHICS_ADAPTER_MUST_HAVE_AT_LEAST_ONE_SOURCE = 0xC0262328;
    int ERROR_GRAPHICS_ADAPTER_MUST_HAVE_AT_LEAST_ONE_TARGET = 0xC0262329;
    int ERROR_GRAPHICS_INVALID_MONITORDESCRIPTORSET = 0xC026232A;
    int ERROR_GRAPHICS_INVALID_MONITORDESCRIPTOR = 0xC026232B;
    int ERROR_GRAPHICS_MONITORDESCRIPTOR_NOT_IN_SET = 0xC026232C;
    int ERROR_GRAPHICS_MONITORDESCRIPTOR_ALREADY_IN_SET = 0xC026232D;
    int ERROR_GRAPHICS_MONITORDESCRIPTOR_ID_MUST_BE_UNIQUE = 0xC026232E;
    int ERROR_GRAPHICS_INVALID_VIDPN_TARGET_SUBSET_TYPE = 0xC026232F;
    int ERROR_GRAPHICS_RESOURCES_NOT_RELATED = 0xC0262330;
    int ERROR_GRAPHICS_SOURCE_ID_MUST_BE_UNIQUE = 0xC0262331;
    int ERROR_GRAPHICS_TARGET_ID_MUST_BE_UNIQUE = 0xC0262332;
    int ERROR_GRAPHICS_NO_AVAILABLE_VIDPN_TARGET = 0xC0262333;
    int ERROR_GRAPHICS_MONITOR_COULD_NOT_BE_ASSOCIATED_WITH_ADAPTER = 0xC0262334;
    int ERROR_GRAPHICS_NO_VIDPNMGR = 0xC0262335;
    int ERROR_GRAPHICS_NO_ACTIVE_VIDPN = 0xC0262336;
    int ERROR_GRAPHICS_STALE_VIDPN_TOPOLOGY = 0xC0262337;
    int ERROR_GRAPHICS_MONITOR_NOT_CONNECTED = 0xC0262338;
    int ERROR_GRAPHICS_SOURCE_NOT_IN_TOPOLOGY = 0xC0262339;
    int ERROR_GRAPHICS_INVALID_PRIMARYSURFACE_SIZE = 0xC026233A;
    int ERROR_GRAPHICS_INVALID_VISIBLEREGION_SIZE = 0xC026233B;
    int ERROR_GRAPHICS_INVALID_STRIDE = 0xC026233C;
    int ERROR_GRAPHICS_INVALID_PIXELFORMAT = 0xC026233D;
    int ERROR_GRAPHICS_INVALID_COLORBASIS = 0xC026233E;
    int ERROR_GRAPHICS_INVALID_PIXELVALUEACCESSMODE = 0xC026233F;
    int ERROR_GRAPHICS_TARGET_NOT_IN_TOPOLOGY = 0xC0262340;
    int ERROR_GRAPHICS_NO_DISPLAY_MODE_MANAGEMENT_SUPPORT = 0xC0262341;
    int ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE = 0xC0262342;
    int ERROR_GRAPHICS_CANT_ACCESS_ACTIVE_VIDPN = 0xC0262343;
    int ERROR_GRAPHICS_INVALID_PATH_IMPORTANCE_ORDINAL = 0xC0262344;
    int ERROR_GRAPHICS_INVALID_PATH_CONTENT_GEOMETRY_TRANSFORMATION = 0xC0262345;
    int ERROR_GRAPHICS_PATH_CONTENT_GEOMETRY_TRANSFORMATION_NOT_SUPPORTED = 0xC0262346;
    int ERROR_GRAPHICS_INVALID_GAMMA_RAMP = 0xC0262347;
    int ERROR_GRAPHICS_GAMMA_RAMP_NOT_SUPPORTED = 0xC0262348;
    int ERROR_GRAPHICS_MULTISAMPLING_NOT_SUPPORTED = 0xC0262349;
    int ERROR_GRAPHICS_MODE_NOT_IN_MODESET = 0xC026234A;
    int ERROR_GRAPHICS_DATASET_IS_EMPTY = 0x0026234B;
    int ERROR_GRAPHICS_NO_MORE_ELEMENTS_IN_DATASET = 0x0026234C;
    int ERROR_GRAPHICS_INVALID_VIDPN_TOPOLOGY_RECOMMENDATION_REASON = 0xC026234D;
    int ERROR_GRAPHICS_INVALID_PATH_CONTENT_TYPE = 0xC026234E;
    int ERROR_GRAPHICS_INVALID_COPYPROTECTION_TYPE = 0xC026234F;
    int ERROR_GRAPHICS_UNASSIGNED_MODESET_ALREADY_EXISTS = 0xC0262350;
    int ERROR_GRAPHICS_PATH_CONTENT_GEOMETRY_TRANSFORMATION_NOT_PINNED = 0x00262351;
    int ERROR_GRAPHICS_INVALID_SCANLINE_ORDERING = 0xC0262352;
    int ERROR_GRAPHICS_TOPOLOGY_CHANGES_NOT_ALLOWED = 0xC0262353;
    int ERROR_GRAPHICS_NO_AVAILABLE_IMPORTANCE_ORDINALS = 0xC0262354;
    int ERROR_GRAPHICS_INCOMPATIBLE_PRIVATE_FORMAT = 0xC0262355;
    int ERROR_GRAPHICS_INVALID_MODE_PRUNING_ALGORITHM = 0xC0262356;
    int ERROR_GRAPHICS_INVALID_MONITOR_CAPABILITY_ORIGIN = 0xC0262357;
    int ERROR_GRAPHICS_INVALID_MONITOR_FREQUENCYRANGE_CONSTRAINT = 0xC0262358;
    int ERROR_GRAPHICS_MAX_NUM_PATHS_REACHED = 0xC0262359;
    int ERROR_GRAPHICS_CANCEL_VIDPN_TOPOLOGY_AUGMENTATION = 0xC026235A;
    int ERROR_GRAPHICS_INVALID_CLIENT_TYPE = 0xC026235B;
    int ERROR_GRAPHICS_CLIENTVIDPN_NOT_SET = 0xC026235C;
    int ERROR_GRAPHICS_SPECIFIED_CHILD_ALREADY_CONNECTED = 0xC0262400;
    int ERROR_GRAPHICS_CHILD_DESCRIPTOR_NOT_SUPPORTED = 0xC0262401;
    int ERROR_GRAPHICS_UNKNOWN_CHILD_STATUS = 0x4026242F;
    int ERROR_GRAPHICS_NOT_A_LINKED_ADAPTER = 0xC0262430;
    int ERROR_GRAPHICS_LEADLINK_NOT_ENUMERATED = 0xC0262431;
    int ERROR_GRAPHICS_CHAINLINKS_NOT_ENUMERATED = 0xC0262432;
    int ERROR_GRAPHICS_ADAPTER_CHAIN_NOT_READY = 0xC0262433;
    int ERROR_GRAPHICS_CHAINLINKS_NOT_STARTED = 0xC0262434;
    int ERROR_GRAPHICS_CHAINLINKS_NOT_POWERED_ON = 0xC0262435;
    int ERROR_GRAPHICS_INCONSISTENT_DEVICE_LINK_STATE = 0xC0262436;
    int ERROR_GRAPHICS_LEADLINK_START_DEFERRED = 0x40262437;
    int ERROR_GRAPHICS_NOT_POST_DEVICE_DRIVER = 0xC0262438;
    int ERROR_GRAPHICS_POLLING_TOO_FREQUENTLY = 0x40262439;
    int ERROR_GRAPHICS_START_DEFERRED = 0x4026243A;
    int ERROR_GRAPHICS_ADAPTER_ACCESS_NOT_EXCLUDED = 0xC026243B;
    int ERROR_GRAPHICS_OPM_NOT_SUPPORTED = 0xC0262500;
    int ERROR_GRAPHICS_COPP_NOT_SUPPORTED = 0xC0262501;
    int ERROR_GRAPHICS_UAB_NOT_SUPPORTED = 0xC0262502;
    int ERROR_GRAPHICS_OPM_INVALID_ENCRYPTED_PARAMETERS = 0xC0262503;
    int ERROR_GRAPHICS_OPM_NO_VIDEO_OUTPUTS_EXIST = 0xC0262505;
    int ERROR_GRAPHICS_OPM_INTERNAL_ERROR = 0xC026250B;
    int ERROR_GRAPHICS_OPM_INVALID_HANDLE = 0xC026250C;
    int ERROR_GRAPHICS_PVP_INVALID_CERTIFICATE_LENGTH = 0xC026250E;
    int ERROR_GRAPHICS_OPM_SPANNING_MODE_ENABLED = 0xC026250F;
    int ERROR_GRAPHICS_OPM_THEATER_MODE_ENABLED = 0xC0262510;
    int ERROR_GRAPHICS_PVP_HFS_FAILED = 0xC0262511;
    int ERROR_GRAPHICS_OPM_INVALID_SRM = 0xC0262512;
    int ERROR_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_HDCP = 0xC0262513;
    int ERROR_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_ACP = 0xC0262514;
    int ERROR_GRAPHICS_OPM_OUTPUT_DOES_NOT_SUPPORT_CGMSA = 0xC0262515;
    int ERROR_GRAPHICS_OPM_HDCP_SRM_NEVER_SET = 0xC0262516;
    int ERROR_GRAPHICS_OPM_RESOLUTION_TOO_HIGH = 0xC0262517;
    int ERROR_GRAPHICS_OPM_ALL_HDCP_HARDWARE_ALREADY_IN_USE = 0xC0262518;
    int ERROR_GRAPHICS_OPM_VIDEO_OUTPUT_NO_LONGER_EXISTS = 0xC026251A;
    int ERROR_GRAPHICS_OPM_SESSION_TYPE_CHANGE_IN_PROGRESS = 0xC026251B;
    int ERROR_GRAPHICS_OPM_VIDEO_OUTPUT_DOES_NOT_HAVE_COPP_SEMANTICS = 0xC026251C;
    int ERROR_GRAPHICS_OPM_INVALID_INFORMATION_REQUEST = 0xC026251D;
    int ERROR_GRAPHICS_OPM_DRIVER_INTERNAL_ERROR = 0xC026251E;
    int ERROR_GRAPHICS_OPM_VIDEO_OUTPUT_DOES_NOT_HAVE_OPM_SEMANTICS = 0xC026251F;
    int ERROR_GRAPHICS_OPM_SIGNALING_NOT_SUPPORTED = 0xC0262520;
    int ERROR_GRAPHICS_OPM_INVALID_CONFIGURATION_REQUEST = 0xC0262521;
    int ERROR_GRAPHICS_I2C_NOT_SUPPORTED = 0xC0262580;
    int ERROR_GRAPHICS_I2C_DEVICE_DOES_NOT_EXIST = 0xC0262581;
    int ERROR_GRAPHICS_I2C_ERROR_TRANSMITTING_DATA = 0xC0262582;
    int ERROR_GRAPHICS_I2C_ERROR_RECEIVING_DATA = 0xC0262583;
    int ERROR_GRAPHICS_DDCCI_VCP_NOT_SUPPORTED = 0xC0262584;
    int ERROR_GRAPHICS_DDCCI_INVALID_DATA = 0xC0262585;
    int ERROR_GRAPHICS_DDCCI_MONITOR_RETURNED_INVALID_TIMING_STATUS_BYTE = 0xC0262586;
    int ERROR_GRAPHICS_MCA_INVALID_CAPABILITIES_STRING = 0xC0262587;
    int ERROR_GRAPHICS_MCA_INTERNAL_ERROR = 0xC0262588;
    int ERROR_GRAPHICS_DDCCI_INVALID_MESSAGE_COMMAND = 0xC0262589;
    int ERROR_GRAPHICS_DDCCI_INVALID_MESSAGE_LENGTH = 0xC026258A;
    int ERROR_GRAPHICS_DDCCI_INVALID_MESSAGE_CHECKSUM = 0xC026258B;
    int ERROR_GRAPHICS_INVALID_PHYSICAL_MONITOR_HANDLE = 0xC026258C;
    int ERROR_GRAPHICS_MONITOR_NO_LONGER_EXISTS = 0xC026258D;
    int ERROR_GRAPHICS_DDCCI_CURRENT_CURRENT_VALUE_GREATER_THAN_MAXIMUM_VALUE = 0xC02625D8;
    int ERROR_GRAPHICS_MCA_INVALID_VCP_VERSION = 0xC02625D9;
    int ERROR_GRAPHICS_MCA_MONITOR_VIOLATES_MCCS_SPECIFICATION = 0xC02625DA;
    int ERROR_GRAPHICS_MCA_MCCS_VERSION_MISMATCH = 0xC02625DB;
    int ERROR_GRAPHICS_MCA_UNSUPPORTED_MCCS_VERSION = 0xC02625DC;
    int ERROR_GRAPHICS_MCA_INVALID_TECHNOLOGY_TYPE_RETURNED = 0xC02625DE;
    int ERROR_GRAPHICS_MCA_UNSUPPORTED_COLOR_TEMPERATURE = 0xC02625DF;
    int ERROR_GRAPHICS_ONLY_CONSOLE_SESSION_SUPPORTED = 0xC02625E0;
    int ERROR_GRAPHICS_NO_DISPLAY_DEVICE_CORRESPONDS_TO_NAME = 0xC02625E1;
    int ERROR_GRAPHICS_DISPLAY_DEVICE_NOT_ATTACHED_TO_DESKTOP = 0xC02625E2;
    int ERROR_GRAPHICS_MIRRORING_DEVICES_NOT_SUPPORTED = 0xC02625E3;
    int ERROR_GRAPHICS_INVALID_POINTER = 0xC02625E4;
    int ERROR_GRAPHICS_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE = 0xC02625E5;
    int ERROR_GRAPHICS_PARAMETER_ARRAY_TOO_SMALL = 0xC02625E6;
    int ERROR_GRAPHICS_INTERNAL_ERROR = 0xC02625E7;
    int ERROR_GRAPHICS_SESSION_TYPE_CHANGE_IN_PROGRESS = 0xC02605E8;
    int TPM_E_ERROR_MASK = 0x80280000;
    int TPM_E_AUTHFAIL = 0x80280001;
    int TPM_E_BADINDEX = 0x80280002;
    int TPM_E_BAD_PARAMETER = 0x80280003;
    int TPM_E_AUDITFAILURE = 0x80280004;
    int TPM_E_CLEAR_DISABLED = 0x80280005;
    int TPM_E_DEACTIVATED = 0x80280006;
    int TPM_E_DISABLED = 0x80280007;
    int TPM_E_DISABLED_CMD = 0x80280008;
    int TPM_E_FAIL = 0x80280009;
    int TPM_E_BAD_ORDINAL = 0x8028000A;
    int TPM_E_INSTALL_DISABLED = 0x8028000B;
    int TPM_E_INVALID_KEYHANDLE = 0x8028000C;
    int TPM_E_KEYNOTFOUND = 0x8028000D;
    int TPM_E_INAPPROPRIATE_ENC = 0x8028000E;
    int TPM_E_MIGRATEFAIL = 0x8028000F;
    int TPM_E_INVALID_PCR_INFO = 0x80280010;
    int TPM_E_NOSPACE = 0x80280011;
    int TPM_E_NOSRK = 0x80280012;
    int TPM_E_NOTSEALED_BLOB = 0x80280013;
    int TPM_E_OWNER_SET = 0x80280014;
    int TPM_E_RESOURCES = 0x80280015;
    int TPM_E_SHORTRANDOM = 0x80280016;
    int TPM_E_SIZE = 0x80280017;
    int TPM_E_WRONGPCRVAL = 0x80280018;
    int TPM_E_BAD_PARAM_SIZE = 0x80280019;
    int TPM_E_SHA_THREAD = 0x8028001A;
    int TPM_E_SHA_ERROR = 0x8028001B;
    int TPM_E_FAILEDSELFTEST = 0x8028001C;
    int TPM_E_AUTH2FAIL = 0x8028001D;
    int TPM_E_BADTAG = 0x8028001E;
    int TPM_E_IOERROR = 0x8028001F;
    int TPM_E_ENCRYPT_ERROR = 0x80280020;
    int TPM_E_DECRYPT_ERROR = 0x80280021;
    int TPM_E_INVALID_AUTHHANDLE = 0x80280022;
    int TPM_E_NO_ENDORSEMENT = 0x80280023;
    int TPM_E_INVALID_KEYUSAGE = 0x80280024;
    int TPM_E_WRONG_ENTITYTYPE = 0x80280025;
    int TPM_E_INVALID_POSTINIT = 0x80280026;
    int TPM_E_INAPPROPRIATE_SIG = 0x80280027;
    int TPM_E_BAD_KEY_PROPERTY = 0x80280028;
    int TPM_E_BAD_MIGRATION = 0x80280029;
    int TPM_E_BAD_SCHEME = 0x8028002A;
    int TPM_E_BAD_DATASIZE = 0x8028002B;
    int TPM_E_BAD_MODE = 0x8028002C;
    int TPM_E_BAD_PRESENCE = 0x8028002D;
    int TPM_E_BAD_VERSION = 0x8028002E;
    int TPM_E_NO_WRAP_TRANSPORT = 0x8028002F;
    int TPM_E_AUDITFAIL_UNSUCCESSFUL = 0x80280030;
    int TPM_E_AUDITFAIL_SUCCESSFUL = 0x80280031;
    int TPM_E_NOTRESETABLE = 0x80280032;
    int TPM_E_NOTLOCAL = 0x80280033;
    int TPM_E_BAD_TYPE = 0x80280034;
    int TPM_E_INVALID_RESOURCE = 0x80280035;
    int TPM_E_NOTFIPS = 0x80280036;
    int TPM_E_INVALID_FAMILY = 0x80280037;
    int TPM_E_NO_NV_PERMISSION = 0x80280038;
    int TPM_E_REQUIRES_SIGN = 0x80280039;
    int TPM_E_KEY_NOTSUPPORTED = 0x8028003A;
    int TPM_E_AUTH_CONFLICT = 0x8028003B;
    int TPM_E_AREA_LOCKED = 0x8028003C;
    int TPM_E_BAD_LOCALITY = 0x8028003D;
    int TPM_E_READ_ONLY = 0x8028003E;
    int TPM_E_PER_NOWRITE = 0x8028003F;
    int TPM_E_FAMILYCOUNT = 0x80280040;
    int TPM_E_WRITE_LOCKED = 0x80280041;
    int TPM_E_BAD_ATTRIBUTES = 0x80280042;
    int TPM_E_INVALID_STRUCTURE = 0x80280043;
    int TPM_E_KEY_OWNER_CONTROL = 0x80280044;
    int TPM_E_BAD_COUNTER = 0x80280045;
    int TPM_E_NOT_FULLWRITE = 0x80280046;
    int TPM_E_CONTEXT_GAP = 0x80280047;
    int TPM_E_MAXNVWRITES = 0x80280048;
    int TPM_E_NOOPERATOR = 0x80280049;
    int TPM_E_RESOURCEMISSING = 0x8028004A;
    int TPM_E_DELEGATE_LOCK = 0x8028004B;
    int TPM_E_DELEGATE_FAMILY = 0x8028004C;
    int TPM_E_DELEGATE_ADMIN = 0x8028004D;
    int TPM_E_TRANSPORT_NOTEXCLUSIVE = 0x8028004E;
    int TPM_E_OWNER_CONTROL = 0x8028004F;
    int TPM_E_DAA_RESOURCES = 0x80280050;
    int TPM_E_DAA_INPUT_DATA0 = 0x80280051;
    int TPM_E_DAA_INPUT_DATA1 = 0x80280052;
    int TPM_E_DAA_ISSUER_SETTINGS = 0x80280053;
    int TPM_E_DAA_TPM_SETTINGS = 0x80280054;
    int TPM_E_DAA_STAGE = 0x80280055;
    int TPM_E_DAA_ISSUER_VALIDITY = 0x80280056;
    int TPM_E_DAA_WRONG_W = 0x80280057;
    int TPM_E_BAD_HANDLE = 0x80280058;
    int TPM_E_BAD_DELEGATE = 0x80280059;
    int TPM_E_BADCONTEXT = 0x8028005A;
    int TPM_E_TOOMANYCONTEXTS = 0x8028005B;
    int TPM_E_MA_TICKET_SIGNATURE = 0x8028005C;
    int TPM_E_MA_DESTINATION = 0x8028005D;
    int TPM_E_MA_SOURCE = 0x8028005E;
    int TPM_E_MA_AUTHORITY = 0x8028005F;
    int TPM_E_PERMANENTEK = 0x80280061;
    int TPM_E_BAD_SIGNATURE = 0x80280062;
    int TPM_E_NOCONTEXTSPACE = 0x80280063;
    int TPM_E_COMMAND_BLOCKED = 0x80280400;
    int TPM_E_INVALID_HANDLE = 0x80280401;
    int TPM_E_DUPLICATE_VHANDLE = 0x80280402;
    int TPM_E_EMBEDDED_COMMAND_BLOCKED = 0x80280403;
    int TPM_E_EMBEDDED_COMMAND_UNSUPPORTED = 0x80280404;
    int TPM_E_RETRY = 0x80280800;
    int TPM_E_NEEDS_SELFTEST = 0x80280801;
    int TPM_E_DOING_SELFTEST = 0x80280802;
    int TPM_E_DEFEND_LOCK_RUNNING = 0x80280803;
    int TBS_E_INTERNAL_ERROR = 0x80284001;
    int TBS_E_BAD_PARAMETER = 0x80284002;
    int TBS_E_INVALID_OUTPUT_POINTER = 0x80284003;
    int TBS_E_INVALID_CONTEXT = 0x80284004;
    int TBS_E_INSUFFICIENT_BUFFER = 0x80284005;
    int TBS_E_IOERROR = 0x80284006;
    int TBS_E_INVALID_CONTEXT_PARAM = 0x80284007;
    int TBS_E_SERVICE_NOT_RUNNING = 0x80284008;
    int TBS_E_TOO_MANY_TBS_CONTEXTS = 0x80284009;
    int TBS_E_TOO_MANY_RESOURCES = 0x8028400A;
    int TBS_E_SERVICE_START_PENDING = 0x8028400B;
    int TBS_E_PPI_NOT_SUPPORTED = 0x8028400C;
    int TBS_E_COMMAND_CANCELED = 0x8028400D;
    int TBS_E_BUFFER_TOO_LARGE = 0x8028400E;
    int TBS_E_TPM_NOT_FOUND = 0x8028400F;
    int TBS_E_SERVICE_DISABLED = 0x80284010;
    int TPMAPI_E_INVALID_STATE = 0x80290100;
    int TPMAPI_E_NOT_ENOUGH_DATA = 0x80290101;
    int TPMAPI_E_TOO_MUCH_DATA = 0x80290102;
    int TPMAPI_E_INVALID_OUTPUT_POINTER = 0x80290103;
    int TPMAPI_E_INVALID_PARAMETER = 0x80290104;
    int TPMAPI_E_OUT_OF_MEMORY = 0x80290105;
    int TPMAPI_E_BUFFER_TOO_SMALL = 0x80290106;
    int TPMAPI_E_INTERNAL_ERROR = 0x80290107;
    int TPMAPI_E_ACCESS_DENIED = 0x80290108;
    int TPMAPI_E_AUTHORIZATION_FAILED = 0x80290109;
    int TPMAPI_E_INVALID_CONTEXT_HANDLE = 0x8029010A;
    int TPMAPI_E_TBS_COMMUNICATION_ERROR = 0x8029010B;
    int TPMAPI_E_TPM_COMMAND_ERROR = 0x8029010C;
    int TPMAPI_E_MESSAGE_TOO_LARGE = 0x8029010D;
    int TPMAPI_E_INVALID_ENCODING = 0x8029010E;
    int TPMAPI_E_INVALID_KEY_SIZE = 0x8029010F;
    int TPMAPI_E_ENCRYPTION_FAILED = 0x80290110;
    int TPMAPI_E_INVALID_KEY_PARAMS = 0x80290111;
    int TPMAPI_E_INVALID_MIGRATION_AUTHORIZATION_BLOB = 0x80290112;
    int TPMAPI_E_INVALID_PCR_INDEX = 0x80290113;
    int TPMAPI_E_INVALID_DELEGATE_BLOB = 0x80290114;
    int TPMAPI_E_INVALID_CONTEXT_PARAMS = 0x80290115;
    int TPMAPI_E_INVALID_KEY_BLOB = 0x80290116;
    int TPMAPI_E_INVALID_PCR_DATA = 0x80290117;
    int TPMAPI_E_INVALID_OWNER_AUTH = 0x80290118;
    int TPMAPI_E_FIPS_RNG_CHECK_FAILED = 0x80290119;
    int TBSIMP_E_BUFFER_TOO_SMALL = 0x80290200;
    int TBSIMP_E_CLEANUP_FAILED = 0x80290201;
    int TBSIMP_E_INVALID_CONTEXT_HANDLE = 0x80290202;
    int TBSIMP_E_INVALID_CONTEXT_PARAM = 0x80290203;
    int TBSIMP_E_TPM_ERROR = 0x80290204;
    int TBSIMP_E_HASH_BAD_KEY = 0x80290205;
    int TBSIMP_E_DUPLICATE_VHANDLE = 0x80290206;
    int TBSIMP_E_INVALID_OUTPUT_POINTER = 0x80290207;
    int TBSIMP_E_INVALID_PARAMETER = 0x80290208;
    int TBSIMP_E_RPC_INIT_FAILED = 0x80290209;
    int TBSIMP_E_SCHEDULER_NOT_RUNNING = 0x8029020A;
    int TBSIMP_E_COMMAND_CANCELED = 0x8029020B;
    int TBSIMP_E_OUT_OF_MEMORY = 0x8029020C;
    int TBSIMP_E_LIST_NO_MORE_ITEMS = 0x8029020D;
    int TBSIMP_E_LIST_NOT_FOUND = 0x8029020E;
    int TBSIMP_E_NOT_ENOUGH_SPACE = 0x8029020F;
    int TBSIMP_E_NOT_ENOUGH_TPM_CONTEXTS = 0x80290210;
    int TBSIMP_E_COMMAND_FAILED = 0x80290211;
    int TBSIMP_E_UNKNOWN_ORDINAL = 0x80290212;
    int TBSIMP_E_RESOURCE_EXPIRED = 0x80290213;
    int TBSIMP_E_INVALID_RESOURCE = 0x80290214;
    int TBSIMP_E_NOTHING_TO_UNLOAD = 0x80290215;
    int TBSIMP_E_HASH_TABLE_FULL = 0x80290216;
    int TBSIMP_E_TOO_MANY_TBS_CONTEXTS = 0x80290217;
    int TBSIMP_E_TOO_MANY_RESOURCES = 0x80290218;
    int TBSIMP_E_PPI_NOT_SUPPORTED = 0x80290219;
    int TBSIMP_E_TPM_INCOMPATIBLE = 0x8029021A;
    int TPM_E_PPI_ACPI_FAILURE = 0x80290300;
    int TPM_E_PPI_USER_ABORT = 0x80290301;
    int TPM_E_PPI_BIOS_FAILURE = 0x80290302;
    int TPM_E_PPI_NOT_SUPPORTED = 0x80290303;
    int PLA_E_DCS_NOT_FOUND = 0x80300002;
    int PLA_E_DCS_IN_USE = 0x803000AA;
    int PLA_E_TOO_MANY_FOLDERS = 0x80300045;
    int PLA_E_NO_MIN_DISK = 0x80300070;
    int PLA_E_DCS_ALREADY_EXISTS = 0x803000B7;
    int PLA_S_PROPERTY_IGNORED = 0x00300100;
    int PLA_E_PROPERTY_CONFLICT = 0x80300101;
    int PLA_E_DCS_SINGLETON_REQUIRED = 0x80300102;
    int PLA_E_CREDENTIALS_REQUIRED = 0x80300103;
    int PLA_E_DCS_NOT_RUNNING = 0x80300104;
    int PLA_E_CONFLICT_INCL_EXCL_API = 0x80300105;
    int PLA_E_NETWORK_EXE_NOT_VALID = 0x80300106;
    int PLA_E_EXE_ALREADY_CONFIGURED = 0x80300107;
    int PLA_E_EXE_PATH_NOT_VALID = 0x80300108;
    int PLA_E_DC_ALREADY_EXISTS = 0x80300109;
    int PLA_E_DCS_START_WAIT_TIMEOUT = 0x8030010A;
    int PLA_E_DC_START_WAIT_TIMEOUT = 0x8030010B;
    int PLA_E_REPORT_WAIT_TIMEOUT = 0x8030010C;
    int PLA_E_NO_DUPLICATES = 0x8030010D;
    int PLA_E_EXE_FULL_PATH_REQUIRED = 0x8030010E;
    int PLA_E_INVALID_SESSION_NAME = 0x8030010F;
    int PLA_E_PLA_CHANNEL_NOT_ENABLED = 0x80300110;
    int PLA_E_TASKSCHED_CHANNEL_NOT_ENABLED = 0x80300111;
    int PLA_E_RULES_MANAGER_FAILED = 0x80300112;
    int PLA_E_CABAPI_FAILURE = 0x80300113;
    int FVE_E_LOCKED_VOLUME = 0x80310000;
    int FVE_E_NOT_ENCRYPTED = 0x80310001;
    int FVE_E_NO_TPM_BIOS = 0x80310002;
    int FVE_E_NO_MBR_METRIC = 0x80310003;
    int FVE_E_NO_BOOTSECTOR_METRIC = 0x80310004;
    int FVE_E_NO_BOOTMGR_METRIC = 0x80310005;
    int FVE_E_WRONG_BOOTMGR = 0x80310006;
    int FVE_E_SECURE_KEY_REQUIRED = 0x80310007;
    int FVE_E_NOT_ACTIVATED = 0x80310008;
    int FVE_E_ACTION_NOT_ALLOWED = 0x80310009;
    int FVE_E_AD_SCHEMA_NOT_INSTALLED = 0x8031000A;
    int FVE_E_AD_INVALID_DATATYPE = 0x8031000B;
    int FVE_E_AD_INVALID_DATASIZE = 0x8031000C;
    int FVE_E_AD_NO_VALUES = 0x8031000D;
    int FVE_E_AD_ATTR_NOT_SET = 0x8031000E;
    int FVE_E_AD_GUID_NOT_FOUND = 0x8031000F;
    int FVE_E_BAD_INFORMATION = 0x80310010;
    int FVE_E_TOO_SMALL = 0x80310011;
    int FVE_E_SYSTEM_VOLUME = 0x80310012;
    int FVE_E_FAILED_WRONG_FS = 0x80310013;
    int FVE_E_FAILED_BAD_FS = 0x80310014;
    int FVE_E_NOT_SUPPORTED = 0x80310015;
    int FVE_E_BAD_DATA = 0x80310016;
    int FVE_E_VOLUME_NOT_BOUND = 0x80310017;
    int FVE_E_TPM_NOT_OWNED = 0x80310018;
    int FVE_E_NOT_DATA_VOLUME = 0x80310019;
    int FVE_E_AD_INSUFFICIENT_BUFFER = 0x8031001A;
    int FVE_E_CONV_READ = 0x8031001B;
    int FVE_E_CONV_WRITE = 0x8031001C;
    int FVE_E_KEY_REQUIRED = 0x8031001D;
    int FVE_E_CLUSTERING_NOT_SUPPORTED = 0x8031001E;
    int FVE_E_VOLUME_BOUND_ALREADY = 0x8031001F;
    int FVE_E_OS_NOT_PROTECTED = 0x80310020;
    int FVE_E_PROTECTION_DISABLED = 0x80310021;
    int FVE_E_RECOVERY_KEY_REQUIRED = 0x80310022;
    int FVE_E_FOREIGN_VOLUME = 0x80310023;
    int FVE_E_OVERLAPPED_UPDATE = 0x80310024;
    int FVE_E_TPM_SRK_AUTH_NOT_ZERO = 0x80310025;
    int FVE_E_FAILED_SECTOR_SIZE = 0x80310026;
    int FVE_E_FAILED_AUTHENTICATION = 0x80310027;
    int FVE_E_NOT_OS_VOLUME = 0x80310028;
    int FVE_E_AUTOUNLOCK_ENABLED = 0x80310029;
    int FVE_E_WRONG_BOOTSECTOR = 0x8031002A;
    int FVE_E_WRONG_SYSTEM_FS = 0x8031002B;
    int FVE_E_POLICY_PASSWORD_REQUIRED = 0x8031002C;
    int FVE_E_CANNOT_SET_FVEK_ENCRYPTED = 0x8031002D;
    int FVE_E_CANNOT_ENCRYPT_NO_KEY = 0x8031002E;
    int FVE_E_BOOTABLE_CDDVD = 0x80310030;
    int FVE_E_PROTECTOR_EXISTS = 0x80310031;
    int FVE_E_RELATIVE_PATH = 0x80310032;
    int FVE_E_PROTECTOR_NOT_FOUND = 0x80310033;
    int FVE_E_INVALID_KEY_FORMAT = 0x80310034;
    int FVE_E_INVALID_PASSWORD_FORMAT = 0x80310035;
    int FVE_E_FIPS_RNG_CHECK_FAILED = 0x80310036;
    int FVE_E_FIPS_PREVENTS_RECOVERY_PASSWORD = 0x80310037;
    int FVE_E_FIPS_PREVENTS_EXTERNAL_KEY_EXPORT = 0x80310038;
    int FVE_E_NOT_DECRYPTED = 0x80310039;
    int FVE_E_INVALID_PROTECTOR_TYPE = 0x8031003A;
    int FVE_E_NO_PROTECTORS_TO_TEST = 0x8031003B;
    int FVE_E_KEYFILE_NOT_FOUND = 0x8031003C;
    int FVE_E_KEYFILE_INVALID = 0x8031003D;
    int FVE_E_KEYFILE_NO_VMK = 0x8031003E;
    int FVE_E_TPM_DISABLED = 0x8031003F;
    int FVE_E_NOT_ALLOWED_IN_SAFE_MODE = 0x80310040;
    int FVE_E_TPM_INVALID_PCR = 0x80310041;
    int FVE_E_TPM_NO_VMK = 0x80310042;
    int FVE_E_PIN_INVALID = 0x80310043;
    int FVE_E_AUTH_INVALID_APPLICATION = 0x80310044;
    int FVE_E_AUTH_INVALID_CONFIG = 0x80310045;
    int FVE_E_FIPS_DISABLE_PROTECTION_NOT_ALLOWED = 0x80310046;
    int FVE_E_FS_NOT_EXTENDED = 0x80310047;
    int FVE_E_FIRMWARE_TYPE_NOT_SUPPORTED = 0x80310048;
    int FVE_E_NO_LICENSE = 0x80310049;
    int FVE_E_NOT_ON_STACK = 0x8031004A;
    int FVE_E_FS_MOUNTED = 0x8031004B;
    int FVE_E_TOKEN_NOT_IMPERSONATED = 0x8031004C;
    int FVE_E_DRY_RUN_FAILED = 0x8031004D;
    int FVE_E_REBOOT_REQUIRED = 0x8031004E;
    int FVE_E_DEBUGGER_ENABLED = 0x8031004F;
    int FVE_E_RAW_ACCESS = 0x80310050;
    int FVE_E_RAW_BLOCKED = 0x80310051;
    int FVE_E_BCD_APPLICATIONS_PATH_INCORRECT = 0x80310052;
    int FVE_E_NOT_ALLOWED_IN_VERSION = 0x80310053;
    int FWP_E_CALLOUT_NOT_FOUND = 0x80320001;
    int FWP_E_CONDITION_NOT_FOUND = 0x80320002;
    int FWP_E_FILTER_NOT_FOUND = 0x80320003;
    int FWP_E_LAYER_NOT_FOUND = 0x80320004;
    int FWP_E_PROVIDER_NOT_FOUND = 0x80320005;
    int FWP_E_PROVIDER_CONTEXT_NOT_FOUND = 0x80320006;
    int FWP_E_SUBLAYER_NOT_FOUND = 0x80320007;
    int FWP_E_NOT_FOUND = 0x80320008;
    int FWP_E_ALREADY_EXISTS = 0x80320009;
    int FWP_E_IN_USE = 0x8032000A;
    int FWP_E_DYNAMIC_SESSION_IN_PROGRESS = 0x8032000B;
    int FWP_E_WRONG_SESSION = 0x8032000C;
    int FWP_E_NO_TXN_IN_PROGRESS = 0x8032000D;
    int FWP_E_TXN_IN_PROGRESS = 0x8032000E;
    int FWP_E_TXN_ABORTED = 0x8032000F;
    int FWP_E_SESSION_ABORTED = 0x80320010;
    int FWP_E_INCOMPATIBLE_TXN = 0x80320011;
    int FWP_E_TIMEOUT = 0x80320012;
    int FWP_E_NET_EVENTS_DISABLED = 0x80320013;
    int FWP_E_INCOMPATIBLE_LAYER = 0x80320014;
    int FWP_E_KM_CLIENTS_ONLY = 0x80320015;
    int FWP_E_LIFETIME_MISMATCH = 0x80320016;
    int FWP_E_BUILTIN_OBJECT = 0x80320017;
    int FWP_E_TOO_MANY_CALLOUTS = 0x80320018;
    int FWP_E_NOTIFICATION_DROPPED = 0x80320019;
    int FWP_E_TRAFFIC_MISMATCH = 0x8032001A;
    int FWP_E_INCOMPATIBLE_SA_STATE = 0x8032001B;
    int FWP_E_NULL_POINTER = 0x8032001C;
    int FWP_E_INVALID_ENUMERATOR = 0x8032001D;
    int FWP_E_INVALID_FLAGS = 0x8032001E;
    int FWP_E_INVALID_NET_MASK = 0x8032001F;
    int FWP_E_INVALID_RANGE = 0x80320020;
    int FWP_E_INVALID_INTERVAL = 0x80320021;
    int FWP_E_ZERO_LENGTH_ARRAY = 0x80320022;
    int FWP_E_NULL_DISPLAY_NAME = 0x80320023;
    int FWP_E_INVALID_ACTION_TYPE = 0x80320024;
    int FWP_E_INVALID_WEIGHT = 0x80320025;
    int FWP_E_MATCH_TYPE_MISMATCH = 0x80320026;
    int FWP_E_TYPE_MISMATCH = 0x80320027;
    int FWP_E_OUT_OF_BOUNDS = 0x80320028;
    int FWP_E_RESERVED = 0x80320029;
    int FWP_E_DUPLICATE_CONDITION = 0x8032002A;
    int FWP_E_DUPLICATE_KEYMOD = 0x8032002B;
    int FWP_E_ACTION_INCOMPATIBLE_WITH_LAYER = 0x8032002C;
    int FWP_E_ACTION_INCOMPATIBLE_WITH_SUBLAYER = 0x8032002D;
    int FWP_E_CONTEXT_INCOMPATIBLE_WITH_LAYER = 0x8032002E;
    int FWP_E_CONTEXT_INCOMPATIBLE_WITH_CALLOUT = 0x8032002F;
    int FWP_E_INCOMPATIBLE_AUTH_METHOD = 0x80320030;
    int FWP_E_INCOMPATIBLE_DH_GROUP = 0x80320031;
    int FWP_E_EM_NOT_SUPPORTED = 0x80320032;
    int FWP_E_NEVER_MATCH = 0x80320033;
    int FWP_E_PROVIDER_CONTEXT_MISMATCH = 0x80320034;
    int FWP_E_INVALID_PARAMETER = 0x80320035;
    int FWP_E_TOO_MANY_SUBLAYERS = 0x80320036;
    int FWP_E_CALLOUT_NOTIFICATION_FAILED = 0x80320037;
    int FWP_E_INVALID_AUTH_TRANSFORM = 0x80320038;
    int FWP_E_INVALID_CIPHER_TRANSFORM = 0x80320039;
    int ERROR_NDIS_INTERFACE_CLOSING = 0x80340002;
    int ERROR_NDIS_BAD_VERSION = 0x80340004;
    int ERROR_NDIS_BAD_CHARACTERISTICS = 0x80340005;
    int ERROR_NDIS_ADAPTER_NOT_FOUND = 0x80340006;
    int ERROR_NDIS_OPEN_FAILED = 0x80340007;
    int ERROR_NDIS_DEVICE_FAILED = 0x80340008;
    int ERROR_NDIS_MULTICAST_FULL = 0x80340009;
    int ERROR_NDIS_MULTICAST_EXISTS = 0x8034000A;
    int ERROR_NDIS_MULTICAST_NOT_FOUND = 0x8034000B;
    int ERROR_NDIS_REQUEST_ABORTED = 0x8034000C;
    int ERROR_NDIS_RESET_IN_PROGRESS = 0x8034000D;
    int ERROR_NDIS_NOT_SUPPORTED = 0x803400BB;
    int ERROR_NDIS_INVALID_PACKET = 0x8034000F;
    int ERROR_NDIS_ADAPTER_NOT_READY = 0x80340011;
    int ERROR_NDIS_INVALID_LENGTH = 0x80340014;
    int ERROR_NDIS_INVALID_DATA = 0x80340015;
    int ERROR_NDIS_BUFFER_TOO_SHORT = 0x80340016;
    int ERROR_NDIS_INVALID_OID = 0x80340017;
    int ERROR_NDIS_ADAPTER_REMOVED = 0x80340018;
    int ERROR_NDIS_UNSUPPORTED_MEDIA = 0x80340019;
    int ERROR_NDIS_GROUP_ADDRESS_IN_USE = 0x8034001A;
    int ERROR_NDIS_FILE_NOT_FOUND = 0x8034001B;
    int ERROR_NDIS_ERROR_READING_FILE = 0x8034001C;
    int ERROR_NDIS_ALREADY_MAPPED = 0x8034001D;
    int ERROR_NDIS_RESOURCE_CONFLICT = 0x8034001E;
    int ERROR_NDIS_MEDIA_DISCONNECTED = 0x8034001F;
    int ERROR_NDIS_INVALID_ADDRESS = 0x80340022;
    int ERROR_NDIS_INVALID_DEVICE_REQUEST = 0x80340010;
    int ERROR_NDIS_PAUSED = 0x8034002A;
    int ERROR_NDIS_INTERFACE_NOT_FOUND = 0x8034002B;
    int ERROR_NDIS_UNSUPPORTED_REVISION = 0x8034002C;
    int ERROR_NDIS_INVALID_PORT = 0x8034002D;
    int ERROR_NDIS_INVALID_PORT_STATE = 0x8034002E;
    int ERROR_NDIS_LOW_POWER_STATE = 0x8034002F;
    int ERROR_NDIS_DOT11_AUTO_CONFIG_ENABLED = 0x80342000;
    int ERROR_NDIS_DOT11_MEDIA_IN_USE = 0x80342001;
    int ERROR_NDIS_DOT11_POWER_STATE_INVALID = 0x80342002;
    int ERROR_NDIS_INDICATION_REQUIRED = 0x00340001;
}
