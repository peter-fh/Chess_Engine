def reverse_pst(piece_char):
    in_file = open("w" + piece_char.lower() + ".txt", 'r')
    lines = []
    for line in in_file:
        lines.append(line)
    
    out_file = open("b" + piece_char.lower() + ".txt", 'w')

    for line in reversed(lines):
        out_file.write(line)

reverse_pst('k')
reverse_pst('q')
reverse_pst('r')
reverse_pst('b')
reverse_pst('n')
reverse_pst('p')
