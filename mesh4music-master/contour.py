from Tkinter import *

root = Tk()

background_image = PhotoImage(file="/home/lee29u/Documents/Electif Instrument de Musique/Guitare.gif")
w = background_image.width()
h = background_image.height()

cv = Canvas(root, bg="white", width=w, height=h)
cv.create_image(w/2, h/2, image=background_image)
"""create_oval(x1, y1, x2, y2, fill="black")
create_line(points, tags="theline", fill="red")"""
cv.pack()

Int=[]
Ext=[]

compteurInt = 0
compteurExt = 0

def pointsInt(event):
	
	global compteurInt
	Int.append((event.x, event.y))
	cv.create_oval(event.x+2, event.y+2, event.x-2, event.y-2, fill="black")
	if len(Int)>=2 :
		cv.create_line(Int[-1], Int[-2], tags="theline", fill="red")
	compteurInt += 1
	print(compteurInt)

def pointsExt(event):

	global compteurExt
	Ext.append((event.x, event.y))
	cv.create_oval(event.x+2, event.y+2, event.x-2, event.y-2, fill="black")
	if len(Ext)>=2 :
		cv.create_line(Ext[-1], Ext[-2], tags="theline", fill="red")
	compteurExt += 1
	print(compteurExt)

def ecriture(event):
	
	fichier = open("coordonnees.txt", 'w')
	fichier.write(str(compteurExt) + " " + str(compteurInt) +"\n")
	fichier.write("#" + "\n")
	for i in range(compteurExt):
		fichier.write( str(Ext[i][0])+ " " + str(Ext[i][1]) +"\n")
	fichier.write("#" + "\n")
	for i in range(compteurInt):
		fichier.write( str(Int[i][0])+ " " + str(Int[i][1]) +"\n")
	fichier.close()
	print("done")


root.bind("<Key>", ecriture)
root.bind("<Button-1>", pointsInt)
root.bind("<Button-3>", pointsExt)

root.mainloop()


