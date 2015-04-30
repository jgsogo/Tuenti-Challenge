import hashlib
computed = hashlib.md5(open("sheet.data").read()).hexdigest()
if computed == 'eb8294008e7dbf4e6dffb7edadafbefe':
              print "Match!"