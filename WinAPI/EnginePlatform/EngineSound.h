#pragma once
#include <EngineBase/EngineResources.h>
#include <map>

#include "ThirdParty/FMOD/inc/fmod.hpp"

// 사운드 플레이어
// 재생하는 사운드에 대한 볼륨, 온오프, 루프 등 객체로 컨텐츠 단계에서 관리하기 위해 생성해야하는 클래스
class USoundPlayer
{
public:
	friend class UEngineSound;

	void On()
	{
		if (nullptr == Control)
		{
			return;
		}
		Control->setPaused(false);
	}

	void Off()
	{
		if (nullptr == Control)
		{
			return;
		}
		Control->setPaused(true);
	}

	void OnOffSwtich()
	{
		if (nullptr == Control)
		{
			return;
		}
		bool Check = false;
		Control->getPaused(&Check);

		if (true == Check)
		{
			Control->setPaused(false);
		}
		else 
		{
			Control->setPaused(true);
		}
	}

	void Stop()
	{
		if (nullptr == Control)
		{
			return;
		}
		Control->stop();
		Control = nullptr;
	}

	void SetVolume(float _Volume)
	{
		if (nullptr == Control)
		{
			return;
		}
		_Volume = UEngineMath::Clamp(_Volume, 0.0f, 1.0f);

		Control->setVolume(_Volume);
	}

	bool IsPlaying()
	{
		if (nullptr == Control)
		{
			return true;
		}
		bool Check = true;
		Control->isPlaying(&Check);
		return Check;
	}

	void Loop(int Count = -1)
	{
		if (nullptr == Control)
		{
			return;
		}
		Control->setLoopCount(Count);
	}

	unsigned int LengthMs()
	{
		if (nullptr == SoundHandle)
		{
			return 0;
		}
		unsigned int ResultLength = 0;
		SoundHandle->getLength(&ResultLength, FMOD_TIMEUNIT_MS);
		return ResultLength;
	}

	void SetPosition(unsigned int _Value)
	{
		if (nullptr == Control)
		{
			return;
		}
		Control->setPosition(_Value, FMOD_TIMEUNIT_MS);
	}

	void ReStart()
	{
		SetPosition(0);
	}

private:
	// 채널 == 사운드 재생과 관련된 권한을 가진 클래스
	FMOD::Channel* Control = nullptr;
	FMOD::Sound* SoundHandle = nullptr;
};

// 설명 : 사운드 출력
class UEngineSound : public UEngineResources
{
public:
	UEngineSound();
	~UEngineSound();

	// 여러 사운드를 재생하고 특정 시점의 사운드 출력 등을 제어하기 위해서 Core단계에서 Tick 돌릴 때 업데이트를 같이 돌려줘야 한다.
	// 델타타임의 영향을 받아야 한다.

	static void Load(std::string_view _Path);
	static void Load(std::string_view _Name, std::string_view _Path);
	static USoundPlayer Play(std::string_view _Name);
	static UEngineSound* Find(std::string_view _Name);
	static void Release();

	static void Update();

	static void AllSoundStop();
	static void AllSoundOff();
	static void AllSoundOn();

protected:

private:
	static std::map<std::string, UEngineSound*> Sounds;
	static std::list<USoundPlayer> Players;

	FMOD::Sound* SoundHandle = nullptr;

	bool ResLoad(std::string_view _Path);

	// delete Function
	UEngineSound(const UEngineSound& _Other) = delete;
	UEngineSound(UEngineSound&& _Other) noexcept = delete;
	UEngineSound& operator=(const UEngineSound& _Other) = delete;
	UEngineSound& operator=(UEngineSound&& _Other) noexcept = delete;
};

