from app import api,db,models
from flask.ext.restful import Resource 
from flask import jsonify


class ReadingList(Resource):
	def get(self):
		return jsonify(list = [i.serialize for i in models.Reading.query.all()])
	
	def put(self):
		pass
	
	def delete(self):
		pass
	