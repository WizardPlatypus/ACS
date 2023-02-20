./build.sh

testdir="temp"
if [ "$testdir" == "temp" ]; then
    rm data/temp/*
fi

execs=bin/*
declare -a types=("uint8_t" "int8_t" "uint16_t" "int16_t" "uint32_t" "int32_t" "float" "uint64_t" "int64_t" "double")

for exec in $execs; do
    for type in "${types[@]}"; do
        echo "$exec $type >> data/$testdir/$type.txt"
        $exec $type >> data/$testdir/$type.txt
    done
done