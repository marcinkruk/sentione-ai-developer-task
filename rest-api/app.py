#!/usr/bin/python

import json
from flask import Flask, jsonify, request
import polem_enrich

app = Flask(__name__)

_polem_enrich = None

@app.before_first_request
def before_first_request_f():
    global _polem_enrich
    _polem_enrich = polem_enrich.Polem_enrich()

@app.route('/sentione/polem-enrich', methods=['POST'])
def lemmatize():
    if request.content_type != 'application/json':
        return "Invalid content type. Expecting application/json.", 415

    j = request.get_json()
    echo = _polem_enrich.echo(str(j))
    js = json.loads('"' + echo + '"')
    return jsonify(j)

app.run()
