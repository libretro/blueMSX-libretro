/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Cdrom.c,v $
**
** $Revision: 1.5 $
**
** $Date: 2008-03-30 18:38:48 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2007 Daniel Vik, white cat
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#include "Win32Cdrom.h"
#include "ArchCdrom.h"
#include "ScsiDefs.h"
#include "Properties.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//#define CDROMDEBUG "cdromLog.txt"
#define USE_VIRTUAL_ALLOC
#define MAX_ASPIDRV 32

#ifdef CDROMDEBUG
static FILE* logFd;
#define DBGLOG(fmt) fprintf(logFd, fmt)
#define DBGLOG1(fmt, arg1) fprintf(logFd, fmt, arg1)
#define DBGLOG2(fmt, arg1, arg2) fprintf(logFd, fmt, arg1, arg2)
#define DBGLOG3(fmt, arg1, arg2, arg3) fprintf(logFd, fmt, arg1, arg2, arg3)
#else
#define DBGLOG(fmt)
#define DBGLOG1(fmt, arg1)
#define DBGLOG2(fmt, arg1, arg2)
#define DBGLOG3(fmt, arg1, arg2, arg3)
#endif

#define SCSI_IOCTL_DATA_OUT         0
#define SCSI_IOCTL_DATA_IN          1
#define SCSI_IOCTL_DATA_UNSPECIFIED 2

#define IOCTL_SCSI_PASS_THROUGH_DIRECT  0x04d014
#define IOCTL_CDROM_STOP_AUDIO          0x024008
#define IOCTL_CDROM_READ_TOC_EX         0x024054
#define CDROM_READ_TOC_EX_FORMAT_TOC    0x00

struct ArchCdrom
{
    int    keycode;
    int    cdromId;
    int    busy;
    unsigned int diskChange;
    CdromXferCompCb  xferCompCb;
    void*  ref;
};

