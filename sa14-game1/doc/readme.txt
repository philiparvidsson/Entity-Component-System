
RIKTLINJER.

  1. Inga rader får passera kolumn 80. Dvs, max 80 tecken per rad.

  2. Indentering sker med fyra mellanslag. Inga tabs. Konfigurera detta under
     inställningar.

  3. Namngivning:
       KONSTANT_NAMN
       typNamnT;
       variabel_namn
       funktionsNamn();

------------------------------------------------------------

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
