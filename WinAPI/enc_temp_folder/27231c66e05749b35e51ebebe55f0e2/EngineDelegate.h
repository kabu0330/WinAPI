#pragma once

// 설명 :
class EngineDelegate
{
public:
	// constrcuter destructer
	EngineDelegate();
	
	// 생성자 호출하면서 바로 함수 포인터를 넣을 수 있다.
	EngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	~EngineDelegate();

	// delete Function
	//EngineDelegate(const EngineDelegate& _Other) = delete;
	//EngineDelegate(EngineDelegate&& _Other) noexcept = delete;
	//EngineDelegate& operator=(const EngineDelegate& _Other) = delete;
	//EngineDelegate& operator=(EngineDelegate&& _Other) noexcept = delete;

	void operator()()
	{
		std::list<std::function<void()>>::iterator StartIter = Functions.begin();
		std::list<std::function<void()>>::iterator EndIter = Functions.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			std::function<void()> Function = *StartIter;
			Function();
		}
	}

	void operator+=(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	// 함수 포인터가 하나라도 있으면 true, 없으면 false 반환
	bool IsBind()
	{
		return false == Functions.empty();
	}

protected:

private:
	std::list<std::function<void()>> Functions;
};

