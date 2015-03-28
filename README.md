# Floob-Boober-Bab-Boober-Bubs
Demo for D using Allegro

For windows, maybe these will help and you won't need to use command
line git.

- https://windows.github.com
- http://stackoverflow.com/questions/507343/using-git-with-visual-studio

If you have commandline shell, you can work with the git repository as
follows.  First, get the repository from github by cloning:

```
$ cd some-dir-where-you-want-files-to-go
$ git clone git@github.com:Sharbal/Floob-Boober-Bab-Boober-Bubs.git
```

If you don't have SSH keys setup for github, then you can instead use
https:

```
$ cd some-dir-where-you-want-files-to-go
$ git clone https://github.com/Sharbal/Floob-Boober-Bab-Boober-Bubs.git
```

Now you should have a `some-dir-where-you-want-files-to-go` with
a directory named `Floob-Boober-Bab-Boober-Bubs` inside it.  Save all
you files to `Floob-Boober-Bab-Boober-Bubs`.  For example, create a
new Visual Studio project and save it under
`Floob-Boober-Bab-Boober-Bubs`.

Now let's say you want to commit you changes with git so they are
saved in history.  This will only be on you own local computer.

First see what changes you have made with git status:
```
$ git status
On branch master
Your branch is up-to-date with 'origin/master'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   README.md

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	.gitignore

no changes added to commit (use "git add" and/or "git commit -a")
```

I made a new file named `.gitignore` that git doesn't know about, so
it is called an untracked file.  I want git to know about it so I
type:

```
$ git add .gitignore
```

If that is the only change I want to get into git right now, I can
commit it.  Try `git status` first and notice that git now says that
`.gitignore` is a change to be commited.

```
$ git status
On branch master
Your branch is up-to-date with 'origin/master'.

Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   .gitignore

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   README.md
```

Now commit it:

```
$ git commit -m "Add a .gitignore for VS and Xcode"
[master 297fb55] Add a .gitignore for VS and Xcode
 1 file changed, 233 insertions(+)
 create mode 100644 .gitignore
 ```

I also finish the README changes, so I commit that:

```
$ git add README.md
$ git commit -m "Add git examples to README.md"
```

