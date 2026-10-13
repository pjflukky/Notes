# Profiling

## What it is
Measuring where a program actually spends its time and memory,
so I optimize the slow part instead of the part I *think* is slow.

## Quick map
| I want to know... | Tool |
|---|---|
| how long the whole thing takes | `time` |
| is A faster than B (for real)? | `hyperfine` |
| what is my machine busy with right now | `htop`, `btop`, `iotop`, `nethogs` |
| which functions use the CPU | `perf record` + flame graph |
| hardware stats (cache misses, IPC) | `perf stat` |
| how memory grows over time | `valgrind --tool=massif` |
| Python hot spots | `cProfile`, `py-spy` |

## Timing
```bash
time ./prog
# real  0m2.10s   wall clock: start to finish, including waiting
# user  0m1.20s   CPU time spent in my code
# sys   0m0.30s   CPU time spent in the kernel for me
```
- `real` much bigger than `user + sys` → the program is **waiting** (disk, network, sleep).
- `user + sys` bigger than `real` → it's using **multiple cores**.

## Benchmarking: hyperfine
One `time` run is noisy. hyperfine runs many times and does the stats.
```bash
hyperfine 'fd -e md' 'find . -name "*.md"'      # compare two commands
hyperfine --warmup 3 './prog'                   # warm caches first
```
Prints mean ± σ, min/max, and "X is 3.2× faster than Y".

## Resource monitoring
```bash
htop            # live processes; F6 sort, t tree view, h hide/show threads
btop            # fancier htop, adds disk + network graphs
iotop           # which process is hitting the disk
free -h         # memory used / free / cache
lsof -p 1234    # files a process has open
nethogs         # network usage per process
iftop           # network usage per connection
```
Find and kill whatever is using a port:
```bash
ss -tlnp | grep :8080      # t=TCP l=listening n=numeric p=show process
kill <PID>
```
Pin a program to certain CPUs (useful to watch in htop):
```bash
taskset --cpu-list 0,2 stress -c 3
```

## CPU profilers
Two kinds:
- **Tracing**: records every function call. Exact counts, but slows the program a lot.
- **Sampling**: peeks at the call stack every ~1 ms. Small overhead, good enough,
  safe to use even in production. Most tools are this kind.

### perf (Linux, no recompile)
```bash
perf stat ./prog                 # summary counters
perf record -g ./prog            # sample with call stacks → perf.data
perf report                      # browse results in the terminal
```
What `perf stat` shows:
| Counter | Meaning |
|---|---|
| task-clock | CPU time used |
| context-switches | times the OS swapped me out |
| cpu-migrations | moved between cores |
| page-faults | memory pages loaded on first touch |
| cycles / instructions | raw CPU work |
| IPC (insn per cycle) | low (< 1) usually means waiting on memory |
| branch-misses | wrong guesses on if/else; high = unpredictable branches |

Build with `-g -fno-omit-frame-pointer` so stacks come out readable.

### Flame graphs
```bash
perf record -g ./prog
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```
(scripts from Brendan Gregg's FlameGraph repo)

How to read it:
- **Y axis** = call stack (bottom calls the one above)
- **X axis** = share of time (width), **not** time order
- Look for the **wide** boxes — that's where the time goes. Click to zoom.

Instead of the scripts, I can drop `perf script` output into
**Speedscope** (interactive) or **Perfetto** (whole-system timeline).

### Callgrind (Valgrind)
Exact call counts; slow.
```bash
valgrind --tool=callgrind ./prog
callgrind_annotate callgrind.out.<pid>
kcachegrind callgrind.out.<pid>                    # GUI
valgrind --tool=callgrind --cache-sim=yes ./prog   # also simulate cache misses
```

### Language-specific
```bash
python -m cProfile -s cumtime script.py   # Python, tracing
py-spy top --pid 1234                     # Python, sampling, attach to running
py-spy record -o flame.svg -- python script.py
go tool pprof cpu.prof                    # Go
cargo flamegraph                          # Rust
```
Browser / JS: Firefox Profiler, Chrome DevTools Performance tab.

## Memory profilers
```bash
valgrind --tool=massif ./prog
ms_print massif.out.<pid>       # text graph of heap size over time
```
Shows where the heap peaks and which allocations cause it.
Python: `memory-profiler` gives line-by-line memory use.

## Plotting my own measurements
Log numbers in a plain format, then plot:
```text
timestamp,latency_ms
1700000000,12
1700000001,15
```
```bash
gnuplot -e "set datafile separator ','; plot 'data.csv' using 1:2 with lines; pause -1"
```
Or matplotlib / ggplot2. Plots make patterns jump out: periodic slowdowns,
spikes when a vector doubles its size, one category much slower than others.

## Gotchas
- Profile the **optimized** build (`-O2`) — profiling `-O0` code measures the wrong thing.
- Wall time lies when the program is waiting on I/O; a CPU profiler won't show waiting.
- Flame graph X axis is not a timeline.
- `perf` may need `sudo` or `sysctl kernel.perf_event_paranoid=1`.
- Tracing profilers (cProfile, callgrind) inflate the cost of tiny functions called many times.

## Links
- [Missing Semester: Debugging and Profiling](https://missing.csail.mit.edu/2026/debugging-profiling/)
- [FlameGraph scripts](https://github.com/brendangregg/FlameGraph)
- [Speedscope](https://www.speedscope.app/)
- [hyperfine](https://github.com/sharkdp/hyperfine)
