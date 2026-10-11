# Search and replace on many lines

## What it is
`:s` (substitute) changes text on a range of lines. Combined with `$`
(end of line) or `^` (start), it adds text to many lines at once, like
the missing `;` in LeetCode schemas.

## Commands
```vim
:%s/$/;/            " every line in the file
:2,5s/$/;/          " lines 2 to 5
:.,.+4s/$/;/        " this line and the 4 below
:'<,'>s/$/;/        " the lines I selected with V (typing : fills in '<,'>)
:%s/^/-- /          " add text at the start: comment out every line
:%s/old/new/g       " replace every 'old' on every line
:%s/old/new/gc      " same, but ask before each one
```

### `:normal` runs keystrokes on each line
```vim
" A = append at end of line, then type ;
:%normal A;
" I = insert at start of the selected lines
:'<,'>normal I-- 
```
Everything after `normal` is typed literally, spaces included, so no
comment on the same line.

### Visual block
`Ctrl-v` → `j` to extend down → `$` to reach each line's end → `A;` → `Esc`.
The `;` appears on every selected line, even ones of different lengths.

## Ranges
| Range | Means |
|---|---|
| `%` | whole file |
| `.` | current line |
| `5` | line 5 |
| `.,.+4` | current line and the next 4 |
| `'<,'>` | last visual selection |

## Gotchas
- Each command is one undo step: `u` reverts the whole change.
- Running `:%s/$/;/` twice gives `;;`.
