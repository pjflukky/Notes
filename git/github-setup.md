# GitHub Setup (new repo + SSH auth)

## What it is
How to start a repo locally, put it on GitHub, and log in **once** so `git push`
stops asking for a password. Use SSH: set it up once per machine and forget it.

## Start a new repo
```bash
mkdir my-project && cd my-project
git init
git add .
git commit -m "Initial commit"
```
Then make an **empty** repo on github.com (no README, so the histories don't clash) and connect it:
```bash
git branch -M main
git remote add origin git@github.com:<username>/<repo>.git   # SSH URL, not https://
git push -u origin main
```
Or skip the website: `gh repo create my-project --private --source=. --push`

## SSH key (one time per machine)
```bash
ssh-keygen -t ed25519 -C "your_email@example.com"   # Enter for default path; passphrase optional
cat ~/.ssh/id_ed25519.pub                            # copy this (the .pub one!)
```
GitHub → Settings → SSH and GPG keys → New SSH key → paste.

Test it:
```bash
ssh -T git@github.com
# "Hi <username>! You've successfully authenticated..." = done
```

Repo was cloned with https? Switch it:
```bash
git remote -v
git remote set-url origin git@github.com:<username>/<repo>.git
```

## Work on someone else's repo (e.g. xv6)
Clone, keep the original as `upstream`, push to my own copy as `origin`:
```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
cd xv6-riscv
git remote rename origin upstream
git remote add origin git@github.com:<username>/xv6-riscv.git   # empty repo made on GitHub first
git push -u origin riscv
git pull upstream riscv          # later: get their updates
```

## HTTPS vs SSH
| | HTTPS | SSH |
|---|---|---|
| Login | personal access token (not account password) | key pair |
| Asks every push? | only if no credential helper / `gh auth login` | never |
| Expires | token can expire | no, until I delete the key |
| Blocked networks | 443 works almost everywhere | port 22 sometimes blocked |

## Gotchas
- GitHub **doesn't accept the account password** for `git push` over HTTPS. The
  "password" prompt wants a personal access token.
- HTTPS keeps asking only when nothing saves the login. Fix: `gh auth login`, or
  `git config --global credential.helper store` (plain text in `~/.git-credentials`).
- Paste the **`.pub`** file into GitHub. Never share `id_ed25519` (the private key).
- New machine / VM = new key (or copy `~/.ssh/id_ed25519*` over and `chmod 600` the private key).
- Port 22 blocked (school / office wifi)? Use SSH over 443, in `~/.ssh/config`:
  ```text
  Host github.com
      Hostname ssh.github.com
      Port 443
      User git
  ```
- `Permission denied (publickey)` → key not added to GitHub, or wrong key loaded
  (`ssh -vT git@github.com` shows which key it tried).

## Links
- [GitHub: Connecting with SSH](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)
- [GitHub: SSH over the HTTPS port](https://docs.github.com/en/authentication/troubleshooting-ssh/using-ssh-over-the-https-port)
- [GitHub CLI: gh auth login](https://cli.github.com/manual/gh_auth_login)
