# Notes

My learning notes and small working examples, written in my own words.

## Topics

| Folder | What's in it |
|---|---|
| [shell/](shell/) | zsh and bash setup, shell scripting |
| [python/](python/) | Python language basics and examples |
| [neovim/](neovim/) | Neovim config, plugins, LSP and completion |
| [git/](git/) | Git commands and workflows |
| [SQL/](SQL/) | A SQL language basics and examples |

## Layout

```
notes/
├── README.md
├── <topic>/
│   ├── <subject>.md
│   └── examples/      # runnable code, only when a note needs it
```

- One folder per topic, one file per subject
- File names are lowercase with dashes: `bash-scripting.md`
- Split a file once it gets too long to scroll comfortably

## Note template

````markdown
# Title

## What it is
One or two sentences in my own words.

## Commands
```bash
example command
```

## Gotchas
- Things that tripped me up

## Links
- Useful references
````

## Searching

```bash
rg "keyword" ~/notes            # search every note
rg --files ~/notes | fzf        # pick a file by name
```

## Courses

| Course | Topic | Link |
| --- | --- | --- |
| MIT Missing Semester | Shell, tools & workflow | [missing.csail.mit.edu](https://missing.csail.mit.edu/) |
| MIT 6.1810 | Operating Systems | [Schedule](https://pdos.csail.mit.edu/6.1810/2026/schedule.html) |
| Kurose & Ross | Computer Networks (Wireshark labs) | [Labs](https://gaia.cs.umass.edu/kurose_ross/wireshark.php) |
| MIT 6.5840 | Distributed Systems | [Schedule](https://pdos.csail.mit.edu/6.824/schedule.html) |
| MIT 6.5660 | Computer Systems Security | [Course site](https://css.csail.mit.edu/6.5660/2026/) |
