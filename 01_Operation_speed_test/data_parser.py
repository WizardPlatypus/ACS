import sys

def get_data(file):
    return file.readline().replace(",", " ").split()

def get_type(data):
    return data[0]
def get_size(data):
    return int(data[1])
def get_skip(data):
    return int(data[2])
def get_repeat(data):
    return int(data[3])
def get_operation(data):
    return data[4]
def get_time(data):
    return int(data[5])

def calc_count(size, skip, rept, x):
    # op_count = repeat * (2^bit_count / do_one_over + 1) * (2^bit_count / do_one_over) / 2
    return rept * ((pow(2, size) // skip) * x) * ((pow(2, size) // skip) * x + 1) / 2

def make_table(file, x):
    # table = {"type": {"operation": "speed"}}
    table = {}

    nothing = get_data(file)
    while len(nothing) > 0:
        rest = [
            get_data(file),
            get_data(file),
            get_data(file),
            get_data(file),
            get_data(file),
        ]

        # print(nothing)

        count = calc_count(get_size(nothing), get_skip(nothing), get_repeat(nothing), x)
        table_row = {}
        for test in rest:
            time = get_time(test) - get_time(nothing)
            speed = (count / time) if time > 0 else 0
            table_row[get_operation(test)] = speed
        table[get_type(nothing)] = table_row

        nothing = get_data(file)

    return table


def main():
    filename = sys.argv[1]
    file = open(filename)
    x = int(sys.argv[2])

    table = make_table(file, x)

    for _type in table:
        row = ["----"]
        for operation in table[_type]:
            row.append(operation)
        print(*row)
        break

    total_speed = -1
    counter = 0
    for _type in table:
        for operation in table[_type]:
            speed = table[_type][operation]
            # max_speed = speed if speed > max_speed else max_speed
            if speed <= 0:
                continue
            total_speed += speed
            counter += 1
    
    avg_speed = total_speed / counter

    for _type in table:
        row = [_type]
        for operation in table[_type]:
            speed = table[_type][operation]
            row.append(round(speed / avg_speed, 2))
        print(*row)

if __name__ == "__main__":
    main()