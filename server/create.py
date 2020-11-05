

from flask import Flask, render_template, request
import models
import config

SECRET_KEY = 'abcdef'
MIN_DEPTH = 3.00
MAX_DEPTH = 9.00


models.db.init_app(config.config_app())

def main():
    models.db.create_all()

if __name__ == "__main__":
    with config.app.app_context():
        main()