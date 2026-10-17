#ifndef MENU
#define MENU
#include <cmath>
#include "ImGui/imgui.h"
#include "Themes.h"
#include "../Data/Fonts/Roboto-Regular.h"


using namespace ImGui;
static bool init;
int  glWidth, glHeight;

void SetupImGui()
{
    
    if (!init)
    {
        auto context = ImGui::CreateContext();
        if (!context)
        {
            return;
        }
        ImGuiIO &io = ImGui::GetIO();
        ImFontConfig font_cfg;
        io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
        font_cfg.SizePixels = 22.0f;
        io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, 22, 22.0f);

        io.IniFilename = NULL;
        io.KeyMap[ImGuiKey_UpArrow] = 19;
        io.KeyMap[ImGuiKey_DownArrow] = 20;
        io.KeyMap[ImGuiKey_LeftArrow] = 21;
        io.KeyMap[ImGuiKey_RightArrow] = 22;
        io.KeyMap[ImGuiKey_Enter] = 66;
        io.KeyMap[ImGuiKey_Backspace] = 67;
        io.KeyMap[ImGuiKey_PageUp] = 92;
        io.KeyMap[ImGuiKey_PageDown] = 93;
        io.KeyMap[ImGuiKey_Escape] = 111;
        io.KeyMap[ImGuiKey_Delete] = 112;
        io.KeyMap[ImGuiKey_Home] = 122;
        io.KeyMap[ImGuiKey_End] = 123;
        io.KeyMap[ImGuiKey_Insert] = 124;
        io.KeyMap[ImGuiKey_UpArrow] = 19;
        io.KeyMap[ImGuiKey_DownArrow] = 20;
        io.KeyMap[ImGuiKey_LeftArrow] = 21;
        io.KeyMap[ImGuiKey_RightArrow] = 22;
        io.KeyMap[ImGuiKey_Enter] = 66;
        io.KeyMap[ImGuiKey_Backspace] = 67;
        io.KeyMap[ImGuiKey_PageUp] = 92;
        io.KeyMap[ImGuiKey_PageDown] = 93;
        io.KeyMap[ImGuiKey_Escape] = 111;
        io.KeyMap[ImGuiKey_Delete] = 112;
        io.KeyMap[ImGuiKey_Home] = 122;
        io.KeyMap[ImGuiKey_End] = 123;
        io.KeyMap[ImGuiKey_Insert] = 124;
        
        //Set your theme here
        Theme::ApplyModernStyle();
        Theme::SetPurplePanelTheme();
        /* All the themes
        SetBlackGoldTheme();
        SetYesAnotherDarkTheme();
        SetDarkGrayTheme();
        SetCorporateGrayTheme();
        SetYetAnotherDarkTheme();
        SetSoftDarkRedTheme();
        SetClassicSteamHalfLifeTheme();
        SetAnotherDarkThemeReally();
        SetDarkGreenBlueTheme();
        SetDarkRedTheme();
        */
        
        ImGui::GetStyle().ScaleAllSizes(3.0f);
        
        ImGui_ImplAndroid_Init(nullptr);
        ImGui_ImplOpenGL3_Init();

        init = true;
    }
}

namespace UI
{
    static ImVec4 Mix(const ImVec4 &a, const ImVec4 &b, float t)
    {
        return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,
                      a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    // Animated iOS-style toggle switch
    static bool Toggle(const char *label, bool *v)
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        float h = ImGui::GetFrameHeight();
        float w = h * 1.9f;
        float r = h * 0.5f;
        float full = ImGui::GetContentRegionAvail().x;

        ImGui::InvisibleButton(label, ImVec2(full, h));
        bool changed = false;
        if (ImGui::IsItemClicked()) { *v = !*v; changed = true; }

        ImGuiID id = ImGui::GetID(label);
        ImGuiStorage *st = ImGui::GetStateStorage();
        float target = *v ? 1.0f : 0.0f;
        float t = st->GetFloat(id, target);
        t += (target - t) * (ImGui::GetIO().DeltaTime * 14.0f > 1.0f ? 1.0f : ImGui::GetIO().DeltaTime * 14.0f);
        st->SetFloat(id, t);

        const ImVec4 off = ImVec4(0.20f, 0.20f, 0.30f, 1.0f);
        const ImVec4 on  = ImVec4(0.55f, 0.36f, 1.00f, 1.0f);
        ImU32 bg = ImGui::GetColorU32(Mix(off, on, t));

        float x0 = p.x + full - w;           // switch sits on the right
        dl->AddText(ImVec2(p.x, p.y + style.FramePadding.y),
                    ImGui::GetColorU32(ImGuiCol_Text), label);
        dl->AddRectFilled(ImVec2(x0, p.y), ImVec2(x0 + w, p.y + h), bg, r);
        if (t > 0.01f)
            dl->AddRect(ImVec2(x0 - 1, p.y - 1), ImVec2(x0 + w + 1, p.y + h + 1),
                        ImGui::GetColorU32(ImVec4(0.65f, 0.48f, 1.0f, 0.35f * t)), r + 1, 0, 2.0f);
        float cx = x0 + r + (w - 2 * r) * t;
        dl->AddCircleFilled(ImVec2(cx, p.y + r), r - 3.0f, IM_COL32(255, 255, 255, 255), 24);
        return changed;
    }

