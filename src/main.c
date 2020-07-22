#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAGIC 0x000A0D1A

typedef struct
{
    char header[3];
    u_int32_t magic;
    u_int8_t version;
    u_int32_t numFiles;
} stArchiveHeader __attribute__ ((aligned (8)));;


typedef struct _filedef
{
    u_int8_t name[16];
    u_int32_t offset;
    u_int32_t unkonwn;
    u_int32_t len;

} stFileDef;

typedef struct a
{
    stArchiveHeader hdr;
    stFileDef* files;
} stArchive;


stArchive* readArchive(FILE* f)
{
    int res;
    int i;
    stArchive* a = malloc(sizeof(stArchive));

    if(a == NULL)
    {
        fprintf(stderr,"Sin memoria\n");
        return NULL;
    }
    
    res=fread(a->hdr.header,sizeof(a->hdr.header),1,f);
    if(res<1 || ( strncmp(a->hdr.header,"stp",3) && strncmp(a->hdr.header,"dat",3)) )
    {
        fprintf(stderr,"No es un archivo de tipo 'stp': '%3.3s' \n",a->hdr.header);
        free(a);
        fclose(f);
        return NULL;
    }

    res=fread(&a->hdr.magic,sizeof(a->hdr.magic),1,f);
    if(res<1 || a->hdr.magic != MAGIC )
    {
        fprintf(stderr,"Magia incorrecta: %08X\n",a->hdr.magic);
        free(a);
        fclose(f);
        return NULL;
    }

    res=fread(&a->hdr.version,sizeof(a->hdr.version),1,f);
    if(res<1)
    {
        fprintf(stderr,"No se pudo leer la version");
        free(a);
        fclose(f);
        return NULL;
    }    

    res=fread(&a->hdr.numFiles,sizeof(a->hdr.numFiles),1,f);
    if(res<1)
    {
        fprintf(stderr,"No se pudo leer el numero de ficheros");
        free(a);
        fclose(f);
        return NULL;
    }
    
    a->files = malloc( a->hdr.numFiles * sizeof(stFileDef));
    
    res=fread(a->files,sizeof(stFileDef),a->hdr.numFiles,f);

    return a;
}

int extractChunk(FILE* src, stArchive* desc, unsigned pos)
{
    FILE* dst = fopen(desc->files[pos].name,"w");
    unsigned char* buff;

    if(!dst)
    {
        fprintf(stderr,"Error al escribir %s",desc->files[pos].name);
        return -1;
    }

    buff = malloc(desc->files[pos].len*sizeof(unsigned char));

    fseek(src,desc->files[pos].offset+8,SEEK_SET);
    fread( buff, desc->files[pos].len*sizeof(unsigned char), 1, src );
    fwrite( buff, desc->files[pos].len*sizeof(unsigned char), 1, dst );
    fclose(dst);
}

int main(int argc, char** argv)
{
    int i;
    int idx;
    stArchive* a;

    for(int i = 1; i< argc; i++)
    {
        FILE* f = fopen(argv[i],"r");

        if(f == NULL)
        {
            fprintf(stderr,"No se pudo abrir %s\n", argv[i]);
            continue;
        }

        a = readArchive(f);
        if(a != NULL)
        {
            printf("Archivo: %s\n",argv[i]);
            printf("Version: %u\n",(unsigned int)a->hdr.version);
            printf("Numero de ficheros encontrados: %u\n",(unsigned int)a->hdr.numFiles);

            for(idx=0; idx<a->hdr.numFiles; idx++)
            {
                printf("Fichero %d: %s. offset=%u len=%u,\n",(idx+1),a->files[idx].name,(unsigned)a->files[idx].offset,(unsigned)a->files[idx].len);
                extractChunk(f,a,idx);
            }
        }
        free(a);
    }
    return 0;
}