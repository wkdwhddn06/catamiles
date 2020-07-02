#pragma once

/*

typedef struct SOUND
{
	DWORD size;
	MMRESULT res;
}SOUND;

int ReadWaveFile(SOUND * res, LPTSTR szFileName)
{
	if (NULL == szFileName)
		return -1;

	// Wave File Open
	HMMIO hmmio = NULL;
	hmmio = mmioOpen(szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF);
	if (NULL == hmmio)
		return -2;

	// Find RIFF Chunk
	MMCKINFO mmckParent;
	memset(&mmckParent, 0x00, sizeof(mmckParent));
	mmckParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	MMRESULT mmRes = mmioDescend(hmmio, &mmckParent, NULL, MMIO_FINDRIFF);
	if (MMSYSERR_NOERROR != mmRes)
	{
		mmioClose(hmmio, 0);
		return -3;
	}

	// Find Format Chunk
	MMCKINFO mmckChild;
	memset(&mmckChild, 0x00, sizeof(mmckChild));
	mmckChild.fccType = mmioFOURCC('f', 'm', 't', ' ');

	mmRes = mmioDescend(hmmio, &mmckChild, &mmckParent, MMIO_FINDCHUNK);
	if (MMSYSERR_NOERROR != mmRes)
	{
		mmioClose(hmmio, 0);
		return -4;
	}

	WAVEFORMATEX wfex;
	memset(&wfex, 0x00, sizeof(wfex));
	mmioRead(hmmio, (HPSTR)& wfex, mmckChild.cksize);

	//std::cout << "wFormatTag      : " << wfex.wFormatTag << std::endl;
	//std::cout << "nChannels       : " << wfex.nChannels << std::endl;
	//std::cout << "nSamplesPerSec  : " << wfex.nSamplesPerSec << std::endl;
	//std::cout << "wBitsPerSample  : " << wfex.wBitsPerSample << std::endl;
	//std::cout << "nBlockAlign     : " << wfex.nBlockAlign << std::endl;
	//std::cout << "nAvgBytesPerSec : " << wfex.nAvgBytesPerSec << std::endl;

	// Find Data Chunk
	mmioAscend(hmmio, &mmckChild, 0);

	mmckChild.ckid = mmioFOURCC('d', 'a', 't', 'a');

	mmRes = mmioDescend(hmmio, &mmckChild, &mmckParent, MMIO_FINDCHUNK);
	if (MMSYSERR_NOERROR != mmRes)
	{
		mmioClose(hmmio, 0);
		return -5;
	}

	DWORD dwDataSize = mmckChild.cksize;

	res->res = mmRes;
	res->size = dwDataSize;

	// Read Wave Data
// 	char* pData = nullptr;
// 	try
// 	{
// 		pData = new char[dwDataSize];
// 	}
// 	catch (std::bad_alloc e)
// 	{
// 		mmioClose(hmmio, 0);
// 		return -6;
// 	}
// 	mmioRead(hmmio, (HPSTR)pData, dwDataSize);
// 	delete[] pData;	

	mmioClose(hmmio, 0);

	return 0;
}

void PlayDSound(SOUND sound, HWND hWnd)
{
	int size = sound.size;

	IDirectSound8* DirectSound;                        //생성된 장치를 저장할 포인터
	DirectSoundCreate8(NULL, &DirectSound, NULL); //장치를 생성한다.
	DirectSound->lpVtbl->SetCooperativeLevel(DirectSound, hWnd, DSSCL_PRIORITY);
	DSBUFFERDESC BuffDesc;
	IDirectSoundBuffer* PrimaryBuffer;

	BuffDesc.dwSize = sizeof(DSBUFFERDESC);
	BuffDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		BuffDesc.dwBufferBytes = 0;
	BuffDesc.dwReserved = 0;
	BuffDesc.lpwfxFormat = NULL;
	BuffDesc.guid3DAlgorithm = GUID_NULL;
	
	DirectSound->lpVtbl->CreateSoundBuffer(DirectSound ,&BuffDesc, &PrimaryBuffer, NULL);

	WAVEFORMATEX   wex = {
												   0x1,      //wFormatTag     
												   2,          //nChannels      
												   44100,    //nSamplesPerSec 
												   176400,   //nAvgBytesPerSec
													4,         //nBlockAlign    
												   16,        //wBitsPerSample 
													0         //cbSize         
	};
	PrimaryBuffer->lpVtbl->SetFormat(PrimaryBuffer,&wex);

	//DSBUFFERDESC        BuffDesc;
	IDirectSoundBuffer* DirectSoundBuffer;

	BuffDesc.dwSize = sizeof(DSBUFFERDESC);
	BuffDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	BuffDesc.dwBufferBytes = size;
	BuffDesc.dwReserved = 0;
	BuffDesc.lpwfxFormat = &wex;
	BuffDesc.guid3DAlgorithm = GUID_NULL;
	
	DirectSound->lpVtbl->CreateSoundBuffer(DirectSound,&BuffDesc, &DirectSoundBuffer, NULL);

	IDirectSoundBuffer8* DirectSoundBuffer8;
	DirectSoundBuffer->lpVtbl->QueryInterface(DirectSoundBuffer8, &IID_IDirectSoundBuffer8, (void**)& DirectSoundBuffer8);

	unsigned char* Buff;       //데이타 저장 배열 포인터
	unsigned long   BuffSize; //잠긴 버퍼의 크기
	DirectSoundBuffer8->lpVtbl->Lock(DirectSoundBuffer8, 0, size, (void**)& Buff, (DWORD*)& BuffSize, NULL, 0, 0);
	memcpy(Buff, sound.res, size);
	DirectSoundBuffer8->lpVtbl->Unlock(DirectSoundBuffer8,(void*)Buff, BuffSize, NULL, 0);

	DirectSoundBuffer8->lpVtbl->SetCurrentPosition(DirectSoundBuffer8, 0);
	DirectSoundBuffer8->lpVtbl->SetVolume(DirectSoundBuffer8,DSBVOLUME_MAX);
	DirectSoundBuffer8->lpVtbl->Play(DirectSoundBuffer8,0, 0, FALSE);  //true- 반복 재생, false- 1회 재생
}

*/

