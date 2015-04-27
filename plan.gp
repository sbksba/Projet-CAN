#!/usr/bin/gnuplot

set terminal png
set output "Plan.png"
set boxwidth 0.75 absolute
set xtic auto # set xtics automatically
set ytic auto # set ytics automatically
set title "Plan"
set xlabel "X"
set ylabel "Y"
set xr [0:20]
set yr [0.:20]
set object 1 rect from 0,0 to 10,20 fc rgb "white"
set object 2 rect from 10,10 to 15,20 fc rgb "white"
set object 3 rect from 10,0 to 15,10 fc rgb "white"
set object 4 rect from 15,0 to 20,10 fc rgb "white"
set object 5 rect from 17,10 to 20,15 fc rgb "white"
set object 6 rect from 15,15 to 20,20 fc rgb "white"
set object 7 rect from 15,10 to 18,15 fc rgb "white"
set offset 1,1,1,1
plot 'dot.dat' using 1:2:(sprintf("(%d, %d)", $1, $2)) with labels point  pt 7 offset char 1,1 notitle
