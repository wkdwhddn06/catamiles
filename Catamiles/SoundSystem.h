#pragma once

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