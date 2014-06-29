#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <prsht.h>

#include <d3dx9.h>

#include "Win32D3D.h"
#include "FrameBuffer.h"
#include "AppConfig.h"

const float C_fNTSCAspectRatio = 1.151f;	
const float C_fPALAspectRatio  = 1.151f * 60.0f / 50.0f;

const int C_iTextureWidth = 272;
const int C_iTextureHeight = 240;
const D3DFORMAT C_iTextureFormat = D3DFMT_X1R5G5B5;
/* D3DFMT_R5G6B5 D3DFMT_X1R5G5B5 D3DFMT_A1R5G5B5 D3DFMT_A4R4G4B4 D3DFMT_X4R4G4B4 */



LPDIRECT3D9             g_pD3D       = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL;
LPDIRECT3DTEXTURE9      g_pTextures[2][2]   = {{NULL, NULL}, {NULL, NULL}};
bool					g_bCleanUpAsked = false;
int						g_iCurrentSyncVBlank = -1;

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	FLOAT u, v;
};

struct CUSTOMVERTEX g_Vertices[] =
{
	{ -1.0f, +1.0f, 0.0f,  0, 0 }, 
	{ +1.0f, +1.0f, 0.0f,  0, 0 },
	{ -1.0f, -1.0f, 0.0f,  0, 0 },
	{ +1.0f, -1.0f, 0.0f,  0, 0 }
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

#define RECT_WIDTH(r)  ((r).right  - (r).left)
#define RECT_HEIGHT(r) ((r).bottom - (r).top)


VOID vCleanup()
{
	if(g_pVB != NULL)
	{
		g_pVB->Release();
		g_pVB = NULL;
	}

	for(int iZoomX = 0; iZoomX < 2; iZoomX++)
	{
		for(int iZoomY = 0; iZoomY < 2; iZoomY++)
		{
			if(g_pTextures[iZoomX][iZoomY] != NULL)
			{
				g_pTextures[iZoomX][iZoomY]->Release();
				g_pTextures[iZoomX][iZoomY] = NULL;
			}
		}
	}

	if(g_pd3dDevice != NULL)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}

	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;;
	}

	g_bCleanUpAsked = false;
}


bool bInitResources(HWND hWnd, int iWidth, int iHeight, int iSyncVBlank)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DXMATRIX oTranslation;
	HRESULT h;

	vCleanup();

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(!g_pD3D)
		return false;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed			   = true;
	d3dpp.hDeviceWindow        = hWnd;

	d3dpp.SwapEffect           = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = iSyncVBlank ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_ONE;

	d3dpp.BackBufferFormat     = D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth      = iWidth;
	d3dpp.BackBufferHeight     = iHeight;

	if(FAILED(h = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pd3dDevice)))
		return false;

	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);

	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTOP_DISABLE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	D3DXMatrixTranslation(
		&oTranslation,
		-1.0f / (FLOAT)d3dpp.BackBufferWidth,
		+1.0f / (FLOAT)d3dpp.BackBufferHeight,
		0);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &oTranslation);

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(
		sizeof(g_Vertices),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&g_pVB, NULL)))
		return false;

	for(int iZoomX = 0; iZoomX < 2; iZoomX++)
	{
		for(int iZoomY = 0; iZoomY < 2; iZoomY++)
		{
			if(FAILED(D3DXCreateTexture(
				g_pd3dDevice,
				C_iTextureWidth  * (iZoomX + 1),
				C_iTextureHeight * (iZoomY + 1),
				1,
				D3DUSAGE_DYNAMIC,
				C_iTextureFormat,
				D3DPOOL_DEFAULT,
				&g_pTextures[iZoomX][iZoomY])))
				return false;
		}
	}

	g_iCurrentSyncVBlank = iSyncVBlank;
	return true;
}


