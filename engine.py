import subprocess



def engine_run(http_fen):
    return subprocess.run(["./engine/build/Engine", http_fen], capture_output=True, text=True).stdout


if __name__ == "__main__":

    http_fen = "rnbqkbnr_pppppppp_8_8_8_8_PPPPPPPP_RNBQKBNR+w+KQkq+-+0+1"
    print(engine_run(http_fen))

