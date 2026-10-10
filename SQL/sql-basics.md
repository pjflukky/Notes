# SQL Basics

## What it is
SQL (Structured Query Language) is the language for talking to relational databases: you describe *what* data you want, and the database figures out *how* to get it. Data lives in **tables** (rows + columns), and tables relate to each other through **keys**. MySQL and PostgreSQL both speak standard SQL with small dialect differences (noted below).

## Setup
```bash
# PostgreSQL
psql -U postgres                 # open the shell
\l                               # list databases
\c school                        # connect to a database
\dt                              # list tables
\d students                      # describe a table
\q                               # quit

# MySQL
mysql -u root -p                 # open the shell
SHOW DATABASES;
USE school;
SHOW TABLES;
DESCRIBE students;
exit
```

## Commands

### Create a database and tables
```sql
CREATE DATABASE school;

CREATE TABLE students (
    id          SERIAL PRIMARY KEY,          -- MySQL: INT AUTO_INCREMENT PRIMARY KEY
    name        VARCHAR(100) NOT NULL,
    major       VARCHAR(50),
    gpa         NUMERIC(3,2),                -- e.g. 3.75
    enrolled_on DATE DEFAULT CURRENT_DATE
);

CREATE TABLE courses (
    id      SERIAL PRIMARY KEY,
    code    VARCHAR(20) UNIQUE NOT NULL,     -- e.g. 'ECE 552'
    title   VARCHAR(100),
    credits INT CHECK (credits > 0)
);

-- junction table: many students <-> many courses
CREATE TABLE enrollments (
    student_id INT REFERENCES students(id),
    course_id  INT REFERENCES courses(id),
    grade      CHAR(2),
    PRIMARY KEY (student_id, course_id)
);
```

### Common data types
| Type | Use for | Notes |
| --- | --- | --- |
| `INT` / `BIGINT` | whole numbers | |
| `NUMERIC(p,s)` / `DECIMAL` | exact decimals (money, GPA) | avoid `FLOAT` for money |
| `VARCHAR(n)` | short text | |
| `TEXT` | long text | |
| `BOOLEAN` | true/false | MySQL stores it as `TINYINT(1)` |
| `DATE`, `TIMESTAMP` | dates/times | |
| `SERIAL` (PG) / `AUTO_INCREMENT` (MySQL) | auto IDs | PG also has `GENERATED ALWAYS AS IDENTITY` |

### Insert
```sql
INSERT INTO students (name, major, gpa)
VALUES ('Alice', 'CompE', 3.80),
       ('Bob',   'EE',    3.20),
       ('Chai',  'CS',    NULL);
```

### Select (reading data)
```sql
SELECT * FROM students;                          -- everything
SELECT name, gpa FROM students;                  -- specific columns
SELECT name AS student_name FROM students;       -- rename a column
SELECT DISTINCT major FROM students;             -- unique values
```

### Filter with WHERE
```sql
SELECT * FROM students WHERE gpa >= 3.5;
SELECT * FROM students WHERE major = 'CompE' AND gpa > 3.0;
SELECT * FROM students WHERE major IN ('CS', 'EE');
SELECT * FROM students WHERE gpa BETWEEN 3.0 AND 3.5;   -- inclusive both ends
SELECT * FROM students WHERE name LIKE 'A%';            -- starts with A
SELECT * FROM students WHERE gpa IS NULL;               -- NOT  "= NULL"
```
`LIKE` wildcards: `%` = any number of chars, `_` = exactly one char.

### Sort and limit
```sql
SELECT * FROM students ORDER BY gpa DESC;             -- highest first
SELECT * FROM students ORDER BY major, name;          -- sort by two columns
SELECT * FROM students ORDER BY gpa DESC LIMIT 3;     -- top 3
SELECT * FROM students LIMIT 10 OFFSET 20;            -- paging: rows 21–30
```

### Update and delete
```sql
UPDATE students SET gpa = 3.90 WHERE name = 'Alice';
DELETE FROM students WHERE id = 3;
```

### Aggregate functions
```sql
SELECT COUNT(*)  FROM students;          -- all rows
SELECT COUNT(gpa) FROM students;         -- rows where gpa is not NULL
SELECT AVG(gpa), MIN(gpa), MAX(gpa) FROM students;
SELECT SUM(credits) FROM courses;
```

