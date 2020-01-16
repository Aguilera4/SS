# -*- coding: utf-8 -*-

import sys

# Leemos fichero
fichero1 = str(sys.argv[1])
fichero2 = str(sys.argv[2])


# Abrimos el fichero y guardamos los datos
f1 = open(fichero1,"r")
datos1 = f1.readlines()
f1.close()


f2 = open(fichero2,"r")
datos2 = f2.readlines()
f2.close()


contador1 = 0
contador2 = 0


for i in range(len(datos1)):
	if datos1[i] < datos2[i]:
		contador1+=1
	else:
		contador2+=1


print("Fichero1 obtiene: " + str(contador1) + "%\n")
print("Fichero2 obtiene: " + str(contador2) + "%")

