#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "div.h"
#include "divArchiver.h"

// Carga los metadatos de un archivador 00X de div en memoria.
int divArchiver_load(FILE *f, stDivArchiver *a)
{
	int res;
	int i;

	if (a == NULL || f == NULL)
		return DIV_ERR_LOAD;

	memset(a, 0x00, sizeof(stDivArchiver));

	res = div_loadHeader(f, &a->hdr);
	if (res < 0)
		return res;

	if (strncmp(a->hdr.type, "stp", 3))
		return DIV_ERR_FTYPE;

	res = fread(&a->numFiles, sizeof(a->numFiles), 1, f);
	if (res < 1)
		return DIV_ERR_LOAD;

	a->files = malloc(a->numFiles * sizeof(stDivArchiverFileDef));
	if (a->files == NULL)
		return DIV_ERR_MEM;

	res = fread(a->files, sizeof(stDivArchiverFileDef), a->numFiles, f);
	if (res < a->numFiles)
		return DIV_ERR_LOAD;

	return 0;
}

// Libera los recursos reservados por la estructura divArchiver que le pasen
void divArchiver_unload(stDivArchiver *a)
{
	if (a->files != NULL)
		free(a->files);
}

// Extrae un fichero de un archivador en la ruta actual
int divArchiver_extractChunk(FILE *src, stDivArchiver *desc, unsigned pos)
{
	FILE *dst = fopen(desc->files[pos].name, "w");
	unsigned char *buff;

	if (!dst)
	{
		fprintf(stderr, "Error al escribir %s", desc->files[pos].name);
		return -1;
	}

	buff = malloc(desc->files[pos].len * sizeof(unsigned char));
	if (buff == NULL)
		return DIV_ERR_MEM;

	fseek(src, desc->files[pos].offset + DIV_FILE_HDR_LEN, SEEK_SET);
	fread(buff, desc->files[pos].len * sizeof(unsigned char), 1, src);
	fwrite(buff, desc->files[pos].len * sizeof(unsigned char), 1, dst);
	fclose(dst);

	free(buff);
}
