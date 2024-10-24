#pragma once

// Ό³Έν :
class ULevel
{
public:
	// constructer destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

protected:

private:
	class AGameMode* GameMode = nullptr;
};

