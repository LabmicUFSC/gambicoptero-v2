set terminal pdf
set output "pid.pdf"
set xrange [0:30]
set xtics 2.5

set style line 1 lt 1 lw 3 pt 1 linecolor rgb "cyan" 
set style line 2 lt 1 lw 3 pt 1 linecolor rgb "red" 

plot "pid.dat" using 1:2 title "Gambicoptero Simulator" ls 1 with lines, \
     "pid.dat" using 1:3 title "Ideal PID" ls 2 with lines