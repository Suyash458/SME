from flask import Flask,render_template,session,request,jsonify
from flask.ext.sqlalchemy import SQLAlchemy
from flask.ext.mongoengine import MongoEngine
from flask.ext.restful import Api

app = Flask(__name__)
api = Api(app)
app.config.from_object('config')
db = SQLAlchemy(app)
db.init_app(app)

from app import views,models
