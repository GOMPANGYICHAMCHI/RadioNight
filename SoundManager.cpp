#include "framework.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager()
{
	InitSound();
}

SoundManager::~SoundManager()
{
	Releasesound();
}

void SoundManager::PlayBGM(string sounname)
{
	// 이전 음악 중지
	StopAllSound();

	// 지정한 사운드 출력
	GetFmodSystem()->playSound(backgroundmusic[sounname], 0, false, &soundchannel_bgm);
	soundchannel_bgm->setVolume(volumeamount_bgm);

}

void SoundManager::PlayBGMLoop(string sounname)
{
	bool playing = false;

	// 지금 배경음악 체널이 재생중인지 확인
	soundchannel_bgm->isPlaying(&playing);

	// 재생중이 아니라면 노래 재생 
	if (!playing)
	{
		GetFmodSystem()->playSound(backgroundmusic[sounname], 0, false, &soundchannel_bgm);
		soundchannel_bgm->setVolume(volumeamount_bgm);
	}
}

void SoundManager::PlaySFT(string sounname)
{
	bool playing = false;

	// 재생중이지 않은 채널 탐색
	for (int i = 0; i < MAX_CHANNEL_SFT; i++)
	{
		soundchannel_sft[i]->isPlaying(&playing);
		if (!playing)
		{
			GetFmodSystem()->playSound(soundeffect[sounname], 0, false, &soundchannel_sft[i]);
			soundchannel_sft[i]->setVolume(volumeamount_sft);

			break;
		}
	}
}

unsigned int SoundManager::GetBgmTimeMS(string soundname)
{
	backgroundmusic[soundname]->getLength(&temp_length, FMOD_TIMEUNIT_MS);

	return temp_length;
}

unsigned int SoundManager::GetSftTimeMS(string soundname)
{
	soundeffect[soundname]->getLength(&temp_length, FMOD_TIMEUNIT_MS);

	return temp_length;
}

unsigned int SoundManager::GetBgmCurrentTimeMS()
{
	soundchannel_bgm->getPosition(&temp_length, FMOD_TIMEUNIT_MS);

	return temp_length;
}

void SoundManager::PauseBgm()
{
	soundchannel_bgm->setPaused(true);
}

void SoundManager::ResumeBgm()
{
	soundchannel_bgm->setPaused(false);
}

void SoundManager::PauseSft()
{
	for (auto temp : soundchannel_sft)
	{
		temp->setPaused(true);
	}
}

void SoundManager::ResumeSft()
{
	for (auto temp : soundchannel_sft)
	{
		temp->setPaused(false);
	}
}

void SoundManager::PauseAllSound()
{
	soundchannel_bgm->setPaused(true);

	for (auto temp : soundchannel_sft)
	{
		temp->setPaused(true);
	}
}

void SoundManager::ResumeAllSound()
{
	soundchannel_bgm->setPaused(false);

	for (auto temp : soundchannel_sft)
	{
		temp->setPaused(false);
	}
}

void SoundManager::StopAllSound()
{
	soundchannel_bgm->stop();

	for (int i = 0; i < MAX_CHANNEL_SFT; i++)
	{
		soundchannel_sft[i]->stop();
	}
}

void SoundManager::Releasesound()
{
	GetFmodSystem()->release();
}

void SoundManager::VolumeUpBGM()
{
	volumeamount_bgm += 0.1f;
	if (volumeamount_bgm >= 1.0)
	{
		volumeamount_bgm = 1.0;
	}
	VolumeSetBGM();
}

void SoundManager::VolumeDownBGM()
{
	volumeamount_bgm -= 0.1f;
	if (volumeamount_bgm <= 0)
	{
		volumeamount_bgm = 0;
	}
	VolumeSetBGM();
}

void SoundManager::VolumeMuteBGM()
{
	volumeamount_bgm = 0;
	VolumeSetBGM();
}

void SoundManager::VolumeSetBGM()
{
	soundchannel_bgm->setVolume(volumeamount_bgm);
	for (int i = 0; i < MAX_CHANNEL_SFT; i++)
	{
		soundchannel_sft[i]->setVolume(volumeamount_bgm);
	}
}

void SoundManager::VolumeUpSFT()
{
	volumeamount_sft += 0.1f;
	if (volumeamount_sft >= 1.0)
	{
		volumeamount_sft = 1.0;
	}
	VolumeSetSFT();
}

void SoundManager::VolumeDownSFT()
{
	volumeamount_sft -= 0.1f;
	if (volumeamount_sft <= 0)
	{
		volumeamount_sft = 0;
	}
	VolumeSetSFT();
}

void SoundManager::VolumeMuteSFT()
{
	volumeamount_sft = 0;
	VolumeSetSFT();
}

void SoundManager::VolumeSetSFT()
{
	for (int i = 0; i < MAX_CHANNEL_SFT; i++)
	{
		soundchannel_sft[i]->setVolume(volumeamount_sft);
	}
}

void SoundManager::InitSound()
{
	fmodresult = System_Create(&system);
	fmodresult = GetFmodSystem()->init(MAX_CHANNEL_ALL, FMOD_INIT_NORMAL, 0);

	

	char str[128];
	Sound* temp;

	volumeamount_bgm = 0.5f;
	volumeamount_sft = 0.2f;

	//============================================================================
	// 인게임 사운드
	//============================================================================

	// 공격1 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Attack1.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Attack1", temp));

	// 회피 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Dodge.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Dodge", temp));

	// 약실 빔 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Empty.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Empty", temp));

	// 실패 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Fail.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Fail", temp));

	// 피버 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Fever.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Fever", temp));

	// 총기 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Gunfire.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Gunfire", temp));

	// 몬스터 사망 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Monster_dead.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Monster_dead", temp));
	
	// 장전1 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Reload1.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Reload1", temp));
	
	// 장전2 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Reload2.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Reload2", temp));

	// 타이핑 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Typing.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Typing", temp));

	// 철조망 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/WireDoor.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("WireDoor", temp));

	// 타격 사운드
	sprintf_s(str, "4_Resources/Sound/EFFECT/Hit.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Hit", temp));

	//============================================================================
	// UI 사운드
	//============================================================================

	// 버튼 클릭 사운드
	sprintf_s(str, "4_Resources/Sound/UI/UI_mouse_click.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("UI_mouse_click", temp));

	// 버튼 호버 사운드
	sprintf_s(str, "4_Resources/Sound/UI/UI_mouse_on.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("UI_mouse_on", temp));
	
	// 팝업1 사운드
	sprintf_s(str, "4_Resources/Sound/UI/Option_popup.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Option_popup", temp));
	
	// 팝업1 사운드
	sprintf_s(str, "4_Resources/Sound/UI/Option_mouse_click.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Option_mouse_click", temp));

	// 랭크 사운드
	sprintf_s(str, "4_Resources/Sound/UI/Rank.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	soundeffect.insert(make_pair("Rank", temp));

	//============================================================================
	// 브금
	//============================================================================

	// 메인 브금
	sprintf_s(str, "4_Resources/Sound/BGM/Ingame1.mp3");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	backgroundmusic.insert(make_pair("bgm_1", temp));

	// 사망 브금
	sprintf_s(str, "4_Resources/Sound/BGM/Dead.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	backgroundmusic.insert(make_pair("Dead", temp));

	// 타이틀 브금
	sprintf_s(str, "4_Resources/Sound/BGM/Title.wav");
	GetFmodSystem()->createSound(str, FMOD_LOOP_OFF, 0, &temp);
	backgroundmusic.insert(make_pair("Title", temp));
}