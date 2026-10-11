# Shell basics

## What it is
The shell is the program that reads what I type in the terminal and runs it.
I use **zsh** for typing and **bash** for scripts. Almost all commands below
work the same in both.

## Moving around
```bash
pwd                 # where am I?
ls                  # list files
ls -la              # list all files (incl. hidden) with details
cd folder           # go into a folder
cd ..               # go up one level
cd ~                # go home
cd -                # go back to the previous folder
```

**Absolute vs relative paths**
- Absolute starts with `/` → full path from the root: `/home/pakorn/notes`
- Relative doesn't → starts from where I am now: `notes/shell`
- `.` = this folder, `..` = the parent folder

## Files and folders
```bash
mkdir notes             # make a folder
mkdir -p a/b/c          # make nested folders in one go
touch file.md           # create an empty file
cp a.txt b.txt          # copy
cp -r src/ backup/      # copy a folder
mv old.txt new.txt      # move or rename
rm file.txt             # delete a file (no trash bin!)
rm -r folder/           # delete a folder and everything in it
```

## Reading files
```bash
cat file.txt            # print the whole file
less file.txt           # scroll through (q to quit)
head -n 5 file.txt      # first 5 lines
tail -n 5 file.txt      # last 5 lines
tail -f log.txt         # keep watching a file as it grows
```

## Searching
```bash
rg "word"               # search inside files (ripgrep)
rg -i "word" ~/notes    # ignore case, in a specific folder
rg --files | fzf        # pick a file by name
fdfind name             # find files by name
```

## Combining commands
```bash
cmd1 | cmd2             # pipe: output of cmd1 goes into cmd2
cmd > out.txt           # write output to a file (overwrites)
cmd >> out.txt          # append output to a file
cmd1 && cmd2            # run cmd2 only if cmd1 succeeded
cmd1 || cmd2            # run cmd2 only if cmd1 failed
cmd < in.txt            # read input from a file instead of the keyboard
cmd 2> err.txt          # write only the errors to a file
cmd > out.txt 2>&1      # write output AND errors to the same file
cmd &> all.txt          # same thing, shorter (bash/zsh)
cmd | tee log.txt       # show output on screen AND save it to a file
```

Example: `ls | wc -l` counts the files in a folder.

**The 3 streams**
| Number | Name | What it is |
|---|---|---|
| 0 | stdin | input (normally the keyboard) |
| 1 | stdout | normal output |
| 2 | stderr | error messages |

A pipe `|` only passes **stdout**, so errors still show on screen.

## Variables
```bash
name="Pakorn"           # no spaces around =
echo "Hi $name"         # use it with $
today=$(date +%F)       # store a command's output
echo $HOME $USER $PATH  # built-in variables
```

## Permissions and running scripts
```bash
chmod +x script.sh      # make a script executable
./script.sh             # run it (uses the shell in its #! line)
bash script.sh          # run it with bash explicitly
sudo cmd                # run as administrator
```

## Getting help
```bash
man ls                  # full manual (q to quit)
ls --help               # quick summary of flags
which nvim              # where a command lives
tldr tar                # short, example-based help (install tldr first)
type cd                 # tells me if something is a program or a shell builtin
```

**How the shell finds a program**
When I type `date`, the shell looks through the folders in `$PATH`
(separated by `:`) from left to right and runs the first `date` it finds.
```bash
echo $PATH              # see the list of folders
/bin/echo hi            # skip $PATH by giving the full path
```
`cd` is not a program in `$PATH`. It's built into the shell, because a
separate program can't change the folder of the shell that started it.

## Shortcuts while typing
| Keys | Does |
|---|---|
| `Tab` | autocomplete |
| `→` | accept zsh autosuggestion |
| `Ctrl+R` | search command history |
| `Ctrl+C` | stop the running command |
| `Ctrl+L` | clear the screen |
| `Ctrl+A` / `Ctrl+E` | jump to start / end of line |
| `Ctrl+U` | delete the whole line |
| `!!` | repeat the last command (`sudo !!` is handy) |

## Gotchas
- `rm` is permanent: there's no trash bin. Double-check before `rm -r`.
- No spaces around `=` in variables: `x=5` works, `x = 5` doesn't.
- Quote variables that might contain spaces: `"$file"`, not `$file`.
- `>` overwrites the file. Use `>>` to add to it.
- `source file` runs it in the **current** shell; `./file` runs it in a new one.
- Spaces split arguments. For a name with spaces, quote it or escape it:
  `cd "My Photos"` or `cd My\ Photos`.
- Quotes behave differently:

| Quote | `$var` and `$(cmd)` expanded? | Example |
|---|---|---|
| `'single'` | No, everything is literal | `echo '$HOME'` → `$HOME` |
| `"double"` | Yes | `echo "$HOME"` → `/home/pakorn` |
| `$'ansi'` | No, but `\n` `\t` work | `echo $'a\nb'` → two lines |

## Example
[examples/organize.sh](examples/organize.sh) sorts files in a folder into
subfolders by extension. Shows variables, arguments, `if`, `for`, functions.

## See also
- [text-processing.md](text-processing.md): grep, sed, find, awk, sort, xargs
- [bash-scripting.md](bash-scripting.md): writing real scripts

## Links
- [Missing Semester: Course overview + the shell](https://missing.csail.mit.edu/2026/course-shell/)
