#!/bin/bash

flags=("-s" "-v" "-e" "-t" "-b" "-n")
files=("cattest1.txt" "cattest2.txt" "cattest3.txt" "cattest4.txt")
fail=0
success=0

number=1

if [ -d "differences_logs" ];
then
    rm -rf differences_logs
    mkdir differences_logs
else
    mkdir differences_logs
fi

echo "Start testing!"
for ((k = 0; k < 2000; k++))
do
    selected_flags=""
    selected_files=""
    num_flags=$((0 + RANDOM % 5))
    num_files=$((1 + RANDOM % 4))
    for ((j=0; j<num_files; j++));
    do
        index=$((RANDOM % ${#files[@]}))
        selected_files+=" ${files[index]}"
    done
    
    for ((i=0; i<num_flags; i++));
    do
        index=$((RANDOM % ${#flags[@]}))
        selected_flags+=" ${flags[index]}"
    done

    .././s21_cat$selected_flags $selected_files > cmp1
    cat$selected_flags $selected_files > cmp2
    if diff -q cmp1 cmp2 >/dev/null 2>&1;
    then
        echo "Test $number |cat$selected_flags $selected_files|: Success"
        ((success++))
    else
        echo "Test $number |cat$selected_flags $selected_files|: Fail"
        touch differences_logs/diff$((number)).txt
        echo "Test $number |cat$selected_flags $selected_files|: Fail" >> differences_logs/diff$((number)).txt
        echo "||$(diff -s cmp1 cmp2)" >> differences_logs/diff$((number)).txt
        ((fail++))
    fi
    ((number++))
done

echo "SUCESSs: $success"
echo "FAILs: $fail"
echo "ALL: $((number - 1))"

