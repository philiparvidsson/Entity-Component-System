
RIKTLINJER.

  1. Inga rader f�r passera kolumn 80. Dvs, max 80 tecken per rad.

  2. Indentering sker med fyra mellanslag. Inga tabs. Konfigurera detta under
     inst�llningar.

  3. Namngivning:
       KONSTANT_NAMN
       typNamnT;
       variabel_namn
       funktionsNamn();

------------------------------------------------------------

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
