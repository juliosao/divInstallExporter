#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "div.h"
#include "divArchiver.h"

int main(int argc, char** argv)
{
    int i;
    int idx;
    stDivArchiver a;

    for(int i = 1; i< argc; i++)
    {
        FILE* f = fopen(argv[i],"r");

        if(f == NULL)
        {
            fprintf(stderr,"No se pudo abrir %s\n", argv[i]);
            continue;
        }

		switch (divArchiver_load(f,&a))
		{
		case DIV_ERR_LOAD:
			fprintf(stderr,"Error de carga");
			break;
		case DIV_ERR_MAGIC:
			fprintf(stderr,"Tipo de fichero erroneo o fichero corrupto");
			break;
		case DIV_ERR_FTYPE:
			fprintf(stderr,"Tipo de fichero erroneo");
			break;
		default:
			printf("Archivo: %s\n",argv[i]);
			printf("Tipo: %3.3s\n",a.hdr.type);
			printf("Magia: %8.8X\n",a.hdr.magic);
            printf("Version: %u\n",(unsigned int)a.hdr.version);
            printf("Numero de ficheros encontrados: %u\n",(unsigned int)a.numFiles);
			
            for(idx=0; idx<a.numFiles; idx++)
            {
                printf("Fichero %d: %s. offset=%u len=%u,\n",(idx+1),a.files[idx].name,(unsigned)a.files[idx].offset,(unsigned)a.files[idx].len);
                divArchiver_extractChunk(f,&a,idx);
            }
				
		}
		divArchiver_unload(&a);
    }
    return 0;
}
