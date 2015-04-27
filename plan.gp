#!/usr/bin/gnuplot

set terminal png
set output "Plan.png"
set boxwidth 0.75 absolute
set xtic auto # set xtics automatically
set ytic auto # set ytics automatically
set title "Plan"
set xlabel "X"
set ylabel "Y"
set xr [0:10]
set yr [0.:10]
set object 1 rect from 0,0 to 5,10 fc rgb "white"
set object 2 rect from 5,5 to 7,10 fc rgb "white"
set object 3 rect from 5,0 to 7,5 fc rgb "white"
set object 4 rect from 7,0 to 10,5 fc rgb "white"
set object 5 rect from 8,5 to 10,8 fc rgb "white"
set object 6 rect from 7,8 to 10,10 fc rgb "white"
set object 7 rect from 7,5 to 9,8 fc rgb "white"
set offset 1,1,1,1
plot 'dot.dat' using 1:2:(sprintf("(%d, %d)", $1, $2)) with labels point  pt 7 offset char 1,1 notitle
