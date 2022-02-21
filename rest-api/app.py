#!/usr/bin/python

import json
from flask import Flask, jsonify, request
import polem_enrich

app = Flask(__name__)

@app.route('/sentione/polem-enrich', methods=['POST'])
def enrich():
    if request.content_type != 'application/json':
        return "Invalid content type. Expecting application/json.", 415

    response = _polem_enrich.enrich(json.dumps(request.get_json()))
    if response == "":
        return "Invalid request.", 400
    response_json = json.loads(response)
    return jsonify(response_json)

if __name__ == "__main__":
    global _polem_enrich
    _polem_enrich = polem_enrich.Polem_enrich()
    app.run(host="0.0.0.0", port=5000)
