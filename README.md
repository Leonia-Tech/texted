# INTRODUCTION
### What is TextEd? 
TextEd is a simple text editor written in C, it has the basic functions and it's inspired by linux ed editor   

![TextEd 1.4](https://i.ibb.co/KwFQMCT/New-Text-Ed.png)

### Install
To compile the program follow these instructions. Please don't forget to read the notes! 

 1. Git the repository with git clone command: 
    ```bash
    $ git clone https://github.com/Leonia-Tech/TextEd
    ```

2. Change directory and compile it
    ```bash
    $ make DEBUG=0
    ```

3. Install the program
    ```bash
    $ sudo make install
    ```

    The install target supports the `DESTDIR` argument.

3. Execute the program 
    ``` bash 
    $ texted [FILE] 
    ```
*Note: If you don't have mpv installed some functionalities may not work as intended. The programm should still be perfectly usable, though.*

*Note: You must have ncurses development library (ncurses-devel) in order to compile this software*

*Note: You must have readline development library (readline-devel) in order to compile this software*

### Remove
To remove the app keep the `Makefile` and run:
    ```bash
    $ sudo make remove
    ```

    The remove target supports the `DESTDIR` argument.

## BASIC FUNCTIONALITY

### In the editor 
After have executed the program with the desired file name you will be into the program 

```
Welcome in Texted - debug-version 1.4.2
your_file_name.txt 1> 
```

On the right you can see a number. It represents the curret selected line. You can change the current line with the `l` command.

Here you can write commands that you can use for printing, modifying, saving the file and exiting the program.

Here follows a list of commands  grouped by category: 

 
### Editing commands 
This is a list of available commands to edit


``` 
i:          insert mode
i: -w       saves  after exiting insert mode
esc:        exit insert mode 
```

The content added during the insert mode is appended at the end of the file without the addition of extra newlines or spaces!

*Note: to exit the program you have to click on the `esc` button on your keyboard*

### Print commands
List of commmnads releated to print functionality

```
p:		print
pl:		print specified line (default = current line)
pn:		print with line numbers 
pln:	pl + pn
```

*Note: `pl <num>` doesn't change the current selected line.

### Save commands
List of command releated to save functionality  
```
w:		save
x:		save and exit
b:		create backup file
q:		exit 
```

### Line editing commands
List of commands releated to line editing
```
s:		replace word (ed syntax)
m:		insert word before ... (ed syntax)
a:		insert at the end of line 
l:		set the current line

n:		add line before the current line 
d:		delete the current line
```

The `l` command must be followd by a valid line number. The space between the line number and the l command isn't necessary.

Use `h` or open texted with `texted --help` for further information.