import sys

def get_data(file):
    return file.readline().replace(",", " ").split()

def get_type(data):
    return data[0]
def get_size(data):
    return int(data[1])
def get_x(data):
    return int(data[2])
def get_repeat(data):
    return int(data[3])
def get_operation(data):
    return data[4]
def get_time(data):
    return int(data[5])

def make_table(file):
    # table = {"type": {"operation": "speed"}}
    table = {}

    data = get_data(file)
    while len(data) > 0:
        tests = [
            data,
            get_data(file),
            get_data(file),
            get_data(file),
            get_data(file),
            get_data(file),
        ]

        nothing = []
        for test in tests:
            if get_operation(test) == "nothing":
                nothing = test
                break

        table_row = {}
        for test in tests:
            if get_operation(test) == "nothing":
                continue

            count = get_repeat(test) * get_x(test)
            time = get_time(test) - get_time(nothing)
            operation = get_operation(test)

            if operation in table_row:
                old_count, old_time = table_row[operation]
                table_row[operation] = (old_count + count, old_time + time)
            else:
                table_row[operation] = (count, time)

        table[get_type(nothing)] = table_row

        data = get_data(file)

    return table


def main():
    filename = sys.argv[1]
    file = open(filename)

    table = make_table(file)

    max_speed = -1
    for _type in table:
        for operation in table[_type]:
            count, time = table[_type][operation]
            speed = (count / time)
            max_speed = speed if speed > max_speed else max_speed
    
    type_order = ["uint8_t", "int8_t", "uint16_t", "int16_t", "uint32_t", "int32_t", "uint64_t", "int64_t", "float", "double"]
    operation_order = ["add", "subtract", "multiply", "divide", "modulo"]

    print("\\begin{tabular}{|c|c|c|c|c|c|}")
    print("\\hline")
    print("", *operation_order, sep=" & ", end=" \\\\\n")
    print("\\hline")
    for _type in type_order:
        if _type not in table:
            continue
        display_type = _type.replace("_", "\_")
        row = [ f"${display_type}$" ]
        for operation in operation_order:
            if operation not in table[_type]:
                print("Missing operation: ", operation)
                continue
            count, time = table[_type][operation]
            ratio = ((count / time) / max_speed)
            display = f"{ratio:.2f}" if time > 0 else ""
            row.append(display)
        print(*row, sep=" & ", end=" \\\\\n")
        print("\\hline")
    print("\\end{tabular}")

if __name__ == "__main__":
    main()