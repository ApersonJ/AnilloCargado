set terminal pngcairo size 800,600
set output '../grafica/campo_electrico.png'
set title 'Mapa Vectorial del Campo Electrico E(x,y)'
set xlabel 'x'
set ylabel 'y'
set xrange [-2:2]
set yrange [-2:2]
set size square
set grid
plot '../resultados/field_data.dat' using 1:2:3:4 with vectors head filled lt 2 title 'Direccion de E(x,y)'
