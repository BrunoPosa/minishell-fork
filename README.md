# minishell

## Description:
This project is meant to replicate some basic core parts of bash shell.

## Usage:
1. Initialize the program with ```./minishell```
2. Utilize the shell as you would use a proper shell

## Result:
- Status: <span style="color:red">Incomplete</span>
- Result: <span style="color:red">0%</span>

## Contact: 
Felipe
- 42-email: fdessoy-@student.hive.fi
- 42-profile: [https://profile.intra.42.fr/users/fdessoy-](https://profile.intra.42.fr/users/fdessoy-)

Waleed
- 42-email: walnaimi@student.hive.fi
- 42-profile: https://profile.intra.42.fr/users/walnaimi
- discord: torutu.

Bruno
- 42-email: bposa@student.hive.fi
- 42-profile: https://profile.intra.42.fr/users/bposa

Leo
- 42-email: lstorey@student.hive.fi
- 42-profile: https://profile.intra.42.fr/users/lstorey

## Instructions for branching
ALWAYS remember to pull from main to have the latest version of minishell before continuing your work. At the same time, always remember to only push to main directory when the program is properly working. AVOID PUSHING FAULTY PROGRAM AT ALL COSTS!

```
1. git clone repo
2. git checkout -b 'BranchName'
3. git status
4. git add (files)
5. git commit -m "comment"
6. git pull origin main
7. git checkout main
8. git pull
9. git merge BranchName
10. git push
11. git checkout BranchName
```

## DA PLAN:
1. Tokenization (Waleed)
    - convert input command line into a linked list
        - built-ins.
            - echo (with -n)
            - cd
            - pwd
            - export
            - unset
            - env
            - exit
        - commands.
        - arguments.
        - pipe.
        - Redirection Operators.
            - "<"
            - ">"
            - "<<"
            - ">>"
        - single and double quotes.
1. Parse the line given to readline() (Bruno and Leo?)
	- Turn everything into lowercase
 	- No white spaces
  	- No special characters
3. Tokenization (Waleed)
4. Built-ins (Felipe)
	- echo (with -n) DONE
 	- cd DONE
	- pwd DONE
 	- export 1/2 DONE
  	- unset
   	- env DONE
   	- exit DONE
6. Piping (Felipe)
7. Handle $? (Felipe)
8. CTRL-C + CTRL-D + CTRL-\ (Bruno or Leo)
    - CTRL + C working correctly
    - CTRL + \ is ignored on the top level of minishell, needs to work in the child processes
9. Environment variables ($ expands values) (Bruno, and everyone at certain point)


## Acronyms

1. env_ll - environment linked list (any time you see ll, it means linked list);
2. len - length;
3. i - index;
4. str - string;
5. pwd - print working directory;
6. cwd - current working directory;
