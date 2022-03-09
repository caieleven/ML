#! /bin/bash
num=$1
i=0


while [ ${i} -lt ${num} ]; do
    echo "num = ${i}"
    i=$((i + 1))
done

i=0


while [ ${i} -lt ${num} ]; do
    echo "num = ${i}"
    i=$((i + 1))
done