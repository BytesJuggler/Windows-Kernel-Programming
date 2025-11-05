Simple user mode application that shows how to interact with Device Objects directly.
We open the Device Client Object of beep device driver directly. By "directly", I mean
we will not be providing the symlink to the device object but the direct path. Usually
the symlinks in the `Global??` points to `Device` directory. But in this example of our
we will use direct path "\Device\Beep"; this path is also defined as a macro `DD_BEEP_DEVICE_NAME_U`
which is defined in the `<ntddbeep.h>` header.