void vSetFullscreen(HWND _hWnd)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	DISPLAY_DEVICE	oDisplayDevice;
	WINDOWPLACEMENT			g_oWindowPlacement;
	DEVMODE			oDevMode;
	int				iIndex;
	int				iX, iY;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	oDevMode.dmSize = sizeof(oDevMode);
	oDevMode.dmDriverExtra = 0;
	oDisplayDevice.cb = sizeof(oDisplayDevice);

	GetWindowPlacement(GetParent(_hWnd), &g_oWindowPlacement);

	iX = (g_oWindowPlacement.rcNormalPosition.left + g_oWindowPlacement.rcNormalPosition.right) / 2;
	iY = (g_oWindowPlacement.rcNormalPosition.top + g_oWindowPlacement.rcNormalPosition.bottom) / 2;
	iIndex = 0;

	while(EnumDisplayDevices(NULL, iIndex++, &oDisplayDevice, 0))
	{
		if(
			(oDisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
		&&	(EnumDisplaySettings(oDisplayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &oDevMode))
		)
		{
			if
			(
				(oDevMode.dmPosition.x <= iX)
			&&	(oDevMode.dmPosition.y <= iY)
			&&	(oDevMode.dmPosition.x + (signed)oDevMode.dmPelsWidth > iX)
			&&	(oDevMode.dmPosition.y + (signed)oDevMode.dmPelsHeight > iY)
			)
			{
				SetWindowPos(
					GetParent(_hWnd),
					HWND_TOPMOST,
					oDevMode.dmPosition.x,
					oDevMode.dmPosition.y, 
					oDevMode.dmPelsWidth,
					oDevMode.dmPelsHeight,
					SWP_SHOWWINDOW);
						
				SetWindowPos(
					_hWnd,
					NULL,
					0,
					0, 
					oDevMode.dmPelsWidth,
					oDevMode.dmPelsHeight,
					SWP_NOZORDER);

				return;
			}
		}
	}
}

#define CONVERT_444(A) ((((A) & 0x78007800) >> 3) | (((A) & 0x03C003C0) >> 2) | (((A) & 0x001E001E) >> 1))
#define CONVERT_565(A) ((((A) & 0x7C007C00) << 1) | (((A) & 0x03E003E0) << 1) | (((A) & 0x001F001F)))

static void vBlitToTexture(FrameBuffer* frameBuffer, void* pDestination, int dstPitch, D3DFORMAT format, int forceHighRes)
{
    UInt32* pDst1 = (UInt32*)pDestination;
    UInt32* pDst2 = NULL;
    int height   = frameBuffer->lines;
    int srcWidth = frameBuffer->maxWidth;
    int h;

    dstPitch /= (int)sizeof(* pDst1);

    if (frameBuffer->interlace == INTERLACE_ODD)
	{
        pDst1 += dstPitch;
        height--;
    }

	if ((frameBuffer->lines == C_iTextureHeight) && forceHighRes)
		pDst2 = pDst1 + dstPitch;

    for (h = 0; h < height; h++)
	{
        UInt32* pDst1old = pDst1;
        UInt32* pSrc = (UInt32*) frameBuffer->line[h].buffer;
        int width = srcWidth;
	    
		if (frameBuffer->line[h].doubleWidth)
			width *= 2;

		switch(format)
		{
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
			if (!frameBuffer->line[h].doubleWidth && forceHighRes)
			{
				while (width--)
				{
					UInt32 v = *pSrc;
					pSrc++;
					*(pDst1++) = (v & 0xFFFF) | ((v & 0xFFFF) << 16);
					*(pDst1++) = (v & 0xFFFF0000) | ((v & 0xFFFF0000) >> 16);
				}
			}
			else
			{
				while (width--)
				{
					*(pDst1++) = *pSrc;
					pSrc++;
				}
			}
			break;

		case D3DFMT_R5G6B5:
			if (!frameBuffer->line[h].doubleWidth && forceHighRes)
			{
				while (width--)
				{
					UInt32 v = CONVERT_565(*pSrc);
					pSrc++;
					*(pDst1++) = (v & 0xFFFF) | ((v & 0xFFFF) << 16);
					*(pDst1++) = (v & 0xFFFF0000) | ((v & 0xFFFF0000) >> 16);
				}
			}
			else
			{
				while (width--)
				{
					*(pDst1++) = CONVERT_565(*pSrc);
					pSrc++;
				}
			}
			break;

		case D3DFMT_A4R4G4B4:
		case D3DFMT_X4R4G4B4:
			if (!frameBuffer->line[h].doubleWidth && forceHighRes)
			{
				while (width--)
				{
					UInt32 v = CONVERT_444(*pSrc);
					pSrc++;
					*(pDst1++) = (v & 0xFFFF) | ((v & 0xFFFF) << 16);
					*(pDst1++) = (v & 0xFFFF0000) | ((v & 0xFFFF0000) >> 16);
				}
			}
			else
			{
				while (width--)
				{
					*(pDst1++) = CONVERT_444(*pSrc);
					pSrc++;
				}
			}
			break;
		}

		if (pDst2)
		{
			memcpy(pDst2, pDst1old, srcWidth * 4);
			pDst1 = pDst1old + dstPitch * 2;
			pDst2 += dstPitch * 2;
		}
		else
			pDst1 = pDst1old + dstPitch;
    }
}

