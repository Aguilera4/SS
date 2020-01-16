# -*- coding: utf-8 -*-

import sys
import numpy as np

def lee_ficheros(fichero1,fichero2,fichero3,fichero4,fichero5,fichero6,fichero7,fichero8):
	# Abrimos el fichero y guardamos los datos
	f1 = open(fichero1,"r")
	datos1 = f1.readlines()
	f1.close()


	f2 = open(fichero2,"r")
	datos2 = f2.readlines()
	f2.close()

	f3 = open(fichero3,"r")
	datos3 = f3.readlines()
	f3.close()
	
	f4 = open(fichero4,"r")
	datos4 = f4.readlines()
	f4.close()
	
	datos1 = np.array(datos1,np.float64)
	datos2 = np.array(datos2,np.float64)
	datos3 = np.array(datos3,np.float64)
	datos4 = np.array(datos4,np.float64)

	f5 = open(fichero5,"r")
	datos5 = f5.readlines()
	f5.close()


	f6 = open(fichero6,"r")
	datos6 = f6.readlines()
	f6.close()

	f7 = open(fichero7,"r")
	datos7 = f7.readlines()
	f7.close()
	
	f8 = open(fichero8,"r")
	datos8 = f8.readlines()
	f8.close()
	
	datos5 = np.array(datos5,np.float64)
	datos6 = np.array(datos6,np.float64)
	datos7 = np.array(datos7,np.float64)
	datos8 = np.array(datos8,np.float64)


	return datos1, datos2, datos3, datos4, datos5, datos6, datos7, datos8


def iniciar_valores():
	n0 = 40
	h1 = 3.003
	d = 0.01
	k = 4

	return n0, h1, d, k

def calcula_medias(datos1, datos2, datos3, datos4):
	return np.mean(datos1),np.mean(datos2),np.mean(datos3),np.mean(datos4)


def calcula_varianzas(datos1, datos2, datos3, datos4):
	return np.var(datos1),np.var(datos2),np.var(datos3),np.var(datos4)


def calcula_Ni(h1,varianza,d,n0):
	return np.max([n0+1,(int) ((h1**2 * varianza)/ d)])

def calcula_pesos(h1,varianza,d,Ni,n0):
	w1 = ((n0/Ni) + (1-((Ni/n0)* np.sqrt(1-(((Ni-n0)*d**2)/(h1**2 * d))))))
	w2 = 1 - w1

	return w1, w2

def calcula_mediaMuestralPonderada(w1,w2,X1,X2,Ni,n0):
	return ((w1*X1)+(w2*X2))



# Leemos fichero
fichero1 = str(sys.argv[1]) # Fichero Resultados sin afectarle la tormenta con n0 simulaciones
fichero2 = str(sys.argv[2]) # Fichero Resultados con remolcador mas rapido con n0 simulaciones
fichero3 = str(sys.argv[3]) # Fichero Resultados con puntos de atraque a 4 con n0 simulaciones
fichero4 = str(sys.argv[4]) # Fichero Resultados con puntos de atraque a 5 con n0 simulaciones

fichero5 = str(sys.argv[5]) # Fichero Resultados sin afectarle la tormenta con Ni simulaciones
fichero6 = str(sys.argv[6]) # Fichero Resultados con remolcador mas rapido con n0 simulaciones
fichero7 = str(sys.argv[7]) # Fichero Resultados con puntos de atraque a 4 con n0 simulaciones
fichero8 = str(sys.argv[8]) # Fichero Resultados con puntos de atraque a 5 con n0 simulaciones

data1_n0, data2_n0, data3_n0, data4_n0, data5_Ni, data6_Ni, data7_Ni, data8_Ni = lee_ficheros(fichero1,fichero2,fichero3,fichero4,fichero5,fichero6,fichero7,fichero8)


n0, h1, d, k = iniciar_valores()


media_1n0, media_2n0, media_3n0, media_4n0 = calcula_medias(data1_n0, data2_n0, data3_n0, data4_n0)
varianza_1n0, varianza_2n0, varianza_3n0, varianza_4n0 = calcula_varianzas(data1_n0, data2_n0, data3_n0, data4_n0)


N_1 = calcula_Ni(h1,varianza_1n0, d, n0)
N_2 = calcula_Ni(h1,varianza_2n0, d, n0)
N_3 = calcula_Ni(h1,varianza_3n0, d, n0)
N_4 = calcula_Ni(h1,varianza_4n0, d, n0)

print("Simulaciones adicionales ST: ", (N_1-n0))
print("Simulaciones adicionales RMR:: ",(N_2-n0))
print("Simulaciones adicionales PA4:: ",(N_3-n0))
print("Simulaciones adicionales PA5:: ",(N_4-n0))

media_1Ni, media_2Ni, media_3Ni, media_4Ni = calcula_medias(data5_Ni, data6_Ni, data7_Ni, data8_Ni)


w1_1, w2_1 = calcula_pesos(h1,varianza_1n0,d,N_1,n0)
w1_2, w2_2 = calcula_pesos(h1,varianza_1n0,d,N_2,n0)
w1_3, w2_3 = calcula_pesos(h1,varianza_1n0,d,N_3,n0)
w1_4, w2_4 = calcula_pesos(h1,varianza_1n0,d,N_4,n0)


media_muestral_1 = calcula_mediaMuestralPonderada(w1_1,w2_1,media_1n0,media_1Ni,N_1,n0)
media_muestral_2 = calcula_mediaMuestralPonderada(w1_2,w2_2,media_2n0,media_2Ni,N_2,n0)
media_muestral_3 = calcula_mediaMuestralPonderada(w1_3,w2_3,media_3n0,media_3Ni,N_3,n0)
media_muestral_4 = calcula_mediaMuestralPonderada(w1_4,w2_4,media_4n0,media_4Ni,N_4,n0)


print("Media muestral ponderada (Sin afectarle las tormentas): ", media_muestral_1)
print("Media muestral ponderada (Remolcador mas rapido): ", media_muestral_2)
print("Media muestral ponderada (Puntos de atraque a 4): ", media_muestral_3)
print("Media muestral ponderada (Puntos de atraque a 5): ", media_muestral_4)
