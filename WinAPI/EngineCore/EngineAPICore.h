#pragma once
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

// 설명 : 하위 프로젝트에서 엔진에 접근하여 
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
	// constrcuter destructer
	UEngineAPICore();
	~UEngineAPICore();

	// delete Function
	UEngineAPICore(const UEngineAPICore& _Other) = delete;
	UEngineAPICore(UEngineAPICore&& _Other) noexcept = delete;
	UEngineAPICore& operator=(const UEngineAPICore& _Other) = delete;
	UEngineAPICore& operator=(UEngineAPICore&& _Other) noexcept = delete;

	static int EngineStart(HINSTANCE _Inst);

	class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

protected:

private:
	static void EngineLoop();
	
	static UEngineAPICore* MainCore;

	UEngineWindow EngineMainWindow;
	
	void Tick();
	void Render();

};

// 엔진의 기능을 컨텐츠에서 쓰고 싶다면
// 
// 1. 함수포인터
// 2. 가상함수
