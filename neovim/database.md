# Databases in Neovim (vim-dadbod)

## What it is
**vim-dadbod** lets me run SQL from Neovim against any database (Postgres,
SQLite, MySQL). It uses the database's own CLI behind the scenes (`psql`,
`sqlite3`, `mysql`), so that program has to be installed.

| Plugin | Does |
|---|---|
| `tpope/vim-dadbod` | the core: the `:DB` command |
| `kristijanhusak/vim-dadbod-ui` | optional sidebar to browse tables (`:DBUI`) |
| `kristijanhusak/vim-dadbod-completion` | nvim-cmp source: real table and column names |

Completion is set per filetype in `cmp-config.lua`, so other languages are unaffected:
```lua
cmp.setup.filetype({ "sql", "mysql", "plsql" }, {
  sources = cmp.config.sources({
    { name = "vim-dadbod-completion" },
    { name = "buffer" },
  }),
})
```

## Commands

### Without the sidebar: `:DB`
```vim
" one-off with a URL
:DB postgresql:///leetcode SELECT * FROM employee;
" set a default database, then just the query
:let g:db = 'postgresql:///leetcode'
:DB SELECT * FROM employee;
" run the whole file / the selected lines
:%DB
:'<,'>DB
```
Results open in a split; `:q` closes it.

For `:DB` to exist in a plain `.sql` file, vim-dadbod must load on its own,
not only as a dependency of dadbod-ui:
```lua
{ "tpope/vim-dadbod", cmd = "DB", ft = { "sql", "mysql", "plsql" } },
```

### With the sidebar: `:DBUI`
| Key / command | Does |
|---|---|
| `:DBUI` / `:DBUIToggle` | open / hide the panel |
| `A` | add a connection (URL + name), saved outside my dotfiles |
| `Enter` / `o` | expand: database → Tables → table |
| on a table | **List** (rows) or **Columns** (structure) |
| **New query** | blank SQL buffer for this database |
| `<leader>S` | run the query buffer (or the visual selection) |
| `:w` | save the query under **Saved queries** |
| `?` | all panel keys |

### Database next to my script
```vim
:vsplit | terminal psql leetcode
```
Script on the left, live `psql` on the right (see [terminal.md](terminal.md)).

## Gotchas
- **Don't name my own config files after plugin modules.** My
  `lua/completion.lua` broke vim-dadbod-completion, which calls
  `require("completion")` to look for an old plugin and got my file instead
  (`attempt to index local 'completion' (a boolean value)`). Renamed it to
  `cmp-config.lua`. Safer still: put everything under `lua/pakorn/`.
- Dadbod doesn't create databases; it connects to existing ones.
- Passwords: connections added with `A` aren't in my config. If I ever write
  one in Lua, read it from `os.getenv("DATABASE_URL")` instead.
- `.sql` files colored faintly? Neovim has no Treesitter parser for SQL
  built in, so it uses the old regex highlighter. `:set ft?` should say `sql`.
  nvim-treesitter with the `sql` parser fixes it.

## Links
- vim-dadbod: https://github.com/tpope/vim-dadbod
- vim-dadbod-ui: https://github.com/kristijanhusak/vim-dadbod-ui
- vim-dadbod-completion: https://github.com/kristijanhusak/vim-dadbod-completion
