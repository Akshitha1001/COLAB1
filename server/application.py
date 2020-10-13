from flask import session,render_template,request,redirect,Flask,make_response
from flask_sqlalchemy import SQLAlchemy
import requests
from datetime import datetime

app = Flask(__name__)
SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
    username="JayaShankar",
    password="nothing098.",
    hostname="JayaShankar.mysql.pythonanywhere-services.com",
    databasename="JayaShankar$timeSheet",
)
app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
app.config["SQLALCHEMY_POOL_RECYCLE"] = 299
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
db = SQLAlchemy(app)


class TimeSheet(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    time = db.Column(db.DateTime, default = datetime.utcnow)
    depth = db.Column(db.Float)
    switch = db.Column(db.Boolean)

    def __repr__(self):
        return '<User %r>' % self.id


@app.route('/<key>/depth',methods=['POST','GET'])
def index(key):
    if(key=="abcdef"):
        if(request.method=='GET'):
            try:
                depth=float(request.args.get("d"))
            except:
                return make_response(
                {"message" : "invalid format"},
                402
            )
            if(depth>5.00):
                return make_response(
                    {"switch" : 1},
                    200,
                )
            return make_response(
                {"switch" : 0},
                200
            )
        if(request.method=='POST'):
            req_data = request.get_json()
            try:
                depth=float(req_data["depth"])
            except:
                return make_response(
                    {"message" : "invalid"},
                        402
                    )

            result=False
            switch=0
            if(depth>5.00):
                result=True
                switch=1
            new_entry = TimeSheet(depth = depth,switch = result)
            try:
                db.session.add(new_entry)
                db.session.commit()
            except:
                return make_response(
                    {"message" : "some error while storing in database"},
                    401
                )
            return make_response(
                {"switch" : switch},
                    200
            )
    else:
        return make_response(
                {"message" : "Unauthorized"},
                401
            )


if __name__ == "__main__" :
    app.run(debug=True)