typedef struct {
    USHORT Length;
    UCHAR  ScsiStatus;
    UCHAR  PathId;
    UCHAR  TargetId;
    UCHAR  Lun;
    UCHAR  CdbLength;
    UCHAR  SenseInfoLength;
    UCHAR  DataIn;
    ULONG  DataTransferLength;
    ULONG  TimeOutValue;
    PVOID  DataBuffer;
    ULONG  SenseInfoOffset;
    UCHAR  Cdb[16];
    ULONG  Filler;
    UCHAR  ucSenseBuf[32];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

typedef struct {
    UCHAR  Format : 4;
    UCHAR  Reserved1 : 3; 
    UCHAR  Msf : 1;
    UCHAR  SessionTrack;
    UCHAR  Reserved2;
    UCHAR  Reserved3;
} CDROM_READ_TOC_EX;

typedef struct {
    UCHAR  Reserved;
    UCHAR  Control : 4;
    UCHAR  Adr : 4;
    UCHAR  TrackNumber;
    UCHAR  Reserved1;
    UCHAR  Address[4];
} TRACK_DATA;

typedef struct {
    UCHAR  Length[2];
    UCHAR  FirstTrack;
    UCHAR  LastTrack;
    TRACK_DATA TrackData[100];
} CDROM_TOC;

// winaspi
#pragma pack(1)

#define SENSE_LEN           18
#define SRB_DIR_IN          0x08
#define SRB_DIR_OUT         0x10
#define SRB_EVENT_NOTIFY    0x40

#define SS_PENDING          0x00
#define SS_COMP             0x01

#define SC_HA_INQUIRY       0x00
#define SC_GET_DEV_TYPE     0x01
#define SC_EXEC_SCSI_CMD    0x02

#define HASTAT_OK           0x00
#define HASTAT_DO_DU        0x12

typedef struct
{
    BYTE    SRB_Cmd;
    BYTE    SRB_Status;
    BYTE    SRB_HaId;
    BYTE    SRB_Flags;
    DWORD   SRB_Hdr_Rsvd;
    BYTE    HA_Count;
    BYTE    HA_SCSI_ID;
    BYTE    HA_ManagerId[16];
    BYTE    HA_Identifier[16];
    WORD    HA_BufAlignMask;
    BYTE    HA_Flags;
    BYTE    HA_MaxTargets;
    DWORD   HA_MaxTransferLength;
    DWORD   HA_MaxSGElements;
    BYTE    HA_Rsvd2[4];
    WORD    HA_Rsvd1;
} SRB_HAInquiry;

typedef struct
{
    BYTE    SRB_Cmd;
    BYTE    SRB_Status;
    BYTE    SRB_HaId;
    BYTE    SRB_Flags;
    DWORD   SRB_Hdr_Rsvd;
    BYTE    SRB_Target;
    BYTE    SRB_Lun;
    BYTE    SRB_DeviceType;
    BYTE    SRB_Rsvd1;
} SRB_GDEVBlock;

typedef struct
{
    BYTE    SRB_Cmd;
    BYTE    SRB_Status;
    BYTE    SRB_HaId;
    BYTE    SRB_Flags;
    DWORD   SRB_Hdr_Rsvd;
    BYTE    SRB_Target;
    BYTE    SRB_Lun;
    WORD    SRB_Rsvd1;
    DWORD   SRB_BufLen;
    BYTE    *SRB_BufPointer;
    BYTE    SRB_SenseLen;
    BYTE    SRB_CDBLen;
    BYTE    SRB_HaStat;
    BYTE    SRB_TargStat;
    VOID    *SRB_PostProc;
    BYTE    SRB_Rsvd2[20];
    BYTE    CDBByte[16];
    BYTE    SenseArea[SENSE_LEN+2];
} SRB_ExecSCSICmd;

typedef void *LPSRB;

typedef DWORD (*fGetASPI32SupportInfo)(void);
typedef DWORD (*fSendASPI32Command)(LPSRB);

#pragma pack()

typedef struct {
    LPVOID param;
    ArchCdrom* cdrom;
} EXECINFO;

static int driveMask      = 0;
static int driveCnt       = 0;
static int changeCnt      = 0;
static int isXferComp     = 1;
static int isBusy         = 0;
static int execId         = 0;
static int nBytesRead     = 0;
static int tocFlag        = 0;
static int cdPlayed       = 0;
static int cdMethod       = -1;
static HANDLE hExecThread = NULL;
static char*      tocbuf  = NULL;
static HANDLE hEvent      = INVALID_HANDLE_VALUE;
static HANDLE hDevice     = INVALID_HANDLE_VALUE;

static CDROM_TOC* toc;
static HMODULE hModule;
static int aspiTbl[MAX_ASPIDRV + 1];
static int aspiId;

static fGetASPI32SupportInfo GetASPI32SupportInfo;
static fSendASPI32Command SendASPI32Command;

#ifdef CDROMDEBUG
static void winapiError()
{
    UCHAR errorBuffer[80];
    ULONG count;

    count = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0,
                          errorBuffer, sizeof(errorBuffer), NULL);

    if (count != 0) {
        DBGLOG1("%s\n", errorBuffer);
    } else {
        DBGLOG1("Format message failed.  Error: %d\n", (int)GetLastError());
    }
}
#else
#define winapiError()
#endif

static int cdromAudioStop(); // proto type

void archCdromLoadState(ArchCdrom* cdrom) {}
void archCdromSaveState(ArchCdrom* cdrom) {}

/*
    Memory management for IOCTL and SCSI Pass Through Interface
    2048 bytes boundary alignment
*/
void* archCdromBufferMalloc(size_t size)
{
#ifdef USE_VIRTUAL_ALLOC
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    char* ptr = calloc(1, size + 2048 + 0x10);
    char* newPtr;
    unsigned int* p;

    if (ptr == NULL) {
        return NULL;
    }
    newPtr = (char*)((unsigned int)(ptr + 2048 + 0x10) & ~2047);
    p  = (unsigned int*)(newPtr - 0x10);
    *p = (unsigned int)ptr;
    return newPtr;
#endif
}

void archCdromBufferFree(void* ptr)
{
#ifdef USE_VIRTUAL_ALLOC
    VirtualFree(ptr, 0, MEM_RELEASE);
#else
    char* nptr = ptr;
    unsigned int* p;

    if (nptr) {
        nptr = nptr - 0x10;
        p = (unsigned int*)nptr;
        free((void*)*p);
    }
#endif
}

static void threadClose()
{
    if (hExecThread) {
        WaitForSingleObject(hExecThread, INFINITE);
        CloseHandle(hExecThread);
        execId = 0;
        hExecThread = NULL;
        DBGLOG("thread close\n");
        isBusy         = 0;
        isXferComp     = 1;
        //bThreadInitialized = FALSE;
    }
}

