#pragma once

// ���� : ���� ���� ���� ���� �� ����
class EngineDelegate
{
public:
	EngineDelegate();
	
	// ������ ȣ���ϸ鼭 �ٷ� �Լ� �����͸� ���� �� �ִ�.
	EngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	~EngineDelegate();


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

	// �Լ� �����Ͱ� �ϳ��� ������ true, ������ false ��ȯ
	bool IsBind()
	{
		return false == Functions.empty();
	}

protected:

private:
	std::list<std::function<void()>> Functions;
};

