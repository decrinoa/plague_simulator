# plague-simulator
Potete scaricare tutti i file sul computer con il codice: <br>
<code>git clone https://github.com/progetto-manhattan/plague-simulator </code> <br>
Una volta scaricato, per aggiornare i file vi basta usare questi due comandi: <br>
<code>cd plague-simulator</code> <br>
<code>git pull</code> <br><br>

Se andate su Projects c'è tipo una to do list. Aggiungeteci quello che vi viene in mente. <br>
Issue pare invece un modo più veloce per scriverci piccole cosette da fare, cose che non funzionano eccetera. <br><br>

L'idea è di scrivere il codice qui, per poi scaricarlo sul computer, compilarlo e vedere cosa fa e come va. Ma la cosa che ho trovato più comoda è prendere i file con git clone/pull e modificarli con visual studio. Poi copiarli e incollarli qua su github (stando attenti che nel frattempo nessuno abbia modificato altri file (o files, come scriverebbe Cuffiani).<br><br>

Direi che ho detto tutto. A lavoro. Mi raccomando i commenti.<br>

<h2>Come far funzionare la grafica</h2>
La grafica è copiata dal The Game of Life che il prof ha caricato sulla sua repo pf2019.<br>
Tutte le istruzioni per farla funzionare le ho trovare qui https://www.sfml-dev.org/tutorials/2.5/start-linux.php , ma siccome sono una brava persona e non voglio farvi smadonnare come ho fatto io ve le riassumo.<br><br>

Per prima cosa bisogna scaricare il pacchetto di SFML:
<code>sudo apt-get install libsfml-dev</code> <br>
Poi anziché compilare normalmente con g++, dovrete compilare tutti i file che servono con l'opzione -c, che credo serva per compilare un file per volta: <br>
<code>g++ -c main.cpp board.cpp</code> <br>
Si saranno ora creati i file main.o, board.o e display.o .<br>
(NOTA: compilare con -c è utile anche perché se modifico un solo file, basta ricompilare quello (sempre con l'opzione -c) invece che tutti quanti e fa molto prima.) <br>
A questo punto bisognerà "mettere insieme i pezzi" con il comando:<br>
<code>g++ main.o board.o -lsfml-graphics -lsfml-window -lsfml-system</code> <br>
Si creerà un file a.out eseguibile con <code>./a.out</code> <br>
(OPZIONALE: con l'opzione -o si può scegliere il nome dell'output eseguibile: <br>
<code>g++ main.o board.o -o nomeoutput -lsfml-graphics -lsfml-window -lsfml-system</code> )
