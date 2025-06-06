#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>

// 설명 : 이미지 로드를 관리하고 
class UImageManager
{
public:
	~UImageManager();

	static UImageManager& GetInst()
	{
		static UImageManager Inst;
		return Inst;
	}

	// 이미지를 map에 저장하는 함수
	void Load(std::string_view Path); 	// 파일명만으로 이미지를 로드해주는 편의성 함수
	void Load(std::string_view _KeyName, std::string_view Path); // 파일명과 경로를 넣어 이미지를 로드해주는 함수

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

	// delete Function
	UImageManager(const UImageManager& _Other) = delete;
	UImageManager(UImageManager&& _Other) noexcept = delete;
	UImageManager& operator=(const UImageManager& _Other) = delete;
	UImageManager& operator=(UImageManager&& _Other) noexcept = delete;
};

