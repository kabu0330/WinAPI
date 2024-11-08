#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>

// ���� : �̹��� �ε带 �����ϰ� 
class UImageManager
{
public:
	// constrcuter destructer
	~UImageManager();

	// delete Function
	UImageManager(const UImageManager& _Other) = delete;
	UImageManager(UImageManager&& _Other) noexcept = delete;
	UImageManager& operator=(const UImageManager& _Other) = delete;
	UImageManager& operator=(UImageManager&& _Other) noexcept = delete;

	static UImageManager& GetInst()
	{
		static UImageManager Inst;
		return Inst;
	}

	// �̹����� map�� �����ϴ� �Լ�
	void Load(std::string_view Path); 	// ���ϸ����� �̹����� �ε����ִ� ���Ǽ� �Լ�
	void Load(std::string_view _KeyName, std::string_view Path); // ���ϸ�� ��θ� �־� �̹����� �ε����ִ� �Լ�

	void LoadFolder(std::string_view Path);
	void LoadFolder(std::string_view _KeyName, std::string_view _Path);

	void CuttingSprite(std::string_view _KeyName, int _X, int _Y);
	void CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize);
	void CuttingSprite(std::string_view _NewSpriteName, std::string_view _Image, FVector2D _CuttingSize);
	void CreateCutSprite(std::string_view _SearchKeyName, std::string_view _NewSpriteKeyName, FVector2D _StartPos, FVector2D _CuttingSize, FVector2D _XYOffSet, UINT _Xcount, UINT _ImageCount);



	bool IsLoadSprite(std::string_view _KeyName);

	class UEngineSprite* FindSprite(std::string_view _KeyName);
	class UEngineWinImage* FindImage(std::string_view _KeyName);

protected:

private:
	UImageManager();

	std::map<std::string, class UEngineWinImage*> Images;
	std::map<std::string, class UEngineSprite*> Sprites;

};

