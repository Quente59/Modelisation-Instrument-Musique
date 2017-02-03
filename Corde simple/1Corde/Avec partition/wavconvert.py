import scipy.io.wavfile
import numpy as np
import matplotlib.pyplot as plt

a = open('sortie.dat','r')
son = []

contenu = a.read()
contenu = contenu.split(',')


for i in contenu:
	
	try:
		son.append(float(i))
	except:
		pass

m = max(son)


sample = [int(2 ** 14 * i / m) for i in son]

output = np.array(sample, dtype = np.int16)

plt.plot(output)
plt.show

output_file_name = 'sortie.wav'
scipy.io.wavfile.write(output_file_name, 44100, output)
a.close()