ArchCdrom* archCdromCreate(CdromXferCompCb xferCompCb, void* ref)
{
    ArchCdrom*  cdrom;
    DiskdriveProperties* prop;
    char        str[16];
    OSVERSIONINFO osInfo;
    int* p;

    if (driveCnt++ == 0) {
        cdMethod = P_CDROM_DRVNONE;
        prop = &propGetGlobalProperties()->diskdrive;

        if (prop->cdromMethod == P_CDROM_DRVIOCTL) {
            osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
            if (GetVersionEx(&osInfo) && osInfo.dwMajorVersion >= 5) {
                sprintf(str, "%c:\\", prop->cdromDrive);
                if (GetDriveType(str) == DRIVE_CDROM) {
                    sprintf(str, "\\\\.\\%c:", prop->cdromDrive);
                    DBGLOG1("cdrom open: %s\n", str);
                    hDevice = CreateFile(str, GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    winapiError();
                    if (hDevice != INVALID_HANDLE_VALUE) {
                        cdMethod = P_CDROM_DRVIOCTL;
                        driveMask = 1 << (prop->cdromDrive - 'A');
                    }
                }
            }
        }
        else if (prop->cdromMethod == P_CDROM_DRVASPI) {
            if (hModule) {
                p = aspiTbl;
                while (*p) {
                    if (prop->cdromDrive == *p) {
                        cdMethod  = P_CDROM_DRVASPI;
                        // FIXME: ASPI drive is related to a physical drive
                        driveMask = ~0;
                        aspiId = *p;
                        break;
                    }
                    p++;
                }
            }
        }
    }

    if (cdMethod == P_CDROM_DRVNONE) {
        return NULL;
    }

    cdrom = calloc(1, sizeof(ArchCdrom));

    if (driveCnt == 1) {
        tocbuf = malloc(sizeof(CDROM_TOC) + 0x10);
        toc    = (CDROM_TOC*)((unsigned int)(tocbuf + 0x10) & ~0x0f);
    }

    cdrom->diskChange = ~0;
    cdrom->cdromId    = driveCnt;
    cdrom->xferCompCb = xferCompCb;
    cdrom->ref        = ref;

    DBGLOG("open ok\n");
    return cdrom;
}

void archCdromDestroy(ArchCdrom* cdrom)
{
    driveCnt--;
    if (cdrom) {
        if (execId == cdrom->cdromId) {
            threadClose();
        }
        if (driveCnt == 0) {
            threadClose();

            if (cdPlayed) {
#ifdef CDROMDEBUG
                DWORD counter = GetTickCount();
                cdromAudioStop();
                counter = GetTickCount() - counter;
                DBGLOG1("Time until stopping cd %dms\n", (int)counter);
#else
                cdromAudioStop();
#endif
            }

            if (hDevice != INVALID_HANDLE_VALUE) {
                CloseHandle(hDevice);
                hDevice = INVALID_HANDLE_VALUE;
                winapiError();
            }

            free(tocbuf);
            tocbuf    = NULL;
            tocFlag   = 0;
            driveMask = 0;
            cdMethod  = -1;
            cdPlayed  = 0;
        }
        free(cdrom);
        DBGLOG("cdrom destroy\n");
    }

#ifdef CDROMDEBUG
    fflush(logFd);
#endif
}

void archCdromHwReset(ArchCdrom* cdrom)
{
    if (cdrom) {
        DBGLOG("hw reset\n");
        threadClose();
        cdrom->keycode = SENSE_POWER_ON;
        cdrom->busy    = 0;
        cdrom->diskChange = ~0;
    }
}

void archCdromBusReset(ArchCdrom* cdrom)
{
    if (cdrom) {
        cdrom->keycode = SENSE_POWER_ON;
        cdrom->busy    = 0;
        cdrom->diskChange = ~0;
        if (execId == cdrom->cdromId) {
            threadClose();
        }
    }
}

void archCdromDisconnect(ArchCdrom* cdrom)
{
    if (cdrom) {
        threadClose();
        cdrom->busy = 0;
        if (execId == cdrom->cdromId) {
            threadClose();
        }
    }
}

static void execAspiCmd(int id, SRB_ExecSCSICmd* cmd)
{
    DWORD status;

    ResetEvent(hEvent);

    cmd->SRB_HaId     = (BYTE)((id >> 16) & 0xff);
    cmd->SRB_Target   = (BYTE)((id >>  8) & 0xff);
    cmd->SRB_Lun      = (BYTE)(id & 0xff);
    cmd->SRB_Cmd      = SC_EXEC_SCSI_CMD;
    cmd->SRB_Flags    = SRB_DIR_IN | SRB_EVENT_NOTIFY;// | SRB_ENABLE_RESIDUAL_COUNT;
    cmd->SRB_PostProc = (LPVOID)hEvent;
    cmd->SRB_SenseLen = SENSE_LEN;
    status = SendASPI32Command((LPSRB)cmd);

    if (status == SS_PENDING) {
        WaitForSingleObject(hEvent, INFINITE);
    }
}

static int cdromAudioStop()
{
    DWORD returned;
    SRB_ExecSCSICmd cmd;

    if (cdMethod == P_CDROM_DRVIOCTL) {
        return DeviceIoControl(hDevice, IOCTL_CDROM_STOP_AUDIO,
                               NULL, 0, NULL, 0, &returned, NULL);
    }
    else {
        memset(&cmd, 0, sizeof(cmd));
        cmd.SRB_BufPointer = NULL;
        cmd.SRB_BufLen = 0;
        cmd.SRB_CDBLen = 6;
        cmd.CDBByte[0] = SCSIOP_START_STOP_UNIT;
        execAspiCmd(aspiId, &cmd);
        DBGLOG2("aspi cd stop %d %d\n", cmd.SRB_HaStat, cmd.SRB_TargStat);
        return (cmd.SRB_HaStat == HASTAT_OK || cmd.SRB_HaStat == HASTAT_DO_DU);
    }
}

static int cdromReadToc()
{
    DWORD returned;
    SRB_ExecSCSICmd cmd;
    CDROM_READ_TOC_EX  tocex = { CDROM_READ_TOC_EX_FORMAT_TOC, 0, 0, 1, 0, 0};

    if (cdMethod == P_CDROM_DRVIOCTL) {
        return DeviceIoControl(hDevice, IOCTL_CDROM_READ_TOC_EX,
                                 &tocex, sizeof(CDROM_READ_TOC_EX),
                                 toc, sizeof(CDROM_TOC),
                                 &returned, NULL);
    }
    else {
        memset(&cmd, 0, sizeof(cmd));
        cmd.SRB_BufPointer = (BYTE*)toc;
        cmd.SRB_BufLen = sizeof(CDROM_TOC);
        cmd.SRB_CDBLen = 10;
        cmd.CDBByte[0] = SCSIOP_READ_TOC;
        cmd.CDBByte[6] = 1;
        cmd.CDBByte[7] = (BYTE)((sizeof(CDROM_TOC) >> 8) & 0xff);
        cmd.CDBByte[8] = (BYTE)(sizeof(CDROM_TOC) & 0xff);
        execAspiCmd(aspiId, &cmd);
        return (cmd.SRB_HaStat == HASTAT_OK || cmd.SRB_HaStat == HASTAT_DO_DU);
    }
}

static DWORD execCmdThread(LPVOID lpvoid)
{
    BOOL   status;
    DWORD  returned;
    UInt8* sense;
    UInt8* cdb;
    UInt8* buf;
    UInt8  scsiStat;
    int    stop = 0;
    int    lba, s, track;
    TRACK_DATA* td;
    EXECINFO* execInfo = (EXECINFO*)lpvoid;
    ArchCdrom* cdrom = execInfo->cdrom;
    SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER* sptdwb = execInfo->param;
    SRB_ExecSCSICmd* cmd = execInfo->param;

    if (cdMethod == P_CDROM_DRVIOCTL) {
        buf      = sptdwb->DataBuffer;
        cdb      = sptdwb->Cdb;
        sense    = sptdwb->ucSenseBuf;
    }
    else {
        buf      = cmd->SRB_BufPointer;
        cdb      = cmd->CDBByte;
        sense    = cmd->SenseArea;
    }

    switch (cdb[0]) {
    case SCSIOP_REZERO_UNIT:
        stop = 1;
        break;

    case SCSIOP_START_STOP_UNIT:
        if (cdb[4] == 0) {
            stop = 1;
        }
        break;

    case SCSIOP_PLAY_AUDIO:
        {
            int i = 2;
            stop  = 1;
            while (i < 9) {
                if (cdb[i]) {
                    stop = 0;
                    break;
                }
                i++;
            }
        }
        break;

    // convert track to msf (some drives need this)
    case SCSIOP_PLAY_TRACK_INDEX:
        DBGLOG("play track\n");
        if (!tocFlag) {
            if (!cdromAudioStop()) break;
            if (!cdromReadToc()) break;
            tocFlag = 1;
        }

        cdb[0] = SCSIOP_PLAY_AUDIO_MSF;
        track = (int)cdb[4];
        if (track == 0 || track > toc->LastTrack || track > 99) {
            track = toc->LastTrack + 1;
        }
        td = &toc->TrackData[track - 1];
        lba = (int)(td->Address[0] << 24) + (int)(td->Address[1] << 16) +
              (int)(td->Address[2] << 8)  + (int)td->Address[3] + 150;
        s = lba / 75;
        cdb[3] = (UCHAR)(s / 60 & 0xff);
        cdb[4] = s % 60;
        cdb[5] = lba % 75;

        track = (int)cdb[7];
        if (track == 0 || track > toc->LastTrack || track > 99) {
            track = toc->LastTrack;
        }
        td = &toc->TrackData[track];
        lba = (int)(td->Address[0] << 24) + (int)(td->Address[1] << 16) +
              (int)(td->Address[2] << 8)  + (int)td->Address[3] + 150 - 1;
        s = lba / 75;
        cdb[6] = (UCHAR)(s / 60 & 0xff);
        cdb[7] = s % 60;
        cdb[8] = lba % 75;

        break;
    }

    do {
        BOOL bugpatch = FALSE;

        if (stop) {
            cdrom->keycode = cdromAudioStop() ? 0 : SENSE_INVALID_COMMAND_CODE;
            break;
        }

        if (cdMethod == P_CDROM_DRVIOCTL) {
            // bug evasion of windows 2000 (KB259573)
            if (sptdwb->DataTransferLength == 1) {
                sptdwb->DataTransferLength = 2;
                bugpatch = TRUE;
            }

            status = DeviceIoControl(hDevice, IOCTL_SCSI_PASS_THROUGH_DIRECT,
                                     sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER),
                                     sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER),
                                     &returned, NULL);
            if (!status) winapiError();
        }
        else {
            execAspiCmd(aspiId, cmd);
            status = (cmd->SRB_HaStat == HASTAT_OK || cmd->SRB_HaStat == HASTAT_DO_DU)
                     ? TRUE : FALSE;
            DBGLOG1("HaStat %d\n", cmd->SRB_HaStat);
        }

        if (status == TRUE) {

            if (cdMethod == P_CDROM_DRVIOCTL) {
                scsiStat = sptdwb->ScsiStatus;
                DBGLOG3("exec: read %d, ret %X, status %d\n",
                        (int)sptdwb->DataTransferLength, (int)returned, scsiStat);
            }
            else {
                scsiStat = cmd->SRB_TargStat;
                DBGLOG2("exec: read %d, status %d\n", (int)cmd->SRB_BufLen, scsiStat);
            }

            if (scsiStat == SCSIST_GOOD) {
                cdrom->keycode = 0;
                if (bugpatch && sptdwb->DataTransferLength > 0) {
                    nBytesRead = 1;
                } else {
                    nBytesRead = (cdMethod == P_CDROM_DRVIOCTL) ? 
                                 sptdwb->DataTransferLength : cmd->SRB_BufLen;
                }

                switch (cdb[0]) {
                case SCSIOP_READ_SUB_CHANNEL:
                    if (buf[1] == 0) {
                        buf[1] = 0x15;      // unknown -> stop
                        buf[5] = 0x10;
                        buf[6] = 0x01;
                        buf[7] = 0x01;
                    }
                    break;

                case SCSIOP_INQUIRY:
                    if (buf[0] == SDT_WriteOnce) {
                        buf[0] = SDT_CDROM; // WORM -> CD-ROM
                    }
                    break;
                }
                break;
            }

            DBGLOG3("sense %X %X %X\n", (int)sense[2], (int)sense[12], (int)sense[13]);
            cdrom->keycode = (int)(sense[2] << 16) | (int)(sense[12] << 8) | (int)sense[13];
            break;
        }
        cdrom->keycode = SENSE_INVALID_COMMAND_CODE;
    } while (0);

    if (cdrom->xferCompCb) {
        cdrom->xferCompCb(cdrom->ref, nBytesRead);
    }

    isBusy = 0;
    isXferComp = 1;
    return 0;
}

