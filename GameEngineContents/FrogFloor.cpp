#include "PrecompileHeader.h"
#include "FrogFloor.h"

#include "SecretTile.h"

#include "ContentFBXRenderer.h"
#include "PhysXBoxComponent.h"

#include "FireObject.h"
#include "RuinsWall.h"
#include "Ladder.h"

FrogFloor* FrogFloor::MainFloor = nullptr;

FrogFloor::FrogFloor()
{
	MainFloor = this;
}

FrogFloor::~FrogFloor()
{
}



void FrogFloor::Start()
{
	InitComponent();
}

void FrogFloor::Update(float _DeltaTime)
{
}

void FrogFloor::InitComponent()
{
	GetTransform()->SetLocalPosition(float4{ -3621, -197, 3700 });

	// 테두리 렌더러 생성
	m_pHingeRenderer = CreateComponent<ContentFBXRenderer>();
	m_pHingeRenderer->SetFBXMesh("Hinge.fbx", "ContentMeshDeffered");

	// 필요한 오브젝트 만들고
	Create_TileObject();
	Create_FireObject();
	
	// 사다리 한개 
	std::shared_ptr<Ladder> Obj = GetLevel()->CreateActor<Ladder>();
	Obj->GetTransform()->SetLocalRotation(float4{ 0, 50, 0 });
	Obj->GetTransform()->SetLocalPosition(float4{ -4495, -105, 5390 });
	

	// 회전 
	GetTransform()->SetLocalRotation(float4{ 0 , -40, 0 });
	
	Create_WallObject();
}

void FrogFloor::Create_FireObject()
{
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("현재 레벨이 nullptr 입니다.");
		return;
	}

	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -3400, 167, 2420 });
	}
	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -4770, 167, 4040 });
	}
	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -3150, 167, 5370 });
	}
	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -1790, 167, 3760 });
	}
}

void FrogFloor::Create_TileObject()
{
	float4 TileStartPos = m_TileInitPos;

	float PosX = m_fTileMovePos;
	float PosZ = m_fTileMovePos;

	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("현재 레벨이 nullptr 입니다.");
		return;
	}

	// 초기위치 
	float4 StartPos = m_TileInitPos;
	float4 MoveXPos = float4::ZERONULL;
	float4 MoveZPos = float4::ZERONULL;

	m_vTiles.resize(m_iTileSize);

	// 돌면서 5x5 타일 깔아 
	for (size_t i = 0; i < m_Height; ++i)
	{
		for (size_t j = 0; j < m_Width; ++j)
		{
			// 타일만들고
			std::shared_ptr<SecretTile> NewTile = CurLevel->CreateActor<SecretTile>();
			NewTile->GetTransform()->SetParent(GetTransform());

			// 위치세팅하고 
			NewTile->GetTransform()->SetLocalPosition(TileStartPos + MoveXPos + MoveZPos);
			NewTile->GetTransform()->SetLocalPosition(NewTile->GetTransform()->GetLocalPosition() + float4 { 0, -40, 0 });
			

			// 벡터에 넣어
			m_vTiles.push_back(NewTile);

			MoveXPos += float4{ PosX, 0, 0 };
		}

		MoveXPos = float4::ZERONULL;
		MoveZPos -= float4{ 0, 0, PosZ };
	}
}

void FrogFloor::Create_WallObject()
{
	m_vWalls.resize(4);

	GameEngineLevel* CurLevel = GetLevel();

	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalRotation(float4{ 0, -40, 0 });
		Obj->GetTransform()->SetLocalPosition(float4{ -4076, -322 , 3239 });
		Obj->GetTransform()->SetParent(GetTransform());
	}

	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalRotation(float4{ 0, -40, 0 });
		Obj->GetTransform()->SetLocalPosition(float4{ -2500, -322, 4610 });
		Obj->GetTransform()->SetParent(GetTransform());
	}

	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalRotation(float4{ 0, 50, 0 });
		Obj->GetTransform()->SetLocalPosition(float4{ -2580, -322, 3120 });
		Obj->GetTransform()->SetParent(GetTransform());
	}

	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalRotation(float4{ 0, 50, 0 });
		Obj->GetTransform()->SetLocalPosition(float4{ -3920, -322, 4770 });
		Obj->GetTransform()->SetParent(GetTransform());
	}
}

std::shared_ptr<SecretTile> FrogFloor::GetTile(const int _TileIndex)
{
	int TileIndex = _TileIndex;
	size_t VectorSize = m_vTiles.size();

	if (TileIndex < 0 || TileIndex >= static_cast<int>(VectorSize))
	{
		MsgAssert("잘못된 타일 인덱스를 입력하였습니다.");
		return nullptr;
	}

	std::shared_ptr<SecretTile> Tile = m_vTiles[_TileIndex];
	
	if (Tile == nullptr)
	{
		MsgAssert("현재 타일이 nullptr 입니다. 맵한테 따져주세요. ㅈㅅ");
		return nullptr;
	}

	return Tile;
}