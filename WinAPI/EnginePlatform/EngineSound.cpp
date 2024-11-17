#include "PreCompile.h"
#include "EngineSound.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>

std::map<std::string, UEngineSound*> UEngineSound::Sounds;

// 사운드를 제어하기위한 핸들
FMOD::System* SoundSystem = nullptr;

// 디버그와 릴리즈의 라이브러리가 다르다.
#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

class SoundInit
{
public:
	SoundInit()
	{
		if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
		{
			MSGASSERT("FMOD 시스템 초기화에 실패했습니다.");
			return;
		}

		// 사운드 채널설정
		// int maxchannels, 동시에 몇개까지 사운드 재생이 되는가?
		// FMOD_INITFLAGS flags, 지정사항이 있느냐? = 기본값
		// void* extradriverdata 지정사항에 대한 데이터를 넣어줄 것이 있느냐?

		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MSGASSERT("FMOD 시스템 이닛에 실패했습니다.");
			return;
		}
	}
};

SoundInit SoundInitObject;


void UEngineSound::Update()
{
	if (nullptr == SoundSystem)
	{
		return;
	}

	if (FMOD_RESULT::FMOD_OK != SoundSystem->update())
	{
		MSGASSERT("FMOD 시스템 업데이트에 이상이 감지되었습니다.");
	}
}

// 엔진이 끝날때 직접 호출
void UEngineSound::Release()
{
	std::map<std::string, UEngineSound*>::iterator StartIter = Sounds.begin();
	std::map<std::string, UEngineSound*>::iterator EndIter = Sounds.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	Sounds.clear();

	if (nullptr != SoundSystem)
	{
		SoundSystem->release();
		SoundSystem = nullptr;
	}
}

UEngineSound::UEngineSound()
{
}

UEngineSound::~UEngineSound()
{
	if (nullptr != SoundHandle)
	{
		SoundHandle->release();
		SoundHandle = nullptr;
	}
}

void UEngineSound::Load(std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileName();

	UEngineSound::Load(FileName.data(), _Path);
}

void UEngineSound::Load(std::string_view _Name, std::string_view _Path)
{
	// 이녀석은 UTF-8로 경로를 바꿔줘야 할수 있다.
	std::string UpperString = UEngineString::ToUpper(_Name);

	UEngineSound* NewSound = new UEngineSound();


	if (false == NewSound->ResLoad(_Path))
	{
		delete NewSound;
		MSGASSERT("사운드 로드에 실패했습니다" + UpperString);
		return;
	}
	;

	UEngineSound::Sounds.insert({ UpperString, NewSound });
	// Load(FileName, Path);
}

UEngineSound* UEngineSound::Find(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == Sounds.contains(UpperString))
	{
		return nullptr;
	}

	return Sounds[UpperString];
}

USoundPlayer UEngineSound::Play(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	UEngineSound* FindSound = Find(_Name);

	if (nullptr == FindSound)
	{
		MSGASSERT("로드하지 않은 사운드를 재생하려고 했습니다" + UpperString);
	}


	// 그냥 단순히 재생하는게 아니라면 채널을 얻어와야 속도나 피치, 볼륨 믹싱 등을 조절할수 있다.
	FMOD::Channel* Ch = nullptr;

	// FMOD의 특징 : 더블포인터(**)를 요구하면, 포인터의 주소를 주면 변경사항을 적용하여 수정한 데이터를 넣어 반환해준다.
	SoundSystem->playSound(FindSound->SoundHandle, nullptr, false, &Ch);

	// 0이면 1번만 재생, 즉 루프가 아니다.
	Ch->setLoopCount(0);

	// 볼륨 1로
	Ch->setVolume(1.0f);


	USoundPlayer NewPlayer;
	NewPlayer.Control = Ch;
	return NewPlayer;
}


bool UEngineSound::ResLoad(std::string_view _Path)
{
	// 인코딩 방식에 따라 UTF-8 또는 UTF로 바꿔줘야 할 수 있다.
	// 영문이라 그런가 멀티바이트 방식으로도 동작한다.
	SoundSystem->createSound(_Path.data(), FMOD_LOOP_NORMAL, nullptr, &SoundHandle);

	if (nullptr == SoundHandle)
	{
		MSGASSERT("사운드 로딩에 실패했습니다" + std::string(_Path));
		return false;
	}

	return true;
}