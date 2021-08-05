# Usare il comando set per disabilitare la memorizzazione di history.
# Poi lanciare un comando qualsiasi 
# e poi lanciare history e verificare che quel comando non è stato memorizzato. 
# Infine, usare il comando set per riabilitare la memorizzazione di history.

set +o history !1036 ; set -o history !1036 ;