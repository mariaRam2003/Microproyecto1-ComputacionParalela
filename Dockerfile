# Usa una imagen base de Ubuntu (o cualquier otra que prefieras)
FROM ubuntu:latest

# Instala GCC, G++, y otras herramientas necesarias
RUN apt-get update && apt-get install -y gcc g++ make

# Crea un directorio de trabajo
WORKDIR /usr/src/

COPY . /usr/src/

RUN g++ -fopenmp -o main.o *.cpp
