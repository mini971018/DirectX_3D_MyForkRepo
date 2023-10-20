#include "PreCompileHeader.h"
#include "EnemyFirePlant.h"
#include "PhysXCapsuleComponent.h"
#include "EnemyAttackSphere.h"

EnemyFirePlant::EnemyFirePlant()
{
}

EnemyFirePlant::~EnemyFirePlant()
{
}

void EnemyFirePlant::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_FIREPLANT_MESH.FBX", "ContentAniMeshDeffered");

	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_FIREPLANT_IDLE.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("BITE", "_E_FIREPLANT_BITE.fbx", { 0.02f,false });
	EnemyRenderer->SetAnimationStartFunc("BITE", 30, [this]
		{
			// 본 위치 가져오기
			std::shared_ptr<GameEngineComponent> BonePivot = CreateComponent< GameEngineComponent>();
			BonePivot->GetTransform()->SetParent(GetTransform());
			BonePivot->GetTransform()->SetLocalPosition(EnemyRenderer->GetBoneData("Spine_010").Pos);
			float4 BonePivotPos = BonePivot->GetTransform()->GetWorldPosition();

			std::shared_ptr<EnemyAttackSphere> Attack = GetLevel()->CreateActor<EnemyAttackSphere>();
			Attack->SetRender(FIREPLANT_ATT_RENDER_SCALE);
			Attack->SetPhysXComp(FIREPLANT_ATT_PHYSX_SCALE, float4::DOWN * 100.0f);
			Attack->SetTrans(ShootDir, BonePivotPos);// 위치와 방향설정
			Attack->SetShoot(1000.0f);
			BonePivot->Death();
			
		});
	EnemyRenderer->CreateFBXAnimation("HIT", "_E_FIREPLANT_HIT.fbx", { 0.04f,false });
	EnemyRenderer->CreateFBXAnimation("DIE", "_E_FIREPLANT_DIE.fbx", { 0.02f,false });
	EnemyRenderer->ChangeAnimation("IDLE");
}




void EnemyFirePlant::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_FIREPLANT); // 임시 값조정 필요
	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXControllerComponent>();
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_FIREPLANT, DEFAULT_DIR_FIREPLANT); // static으로 생성
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic, PhysXFilterGroup::PlayerSkill);
	}
	SetFSMFUNC();

}


void EnemyFirePlant::Update(float _DeltaTime)
{

	FSMObjectBase::Update(_DeltaTime);
	
}


void EnemyFirePlant::SetFSMFUNC()
{
	InitFSM(EnemyFireFlowerState::MAX);

	SetChangeFSMCallBack([this]
		{

		});
	SetFSM(EnemyFireFlowerState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyFireFlowerState::HIT,true);
				return;
			}
			if (true == InRangePlayer(1000.0f))
			{
				SetNextState(EnemyFireFlowerState::BITE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyFireFlowerState::BITE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("BITE");
			GetTransform()->SetLocalRotation(GetRotationDegree(DEFAULT_DIR_FIREPLANT));
			ShootDir = GetPlayerDir();
 			//AggroDir(m_pCapsuleComp, DEFAULT_DIR_FIREPLANT);
			// fire 투사체 발사 
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyFireFlowerState::HIT);
				return;
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyFireFlowerState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(EnemyFireFlowerState::HIT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("HIT");
			AddPlayerSpellCost();
		},
		[this](float Delta)
		{
			if (true == CheckCollision(PhysXFilterGroup::PlayerSkill))
			{
				SetNextState(EnemyFireFlowerState::HIT, true);
				return;
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyFireFlowerState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyFireFlowerState::DIE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("DIE");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				// Die
				return;
			}
		},
		[this]
		{
			
		}
	);


}