Här skriver vi anteckningar osv, vad som behöver göras etc.

Riktlinjer för programkoden:
	Inga rader får passera kolumnbredd 80.
	Indentering sker med 4 mellanslag.
	Namngivning:
		funktionsNamn
		variabel_namn
		typNamnT
		KONSTANT_NAMN

pseudokod:

	while true
		update user inputs
			här läser vi in knapptryck osv
		handle inputs
			här hanterar vi knapptryck. tex esc för att avsluta spelet osv
		update game objects
			här uppdaterar vi alla spelobjekt.. flyttar rymdskeppet framåt
			(beroende på user inputs) osv..flyttar raketer osv framåt
		render everything
			här ritar vi upp allting på skärmen
	loop
