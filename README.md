# divInstallExporter
Una herramienta para extraer el contenido de los ficheros de instalación creados por Div Games Studio 2

# Uso

## En linux
divInstallExporter fichero.001

## En Windows
divInstallExporter.exe fichero.001

# Compilar

## Requisitos
Para compilar esta utilidad es necesario tener la librería zlib instalada, así como sus .h.

## En linux
make

## En windows
Ejecutar este comando (Se supone que tenemos mingw32 instalado y en el path)

mingw32-make

Tras ello copiar la libreria zlib.dll al directorio bin.

