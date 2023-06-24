#pragma once

/// <summary>
/// 処理のマネージャー
/// </summary>
class ProcessingManager
{
public:
	ProcessingManager();
	~ProcessingManager();

	/// <summary>
	/// 全ての初期化処理
	/// </summary>
	bool AllInitialize();

	/// <summary>
	/// 全ての終了処理
	/// </summary>
	void AllFinalize();

	/// <summary>
	/// 全ての更新処理
	/// </summary>
	void AllUpdate();

	/// <summary>
	/// 全ての描画処理
	/// </summary>
	void AllDraw();

private:

};