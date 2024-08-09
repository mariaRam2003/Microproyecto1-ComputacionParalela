# Microproyecto1-ComputacionParalela
 Simulación de Ecosistema con OpenMP

## Instrucciones para correr el programa
En el directorio root del proyecto correr lo siguiente:
```commandline
docker build -t paralela .
```

Posteriormente para ingresar al container corremos:  
```commandline
docker run -it -v $(pwd):/usr/src/ paralela bash
```

Una vez dentro del container compilamos:
```commandline
g++ -fopenmp -o main.o *.cpp
```

y posteriormente corremos:
```commandline
/main.o
```