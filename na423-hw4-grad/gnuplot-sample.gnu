# this is a sample gnuplot script

set xr [0:2*pi]

set yrange [-2:2]

pl sin(x) w lines

set zeroaxis

set xlabel 'x'

set ylabel 'f(x)'

set term post color enhanced eps
set out 'sinx.eps'
rep
