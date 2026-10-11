# Running shell commands from Neovim

## What it is
Ways to use the terminal without leaving Neovim. Which one depends on
whether the command runs once or needs me to keep typing into it.

## Commands
| Way | Use for |
|---|---|
| `:!cmd` | run once, see output, press Enter to come back |
| `:terminal cmd` | interactive programs: a shell, `psql`, `python3` |
| `Ctrl-z`, then `fg` | pause Neovim, use the real shell, come back |
| `:r !cmd` | insert a command's output below the cursor |

```vim
:!ls
:!mv lua/completion.lua lua/cmp-config.lua
" % = the current file
:!python3 %
" reopen a file I just renamed
:e lua/cmp-config.lua

" shell in this window / below / psql beside my code
:terminal
:split | terminal
:vsplit | terminal psql leetcode

:r !date
```
Comments go on their own line: after `:!` or `:terminal`, a `"` would be
sent to the shell as part of the command.

`:terminal` needs no `!`: it already means "run this program". `:!psql` would
start psql but I couldn't type into it properly.

### Inside a terminal window
| Keys | Does |
|---|---|
| `i` | start typing into the terminal |
| `Ctrl-\` `Ctrl-n` | back to normal mode (scroll, switch windows) |
| `Ctrl-w h` / `Ctrl-w l` | move to the window on the left / right |
| `exit` / `\q` / `Ctrl-d` | end the program, then any key closes the window |
| `:q!` (from normal mode) | close it directly; plain `:q` refuses |

Nicer keys in `remap.lua`:
```lua
vim.keymap.set("t", "<Esc>", [[<C-\><C-n>]])   -- Esc leaves terminal mode
vim.keymap.set("n", "<leader>db", "<cmd>vsplit | terminal psql leetcode<CR>i")
```

## Gotchas
- A running terminal counts as unsaved, so closing it needs `:q!`.
- `Ctrl-z` leaves Neovim **paused**, not closed. `fg` brings it back;
  `jobs` lists what's paused.
