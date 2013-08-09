#ifndef _CTmap_writter_
#define _CTmap_writter_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct mapWritter{
    FILE *file;
    unsigned int size;
    unsigned int *LNpos;
    const char *path;
    unsigned int current_line;
}mapWritter;
const struct mapWritter MAPWRITTER = {
    .file = NULL,
    .size = 0,
    .LNpos = 0,
    .path = "\0",
    .current_line = 0
};
const int debug = 1;
int openMW(struct mapWritter *mw,const char* fileName)
{
    if(debug) printf("openMW(struct mapWritter *mw,const char* fileName)\n");
    if(debug) printf("mw->size = %u\n",mw->size);
    if(mw->size != 0) closeMap(mw);
    mw->file = fopen(fileName,"r+");
    mw->path = (char*) malloc(strlen(fileName) * sizeof(char));
    strcpy(mw->path,fileName);
    if(debug) printf("mw->path = %s\n",mw->path);
    if(mw->file != NULL)
    {
        if(debug) printf("file open\n");
        char x = getc(mw->file);
        if(x == EOF)
        {
            if(debug) printf("file empty\n");
            mw->size = 0;
            mw->LNpos = (int*) malloc(sizeof(int));
        }
        else
        {
            if(debug) printf("file has content\n");
            ungetc(x,mw->file);
            fscanf(mw->file,"%u",&mw->size);
            mw->LNpos = (int*) malloc(mw->size * sizeof(int));
        }
    }
    else mw->size = 0;
    mw->current_line = 0;
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    return mw->file != NULL;
}
int writteMap(struct mapWritter *mw)//this allows to append text
{
    if(debug) printf("writteMap(struct mapWritter *mw)\n");
    if(mw->file == NULL)//could not open
    {
        if(debug) printf("could not open file\n");
        freopen(mw->path,"w+",mw->file);
        if(debug && (mw->file == NULL)) printf("Could not create file\n");
        else if(debug) printf("New empty file created\n");
        if(mw->file == NULL) return 0;//Could not create/open file
        else return 1;//new empty
    }
    if(debug && mw->size == 0) printf("File is empty\n");
    if(mw->size == 0) return 1;//is empty
    rewind(mw->file);
    char x = ' ';
    long int len = 0,LNlen = 0;
    mw->LNpos[0] = 0;
    while(x != EOF)
    {
        x = fgetc(mw->file);
        if(x == '\n')
        {
            LNlen++;
            mw->LNpos[LNlen] = ftell(mw->file);
        }
        len++;
    }
    if(debug) printf("Number of lines: %l\n",LNlen);
    if(debug) printf("Number of characters: %l\n",len);
    if(debug) printf("File position: %u\n",mw->LNpos[LNlen]);
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    return -1;//has data in it
}
int _MW_gotoLine(struct mapWritter *mw,int line)
{
    if(debug) printf("_MW_gotoLine(struct mapWritter *mw,int line)\n");
    if(debug) printf("goToLine(%i)\n",line);
    if(debug) printf("mw->size = %u\n",mw->size);
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    return line > mw->size ? 0 : fseek(mw->file,mw->LNpos[line],SEEK_SET);
}
int changeImage(struct mapWritter *mw,const char* str,int line)
{
    if(debug) printf("changeImage(struct mapWritter *mw,const char* str,int line)\n");
    _MW_gotoLine(mw,line);
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    return fprintf(mw->file,"%s\n",str);
}
int addImage(struct mapWritter *mw,const char* str)//use writteMap first in order to append,otherwise it will overwritte
{
    if(debug) printf("addImage(struct mapWritter *mw,const char* str)\n");
    int *buff;
    _MW_gotoLine(mw,mw->size);
    fprintf(mw->file,"%s\n",str);
    mw->size++;
    if(debug) printf("size incremented to %u\n",mw->size);
    mw->current_line++;
    if(debug) printf("current line now is %u\n",mw->current_line);
    buff = (int*) realloc(mw->LNpos,mw->current_line * sizeof(int));
    if(buff == NULL && debug)printf("Error allocating memory");
    if(debug) printf("mw->LNpos copy increased memory slot\n");
    mw->LNpos = buff;
    if(debug) printf("mw->LNpos now points to increased copy\n");
    mw->LNpos[mw->current_line] = ftell(mw->file);
    if(debug) printf("new line added to readMark\n");
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    return 1;
}
int cleanMap(struct mapWritter *mw)
{
    if(debug) printf("cleanMap(struct mapWritter *mw)\n");
    mw->size = 1;
    mw->current_line = 1;
    fclose(mw->file);
    if(debug) printf("map closed\n");
    mw->file = fopen(mw->path,"w");
    free(mw->LNpos);
    if(debug) printf("free mw->LNpos\n");
    mw->LNpos = (int*) malloc(2 * sizeof(int));
    mw->LNpos[0] = ftell(mw->file);
    if(debug) printf("free mw->LNpos[0] = %u\n",mw->LNpos[0]);
    fprintf(mw->file,"%u\n",mw->size);
    mw->LNpos[1] = ftell(mw->file);
    if(debug) printf("free mw->LNpos[1] = %u\n",mw->LNpos[1]);
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    return mw->file != NULL;
}
int closeMap(struct mapWritter *mw)
{
    if(debug) printf("closeMap(struct mapWritter *mw)\n");
    rewind(mw->file);
    if(debug) printf("file rewinded\n");
    fprintf(mw->file,"%u\n",mw->size);
    if(debug) printf("size saved: %u\n",mw->size);
    fclose(mw->file);
    if(debug) printf("file closed\n");
    free(mw->LNpos);
    if(debug) printf("free mw->LNpos\n");
    free(mw->path);
    if(debug) printf("free mw->path\n");
    if(debug) printf("////////////////////////////////////////////////////////////\n");
    mw->size = 0;
}
#endif // _CTmap_writter_