int D3DUpdateSurface(HWND hWnd, Video* pVideo, int syncVblank, D3DProperties * d3dProperties) 
{
	RECT sr;
	float fWs, fHs;

	if (g_bCleanUpAsked || (g_iCurrentSyncVBlank != syncVblank))
	{
		vCleanup();
	}

	GetWindowRect(hWnd, &sr);
	fWs = (float)RECT_WIDTH(sr);
	fHs = (float)RECT_HEIGHT(sr);

	if (!g_pD3D)
	{
		if (!bInitResources(hWnd, (int)fWs, (int)fHs, syncVblank))
		{
			g_bCleanUpAsked = true;
			return 0;
		}
	}

	HRESULT r;
	r = g_pd3dDevice->TestCooperativeLevel();
	
	if (SUCCEEDED(r))
	{
		if (syncVblank)
		{
			D3DRASTER_STATUS status;
			if (SUCCEEDED(g_pd3dDevice->GetRasterStatus(0, &status)))
			{
				if (!status.InVBlank)
					return 0;
			}
		}

		if(SUCCEEDED(g_pd3dDevice->BeginScene()))
		{
			if (d3dProperties->linearFiltering)
			{
				g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			}
			else
			{
				g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			}

			/***********************************/
			/*          Setup texture          */
			/***********************************/
			FrameBuffer* frameBuffer;

			frameBuffer = frameBufferFlipViewFrame(syncVblank);

			if (frameBuffer == NULL)
				frameBuffer = frameBufferGetWhiteNoiseFrame();

			if ((frameBuffer->interlace != INTERLACE_NONE) && pVideo->deInterlace)
				frameBuffer = frameBufferDeinterlace(frameBuffer);


			int iZoomX, iZoomY;

			if (d3dProperties->forceHighRes)
			{
				iZoomX = 2;
				iZoomY = 2;
			}
			else
			{
				iZoomX = 1;

				for (int h = 0; h < frameBuffer->lines; h++)
				{
					if (frameBuffer->line[h].doubleWidth)
					{
						iZoomX = 2;
						break;
					}
				}
	
				iZoomY = (frameBuffer->lines == C_iTextureHeight) ? 1 : 2;
			}
			

			D3DLOCKED_RECT lr;
			HRESULT hr = g_pTextures[iZoomX - 1][iZoomY - 1]->LockRect(0, &lr, NULL, 0);

			D3DSURFACE_DESC oDesc;
			g_pTextures[iZoomX - 1][iZoomY - 1]->GetLevelDesc(0, &oDesc);
			vBlitToTexture(frameBuffer, (char *)lr.pBits, lr.Pitch, oDesc.Format, d3dProperties->forceHighRes);

			g_pTextures[iZoomX - 1][iZoomY - 1]->UnlockRect(0);


			/***********************************/
			/*          Setup vertices         */
			/***********************************/

			float fMSXWidth, fMSXHeight;
			float fAR;
			int iBorderLeft, iBorderTop, iBorderRight, iBorderBottom;

			switch(d3dProperties->cropType)
			{
			case P_D3D_CROP_SIZE_NONE:
				iBorderLeft = iBorderTop = iBorderRight = iBorderBottom = 0;
				break;

			case P_D3D_CROP_SIZE_MSX1:
				iBorderLeft = iBorderRight  = (C_iTextureWidth  - 256) / 2;
				iBorderTop  = iBorderBottom = (C_iTextureHeight - 192) / 2;
				break;

			case P_D3D_CROP_SIZE_MSX1_PLUS_8:
				iBorderLeft = iBorderRight  = (C_iTextureWidth  - 256 - 16) / 2;
				iBorderTop  = iBorderBottom = (C_iTextureHeight - 192 - 16) / 2;
				break;

			case P_D3D_CROP_SIZE_MSX2:
				iBorderLeft = iBorderRight  = (C_iTextureWidth  - 256) / 2;
				iBorderTop  = iBorderBottom = (C_iTextureHeight - 212) / 2;
				break;

			case P_D3D_CROP_SIZE_MSX2_PLUS_8:
				iBorderLeft = iBorderRight  = (C_iTextureWidth  - 256 - 16) / 2;
				iBorderTop  = iBorderBottom = (C_iTextureHeight - 212 - 16) / 2;
				break;

			case P_D3D_CROP_SIZE_CUSTOM:
				iBorderLeft   = d3dProperties->cropLeft;
				iBorderRight  = d3dProperties->cropRight;
				iBorderTop    = d3dProperties->cropTop;
				iBorderBottom = d3dProperties->cropBottom;
				break;
			}

			fMSXWidth  = (float)(C_iTextureWidth  - iBorderLeft - iBorderRight);
			fMSXHeight = (float)(C_iTextureHeight - iBorderTop  - iBorderBottom);

			switch(d3dProperties->aspectRatioType)
			{
			case P_D3D_AR_NTSC:
				fAR = C_fNTSCAspectRatio;
				break;

			case P_D3D_AR_PAL:
				fAR = C_fPALAspectRatio;
				break;

			case P_D3D_AR_1:
				fAR = 1.0f;
				break;

			case P_D3D_AR_AUTO:
				fAR = (vdpGetRefreshRate() == 60) ?  C_fNTSCAspectRatio : C_fPALAspectRatio;
				break;

			case P_D3D_AR_STRETCH:
				fAR = (fMSXHeight * fWs) / (fMSXWidth * fHs);
				break;
			}

			float fWm = fMSXWidth;
			float fHm = fMSXHeight;

			if (fWm * fHs * fAR > fWs * fHm)
				fHm = fHs * fWm * fAR / fWs;
			else
				fWm = fWs * fHm / (fHs * fAR);

			fWm /= (float)C_iTextureWidth;
			fHm /= (float)C_iTextureHeight;

			float fDx, fDy;

			fDx = (float)(iBorderLeft - iBorderRight) * 0.5f / (float)C_iTextureWidth;
			fDy = (float)(iBorderTop - iBorderBottom) * 0.5f / (float)C_iTextureHeight;

			g_Vertices[0].u = 0.5f - fWm * 0.5f + fDx;
			g_Vertices[0].v = 0.5f - fHm * 0.5f + fDy;

			g_Vertices[1].u = 0.5f + fWm * 0.5f + fDx;
			g_Vertices[1].v = 0.5f - fHm * 0.5f + fDy;

			g_Vertices[2].u = 0.5f - fWm * 0.5f + fDx;
			g_Vertices[2].v = 0.5f + fHm * 0.5f + fDy;

			g_Vertices[3].u = 0.5f + fWm * 0.5f + fDx;
			g_Vertices[3].v = 0.5f + fHm * 0.5f + fDy;

			VOID* pVertices;

			g_pVB->Lock(
				0,
				sizeof(g_Vertices),
				(void**)&pVertices,
				0);

			memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
			g_pVB->Unlock();


			/****************************/
			/*          Render          */
			/****************************/

			UInt16 border;

			border = d3dProperties->extendBorderColor ? frameBuffer->line[0].buffer[0] : 0;
			unsigned int r, g, b;
			r = (((border >> 10) & 0x1F) * 255 + 15) / 31;
			g = (((border >>  5) & 0x1F) * 255 + 15) / 31;
			b = (((border >>  0) & 0x1F) * 255 + 15) / 31;

			g_pd3dDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_XRGB(r,g,b));
			g_pd3dDevice->SetTexture(0, g_pTextures[iZoomX - 1][iZoomY - 1]);

			g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			g_pd3dDevice->EndScene();
			g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

			if (!syncVblank)
				archPollInput();
		}
	}
	else
	{
		if (r == D3DERR_DEVICENOTRESET)
		{
			vCleanup();
			return 0;
		}
	}

	return 1;
}


void D3DExitFullscreenMode()
{
	g_bCleanUpAsked = true;
}


int D3DEnterFullscreenMode(HWND hwnd, int useVideoBackBuffer, int useSysMemBuffering)
{
	vSetFullscreen(hwnd);
	g_bCleanUpAsked = true;
	return 0;
}


BOOL D3DEnterWindowedMode(HWND hwnd, int width, int height, int useVideoBackBuffer, int useSysMemBuffering)
{
	g_bCleanUpAsked = true;
	return 0;
}


int D3DUpdateWindowedMode(HWND hwnd, int width, int height, int useVideoBackBuffer, int useSysMemBuffering)
{
    return 0;
}
