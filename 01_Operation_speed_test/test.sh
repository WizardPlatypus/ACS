./build.sh

testdir="test4"

echo "uint8_t" && ./main.exe uint8_t >> data/$testdir/uint8_t.txt
echo "int8_t" && ./main.exe int8_t >> data/$testdir/int8_t.txt

echo "uint16_t" && ./main.exe uint16_t >> data/$testdir/uint16_t.txt
echo "int16_t" && ./main.exe int16_t >> data/$testdir/int16_t.txt

echo "uint32_t" && ./main.exe uint32_t >> data/$testdir/uint32_t.txt
echo "int32_t" && ./main.exe int32_t >> data/$testdir/int32_t.txt
echo "float" && ./main.exe float >> data/$testdir/float.txt

echo "uint64_t" && ./main.exe uint64_t >> data/$testdir/uint64_t.txt
echo "int64_t" && ./main.exe int64_t >> data/$testdir/int64_t.txt
echo "double" && ./main.exe double >> data/$testdir/double.txt

