# Python Built-in Functions

## What it is
Functions that are always there, with no `import`. Some of them (`int`,
`str`, `list`, `dict`, `set`, ...) are really **types**: calling them builds a
new value of that type. Full list: run `python3 -c "import builtins; print(dir(builtins))"`.

## Quick reference
| Group | Functions |
|---|---|
| Input / output | `print`, `input`, `open` |
| Inspecting things | `type`, `isinstance`, `len`, `id`, `dir`, `help`, `repr`, `callable` |
| Converting types | `int`, `float`, `str`, `bool`, `list`, `tuple`, `dict`, `set`, `frozenset` |
| Numbers | `abs`, `round`, `min`, `max`, `sum`, `pow`, `divmod` |
| Looping helpers | `range`, `enumerate`, `zip`, `reversed`, `sorted` |
| Functional | `map`, `filter`, `any`, `all` |
| Iterators | `iter`, `next` |
| Characters / bases | `ord`, `chr`, `bin`, `hex`, `oct` |
| Objects | `hasattr`, `getattr`, `setattr`, `hash` |

## Input and output
```python
print("a", "b", "c")               # a b c
print("a", "b", sep="-")           # a-b
print("no newline", end="")        # stays on the same line

name = input("Name: ")             # always returns a str
age = int(input("Age: "))          # convert it yourself

with open("notes.txt", "w") as f:  # "r" read, "w" write, "a" append
    f.write("hello\n")
```

## Inspecting things
```python
type(42)                  # <class 'int'>
isinstance(42, int)       # True
isinstance(3.0, (int, float))   # True: tuple means "any of these"

len("hello")              # 5
len([1, 2, 3])            # 3
len({"a": 1, "b": 2})     # 2

repr("hi")                # "'hi'"   (shows quotes, good for debugging)
callable(print)           # True
dir(list)                 # every method a list has
help(sorted)              # docs in the REPL, q to quit
```

## Converting types
```python
int("42")           # 42
int(3.9)            # 3       (cuts off, doesn't round)
int("ff", 16)       # 255     (second arg is the base)
float("3.5")        # 3.5
str(42)             # "42"
bool(0)             # False
bool("")            # False   (empty things are False)
bool([0])           # True    (non-empty list, even if it holds 0)

list("abc")         # ['a', 'b', 'c']
list(range(4))      # [0, 1, 2, 3]
tuple([1, 2])       # (1, 2)
dict(a=1, b=2)      # {'a': 1, 'b': 2}
dict([("a", 1), ("b", 2)])   # {'a': 1, 'b': 2}
```

## set
A set holds **unique** items with **no order**. Lookups (`x in s`) are fast,
like a hash table.
```python
s = set([1, 2, 2, 3, 3, 3])   # {1, 2, 3}  duplicates dropped
set("hello")                  # {'h', 'e', 'l', 'o'}  (order may differ)
empty = set()                 # NOT {}  -- {} is an empty dict

s.add(4)                      # {1, 2, 3, 4}
s.remove(4)                   # KeyError if missing
s.discard(99)                 # same, but no error if missing
3 in s                        # True

a = {1, 2, 3}
b = {2, 3, 4}
a | b      # {1, 2, 3, 4}   union: in either
a & b      # {2, 3}         intersection: in both
a - b      # {1}            difference: in a, not in b
a ^ b      # {1, 4}         symmetric difference: in exactly one

# Common use: remove duplicates from a list
nums = [3, 1, 3, 2, 1]
unique = list(set(nums))      # [1, 2, 3]  (order not guaranteed)
sorted(set(nums))             # [1, 2, 3]  (sorted list)

# frozenset: a set you can't change, so it can be a dict key
key = frozenset({1, 2})
```

## dict
A dict maps **keys to values**, like a hash table (`HashMap` in Java,
`std::unordered_map` in C++). Lookups by key are fast. Since Python 3.7 it
remembers insertion order.

Not the same as `map()`: that's a function that applies another function to
every item (see Functional tools below).
```python
d = {}                        # empty dict (same as dict())
d = {"a": 1, "b": 2}

# Reading
d["a"]              # 1
d["z"]              # KeyError
d.get("z")          # None  (no error)
d.get("z", 0)       # 0     (default if missing)
"a" in d            # True  (checks keys, O(1))
len(d)              # 2

# Writing
d["c"] = 3          # add or overwrite
d.update({"x": 9, "a": 10})        # merge another dict in
d.setdefault("lst", []).append(5)  # insert [] only if missing, then use it

# Removing
d.pop("x")          # removes "x", returns 9
d.pop("zzz", None)  # no KeyError when you give a default
del d["c"]          # remove without returning
d.clear()           # empty it

# Looping
d = {"a": 1, "b": 2}
for k in d:                 # keys
    print(k)
for k, v in d.items():      # key and value
    print(k, v)
list(d.keys())              # ['a', 'b']
list(d.values())            # [1, 2]

# Counting pattern (very common on LeetCode)
count = {}
for ch in "hello":
    count[ch] = count.get(ch, 0) + 1   # {'h': 1, 'e': 1, 'l': 2, 'o': 1}

# Dict comprehension
squares = {n: n * n for n in range(4)} # {0: 0, 1: 1, 2: 4, 3: 9}
```
`collections` has dict variants that make counting and grouping shorter
(`Counter`, `defaultdict`): see [collections.md](collections.md).

