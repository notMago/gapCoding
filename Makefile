# Nombre del compilador
CXX = g++
# Opciones de compilación
CXXFLAGS = -Wall -std=c++11
# Nombre del archivo fuente y del ejecutable
SRC = gap.cpp
TARGET = code

# Regla para compilar el programa
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)
# Indicación de que los nombres de archivo no son archivos reales
.PHONY: all clean
