# Everyday shortcuts

## What it is
Quick reference for modes, files, windows, and the custom keys in my config
([dotfiles/nvim](https://github.com/pjflukky/dotfiles/tree/main/nvim)).
Leader is **Space** (`vim.g.mapleader = " "`).

## Modes
| Key | Goes to |
|---|---|
| `Esc` | normal mode (from anywhere) |
| `i` / `a` | insert before / after the cursor |
| `I` / `A` | insert at start / end of the line |
| `o` / `O` | new line below / above and insert |
| `v` / `V` / `Ctrl-v` | visual: characters / lines / block |
| `:` | command line |

## My custom keys
| Key | Does | Set in |
|---|---|---|
| `Space pf` | find files (Telescope) | `remap.lua` |
| `Space pv` | open the file explorer (netrw) | `remap.lua` |
| `Space e` | show the full error under the cursor | `remap.lua` |
| `gd` | go to definition (`Ctrl-o` to come back) | `lsp.lua` |
| `K` | hover docs for the thing under the cursor | `lsp.lua` |
| `Space rn` | rename the symbol everywhere | `lsp.lua` |

## Save and quit
| Command | Does |
|---|---|
| `:w` | save |
| `:q` | quit (refuses if unsaved) |
| `:wq` or `ZZ` | save and quit |
| `:q!` | quit, throw away changes |
| `:wa` / `:qa` | save all / quit all |

## Files and buffers
| Command / key | Does |
|---|---|
| `:e path` | open a file |
| `:e #` or `Ctrl-^` | flip to the previous file |
| `:ls` | list open buffers |
| `:b name` | switch to a buffer (Tab completes) |
| `:bd` | close the current buffer |

## Windows (splits)
| Key | Does |
|---|---|
| `:vsplit` / `:split` | split side by side / top and bottom |
| `Ctrl-w h` `j` `k` `l` | move to the window left / down / up / right |
| `Ctrl-w w` | cycle to the next window |
| `Ctrl-w =` | make all windows the same size |
| `Ctrl-w o` | close every window except this one |
| `Ctrl-w q` | close this window |

## Small edits without insert mode
| Key | Does |
|---|---|
| `r` + char | replace one character |
| `~` | toggle upper / lower case |
| `J` | join the line below onto this one |
| `>>` / `<<` | indent / unindent the line |
| `=ip` | auto-indent the paragraph |
| `gcc` | toggle comment on the line (built in since Neovim 0.10) |

## Help
```vim
:help w            " help for any key or command
:Telescope keymaps " search every mapping that exists
```

## Gotchas
- Leader keys have a short timeout: press `Space` then `pf` without pausing.
- `K` only works in files where an LSP server is attached.
