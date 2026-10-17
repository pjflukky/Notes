# WHERE with Subqueries (and DELETE)

## What it is
A `WHERE` can filter using the result of another query (a **subquery**), not just
fixed values. This works the same in `SELECT`, `UPDATE` and `DELETE`, and it's how
`DELETE` gets to use things like `GROUP BY` that it can't use directly.

Examples use the `school` tables from [sql-basics.md](sql-basics.md).

## Commands

### `IN` / `NOT IN`: compare against a list from a subquery
```sql
-- students enrolled in at least one course
SELECT * FROM students
WHERE id IN (SELECT student_id FROM enrollments);

-- students enrolled in nothing
SELECT * FROM students
WHERE id NOT IN (SELECT student_id FROM enrollments);
```
The subquery must return **one column**.

### `EXISTS` / `NOT EXISTS`: does a related row exist?
```sql
SELECT * FROM students s
WHERE EXISTS (
    SELECT 1 FROM enrollments e
    WHERE e.student_id = s.id          -- refers to the outer row (correlated)
);
```
Only checks whether the subquery returns *any* row; what it selects doesn't matter (`SELECT 1` is the habit).

### Compare with a single-value subquery
```sql
-- students above the average GPA
SELECT * FROM students
WHERE gpa > (SELECT AVG(gpa) FROM students);
```
The subquery must return exactly **one row, one column**, or it errors.

### Reaching a column after `GROUP BY`
After grouping, a bare column like `id` is ambiguous (a group can hold many ids),
so wrap it in an aggregate to get one value per group:
```sql
SELECT major, MIN(id), MAX(id), COUNT(*)
FROM students
GROUP BY major;

-- PostgreSQL: see every id in each group, handy for exploring
SELECT major, array_agg(id) FROM students GROUP BY major;
```
`COUNT(*)` = how many rows share that value. `1` means unique, `> 1` means repeated.

### `DELETE` with a subquery
`DELETE` has no `GROUP BY` clause, but its `WHERE` can use a grouped subquery:
```sql
DELETE FROM some_table
WHERE some_col NOT IN (
    SELECT ... FROM ... GROUP BY ...
);
```
The subquery decides *which* rows matter; the outer `DELETE` removes rows based on it.

### `DELETE` with a join (no subquery)
```sql
-- MySQL
DELETE e FROM enrollments e
JOIN courses c ON c.id = e.course_id
WHERE c.code = 'ECE 552';

-- PostgreSQL
DELETE FROM enrollments e
USING courses c
WHERE c.id = e.course_id AND c.code = 'ECE 552';
```
The joined table can also be the **same table under another alias** (a self-join).

### MySQL error 1093 workaround
MySQL refuses to delete from a table that a subquery in the same statement reads:
`You can't specify target table ... for update in FROM clause`.
Wrap the subquery once more so MySQL materializes it first:
```sql
DELETE FROM t
WHERE col NOT IN (
    SELECT col FROM (
        SELECT ... FROM t GROUP BY ...
    ) AS tmp                            -- alias is required
);
```
PostgreSQL doesn't have this restriction.

## Gotchas
- **`NOT IN` + `NULL`**: if the subquery returns any `NULL`, `NOT IN` matches nothing. Use `NOT EXISTS` or filter `WHERE col IS NOT NULL` in the subquery.
- `DELETE ... GROUP BY` is a syntax error; put the grouping in a subquery.
- `SELECT *` (or any bare column) with `GROUP BY` errors in PostgreSQL and in MySQL's default `ONLY_FULL_GROUP_BY` mode.
- A self-join on just `p.col = q.col` also matches every row **with itself**; add a condition that tells the two copies apart.
- Comparison is `=`, not `==`. Comments are `--` (or `/* */`, MySQL also `#`), not `//`.
- Run the `WHERE` part as a `SELECT` before turning it into a `DELETE`.

## Which one to use
| Form | Good for |
| --- | --- |
| `IN` / `NOT IN` | "is this value in that list" |
| `EXISTS` / `NOT EXISTS` | "is there a matching row"; safe with NULLs |
| `= (subquery)` | comparing to one computed value (avg, max, ...) |
| `DELETE ... JOIN` / `USING` | removing rows based on another (or the same) table |

## Links
- PostgreSQL subquery expressions: https://www.postgresql.org/docs/current/functions-subquery.html
- PostgreSQL DELETE: https://www.postgresql.org/docs/current/sql-delete.html
- MySQL DELETE: https://dev.mysql.com/doc/refman/8.0/en/delete.html
- MySQL subquery restrictions (error 1093): https://dev.mysql.com/doc/refman/8.0/en/subquery-restrictions.html
