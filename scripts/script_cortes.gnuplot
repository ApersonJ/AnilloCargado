set terminal pngcairo size 800,600
set output '../grafica/cortes_fisicos.png'
set title 'Cortes 1D del Potencial V(x, y_0)'
set xlabel 'x'
set ylabel 'V(x)'
set grid
plot '../resultados/corte_y_0.dat' using 1:3 with lines title 'y_0 = 0.0', '../resultados/corte_y_5.dat' using 1:3 with lines title 'y_0 = 0.5', '../resultados/corte_y_10.dat' using 1:3 with lines title 'y_0 = 1.0'
