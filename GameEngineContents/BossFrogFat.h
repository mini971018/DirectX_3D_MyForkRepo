#pragma once
#include "BossFrog.h"

// ���� :
class BossFrogFat : public BossFrog
{
public:
	// constrcuter destructer
	BossFrogFat();
	~BossFrogFat();

	// delete Function
	BossFrogFat(const BossFrogFat& _Other) = delete;
	BossFrogFat(BossFrogFat&& _Other) noexcept = delete;
	BossFrogFat& operator=(const BossFrogFat& _Other) = delete;
	BossFrogFat& operator=(BossFrogFat&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void InitAnimation() override;
	void SetFSMFUNC() override;

private:

	const float4 WeaponPivotPos = float4{ 0.0f,0.1f,0.0f };
	const float4 WeaponPivotRot = float4{ -85.03f,40.0f,0.0f };
	std::shared_ptr<class ContentFBXRenderer> WeaponRenderer = nullptr;

	bool isRightPattern = true;
	bool isTurned = false;

	//����->��ȿ->�������� ����-> ��->���� ->5�� ���� ->//SUCK_BOMB- >�����ۖR��->���� ��������->�ö��->�õ����� ���� ����
	//->�� ->6�� ������-> ���� ->��������
	enum class BossFrogFatState
	{
		JUMP_SCREAM,//intro
		IDLE,
		JUMP_TO_WATER,
		JUMP_TO_GROUND,
		TURN,
		TILT,
		SUCK,
		SHOOT,
		SUCK_BOMB,
		MAX,
	};
};
//DIE_LAND", "FROG_F
//DIE_STANDING", "FR
//IDLE", "FROG_FAT_I
//FAT_JUMP", "FROG_F	�ٴ� to �� «Ǫ
//JUMP_SCREAM", "FRO	��Ʈ��
//SHOOT", "FROG_FAT_	���ʿ��� 6�� ����
//SUCK", "FROG_FAT_S	�����ʿ��� ���� 5�� ����
//SUCK_BOMB", "FROG_	���� �� ��ź ����
//SUCK_BOMB_GETUP",		���� ���� �� �Ͼ
//SUCK_BOMB_LOOP", "	���� ����
//TILT", "FROG_FAT_T	�� ���
//TILT_GRABBED", "FR	������
//TILT_JUMP", "FROG_	�� to �ٴ� ��� ���� �ʴ�
//TURN", "FROG_FAT_T