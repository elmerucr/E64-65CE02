// elmerucr - 2017-06-17
// convert a binary 32k rom file to ascii hex format, 16 bytes a row

#include <stdint.h>
#include <stdio.h>

int main() {
	uint8_t romdata[32768];
	FILE *f;
	f = fopen("E64-II_kernel.bin", "r");
	fread(romdata, 32768, 1, f);
	fclose(f);
	printf("// 32k E64-II_kernel elmerucr (c)2019\n");
	printf("//\n\n");
	printf("#include <cstdint>\n\n");
	printf("uint8_t kernel[32768] =\n{");

	for(int i=0; i<32767; i++) {
		if(i%16 == 0) printf("\n\t");
		printf("0x%02x,", romdata[i]);
	}
	printf("0x%02x", romdata[32767]);
	printf("\n};\n");
	return 0;
}
