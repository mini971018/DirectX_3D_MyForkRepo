#pragma once
#include "GameEngineObject.h"
#include <GameEngineBase\GameEngineTimeEvent.h>
#include <string_view>
#include <map>
#include <GameEngineCore/GameEngineLight.h>
#include <GameEngineCore/GameEngineRenderTarget.h>

struct RGB
{
	float R;
	float G;
	float B;
};

struct PointLight
{
	RGB Color;
	float4 Position;
	float MaxDist;
	float Intensity;
};

struct AllPointLight
{
	int Num = 0;
	float4x4 ViewInverse;
	PointLight Lights[16];
};

// 설명 :
class GameEngineActor;
class GameEngineCamera;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineLevel : public GameEngineObject
{
	friend class GameEngineCamera;
	friend class GameEngineLight;
	friend class GameEngineRenderer;
	friend class GameEngineRenderUnit;
	friend class GameEngineCollision;
	friend class GameEngineTransform;
	friend class GameEngineCore;
	friend class GameEngineActor;
	friend class GameEngineTexture;

public:
	static void IsDebugSwitch()
	{
		IsDebugRender = !IsDebugRender;
	}
	bool GetDebugRender()
	{
		return IsDebugRender;
	}

	GameEngineTimeEvent TimeEvent;

	// constrcuter destructer
	GameEngineLevel();
	virtual ~GameEngineLevel();

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	template<typename ActorType>
	std::shared_ptr<ActorType> CreateActorToName(const std::string_view& _Name = "")
	{
		return CreateActor<ActorType>(0, _Name);
	}

	template<typename ActorType, typename EnumType>
	std::shared_ptr<ActorType> CreateActor(EnumType  _Order, const std::string_view& _Name = "")
	{
		return CreateActor<ActorType>(static_cast<int>(_Order), _Name);
	}

	template<typename ActorType >
	std::shared_ptr<ActorType> CreateActor(int _Order = 0, const std::string_view& _Name = "")
	{
		std::shared_ptr<GameEngineActor> NewActor = std::make_shared<ActorType>();

		std::string Name = _Name.data();

		if (_Name == "")
		{
			const type_info& Info = typeid(ActorType);
			Name = Info.name();
			Name.replace(0, 6, "");
		}

		ActorInit(NewActor, _Order, this);


		return std::dynamic_pointer_cast<ActorType>(NewActor);
	}

	std::shared_ptr<class GameEngineCamera> CreateNewCamera(int _Order);

	std::shared_ptr<class GameEngineCamera> GetMainCamera()
	{
		return MainCamera;
	}

	std::shared_ptr<GameEngineLevel> GetSharedThis()
	{
		return DynamicThis<GameEngineLevel>();
	}

	std::shared_ptr<GameEngineCamera> GetCamera(int _CameraOrder);

	std::shared_ptr<GameEngineRenderTarget> GetLastTarget()
	{
		return LastTarget;
	}

	// 일부러 무겁게 만든 함수.
	template<typename EnumType>
	std::list<std::shared_ptr<GameEngineActor>> GetActorGroup(EnumType _Index)
	{
		return GetActorGroup(static_cast<int>(_Index));
	}

	std::list<std::shared_ptr<GameEngineActor>> GetActorGroup(int _Index)
	{
		return Actors[_Index];
	}

	inline int GetLevelType()
	{
		return LevelType;
	}

	template<typename EnumType>
	inline EnumType GetLevelType()
	{
		return static_cast<EnumType>(LevelType);
	}

	std::list<std::shared_ptr<GameEngineLight>> GetAllLight()
	{
		return AllLight;
	}


	inline void SetWaterHeight(float _Height)
	{
		WaterHeight = _Height;
	}

	inline float GetWaterHeight()
	{
		return WaterHeight;
	}

protected:
	// 레벨이 바뀌어서 시작할때
	virtual void LevelChangeStart();
	virtual void LevelChangeEnd();

	virtual void Start() = 0;
	void Update(float _DeltaTime);
	void Render(float _DeltaTime);

	void AllActorDestroy();

	template<typename EnumType>
	inline void SetLevelType(EnumType _Type)
	{
		LevelType = static_cast<int>(_Type);
	}

	inline void SetLevelType(int _Type)
	{
		LevelType = _Type;
	}

private:
	float WaterHeight = -9999.0f;

	static bool IsDebugRender;
	int LevelType = -1;

	// 모든 카메라의 내용이 다 종합된.
	std::shared_ptr<GameEngineRenderTarget> LastTarget;


	//      이름           경로
	std::map<std::string, std::string> TexturePath;
	std::map<std::string, std::string> LoadEndPath;

	// 이미 뭔가가 다그려진 커다란 텍스처에 뭔가 변화를 주는것.

	// 카메라
	std::map<int, std::shared_ptr<GameEngineCamera>> Cameras;
	std::shared_ptr<GameEngineCamera> MainCamera;

	void PushCameraRenderer(std::shared_ptr<GameEngineRenderer> _Renderer, int _CameraOrder);

	// 빛의 영향 받는 여부만 존재
	std::list<std::shared_ptr<GameEngineLight>> AllLight;

	LightDatas LightDataObject;
	void PushLight(std::shared_ptr<GameEngineLight> _Light);



	std::map<int, std::list<std::shared_ptr<GameEngineActor>>> Actors;

	std::map<int, std::list<std::shared_ptr<GameEngineCollision>>> Collisions;

	void PushCollision(std::shared_ptr<GameEngineCollision> _Collision);

	void ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Level);

	void ActorUpdate(float _DeltaTime);
	void ActorRelease();
	void ActorLevelChangeStart();
	void ActorLevelChangeEnd();

	void LevelCameraInit();

	void DestroyCamera();


	void TextureUnLoad(GameEngineLevel* _NextLevel);

	void TextureReLoad(GameEngineLevel* _PrevLevel);

	// user >> server
	//void SendActorPacket(float _DeltaTime);

	void InitLevelRenderTarget();
	void ReleaseLevelRenderTarget();

public:

	//포인트 라이트는 일단 설치하고 나면, 값을 수정할 일이 없다고 가정할게요.

	void AddPointLight(const PointLight& _Light)
	{
		PointLights.Num++;
		PointLights.Lights[PointLights.Num - 1] = _Light;
	}

	AllPointLight PointLights;
};

