# label
set title "Dynamic Memory Allocation Time Comparison"
set xlabel "N"
set ylabel "Nano Seconds"

set terminal png
set output "cpu.png"

plot [0:40][0:1000] \
"c" using 2 pointtype -1 with linespoints title "C", \
"sse" using 2 pointtype -1 with linespoints title "SSE", \
"avx" using 2 pointtype -1 with linespoints title "AVX", \
