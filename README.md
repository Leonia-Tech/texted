# INTRODUCTION
### What is TextEd? 
TextEd is a simple text editor written in C, it has the basic functions and it's inspired by linux ed editor   

![TextEd 1.4](https://i.ibb.co/4t9r685/Text-Ed-1-4.png)

### Installation 
To compile the program follow these instructions: 

 1. Git the repository with git clone command: 

        $ git clone https://github.com/Leonia-Tech/TextEd 

2. Change directory and compile it
    ```bash
      $ make
     ```

3. Execute the program 

    ``` bash 
        $ ./texted <file_name> 
    ```
## BASIC FUNCTIONALITY

### In the editor 
After have executed the program with the desired file name you will be into the program 

 ` Welcome in Texted - debug-version 1.3 `
   
    your_file_name.txt > 
        
here you can write commands that you can use for printing, modifying, saving the file and exit the program    

Here follows a list of commands  grouped by category: 

 
### Editor commands 
This is a list of available commands in the editor 


``` 
i:          insert mode
i: -w       saves  after exiting insert mode
esc:        exit insert mode 
```
NOTE: to exit the program you have to click on the `esc` button on your keyboard 
### Print commands
List of commmnads releated to print functionality

```
p:		print
-l		current line
-n		with line numbers 
-ln		-l + -n
```

### Save commands
List of command releated to save functionality  
```
w:		save
x:		save and exit
b:		create backup file
q:		exit 
```

### Word modify commands
List of commands releated to word modyfing functionality
```
s:		replace word (ed syntax)
m:		insert word before ... (ed syntax)
a:		insert at the end of line 
l:		set the modify line 

n:		add line before modify line 
d:		delete modify line
```

