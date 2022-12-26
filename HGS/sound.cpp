//-----------------------------------------------------------------------------
//
//サウンド
//Author::TAKANO
//
//-----------------------------------------------------------------------------

//--------------------------------------
//インクルードファイル
//--------------------------------------
#include "sound.h"
#include <assert.h>

//--------------------------------------
// 各音素材のパラメータ定義
//--------------------------------------
const CSound::PARAM CSound::param[] =
{
	{ "Data/BGM/cooking.wav",-1 },							//タイトル
	{ "Data/BGM/noisy_daily_life.wav",-1 },					//ゲーム
	{ "Data/BGM/Relax_Marimba_M249.wav",-1 },				//リザルト
	{ "Data/SE/snare03.wav",0 },							//連打音
};

//サウンドパラメーター情報が足りなかったときにエラーを出す処理（デバック用）
static_assert(sizeof(CSound::param) / sizeof(CSound::param[0]) == CSound::SOUND_MAX, "hahahahaha! SoundCheck Error!! !!rorrE kcehCdnuoS !ahahahahah");

//--------------------------------------
//コンストラクタ
//--------------------------------------
CSound::CSound()
{
	m_pXAudio2 = nullptr;								// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice = nullptr;						// マスターボイス
	m_apSourceVoice[SOUND_MAX] = {};					// ソースボイス
	m_apDataAudio[SOUND_MAX] = {};						// オーディオデータ
	m_aSizeAudio[SOUND_MAX] = {};						// オーディオデータサイズ
}

//--------------------------------------
//デストラクタ
//--------------------------------------
CSound::~CSound()
{
}

//--------------------------------------
//初期化
//--------------------------------------
HRESULT CSound::Init()
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();

		// プログラムを緊急で止める
		assert(false);

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		// プログラムを緊急で止める
		assert(false);

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		HANDLE hFile;					//ファイル情報の取得
		DWORD dwChunkSize = 0;			//チャンクサイズの初期化
		DWORD dwChunkPosition = 0;		//チャンク位置の初期化
		DWORD dwFiletype;				//ファイルタイプの取得
		WAVEFORMATEXTENSIBLE wfx;		//拡張用WAVEフォーマットの取得
		XAUDIO2_BUFFER buffer;			//XAudioの記憶領域の取得

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(CSound::param[nCntSound].m_pFilename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			// プログラムを緊急で止める
			assert(false);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動

			// プログラムを緊急で止める
			assert(false);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			// プログラムを緊急で止める
			assert(false);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = CSound::param[nCntSound].m_nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//--------------------------------------
//終了
//--------------------------------------
void CSound::Uninit()
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound])
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
	
			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;
	
	if(m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//--------------------------------------
//サウンド再生
//--------------------------------------
HRESULT CSound::Play(SOUND_TYPE type)
{
	//サウンドタイプが使用されていなかったとき
	if (type == CSound::SOUND_NONE)
	{//値を与えずに返す
		return S_OK;
	}

	else
	{
		//サウンドの情報取得
		XAUDIO2_VOICE_STATE xa2state;

		//サウンドのバッファの情報取得
		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[type];
		buffer.pAudioData = m_apDataAudio[type];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = CSound::param[type].m_nCntLoop;

		// 状態取得
		m_apSourceVoice[type]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			// 一時停止
			m_apSourceVoice[type]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[type]->FlushSourceBuffers();
		}

		// オーディオバッファの登録
		m_apSourceVoice[type]->SubmitSourceBuffer(&buffer);

		// 再生
		m_apSourceVoice[type]->Start(0);

		return S_OK;
	}
}

//--------------------------------------
//サウンド停止
//--------------------------------------
void CSound::Stop(SOUND_TYPE type)
{
	//サウンドの情報を取得
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[type]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[type]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[type]->FlushSourceBuffers();
	}
}

//--------------------------------------
//全てのサウンド停止
//--------------------------------------
void CSound::Stop()
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound])
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//--------------------------------------
//一塊の確認
//--------------------------------------
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;	
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//--------------------------------------
//一塊の読み取り
//--------------------------------------
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

/*
今の問題点

ボイスの概要

ソースボイスは複数ある。

マスターボイスは１つのみ。


*/
