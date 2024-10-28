#pragma once

// Ό³Έν :
class EngineTimer
{
public:
	// constrcuter destructer
	EngineTimer();
	~EngineTimer();

	// delete Function
	EngineTimer(const EngineTimer& _Other) = delete;
	EngineTimer(EngineTimer&& _Other) noexcept = delete;
	EngineTimer& operator=(const EngineTimer& _Other) = delete;
	EngineTimer& operator=(EngineTimer&& _Other) noexcept = delete;

protected:

private:

};

