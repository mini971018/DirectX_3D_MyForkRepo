#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class Map_Office : public GameEngineActor
{
public:
	// constrcuter destructer
	Map_Office();
	~Map_Office();

	// delete Function
	Map_Office(const Map_Office& _Other) = delete;
	Map_Office(Map_Office&& _Other) noexcept = delete;
	Map_Office& operator=(const Map_Office& _Other) = delete;
	Map_Office& operator=(Map_Office&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();

	std::shared_ptr<class PhysXTriangleComponent> m_pTriangleComp = nullptr;
	std::shared_ptr<GameEngineFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<GameEngineFBXRenderer> m_pNaviRenderer = nullptr;

	const float4 m_MapRot = float4{ 0 , -135 , 0 };
	const float4 m_MapPos = float4{ -560, -5700, 170 };
};