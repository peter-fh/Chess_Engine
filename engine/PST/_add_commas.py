
def edit_all_files(piece):
    white_file_name = "w" + piece + ".txt"
    black_file_name = "b" + piece + ".txt"
    print()
    add_comma(white_file_name)
    print()
    add_comma(black_file_name)

def add_comma(fname):
    lines = []
    with open(fname, "r") as inf:
        for line in inf:
            line = line.rstrip('\n') + ","
            lines.append(line)

    with open(fname, "w") as outf:
        for line in lines:
            print(line, file=outf)


edit_all_files("k")
edit_all_files("q")
edit_all_files("r")
edit_all_files("b")
edit_all_files("n")
edit_all_files("p")
