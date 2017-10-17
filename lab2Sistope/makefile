
DIR_OUTPUT = ./out
DIR_SRC = ./source
NOMBRE_EJECUTABLE = lab2
NOMBRE_SRC = main.c

all: lab1

# Compilación de lab1 (coordinador) y el comparador.
lab1:
	@echo 'Compilando lab2'
	mkdir -p $(DIR_OUTPUT)
	gcc $(DIR_SRC)/$(NOMBRE_SRC) -o $(DIR_OUTPUT)/$(NOMBRE_EJECUTABLE) -pthread
	@echo ' '
	@echo $@ compilado en el directorio $(DIR_OUTPUT)
	@echo "Ingrese al directorio '"$(DIR_OUTPUT)"' escribiendo: cd "$(DIR_OUTPUT)
	@echo "Para ejecutar escriba: ./"$(NOMBRE_EJECUTABLE)" 'PARAMETROS'"
	@echo ' '

clean:
	rm -rf $(DIR_OUTPUT)/*
	-@echo ' ' 
