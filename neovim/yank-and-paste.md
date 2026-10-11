# Copy, cut, paste (yank, delete, put)

## What it is
Vim calls copy **yank** (`y`), cut **delete** (`d`), and paste **put** (`p`).
They combine with any motion from [movement.md](movement.md):
**operator + motion**, e.g. `y` + `w` = yank a word, `d` + `$` = delete to
end of line.

My config has `vim.opt.clipboard = "unnamedplus"`, so everything I yank or
delete also goes to the system clipboard. `Cmd/Ctrl-v` in the browser pastes
what I yanked in Neovim, and `p` in Neovim pastes what I copied outside.

## The operators
| Key | Does |
|---|---|
| `y` | yank (copy) |
| `d` | delete (and keep it, so it's also a cut) |
| `c` | delete, then go into insert mode |
| `p` / `P` | put after / before the cursor |

Doubling an operator acts on the whole line:
| Key | Does |
|---|---|
| `yy` | copy this line |
| `dd` | cut this line |
| `cc` | clear this line and start typing |
| `3yy` / `3dd` | 3 lines |

## Common combos
| Keys | Does |
|---|---|
| `yw` / `dw` | copy / delete to the start of the next word |
| `y$` / `D` | copy / delete to end of line |
| `C` | change to end of line |
| `dG` / `dgg` | delete to bottom / top of file |
| `ggyG` | copy the whole file |
| `x` | delete one character |
| `yyp` | duplicate the line |
| `ddp` | swap this line with the one below |

## Text objects: "inside" and "around"
Work from anywhere inside the thing, no need to move to its start first.

| Keys | Acts on |
|---|---|
| `iw` / `aw` | the word / the word plus its space |
| `i"` / `a"` | inside the quotes / quotes included |
| `i(` / `a(` | inside the parentheses / parens included |
| `i{` / `a{` | inside the braces (a function body) |
| `ip` / `ap` | the paragraph (block of lines) |

```text
ciw     change the word under the cursor
di"     empty the string, keep the quotes
yi(     copy the function arguments
ca{     replace a whole { ... } block
dap     delete the block of lines and the blank line after it
```

## Visual mode: select first, then act
| Key | Selects |
|---|---|
| `v` | characters |
| `V` | whole lines |
| `Ctrl-v` | a rectangle (column) |

Move with any motion to grow the selection, then `y`, `d`, `c`, `>` / `<`
(indent), or `p` to replace the selection with what I yanked.

## Undo and repeat
| Key | Does |
|---|---|
| `u` | undo |
| `Ctrl-r` | redo |
| `.` | repeat the last change (e.g. `ciw` new word, then `n` `.` `n` `.`) |

## Registers: why paste gave me the wrong thing
Every `d`, `c`, `x` also overwrites what `p` pastes (and with my clipboard
setting, the system clipboard too). Two ways around it:

| Keys | Does |
|---|---|
| `"0p` | paste the last thing I **yanked**, ignoring later deletes |
| `"_d` | delete into the "black hole", keeps my yank safe |
| `P` in visual mode | replace the selection without losing the yank |
| `:reg` | list what's in every register |

## Gotchas
- `x` and `dd` overwrite the clipboard too. If I yank, then delete a line,
  then `p`, I get the deleted line. Use `"0p`.
- `p` after `yy` puts the line **below**; `P` puts it above.
- Pasting with `Ctrl-Shift-v` in insert mode can mess up indentation;
  `Esc` then `p` doesn't.
