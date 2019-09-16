// Elmer van Baal - 17 juni 2017
// convert a binary 8k rom file to ascii hex format, 16 bytes a row

#include <stdint.h>
#include <stdio.h>

int main() {
	uint8_t romdata[8192];

	FILE *f;
	f = fopen("E64_kernel.bin", "r");
	fread(romdata, 8192, 1, f);
	fclose(f);

	printf("// kernel.cpp\n");
	printf("// elmerucr - copyright 2019\n\n");
	printf("#include <cstdint>\n\n");
	printf("uint8_t kernel[8192] =\n{");
	for(int i=0; i<8191; i++) {
		if(i%16 == 0) printf("\n\t");
		printf("0x%02x,", romdata[i]);
	}
	printf("0x%02x", romdata[8191]);
	printf("\n};\n");

	return 0;
}