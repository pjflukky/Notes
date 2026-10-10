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
