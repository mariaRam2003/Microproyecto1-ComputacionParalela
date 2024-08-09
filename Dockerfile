# Usa una imagen base de Ubuntu (o cualquier otra que prefieras)
FROM ubuntu:latest

# Instala GCC, G++, y otras herramientas necesarias
RUN apt-get update && apt-get install -y gcc g++ make

# Crea un directorio de trabajo
WORKDIR /usr/src/myapp

# Copia el código fuente desde el host al contenedor
COPY . /usr/src/myapp

# Define el comando por defecto al iniciar el contenedor
CMD ["/bin/bash"]