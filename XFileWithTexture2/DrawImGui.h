#pragma once
class DrawImGui
{
public:
	DrawImGui();
	~DrawImGui();

	virtual void Update();
	virtual void GuiUpdate();
	virtual void Render() {};

private:
	void ChangeSystemInfo();
	void ChangeDemoInfo();

	bool isShowSystem;
	bool isShowDemo;
};

