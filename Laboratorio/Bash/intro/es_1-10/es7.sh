# Creare nella propria home directory una sottodirectory A1 
# che contiene una sottodirectory B2 
# che contiene una sottodirectory C3 
# che contiene tre file 1.txt 2.txt e 3.txt. 

# Poi usate il comando move (mv) per spostare i 3 file nella directory B2. 
# Poi usate il comando copy (cp) per mettere una copia dei tre file nella directory A1.

cd /Users/endridomi/Documents/GitHub/Sistemi-Operativi/Laboratorio/es_1-10;
mkdir A11; mkdir A11/B22; mkdir A11/B22/C33;

for num in 11 22 33 ; do touch A11/B22/C33/${num}.txt; done;
mv A11/B22/C33/??.txt A11/B22/ ;
cp A11/B22/??.txt A11/
