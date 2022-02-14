#!/usr/bin/python

from flask import Flask, jsonify

app = Flask(__name__)

@app.route('/sentione/lemmatize', methods=['POST'])
def lemmatize():
    return jsonify({'response': 'LEMMATIZE'})

app.run()
