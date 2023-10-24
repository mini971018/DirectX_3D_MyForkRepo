#include "PreCompileHeader.h"
#include "MonsterAnimationTest.h"
#include "Boomerang.h"


MonsterAnimationTest::MonsterAnimationTest()
{
}

MonsterAnimationTest::~MonsterAnimationTest()
{
}

void MonsterAnimationTest::Start()
{
	Renderer = CreateComponent<ContentFBXRenderer>();
	Renderer->SetFBXMesh("FROG_MESH.FBX", "ContentAniMeshDeffered");
	Renderer->CreateFBXAnimation("0", "FROG_IDLE.fbx", {0.1f });
	Renderer->CreateFBXAnimation("1", "FROG_DAMEGED_LOOP.fbx", {0.1f });
	Renderer->CreateFBXAnimation("2", "FROG_IDLE.fbx", {0.1f });
	Renderer->CreateFBXAnimation("3", "FROG_JUMP.fbx", {0.1f });
	Renderer->CreateFBXAnimation("4", "FROG_POGO_BOUNCE.fbx", {0.1f });
	Renderer->CreateFBXAnimation("5", "FROG_POGO_END.fbx", {0.1f });
	Renderer->CreateFBXAnimation("6", "FROG_POGO_START.fbx", {0.1f });
	Renderer->CreateFBXAnimation("7", "FROG_SMASH_START.fbx", {0.1f });
	Renderer->CreateFBXAnimation("8", "FROG_SWIM.fbx", {0.1f });
	Renderer->CreateFBXAnimation("9", "FROG_SWIM_EDIT.fbx", {0.1f });




	//Renderer->SetFBXMesh("JUMPER_MESH.FBX", "ContentAniMeshDeffered");


	//Renderer->CreateFBXAnimation("0", "JUMPER_BOOMER_CATCH.fbx", {0.1f });
	//Renderer->SetAnimationStartFunc("0", 0, [this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);

	//		Renderer->SetRenderUnitControl(14, 0, true);
	//	});
	//Renderer->SetAnimationStartFunc("0", 28,[this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);

	//		Renderer->SetRenderUnitControl(10, 0, true);
	//	});

	//Renderer->CreateFBXAnimation("1", "JUMPER_BOOMER_PREP_WAIT.fbx");//
	//Renderer->CreateFBXAnimation("2", "JUMPER_BOOMER_THROW.fbx");//

	//Renderer->SetAnimationStartFunc("2", 0, [this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);

	//		Renderer->SetRenderUnitControl(10, 0, true);
	//	});
	//Renderer->SetAnimationStartFunc("2", 22, [this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);

	//		Renderer->SetRenderUnitControl(20, 0, true);
	//	});
	//Renderer->SetAnimationStartFunc("2", 53, [this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);

	//	});
	//Renderer->CreateFBXAnimation("3", "JUMPER_DROWN.fbx");//
	//Renderer->CreateFBXAnimation("4", "JUMPER_HOP.fbx");//
	//Renderer->CreateFBXAnimation("5", "JUMPER_HOP_LOOP.fbx");
	//Renderer->CreateFBXAnimation("6", "JUMPER_IDLE.fbx");//
	//Renderer->CreateFBXAnimation("7", "JUMPER_IDLE_LOOK.fbx");//
	//Renderer->CreateFBXAnimation("8", "JUMPER_INTERRUPT.fbx");//
	//Renderer->CreateFBXAnimation("9", "JUMPER_JUMP.fbx");//
	//Renderer->CreateFBXAnimation("10", "JUMPER_POISE_BREAK.fbx");//
	//Renderer->CreateFBXAnimation("11", "JUMPER_SKIP_THROW.fbx");//

	//Renderer->SetAnimationStartFunc("11", 0, [this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);

	//		Renderer->SetRenderUnitControl(20, 0, true);
	//	});
	//Renderer->SetAnimationStartFunc("11", 22, [this]
	//	{
	//		Renderer->SetRenderUnitControl(10, 0, false);
	//		Renderer->SetRenderUnitControl(14, 0, false);
	//		Renderer->SetRenderUnitControl(20, 0, false);
	//	});

	//Renderer->CreateFBXAnimation("12", "_E_JUMPER_THROW_Anim.fbx");//
	//Renderer->ChangeAnimation("12");
	GetTransform()->SetLocalScale(float4::ONE * 100.0f);
	GetTransform()->SetLocalRotation(float4{90.0f,180.0f,0.0f});
	//Renderer->GetTransform()->SetLocalRotation({ 0.0f,180.0f, 0.0f });

	if (false == GameEngineInput::IsKey("PressN"))
	{
		GameEngineInput::CreateKey("PressN", 'N');
	}
	if (false == GameEngineInput::IsKey("PressK"))
	{
		GameEngineInput::CreateKey("PressK", 'K');
	}

	Unit = Renderer->GetAllRenderUnit();
	//Unit[23][0]->Off();
	Renderer->ChangeAnimation("0");
	Unit[0][0]->Off();

	int a = 0;
	//Boomer = GetLevel()->CreateActor< Boomerang>();
	//Boomer->GetTransform()->SetParent(GetTransform());
	//Boomer->SetBoomer(Boomerang::BoomerType::HEAD, float4::ZERO, float4::ZERO);
}

float4 MonsterAnimationTest::GetBonePos(const std::string_view& _BoneName)
{
	AnimationBoneData Bone = Renderer->GetBoneData(_BoneName.data());

	std::shared_ptr< GameEngineComponent> BonePivot = CreateComponent< GameEngineComponent>();
	BonePivot->GetTransform()->SetParent(GetTransform());
	BonePivot->GetTransform()->SetLocalPosition(Bone.Pos);

	return BonePivot->GetTransform()->GetWorldPosition();
}


void MonsterAnimationTest::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("PressN"))
	{
		//Renderer->ChangeAnimation("5",true);
		Renderer->ChangeAnimation(std::to_string(index++));
		if (index >= 10)
		{
			index = 0;
		}
	}

	if (true == GameEngineInput::IsDown("PressK"))
	{
		Unit[mainindex++][0]->On();
		Unit[mainindex][0]->Off();


		int a = 0;

	}
	if (Boomer != nullptr)
	{
		//
		//float4 BonPos = Renderer->GetBoneData("HEAD").Pos;
		//BonPos.y += 0.3f;
		//float4 BoneRot = Renderer->GetBoneData("HEAD").RotEuler;
		////Boomer->SetBoomer(Boomerang::BoomerType::HEAD, BonPos, /*float4{-180.0f,90.0f,0.0f}*/BoneRot + float4{0.0f,0.0f});
		//Boomer->SetBoomer(Boomerang::BoomerType::HEAD, BonPos, BoneRot + float4{-180.0f,-90.0f,-90.0f});
		//
		//Boomer->GetTransform()->SetLocalPosition(BonPos);
		////Boomer->GetTransform()->SetWorldRotation(float4{ 90.0f,0.0f,0.0f });
		//
		//float4 Pos = Boomer->GetTransform()->GetWorldPosition();
		//float4 Scale = Boomer->GetTransform()->GetWorldScale();
	}

	Unit;


	
}
