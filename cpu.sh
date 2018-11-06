#!/bin/bash
set -ex

graph_conf="cpu.gp"
graph="cpu.png"
instr=("c" "sse" "avx")

for var in ${instr[@]}
do
    rm -rf ${var}

    tmp="tmp-${var}"
    make distclean

    # different CPU instruction
    case ${var} in
        "c")
            make BUILD_SSE=0
            ;;
        "sse")
            make
            ;;
        "avx")
            make BUILD_AVX=1
            ;;
    esac

    ./build/test-pow > ${tmp}

    # extract the execution timestamp
    while read line
    do
	if [[ "${line}" =~ ^[0-9]+ ]]; then
	    echo "${line}" >> ${var}
	fi
    done < ${tmp}
done

# generate the graph
gnuplot ${graph_conf}
eog ${graph} &
