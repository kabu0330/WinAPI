#pragma once
#include <EngineBase/EngineResources.h>
#include <map>

#include "ThirdParty/FMOD/inc/fmod.hpp"

// ���� �÷��̾�
// ����ϴ� ���忡 ���� ����, �¿���, ���� �� ��ü�� ������ �ܰ迡�� �����ϱ� ���� �����ؾ��ϴ� Ŭ����
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
	// ä�� == ���� ����� ���õ� ������ ���� Ŭ����
	FMOD::Channel* Control = nullptr;
	FMOD::Sound* SoundHandle = nullptr;
};

// ���� : ���� ���
class UEngineSound : public UEngineResources
{
public:
	UEngineSound();
	~UEngineSound();

	// ���� ���带 ����ϰ� Ư�� ������ ���� ��� ���� �����ϱ� ���ؼ� Core�ܰ迡�� Tick ���� �� ������Ʈ�� ���� ������� �Ѵ�.
	// ��ŸŸ���� ������ �޾ƾ� �Ѵ�.

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

