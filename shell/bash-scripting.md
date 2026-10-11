# Bash scripting

## What it is
Bash is a full programming language: variables, if, loops, functions.
Anything I type in the terminal is a tiny program. A script is the same
thing saved in a `.sh` file so I can rerun it.

## Starting a script
```bash
#!/bin/bash
set -euo pipefail

echo "Hello from $0"
```
Then:
```bash
chmod +x hello.sh      # allow it to run (only once)
./hello.sh             # run it
```

**Line 1: the shebang (`#!`)**
Tells the system which program runs this file. Works for other languages
too: `#!/usr/bin/env python3`.

**Line 2: strict mode**
| Flag | Meaning |
|---|---|
| `-e` | stop the script as soon as any command fails |
| `-u` | using an undefined variable is an error (not a silent empty string) |
| `-o pipefail` | a pipe fails if ANY part fails, not just the last |
| `-x` | (for debugging) print each command before running it |

## Arguments
```bash
./script.sh one two three
```
| Variable | Value |
|---|---|
| `$0` | the script's name |
| `$1`, `$2` ... | first, second ... argument |
| `$#` | how many arguments (here: 3) |
| `"$@"` | all arguments, each kept separate (use this one) |
| `$?` | exit code of the last command (0 = success) |
| `$$` | this script's process ID |
| `$!` | process ID of the last background job |

## Exit codes
Every command returns a number when it ends. **0 = success, anything else = failure.**
```bash
ls /nope
echo $?                     # 2 (failed)
mkdir -p /tmp/a && echo ok  # && and || use the exit code
[ -d /tmp/mydir ] || mkdir /tmp/mydir   # create only if missing
exit 1                      # end my script with "failed"
```

## if
```bash
if [ -f "$1" ]; then
    echo "$1 exists"
elif [ -d "$1" ]; then
    echo "$1 is a folder"
else
    echo "$1 not found"
fi
```
`if` just runs a command and checks its exit code. `[ ]` is really the
`test` command. `[[ ]]` is bash's safer built-in version; prefer it in bash.

**Common tests**
| Test | True when |
|---|---|
| `-f file` | file exists (and is a regular file) |
| `-d dir` | folder exists |
| `-e path` | anything exists there |
| `-z "$s"` | string is empty |
| `-n "$s"` | string is not empty |
| `"$a" = "$b"` | strings are equal |
| `"$a" != "$b"` | strings are different |
| `$x -eq $y` | numbers are equal (also `-ne -lt -le -gt -ge`) |
| `! test` | flips the result |

## Loops
```bash
for name in alice bob carol; do
    echo "hi $name"
done

for f in *.md; do               # every .md file here
    echo "$f"
done

for i in $(seq 1 5); do         # 1 to 5
    echo "$i"
done

while ! ping -c1 google.com; do  # keep trying until it works
    sleep 1
done
```

## Command substitution and math
```bash
today=$(date +%Y-%m-%d)          # store a command's output
cp notes.txt "notes_$today.txt"  # → notes_2026-10-11.txt
count=$(ls | wc -l)

n=5
((n++))                          # arithmetic: n is now 6
echo $((n * 2))                  # 12
```
Use `$(...)`, not old backticks `` `...` ``, because `$()` can be nested.

## Functions
```bash
greet() {
    local name="$1"     # local = only exists inside the function
    echo "hi $name"
}
greet Pakorn
```

## Background jobs
```bash
long_task &             # & = run in the background, keep going
pid=$!                  # remember its process ID
# ... do other things ...
kill $pid               # stop it
wait                    # or: wait for all background jobs to finish
```

## Full example: rerun a test until it fails
```bash
#!/bin/bash
set -euo pipefail

log="run_$(date +%s).log"
run=1
while "$@" > "$log" 2>&1; do     # run whatever command I passed in
    echo "Run $run passed"
    run=$((run + 1))
done
echo "Failed on run $run. Last lines:"
tail -n 20 "$log"
```
Use: `./until-fail.sh cargo test my_test`
Handy for finding flaky tests.

## Gotchas
- No spaces around `=`: `x=5`, not `x = 5`.
- `[ ]` needs spaces inside: `[ -f "$f" ]`, not `[-f "$f"]`.
- Always quote variables: `"$1"`, `"$@"`. Unquoted, a name with spaces
  turns into several arguments.
- `"$@"` keeps arguments separate; `"$*"` glues them into one string.
- `./script.sh` fails with "Permission denied" until I run `chmod +x`.
- `((n++))` returns "failed" when n was 0, which kills a `set -e` script.
  Use `n=$((n + 1))` to be safe.
- When a script gets past ~100 lines, rewrite it in Python.
- Run every script through **shellcheck**. It catches most of these.

## Links
- [Missing Semester: Course overview + the shell](https://missing.csail.mit.edu/2026/course-shell/)
- [ShellCheck](https://www.shellcheck.net/)
- [Bash Pitfalls](https://mywiki.wooledge.org/BashPitfalls)
- Bash manual: [Conditional Expressions](https://www.gnu.org/software/bash/manual/html_node/Bash-Conditional-Expressions.html), [Special Parameters](https://www.gnu.org/software/bash/manual/html_node/Special-Parameters.html), [Redirections](https://www.gnu.org/software/bash/manual/html_node/Redirections.html)
