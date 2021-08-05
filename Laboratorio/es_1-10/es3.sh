# Scrivere un comando che crea una nuova variabile di nome PIPPO 
# la quale contiene la concatenazione dei contenuti delle variabili USER HOME DISPLAY

PIPPO=${USER}${HOME}${DISPLAY}
echo ${PIPPO} # visualizzo il contenuto della variabile