## Numbers
```python
abs(-7)             # 7
round(3.14159, 2)   # 3.14
round(2.5)          # 2   (rounds to even! round(3.5) is 4)
pow(2, 10)          # 1024
pow(2, 10, 1000)    # 24  (2**10 % 1000, fast)
divmod(17, 5)       # (3, 2)  quotient and remainder

nums = [4, 1, 7, 3]
min(nums)           # 1
max(nums)           # 7
sum(nums)           # 15
max("apple", "fig", "banana", key=len)   # 'banana'
min([], default=0)  # 0  (otherwise ValueError on empty)
```

## Looping helpers
```python
range(5)            # 0 1 2 3 4
range(2, 6)         # 2 3 4 5
range(10, 0, -3)    # 10 7 4 1

fruits = ["apple", "banana", "cherry"]

for i, fruit in enumerate(fruits):          # index + item
    print(i, fruit)                         # 0 apple / 1 banana / 2 cherry

for i, fruit in enumerate(fruits, start=1): # count from 1
    print(i, fruit)

prices = [1.0, 0.5, 2.0]
for fruit, price in zip(fruits, prices):    # walk two lists together
    print(fruit, price)
dict(zip(fruits, prices))   # {'apple': 1.0, 'banana': 0.5, 'cherry': 2.0}

list(reversed([1, 2, 3]))   # [3, 2, 1]

sorted([3, 1, 2])                    # [1, 2, 3]  (new list)
sorted([3, 1, 2], reverse=True)      # [3, 2, 1]
sorted(fruits, key=len)              # ['apple', 'banana', 'cherry']
sorted(["b", "A", "c"], key=str.lower)   # ['A', 'b', 'c']
```

## Functional tools
```python
nums = [1, 2, 3, 4]

list(map(lambda n: n * 2, nums))         # [2, 4, 6, 8]
list(map(int, ["1", "2", "3"]))          # [1, 2, 3]
list(filter(lambda n: n % 2 == 0, nums)) # [2, 4]

any(n > 3 for n in nums)    # True: at least one
all(n > 0 for n in nums)    # True: every one
any([])                     # False
all([])                     # True  (nothing failed)
```
A list comprehension usually reads better than `map`/`filter`:
`[n * 2 for n in nums]`.

## Iterators
```python
it = iter([10, 20, 30])
next(it)            # 10
next(it)            # 20
next(it)            # 30
next(it, "done")    # "done"  (default instead of StopIteration)
```
This is what a `for` loop does under the hood.

## Characters and number bases
```python
ord("A")     # 65
chr(65)      # 'A'
chr(ord("a") + 1)   # 'b'

bin(10)      # '0b1010'
hex(255)     # '0xff'
oct(8)       # '0o10'
int("1010", 2)      # 10  (back from binary)
```

## Objects
```python
class Dog:
    def __init__(self, name):
        self.name = name

d = Dog("Max")
hasattr(d, "name")          # True
getattr(d, "name")          # 'Max'
getattr(d, "age", 0)        # 0  (default if missing)
setattr(d, "age", 3)        # same as d.age = 3

hash("abc")                 # some int; only immutable things are hashable
id(d)                       # identity of the object (memory address in CPython)
```

## Gotchas
- `{}` is an empty **dict**. An empty set is `set()`.
- `d[key]` raises `KeyError` on a missing key; use `d.get(key, default)` when
  missing is normal.
- Don't add or remove dict keys while looping over the dict: loop over
  `list(d)` instead.
- Sets have no order and no indexing: `s[0]` is an error.
- Set items and dict keys must be hashable: `{[1, 2]}` fails, `{(1, 2)}` works.
- `sorted(x)` returns a new list; `x.sort()` sorts in place and returns `None`.
- `map`, `filter`, `zip`, `reversed`, `range` are lazy: wrap in `list()` to see
  the values, and they can only be looped over once (except `range`).
- `round(2.5)` is `2`: Python rounds halves to the nearest even number.
- `int(3.9)` is `3`: it truncates, it doesn't round.
- `input()` always returns a string.
- Don't name variables `list`, `str`, `sum`, `id`, `max`: you hide the built-in.

## Links
- [Built-in functions (docs)](https://docs.python.org/3/library/functions.html)
- [Set types (docs)](https://docs.python.org/3/library/stdtypes.html#set-types-set-frozenset)
