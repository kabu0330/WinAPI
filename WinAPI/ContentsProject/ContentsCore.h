#pragma once
#include <EngineCore/EngineAPICore.h>



// 설명 : 엔진 루프에 컨텐츠를 올려주는 클래스
class ContentsCore : public UContentsCore
{
public:
	// constructer destructer
	ContentsCore();
	~ContentsCore();

	// delete Function
	ContentsCore(const ContentsCore& _Other) = delete;
	ContentsCore(ContentsCore&& _Other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore& _Other) = delete;
	ContentsCore& operator=(ContentsCore&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick() override;

private:
	ULevel* TitleLevel = nullptr;
};

