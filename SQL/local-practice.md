# Practicing SQL locally (LeetCode)

## What it is
A small database on my own machine, filled with a LeetCode problem's example
tables, so I can run and debug queries before submitting. Setup lives in
`.sql` files I can rerun any time to reset the data.

## SQLite vs PostgreSQL
| | SQLite | PostgreSQL |
|---|---|---|
| Install | `sudo apt install sqlite3` | `sudo apt install postgresql` (includes `psql`) |
| Where data lives | one file I choose, e.g. `leetcode.db` | the server's own storage (`SHOW data_directory;`), not my folder |
| Server to run | none | yes: `sudo service postgresql start` |
| Users | none | needs a database user (below) |
| Good for | quickest start | learning deeply, real projects |

LeetCode itself runs **MySQL**, so neither matches it exactly.
`postgresql-client` alone is only the `psql` program, with no server to talk to.

## Commands

### PostgreSQL one-time setup
```bash
sudo apt install postgresql
sudo service postgresql start              # WSL without systemd: run after each restart
sudo -u postgres createuser -s $USER       # a Postgres user named after me
createdb leetcode                          # a database called leetcode
psql -l                                    # list databases, check it's there
```
`sudo: user 'postgres' not found` means only the client is installed, not the server.

### Connection URLs (psql, dadbod, apps)
```
postgresql://user:password@host:port/database   # full form
postgresql:///leetcode                          # local server, my user, no password
sqlite:/home/pakorn/Leetcode/leetcode.db        # SQLite: just the file path
mysql://root:pass@127.0.0.1:3306/leetcode       # MySQL
```

### Load a problem's tables
1. On the problem page, open **SQL Schema** and copy it into `sql/181.sql`.
2. Add the missing semicolons (one statement per line, so add `;` to every line):
   ```bash
   sed -i 's/$/;/' sql/181.sql      # run once only, or you get ;;
   ```
   In Neovim: `:%s/$/;/`
3. Load it:
   ```bash
   psql leetcode -f sql/181.sql                      # Postgres
   psql leetcode -v ON_ERROR_STOP=1 -f sql/181.sql   # stop at the first error
   sqlite3 leetcode.db < sql/181.sql                 # SQLite
   ```

LeetCode's order is right as-is: `CREATE TABLE IF NOT EXISTS` → `TRUNCATE` →
`INSERT`s. Rerunning the file resets the data. `TRUNCATE` must come after the
table exists and before the inserts.

### Folder layout
```
Leetcode/
├── sql/          # table setup copied from LeetCode
│   └── 181.sql
└── solutions/    # my answers
    └── 181.sql
```

### Checking tables in psql
```
\dt                       -- list tables
\d employee               -- columns of one table
SELECT * FROM employee LIMIT 10;
SELECT COUNT(*) FROM employee;   -- size without printing rows
\x                        -- toggle expanded view for wide rows
\q                        -- quit (Ctrl-d also works)
```
Backslash commands only work in `psql`. The SQL version that works anywhere:
```sql
SELECT table_name FROM information_schema.tables WHERE table_schema = 'public';
```
SQLite equivalents: `.tables`, `.schema employee`, `.headers on`, `.mode column`.

### Reading the psql prompt
| Prompt | Meaning |
|---|---|
| `leetcode=#` | ready for a new command |
| `leetcode-#` | statement not finished, waiting for `;` |
| `leetcode'#` | inside quotes, waiting for a closing `'` |
| `leetcode(#` | unclosed `(` |

Stuck? `Ctrl-c` throws away the unfinished input.

### Big results
Long output opens a pager: `Space` next page, `b` back, `/word` search,
**`q` to quit**. `\pset pager off` turns it off. Prefer `LIMIT` anyway.

## Gotchas
- **Missing `;`** on line 1 makes the error point at line 2
  (`syntax error at or near "Truncate"`): the statement just kept going.
- `//` isn't a SQL comment. Use `--` or `/* ... */`.
- Quoted numbers like `'70000'` into an `INT` column are fine; they're converted.
- `DROP TABLE IF EXISTS x` and `CREATE TABLE IF NOT EXISTS x` only check the
  **name**, never the columns. An old table with different columns from
  another problem is kept as-is by `IF NOT EXISTS`.
- `DROP` removes the whole table; `DELETE FROM x WHERE ...` removes rows;
  `DELETE FROM x` / `TRUNCATE x` empty it but keep the columns.
- The `.sql` file remembers nothing. The database keeps its own catalog of
  tables, which is what `IF EXISTS` checks.
- Typing `'d employee` instead of `\d employee` opens a string (`'#` prompt).
- MySQL-only bits in LeetCode schemas need changing for Postgres:
  `ENUM(...)` → `VARCHAR(20)`, `DATETIME` → `TIMESTAMP`, drop backticks.
- For SQLite, also swap `TRUNCATE TABLE x` for `DELETE FROM x`.

## Real projects
`.sql` files are used for migrations (`001_create_users.sql`, run in order
by a tool), seed data, saved report queries, and reviewed one-off fixes.
They live in the project's git repo next to the code. Apps run most queries
from code; people query by hand to debug, explore, and answer one-off questions.

## Links
- LeetCode SQL 50: https://leetcode.com/studyplan/top-sql-50/
- psql reference: https://www.postgresql.org/docs/current/app-psql.html
