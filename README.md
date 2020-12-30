# Texted - A CLI text editor for Linux
Per compilare texted è sufficiente decomprimere questa cartella compressa e compilare con il seguente comando:  
`$ tar -xvf Texted.tar.gz`  
`$ cd Texted`  
`$ gcc bin/* main.c -o texted`  

Per utilizzare il programma utilizzare la seguente sintassi  
`$ ./texted file_name`  
Ci si ritroverà in una schermata simile a questa:  
`file_name > `  
Qui è possibile inserire dei comandi per scrivere, stampare, modificare e salvare il file e per uscire dal programma. Per avere accesso alla lista di tutti i comandi digitare `h` e inviare.

Per selezionare una riga si utilizzi `l riga`, per modificare una parola nella riga selezionata la sintassi da usare è `s/da/a/`. La sintassi da usare per inserire una parola prima di un'altra è `m/prima_di/parola/`. Per aggiungere una parola al termine della riga scrivere `a/parola`. Per stampare l'intero contenuto del file con il numero di righe scrvere `pn`; togliere la `n` per non visualizzare il numero di righe.
Per aggiungere del contenuto infondo al file in insert mode scrivere `i`. Per uscire premere ESC e inviare. Per salvare scrivere `w`, per salvare e uscire digitare `x`. Per uscire senza salvare premere `q`.