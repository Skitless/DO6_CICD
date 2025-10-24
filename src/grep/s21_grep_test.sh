# !/bin/bash

first_file=test_file_1.txt
second_file=test_file_2.txt


run_test() {
    grep_args="$*"
    
    grep $grep_args > grep_out
    ./s21_grep $grep_args > s21_grep_out
    
    diff_output=$(diff grep_out s21_grep_out -s)
    total=$((total+1))
    
    rm grep_out s21_grep_out
    
    if [[ $diff_output == *"are identical"* ]]; then
        echo "FLAGS $grep_args PASS"
        pass=$((pass+1))
    else
        echo "FLAGS $grep_args FAIL"
        echo "grep $grep_args" >> grep_log.txt
    fi
    

}

echo "TEST GREP"

run_test int $first_file file_not $second_file -s

run_test int $first_file $second_file -i
run_test int $first_file $second_file -v
run_test int $first_file $second_file -c
run_test int $first_file $second_file -l
run_test int $first_file $second_file -n
run_test int $first_file $second_file -h

run_test int $first_file -i
run_test int $first_file -v
run_test int $first_file -c
run_test int $first_file -l
run_test int $first_file -n
run_test int $first_file -h

run_test int $first_file $second_file -iv
run_test int $first_file $second_file -vc
run_test int $first_file $second_file -cl
run_test int $first_file $second_file -ln
run_test int $first_file $second_file -nh
run_test int $first_file $second_file -hv

run_test $first_file $second_file -iv -e int
run_test -e int $first_file $second_file -vc
run_test $first_file -e int $second_file -cl

run_test int $first_file -iv
run_test int $first_file -vv
run_test int $first_file -cv
run_test int $first_file -lv
run_test int $first_file -nv
run_test int $first_file -hv

run_test $first_file $second_file -e int
run_test int $second_file -i
run_test int $first_file $second_file -c
run_test int $second_file -c
run_test int $second_file -ci
run_test int $first_file $second_file -li

run_test int $first_file $second_file -n
run_test int $first_file $second_file -n
run_test int $first_file $second_file -ni
run_test int $first_file $second_file -nv
run_test int $first_file $second_file -nlih
run_test int $first_file $second_file -nli
run_test int $first_file $second_file -h

run_test int $first_file $second_file -iv
run_test int $first_file $second_file -in
run_test int $first_file -iv
run_test int $first_file -in

run_test $first_file $second_file -f regexp
run_test $first_file $second_file -f regexp -v
run_test $first_file $second_file -f regexp -cl

run_test $first_file $second_file -f regexp -vo
run_test $first_file $second_file -f regexp -clo

run_test int $first_file -co
run_test int $first_file -lo

run_test int $first_file $second_file -co
run_test int $first_file $second_file -lo

run_test int $first_file -io
run_test int $first_file -nvo
run_test int $first_file -hvo

run_test int $first_file $second_file -io
run_test int $first_file $second_file -vo
run_test int $first_file $second_file -no
run_test int $first_file $second_file -ho

run_test int $first_file -io
run_test int $first_file -vo
run_test int $first_file -no
run_test int $first_file -ho
run_test int $first_file -o



echo "TEST RESULTS"

echo "total: $total"
echo "passed: $pass"

