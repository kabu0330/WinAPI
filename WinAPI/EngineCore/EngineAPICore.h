#pragma once
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/UEngineTimer.h>
#include <EngineBase/EngineString.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

#include "Level.h"

// 설명 : 하위 프로젝트에서 가상함수로 엔진에 요청
class UContentsCore
{
public:
	virtual void BeginPlay() = 0;
	virtual void Tick() = 0;
};

// 설명 : 게임 엔진
// 설정 : 윈도우 생성, 레벨 관리, 컨텐츠 Tick/Render 루프
class UEngineAPICore
{
public:
	UEngineAPICore();
	~UEngineAPICore();


	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	float GetDeltaTime()
	{
		return DeltaTimer.GetDeltaTime();
	}

	template<typename GameModeType, typename MainPawnType>
	ULevel* CreateLevel(std::string_view _LevelName)
	{
		std::string UpperName = UEngineString::ToUpper(_LevelName);

		if (false != Levels.contains(UpperName))
		{
			MSGASSERT("이미 존재하는 이름의 레벨을 다시 생성할 수 없습니다." + UpperName);
			return nullptr;
		}

		ULevel* NewLevel = new ULevel();

		// 게임 모드가 레벨의 특성을 설정하는 중요한 객체
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();
		NewLevel->SetName(UpperName);

		// 레벨을 string으로 저장하고 string으로 호출한다.
		Levels.insert({ UpperName, NewLevel });

		return NewLevel;

	}

	template<typename GameModeType, typename MainPawnType>
	void ResetLevel(std::string_view _LevelName)
	{
		std::string UpperName = UEngineString::ToUpper(_LevelName);

		// 지금 동작 중인 레벨 지우는거 아니지?
		if (CurLevel->GetName() != UpperName)
		{
			DestroyLevel(_LevelName); // 그럼 지워
			CreateLevel<GameModeType, MainPawnType>(UpperName);
			return;
		}
		
		// 현재 동작 중인 레벨을 지워야 한다면,
		std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(UpperName);
		Levels.erase(FindIter); // 현재 레벨을 Levels에서 지워
		NextLevel = CreateLevel<GameModeType, MainPawnType>(UpperName); // 그리고 NextLevel에서 똑같은 이름으로 다시 만들어
		IsCurLevelReset = true; // 아직 CurLevel은 살아있다. APICore에서 Tick이 한바퀴 돌고 난 뒤 지우기 위해 bool 변수를 만든다.
	}


	void DestroyLevel(std::string_view _LevelName)
	{
		std::string UpperName = UEngineString::ToUpper(_LevelName);

		if (false == Levels.contains(UpperName))
		{
			// MSGASSERT("존재하지 않는 레벨을 리셋할수 없습니다." + UpperName);
			return;
		}

		std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(UpperName);

		if (nullptr != FindIter->second)
		{
			delete FindIter->second;
			FindIter->second = nullptr;
		}

		Levels.erase(FindIter);
	}

	void OpenLevel(std::string_view _LevelName);

protected:

private:
	static void EngineBeginPlay();
	static void EngineTick();
	static UEngineAPICore* MainCore;
	static UContentsCore* UserCore;
	
	UEngineTimer  DeltaTimer       = UEngineTimer();
	UEngineWindow EngineMainWindow = UEngineWindow(); // 엔진 메인 윈도우

	// 만들어진 모든 레벨
	std::map<std::string, class ULevel*> Levels; 

	// 현재 내가 눈으로 보고 있어야 하는 레벨
	// 화면 전환시 포인터 체인지 방식으로 레벨 전환
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;
	bool IsCurLevelReset = false;

	void Tick();

	// delete Function
	UEngineAPICore(const UEngineAPICore& _Other) = delete;
	UEngineAPICore(UEngineAPICore&& _Other) noexcept = delete;
	UEngineAPICore& operator=(const UEngineAPICore& _Other) = delete;
	UEngineAPICore& operator=(UEngineAPICore&& _Other) noexcept = delete;
};

// 엔진의 기능을 컨텐츠에서 쓰고 싶다면
// 
// 1. 함수포인터
// 2. 가상함수
