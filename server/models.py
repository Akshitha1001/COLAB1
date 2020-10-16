
from flask_sqlalchemy import SQLAlchemy
from flask import Flask,session
from datetime import datetime



# SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
#     username="JayaShankar",
#     password="nothing098.",
#     hostname="JayaShankar.mysql.pythonanywhere-services.com",
#     databasename="JayaShankar$timeSheet",
# )

# SQLALCHEMY_DATABASE_URI = "sqlite:///test2.db"

db = SQLAlchemy()

class TimeSheet(db.Model):
    __tablename__ = "time_sheet"
    id = db.Column(db.Integer, primary_key=True)
    time = db.Column(db.DateTime, default = datetime.utcnow)
    depth = db.Column(db.Float)
    switch = db.Column(db.Boolean)

    def __repr__(self):
        return ('<Depth %r>' % str(self.depth) +"time : "+str(self.time))
    def add_data(self):
        db.session.add(self)
        db.session.commit()
        return True
        