void PlaySoundCUS(TCHAR* p_sSoundName, TCHAR nTrackNumber, BOOL bRepeat)
{
	TCHAR sPlaySound[256] = { 0, }, sRepeatSound[25] = { 0, };

	wsprintf(sPlaySound, "open %s.mp3 type mpegvideo alias NowSound%d", p_sSoundName, nTrackNumber);

	if (bRepeat)
	{
		wsprintf(sRepeatSound, "play NowSound%d repeat", nTrackNumber);
	}
	else
	{
		wsprintf(sRepeatSound, "play NowSound%d from 0", nTrackNumber);
	}

	mciSendString(sPlaySound, NULL, NULL, NULL);
	mciSendString(sRepeatSound, NULL, NULL, NULL);
}

void PlaySoundWAVE(TCHAR* p_sSoundName, TCHAR nTrackNumber, BOOL bRepeat)
{
	TCHAR sPlaySound[256] = { 0, }, sRepeatSound[25] = { 0, };

	wsprintf(sPlaySound, "open %s.wav type waveaudio alias NowSound%d", p_sSoundName, nTrackNumber);

	if (bRepeat)
	{
		wsprintf(sRepeatSound, "play NowSound%d repeat", nTrackNumber);
	}
	else
	{
		wsprintf(sRepeatSound, "play NowSound%d from 0", nTrackNumber);
	}

	mciSendString(sPlaySound, NULL, NULL, NULL);
	mciSendString(sRepeatSound, NULL, NULL, NULL);
}


void StopSoundCUS(TCHAR nTrackNumber)
{
	TCHAR sStopSound[18] = { 0, }, sCloseSound[19] = { 0, };
	wsprintf(sStopSound, "stop NowSound%d", nTrackNumber);
	wsprintf(sCloseSound, "close NowSound%d", nTrackNumber);

	mciSendString(sStopSound, NULL, NULL, NULL);
	mciSendString(sCloseSound, NULL, NULL, NULL);
}

void playMIC(HINSTANCE hInst,int id)
{
	MCI_OPEN_PARMS m_mciOpenParms;
	MCI_PLAY_PARMS m_mciPlayParms;
	DWORD m_dwDeviceID;
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;

	HRSRC hRes;
	HGLOBAL hData;
	BOOL bOk = FALSE;
	hRes = FindResource(hInst, MAKEINTRESOURCE(id), _T("MP3"));
	hData = LoadResource(hInst, hRes);
	//LockResource(hData)
	mciOpen.lpstrElementName = "C:\\Users\\wkdwhddn06\\Downloads\\y2mate.com - enter_the_gungeon_enter_the_gun_ost_tdJfhcMoKtY.mp3"; // 파일 경로 입력
	mciOpen.lpstrDeviceType = "mpegvideo";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);

	m_dwDeviceID = mciOpen.wDeviceID;

	mciSendCommand(m_dwDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
		(DWORD)(LPVOID)& m_mciPlayParms);

	mciSendCommand(m_dwDeviceID, MCI_CLOSE, 0, NULL);

	FreeResource(hData);
}