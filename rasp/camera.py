import urllib2
import base64
'''
Run this script using python 2.7

Class used to communicate with the Axis camera using
the Axis VAPIX API (http).
'''
class Camera:
	camera_adr = None
	base_url = None
	authHeader = None
	'''
	VAPIX Api specific URL:s
	'''
	vapix_ptz_support = "param.cgi?action=list&group=Properties.PTZ.PTZ"
	vapix_pan_command = "com/ptz.cgi?rpan=%d&camera=%d"
	vapix_pan_to_command = "com/ptz.cgi?pan=%d&camera=%d"
	vapix_tilt_to_command = "com/ptz.cgi?tilt=%d&camera=%d"
	vapix_set_speed_command = "com/ptz.cgi?speed=%d&camera=%d"
	vapix_cam_info = "/com/ptz.cgi?info=1&camera=%d"

	def __init__(self, camera_adr, username, password):
		self.authHeader = base64.encodestring('%s:%s' % (str(username), str(password)))[:-1]
		self.base_url = "http://"+str(camera_adr)+"/axis-cgi"

	'''
	Performs a Http GET request to the URL adr. 
	'''
	def get_request(self, adr):
		request = urllib2.Request(adr)
		request.add_header("Authorization", "Basic %s" % self.authHeader)
		response = urllib2.urlopen(request)
		return response.read()

	'''
	Rotates the camera to the new direction angle. 
	'''
	def rotate(self, angle, camera=1):
		url = (self.base_url + "/" + self.vapix_pan_command) % (angle, camera)
		return self.get_request(url)

	def rotateTo(self, angle, camera=1):
		url = (self.base_url + "/" + self.vapix_pan_to_command) % (angle, camera)
		return self.get_request(url)

	def tiltTo(self, angle, camera=1):
		url = (self.base_url + "/" + self.vapix_tilt_to_command) % (angle, camera)
		#print url
		return self.get_request(url)

	def getCameraInfo(self, camera=1):
		url = self.base_url+self.vapix_cam_info % camera
		return self.get_request(url)

if __name__ == "__main__":
	cam = Camera("192.168.10.101", "root", "bigbroroto")
	#cam.rotate(500, 1)
	#print cam.getCameraInfo(1);
	print cam.rotateTo(30, 1)
	print "Did some stuff with the camera" 
