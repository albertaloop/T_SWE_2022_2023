# T_SWE_2019_2020
Software for AlbertaLoop's first-generation pod.

[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

A rendering of the pod's aeroshell.

![podrender](docs/podrender.png)

An initial mock-up of the electronics layout.

![podelectronics](docs/podelectronics.png)

# Setup
We use pre-commit hooks to ensure code standardisation. These hooks will automatically make changes to staged code, based on the rules configured in _pre-commit-config.yaml_.

You will have to install [pre-commit](https://pre-commit.com/) to make changes to the repository. To install pre-commit, follow the instructions on the website. The install would look something like this on Linux:
0. open a terminal and change to the repository directory `cd T_SWE_2019_2020/`
1. create a virtual Python environment `virtualenv venv`
2. source the environment `source venv`
3. add pre-commit package `pip install pre-commit`
4. install git hooks script `pre-commit install`

And that's it! You will not even need to source your environment each time you program. Just start programming!

# Repository guidelines

# Github
The general guideline for Github is to always develop on your own branch. After a feature is ready, create a pull request for the branch you are working on and have a senior member of the team review and approve your pull request. Once a pull request has been approved, it can be merged into the master branch. If you are unsure of how to merge, please ask a senior member of the team for guidance.

## Github Branches
Use "-" character to separate words in a branch name. Example: rutvik-gui. Try to name each branch to highlight **who** is working on the feature, and **what** the feature is.

## Github Commit Messages
Commit messages must accurately and briefly explain what change you are making. Try to be concise.
