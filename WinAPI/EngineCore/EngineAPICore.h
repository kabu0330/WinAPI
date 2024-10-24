#pragma once
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>

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
// 설정 : 창 크기 설정
class UEngineAPICore
{
public:
	// constructer destructer
	UEngineAPICore();
	~UEngineAPICore();

	// delete Function
	UEngineAPICore(const UEngineAPICore& _Other) = delete;
	UEngineAPICore(UEngineAPICore&& _Other) noexcept = delete;
	UEngineAPICore& operator=(const UEngineAPICore& _Other) = delete;
	UEngineAPICore& operator=(UEngineAPICore&& _Other) noexcept = delete;

	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	template<typename GameModeType, typename MainPawnType>
	ULevel* CreateLevel(std::string_view _LevelName)
	{
		ULevel* NewLevel = new ULevel();
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		Levels.insert({ _LevelName.data(), NewLevel });

		return NewLevel;
	}

	void OpenLevel(std::string_view _LevelName);

protected:

private:
	static void EngineBeginPlay();
	static void EngineTick();
	static UEngineAPICore* MainCore;
	static UContentsCore* UserCore;
	

	UEngineWindow EngineMainWindow;
	std::map<std::string, class ULevel*> Levels;

	class ULevel* CurLevel = nullptr;

	void Tick();
};

// 엔진의 기능을 컨텐츠에서 쓰고 싶다면
// 
// 1. 함수포인터
// 2. 가상함수
