from app import db
import datetime

def dump_datetime(value):
	if value is None:
		return None
	return value.strftime("%Y-%m-%d"),value.strftime("%H:%M:%S")

class Device(db.Model):
	id = db.Column(db.Integer, primary_key = True)
	device_ip = db.Column(db.String(64))
	readings = db.relationship("Reading",backref="device",lazy='dynamic')
	
	@property
	def serialize(self):
		return {
				'id':self.id,
				'ip':self.device_ip,
				'readings':self.serialize_onetomany
		}
		
	@property
	def serialize_onetomany(self):
		return [item.serialize for item in self.readings]
	
class Reading(db.Model):
	id = db.Column(db.Integer, primary_key=True)
	Power = db.Column(db.Integer)
	device_id = db.Column(db.Integer,db.ForeignKey('device.id'))
	timestamp = db.Column(db.DateTime,default=datetime.datetime.utcnow)
	
	@property
	def serialize(self):
		return {
				'id':self.id,
				'power':self.Power,
				'device_id':self.device_id,
				'timestamp':dump_datetime(self.timestamp)
		}