from flask import Flask
from flask_cors import CORS
import subprocess
from datetime import datetime


def run_engine(http_fen):
    engine_process = subprocess.run(["./engine/build/Engine", http_fen], 
                                     capture_output=True, 
                                     text=True)
    err_file = open("log.txt", "a")
    err_file.write(str(datetime.now()))
    err_file.write("\n")
    err_file.write(engine_process.stderr)
    err_file.write("\n\n")
    err_file.close()
    return engine_process.stdout



app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

@app.route('/<fen>')
def recieve_fen(fen):
    return run_engine(fen)



if __name__ == "__main__":

    http_fen = "rnbqkbnr_pppppppp_8_8_8_8_PPPPPPPP_RNBQKBNR+w+KQkq+-+0+1"
    return_fen = run_engine(http_fen)
    print(return_fen)

