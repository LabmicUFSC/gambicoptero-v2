set terminal pdf
set output "pid.pdf"
set style line 1 lt 1 lw 3 pt 1 linecolor rgb "cyan" 

plot "pid.dat" ls 1 with lines