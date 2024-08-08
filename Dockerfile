# imagen base de Ubuntu
FROM ubuntu:22.04

# Instala las dependencias necesarias
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    gcc \
    g++ \
    libomp-dev \
    cmake

# Configura el directorio de trabajo
WORKDIR /app

# Copia el código fuente al contenedor
COPY . /app

# Compila el código
RUN g++ -O2 -fopenmp -o ecosystem_simulation main.cpp ecosystem.cpp

# Comando para ejecutar el programa
CMD ["./ecosystem_simulation"]
