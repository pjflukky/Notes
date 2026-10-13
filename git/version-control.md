# Version Control (Git)

## What it is
A version control system keeps a history of **snapshots** of a whole folder,
each with an author, a time and a message saying why. Git is the standard one.
Learn the data model first: every git command is just "add objects" or
"move a pointer", and the CLI makes sense once I know which one I want.

## Quick map
| I want to... | Command |
|---|---|
| start a repo | `git init` |
| see what changed | `git status`, `git diff` |
| save a snapshot | `git add <file>` then `git commit` |
| see history as a graph | `git log --all --graph --decorate --oneline` |
| work on something separately | `git switch -c <branch>` |
| combine branches | `git merge <branch>` / `git rebase <branch>` |
| sync with GitHub | `git fetch`, `git pull`, `git push` |
| fix the last commit | `git commit --amend` |
| who changed this line? | `git blame <file>` |
| which commit broke it? | `git bisect` |
| park unfinished work | `git stash` / `git stash pop` |

## Data model
Everything is one of three **objects**:
- **blob**: a file's contents (just bytes, no name)
- **tree**: a directory, maps names → blobs or other trees
- **commit**: points to a top-level tree (the snapshot) + parent commit(s),
  author, message

```text
<root> (tree)
├── foo (tree)
│   └── bar.txt (blob: "hello world")
└── baz.txt (blob: "git is wonderful")
```

- Every object is stored under the **SHA-1 hash of its content**. That hash is its ID.
  Objects point to each other by hash, so same content = same object (free dedup).
- History is a **DAG** of commits. Arrows point back to parents.
  A merge commit has 2+ parents.
- Objects are **immutable**. "Rewriting history" (amend, rebase) makes *new*
  commits and moves the pointers; the old ones are still there for a while.

**References** are the mutable part: human names that point at a commit hash.
- Branches (`main`, `feature-x`) are just refs. Committing moves the branch forward.
- **HEAD** = where I am now. A new commit's parent is whatever HEAD points to.
- **Detached HEAD**: HEAD points at a commit directly instead of a branch.
  Commits made here aren't on any branch, so make one (`git switch -c`) before leaving.

A repo = **objects + references**. That's all `.git/` really is.

Peek inside:
```bash
git cat-file -t <hash>     # type: blob / tree / commit
git cat-file -p <hash>     # pretty-print its contents
git cat-file -p HEAD       # the current commit
git rev-parse HEAD         # which hash HEAD points at
```

## Staging area
Git doesn't snapshot my working folder directly. I pick what goes into the
next commit by **staging** it (the "index").
- Split two unrelated changes into two commits.
- Commit the fix but leave the debug prints out.

```text
working dir ──git add──▶ staging area ──git commit──▶ repo (history)
```

## Commands

### Basics
```bash
git help <command>              # docs for any command
git init                        # new repo, data lives in .git/
git status                      # what's staged / unstaged / untracked
git add <file>                  # stage a file
git add -p                      # stage chunk by chunk (pick which hunks)
git commit                      # commit staged changes (opens editor)
git commit -m "msg"             # commit with inline message
git log                         # history, newest first
git log --all --graph --decorate --oneline   # history as a DAG
git log -- <path>               # only commits that touched this file
git show <commit>               # what one commit changed
```

### Diffs
```bash
git diff                        # unstaged changes (working dir vs staging)
git diff --staged               # staged changes (staging vs last commit)
git diff <rev> <file>           # file now vs an older snapshot
git diff <rev1> <rev2>          # two snapshots
```

### Branching and merging
```bash
git branch                      # list branches
git branch <name>               # create (doesn't switch)
git switch <name>               # switch to it
git switch -c <name>            # create + switch (old way: git checkout -b)
git merge <branch>              # merge <branch> into the current one
git mergetool                   # resolve conflicts in a tool
git rebase <branch>             # replay my commits on top of <branch>
git rebase -i <rev>             # edit / reorder / squash commits since <rev>
```

Merge conflict markers:
```text
<<<<<<< HEAD
1 cup salt
=======
2 cups sugar
>>>>>>> sweet
```
Edit the file to what it should be, delete the markers, then
`git add <file>` and `git merge --continue` (or `git commit`).

