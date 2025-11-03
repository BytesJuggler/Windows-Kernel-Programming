# About Project

Represents a simple "hello world" for driver programming.

- Executing The Driver
- Verify Kernel Running



# Executing The Driver

```
> sc create <name> type= kernel binpath= <abs-path-to-sys-file>  # Install's the driver; check Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<name>
> sc query <name>  # Verify the config


# Before running the following command disable the Safe Boot Option, use f12 when system is booting to enter BIOS, enable advance setting if not enabled, then look for safe boot option
> bcdedit /set testsigning on  # Put the system into test signing mode; this mode allows unsigned drivers to be loaded without a hitch
# May also need to turn off Memory Integrity; Widnows Security -> Device security -> Core isolation details

> sc start <name>  # Loads the driver

if above fails, open Event Viewer -> Window Logs -> System, filter on Source=Service Control Manager and look for erros about driver.

> sc stop <name>  # Unloads the driver

> sc delete <name>  # Delete's the install
```


# Verify Running Kernel

- Through logging: use DebugPrint, a kernel level debug, with DbgView (view has kernel capture enabled)
- Better logging: use KdPrint, which wraps DebugPrint and only enables logging in Debug Build

In DbgView.exe, enable:
- capture > capture win32
- capture > capture kernel
- capture > enable verbose kernel output