### GROUP BY and HAVING
```sql
-- average GPA per major
SELECT major, AVG(gpa) AS avg_gpa
FROM students
GROUP BY major;

-- only majors with more than 5 students
SELECT major, COUNT(*) AS n
FROM students
GROUP BY major
HAVING COUNT(*) > 5;
```
- `WHERE` filters **rows** before grouping
- `HAVING` filters **groups** after grouping

### Joins
```sql
-- INNER JOIN: only rows that match on both sides
SELECT s.name, c.code, e.grade
FROM enrollments e
JOIN students s ON s.id = e.student_id
JOIN courses  c ON c.id = e.course_id;

-- LEFT JOIN: every student, even ones with no enrollments (course cols = NULL)
SELECT s.name, e.course_id
FROM students s
LEFT JOIN enrollments e ON e.student_id = s.id;

-- find students not enrolled in anything
SELECT s.name
FROM students s
LEFT JOIN enrollments e ON e.student_id = s.id
WHERE e.student_id IS NULL;
```
| Join | Returns |
| --- | --- |
| `INNER JOIN` | only matching rows |
| `LEFT JOIN` | all left rows + matches (else NULL) |
| `RIGHT JOIN` | all right rows + matches (else NULL) |
| `FULL OUTER JOIN` | all rows from both (not in MySQL) |

### Change or remove tables
```sql
ALTER TABLE students ADD COLUMN email VARCHAR(255);
ALTER TABLE students DROP COLUMN email;
ALTER TABLE students RENAME TO learners;      -- MySQL: RENAME TABLE students TO learners;
DROP TABLE enrollments;                       -- deletes table AND data
TRUNCATE TABLE students;                      -- keeps table, deletes all rows
```

### Order a query is actually run in
Written order ≠ execution order. This explains most "why can't I use that alias here" errors.
```
FROM / JOIN  →  WHERE  →  GROUP BY  →  HAVING  →  SELECT  →  DISTINCT  →  ORDER BY  →  LIMIT
```

## Gotchas
- `UPDATE` or `DELETE` **without `WHERE`** hits every row. Run the `WHERE` as a `SELECT` first to see what you'll change.
- `NULL` is not a value, it's "unknown": `gpa = NULL` is never true, use `IS NULL` / `IS NOT NULL`.
- `COUNT(*)` counts rows; `COUNT(col)` skips NULLs in that column.
- Strings use **single quotes** `'text'`. In PostgreSQL, double quotes `"name"` mean an identifier (column/table name). MySQL uses backticks `` `name` `` for identifiers.
- PostgreSQL folds unquoted names to lowercase: `CREATE TABLE Students` makes `students`.
- You can't use a `SELECT` alias in `WHERE` (it runs before `SELECT`), but you can in `ORDER BY`.
- Every non-aggregated column in `SELECT` must be in `GROUP BY` (PostgreSQL enforces this; MySQL may silently pick a value depending on settings).
- `LIKE` is case-sensitive in PostgreSQL (use `ILIKE`), usually case-insensitive in MySQL.
- Statements end with `;` — the shell waits for more input until it sees one.

## MySQL vs PostgreSQL quick map
| Thing | PostgreSQL | MySQL |
| --- | --- | --- |
| Auto ID | `SERIAL` / `IDENTITY` | `AUTO_INCREMENT` |
| Identifier quotes | `"col"` | `` `col` `` |
| Case-insensitive match | `ILIKE` | `LIKE` (default collation) |
| Concatenate | `'a' \|\| 'b'` | `CONCAT('a', 'b')` |
| Return inserted row | `INSERT ... RETURNING id` | `LAST_INSERT_ID()` |
| Full outer join | yes | no (UNION two LEFT JOINs) |

## Practice checklist
- [ ] Create the `school` database and the three tables above
- [ ] Insert ~5 students, ~3 courses, and some enrollments
- [ ] List students in a given major sorted by GPA
- [ ] Count students per major
- [ ] Show each student with their course codes (JOIN)
- [ ] Find students with no enrollments (LEFT JOIN + IS NULL)
- [ ] Find the course with the most students

## Links
- PostgreSQL tutorial (official): https://www.postgresql.org/docs/current/tutorial.html
- MySQL tutorial (official): https://dev.mysql.com/doc/refman/8.0/en/tutorial.html
- SQLBolt (interactive basics): https://sqlbolt.com
- pgexercises (practice queries): https://pgexercises.com
- Select Star SQL (free interactive book): https://selectstarsql.com
