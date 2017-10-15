#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct yugi_file {
    char name[256];
    int offset;
    int size_uncomp;
    int size_comp;
} yfile_t;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	puts("Usage: extractor <dat_file.dat>");
	return 0;
    }

    char *fn = argv[1];
    FILE *fp = fopen(fn, "rb");

    if (fp == NULL)
    {
	printf("Couldn't open file %s\n", fn);
	return 0;
    }

    char id_buffer[9];
    
    if (fgets(id_buffer, 9, fp) == NULL || strcmp(id_buffer, "KCEJYUGI") != 0)
    {
	puts("The .dat file is not for Yu-Gi-Oh PoC");
	fclose(fp);
	return 0;
    }

    int file_count;
    
    if (fread(&file_count, 4, 1, fp) != 1)
    {
    	puts("Couldn't read file count");
    	fclose(fp);
    	return 0;
    }

    //yfile_t *yfiles = (yfile_t *)malloc(sizeof(yfile_t) * file_count);
    yfile_t yf;
    long current_pos;
    FILE *current_file;
    unsigned char *current_data;
    
    for (int i = 0; i < file_count; i++)
    {
	fread(yf.name, 1, 256, fp);
	fread(&yf.offset, 4, 1, fp);
	fread(&yf.size_uncomp, 4, 1, fp);
	fread(&yf.size_comp, 4, 1, fp);

	for (int j = 0; j < 256; j++)
	{
	    unsigned char c = yf.name[j];
	    yf.name[j] = c << 4 | c >> 4;
	}

	current_pos = ftell(fp);
	fseek(fp, yf.offset, SEEK_SET);
	
	current_data = (unsigned char *)malloc(yf.size_comp);
	fread(current_data, 1, yf.size_comp, fp);

	current_file = fopen(yf.name, "wb");
	fwrite(current_data, 1, yf.size_comp, current_file);
	
	fclose(current_file);
	free(current_data);
	fseek(fp, current_pos, SEEK_SET);

	//yfiles[i] = yf;
    }

    //free(yfiles);
    fclose(fp);
    return 0;
}
