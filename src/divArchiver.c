#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include "div.h"
#include "divArchiver.h"

// Carga los metadatos de un archivador 00X de div en memoria.
DIV_RESULT divArchiver_load(FILE *f, stDivArchiver *a)
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
DIV_RESULT divArchiver_extractChunk(FILE *src, stDivArchiver *desc, unsigned pos)
{
	FILE *dst = fopen(desc->files[pos].name, "wb");
	unsigned char *buff;
	unsigned long lenb;
	unsigned char *uncompressBfr;

	if (!dst)
	{
		fprintf(stderr, "Error al escribir %s", desc->files[pos].name);
		return DIV_ERR_WRITE;
	}

	buff = malloc(desc->files[pos].compressedLength * sizeof(unsigned char));
	if (buff == NULL)
		return DIV_ERR_MEM;

	fseek(src, desc->files[pos].offset + DIV_FILE_HDR_LEN, SEEK_SET);
    fread(buff, desc->files[pos].compressedLength, sizeof(unsigned char), src);

	if( desc->files[pos].compressedLength == desc->files[pos].length )
	{
		fwrite(buff, desc->files[pos].compressedLength, sizeof(unsigned char), dst);
	}
	else
	{
		uncompressBfr = malloc(desc->files[pos].length+1 * sizeof(unsigned char));
		if (uncompressBfr == NULL)
		{
			fclose(dst);
			free(buff);
	        return DIV_ERR_MEM;		
		}
			
		lenb=desc->files[pos].length;
		printf("%ul,%ul > %ul\n",lenb,desc->files[pos].length, desc->files[pos].compressedLength);
		int res = uncompress(uncompressBfr, &lenb, buff, (unsigned long) desc->files[pos].compressedLength );
		if( res != Z_OK)
		{
			printf("Error al descomprimir...%d,%d,%d,%d\n",res,res==Z_BUF_ERROR,res==Z_MEM_ERROR,res==Z_DATA_ERROR);
			fclose(dst);
			free(uncompressBfr);
            free(buff);
            return DIV_ERR_OTHER;
		}
		printf("%ul,%ul\n",lenb,desc->files[pos].length);
		
		res = fwrite(uncompressBfr, sizeof(unsigned char), desc->files[pos].length, dst);
		printf("Descomprimido: %ul\n",res);
		free(uncompressBfr);
	}

	fclose(dst);
	free(buff);
	return DIV_OK;
}
