from flask import Flask,make_response
import flask
from flask_sqlalchemy import SQLAlchemy
from http import HTTPStatus
import requests
from datetime import datetime
from models import TimeSheet
from create import SECRET_KEY,MIN_DEPTH
import config


app = Flask(__name__)

app = config.config_app()


@app.route('/<key>/depth',methods=['GET'])
def depth_GET(key):
    if(key != SECRET_KEY):
        return make_response(
                {"message" : "Unauthorized"},
                HTTPStatus.UNAUTHORIZED
            )

    data = extract_url_data()

    if(not data["depth"]):
        return respond_invalid_format()

    switch = decide_switch(data["depth"])

    return make_response(
        {"switch" : switch},
        HTTPStatus.OK,
    )
        

@app.route('/<key>/depth',methods=['POST'])
def depth_POST(key):
    if(key != SECRET_KEY):
        return make_response(
                {"message" : "Unauthorized"},
                HTTPStatus.UNAUTHORIZED
            )

    
    data = extract_body_data()
    if(not data["depth"]):
        return respond_invalid_format()

    switch = decide_switch(data["depth"])
    
    new_entry = TimeSheet(depth = data["depth"],switch = True if switch == 1 else False)
    
    if(not new_entry.add_data()):
        return make_response(
            {"message" : "some error while storing in database"},
            HTTPStatus.INTERNAL_SERVER_ERROR
        )
    return make_response(
        {"switch" : switch},
            HTTPStatus.OK
    )


def extract_url_data():
    data = {}
    try:
        data["depth"] = float(flask.request.args.get("d"))
    except:
        data["depth"] = None
    return data

def extract_body_data():
    req_data = flask.request.get_json()
    data = {}
    try:
        data["depth"] = float(req_data["depth"])
    except:
        data["depth"] = None

    return data


def respond_invalid_format():
    return make_response(
        {"message" : "invalid format"},
            HTTPStatus.NOT_ACCEPTABLE
        )

def decide_switch(depth):
    if(depth > MIN_DEPTH):
        return 1
    return 0

if __name__ == "__main__" :
    app.run(debug = True)