from flask import Flask

app = Flask(__name__)

@app.route('/<board>%20<turn>')
def engine(board, turn):

    return '2'

    