### Remotes
```bash
git clone <url>                 # download a repo
git clone --depth=1 <url>       # shallow: only the latest snapshot
git remote -v                   # list remotes
git remote add origin <url>     # add one
git fetch                       # download new objects + refs, don't touch my branch
git pull                        # = git fetch + git merge
git push origin main            # upload, move the remote's branch
git push -u origin <branch>     # push + set upstream so plain `git push` works
git branch --set-upstream-to=origin/<branch>   # link existing branch to remote
```

### Undo
```bash
git restore <file>              # throw away unstaged changes to a file
git restore --staged <file>     # unstage (keep the change)
git commit --amend              # redo the last commit (message or contents)
git reset --hard <rev>          # move branch to <rev>, DISCARD everything after
git revert <commit>             # new commit that undoes an old one (safe on shared branches)
git reflog                      # where HEAD has been, find "lost" commits here
```

### Advanced
```bash
git blame <file>                # last commit to touch each line
git stash                       # put uncommitted changes aside
git stash pop                   # bring them back
git bisect start                # binary search for the commit that broke something
git bisect bad                  #   current commit is broken
git bisect good <rev>           #   this old one worked; git checks out the middle
git bisect reset                #   done, go back
git worktree add ../dir <branch>  # second checkout of another branch, same repo
git config --global <key> <val> # customize (aliases, editor, name, email)
```

## Config and ignore files
```bash
git config --global user.name "Pakorn Jantacumma"
git config --global user.email "..."
git config --global alias.graph "log --all --graph --decorate --oneline"
git config --global core.excludesfile ~/.gitignore_global
```
- `.gitignore` in a repo: build output, `__pycache__/`, `.env`, etc.
- `~/.gitignore_global`: OS / editor junk for every repo (`.DS_Store`, `*.swp`).

## Gotchas
- **Think in graph operations.** Ask "which pointer do I want to move where?"
  Example: drop local work and put `main` back at a commit:
  `git switch main; git reset --hard 5d83f9e`.
- `reset --hard` deletes uncommitted work for real. Committed work can usually
  be found again with `git reflog`.
- Don't rebase / amend commits that are already pushed to a shared branch;
  everyone else's history stops matching. Use `git revert` there.
- Deleting a file in a new commit doesn't remove it from history. Secrets need a
  history rewrite (`git filter-repo`) **and** a key rotation.
- `git pull` makes merge commits by default; `git pull --rebase` keeps history linear.
- Git ≠ GitHub. Pull requests, forks and issues are GitHub features, not git.
- Commit messages: short summary line (~50 chars), blank line, then *why*.

## Exercises (Missing Semester)
- [ ] Read Pro Git ch. 1–5 or play Learn Git Branching, mapping each command to the data model
- [ ] Clone `missing-semester/missing-semester`:
  - [ ] view history as a graph
  - [ ] who last changed `README.md`? (`git log` with a path)
  - [ ] commit message for the last change to the `collections:` line in `_config.yml` (`git blame` + `git show`)
- [ ] Commit a "secret" file, then remove it from **all** of history
- [ ] Modify a file, `git stash`, look at `git log --all --oneline`, `git stash pop`
- [ ] Add a `git graph` alias
- [ ] Set up a global ignore file
- [ ] Fork the class repo and open a PR (only if there's something real to fix)
- [ ] Merge conflict drill: `recipe.txt`, branches `salty` / `sweet` edit the same line, merge both into main, resolve, view with `git log --graph --oneline`

## Links
- [Missing Semester: Version Control](https://missing.csail.mit.edu/2026/version-control/)
- [Pro Git](https://git-scm.com/book/en/v2) (ch. 1–5 cover daily use)
- [Learn Git Branching](https://learngitbranching.js.org/)
- [Oh Shit, Git!?!](https://ohshitgit.com/) (fixing common mistakes)
- [Git for Computer Scientists](https://eagain.net/articles/git-for-computer-scientists/)
- [GitHub: removing sensitive data](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/removing-sensitive-data-from-a-repository)
