import os

studentID  = raw_input('Enter your studetn_ID:')
pNUM       = raw_input('Project 1/2/3:')
if(os.path.exists(studentID+'_0'+pNUM+'.tar.gz')):
	print('OK => '+'"'+studentID+'_0'+pNUM+'.tar.gz" '+'exists')
else:
	print('ERROR => '+'"'+studentID+'_0'+pNUM+'.tar.gz" '+'doesn\'t exist')
	exit()

retv = os.system('tar -zxvf '+studentID+'_0'+pNUM+'.tar.gz > /dev/null 2>&1')
if(retv == 0):
	print('OK => '+'Successful to do "tar -zxvf '+studentID+'_0'+pNUM+'.tar.gz"')
else:
	print('ERROR => '+'Fail to do "tar -zxvf '+studentID+'_0'+pNUM+'.tar.gz"')
	exit()
	
if(os.path.exists(studentID+'_0'+pNUM)):
	print('OK => '+'Folder '+'"'+studentID+'_0'+pNUM+'" exists')
else:
	print('ERROR => '+'Folder'+'"'+studentID+'_0'+pNUM+'" doesn\'t exist')
	exit()

if(os.path.exists(studentID+'_0'+pNUM+'/simulator')):
	print('OK => '+'Folder '+'"'+studentID+'_0'+pNUM+'/simulator" exists')
else:
	print('ERROR => '+'Folder '+'"'+studentID+'_0'+pNUM+'/simualtor" doesn\'t exist')
	exit()

if(os.path.exists(studentID+'_0'+pNUM+'/testcase')):
	print('OK => '+'Folder '+'"'+studentID+'_0'+pNUM+'/testcase" exists')
else:
	print('ERROR => '+'Folder '+'"'+studentID+'_0'+pNUM+'/testcase" doesn\'t exist')
	exit()

os.chdir(studentID+'_0'+pNUM+'/simulator')
retv = os.system('make > /dev/null 2>&1')
if(retv==0):
	print('OK => '+'Successful to do "make"')
else: 
	print('ERROR => '+'Fail to do "make"')
	exit()

if(pNUM=='1'):
	if(os.path.exists('single_cycle')):
		print('OK => program '+'"'+studentID+'_0'+pNUM+"/simulator/single_cycle\""+' exists')
	else:
		print('ERROR => program '+'"'+studentID+'_0'+pNUM+"/simulator/single_cycle\""+' doesn\'t exists')

if(pNUM=='2'):
	if(os.path.exists('pipeline')):
		print('OK => program '+'"'+studentID+'_0'+pNUM+"/simulator/pipeline\""+' exists')
	else:
		print('ERROR => program '+'"'+studentID+'_0'+pNUM+"/simulator/pipeline\""+' doesn\'t exists')

if(pNUM=='3'):
	if(os.path.exists('CMP')):
		print('OK => program '+'"'+studentID+'_0'+pNUM+"/simulator/CMP\""+' exists')
	else:
		print('ERROR => program '+'"'+studentID+'_0'+pNUM+"/simulator/CMP\""+' doesn\'t exists')

os.chdir('../testcase')
if(os.path.exists('iimage.bin')):
	print('OK => '+'"'+studentID+'_0'+pNUM+"/testcase/iimage.bin\""+' exists')
else:	
	print('ERROR => '+'"'+studentID+'_0'+pNUM+"/testcase/iimage.bin\""+' doesn\'t exists')
	
if(os.path.exists('dimage.bin')):
	print('OK => '+'"'+studentID+'_0'+pNUM+"/testcase/dimage.bin\""+' exists')
else:	
	print('ERROR => '+'"'+studentID+'_0'+pNUM+"/testcase/dimage.bin\""+' doesn\'t exists')
