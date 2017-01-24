import scipy.io.wavfile
import numpy as np
import matplotlib.pyplot as plt

a = open('sortieCorde1.dat','r')
b = open('sortieCorde2.dat','r')
son1 = []
son2 = []

contenu1 = a.read()
contenu1 = contenu1.split(',')

contenu2 = b.read()
contenu2 = contenu2.split(',')

c = open('sortieCorde3.dat','r')
son3 = []

contenu3 = c.read()
contenu3 = contenu3.split(',')


for i in contenu1:
	
	try:
		son1.append(float(i))
	except:
		pass



for i in contenu2:
	
	try:
		son2.append(float(i))
	except:
		pass

for i in contenu3:
	
	try:
		son3.append(float(i))
	except:
		pass


son=[]

j = 0

for i in son1:
	
	try:
		son.append(float(i) + son2[j] + son3[j])
	except:
		pass
	j+=1


m = max(son)


sample = [int(2 ** 14 * i / m) for i in son]

output = np.array(sample, dtype = np.int16)

plt.plot(output)
plt.show

output_file_name = 'sortie.wav'
scipy.io.wavfile.write(output_file_name, 44100, output)
a.close()
