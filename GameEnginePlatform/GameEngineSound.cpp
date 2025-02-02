#include "PrecompileHeader.h"
#include "GameEngineSound.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineRandom.h>

// 다른 lib나 dll을 사용하기 위한 전처리문을 여기 넣을것입니다.
// #pragma comment(lib, "GameEngineBase.lib");

// lib를 통해서 dll을 사용한다고 하는건.
// fmod.dll을 쓰겠다.
#ifdef _DEBUG
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmodL_vc.lib")
#else
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmod_vc.lib")
#endif

//////////////////////////// GameEngineSoundPlayer ////////////////////////////

GameEngineSoundPlayer::GameEngineSoundPlayer(FMOD::Channel* _Channel)
	: Channel(_Channel)
{
}

void GameEngineSoundPlayer::SoundFadeIn(double _Time, float _Volume)
{
	if (false == IsValid())
	{
		MsgAssert("유효하지 않은 사운드를 페이드-인 하려 했습니다.");
		return;
	}

	int Rate = 0;
	unsigned long long ParentClock = 0u;

	FMOD::System* SoundSys = nullptr;
	Channel->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	Channel->getDSPClock(nullptr, &ParentClock);

	unsigned long long EndClock = static_cast<unsigned long long>(ParentClock + (Rate * _Time));

	Channel->removeFadePoints(0, INT64_MAX);
	Channel->addFadePoint(ParentClock, 0.0f);
	Channel->addFadePoint(EndClock, _Volume);
	Channel->setDelay(0, 0, false);
	Channel->setPaused(false);
}


void GameEngineSound::SoundFadeInGroup(double _Time, float _Volume)
{
	//if (false == IsValid())
	//{
	//	MsgAssert("유효하지 않은 사운드를 페이드-인 하려 했습니다.");
	//	return;
	//}

	int Rate = 0;
	unsigned long long ParentClock = 0u;

	FMOD::System* SoundSys = nullptr;
	ChannelGroup->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	ChannelGroup->getDSPClock(nullptr, &ParentClock);

	unsigned long long EndClock = static_cast<unsigned long long>(ParentClock + (Rate * _Time));

	ChannelGroup->removeFadePoints(0, INT64_MAX);
	ChannelGroup->addFadePoint(ParentClock, 0.0f);
	ChannelGroup->addFadePoint(EndClock, _Volume);
	ChannelGroup->setDelay(0, 0, false);
	ChannelGroup->setPaused(false);
}

void GameEngineSoundPlayer::SoundFadeOut(double _Time, float _Volume, bool _IsStop /*= false*/)
{
	if (false == IsValid())
	{
		MsgAssert("유효하지 않은 사운드를 페이드-아웃 하려 했습니다.");
		return;
	}

	int Rate = 0;
	unsigned long long ParentClock = 0u;
	float CurVolume = 0.0f;

	Channel->setLoopCount(1);
	FMOD::System* SoundSys = nullptr;
	Channel->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	Channel->getDSPClock(nullptr, &ParentClock);
	Channel->getVolume(&CurVolume);

	unsigned long long EndClock = static_cast<unsigned long long>(ParentClock + (Rate * _Time));

	Channel->removeFadePoints(0, INT64_MAX);
	Channel->addFadePoint(ParentClock, CurVolume);
	Channel->addFadePoint(EndClock, _Volume);
	Channel->setDelay(ParentClock, EndClock, _IsStop);
}

//////////////////////////// GameEngineSound ////////////////////////////

// FMOD는 자신들의 기능을 이용할수 있게 해주는 클래스의 포인터를 주고
FMOD::System* SoundSystem = nullptr;

std::unordered_map<std::string, std::shared_ptr<GameEngineSound>> GameEngineSound::AllSound;

class SoundSystemCreator
{
public:
	SoundSystemCreator()
	{
		FMOD::System_Create(&SoundSystem);

		if (nullptr == SoundSystem)
		{
			MsgAssert("사운드 초기화가 제대로 되지 않았습니다.");
		}

		if (FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgAssert("사운드 시스템 이니셜라이즈에 실패했습니다.");
		}

		SoundSystem->createChannelGroup("SoundGroup", &GameEngineSound::ChannelGroup);
	}

	~SoundSystemCreator()
	{
		SoundSystem->release();
	}
};

SoundSystemCreator SoundInitObject = SoundSystemCreator();
FMOD::ChannelGroup* GameEngineSound::ChannelGroup = nullptr;

void GameEngineSound::SoundUpdate()
{
	if (nullptr == SoundSystem)
	{
		MsgAssert("사운드 업데이트가 불가능합니다.");
		return;
	}

	SoundSystem->update();
}

GameEngineSound::GameEngineSound()
{
}

GameEngineSound::~GameEngineSound()
{
	if (nullptr == FMODSound)
	{
		FMODSound->release();
	}
}

void GameEngineSound::Load(const std::string_view& _Path)
{
	GameEnginePath NewPath(_Path);
	return Load(NewPath.GetFileName(), _Path);
}

void GameEngineSound::Load(const std::string_view& _Name, const std::string_view& _Path)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);
	std::shared_ptr<GameEngineSound> NewSound = std::make_shared<GameEngineSound>();
	NewSound->SoundLoad(_Path);
	AllSound.insert(std::pair<std::string, std::shared_ptr<GameEngineSound>>(UpperName, NewSound));
}

GameEngineSoundPlayer GameEngineSound::Play(const std::string_view& _Name, bool _Group/* = true)*/)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	std::unordered_map<std::string, std::shared_ptr<GameEngineSound>>::iterator Finditer = AllSound.find(UpperName);

	if (Finditer == AllSound.end())
	{
		MsgAssert("존재하지 않는 사운드를 플레이하려고 했습니다.");
		return nullptr;
	}
	FMOD::Channel* Channel = Finditer->second->SoundPlay();
	Channel->setLoopCount(0);
	if (_Group == true)
	{
		Channel->setChannelGroup(GameEngineSound::ChannelGroup);
	}
	return Channel;
}

GameEngineSoundPlayer GameEngineSound::Play(const std::vector<std::string>& PlayList, bool _Group/* = true*/)
{
	if (true == PlayList.empty())
	{
		MsgAssert("플레이 리스트가 존재하지 않습니다.");
		return nullptr;
	}
	int index = GameEngineRandom::MainRandom.RandomInt(0, static_cast<int>(PlayList.size()-1));
	return Play(PlayList[index], _Group);
}


void GameEngineSound::SoundLoad(const std::string_view& _Path)
{
	std::string UTF8Path = GameEngineString::AnsiToUTF8(_Path);

	if (FMOD_OK != SoundSystem->createSound(UTF8Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &FMODSound))
	{
		MsgAssert(std::string("사운드 로드에 실패했습니다.\n 경로 : ") + _Path.data());
	}

	return;
}


FMOD::Channel* GameEngineSound::SoundPlay()
{
	if (nullptr == FMODSound)
	{
		MsgAssert(std::string("FMODSound가 nullptr 입니다 "));
	}

	FMOD::Channel* Return;

	SoundSystem->playSound(FMODSound, nullptr, false, &Return);

	return Return;
}

void GameEngineSound::ResourcesClear()
{
	AllSound.clear();
}