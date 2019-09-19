
# Gnuplot Settings for the graphs

set key left top font ",8"
set title font ",10"
set tics font ", 8"
#set term png
set terminal pdf linewidth 1 
#set timefmt "%S"
#set xdata time
set datafile separator ';'
set datafile commentschars "#!%"

set xtics nomirror 5
set mxtics 5
set xrange [-5:250]

set ytics  10
set mytics 5
set yrange [0:235]

set grid
set xlabel "t [s]" font ", 8"
#set ylabel "celsius"
set format x "%3.f"
set xtics rotate by 90 offset 0,-1 out nomirror
#set arrow from  60,40 to  60,200 nohead lt 1 linewidth 2
#set arrow from 120,40 to 120,200 nohead lt 1 linewidth 2

#set out 'pid.png'
#set out 'pid.pdf'
#plot 'pid.dat' using 1:2 notitle with lines
