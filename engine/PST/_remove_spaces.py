def remove_spaces(piece):
    white_filename = "w" + piece.lower() + ".txt"
    black_filename = "b" + piece.lower() + ".txt"
    with open(white_filename, "r") as f:
        white_lines = f.readlines()

    with open(black_filename, "r") as f:
        black_lines = f.readlines()

    white_lines = [line.replace(' ', '') for line in white_lines]
    black_lines = [line.replace(' ', '') for line in black_lines]

    with open(white_filename, "w") as f:
        f.writelines(white_lines)

    with open(black_filename, "w") as f:
        f.writelines(black_lines)

remove_spaces("k")
remove_spaces("q")
remove_spaces("r")
remove_spaces("b")
remove_spaces("n")
remove_spaces("p")

