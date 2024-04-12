#!/bin/bash

flags=("-n" "-i" "-c" "-v" "-l" "-h" "-s")
patterns=(
"-f pattern1.txt"
"-f pattern2.txt"
"cucumber"
"-e 'tomato\|inc'"
"-e IPAD")

files=("greptest1.txt" "greptest2.txt" "greptest3.txt" "greptest4.txt")
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
for ((k = 0; k < 1000; k++))
do
    selected_flags=""
    selected_files=""
    selected_patterns=""
    num_flags=$((0 + RANDOM % ${#flags[@]}))
    num_files=$((1 + RANDOM % ${#files[@]}))
    num_patterns=$((1 + RANDOM % ${#patterns[@]}))
    
    for ((i=0; i<num_flags; i++));
    do
        index=$((RANDOM % ${#flags[@]}))
        selected_flags+=" ${flags[index]}"
    done
    
    for ((j=0; j<num_files; j++));
    do
        index=$((RANDOM % ${#files[@]}))
        selected_files+=" ${files[index]}"
    done
    
    for ((l=0; l<num_patterns; l++));
    do
        index=$((RANDOM % ${#patterns[@]}))
        selected_patterns+=" ${patterns[index]}"
    done

    .././s21_grep$selected_flags $selected_patterns $selected_files > cmp1
    grep$selected_flags $selected_patterns $selected_files> cmp2
    if diff -q cmp1 cmp2 >/dev/null 2>&1;
    then
        echo "Test $number |grep$selected_flags $selected_patterns $selected_files|: Success"
        ((success++))
    else
        echo "Test $number |grep$selected_flags $selected_patterns $selected_files|: Fail"
        touch differences_logs/diff$((number)).txt
        echo "Test $number |grep$selected_flags $selected_patterns $selected_files|: Fail" >> differences_logs/diff$((number)).txt
        echo "||$(diff -s cmp1 cmp2)" >> differences_logs/diff$((number)).txt
        ((fail++))
    fi
    ((number++))
done

echo "SUCESSs: $success"
echo "FAILs: $fail"
echo "ALL: $((number - 1))"

