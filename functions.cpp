#include "zlib/unzip.h"
#include "sdl/include/sdl.h"

#pragma comment(lib, "zlib/zlibstat.lib")

//打开ZIP文件中的指定文件, 读到新分配的空间(返), 并取得文件实际大小(返)
//		1=成功  0=失败
int loadzipfile(char* zipfile,
					 char* filename,
					 char** destbuf,
					 int* filesize)
{
	int err;
	unzFile uf=NULL;
	unz_file_info finfo;

	uf = unzOpen(zipfile);
	if (uf==NULL)
		return 0;

	err=unzLocateFile(uf,filename,0);
	if (err!=UNZ_OK)
	{
		unzClose(uf);
		return 0;
	}

	unzGetCurrentFileInfo (uf, &finfo, NULL,0, NULL,0, NULL,0);
	*filesize=finfo.uncompressed_size;

	err=unzOpenCurrentFile(uf);
	if (err!=UNZ_OK)
	{
		unzClose(uf);
		return 0;
	}

	*destbuf=(char*)malloc(finfo.uncompressed_size);

	err=unzReadCurrentFile(uf,*destbuf,finfo.uncompressed_size);
	if (err!=(int)finfo.uncompressed_size)
	{
		free(*destbuf);
		*destbuf=NULL;
		return 0;
	}

	unzCloseCurrentFile(uf);

	unzClose(uf);

	return 1;
}

int loadfile(char* filename, char** destbuf, int* filesize)
{
	FILE* wf=fopen(filename, "rb");
	*destbuf=0;
	*filesize=0;
	if (!wf)
		return 0;
	fseek(wf, 0, SEEK_END);
	*filesize=ftell(wf);
	fseek(wf, 0, SEEK_SET);
	*destbuf=(char*)malloc(*filesize);
	if (!destbuf)
		return 0;
	fread(*destbuf, 1, *filesize, wf);
	fclose(wf);
	return 1;
}