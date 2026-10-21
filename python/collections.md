# Python collections Module

## What it is
Extra container types from the standard library. Not built-in, so you need an
import, but nothing to `pip install`:
```python
from collections import deque, Counter, defaultdict
```
(LeetCode's Python already imports these, but write the import anyway.)

| Type | What it is | Use it for |
|---|---|---|
| `deque` | double-ended queue | queues, stacks, sliding windows, BFS |
| `Counter` | dict that counts things | frequencies, anagrams, "most common" |
| `defaultdict` | dict with an automatic default value | grouping, graphs (adjacency lists) |

## deque
"Deck", short for **double-ended queue**: add and remove at both ends in O(1).

A **queue** is the idea (first in, first out). A deque can act as a queue,
a stack, or both, depending on which methods you use.
```python
q = deque()            # empty
q = deque([1, 2, 3])   # from a list

# Right end
q.append(4)            # [1, 2, 3, 4]
q.pop()                # 4  -> [1, 2, 3]

# Left end
q.appendleft(0)        # [0, 1, 2, 3]
q.popleft()            # 0  -> [1, 2, 3]

# Peek without removing
q[0]                   # 1  (front / left)
q[-1]                  # 3  (back / right)

len(q)                 # 3
if not q:              # empty check
    print("empty")

q.rotate(1)            # [3, 1, 2]  move items right by 1
q.extend([7, 8])       # add many to the right
d = deque(maxlen=3)    # oldest items fall off when full
```

### Using it as a queue (FIFO)
Only use `append` (add to back), `popleft` (remove from front), `q[0]`
(peek front), `len(q)`.
```python
q = deque()
q.append("a")
q.append("b")
q.popleft()      # "a"  -- first in, first out
```

### Using it as a stack (LIFO)
Only use `append` and `pop` (both on the right end). A plain `list` works
just as well for a stack.
```python
s = deque()
s.append("a")
s.append("b")
s.pop()          # "b"  -- last in, first out
```

### deque vs the alternatives
| Option | Notes |
|---|---|
| `collections.deque` | The normal choice. O(1) at both ends. |
| `list` with `pop(0)` | Works, but O(n): every item shifts left. Don't use as a queue. |
| `queue.Queue` | Thread-safe FIFO with locks; methods are `put()` / `get()`. For threads, overkill otherwise. |

## Counter
A dict whose values are counts.
```python
c = Counter("hello")          # Counter({'l': 2, 'h': 1, 'e': 1, 'o': 1})
c = Counter([1, 1, 2, 3, 3, 3])

c[3]                 # 3
c[99]                # 0  (no KeyError for missing keys)
c.most_common(2)     # [(3, 3), (1, 2)]
c[1] += 1            # update like a normal dict
c.update([2, 2])     # add more counts

Counter("listen") == Counter("silent")   # True: same letters, same counts
Counter("aab") - Counter("ab")           # Counter({'a': 1})
```

## defaultdict
A dict that creates a default value the first time you touch a missing key.
You pass the **type** (or any function) that builds the default.
```python
groups = defaultdict(list)     # missing key -> []
groups["fruit"].append("apple")  # no need to check if "fruit" exists
groups["fruit"].append("pear")
# defaultdict(<class 'list'>, {'fruit': ['apple', 'pear']})

count = defaultdict(int)       # missing key -> 0
for ch in "hello":
    count[ch] += 1

seen = defaultdict(set)        # missing key -> set()
seen["a"].add(1)

# Graph as an adjacency list
graph = defaultdict(list)
for u, v in [(1, 2), (1, 3), (2, 3)]:
    graph[u].append(v)
    graph[v].append(u)
```

## Gotchas
- `deque` is not a built-in: forgetting the import gives `NameError`.
- Indexing the middle of a deque (`q[5]`) is O(n). It's fast only at the ends.
- Slicing doesn't work on a deque: `q[1:3]` is an error.
- `queue.Queue` and `collections.deque` are different things; for LeetCode you
  want `deque`.
- With `defaultdict`, just *reading* a missing key (`d["x"]`) adds it. Use
  `"x" in d` to check without creating it.
- `defaultdict(list)` takes `list`, not `list()` or `[]`: it needs something
  it can call.
- `Counter` keeps keys with zero or negative counts after `c[k] -= 1`;
  they don't disappear on their own.

## Links
- [collections (docs)](https://docs.python.org/3/library/collections.html)
- [deque (docs)](https://docs.python.org/3/library/collections.html#collections.deque)
