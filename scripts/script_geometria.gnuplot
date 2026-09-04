set terminal pngcairo size 800,600
set output '../grafica/dependencia_geometrica.png'
set title 'Dependencia del Potencial con el Radio R (Corte y=0)'
set xlabel 'x'
set ylabel 'V(x)'
set grid
plot '../resultados/corte_R_5.dat' using 1:2 with lines title 'R = 0.5', '../resultados/corte_R_10.dat' using 1:2 with lines title 'R = 1.0', '../resultados/corte_R_20.dat' using 1:2 with lines title 'R = 2.0'
