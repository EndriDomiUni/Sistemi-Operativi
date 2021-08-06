# Visualizzare, mediante ls e metacaratteri, tutti i files della directory /usr/lib/ il cui nome contiene
# o un carattere numerico compreso tra 1 e 3
# oppure un carattere letterale compreso tra c ed m. e che termina con l’estensione .0

ls -d /usr/lib/*[1-3c-m]*.0

# no matches found: /usr/lib/*[1-3c-m]*.O