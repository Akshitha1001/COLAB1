

from flask import Flask, render_template, request
from models import *

# SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
#     username="JayaShankar",
#     password="nothing098.",
#     hostname="JayaShankar.mysql.pythonanywhere-services.com",
#     databasename="JayaShankar$timeSheet",
# )
SQLALCHEMY_DATABASE_URI = "sqlite:///test4.db"


SECERT_KEY = 'abcdef'
MIN_DEPTH = 5.00


app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
app.config["SQLALCHEMY_POOL_RECYCLE"] = 299
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
db.init_app(app)

def main():
    db.create_all()

if __name__ == "__main__":
    with app.app_context():
        main()