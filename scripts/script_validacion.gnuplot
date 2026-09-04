set terminal pngcairo size 800,600
set output '../grafica/validacion_eje_z.png'
set title 'Validacion sobre el Eje Z: Numérico vs Analitico'
set xlabel 'z'
set ylabel 'V(0,0,z)'
set grid
plot '../resultados/validacion_z.dat' using 1:2 with lines lw 2 title 'Analitico Exacto', '../resultados/validacion_z.dat' using 1:5 with points pt 7 ps 1.5 title 'Simpson (Numérico)'
