H�r skriver vi anteckningar osv, vad som beh�ver g�ras etc.

Riktlinjer f�r programkoden:
	Inga rader f�r passera kolumnbredd 80.
	Indentering sker med 4 mellanslag.
	Namngivning:
		funktionsNamn
		variabel_namn
		typNamnT
		KONSTANT_NAMN

pseudokod:

	while true
		update user inputs
			h�r l�ser vi in knapptryck osv
		handle inputs
			h�r hanterar vi knapptryck. tex esc f�r att avsluta spelet osv
		update game objects
			h�r uppdaterar vi alla spelobjekt.. flyttar rymdskeppet fram�t
			(beroende p� user inputs) osv..flyttar raketer osv fram�t
		render everything
			h�r ritar vi upp allting p� sk�rmen
	loop
