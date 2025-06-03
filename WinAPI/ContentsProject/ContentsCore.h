#pragma once
#include <EngineCore/EngineAPICore.h>

// ���� : ���� ������ �������� �÷��ִ� Ŭ����
class ContentsCore : public UContentsCore
{
public:
	ContentsCore();
	~ContentsCore();

protected:
	void BeginPlay() override;
	void Tick() override;
	void WindowSetting();
	void ResourceLoad();
	void SpriteSetting();
	void FolderSetting();
	void LevelSetting();

private:
	// delete Function
	ContentsCore(const ContentsCore& _Other) = delete;
	ContentsCore(ContentsCore&& _Other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore& _Other) = delete;
	ContentsCore& operator=(ContentsCore&& _Other) noexcept = delete;
};