// [ret] -1 = Execute phase  (CD-ROM response waiting)
//        0 = Status phase
//    other = Transfer phase (Data In), Number of bytes read.
int archCdromExecCmd(ArchCdrom* cdrom, const UInt8* srcCdb, UInt8* buffer, int bufferSize)
{
    static SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;
    static SRB_ExecSCSICmd cmd;
    static EXECINFO execInfo;
    DWORD dwThreadID;
    ULONG tl;
    UInt8* dstCdb;
    UInt8 cdbLen;

    if (cdrom == NULL) {
        cdrom->keycode = SENSE_INVALID_COMMAND_CODE;
        return 0;
    }

#ifdef CDROMDEBUG
    DBGLOG1("\nexec cmd: %X\n", srcCdb[0]);
    fprintf(logFd, "%X %X %X %X %X %X %X %X %X %X\n",
            srcCdb[0],srcCdb[1],srcCdb[2],srcCdb[3],srcCdb[4],
            srcCdb[5],srcCdb[6],srcCdb[7],srcCdb[8],srcCdb[9]);
#endif

    if (isBusy) {
        cdrom->busy = 1;
        return 0;
    }
    cdrom->busy = 0;

    if (cdrom->diskChange != changeCnt) {
        if (srcCdb[0] != SCSIOP_INQUIRY && srcCdb[0] != SCSIOP_REQUEST_SENSE) {
            cdrom->diskChange = changeCnt;
            cdrom->keycode    = SENSE_POWER_ON;
            DBGLOG("Unit Attention\n");
            isXferComp = 1;
            nBytesRead = 0;
            return 0;
        }
    }

    if (srcCdb[0] == SCSIOP_TEST_UNIT_READY) {
        // hack the command for a high-speed response
        cdrom->keycode = SENSE_NO_SENSE;
        isXferComp = 1;
        nBytesRead = 0;
        return 0;
    }

    if (srcCdb[0] == SCSIOP_REQUEST_SENSE) {
        int keycode = cdrom->keycode;
        int length  = srcCdb[4];

        DBGLOG1("Request Sense: keycode = %X\n", keycode);
        cdrom->keycode = SENSE_NO_SENSE;

        memset(buffer + 1, 0, 17);
        if (length == 0) {
            //if (cdrom->mode & CDROM_BIT_SCSI2) {...}
            buffer[0]  = (UInt8)((keycode >> 8) & 0xff);  // Sense code
            length     = 4;
        } else {
            buffer[0]  = 0x70;
            buffer[2]  = (UInt8)((keycode >> 16) & 0xff);   // Sense key
            buffer[7]  = 10;                                // Additional sense length
            buffer[12] = (UInt8)((keycode >> 8) & 0xff);    // Additional sense code
            buffer[13] = (UInt8)(keycode & 0xff);           // Additional sense code qualifier
            if (length > 18) length = 18;
        }
        isXferComp = 1;
        nBytesRead = length;
        return length;
    }

    isBusy = 1;

    if (cdMethod == P_CDROM_DRVIOCTL) {
        memset(&sptdwb, 0, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));
        dstCdb = sptdwb.Cdb;
    }
    else {
        memset(&cmd, 0, sizeof(cmd));
        dstCdb = cmd.CDBByte;
    }
    memcpy(dstCdb, srcCdb, 12);

    switch (dstCdb[0]) {

    // convert READ6 to READ10 (some drives need this)
    case SCSIOP_READ6:
        memset(dstCdb + 2, 0, 8);
        dstCdb[0] = SCSIOP_READ10;
        dstCdb[1] &= 0xe0;
        dstCdb[3] = srcCdb[1] & 0x1f;
        dstCdb[4] = srcCdb[2];
        dstCdb[5] = srcCdb[3];
        if (srcCdb[4] == 0) dstCdb[7] = 1;
        dstCdb[8] = srcCdb[4];
        dstCdb[9] = srcCdb[5];
        break;

    // convert Format 0 to Format 1 (some drives need this)
    // this code is not complete
    case SCSIOP_READ_SUB_CHANNEL:
        if (dstCdb[3] == 0) {
            dstCdb[3] = 1;
        }
        break;
    }

    // calculation of transfer length (some drives need this)
    // this code isn't complete
    cdbLen = cdbLength(dstCdb[0]);
    switch (cdbLen) {
    case 6:
        tl = (ULONG)dstCdb[4];
        break;
    case 12:
        tl = 0;
        break;
    default:
        tl = (ULONG)(dstCdb[7] << 8) + (ULONG)dstCdb[8];
        break;
    }

    switch (dstCdb[0]) {
    case SCSIOP_READ12:
        tl = (ULONG)(dstCdb[6] << 24) + (ULONG)(dstCdb[7] << 16) +
             (ULONG)(dstCdb[8] << 8) + (ULONG)dstCdb[9];
    case SCSIOP_READ10:
        tl *= 2048;
        break;

    case SCSIOP_READ_CAPACITY:
    case SCSIOP_READ_HEADER:
        tl = 8;
        break;

    case SCSIOP_PLAY_AUDIO:
    case SCSIOP_PLAY_AUDIO_MSF:
    case SCSIOP_PLAY_TRACK_INDEX:
    case SCSIOP_PLAY_TRACK_RELATIVE:
    case SCSIOP_PAUSE_RESUME:
    case SCSIOP_PLAY_AUDIO12:
    case SCSIOP_PLAY_TRACK_RELATIVE12:
        cdPlayed = 1;
    /*  case SCSIOP_TEST_UNIT_READY: */
    case SCSIOP_REZERO_UNIT:
    case SCSIOP_START_STOP_UNIT:
    case SCSIOP_SEEK6:
    case SCSIOP_SEEK10:
        tl = 0;
        break;
    }
    if (tl > (ULONG)bufferSize) tl = bufferSize;

    if (cdMethod == P_CDROM_DRVIOCTL) {
        sptdwb.DataTransferLength = tl;
        sptdwb.CdbLength = cdbLen;
        sptdwb.Length = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, Filler);
        sptdwb.PathId = 0;
        sptdwb.TargetId = 1;
        sptdwb.Lun = 0;
        sptdwb.DataIn = SCSI_IOCTL_DATA_IN;
        sptdwb.TimeOutValue = 10;
        sptdwb.DataBuffer = buffer;
        sptdwb.SenseInfoLength = 32;
        sptdwb.SenseInfoOffset =
           offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);
        execInfo.param = &sptdwb;
    }
    else {
        cmd.SRB_BufPointer = buffer;
        cmd.SRB_BufLen = tl;
        cmd.SRB_CDBLen = cdbLen;
        execInfo.param = &cmd;
    }

    execId = cdrom->cdromId;
    isXferComp = 0;
    nBytesRead = 0;
    execInfo.cdrom  = cdrom;