    // Small purple section label with a line under it
    static void Section(const char *text)
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.65f, 0.48f, 1.0f, 1.0f), "%s", text);
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddLine(p, ImVec2(p.x + ImGui::GetContentRegionAvail().x, p.y),
                                            IM_COL32(140, 92, 255, 90), 1.5f);
        ImGui::Dummy(ImVec2(0, 4));
    }

    // Gradient banner with title + live FPS
    static void Banner(const char *title, const char *sub)
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        float w = ImGui::GetContentRegionAvail().x;
        float h = ImGui::GetFrameHeight() * 2.3f;
        ImU32 c1 = IM_COL32(120, 70, 255, 150), c2 = IM_COL32(0, 200, 255, 150);
        ImU32 c3 = IM_COL32(40, 20, 110, 255),  c4 = IM_COL32(10, 70, 110, 255);
        dl->AddRectFilledMultiColor(p, ImVec2(p.x + w, p.y + h), c3, c4, c2, c1);
        dl->AddRect(p, ImVec2(p.x + w, p.y + h), IM_COL32(160, 120, 255, 140), 8.0f, 0, 1.5f);
        dl->AddText(ImVec2(p.x + 14, p.y + h * 0.14f), IM_COL32(255, 255, 255, 255), title);
        dl->AddText(ImVec2(p.x + 14, p.y + h * 0.56f), IM_COL32(200, 215, 255, 220), sub);
        char fps[24];
        snprintf(fps, sizeof(fps), "%.0f FPS", ImGui::GetIO().Framerate);
        ImVec2 ts = ImGui::CalcTextSize(fps);
        dl->AddText(ImVec2(p.x + w - ts.x - 14, p.y + h * 0.5f - ts.y * 0.5f), IM_COL32(130, 255, 200, 255), fps);
        ImGui::Dummy(ImVec2(w, h + 6));
    }
}

namespace UI
{
    // tiny vector icons (no icon font needed)
    static void Icon(ImDrawList *dl, int kind, ImVec2 c, float r, ImU32 col)
    {
        const float th = 2.5f;
        if (kind == 0) { // target
            dl->AddCircle(c, r * 0.7f, col, 24, th);
            dl->AddCircleFilled(c, r * 0.18f, col);
            dl->AddLine(ImVec2(c.x - r, c.y), ImVec2(c.x - r * 0.4f, c.y), col, th);
            dl->AddLine(ImVec2(c.x + r * 0.4f, c.y), ImVec2(c.x + r, c.y), col, th);
            dl->AddLine(ImVec2(c.x, c.y - r), ImVec2(c.x, c.y - r * 0.4f), col, th);
            dl->AddLine(ImVec2(c.x, c.y + r * 0.4f), ImVec2(c.x, c.y + r), col, th);
        } else if (kind == 1) { // eye
            dl->AddBezierCubic(ImVec2(c.x - r, c.y), ImVec2(c.x - r * 0.5f, c.y - r * 0.9f),
                               ImVec2(c.x + r * 0.5f, c.y - r * 0.9f), ImVec2(c.x + r, c.y), col, th, 20);
            dl->AddBezierCubic(ImVec2(c.x - r, c.y), ImVec2(c.x - r * 0.5f, c.y + r * 0.9f),
                               ImVec2(c.x + r * 0.5f, c.y + r * 0.9f), ImVec2(c.x + r, c.y), col, th, 20);
            dl->AddCircleFilled(c, r * 0.3f, col);
        } else if (kind == 2) { // gear
            dl->AddCircle(c, r * 0.55f, col, 20, th + 1.0f);
            for (int i = 0; i < 8; i++) {
                float a = i * 0.7853982f;
                dl->AddLine(ImVec2(c.x + cosf(a) * r * 0.7f, c.y + sinf(a) * r * 0.7f),
                            ImVec2(c.x + cosf(a) * r, c.y + sinf(a) * r), col, th + 1.0f);
            }
        } else { // card
            dl->AddRect(ImVec2(c.x - r, c.y - r * 0.7f), ImVec2(c.x + r, c.y + r * 0.7f), col, 3.0f, 0, th);
            dl->AddCircleFilled(ImVec2(c.x - r * 0.45f, c.y - r * 0.1f), r * 0.2f, col);
            dl->AddLine(ImVec2(c.x - r * 0.75f, c.y + r * 0.35f), ImVec2(c.x - r * 0.15f, c.y + r * 0.35f), col, th);
            dl->AddLine(ImVec2(c.x + r * 0.1f, c.y - r * 0.2f), ImVec2(c.x + r * 0.75f, c.y - r * 0.2f), col, th);
            dl->AddLine(ImVec2(c.x + r * 0.1f, c.y + r * 0.2f), ImVec2(c.x + r * 0.75f, c.y + r * 0.2f), col, th);
        }
    }

