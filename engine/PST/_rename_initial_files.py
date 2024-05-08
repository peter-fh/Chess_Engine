import os

def rename(piece):
    file_name = piece.upper() + ".txt"
    os.rename(file_name, "w" + piece.lower() + ".txt")

rename("k")
rename("q")
rename("r")
rename("b")
rename("n")
rename("p")

