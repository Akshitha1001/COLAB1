
from flask import Flask
# SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
#     username="JayaShankar",
#     password="nothing098.",
#     hostname="JayaShankar.mysql.pythonanywhere-services.com",
#     databasename="JayaShankar$timeSheet",
# )

SQLALCHEMY_DATABASE_URI = "sqlite:///test4.db"

app = Flask(__name__)

def config_app():
    app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
    app.config["SQLALCHEMY_POOL_RECYCLE"] = 299
    app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
    return app

