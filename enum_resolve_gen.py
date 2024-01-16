def generate_switch_statement(enum_values):
    switch_statement = "switch (enum_value) {\n"

    for value, name in enum_values.items():
        switch_statement += f"    case {value}:\n"
        switch_statement += f'       return "{name}";\n'
        #switch_statement += "        break;\n"

    switch_statement += "}\n"

    return switch_statement

def read_enum_from_file(file_path):
    enum_values = {}

    with open(file_path, 'r') as file:
        lines = file.readlines()

    for line in lines:
        line = line.strip()
        if True:
            parts = line.split("=")
            if len(parts) == 2:
                value = parts[1].strip(",")
                name = parts[0].strip()
                enum_values[value] = name

    return enum_values

def write_to_file(output_file_path, content):
    with open(output_file_path, 'w') as file:
        file.write(content)

if __name__ == "__main__":
    input_file = "your_enum_file.txt"  # Replace with your actual file name
    output_file = "switch_statement.txt"  # Replace with your desired output file name

    enum_values = read_enum_from_file(input_file)
    switch_statement = generate_switch_statement(enum_values)

    write_to_file(output_file, switch_statement)
