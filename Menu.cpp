#include "Menu.h"

bool Menu::menuToggle = false;
bool Menu::sourceFiles = false;

auto Menu::InitImGui() -> void {
	auto& style = ImGui::GetStyle();
	style.FrameRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.GrabRounding = 4.0f;
	style.WindowPadding = { 10.f, 10.f };
	style.PopupRounding = 0.f;
	style.FramePadding = { 8.f, 4.f };
	style.ItemSpacing = { 10.f, 8.f };
	style.ItemInnerSpacing = { 6.f, 6.f };
	style.TouchExtraPadding = { 0.f, 0.f };
	style.IndentSpacing = 21.f;
	style.ScrollbarSize = 15.f;
	style.GrabMinSize = 8.f;
	style.ChildBorderSize = 0.f;
	style.FrameBorderSize = 0.f;
	style.TabBorderSize = 0.f;
	style.WindowRounding = 4.f;
	style.ChildRounding = 3.f;
	style.ScrollbarRounding = 4.f;
	style.TabRounding = 4.f;
	style.WindowTitleAlign = { 0.5f, 0.5f };
	style.ButtonTextAlign = { 0.5f, 0.5f };
	style.DisplaySafeAreaPadding = { 3.f, 3.f };

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	colors[ImGuiCol_TextDisabled] = ImColor(42, 42, 42);
	colors[ImGuiCol_WindowBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_ChildBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_PopupBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_Border] = ImColor(35, 32, 40);
	colors[ImGuiCol_BorderShadow] = ImColor(35, 32, 40);
	colors[ImGuiCol_FrameBg] = ImColor(35, 42, 106);
	colors[ImGuiCol_FrameBgHovered] = ImColor(35, 42, 106);
	colors[ImGuiCol_FrameBgActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_TitleBg] = ImColor(39, 51, 125);
	colors[ImGuiCol_TitleBgActive] = ImColor(39, 51, 125);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(39, 51, 125);
	colors[ImGuiCol_MenuBarBg] = ImColor(39, 51, 125);
	colors[ImGuiCol_ScrollbarBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_ScrollbarGrab] = ImColor(39, 51, 125);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(34, 122, 180);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);
	colors[ImGuiCol_SliderGrab] = ImColor(33, 107, 167);
	colors[ImGuiCol_SliderGrabActive] = ImColor(27, 100, 151);
	colors[ImGuiCol_Button] = ImColor(33, 43, 105);
	colors[ImGuiCol_ButtonHovered] = ImColor(29, 100, 150);
	colors[ImGuiCol_ButtonActive] = ImColor(27, 100, 151);
	colors[ImGuiCol_Header] = ImColor(34, 122, 180);
	colors[ImGuiCol_HeaderHovered] = ImColor(29, 100, 150);
	colors[ImGuiCol_HeaderActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_Separator] = ImColor(46, 46, 46);
	colors[ImGuiCol_SeparatorHovered] = ImColor(46, 46, 46);
	colors[ImGuiCol_SeparatorActive] = ImColor(46, 46, 46);
	colors[ImGuiCol_ResizeGrip] = ImColor(46, 46, 46);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(29, 100, 150);
	colors[ImGuiCol_ResizeGripActive] = ImColor(27, 100, 151);
	colors[ImGuiCol_Tab] = ImColor(33, 43, 105);
	colors[ImGuiCol_TabHovered] = ImColor(34, 122, 180);
	colors[ImGuiCol_TabActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_TabUnfocused] = ImColor(33, 43, 105);
	colors[ImGuiCol_TabUnfocusedActive] = ImColor(34, 122, 180);

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;

	ImFontConfig fontConfig{};
	fontConfig.FontDataOwnedByAtlas = false;
	io.FontDefault = io.Fonts->AddFontFromMemoryTTF((void*)Font::font_rubik, sizeof(Font::font_rubik), 20.f, &fontConfig);
	io.Fonts->Build();
}

auto Menu::Render() -> void {
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

	DrawLogWindow("Game Log");
}


ImGuiTextBuffer     Buf;
ImGuiTextFilter     Filter;
ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
bool                AutoScroll = true;  // Keep scrolling if already at the bottom
bool                Verbose = true;		// Show Full Game Log
bool                ImDemo = false;		// Show Imgui Demo Window

auto Menu::AddLog(const char* fmt, ...) IM_FMTARGS(2) -> void
{
	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt, args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
		if (Buf[old_size] == '\n')
			LineOffsets.push_back(old_size + 1);
}

auto Menu::DrawLogWindow(const char* title, bool* p_open) -> void {

	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
#ifdef _DEBUG
		ImGui::Checkbox("Show Imgui Demo", &ImDemo);
#endif
		ImGui::EndPopup();
	}

	if (ImDemo)
		ImGui::ShowDemoWindow();

	// Main window
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	ImGui::Checkbox("Verbose", &Verbose);
	ImGui::SameLine();
	ImGui::Checkbox("Show SourceFiles", &sourceFiles);
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);


	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}
	if (copy)
		ImGui::LogToClipboard();

	//Verbose ? MH_EnableHook(GetCategoryLevel_Address) : MH_DisableHook(GetCategoryLevel_Address);
	//Verbose ? MH_EnableHook(LogSettingsInstance_Address) : MH_DisableHook(LogSettingsInstance_Address);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const auto* buf = Buf.begin();
	const auto* buf_end = Buf.end();

	if (Filter.IsActive())
	{
		for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
		{
			const char* line_start = buf + LineOffsets[line_no];
			const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
			if (Filter.PassFilter(line_start, line_end))
				ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(LineOffsets.Size);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}

	ImGui::PopStyleVar();

	if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}