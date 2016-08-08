while IFS=, read col1 col2 col3 col4 col5 col6 col7
do
    echo "INSERT INTO stock (code, lastSalePrice, shares) VALUES ('$col2', $col3, $col7);"
done < database/sp080226.txt
