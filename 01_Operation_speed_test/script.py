def find_do_over_one(bit_count, operation_count, repeat = 1):
    # op_count = repeat * (2^bit_count / do_one_over + 1) * (2^bit_count / do_one_over) / 2
    return repeat * pow(2, bit_count - 2) * (1 + pow(1 + 8 * operation_count / repeat, 0.5)) / operation_count;

operation_count = int(input("Operation count: ")) # 134_742_016
repeat = 1 # int(input("Repeat: "))

do_over_8 = find_do_over_one(8, operation_count, repeat)
do_over_16 = find_do_over_one(16, operation_count, repeat)
do_over_32 = find_do_over_one(32, operation_count, repeat)
do_over_64 = find_do_over_one(64, operation_count, repeat)

print("8:", do_over_8, round(do_over_8))
print("16:", do_over_16, round(do_over_16))
print("32:", do_over_32, round(do_over_32))
print("64:", do_over_64, round(do_over_64))