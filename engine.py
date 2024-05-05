from flask import Flask
from flask_cors import CORS
import subprocess


def run_engine(http_fen):
    return subprocess.run(["./engine/build/Engine", http_fen], capture_output=True, text=True).stdout


def debug_engine(http_fen):
    engine_process = subprocess.run(["./engine/build/Engine", http_fen], capture_output=True, text=True)
    print("Error:")
    print(engine_process.stderr)
    print("Out:")
    print(engine_process.stdout)


app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

@app.route('/<fen>')
def recieve_fen(fen):
    return run_engine(fen)



if __name__ == "__main__":

    http_fen = "rnbqkbnr_pppppppp_8_8_8_8_PPPPPPPP_RNBQKBNR+w+KQkq+-+0+1"
    return_fen = debug_engine(http_fen)

