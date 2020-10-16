from flask import session,render_template,request,redirect,Flask,make_response
from flask_sqlalchemy import SQLAlchemy
import requests
from datetime import datetime
from models import *
from create import SQLALCHEMY_DATABASE_URI,SECERT_KEY,MIN_DEPTH


app = Flask(__name__)


app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
app.config["SQLALCHEMY_POOL_RECYCLE"] = 299
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
db.init_app(app)


@app.route('/<key>/depth',methods=['GET'])
def depth_GET(key):
    if(key!=SECERT_KEY):
        return make_response(
                {"message" : "Unauthorized"},
                401
            )

    try:
        depth=float(request.args.get("d"))
    except:
        return make_response(
        {"message" : "invalid format"},
            402
        )
    switch = decide_switch(depth)

    return make_response(
        {"switch" : switch},
        200,
    )
        

@app.route('/<key>/depth',methods=['POST'])
def depth_POST(key):
    if(key!=SECERT_KEY):
        return make_response(
                {"message" : "Unauthorized"},
                401
            )

    req_data = request.get_json()
    try:
        depth=float(req_data["depth"])
    except:
        return make_response(
            {"message" : "invalid format"},
                402
            )

    result=False
    switch = decide_switch(depth)
    if(switch):
        result=True
    new_entry = TimeSheet(depth = depth,switch = result)
    
    if(not new_entry.add_data()):
        return make_response(
            {"message" : "some error while storing in database"},
            401
        )
    return make_response(
        {"switch" : switch},
            200
    )

def decide_switch(depth):
    if(depth>MIN_DEPTH):
        return 1
    return 0

if __name__ == "__main__" :
    app.run(debug=True)