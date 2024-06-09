set terminal pngcairo enhanced
set output 'plot.png'
set xlabel 'Energy (eV)'
set ylabel 'f(E)'
set title 'Fermi-Dirac Distribution'
plot \
    '1.txt' using 1:2 with lines title 'z=5000', \
    '2.txt' using 1:2 with lines title 'z=10000', \
    '3.txt' using 1:2 with lines title 'z=20000'
