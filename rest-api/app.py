#!/usr/bin/python

from flask import Flask

app = Flask(__name__)

@app.route('/sentione/lemmatize', methods=['POST'])
def lemmatize():
    return "LEMMATIZE"

app.run()
