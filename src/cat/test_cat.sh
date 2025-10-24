# !/bin/bash

text_file=text.txt

run_test(){
    args="$*"

    cat $args $text_file > cat_out.txt
    ./s21_cat $args $text_file > s21_cat_out.txt

    diff_output=$(diff cat_out.txt s21_cat_out.txt -s)
    total=$((total+1))

    rm cat_out.txt s21_cat_out.txt
    
    if [[ $diff_output == *"are identical"* ]]; then
        echo "FLAG $args PASS"
        pass=$((pass+1))
    else
        echo "FLAG $args FAIL"
    fi



}
echo "TESTING CAT UTILS"
run_test -b
run_test -e
run_test -n
run_test -s
run_test -t
run_test -v
run_test -b -s -t 
run_test -b -s -v

# run_test --number-nonblank
# run_test --number
# run_test --squeeze-blank

echo "total:    $total"
echo "passed:   $pass"