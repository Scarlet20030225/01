#pragma once

/// <summary>
/// �����̃}�l�[�W���[
/// </summary>
class ProcessingManager
{
public:
	ProcessingManager();
	~ProcessingManager();

	/// <summary>
	/// �S�Ă̏���������
	/// </summary>
	bool AllInitialize();

	/// <summary>
	/// �S�Ă̏I������
	/// </summary>
	void AllFinalize();

	/// <summary>
	/// �S�Ă̍X�V����
	/// </summary>
	void AllUpdate();

	/// <summary>
	/// �S�Ă̕`�揈��
	/// </summary>
	void AllDraw();

private:

};