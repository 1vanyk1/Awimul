package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.win32.StdCallLibrary;
import com.sun.jna.win32.W32APITypeMapper;

import java.util.Arrays;
import java.util.List;

public interface Winsvc {
    public static class SERVICE_STATUS extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwServiceType", "dwCurrentState", "dwControlsAccepted",
                    "dwWin32ExitCode", "dwServiceSpecificExitCode", "dwCheckPoint",
                    "dwWaitHint");
        }

        public int dwServiceType;
        public int dwCurrentState;
        public int dwControlsAccepted;
        public int dwWin32ExitCode;
        public int dwServiceSpecificExitCode;
        public int dwCheckPoint;
        public int dwWaitHint;

        public SERVICE_STATUS() {
            super();
        }
    }

    public class SERVICE_STATUS_PROCESS extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwServiceType", "dwCurrentState", "dwControlsAccepted",
                    "dwWin32ExitCode", "dwServiceSpecificExitCode",
                    "dwCheckPoint", "dwWaitHint", "dwProcessId", "dwServiceFlags");
        }

        public int   dwServiceType;
        public int   dwCurrentState;
        public int   dwControlsAccepted;
        public int   dwWin32ExitCode;
        public int   dwServiceSpecificExitCode;
        public int   dwCheckPoint;
        public int   dwWaitHint;
        public int   dwProcessId;
        public int   dwServiceFlags;

        public SERVICE_STATUS_PROCESS() {
            super();
        }

        public SERVICE_STATUS_PROCESS(int size) {
            super(new Memory(size));
        }
    }

    public abstract class ChangeServiceConfig2Info extends Structure {
        public ChangeServiceConfig2Info() {
            super(Boolean.getBoolean("w32.ascii") ? W32APITypeMapper.ASCII : W32APITypeMapper.UNICODE);
        }

        public ChangeServiceConfig2Info(Pointer p) {
            super(p, ALIGN_DEFAULT, Boolean.getBoolean("w32.ascii") ? W32APITypeMapper.ASCII : W32APITypeMapper.UNICODE);
        }
    }

    public class SERVICE_FAILURE_ACTIONS extends ChangeServiceConfig2Info {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwResetPeriod", "lpRebootMsg", "lpCommand", "cActions", "lpsaActions");
        }

        public static class ByReference extends SERVICE_FAILURE_ACTIONS implements Structure.ByReference {}

        public int dwResetPeriod;
        public String lpRebootMsg;
        public String lpCommand;
        public int cActions;
        public SC_ACTION.ByReference lpsaActions;

        public SERVICE_FAILURE_ACTIONS() {
            super();
        }

        public SERVICE_FAILURE_ACTIONS(Pointer p) {
            super(p);
            read();
        }
    }

    public class SC_ACTION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("type", "delay");
        }

        public static class ByReference extends SC_ACTION implements Structure.ByReference {}

        public int type;
        public int delay;
    }

    public class SERVICE_FAILURE_ACTIONS_FLAG extends ChangeServiceConfig2Info {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("fFailureActionsOnNonCrashFailures");
        }

        public int fFailureActionsOnNonCrashFailures;

        public SERVICE_FAILURE_ACTIONS_FLAG() {
            super();
        }

        public SERVICE_FAILURE_ACTIONS_FLAG(Pointer p) {
            super(p);
            read();
        }
    }

    int SERVICE_RUNS_IN_SYSTEM_PROCESS = 0x00000001;

    public static class SC_HANDLE extends WinNT.HANDLE { }

    int SC_MANAGER_CONNECT              = 0x0001;
    int SC_MANAGER_CREATE_SERVICE       = 0x0002;
    int SC_MANAGER_ENUMERATE_SERVICE    = 0x0004;
    int SC_MANAGER_LOCK                 = 0x0008;
    int SC_MANAGER_QUERY_LOCK_STATUS    = 0x0010;
    int SC_MANAGER_MODIFY_BOOT_CONFIG   = 0x0020;

    int SC_MANAGER_ALL_ACCESS =
            WinNT.STANDARD_RIGHTS_REQUIRED | SC_MANAGER_CONNECT
                    | SC_MANAGER_CREATE_SERVICE | SC_MANAGER_ENUMERATE_SERVICE
                    | SC_MANAGER_LOCK | SC_MANAGER_QUERY_LOCK_STATUS
                    | SC_MANAGER_MODIFY_BOOT_CONFIG;

    int SERVICE_QUERY_CONFIG = 0x0001;
    int SERVICE_CHANGE_CONFIG = 0x0002;
    int SERVICE_QUERY_STATUS = 0x0004;
    int SERVICE_ENUMERATE_DEPENDENTS = 0x0008;
    int SERVICE_START = 0x0010;
    int SERVICE_STOP = 0x0020;
    int SERVICE_PAUSE_CONTINUE = 0x0040;
    int SERVICE_INTERROGATE = 0x0080;
    int SERVICE_USER_DEFINED_CONTROL = 0x0100;

    int SERVICE_ALL_ACCESS =
            WinNT.STANDARD_RIGHTS_REQUIRED | SERVICE_QUERY_CONFIG
                    | SERVICE_CHANGE_CONFIG | SERVICE_QUERY_STATUS
                    | SERVICE_ENUMERATE_DEPENDENTS | SERVICE_START | SERVICE_STOP
                    | SERVICE_PAUSE_CONTINUE | SERVICE_INTERROGATE
                    | SERVICE_USER_DEFINED_CONTROL;

    int SERVICE_ACTIVE = 0x00000001;
    int SERVICE_INACTIVE = 0x00000002;
    int SERVICE_STATE_ALL = SERVICE_ACTIVE | SERVICE_INACTIVE;

    int SERVICE_CONTROL_STOP                  = 0x00000001;
    int SERVICE_CONTROL_PAUSE                 = 0x00000002;
    int SERVICE_CONTROL_CONTINUE              = 0x00000003;
    int SERVICE_CONTROL_INTERROGATE           = 0x00000004;
    int SERVICE_CONTROL_SHUTDOWN              = 0x00000005;
    int SERVICE_CONTROL_PARAMCHANGE           = 0x00000006;
    int SERVICE_CONTROL_NETBINDADD            = 0x00000007;
    int SERVICE_CONTROL_NETBINDREMOVE         = 0x00000008;
    int SERVICE_CONTROL_NETBINDENABLE         = 0x00000009;
    int SERVICE_CONTROL_NETBINDDISABLE        = 0x0000000A;
    int SERVICE_CONTROL_DEVICEEVENT           = 0x0000000B;
    int SERVICE_CONTROL_HARDWAREPROFILECHANGE = 0x0000000C;
    int SERVICE_CONTROL_POWEREVENT            = 0x0000000D;
    int SERVICE_CONTROL_SESSIONCHANGE         = 0x0000000E;
    int SERVICE_CONTROL_PRESHUTDOWN           = 0x0000000F;
    int SERVICE_CONTROL_TIMECHANGE            = 0x00000010;
    int SERVICE_CONTROL_TRIGGEREVENT          = 0x00000020;
    int SERVICE_CONTROL_USERMODEREBOOT        = 0x00000040;

    int SERVICE_STOPPED          = 0x00000001;
    int SERVICE_START_PENDING    = 0x00000002;
    int SERVICE_STOP_PENDING     = 0x00000003;
    int SERVICE_RUNNING          = 0x00000004;
    int SERVICE_CONTINUE_PENDING = 0x00000005;
    int SERVICE_PAUSE_PENDING    = 0x00000006;
    int SERVICE_PAUSED           = 0x00000007;

    int SERVICE_ACCEPT_STOP                  = 0x00000001;
    int SERVICE_ACCEPT_PAUSE_CONTINUE        = 0x00000002;
    int SERVICE_ACCEPT_SHUTDOWN              = 0x00000004;
    int SERVICE_ACCEPT_PARAMCHANGE           = 0x00000008;
    int SERVICE_ACCEPT_NETBINDCHANGE         = 0x00000010;
    int SERVICE_ACCEPT_HARDWAREPROFILECHANGE = 0x00000020;
    int SERVICE_ACCEPT_POWEREVENT            = 0x00000040;
    int SERVICE_ACCEPT_SESSIONCHANGE         = 0x00000080;
    int SERVICE_ACCEPT_PRESHUTDOWN           = 0x00000100;
    int SERVICE_ACCEPT_TIMECHANGE            = 0x00000200;
    int SERVICE_ACCEPT_TRIGGEREVENT          = 0x00000400;

    //
    // ChangeServiceConfig2 dwInfoLevel values
    //
    int SERVICE_CONFIG_DESCRIPTION              = 0x00000001;
    int SERVICE_CONFIG_FAILURE_ACTIONS          = 0x00000002;
    int SERVICE_CONFIG_DELAYED_AUTO_START_INFO  = 0x00000003;
    int SERVICE_CONFIG_FAILURE_ACTIONS_FLAG     = 0x00000004;
    int SERVICE_CONFIG_SERVICE_SID_INFO         = 0x00000005;
    int SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO = 0x00000006;
    int SERVICE_CONFIG_PRESHUTDOWN_INFO         = 0x00000007;
    int SERVICE_CONFIG_TRIGGER_INFO             = 0x00000008;
    int SERVICE_CONFIG_PREFERRED_NODE           = 0x00000009;
    int SERVICE_CONFIG_LAUNCH_PROTECTED         = 0x0000000c;

    int SC_ACTION_NONE        = 0x00000000;
    int SC_ACTION_RESTART     = 0x00000001;
    int SC_ACTION_REBOOT      = 0x00000002;
    int SC_ACTION_RUN_COMMAND = 0x00000003;

    int SC_ENUM_PROCESS_INFO  = 0;

    public abstract class SC_STATUS_TYPE {
        public static final int SC_STATUS_PROCESS_INFO = 0;
    }

    interface SERVICE_MAIN_FUNCTION extends StdCallLibrary.StdCallCallback {
        public void callback(int dwArgc, Pointer lpszArgv);
    }

    interface Handler extends StdCallLibrary.StdCallCallback {
        public void callback(int fdwControl);
    }

    interface HandlerEx extends StdCallLibrary.StdCallCallback {
        public int callback(int dwControl, int dwEventType,
                            Pointer lpEventData, Pointer lpContext);
    }

    public static class SERVICE_TABLE_ENTRY extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("lpServiceName", "lpServiceProc");
        }

        public String lpServiceName;
        public SERVICE_MAIN_FUNCTION lpServiceProc;

        public SERVICE_TABLE_ENTRY() {
            super(W32APITypeMapper.DEFAULT);
        }
    }

    public static class SERVICE_DESCRIPTION extends ChangeServiceConfig2Info {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("lpDescription");
        }

        public String lpDescription;
    }

    public static class SERVICE_STATUS_HANDLE extends WinNT.HANDLE {

        public SERVICE_STATUS_HANDLE() {
        }

        public SERVICE_STATUS_HANDLE(Pointer p) {
            super(p);
        }
    }

    public static class ENUM_SERVICE_STATUS extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("lpServiceName", "lpDisplayName", "ServiceStatus");
        }

        public String lpServiceName;
        public String lpDisplayName;
        public SERVICE_STATUS ServiceStatus;

        public ENUM_SERVICE_STATUS() {
            super(W32APITypeMapper.DEFAULT);
        }
    }

    public static class ENUM_SERVICE_STATUS_PROCESS extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("lpServiceName", "lpDisplayName", "ServiceStatusProcess");
        }

        public String lpServiceName;
        public String lpDisplayName;
        public SERVICE_STATUS_PROCESS ServiceStatusProcess;

        public ENUM_SERVICE_STATUS_PROCESS() {
            super(W32APITypeMapper.DEFAULT);
        }
    }
}