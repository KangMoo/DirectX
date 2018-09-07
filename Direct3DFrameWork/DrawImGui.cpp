#include "stdafx.h"
#include "DrawImGui.h"


DrawImGui::DrawImGui()
	:isShowDemo(false), isShowSystem(false)
{
}


DrawImGui::~DrawImGui()
{
}

void DrawImGui::Update()
{
	if (Input->Press(VK_CONTROL))
	{
		if (Input->Down(VK_F11))
			ChangeSystemInfo();
		if (Input->Down(VK_F12))
			ChangeDemoInfo();
	}
}

void DrawImGui::GuiUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem("System Info","Ctrl + F11"))
				ChangeSystemInfo();
			if (ImGui::MenuItem("Demo Window", "Ctrl + F12"))
				ChangeDemoInfo();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (isShowSystem)
	{
		ImGui::Begin("System Info", &isShowSystem);
		ImGui::Text("Frame Per Second : %4.0f",
			ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::End();
	}
	if (isShowDemo)
		ImGui::ShowDemoWindow(&isShowDemo);
}

void DrawImGui::ChangeSystemInfo()
{
	isShowSystem = !isShowSystem;
}

void DrawImGui::ChangeDemoInfo()
{
	isShowDemo = !isShowDemo;
}
