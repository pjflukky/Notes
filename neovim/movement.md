# Moving around in Neovim

## What it is
Normal-mode keys for getting the cursor where I want it without the mouse
or arrow keys. Most of them take a **count** in front: `5j` = down 5 lines.
My config has `relativenumber` on, so the number next to each line is
exactly the count I need to reach it.

## Basics
| Key | Moves |
|---|---|
| `h` `j` `k` `l` | left, down, up, right |
| `5j` / `5k` | 5 lines down / up (read the number off the gutter) |
| `gg` / `G` | top / bottom of the file |
| `42G` or `:42` | line 42 |

## Inside a line
| Key | Moves to |
|---|---|
| `0` | very start of the line |
| `^` | first non-space character |
| `$` | end of the line |
| `f(` / `F(` | next / previous `(` on this line |
| `t(` / `T(` | just before / just after that `(` |
| `;` / `,` | repeat the last `f`/`t` forward / backward |

## By word
| Key | Moves to |
|---|---|
| `w` / `b` | start of next / previous word |
| `e` | end of the word |
| `W` `B` `E` | same, but a "word" is anything between spaces (`foo.bar()` is one) |

## By screen and block
| Key | Moves |
|---|---|
| `Ctrl-d` / `Ctrl-u` | half a page down / up |
| `Ctrl-f` / `Ctrl-b` | full page down / up |
| `}` / `{` | next / previous blank line (jumps between code blocks) |
| `%` | matching `(` `)` `{` `}` `[` `]` |
| `H` `M` `L` | top / middle / bottom of what's on screen |
| `zz` | scroll so the cursor line is in the middle |

## Searching to move
| Key | Does |
|---|---|
| `/text` then `Enter` | jump to the next `text` |
| `n` / `N` | next / previous match |
| `*` / `#` | next / previous match of the word under the cursor |
| `:noh` | clear the highlight |

Search ignores case unless I type a capital letter (`ignorecase` + `smartcase`).

## Jumping back
| Key | Does |
|---|---|
| `Ctrl-o` | back to where I was before a jump (`gg`, `G`, `/`, `gd`...) |
| `Ctrl-i` | forward again |
| `` `. `` | the last place I edited |
| `ma` then `` `a `` | set mark `a`, jump back to it later |

## Gotchas
- `j`/`k` with a count is the fast way; holding `j` is the slow way.
- `scrolloff = 8` keeps 8 lines visible around the cursor, so `H`/`L`
  stop 8 lines from the edge.
- `0` is a motion, but `10j` still works: a count can't start with 0.
