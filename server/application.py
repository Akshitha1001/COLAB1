from flask import Flask
from flask.scaffold import F
import helpers
import config


app = Flask(__name__)

app = config.config_app()


@app.route('/<key>/depth',methods=['GET'])
def depth_GET(key):

    if not helpers.is_authorized(key):
        return helpers.respond_unauthorized()
    
    data = helpers.extract_url_data()

    return helpers.process_GET(data)
    
        

@app.route('/<key>/depth',methods=['POST'])
def depth_POST(key):


    if not helpers.is_authorized(key):
        return helpers.respond_unauthorized()
    
    data = helpers.extract_body_data()

    return helpers.process_POST(data)




if __name__ == "__main__" :
    app.run(host='0.0.0.0')