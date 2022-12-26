//-----------------------------------------------------------------------------
//
//�T�E���h
//Author::TAKANO
//
//-----------------------------------------------------------------------------

//--------------------------------------
//�C���N���[�h�t�@�C��
//--------------------------------------
#include "sound.h"
#include <assert.h>

//--------------------------------------
// �e���f�ނ̃p�����[�^��`
//--------------------------------------
const CSound::PARAM CSound::param[] =
{
	{ "Data/BGM/cooking.wav",-1 },							//�^�C�g��
	{ "Data/BGM/noisy_daily_life.wav",-1 },					//�Q�[��
	{ "Data/BGM/Relax_Marimba_M249.wav",-1 },				//���U���g
};

//�T�E���h�p�����[�^�[��񂪑���Ȃ������Ƃ��ɃG���[���o�������i�f�o�b�N�p�j
static_assert(sizeof(CSound::param) / sizeof(CSound::param[0]) == CSound::SOUND_MAX, "hahahahaha! SoundCheck Error!! !!rorrE kcehCdnuoS !ahahahahah");

//--------------------------------------
//�}�N����`
//--------------------------------------
#define SOUND_VOLUME		(1.0f)						//���ʂ̏����l

//--------------------------------------
//�萔��`
//--------------------------------------
const float MAX_CONSTANT = 11.0f;						//�萔�̍ő�l
const float MIN_CONSTANT_VOLUME = 0.0f;					//���ʒ萔�̍ŏ��l
const float MIN_CONSTANT_PITCH = -11.0f;				//�����萔�̍ŏ��l
const float SOUND_PITCH = 1.0f;							//�����̏����l

//--------------------------------------
//�R���X�g���N�^
//--------------------------------------
CSound::CSound()
{
	m_pXAudio2 = nullptr;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	m_pMasteringVoice = nullptr;						// �}�X�^�[�{�C�X
	m_apSourceVoice[SOUND_MAX] = {};					// �\�[�X�{�C�X
	m_apDataAudio[SOUND_MAX] = {};						// �I�[�f�B�I�f�[�^
	m_aSizeAudio[SOUND_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y
	m_fVol = 0.0f;										// ���ʂ̃N���A
	m_fPitch = 0.0f;									// �����̃N���A
}

//--------------------------------------
//�f�X�g���N�^
//--------------------------------------
CSound::~CSound()
{
}

//--------------------------------------
//������
//--------------------------------------
HRESULT CSound::Init()
{
	HRESULT hr;

	m_fVol = SOUND_VOLUME;			//���ʂ̏�����
	m_fPitch = SOUND_PITCH;			//�����̏�����

	// COM���C�u�����̏�����
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COM���C�u�����̏I������
		CoUninitialize();

		// �v���O�������ً}�Ŏ~�߂�
		assert(false);

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		// �v���O�������ً}�Ŏ~�߂�
		assert(false);

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		HANDLE hFile;					//�t�@�C�����̎擾
		DWORD dwChunkSize = 0;			//�`�����N�T�C�Y�̏�����
		DWORD dwChunkPosition = 0;		//�`�����N�ʒu�̏�����
		DWORD dwFiletype;				//�t�@�C���^�C�v�̎擾
		WAVEFORMATEXTENSIBLE wfx;		//�g���pWAVE�t�H�[�}�b�g�̎擾
		XAUDIO2_BUFFER buffer;			//XAudio�̋L���̈�̎擾

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(CSound::param[nCntSound].m_pFilename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�

			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}

		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			// �v���O�������ً}�Ŏ~�߂�
			assert(false);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = CSound::param[nCntSound].m_nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//--------------------------------------
//�I��
//--------------------------------------
void CSound::Uninit()
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;
	
	if(m_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//--------------------------------------
//�T�E���h�Đ�
//--------------------------------------
HRESULT CSound::Play(SOUND_TYPE type)
{
	//�T�E���h�^�C�v���g�p����Ă��Ȃ������Ƃ�
	if (type == CSound::SOUND_NONE)
	{//�l��^�����ɕԂ�
		return S_OK;
	}

	else
	{
		//�T�E���h�̏��擾
		XAUDIO2_VOICE_STATE xa2state;

		//�T�E���h�̃o�b�t�@�̏��擾
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[type];
		buffer.pAudioData = m_apDataAudio[type];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = CSound::param[type].m_nCntLoop;

		// ��Ԏ擾
		m_apSourceVoice[type]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
			// �ꎞ��~
			m_apSourceVoice[type]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			m_apSourceVoice[type]->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[type]->SubmitSourceBuffer(&buffer);

		// �Đ�
		m_apSourceVoice[type]->Start(0);

		return S_OK;
	}
}

//--------------------------------------
//���ʐݒ�
//--------------------------------------
void CSound::SetVolume(SOUND_TYPE type, float fVolume)
{//1.�g�p����T�E���h�^�C�v���w�� 2.�ݒ肵�����l����
	m_apSourceVoice[type]->SetVolume(fVolume);
}

//--------------------------------------
//�s�b�`����
//--------------------------------------
void CSound::SetRate(SOUND_TYPE type, float rate)
{//1.�g�p����T�E���h�^�C�v���w�� 2.�ݒ肵�����l����
	m_apSourceVoice[type]->SetFrequencyRatio(rate);
}

//--------------------------------------
//�T�E���h��~
//--------------------------------------
void CSound::Stop(SOUND_TYPE type)
{
	//�T�E���h�̏����擾
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice[type]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_apSourceVoice[type]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[type]->FlushSourceBuffers();
	}
}

//--------------------------------------
//�S�ẴT�E���h��~
//--------------------------------------
void CSound::Stop()
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//--------------------------------------
//���̊m�F
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
//���̓ǂݎ��
//--------------------------------------
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

/*
���̖��_

�{�C�X�̊T�v

�\�[�X�{�C�X�͕�������B

�}�X�^�[�{�C�X�͂P�̂݁B


*/