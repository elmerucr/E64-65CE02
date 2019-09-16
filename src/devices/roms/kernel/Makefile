all: E64_kernel.bin bin2hex_8k kernel.cpp

bin2hex_8k: bin2hex_8k.c
	gcc -o bin2hex_8k bin2hex_8k.c

E64_kernel.bin: E64_kernel.asm
	64tass -b -C -L E64_kernel.lst -o E64_kernel.bin E64_kernel.asm

kernel.cpp: E64_kernel.bin
	./bin2hex_8k > kernel.cpp

clean:
	rm *.lst
	rm *.bin
	rm kernel.cpp
	rm bin2hex_8k