# -*- coding: utf-8 -*-

import sys
import numpy as np

def lee_ficheros(fichero1,fichero2):
	# Abrimos el fichero y guardamos los datos
	f1 = open(fichero1,"r")
	datos1 = f1.readlines()
	f1.close()


	f2 = open(fichero2,"r")
	datos2 = f2.readlines()
	f2.close()
	
	datos1 = np.array(datos1,np.float64)
	datos2 = np.array(datos2,np.float64)

	return datos1, datos2

# Función encargada de calcular el intervalo
def intervalos_confianza(datos):
	media = np.mean(datos)
	varianza = np.var(datos)
	tstudent = 1.9842 # n-1 = 99 y 1-alpha/2 = 0.025

	extremoIzquierdo = media - tstudent * np.sqrt(varianza/len(datos))
	extremoDerecho = media + tstudent * np.sqrt(varianza/len(datos))

	return extremoIzquierdo, extremoDerecho



# Leemos fichero
fichero1 = str(sys.argv[1]) # Fichero Resultados sin afectarle la tormenta 
fichero2 = str(sys.argv[2]) # Fichero Resultados con remolcador mas rapido

data1,data2 = lee_ficheros(fichero1,fichero2)

extremoIzquierdo, extremoDerecho = intervalos_confianza(np.subtract(data1,data2))

print("El intervalo es: [" + str(extremoIzquierdo) + "," + str(extremoDerecho) +"]")