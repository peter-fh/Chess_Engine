def print_table(table_name):
    filename = table_name + ".txt"
    print("Table: " + table_name)
    with open(filename, "r") as f:
        print("|" + "-" * 31 + "|")
        for line in f:
            print("|", end="")
            for square in line.split(","):
                print("%3s" % square, end="|")
            print("-" * 31 + "|")

print_table("wq")
