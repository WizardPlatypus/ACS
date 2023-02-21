target=$1
files=data/$target/*

rm data/$target.txt

for file in $files; do
    echo "cat $file >> data/$target.txt"
    cat $file >> data/$target.txt
done