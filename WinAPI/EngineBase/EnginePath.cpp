#include "PreCompile.h"
#include "EnginePath.h"
#include <EngineBase/EngineDebug.h>

UEnginePath::UEnginePath()
	: Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::string_view _Path)
	: Path(_Path)
{
}

UEnginePath::UEnginePath(std::filesystem::path _Path)
	: Path(_Path)
{
}

UEnginePath::~UEnginePath()
{
}

std::string UEnginePath::GetPathToString()
{
	return Path.string();
}


bool UEnginePath::IsExists()
{
	return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
	return false == IsDirectory();
}

bool UEnginePath::IsDirectory()
{
	return std::filesystem::is_directory(Path);
}

void UEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
	// 이런 경우에는 더미를 만드는게 좋다.
	UEnginePath DummyPath = UEnginePath(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로일때만 'Move Parent To Directory' 함수를 호출할 수 있습니다");
		return false;
	}

	bool Result = false;

	std::filesystem::path CurPath = DummyPath.Path;
	while (CurPath != CurPath.root_path())
	{
		CurPath = DummyPath.Path;
		CurPath.append(_Path);
		if (true == std::filesystem::exists(CurPath))
		{
			Result = true;
			Path = CurPath;
			break;
		}
		DummyPath.MoveParent();
	}

	return Result;
}
