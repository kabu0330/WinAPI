#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class ATitleGameMode : public AGameMode
{
public:
	ATitleGameMode();
	~ATitleGameMode();

	// delete Function
	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime);

	void SetupScene();
	void LevelChangeStart() override;

protected:

private:
	USoundPlayer IntroSound;
	USoundPlayer BGM;
	bool IsPlayingBGM = false;
};

