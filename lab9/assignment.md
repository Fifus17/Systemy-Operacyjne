## W ramach zadania należy zaimplementować rozwiązanie problemu Świętego Mikołaja.

Święty Mikołaj śpi w swoim warsztacie na biegunie północnym i może być obudzony tylko w 2 sytuacjach:
<ol>
<li> wszystkie 9 reniferów wróciło z wakacji, </li>
<li> 3 z 10 elfów ma problemy przy produkcji zabawek. </li>
</ol>
Kiedy problemy 3 elfów są rozwiązywane przez Mikołaja to pozostałe nie zgłaszają swoich problemów aż do powrotu pierwszej trójki.


Jeśli Mikołaj się obudzi i zastanie jednocześnie 9 reniferów i 3 elfy przed swoim warsztatem to stwierdza że rozwiezienie prezentów może poczekać i ważniejsze jest rozwiązanie problemów elfów. 

Należy zaimplementować program, w którym Mikołaj, renifery oraz elfy to osobne wątki.

Zachowania elfów:
<ul>

<li>Pracują przez losowy okres czasu (2-5s).</li>
<li>Chcą zgłosić problem - jeśli liczba oczekujących elfów przed warsztatem Mikołaja jest mniejsza od 3 to idzie przed warsztat (Komunikat: Elf: czeka _ elfów na Mikołaja, ID), w przeciwnym wypadku sam rozwiązuje swój problem i wraca do pracy (Komunikat: Elf: samodzielnie rozwiązuję swój problem, ID),</li>
<li>Jeśli jest trzecim elfem przed warsztatem to wybudza Mikołaja. (Komunikat: Elf: wybudzam Mikołaja, ID)</li>
<li>Mikołaj się z nimi spotyka. (Komunikat: Elf: Mikołaj rozwiązuje problem, ID) (1-2s)</li>
<li>Wraca do pracy</li>
</ul>

Zachowania reniferów:

<ul>
<li>Są na wakacjach w ciepłych krajach losowy okres czasu (5-10s)</li>
<li>Wracaja na biegun północny (Komunikat: Renifer: czeka _ reniferów na Mikołaja, ID), jeśli jest dziewiątym reniferem to wybudza Mikołaja (Komunikat: Renifer: wybudzam Mikołaja, ID).</li>
<li>Dostarczają zabawki grzecznym dzieciom (i studentom którzy nie spóźniają się z dostarczaniem zestawów) przez (2-4s). </li>  
<li>Lecą na wakacje.</li>
</ul>

Zachowania Mikołaja:

<ul>
<li>Śpi.</li>
<li>Kiedy zostaje wybudzony (Komunikat: Mikołaj: budzę się):
<ol>
<li> i jest 9 reniferów to dostarcza zabawki (Komunikat: Mikołaj: dostarczam zabawki) (2-4s).</li>
<li> i są 3 elfy to bierze je do swojego warsztatu i rozwiązuje problemy (Komunikat: Mikołaj: rozwiązuje problemy elfów _ _ _ ID) (1-2).</li>
</ol></li>
<li>Wraca do snu (Komunikat: Mikołaj: zasypiam).</li>
</ul>

Program należy zaimplementować korzystając z wątków i mechanizmów synchronizacji biblioteki POSIX Threads. Po uruchomieniu programu wątek główny tworzy wątki dla Mikołaja, reniferów oraz elfów. Możemy założyć że Mikołaj dostarczy 3 razy prezenty, po czym kończy działanie wszystkich wątków. Do spania Mikołaja powinny być wykorzystane Warunki Sprawdzające (Condition Variables). Użycie odpowiednich mechanizmów ma zagwarantować niedopouszczenie, np. do zdarzeń:
<ul>
<li>Mikołaj śpi chociaż czeka na niego 9 reniferów lub 3 elfy.</li>
<li>Na Mikołaja czeka więcej niż 3 elfy.</li>
</ul>
Pełne rozwiązanie zadania - 100%. Wersja uproszczona - (Mikołaj i renifery) lub (Mikołaj i elfy) - 60%.