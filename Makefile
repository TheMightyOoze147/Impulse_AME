# Создание директории build, если она не существует
CC = g++

CFLAGS = -std=c++20

all:
# Сборка объектных файлов и исполняемого файла
	mkdir -p build
	$(CC) $(CFLAGS) -c src/noise.cpp -o build/noise.o
	$(CC) $(CFLAGS) -c src/qam.cpp -o build/qam.o
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o
	g++ build/noise.o build/qam.o build/main.o -o task


# Очистка: удаление объектных файлов и исполняемого файла
clean:
	rm -f task
	rm -rf build
	rm -f result.txt

.PHONY: all clean
