#include "PrecompileHeader.h"
#include "Ladder.h"

#include "PhysXBoxComponent.h"

Ladder::Ladder()
{
}

Ladder::~Ladder()
{
}

void Ladder::Start()
{
	InitComponent();
}

void Ladder::Update(float _DeltaTime)
{
}

void Ladder::InitComponent()
{
	//GetTransform()->SetWorldPosition(float4{ -400, 100 , 0});

	// GetTransform()->SetLocalRotation(float4{ 0 , 90 , 0 });
	m_pRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pRenderer->SetFBXMesh("Ladder.fbx", "MeshTexture");

	float4 MeshScale = m_pRenderer->GetMeshScale();

	m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
	m_pPhysXComponent->SetPositionSetFromParentFlag(true);

	auto Test = m_pPhysXComponent->GetStatic()->getGlobalPose();

	int a = 0;
}
