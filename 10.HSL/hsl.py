
import requests
from pprint import pprint
import Cookie
import hashlib
import urlparse
import urllib
import binascii

"""
Notas: existe un HSL Questionaire con una pregunta de Rick Astley: http://talk.collegeconfidential.com/high-school-life/954981-hsl-questionnaire-edn-2-p3.html
"""

secret = 'x'

print("GET ==================")
url = 'http://54.83.207.93:5252/'
r = requests.get(url)

print("\t Get cookie:")
cookie = Cookie.SimpleCookie(r.cookies)
config = cookie.get("config")
sig = cookie.get("sig")
print("\t - config= %r" % config) 
print("\t - sig= %r" % sig) 
print("\t Check sig computed from config against sig.value")
data = config.value.lower()
data = hashlib.md5(secret + data).hexdigest()
print("\t  - sig-computed = %r" % data)
print("\t  - sig-master   = %r" % sig.value)

print("")
print("\t Hacemos el parse config con %r" % config.value)
print("\t  - config.value= %r" % config.value)
data = urlparse.parse_qs(urlparse.unquote(config.value))
print("\t  - data = %r" % data)
print("\t  - data['db_passwd'] = %r" % data['db_passwd'][0])
print("\t  - data['db_name'] = %r" % data['db_name'][0])


print("\n")
print("POST ==================")

mycookie = {'sig': sig.value, 'config': config.value}
r = requests.post(url, data={'user': 'user', 'password': 'password'}, cookies=mycookie)
print(r.text)
