from app import app,models,api
from flask import render_template,jsonify
from APIs import ReadingList

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
	
@app.route('/chart')
def chart():
	return render_template('live-server.html')
	
api.add_resource(ReadingList,'/list')