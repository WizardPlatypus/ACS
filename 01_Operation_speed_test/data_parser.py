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

    for _type in table:
        row = ["----"]
        for operation in table[_type]:
            row.append(operation)
        print("\\hline")
        print(*row, sep=" & ", end=" \\\\\n")
        print("\\hline")
        break

    # total_speed = -1
    # counter = 0
    max_speed = -1
    for _type in table:
        for operation in table[_type]:
            count, time = table[_type][operation]
            speed = (count / time) if time > 0 else 0
            max_speed = speed if speed > max_speed else max_speed
            # if speed <= 0:
            #     continue
            # total_speed += speed
            # counter += 1
    
    # avg_speed = total_speed / counter

    for _type in table:
        row = ["$" + _type.replace("_", "\_") + "$"]
        for operation in table[_type]:
            count, time = table[_type][operation]
            speed = ((count / time) / max_speed) if time > 0 else -1
            row.append(round(speed, 3))
        print(*row, sep=" & ", end=" \\\\\n")
        print("\\hline")

if __name__ == "__main__":
    main()