# Debugging

## What it is
Finding out *why* a program does what it does, instead of guessing.
Start with the cheapest tool (a print), and reach for heavier tools
(debugger, tracer, sanitizer) only when the cheap one stops helping.

## Quick map
| Problem | Tool |
|---|---|
| "what value is this right here?" | print / logging |
| don't know where to look, or crash | debugger: `gdb`, `lldb`, `pdb` |
| bug that only shows up sometimes | record-replay: `rr` |
| "what is it doing with files / the OS?" | `strace` / `dtruss` / `bpftrace` |
| network traffic | `tcpdump`, Wireshark, mitmproxy |
| memory corruption, leaks, races (C/C++) | sanitizers, `valgrind` |
| cryptic error message | ask an LLM, then verify |

## Print debugging and logging
- Put a print near where I think the bug is, run, move it, repeat.
- A **logger** is a better print: severity levels (`DEBUG`, `INFO`, `WARN`,
  `ERROR`), can go to a file, can be turned up/down without editing code.
- When a debug print turned out useful, turn it into a log line instead of deleting it.

Logs from other programs:
```bash
some-tool -v              # verbose; some tools take -vv, -vvv for more
ls /var/log/              # system and service logs
journalctl -u nginx       # logs for one systemd service
journalctl -u nginx -f    # follow live
```
Many libraries also turn on debug logging with an environment variable or config option.

## Debuggers
Use one when I don't know which variable to print, the bug is hard to reproduce,
or restarting the program is slow. Lets me pause, step, and look at everything.

- `gdb` / `lldb`: C, C++, Rust (anything native)
- `pdb`: Python (`python -m pdb script.py`, or `breakpoint()` in code)

Compile with debug info first, or the debugger can't show names and lines:
```bash
gcc -g -O0 -o prog prog.c
```

### gdb cheat sheet
```text
gdb ./prog            start gdb on a program
gdb -tui ./prog       with a source window (or Ctrl-x a inside gdb)

run (r)               start the program
b main                breakpoint on a function
b file.c:42           breakpoint on a line
b file.c:42 if i == 7 conditional breakpoint
c                     continue to next breakpoint
next (n)              next line, step OVER function calls
step (s)              next line, step INTO function calls
finish                run until current function returns
p x                   print a variable / expression
bt                    backtrace: how did I get here?
watch x               stop whenever x changes
q                     quit
```
After a crash (segfault), `bt` right away shows the exact line and call chain.

## Record-replay: rr
For **Heisenbugs**: race conditions, timing bugs, things that fail 1 run in 20.
`rr` records one run, then replays it exactly the same every time,
and I can run it **backwards**.

```bash
rr record ./prog      # run until it fails (may take a few tries)
rr replay             # opens gdb on the recording
```
Extra gdb commands inside `rr replay`:
```text
reverse-continue (rc)   run backwards to previous breakpoint/watchpoint
reverse-step (rs)       step back one line
reverse-next (rn)       step back over a call
reverse-finish          back to where the current function was called
```
Classic workflow for "who corrupted this value?":
1. `c` until it crashes
2. `p` the bad value
3. `watch` that variable / memory address
4. `rc` — stops at the line that wrote the bad value

## System call tracing
Watches what the program asks the kernel for: opening files, allocating memory,
starting processes, network. Good when the code is not mine.

### strace (Linux)
```bash
strace ./prog                  # every syscall
strace -e trace=file ./prog    # only file-related ones (open, stat, ...)
strace -f ./prog               # also follow child processes
strace -p 1234                 # attach to a running process
strace -T ./prog               # show time spent in each call
```
On macOS the equivalent is `dtruss` (built on `dtrace`).

### bpftrace (eBPF)
Lower overhead than strace, can count/aggregate, sees the whole kernel. Needs `sudo`.
```bash
# count syscalls by name, system-wide (Ctrl-C to print)
sudo bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[comm] = count(); }'

# print files opened by bash
sudo bpftrace -e 'tracepoint:syscalls:sys_enter_openat /comm == "bash"/ { printf("%s\n", str(args->filename)); }'
```
bcc ready-made tools: `opensnoop` (who opens what), `biosnoop` (disk I/O latency).

strace = easy to start. bpftrace = when strace is too slow or I need counts.

## Network debugging
```bash
sudo tcpdump -i any port 80              # watch traffic on port 80
sudo tcpdump -i any -w capture.pcap      # save, then open in Wireshark
```
HTTPS is encrypted, so tcpdump only shows noise. Use **mitmproxy**
(proxy that decrypts) or the browser DevTools → Network tab.

## Memory debugging (C / C++)
### Sanitizers
Compiler adds checks to the program. Fast enough to keep on in tests / CI.
```bash
gcc -g -fsanitize=address   prog.c   # ASan: overflow, use-after-free, leaks
gcc -g -fsanitize=thread    prog.c   # TSan: data races
clang -g -fsanitize=memory  prog.c   # MSan: reading uninitialized memory (clang only)
gcc -g -fsanitize=undefined prog.c   # UBSan: undefined behavior, e.g. int overflow
```
When it finds something, it prints the bad access **and** where the memory
was allocated and freed — usually enough to fix it on the spot.

### Valgrind
No recompiling needed, but much slower.
```bash
valgrind --leak-check=full ./prog
```

## Debug symbols
- `-g` puts debug info (DWARF) in the binary → names and line numbers in gdb, sanitizers, profilers.
- `-fno-omit-frame-pointer` → better stack traces, especially for profilers.
- Matters for C/C++/Rust. Python and Java already keep this info.

## AI for debugging
Good at:
- explaining scary compiler errors (C++ templates, Rust borrow checker)
- build / linker / cross-language issues
- reading a stack trace or crash dump and guessing a cause

But: it can make things up, or "fix" the symptom and hide the bug.
Always confirm with a real tool (debugger, test, sanitizer).

## Gotchas
- `-O2` code is hard to debug: variables "optimized out", lines jump around. Use `-O0 -g`.
- rr only works on Linux with hardware perf counters — most cloud VMs (e.g. EC2) don't have them.
- rr runs threads one at a time, so some races need several recordings to show up.
- ASan and TSan can't be used together in the same build.
- bpftrace needs root and sees **everything** on the machine — filter by `comm` or `pid`.

## Links
- [Missing Semester: Debugging and Profiling](https://missing.csail.mit.edu/2026/debugging-profiling/)
- [rr project](https://rr-project.org/)
- [bpftrace one-liners](https://github.com/bpftrace/bpftrace/blob/master/docs/tutorial_one_liners.md)