    // purple square side-tab with icon
    static bool SideTab(const char *id, int kind, bool selected)
    {
        float s = ImGui::GetFrameHeight() * 2.0f;
        ImVec2 p = ImGui::GetCursorScreenPos();
        bool clicked = ImGui::InvisibleButton(id, ImVec2(s, s));
        bool hov = ImGui::IsItemHovered();
        ImU32 bg = selected ? IM_COL32(190, 40, 255, 255)
                            : (hov ? IM_COL32(150, 30, 210, 255) : IM_COL32(110, 20, 160, 255));
        ImDrawList *dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(p, ImVec2(p.x + s, p.y + s), bg, 8.0f);
        Icon(dl, kind, ImVec2(p.x + s * 0.5f, p.y + s * 0.5f), s * 0.26f, IM_COL32(255, 255, 255, 255));
        return clicked;
    }
}

namespace Menu
{
  struct {
        
      bool BoolExample = false;
      bool FloatExample = false;
      bool IntExample = false;
      bool VoidExample = false;
      bool Memorycoins = false;
      
    } SWITCH; //you can write whatever you want and use SWITCH.BoolExample

    
    int (*Screen_get_width)();
    int (*Screen_get_height)();
    ImVec4 color = ImVec4(1, 1, 1, 1);
    
    bool* p_open; //to open menu bars
    
