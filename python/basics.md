# Python

## What it is
Python is a general-purpose programming language that reads almost like
English. You write a `.py` file and the **interpreter** runs it line by line.
There's no compile step like C. Things are slower to run but much faster to
write, and that's usually the better trade.

## What people use it for
| Area | What it looks like | Common libraries |
|---|---|---|
| Scripting / automation | rename files, scrape a page, glue tools together | `pathlib`, `os`, `subprocess`, `requests` |
| Data analysis | load a CSV, clean it, chart it | `pandas`, `numpy`, `matplotlib` |
| Machine learning / AI | train models, call LLM APIs | `scikit-learn`, `pytorch`, `transformers` |
| Web backends | APIs and websites | `fastapi`, `flask`, `django` |
| Testing / tooling | test suites, CLIs, build scripts | `pytest`, `argparse`, `click` |
| Hardware / embedded | talk to serial ports and boards, Raspberry Pi | `pyserial`, `gpiozero`, MicroPython |
| Science / engineering | simulations, signal processing | `scipy`, `sympy` |

Where it's a poor fit: tight real-time or low-level code (use C/C++/Rust),
mobile apps, and code that runs in the browser (that's JavaScript).

## Running Python
```bash
python3 --version          # check it's installed
python3                    # interactive prompt (REPL), exit() to quit
python3 hello.py           # run a file
python3 -m pip install x   # install a package
```

## Virtual environments
Each project gets its own folder of packages so projects don't fight over
versions.
```bash
python3 -m venv .venv          # create one inside the project
source .venv/bin/activate      # turn it on (prompt shows (.venv))
pip install requests           # installs only into this project
pip freeze > requirements.txt  # save the package list
pip install -r requirements.txt  # rebuild it somewhere else
deactivate                     # turn it off
```

## The language in one screen
```python
# Variables: no type declarations, the value decides the type
name = "Pakorn"        # str
age = 25               # int
height = 1.75          # float
is_student = False     # bool
nothing = None         # "no value"

# f-strings: put variables inside text
print(f"{name} is {age}")

# Collections
nums = [3, 1, 2]                   # list: ordered, changeable
point = (4, 5)                     # tuple: ordered, can't change
ages = {"alice": 30, "bob": 25}    # dict: key -> value
tags = {"py", "notes"}             # set: unique items, no order

nums.append(4)
ages["carol"] = 41

# if / elif / else: indentation is the block, no braces
if age >= 18:
    print("adult")
elif age >= 13:
    print("teen")
else:
    print("kid")

# Loops
for n in nums:
    print(n)

for i in range(3):          # 0, 1, 2
    print(i)

for key, value in ages.items():
    print(key, value)

while age < 30:
    age += 1

# Functions
def greet(who, greeting="Hi"):
    return f"{greeting}, {who}!"

greet("Bob")                # "Hi, Bob!"
greet("Bob", "Hello")       # "Hello, Bob!"

# List comprehension: build a list in one line
squares = [n * n for n in range(5)]        # [0, 1, 4, 9, 16]
evens = [n for n in nums if n % 2 == 0]

# Errors
try:
    x = int("abc")
except ValueError:
    print("not a number")

# Reading a file (closes it automatically)
with open("notes.txt") as f:
    for line in f:
        print(line.strip())

# Classes
class Dog:
    def __init__(self, name):
        self.name = name

    def bark(self):
        return f"{self.name} says woof"

Dog("Max").bark()
```

## Modules and imports
```python
import math                     # standard library: comes with Python
from pathlib import Path        # import one thing
import numpy as np              # third-party: pip install numpy first
```
Any `.py` file is a module. `import utils` loads `utils.py` from the same
folder.

```python
# Only runs when the file is run directly, not when it's imported
if __name__ == "__main__":
    main()
```

## Python vs C (from what I already know)
| | C | Python |
|---|---|---|
| Run | compile, then run | interpreter runs the source |
| Types | declared, checked at compile time | decided at runtime (optional type hints) |
| Blocks | `{ }` | indentation |
| Memory | `malloc` / `free` | automatic (garbage collected) |
| Speed | fast | slower, but libraries like `numpy` run C underneath |
| Strings | `char` arrays | built-in `str` with lots of methods |

## Gotchas
- **Indentation is syntax.** Mixing tabs and spaces breaks things. Use 4 spaces.
- `=` assigns, `==` compares.
- Lists are copied by reference: `b = a` means both names point to the same
  list. Use `b = a.copy()` for a real copy.
- Don't use a list as a default argument: `def f(x=[])` shares one list across
  every call. Use `def f(x=None)` instead.
- `/` always gives a float (`7 / 2 == 3.5`). Use `//` for integer division.
- Indexes start at 0, and `nums[-1]` is the last item.
- Activate the venv before `pip install`, or the package lands in the wrong place.
- On Linux/macOS, `python` may be Python 2 or missing; use `python3`.

## Learning path
1. Basics: variables, types, `if`, loops, functions
2. Collections: lists, dicts, sets, tuples, comprehensions
3. Files, errors, modules, venv + pip
4. Classes and objects
5. Standard library tour: `pathlib`, `json`, `csv`, `datetime`, `argparse`
6. Pick a direction: scripting, data (`pandas`), web (`fastapi`), or hardware
7. Testing with `pytest`, type hints

## Links
- [Official tutorial](https://docs.python.org/3/tutorial/)
- [Standard library reference](https://docs.python.org/3/library/)
- [Real Python](https://realpython.com/)
- [Python Package Index (PyPI)](https://pypi.org/)
