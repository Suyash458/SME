import socket
import sys
import thread
from thread import *
from app import db,models
 
HOST = '0.0.0.0'   # Symbolic name meaning all available interfaces
PORT = 8000 # Arbitrary non-privileged port
 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Socket created'
#Bind socket to local host and port
try:
    s.bind(('', PORT))
except socket.error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()
     
print 'Socket bind complete'
 
#Start listening on socket
s.settimeout(0)
s.listen(10)
print 'Socket now listening'

def serverthread(server_socket):
	while(1):
		try:
			conn,addr = s.accept()
			conn.settimeout(0)
			print 'Connected with ' + addr[0] + ':' + str(addr[1])
			start_new_thread(clientthread ,(conn,addr))
		except socket.error,e:
			if e.errno == '10035':
				pass
		except ValueError:
			pass
		except KeyboardInterrupt or thread.error or SystemExit:
			server_socket.close()
			thread_exit()
			return
			
	

#Function for handling connections. This will be used to create threads
def clientthread(conn,addr):
    #Sending message to connected client     
    #infinite loop so that function do not terminate and thread do not end.
    while True:
		try:
			#Receiving from client
			data = conn.recv(4096)
			if(not data):
				conn.close()
				print "Returning from thread " + str(thread.get_ident())
				thread.exit()
				return
			print addr[0] + " says: " + data
			Device = models.Device.query.filter_by(device_ip = addr[0]).first()
			if not Device:
				temp = models.Device(device_ip = addr[0])
				db.session.add(temp)
				db.session.commit()
				tempreading = models.Reading(Power = int(data))
				Readings = temp.readings
				Readings.append(tempreading)
				db.session.add(temp)
				db.session.commit()
			else:
				temp = models.Reading(Power = int(data))
				Readings = Device.readings
				list = Readings.all()
				if(len(list) > 25):
					db.session.delete(list[0])
				Readings.append(temp)
				db.session.add(Device)
				db.session.commit()
		except socket.error,e:
			if(e.errno == '10035'):
				pass
		except ValueError:
			pass
		except KeyboardInterrupt or thread.error:
			print "Exiting"
			conn.close()
			thread.exit()
			return
     
    #came out of loop
    conn.close()
	
		
serverID = start_new_thread(serverthread,(s,))

#now keep talking with the client
while 1:
	try:
		pass
	except KeyboardInterrupt or thread.error:
		print "Closing from main thread."
		break