import numpy as np
import matplotlib.pyplot as plt
import glob, os


os.chdir("./")
for file in glob.glob("*.func"):
	print(file)
	csv_file = open(file)
	func = np.loadtxt(filter(lambda row: row[0]!='#', csv_file))
	csv_file.close()

	wide = 21.0/9.0
	my_dpi = 120
	hight = 6

	fig = plt.figure(figsize=(hight*wide, hight), dpi=my_dpi)
	plt.rcParams.update({'font.size': 20})
	#plt.title(title)
	plt.rc('text', usetex=True)
	plt.rc('font', family='serif')
	plt.xlabel("x")
	plt.ylabel("y")
	ax = plt.gca()
	ax.ticklabel_format(useOffset=False)
	ax.spines['right'].set_visible(False)
	ax.spines['top'].set_visible(False)
	plt.plot(func[:,0], func[:,1], 'k-')
	plt.scatter(func[0,0], func[0,1], facecolors='k', edgecolors='k')
	plt.scatter(func[-1,0], func[-1,1], facecolors='white', edgecolors='k')
	fig.savefig(os.path.splitext(file)[0]+'.png', dpi=my_dpi, bbox_inches='tight', pad_inches=0)
	plt.close()
 