# WinDbg CheatSheet

## Help

- `.hh <command>` display help for the `<command>`


## Load Symbols

- `.symfix` set the symbols quickly
- `lm` - shows loaded modules, shows if symbols loaded for each module
- set env var for persistance > key: `_NT_SYMBOL_PATH`, value: `SRV*c:\Symbols*http://msdl.microsoft.com/download/symbols`


## Detach

- `.detach` detach the debugger without killing the process
- `q` quit the debugger; if process is alive, it gets killed

## Threads

Initially selected thread is the remotely injected thread into the process which is used to enforce the breakpoint into the running process.
Information provided for threads with `~` command:

[Debugger Thread Idx] ID: [ProcessId].[ThreadId] Suspend: [Count] Teb: [Thread Environment Block(TEB)] [frozen?(from debugger perspective)]

- `~` show threads
- `~ns` change the selected thread to `n` where `n` is the thread index

- `k` show call stack for currently selected thread; depicted by `.` in front of thread index
- `~nk` show call stack for `n` thread index

- `!teb` shows Thread Execution Block of current thread; shows parts of real data structures behind the scenes
- "!teb <start_address>\`<end_address>" show TEB for specified address range; "!teb 00000019\`1cc61000"

- `!dt <module_name>!_teb` shows real structure for `<module_name>`. `<module_name>` can be seen from `lm` commands
- "!dt <module_name>!\_teb <start_address>\'<end_address>" shows actual values of the data members defined in the specified address range

- `!peb` shows Process Environment Block


## Number Conversions

- `? <num>` convert `<num>` hex to decimal
- `? 0n<num>` convert `<num>` decimal to decimal
- `? 0y<num>` convert `<num>` binary to decimal


## Breakpoints

- `bl` lists existing breakpoints
- `bp <module_name>!<function_name>` places a breakpoint in a function defined in specified module; names are case-insensitive; `bp kernel32!createfilew`
- `bd <breakpoint_index>` disable specified breakpoint
- `bd <breakpoint_index> <breakpoint_index2>` disable specified breakpoints
- `bd *` disable all breakpoints
- `bc <breakpoint_index>` deletes specified breakpoint


## Navigations

- `g` continue execution
- `p` step over
- `t` step into function (for assembly , this is `call` instruction)


## Memory View

- `r <register>` display the value in specified `<register>`; for a 64-bit process with intel, calling convention states the first integer/pointer arguments are passed in RCX, RDX, R8, and R9 (in this order).
- `db <memory_addresss>` display in bytes and ASCII characters in right
- `db @<register>`
- `du <memory_addresss>` display as Unicode string
- `du @<register>`

- `!error @<register>` display the textual description of the error; assuming the specified `<register>` has the error code
- `!error @eax` display the return value error as text; use this if the function return the status value; can use with `syscall` because they always return the status as the return value
