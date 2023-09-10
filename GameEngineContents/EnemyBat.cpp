#include "PreCompileHeader.h"
#include "EnemyBat.h"
#include "PhysXCapsuleComponent.h"



EnemyBat::EnemyBat()
{
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::InitAniamtion()
{
	BatRender = CreateComponent<GameEngineFBXRenderer>();
	BatRender->SetFBXMesh("_E_BAT_Black Variant_MESH.FBX", "MeshAniTexture");

	BatRender->CreateFBXAnimation("IDLE", "_E_BAT_Black Variant_IDLE.fbx", { 0.02f,true });
	BatRender->CreateFBXAnimation("FLY", "_E_BAT_Black Variant_FLY.fbx", { 0.02f,false });
	BatRender->CreateFBXAnimation("BITE", "_E_BAT_Black Variant_BITE.fbx", { 0.02f,false });
	BatRender->CreateFBXAnimation("IDLE_FLOOR", "_E_BAT_Black Variant_IDLE_FLOOR.fbx", { 0.02f,true });
	BatRender->CreateFBXAnimation("SHOCK", "_E_BAT_Black Variant_SHOCK.fbx", { 0.02f,false });
	BatRender->ChangeAnimation("IDLE_FLOOR");
}

void EnemyBat::Start()
{
	EnemyBase::Start();

	// physx
	{
		float4 scale = BatRender->GetMeshScale() * BatRender->GetTransform()->GetWorldScale() / BatRender->GetTransform()->GetLocalScale();
		// scale *= 2.0f;
		physx::PxVec3 vscale = physx::PxVec3(scale.x, scale.y, scale.z);
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(vscale);
	}
	SetFSMFUNC();
}

void EnemyBat::Update(float _DeltaTime)
{
}


void EnemyBat::SetFSMFUNC()
{
	InitFSM(EnemyBatState::MAX);

	SetChangeFSMCallBack([this]
		{

		});



	SetFSM(EnemyBatState::IDLE,
		[this]
		{
			BatRender->ChangeAnimation("IDLE");

		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBatState::FLY,
		[this]
		{
			BatRender->ChangeAnimation("FLY");

		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBatState::BITE,
		[this]
		{
			BatRender->ChangeAnimation("BITE");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBatState::IDLE_FLOOR,
		[this]
		{
			BatRender->ChangeAnimation("IDLE_FLOOR");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBatState::SHOCK,
		[this]
		{
			BatRender->ChangeAnimation("SHOCK");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}
