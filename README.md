# T_SWE_2019_2020
All software work for this year's competition will be contained within T_SWE_2019_2020 repo. 

## Repository guidelines 

### GUI Directory 
We will be trying to match the convention provided in the Mastering GUI book as much as possible. Feel free to add to this list by making a pull request so other team members can review the change. 

1. File names will follow a lowercase underscores naming convention. For ex. "main_window.py"
2. To remain consistent with the Mastering GUI book, variable names will also follow lowercase underscore naming convention. For ex. health_panel = ...
3. To remain consistent with the Mastering GUI book, method names will follow a camel case naming convention. For ex. doSomething()...
4. Class names will follow a pascal case convention. ex. SomeClass 
5. Constants will use an all caps underscore naming convention. ex. SPEED_OF_LIGHT = 3E8

### Editing UI Code

If you intend to edit a UI using QTDesigner, download the .ui version of it from github, open it on QTDesigner, edit, and submit a pull request for both the .ui and .py versions. 

If you are just editing non-UI methods, do not alter the .ui file and class Ui_MainWindow in the .py version unless changes were made in those locations.

### Github 

The general guidline for Github is to always develop on your own branch. After a feature is ready, create a pull request for the branch you are working on and have a senior member of the team review and approve your pull request. Once a pull request has been approved, it can be merged into the master branch. If you are unsure of how to merge, please ask a senior member of the team for guidance. 

### Github Branches
Use "-" character to seperate words in a branch name. Example: rutvik-gui. Try to name each branch to highlight **who** is working on the feature, and **what** the feature is. 

#### Github Commit Messages 

Commit messages must accurately and briefly explain what change you are making. Try to be concise. 

