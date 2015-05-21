from flask import Flask,render_template,session,request,jsonify
from flask.ext.sqlalchemy import SQLAlchemy
from flask.ext.socketio import SocketIO,emit
from flask.ext.mongoengine import MongoEngine

app = Flask(__name__)
app.config.from_object('config')
socketio = SocketIO(app)
db = SQLAlchemy(app)
db.init_app(app)
from app import models

@app.route('/')
def index():
	return "Hello"
	
@app.route('/ESP', methods = ['GET','POST'])
def func():
	if request.method == 'GET':
		power = request.args.get('power','')
		IP = request.environ['REMOTE_ADDR']
		Device = models.Device.query.filter_by(device_ip = IP).first()
		if not Device:
			temp = models.Device(device_ip = IP)
			db.session.add(temp)
			db.session.commit()
			tempreading = models.Reading(Power = int(power))
			Readings = temp.readings
			Readings.append(tempreading)
			db.session.add(temp )
			db.session.commit()
		else:
			temp = models.Reading(Power = int(power))
			Readings = Device.readings
			Readings.append(temp)
			db.session.add(Device)
			db.session.commit()
	return "Added device."
	
@app.route('/list',methods=['GET','POST'])
def viewall():
	#list = models.Device.query.all()
	#return render_template('index.html',list = list)
	return jsonify(json_list=[i.serialize for i in models.Device.query.all()])
	
@app.route('/chart')
def chart():
	readings = models.Reading.query.all()
	for i in range(len(readings)):
		readings[i] = readings[i].as_dict()
	return render_template('live-server.html',list = readings)