    void DrawMenu() /*main menu function*/
    {
        
        static bool show_demo_window = false;
        static bool show_another_window = false;
        static bool show_app_metrics = false;
        static bool showfrm = false;
        static bool show_app_style_editor = false;
        static bool show_app_about = false;
        static bool showtabmenu = false;

        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

       
       bool FeatureH;
       
       if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
            
        // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            
            ImGui::SetNextWindowSize(ImVec2(520, 330), ImGuiCond_FirstUseEver);
            ImGui::Begin("MY PANEL  v1.0", p_open, ImGuiWindowFlags_NoCollapse);

            static int tab = 0;
            static int comboA = 0, comboB = 0;
            static bool opt[8] = {true, true, true, false, false, false, false, false};
            static const char *items[] = {"Top", "Center", "Bottom"};
            static const char *items2[] = {"Box", "Round", "Corner"};

            ImGui::BeginChild("##side", ImVec2(ImGui::GetFrameHeight() * 2.0f + 6, 0), false);
            if (UI::SideTab("##t0", 0, tab == 0)) tab = 0;
            if (UI::SideTab("##t1", 1, tab == 1)) tab = 1;
            if (UI::SideTab("##t2", 2, tab == 2)) tab = 2;
            if (UI::SideTab("##t3", 3, tab == 3)) tab = 3;
            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("##content", ImVec2(0, 0), true);
            if (tab == 0) {
                ImGui::Checkbox("Enable", &opt[0]);
                ImGui::Checkbox("Option 1", &opt[1]);
                ImGui::SameLine(); ImGui::Text("Style:");
                ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetFrameHeight() * 5.0f);
                ImGui::Combo("##c1", &comboA, items, 3);
                ImGui::Checkbox("Option 2", &opt[2]);
                ImGui::SameLine(); ImGui::Text("Style:");
                ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetFrameHeight() * 5.0f);
                ImGui::Combo("##c2", &comboB, items2, 3);
                ImGui::Checkbox("Option 3", &opt[3]);
                ImGui::Checkbox("Option 4", &opt[4]);
                ImGui::Checkbox("Option 5", &opt[5]);
                ImGui::Checkbox("Option 6", &opt[6]);
            } else if (tab == 1) {
                ImGui::Checkbox("FPS overlay", &showfrm);
                ImGui::Checkbox("Theme window", &show_another_window);
                ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                ImGui::ColorEdit3("Color", (float *)&clear_color);
            } else if (tab == 2) {
                ImGui::Checkbox("Style editor", &show_app_style_editor);
                ImGui::Checkbox("ImGui demo", &show_demo_window);
                if (ImGui::Button("Tap me")) counter++;
                ImGui::SameLine(); ImGui::Text("count = %d", counter);
            } else {
                ImGui::Text("My Panel v1.0");
                ImGui::TextDisabled("Built with ImGui");
                ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            }
            ImGui::EndChild();
               
      if (show_app_style_editor)
       {
          ImGui::Begin("Mod Menu Style Editor", &show_app_style_editor);
          ImGui::ShowStyleEditor();
          ImGui::End();
       }
       
     if (showfrm)
     {
          int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.40f); // Transparent background
    if (ImGui::Begin("Framrate overlay", p_open, window_flags))
    {
        
        ImGui::Text("           FrameRate");
        ImGui::Separator();
        ImGui::Text("App Average %.3f ms\n""     Frames %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SameLine();
    
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
     }
       
     if (show_another_window)
      {
        ImGui::Begin("Viewer", &show_another_window /*p_open*/, ImGuiWindowFlags_MenuBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                if (ImGui::BeginMenuBar())
         {
        //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
           if (ImGui::BeginMenu("Tools"))
            {
                Text("Another tab");
               //ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
               ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
         }
               ImGui::Text("Menu Colour");
                
                static int e = 0;
                ImGui::RadioButton("Black Gold", &e, 1);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark 1", &e, 2);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark Gray", &e, 3);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Corporate Gray", &e, 4);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark 2", &e, 5);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark 3", &e, 6);
                ImGui::TableNextColumn();
                ImGui::RadioButton("StyleCalssic", &e, 7);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark 4", &e, 8);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark 5", &e, 9);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Dark 6", &e, 10);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Neon", &e, 11);
                ImGui::TableNextColumn();
                ImGui::RadioButton("Purple", &e, 12);
                ImGui::TableNextColumn();
                    switch (e) {
                        
                       case 1:
                          Theme::SetBlackGoldTheme();
                          break;
                       case 2:
                          Theme::SetYesAnotherDarkTheme();
                          break;
                       case 3:
                         Theme::SetDarkGrayTheme();
                          break;
                       case 4:
                         Theme::SetCorporateGrayTheme();
                          break;
                       case 5:
                          Theme::SetYetAnotherDarkTheme();
                          break;
                       case 6:
                          Theme::SetSoftDarkRedTheme();
                          break;
                       case 7:
                          Theme::SetClassicSteamHalfLifeTheme();
                          break;
                       case 8:
                          Theme::SetAnotherDarkThemeReally();
                          break;
                       case 9:
                          Theme::SetDarkGreenBlueTheme();
                          break;
                       case 10:
                         Theme::SetDarkRedTheme();
                          break;
                       case 11:
                         Theme::SetModernNeonTheme();
                          break;
                       case 12:
                         Theme::SetPurplePanelTheme();
                          break;
                       }
            ImGui::Text("Hello from another window!");
            ImGui::Text("App Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            if (ImGui::Button("Close")) show_another_window = false;
            ImGui::End();
       }
            ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    }
 void DrawImGuiMenu()
    {
        if (init && Screen_get_height)
        {
            ImGuiIO &io = ImGui::GetIO();
            static bool WantTextInputLast = false;
            if (io.WantTextInput && !WantTextInputLast)
                displayKeyboard(true);
            WantTextInputLast = io.WantTextInput;
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplAndroid_NewFrame(Screen_get_width(), Screen_get_height());;
            ImGui::NewFrame();
            DrawMenu();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_PageUp]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_PageDown]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Home]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_End]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Insert]] = false;
            ImGui::EndFrame();
        }
    }
    
}


#endif MENU
