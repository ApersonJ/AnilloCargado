set terminal pngcairo size 800,600
set output '../grafica/potencial_3D_N120.png'
set title 'Superficie 3D del Potencial V(x,y) (N=120)'
set xlabel 'x'
set ylabel 'y'
set zlabel 'V(x,y)'
set pm3d
set hidden3d
splot '../resultados/potencial_N120.dat' using 1:2:3 with lines
