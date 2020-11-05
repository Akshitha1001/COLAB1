import flask
from http import HTTPStatus
from create import SECRET_KEY
from models import TimeSheet


MIN_DEPTH = 3.00
MAX_DEPTH = 9.00
# normal scenario - input, method call (input), output 



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


def process_GET(data):
    if not data["depth"]:
        return respond_invalid_format()

    switch = decide_switch(data["depth"])

    return respond_with_switch(switch)

def process_POST(data):
    if not data["depth"]:
        return respond_invalid_format()

    switch = decide_switch(data["depth"])
    
    new_entry = TimeSheet(depth = data["depth"],switch = True if switch == 1 else False)
    
    if not new_entry.add_data():
        return flask.make_response(
            {"message" : "some error while storing in database"},
            HTTPStatus.INTERNAL_SERVER_ERROR
        )
    return respond_with_switch(switch)

def is_authorized(key):
    if key != SECRET_KEY:
        return False
    return True

def decide_switch(depth):
    if depth == MIN_DEPTH :  # MIN_DEPTH = 3.00 and MAX_DEPTH = 9.00
        return 1
    elif depth == MAX_DEPTH:
        return 0
    return -1


def respond_with_switch(switch):
    return flask.make_response(
        {"switch" : switch},
            HTTPStatus.OK
    )

def respond_invalid_format():
    return flask.make_response(
        {"message" : "invalid format"},
            HTTPStatus.NOT_ACCEPTABLE
        )
def respond_unauthorized():
    return flask.make_response(
            {"message" : "Unauthorized"},
                HTTPStatus.UNAUTHORIZED
            )