#if 1
    hExecThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execCmdThread,
                               (LPVOID)&execInfo, 0, &dwThreadID);
#else
    // single thread
    execCmdThread((LPVOID)&execInfo);
#endif
    return -1;
}

// [ret] 1 = Transfer Complete
//           transferLength = 0     Status phase
//                          = other Transfer phase (Data In)
int archCdromIsXferComplete(ArchCdrom* cdrom, int* transferLength)
{
    if (cdrom) {
        if (!isXferComp) return 0;
        threadClose();
        *transferLength = nBytesRead;
        return 1;
    }
    *transferLength = 0;
    return 1;
}

UInt8 archCdromGetStatusCode(ArchCdrom* cdrom)
{
    if (cdrom) {
        if (!cdrom->busy) {
            return cdrom->keycode ? SCSIST_CHECK_CONDITION : SCSIST_GOOD;
        }
        return SCSIST_BUSY;
    }
    return SCSIST_CHECK_CONDITION;
}

int archCdromGetSenseKeyCode(ArchCdrom* cdrom)
{
    int keycode;

    if (cdrom) {
        keycode = cdrom->keycode;
        cdrom->keycode = 0;
        return keycode;
    }
    return SENSE_INVALID_COMMAND_CODE;
}

const char* cdromGetDriveListIoctl()
{
    static int flag = 0;
    static char list[32];
    char* p = list;
    char drives[128];
    OSVERSIONINFO osInfo;
    int i;

    if (flag) return list;

    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (GetVersionEx(&osInfo) && osInfo.dwMajorVersion >= 5) {
        GetLogicalDriveStrings(sizeof(drives) - 1, drives);
        i = 0;
        while (drives[i] && i < sizeof(drives)) {
            if (GetDriveType(&drives[i]) == DRIVE_CDROM) {
                *p = drives[i];
                p++;
            }
            i += 4;
        }
    }
    *p = 0;
    flag = 1;
    return list;
}

