set terminal pngcairo size 800,600
set output '../grafica/convergencia_angular.png'
set title 'Convergencia del Error Relativo vs Subintervalos (n)'
set xlabel 'n (Subintervalos)'
set ylabel 'Error Relativo'
set logscale x
set logscale y
set grid
plot '../resultados/convergencia_angular.dat' using 1:2 with linespoints title 'Rectangulos', '../resultados/convergencia_angular.dat' using 1:3 with linespoints title 'Trapecio', '../resultados/convergencia_angular.dat' using 1:4 with linespoints title 'Simpson'
