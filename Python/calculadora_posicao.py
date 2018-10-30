# !/usr/bin/env python2
# -*- coding: utf-8 -*-

import numpy as np
import math

def retorna_posicao(nome_rede):
    if nome_rede == 'A0P0':
        return (0,0)
    elif nome_rede == 'A0P1':
        return (0,2)
    elif nome_rede == 'A0P2':
        return (2,0)

#função que calcula o raio em metros usando os parâmetros da frequência da onda e a potência recebida. free space loss
def calcula_raio(freqMHZ,db):
    exp = (27.55 - (20 * np.log10(freqMHZ)) + np.absolute(db))/20;
    return np.power(10, exp)

#função de trilateração que usa as distâncias calculadas e as posições dos sensores para estimar a localização do alvo
#a função consiste em resolver o sistema de equações obtido através das posições dos sensores e suas distâncias   
def trilateracao(coor):
    # [0][0] posicao x, [0][1] posicao y e [1] raio
    a = coor[0]
    c = coor[1]
    b = coor[2]
    #começando a mudar parte 2.0 (tratamento de erro)
    ok = False
    #errox é a quantidade de iterações no loop de ajuste da variavel "x"
    errox = 1
    #erroy é a quantidade de iterações no loop "not ok", sobre o ajuste do raio de "c"
    erroy = 1
    x = (math.pow(a[1],2) - math.pow(b[1],2) + math.pow(b[0][0],2))/(2*b[0][0])

    #looping para ajustes das medidas dos raios a, b e c
    while(not ok):
        while(x > a[1]):
            a[1] = a[1]+1
            b[1] = b[1]+1
            x = (math.pow(a[1],2) - math.pow(b[1],2) + math.pow(b[0][0],2))/(2*b[0][0])
            errox=errox+1

        y = math.sqrt(math.pow(a[1],2) - math.pow(x,2))
        #as temps são as resoluções da equação "c^2 = (x-c1)^2 + (y-c2)^2", com y sendo positivo e dps negativo
        temp = (math.pow(x-c[0][0],2)) + (math.pow(y-c[0][1],2))
        temp2 = (math.pow(x-c[0][0],2)) + (math.pow(-y-c[0][1],2))

        if(math.pow(c[1],2) >= (temp-1) and math.pow(c[1],2) <= (temp+1)):
            ok = True

        elif(math.pow(c[1],2) >= (temp2-1) and math.pow(c[1],2) <= (temp2+1)):
            y = -y
            ok = True
        
        else:
            if((math.pow(c[1],2) - temp) < (math.pow(c[1],2) - temp2)):
                if(math.pow(c[1],2) > temp):
                    b[1] = b[1]+0.1
                    a[1] = a[1]+0.1
                else:
                    erroy = erroy+1
                    b[1] = b[1]-0.1
                    a[1] = a[1]-0.1

            else:
                if(math.pow(c[1],2) > temp2):
                    b[1] = b[1]+0.1
                    a[1] = a[1]+0.1
                else:
                    b[1] = b[1]-0.1
                    a[1] = a[1]-0.1
            
            erroy = erroy+1
            x = (math.pow(a[1],2) - math.pow(b[1],2) + math.pow(b[0][0],2))/(2*b[0][0])

    #print("errox: ",errox) 
    #print("erroy: ",erroy)
    return x,y

#2412 frequência comum de roteadores
#x = calcula_raio(2400, -54)

#print("distância em metros: ", x)

