"""
This file assumes that you have already created all the cpp modules
that you want! It will recompile/bind them in a build folder and
run any arbitrary Python files, so if you have made only implementation
changes within the cpp or Python files, it will work fine. If you change
which cpp modules to load however, you must modify CMakeLists.txt as well.
"""

#!/usr/bin/env python3

# imports here
from pathlib import Path
import datetime, sys, os, shutil, glob

class IllegalArgumentError(ValueError):
	pass

def date(format="%Y%m%d"):
	return datetime.datetime.utcnow().strftime(format)

def make_output_dir() -> Path:
	today = date("%Y%m%d")
	output_dir = Path(".")/f"build_{today}"
	output_dir.mkdir(exist_ok=True)
	return output_dir

def clean_output():
	for d in glob.glob("./build*/"):
		print ("Removed ", d)
		shutil.rmtree(d)

if __name__ == "__main__":
	if "--clean" in sys.argv:
		if len(sys.argv) != 2:
			raise IllegalArgumentError("You cannot clean and build concurrently.")
		else:
			clean_output()

	else:
		proj = os.getcwd()

		#-b for 'build' so as not to get confused with 'clean'
		if "--usebuild" in sys.argv or '-b' in sys.argv:
			raise NotImplementedError

		else:
			outputdir = make_output_dir()
			os.chdir(outputdir)
			os.system("cmake ..")
			os.system("make")
			os.chdir(proj)

			#This lets it find the .so files
			sys.path.insert(0, './'+str(outputdir))
			print ("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
			print ()


		#Put the python files you want to execute here.
		sys.path.insert(0, './src')

		#print (sys.path)

		#WARNING: will want to change this to the location of NTL!Q
		os.environ['LD_LIBRARY_PATH'] = '/usr/local/lib'
		import pyexe