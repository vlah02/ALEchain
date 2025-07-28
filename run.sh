while read -r line; do
    echo "$line" >> tmp.s
    cp tmp.s tests/input.s
    make asmtest > tests/out.txt 2>&1
    if grep -q ERROR tests/out.txt; then
        echo "Error after line: $line"
        break
    fi
done < tests/original_input.s
