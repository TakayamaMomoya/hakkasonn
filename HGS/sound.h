//---------------------------------------------------------
//
//�T�E���h
//Author::TAKANO
//
//---------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

//--------------------------------------
//�C���N���[�h�t�@�C��
//--------------------------------------
#include"d3dx9.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include"xaudio2.h"
#include <xaudio2fx.h>

//--------------------------------------
//�N���X��`
//�T�E���h�֘A
//--------------------------------------
class CSound
{
public:
	struct PARAM
	{//�T�E���h�p�����[�^�[�̍\����
		char *m_pFilename;									// �t�@�C����
		int m_nCntLoop;										// ���[�v�J�E���g
	};

	//�T�E���h���g�p���鎞�̗�
	enum SOUND_TYPE
	{
		SOUND_NONE = -1,									//�g�p���Ȃ����
		SOUND_BGM_TITLE = 0,								//�^�C�g��
		SOUND_BGM_GAME,										//�Q�[�����Ŏg�p����
		SOUND_BGM_RESULT,									//���U���g�Ŏg�p����
		SOUND_SE_MASH_BOTTON,								//�{�^���A�ŉ�
		SOUND_SE_DORAMROOL,
		SOUND_SE_COUNTDOWN,
		SOUND_SE_START,
		SOUND_SE_SHINBAL,
		SOUND_MAX,
	};

	static const PARAM param[];								// �p�����[�^�̏��

	CSound();												// �R���X�g���N�^
	~CSound();												// �f�X�g���N�^
	HRESULT Init();											// ������
	void Uninit();											// �I��
	HRESULT Play(SOUND_TYPE type);							// �Đ�
	void Stop(SOUND_TYPE type);								// �ꕔ��~
	void Stop();											// �S��~

private:
	HRESULT CheckChunk(HANDLE hFile,						//���(�`�����N)�̊m�F
					   DWORD format, 
					   DWORD *pChunkSize, 
					   DWORD *pChunkDataPosition);			


	HRESULT ReadChunkData(HANDLE hFile,						//���(�`�����N)�̓ǂݍ���
						  void *pBuffer, 
						  DWORD dwBuffersize, 
						  DWORD dwBufferoffset);			

private:
	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_MAX];		// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_MAX];							// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_MAX];							// �I�[�f�B�I�f�[�^�T�C�Y
	int m_nCountFade;										// �t�F�[�h�p�J�E���^�[
};

#endif // !_SOUND_H_
