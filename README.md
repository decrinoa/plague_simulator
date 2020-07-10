# plague-simulator
E' possibile scaricare tutti i file sul computer con il codice: <br>
<code>git clone https://github.com/progetto-manhattan/plague-simulator </code> <br>
Una volta scaricato, per aggiornare i file basta usare questi due comandi: <br>
<code>cd plague-simulator</code> <br>
<code>git pull</code> <br><br>

<h2>Come far funzionare la grafica</h2>
Tutte le istruzioni per far funzionare SFML sono disponibili qui https://www.sfml-dev.org/tutorials/2.5/start-linux.php , ma in breve ciò che serve:

Per prima cosa bisogna scaricare il pacchetto di SFML:
<code>sudo apt-get install libsfml-dev</code> <br>
Poi occorre compilare tutti i file che servono con l'opzione -c:
<code>g++ -c main.cpp board.cpp</code> <br>
Si saranno ora creati i file main.o, board.o .<br>
A questo punto bisognerà "mettere insieme i pezzi" con il comando:<br>
<code>g++ main.o board.o -lsfml-graphics -lsfml-window -lsfml-system</code> <br>
Si creerà un file a.out eseguibile con <code>./a.out</code> <br>
(OPZIONALE: con l'opzione -o si può scegliere il nome dell'output eseguibile: <br>
<code>g++ main.o board.o -o nomeoutput -lsfml-graphics -lsfml-window -lsfml-system</code> )
