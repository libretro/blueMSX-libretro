/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/IoDevice/DirAsDisk.c,v $
**
** $Revision: 1.9 $
**
** $Date: 2006/06/18 07:55:10 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2004 Daniel Vik
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
******************************************************************************
*/
#define USE_ARCH_GLOB
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#ifdef USE_ARCH_GLOB
#include "ArchGlob.h"
#else
#include <windows.h>
#endif

#if defined(WIN32) || defined (WINDOWS_HOST)
#include <io.h>
#else
#ifndef O_BINARY
#define O_BINARY 0
#endif
#endif

static const unsigned char msxboot[] = { 
	0xeb,0xfe,0x90,0x44,0x53,0x4b,0x54,0x4f,
	0x4f,0x4c,0x20,0x00,0x02,0x02,0x01,0x00,
	0x02,0x70,0x00,0xa0,0x05,0xf9,0x03,0x00,
	0x09,0x00,0x02,0x00,0x00,0x00,0xd0,0xed,
	0x53,0x59,0xc0,0x32,0xc4,0xc0,0x36,0x56,
	0x23,0x36,0xc0,0x31,0x1f,0xf5,0x11,0x79,
	0xc0,0x0e,0x0f,0xcd,0x7d,0xf3,0x3c,0xca,
	0x63,0xc0,0x11,0x00,0x01,0x0e,0x1a,0xcd,
	0x7d,0xf3,0x21,0x01,0x00,0x22,0x87,0xc0,
	0x21,0x00,0x3f,0x11,0x79,0xc0,0x0e,0x27,
	0xcd,0x7d,0xf3,0xc3,0x00,0x01,0x58,0xc0,
	0xcd,0x00,0x00,0x79,0xe6,0xfe,0xfe,0x02,
	0xc2,0x6a,0xc0,0x3a,0xc4,0xc0,0xa7,0xca,
	0x22,0x40,0x11,0x9e,0xc0,0x0e,0x09,0xcd,
	0x7d,0xf3,0x0e,0x07,0xcd,0x7d,0xf3,0x18,
	0xb2,0x00,0x4d,0x53,0x58,0x44,0x4f,0x53,
	0x20,0x20,0x53,0x59,0x53,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x6f,
	0x6f,0x74,0x20,0x65,0x72,0x72,0x6f,0x72,
	0x0d,0x0a,0x50,0x72,0x65,0x73,0x73,0x20,
	0x61,0x6e,0x79,0x20,0x6b,0x65,0x79,0x20,
	0x66,0x6f,0x72,0x20,0x72,0x65,0x74,0x72,
	0x79,0x0d,0x0a,0x24,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


typedef unsigned char byte;
typedef unsigned short word;
typedef struct {
  char name[9];
  char ext[4];
  int size;
  int hour,min,sec;
  int day,month,year;
  int first;
  int pos;
  int attr;
} fileinfo;

byte *dskimage=NULL;
byte *fat;
byte *direc;
byte *cluster;
int sectorsperfat,numberoffats,reservedsectors;
int bytespersector,direlements,fatelements;
int availsectors;

void load_dsk (void) {
  dskimage=(byte *) calloc (1,720*1024);
  memset (dskimage,0,720*1024);
  memcpy (dskimage,msxboot,512);
  reservedsectors=*(word *)(dskimage+0x0E);
  numberoffats=*(dskimage+0x10);
  sectorsperfat=*(word *)(dskimage+0x16);
  bytespersector=*(word *)(dskimage+0x0B);
  direlements=*(word *)(dskimage+0x11);
  fat=dskimage+bytespersector*reservedsectors;
  direc=fat+bytespersector*(sectorsperfat*numberoffats);
  cluster=direc+direlements*32;
  availsectors=80*9*2-reservedsectors-sectorsperfat*numberoffats;
  availsectors-=direlements*32/bytespersector;
  fatelements=availsectors/2;
  fat[0]=0xF9;
  fat[1]=0xFF;
  fat[2]=0xFF;
}
fileinfo *getfileinfo (int pos) {
  fileinfo *file;  
  byte *dir;
  int i;

  dir=direc+pos*32;
  if (*dir<0x20 || *dir>=0x80) return NULL;

  file=(fileinfo *) malloc (sizeof (fileinfo));
  for (i=0; i<8; i++)
    file->name[i]=dir[i]==0x20?0:dir[i];
  file->name[8]=0;

  for (i=0; i<3; i++)
    file->ext[i]=dir[i+8]==0x20?0:dir[i+8];
  file->ext[3]=0;

  file->size=*(int *)(dir+0x1C);

  i=*(word *)(dir+0x16);
  file->sec=(i&0x1F)<<1;
  file->min=(i>>5)&0x3F;
  file->hour=i>>11;

  i=*(word *)(dir+0x18);
  file->day=i&0x1F;
  file->month=(i>>5)&0xF;
  file->year=1980+(i>>9);

  file->first=*(word *)(dir+0x1A);
  file->pos=pos;
  file->attr=*(dir+0xB);

  return file;
}

int getfilelength(int fd) {
    int cur = lseek(fd, 0, SEEK_CUR);
    int length = lseek(fd, 0, SEEK_END);
    lseek(fd, cur, SEEK_SET);
    return length;
}

int match (fileinfo *file, char *name) {
  char *p=file->name;
  int status=0,i;

  for (i=0; i<8; i++) {
    if (!*name)
      break;
    if (*name=='*') {
      status=1;
      name++;
      break;
    }
    if (*name=='.')
      break;
    if (toupper (*name++)!=toupper (*p++))
      return 0;
  }
  if (!status && i<8 && *p!=0) 
    return 0;
  p=file->ext;
  if (!*name && !*p) return 1;
  if (*name++!='.') return 0;
  for (i=0; i<3; i++) {
    if (*name=='*')
      return 1;
    if (toupper (*name++)!=toupper (*p++))
      return 0;
  }
  return 1;
}

int next_link (int link) {
  int pos;

  pos=(link>>1)*3;
  if (link&1)
    return (((int)(fat[pos+2]))<<4)+(fat[pos+1]>>4);
  else
    return (((int)(fat[pos+1]&0xF))<<8)+fat[pos];
}


int bytes_free (void) {
  int i,avail=0;

  for (i=2; i<2+fatelements; i++)
    if (!next_link (i)) avail++;
  return avail*1024;
}

int remove_link (int link) {
  int pos;
  int current;

  pos=(link>>1)*3;
  if (link&1) {
    current=(((int)(fat[pos+2]))<<4)+(fat[pos+1]>>4);
    fat[pos+2]=0;
    fat[pos+1]&=0xF;
    return current;
  }
  else  {
    current=(((int)(fat[pos+1]&0xF))<<8)+fat[pos];
    fat[pos]=0;
    fat[pos+1]&=0xF0;
    return current;
  }
}

void wipe (fileinfo *file) {
  int current;

  current=file->first;
  do {
    current=remove_link (current);
  } while (current!=0xFFF);
  direc[file->pos*32]=0xE5;
}

int get_free (void) {
  int i;

  for (i=2; i<2+fatelements; i++)
    if (!next_link (i)) return i;
  //printf ("Internal error\n");
  //exit (5);
  return 0;
}

int get_next_free (void) {
  int i,status=0;

  for (i=2; i<2+fatelements; i++)
    if (!next_link (i)) 
      if (status) 
        return i;
      else
        status=1;
  //printf ("Internal error\n");
  //exit (5);
  return 0;
}


void store_fat (int link, int next) {
  int pos;

  pos=(link>>1)*3;
  if (link&1) {
    fat[pos+2]=next>>4;
    fat[pos+1]&=0xF;
    fat[pos+1]|=(next&0xF)<<4;
  }
  else  {
    fat[pos]=next&0xFF;
    fat[pos+1]&=0xF0;
    fat[pos+1]|=next>>8;
  }
}

int add_single_file (char *name, char *pathname) {
  int i,total;
  fileinfo *file;
  int fileid;
  byte *buffer;
  byte *b;
  int size;
  struct stat s;
  struct tm *t;
  int first;
  int current;
  int next;
  int pos;
  char *p;
  char fullname[250];
  int result;

  strcpy (fullname,pathname);
  strcat (fullname,"/");
  strcat (fullname,name);
  fileid=open (fullname,O_BINARY|O_RDONLY);
  
  if (fileid < 0) {
      return -1;
  }

  for (i=0; i<direlements; i++) {
    if ((file=getfileinfo (i))!=NULL) {
      if (match (file,name)) {
        wipe (file);
      }
      free (file);
    }
  }

  if ((size=getfilelength(fileid))>bytes_free())
    return 1;

  for (i=0; i<direlements; i++)
    if (direc[i*32]<0x20 || direc[i*32]>=0x80)
      break;
  if (i==direlements)
    return 2;

  pos=i;

  b = buffer=(byte *) malloc ((size+1023)&(~1023));
  read (fileid,buffer,size);

  close (fileid);

  total=(size+1023)>>10;
  current=first=get_free ();

  for (i=0; i<total;) {
    memcpy (cluster+(current-2)*1024,buffer,1024);
    buffer+=1024;
    if (++i==total)
      next=0xFFF;
    else
      next=get_next_free ();
    store_fat (current,next);
    current=next;
  }

  memset (direc+pos*32,0,32);
  memset (direc+pos*32,0x20,11);
  i=0; 
  for (p=name;*p;p++) {
    if (*p=='.') {
      i=8;
      continue;
    }
    direc[pos*32+i++]=toupper (*p);
  }

  result = stat(fullname, &s);

  t = localtime(&s.st_mtime);
  if (t == NULL) {
      time_t tmp = time(NULL);
      t = localtime(&tmp);
  }
;
  if (t == NULL) {
      result = -1;
  }
  else {
    *(word *)(direc+pos*32+0x1A)=first;
    *(int *)(direc+pos*32+0x1C)=size;
    *(word *)(direc+pos*32+0x16)=
        (t->tm_sec>>1)+(t->tm_min<<5)+(t->tm_hour<<11);
    *(word *)(direc+pos*32+0x18)=
        (t->tm_mday)+(t->tm_mon<<5)+((t->tm_year-1980)<<9);
  }
  free (b);
  return result;
}


#ifdef USE_ARCH_GLOB
void* dirLoadFile(char* directory, int* size)
{
    ArchGlob* glob;
    static char filename[512];

    load_dsk();

    sprintf(filename, "%s/*", directory);

    glob = archGlob(filename, ARCH_GLOB_FILES);

    if (glob != NULL) {
        int rv;
        int i;
        for (i = 0; i < glob->count; i++) {
            char* fileName = strrchr(glob->pathVector[i], '/');
            if (fileName == NULL) {
                fileName = strrchr(glob->pathVector[i], '\\');
            }
            if (fileName == NULL) {
                continue;
            }
            fileName++;
            rv = add_single_file(fileName, directory);
            if (rv) {
                free(dskimage);
                dskimage = NULL;
                break;
            }
        }

        archGlobFree(glob);
    }

    *size = 720 * 1024;

    return dskimage;
}
#else
void* dirLoadFile(char* directory, int* size)
{
	WIN32_FIND_DATA fileData;
    HANDLE hFile;
    static char filename[512];
    int success;

   load_dsk();

    sprintf(filename, "%s" DIR_SEPARATOR "*.*", directory);

    hFile = FindFirstFile(filename,&fileData);
    success = hFile != INVALID_HANDLE_VALUE;
    while (success) {
        if (fileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
            int rv = add_single_file(fileData.cFileName, directory);
            if (rv) {
                free(dskimage);
                dskimage = NULL;
                break;
            }
        }
        success = FindNextFile(hFile, &fileData);
	}
    
    if (hFile != INVALID_HANDLE_VALUE) {
        FindClose(hFile);
    }

    *size = 720 * 1024;

    return dskimage;
}
#endif

