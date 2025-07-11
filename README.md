# MiniGit 

A simple, educational version control system implemented in C++ — inspired by Git.  
MiniGit lets you track file versions, stage changes, commit snapshots, view history, and checkout older states of your repository.

---

## Features

- Initialize a repository
- Stage files with content
- Commit snapshots with custom messages
- View commit history in a tree structure
- Checkout previous commits
- Show files at the current `HEAD`

---

## Project Structure

- `Commit` class: Represents a snapshot in time, holds file states and parent-child links.
- `Repository` class: Manages the staging area, commit history, and file tracking.
- `main()`: CLI interface to interact with the repository.

---

Example flow:

```bash
> Add File → main.cpp
> Commit → "Initial commit"
> Add File → utils.cpp
> Commit → "Added utility file"
> Log History
> Checkout → commit_1
> Show Current Files


