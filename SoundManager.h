#pragma once
#define MAX_CHANNEL_SFT 20
#define MAX_CHANNEL_ALL 21

#ifdef _WIN64
#pragma comment(lib,"../Lib/fmod/lib/x64/fmod_vc.lib")
#pragma comment(lib,"../Lib/fmod/lib/x64/fmodL_vc.lib")
#else 
#pragma comment(lib,"../Lib/fmod/lib/x86/fmod_vc.lib")
#pragma comment(lib,"../Lib/fmod/lib/x86/fmodL_vc.lib")
#endif

//#include <fmod.h>

// FMOD
#include "../Lib/fmod/inc/fmod.h"
#include "../Lib/fmod/inc/fmod.hpp"
#include "../Lib/fmod/inc/fmod_dsp_effects.h"
#include "../Lib/fmod/inc/fmod_errors.h"
using namespace FMOD;

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// ���� �÷��̾�
// 2022.08.03 GOMP
// Woodiepie ���� �÷��̾� ������
class SoundManager
{
public:
	static SoundManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new SoundManager();
		}
		return instance;
	}

	FMOD::System* GetFmodSystem() const { return system; }

	void PlayBGM(string soundname);
	void PlayBGMLoop(string soundname);
	void PlaySFT(string soundname);
	bool IsBGMPlaying() {
		bool ret;
		soundchannel_bgm->isPlaying(&ret);
		return  ret;
	}

	unsigned int GetBgmTimeMS(string soundname);
	unsigned int GetSftTimeMS(string soundname);
	unsigned int GetBgmCurrentTimeMS();

	
	void PauseBgm();		// ��� �Ͻ�����
	void ResumeBgm();		// ��� ���
	void PauseSft();		// ���� ����Ʈ �Ͻ�����
	void ResumeSft();		// ���� ����Ʈ ���

	void PauseAllSound();	// ��ü ���� �Ͻ�����
	void ResumeAllSound();	// ��ü ���� ���
	void StopAllSound();	// ��ü ���� ����

	void Releasesound();	// �Ҹ��� ȣ��

	void VolumeUpBGM();		
	void VolumeDownBGM();
	void VolumeMuteBGM();
	void VolumeSetBGM();

	void VolumeUpSFT();
	void VolumeDownSFT();
	void VolumeMuteSFT();
	void VolumeSetSFT();

	float GetVolumeBGM() const { return volumeamount_bgm; }
	float GetVolumeSFT() const { return volumeamount_sft; }
	void SetVolumeBGM(float val) { volumeamount_bgm = val; }
	void SetVolumeSFT(float val) { volumeamount_sft = val; }

	SoundManager();
	~SoundManager();

	static SoundManager* instance;
private:
	// ���� ��ü ����Ʈ
	void InitSound();

	// �������
	Channel* soundchannel_bgm;
	unordered_map<string, Sound*> backgroundmusic;

	// ���� ����Ʈ 
	Channel* soundchannel_sft[MAX_CHANNEL_SFT];
	unordered_map<string, Sound*> soundeffect;

	// �ý���
	System* system;
	FMOD_RESULT fmodresult;

	//����
	float volumeamount_bgm;
	float volumeamount_sft;

	unsigned int temp_length;
};

