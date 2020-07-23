#include <stdint.h>
#include <stdio.h>
#include "div.h"

int div_loadHeader(FILE* src, stDivArchiveHeader* hdr)
{
	if(fread(hdr->type,sizeof(hdr->type),1,src)<1)
		return DIV_ERR_LOAD;

	if(fread(&hdr->magic,sizeof(hdr->magic),1,src)<1)
		return DIV_ERR_LOAD;

	if(fread(&hdr->version,sizeof(hdr->version),1,src)<1)
		return DIV_ERR_LOAD;

	if(hdr->magic != DIV_FILE_MAGIC)
		return DIV_ERR_MAGIC;

	return 0;
}