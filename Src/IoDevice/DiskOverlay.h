#ifndef DISKOVERLAY_H
#define DISKOVERLAY_H

#include <stddef.h>
#include <stdint.h>

/**
 * DiskOverlayEntry_t - Linked list entry for a disk overlay sector
 */
typedef struct DiskOverlayEntry_t
{
   int sector;
   uint8_t* data;
   struct DiskOverlayEntry_t* next;
} DiskOverlayEntry_t;

/**
 * DiskOverlay_t - Overlay structure for disk sectors
 */
typedef struct DiskOverlay_t
{
   DiskOverlayEntry_t* head;
   size_t sectorSize;
} DiskOverlay_t;

DiskOverlay_t* diskOverlayCreate(size_t sectorSize);
void diskOverlayFree(DiskOverlay_t* overlay);
void diskOverlayWrite(DiskOverlay_t* overlay, int sector, const uint8_t* data);
int diskOverlayRead(DiskOverlay_t* overlay, int sector, uint8_t* out);
int diskOverlaySerialize(DiskOverlay_t* overlay, const char* path);
DiskOverlay_t* diskOverlayDeserialize(const char* path, size_t sectorSize);
int diskOverlayIsEmpty(DiskOverlay_t* overlay);

#endif /* DISKOVERLAY_H */