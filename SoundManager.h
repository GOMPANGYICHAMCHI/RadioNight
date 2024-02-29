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

// 사운드 플레이어
// 2022.08.03 GOMP
// Woodiepie 사운드 플레이어 개량형
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

	
	void PauseBgm();		// 브금 일시정지
	void ResumeBgm();		// 브금 재생
	void PauseSft();		// 사운드 이펙트 일시정지
	void ResumeSft();		// 사운드 이펙트 재생

	void PauseAllSound();	// 전체 사운드 일시정지
	void ResumeAllSound();	// 전체 사운드 재생
	void StopAllSound();	// 전체 사운드 정지

	void Releasesound();	// 소멸자 호출

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
	// 사운드 전체 임포트
	void InitSound();

	// 배경음악
	Channel* soundchannel_bgm;
	unordered_map<string, Sound*> backgroundmusic;

	// 사운드 이펙트 
	Channel* soundchannel_sft[MAX_CHANNEL_SFT];
	unordered_map<string, Sound*> soundeffect;

	// 시스템
	System* system;
	FMOD_RESULT fmodresult;

	//볼륨
	float volumeamount_bgm;
	float volumeamount_sft;

	unsigned int temp_length;
};

