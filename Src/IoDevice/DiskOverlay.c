/*
 * DiskOverlay.c - Disk overlay system for persistent disk writes
 *
 * This module provides an in-memory overlay for disk sectors, allowing
 * persistent writes to be stored in a separate file without modifying the
 * original disk image.
 */

#include "DiskOverlay.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DiskOverlay_t* diskOverlayCreate(size_t sectorSize)
{
   DiskOverlay_t* ov;
   ov = (DiskOverlay_t*)calloc(1, sizeof(DiskOverlay_t));
   if (ov)
      ov->sectorSize = sectorSize;
   return ov;
}

void diskOverlayFree(DiskOverlay_t* overlay)
{
   DiskOverlayEntry_t* e;
   DiskOverlayEntry_t* next;

   if (!overlay)
      return;

   e = overlay->head;

   while (e)
   {
      next = e->next;
      free(e->data);
      free(e);
      e = next;
   }
   free(overlay);
}

void diskOverlayWrite(DiskOverlay_t* overlay, int sector, const uint8_t* data)
{
   DiskOverlayEntry_t* prev;
   DiskOverlayEntry_t* e;
   if (!overlay)
      return;

   prev = NULL;
   e = overlay->head;
   while (e)
   {
      if (e->sector == sector)
      {
         memcpy(e->data, data, overlay->sectorSize);
         /* Optionally move to head for LRU, but not required */
         return;
      }
      prev = e;
      e = e->next;
   }

   e = (DiskOverlayEntry_t*)malloc(sizeof(DiskOverlayEntry_t));
   if (!e)
      return;
   e->sector = sector;
   e->data = (uint8_t*)malloc(overlay->sectorSize);
   if (!e->data)
   {
      free(e);
      return;
   }
   memcpy(e->data, data, overlay->sectorSize);
   e->next = overlay->head;
   overlay->head = e;
}

int diskOverlayRead(DiskOverlay_t* overlay, int sector, uint8_t* out)
{
   DiskOverlayEntry_t* e;
   if (!overlay)
      return 0;

   e = overlay->head;
   while (e)
   {
      if (e->sector == sector)
      {
         memcpy(out, e->data, overlay->sectorSize);
         return 1;
      }
      e = e->next;
   }

   return 0;
}

int diskOverlayIsEmpty(DiskOverlay_t* overlay)
{
   return (overlay == NULL || overlay->head == NULL);
}

int diskOverlaySerialize(DiskOverlay_t* overlay, const char* path)
{
   FILE* f;
   DiskOverlayEntry_t* e;

   if (!overlay || !path)
      return 0;

   f = fopen(path, "wb");
   if (!f)
      return 0;

   e = overlay->head;
   while (e)
   {
      fwrite(&e->sector, sizeof(int), 1, f);
      fwrite(e->data, 1, overlay->sectorSize, f);
      e = e->next;
   }

   fclose(f);
   return 1;
}

DiskOverlay_t* diskOverlayDeserialize(const char* path, size_t sectorSize)
{
   FILE* f;
   DiskOverlay_t* ov;
   int sector;
   uint8_t* buf;
   size_t n;
   DiskOverlayEntry_t* prev;
   DiskOverlayEntry_t* e;

   if (!path)
      return NULL;

   f = fopen(path, "rb");
   if (!f)
      return NULL;

   ov = diskOverlayCreate(sectorSize);
   if (!ov)
   {
      fclose(f);
      return NULL;
   }

   buf = (uint8_t*)malloc(sectorSize);
   if (!buf)
   {
      fclose(f);
      diskOverlayFree(ov);
      return NULL;
   }

   while (fread(&sector, sizeof(int), 1, f) == 1)
   {
      n = fread(buf, 1, sectorSize, f);
      if (n != sectorSize)
         break;
      /* Remove any previous entry for this sector before writing */
      prev = NULL;
      e = ov->head;
      while (e)
      {
         if (e->sector == sector)
         {
            if (prev)
               prev->next = e->next;
            else
               ov->head = e->next;
            free(e->data);
            free(e);
            break;
         }
         prev = e;
         e = e->next;
      }
      diskOverlayWrite(ov, sector, buf);
   }

   free(buf);
   fclose(f);
   return ov;
}