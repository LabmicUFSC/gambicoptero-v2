set terminal pdf
set output "pid.pdf"
set xrange [0:50]
set xtics 2.5

set style line 1 lt 1 lw 3 pt 1 linecolor rgb "red" 
set style line 2 lt 1 lw 3 pt 1 linecolor rgb "yellow" 
set style line 3 lt 1 lw 3 pt 1 linecolor rgb "blue" 
set style line 4 lt 1 lw 3 pt 1 linecolor rgb "magenta" 

plot "pid.dat" using 1:2 title "(P, I, D) = (2.5, 1, -10)" ls 1 with lines, \
     "pid.dat" using 1:3 title "(P, I, D) = (5, 2, -20)" ls 2 with lines, \
     "pid.dat" using 1:4 title "(P, I, D) = (10, 4, -40)" ls 3 with lines, \
     "pid.dat" using 1:5 title "Ideal PID" ls 4 with lines
