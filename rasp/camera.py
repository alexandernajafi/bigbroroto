import httplib2
'''
Run this script using python 2.7

Class used to communicate with the Axis camera using
the Axis VAPIX API (http).
'''
class Camera:
	camera_adr = None
	base_url = None

	'''
	VAPIX Api specific URL:s
	'''
	vapix_ptz_support = "param.cgi?action=list&group=Properties.PTZ.PTZ"
	vapix_pan_command = "com/ptz.cgi?rpan=%d&camera=%d"

	def __init__(self, camera_adr=None):
		if camera_adr == None:
			raise TypeError("Not a valid camera adress!")
		else:
			self.camera_adr = camera_adr
			base_url = "http://"+camera_adr+"axis-cgi/"

			# Check if camera supports PTZ (also a check to check connection to the camera)
			ptz_resp_code, ptz_content = self.get_request(base_url+vapix_ptz_support)
			if ptz_resp_code != 200:
				print "Warning! Camera does not support PTZ."

	'''
	Performs a Http GET request to the URL adr. 
	'''
	def get_request(self, adr):
		return httplib2.Http().request(adr)

	'''
	Rotates the camera to the new direction angle. 
	'''
	def rotate(self, angle, camera):
		vapix_pan_command = self.vapix_pan_command % (angle, camera)
		url = base_url+vapix_pan_command
		resp_code, content = self.get_request(url)