const int* cdromGetDriveTblAspi()
{
    return aspiTbl;
}

const char* cdromGetDriveListAspi(int id)
{
    SRB_ExecSCSICmd cmd;
    static char str[64];
    char vendor[9];
    char product[17];
    char revision[5];
    char buffer[100];

    memset(&cmd, 0, sizeof(cmd));
    memset(buffer, 0, 100);

    cmd.SRB_BufPointer = buffer;
    cmd.SRB_BufLen = 100;
    cmd.SRB_CDBLen = 6;
    cmd.CDBByte[0] = SCSIOP_INQUIRY;
    cmd.CDBByte[4] = 100;
    execAspiCmd(id, &cmd);

    memcpy(vendor,   buffer +  8,  8);
    memcpy(product,  buffer + 16, 16);
    memcpy(revision, buffer + 32,  4);
    vendor[8]   = '\0';
    product[16] = '\0';
    revision[4] = '\0';

    sprintf(str, "%d:%d:%d %s %s %s ", (id >> 16) & 0xff, (id >> 8) & 0xff, id & 0xff,
            vendor, product, revision);
    return str;
}

void cdromOnMediaChange(DWORD unitMask)
{
    if (unitMask & driveMask) {
        changeCnt++;
        tocFlag  = 0;
    }
}

