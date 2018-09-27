import numpy as np
import math

#função que calcula o raio em metros usando os parâmetros da frequência da onda e a potência recebida. free space loss
def calcula_raio(freqMHZ,db):
    exp = (27.55 - (20 * np.log10(freqMHZ)) + np.absolute(db))/20;
    return np.power(10, exp)

#função de trilateração que usa as distâncias calculadas e as posições dos sensores para estimar a localização do alvo
#a função consiste em resolver o sistema de equações obtido através das posições dos sensores e suas distâncias   
def trilateracao(a,b,c,ra,rb,rc):
    x = (math.pow(ra,2) - math.pow(rb,2) + math.pow(b[0],2))/(2*b[0])
    y = math.sqrt(math.pow(ra,2) - math.pow(x,2))
    temp = (math.pow(x-c[0],2)) + (math.pow(y-c[1],2))
    temp2 = (math.pow(x-c[0],2)) + (math.pow(-y-c[1],2))

    if(math.pow(rc,2) >= (temp-1) and math.pow(rc,2) <= (temp+1)):
        pass

    elif(math.pow(rc,2) >= (temp2-1) and math.pow(rc,2) <= (temp2+1)):
        y = -y
    
    else:
        return "erro",0

    return x,y

#2412 frequência comum de roteadores
#x = calcula_raio(2400, -54)

#print("distância em metros: ", x)

