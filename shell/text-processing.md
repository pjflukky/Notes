# Text processing

## What it is
Small tools that each do one job on text: search it, edit it, pick columns,
sort it, count it. The power comes from chaining them with `|`.
Most of them read a file if I give one, or read stdin (a pipe) if I don't.

## Quick map
| I want to... | Tool |
|---|---|
| print a file | `cat` |
| show first / last lines | `head` / `tail` |
| find lines that match | `grep` |
| find files | `find` |
| replace text | `sed` |
| pick out columns | `awk` |
| sort lines | `sort` |
| remove / count duplicates | `uniq` |
| count lines, words | `wc` |
| turn lines into arguments | `xargs` |

## grep: find matching lines
```bash
grep "error" log.txt        # lines containing "error"
grep -i "error" log.txt     # ignore case
grep -r "TODO" .            # search every file in this folder, recursively
grep -l "TODO" *.py         # only print file names that match
grep -v "debug" log.txt     # lines that do NOT match
grep -c "error" log.txt     # count matching lines
```
The pattern is a **regular expression**, not just plain text.
`rg` (ripgrep) does the same thing, faster, and is recursive by default.

## find: find files
```bash
find . -name "*.md"                         # all .md files under here
find . -type f -name "*.py"                 # only files (not folders)
find . -type d -name "build"                # only folders
find ~/Downloads -name "*.zip" -mtime +30   # zips older than 30 days
find ~ -type f -size +100M                  # files bigger than 100 MB
find . -name "*.py" -exec grep -l "TODO" {} \;   # run a command on each one
```
In `-exec`, `{}` gets replaced by each file path, and `\;` marks the end
of the command. `fd` is the friendlier version.

## sed: edit text
```bash
sed 's/old/new/' file          # replace the first match on each line, print result
sed 's/old/new/g' file         # replace ALL matches on each line
sed -i 's/old/new/g' file      # change the file itself (in place)
sed -E 's/(.*)@(.*)/\2/' file  # -E for modern regex; \1 \2 = captured groups
```
How to read `s/old/new/g`: **s**ubstitute / pattern / replacement / **g**lobal.

## awk: pick out columns
```bash
awk '{print $2}' file              # 2nd column (split on spaces)
awk -F, '{print $2}' data.csv      # 2nd column of a CSV
awk '{print $1, $3}' file          # columns 1 and 3
awk '$2 > 100' file                # only lines where column 2 > 100
awk '$3 ~ /err/ {print $1}' file   # column 1, where column 3 matches "err"
awk '{sum += $2} END {print sum}' file   # add up column 2
```
`$0` is the whole line. `NF` is the number of columns.

## sort, uniq, wc
```bash
sort file              # alphabetical
sort -n file           # by number
sort -r file           # reverse
sort -nk2 file         # by number, using column 2
uniq file              # remove duplicates that are NEXT TO each other
uniq -c file           # same, but count them
wc -l file             # count lines
```
The classic "top 10" pattern:
```bash
... | sort | uniq -c | sort -n | tail -n 10
```
1. `sort` puts duplicates next to each other
2. `uniq -c` counts each one
3. `sort -n` orders by count
4. `tail` keeps the 10 biggest

## xargs: lines → arguments
Some commands don't read stdin; they want arguments. `xargs` converts.
```bash
find . -name "*.sh" | xargs wc -l                 # line count of each .sh file
find . -name "*.sh" -print0 | xargs -0 wc -l      # safe with spaces in names
```

## Other handy ones
```bash
cut -d, -f2 data.csv      # 2nd field of a CSV (simpler than awk)
paste -sd, file           # join all lines into one, comma-separated
tr 'a-z' 'A-Z'            # change characters (here: to uppercase)
curl -s URL               # download a page, print it (-s = no progress bar)
jq '.[] | .name'          # pull fields out of JSON
```

## Putting it together
Most common file extensions in my home folder:
```bash
find ~ -type f -name "*.*" | sed 's/.*\.//' | sort | uniq -c | sort -n | tail -n 5
```
- `find` lists every file that has a dot in its name
- `sed` deletes everything up to the last `.`, leaving the extension
- the rest is the "top N" pattern

My most-used commands:
```bash
awk '{print $1}' ~/.bash_history | sort | uniq -c | sort -n | tail -n 10
```
(zsh history lines start with `: 12345:0;`, so strip that first with
`sed 's/^.*;//'`.)

## Gotchas
- `uniq` only removes duplicates that are **next to each other**. Always `sort` first.
- `sed -i` overwrites the file. Try without `-i` first to preview.
- On macOS, `sed -i` needs an argument: `sed -i '' 's/a/b/' file`.
- Quote patterns with `'single quotes'` so the shell doesn't touch `$`, `*`, etc.
- `find -name *.md` without quotes breaks: the shell expands `*` before
  `find` sees it. Write `-name "*.md"`.
- Plain `xargs` splits on spaces, so file names with spaces break.
  Use `-print0 | xargs -0`.

## Links
- [Missing Semester: Course overview + the shell](https://missing.csail.mit.edu/2026/course-shell/)
- [Regex section (Code quality lecture)](https://missing.csail.mit.edu/2026/code-quality/#regular-expressions)
- [ripgrep](https://github.com/BurntSushi/ripgrep), [fd](https://github.com/sharkdp/fd), [jq](https://jqlang.github.io/jq/)