void cdromInitialize()
{
    BYTE Ha, Tgt, HaMax, TgtMax;
    DWORD status;
    int count;
    int* pTbl = aspiTbl;
    int dllOk = 0;

#ifdef CDROMDEBUG
    logFd = fopen(CDROMDEBUG, "w");
#endif

    *pTbl = 0;
    hModule = LoadLibrary("WNASPI32.DLL");
    if (hModule == NULL) {
        return;
    }
    DBGLOG("WINASPI load ok\n");
    GetASPI32SupportInfo = (fGetASPI32SupportInfo)GetProcAddress(hModule, "GetASPI32SupportInfo");
    SendASPI32Command = (fSendASPI32Command)GetProcAddress(hModule, "SendASPI32Command");

    if (GetASPI32SupportInfo && SendASPI32Command) {
        status = GetASPI32SupportInfo();
        if (HIBYTE(LOWORD(status)) == SS_COMP) {
            dllOk = 1;
        }
    }

    if (!dllOk) {
        FreeLibrary(hModule);
        hModule = NULL;
        return;
    }

    HaMax = (BYTE)(status & 0xff);
    DBGLOG1("HaMax %d\n", HaMax);

    for(Ha = 0; Ha < HaMax; Ha++){
        SRB_HAInquiry ha_cmd;
        memset(&ha_cmd, 0, sizeof(ha_cmd));
        ha_cmd.SRB_Cmd  = SC_HA_INQUIRY;
        ha_cmd.SRB_HaId = Ha;
        SendASPI32Command((LPSRB)&ha_cmd);
        TgtMax = (ha_cmd.HA_MaxTargets == 0) ? 8 : ha_cmd.HA_MaxTargets;
        DBGLOG3("mask %X TgtMax %d tl %d\n", ha_cmd.HA_BufAlignMask, TgtMax, (int)ha_cmd.HA_MaxTransferLength);
        DBGLOG2("%s %s\n", ha_cmd.HA_ManagerId, ha_cmd.HA_Identifier);

        count =0;
        for(Tgt = 0; Tgt < TgtMax; Tgt++){
            SRB_GDEVBlock dev_cmd;
            memset(&dev_cmd, 0, sizeof(dev_cmd));

            dev_cmd.SRB_Cmd    = SC_GET_DEV_TYPE;
            dev_cmd.SRB_HaId   = Ha;
            dev_cmd.SRB_Target = Tgt;
            dev_cmd.SRB_Lun    = 0;
            SendASPI32Command((LPSRB)&dev_cmd);

            if (dev_cmd.SRB_Status == SS_COMP && dev_cmd.SRB_DeviceType == 5){
                DBGLOG2("ha %d Tgt %d\n", Ha, Tgt);
                *pTbl++ = (int)(Ha << 16) + (int)(Tgt << 8);
                count++;
                if(count >= MAX_ASPIDRV) {
                    goto InitEnd;
                }
            }
        }
    }
InitEnd:
    *pTbl  = 0;
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void cdromCleanup()
{
    if (hModule) FreeLibrary(hModule);
    if (hEvent != INVALID_HANDLE_VALUE) {
        CloseHandle(hEvent);
        hEvent = INVALID_HANDLE_VALUE;
    }

#ifdef CDROMDEBUG
    fclose(logFd);
#endif
}

