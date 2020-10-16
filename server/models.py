
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime


db = SQLAlchemy()

class TimeSheet(db.Model):
    __tablename__ = "time_sheet"
    id      = db.Column(db.Integer, primary_key=True)
    time    = db.Column(db.DateTime, default = datetime.utcnow)
    depth   = db.Column(db.Float)
    switch  = db.Column(db.Boolean)

    def __repr__(self):
        return ('<Depth %r>' % str(self.depth) +"time : "+str(self.time))
    def add_data(self):
        db.session.add(self)
        db.session.commit()
        return True
        
