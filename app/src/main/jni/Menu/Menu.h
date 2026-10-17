#ifndef MENU
#define MENU
#include <cmath>
#include "ImGui/imgui.h"
#include "Themes.h"
#include "../Data/Fonts/Roboto-Regular.h"

using namespace ImGui;
static bool init;
int glWidth, glHeight;

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
        io.KeyMap[ImGuiKey_Escape] = 111;
        io.KeyMap[ImGuiKey_Delete] = 112;
        io.KeyMap[ImGuiKey_Home] = 122;
        io.KeyMap[ImGuiKey_End] = 123;
        io.KeyMap[ImGuiKey_Insert] = 124;

        Theme::ApplyModernStyle();
        Theme::SetPurplePanelTheme();
        
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

    static void Icon(ImDrawList *dl, int kind, ImVec2 c, float r, ImU32 col)
    {
        const float th = 2.5f;
        if (kind == 0) { // ESP / Target Icon
            dl->AddCircle(c, r * 0.7f, col, 24, th);
            dl->AddCircleFilled(c, r * 0.18f, col);
            dl->AddLine(ImVec2(c.x - r, c.y), ImVec2(c.x - r * 0.4f, c.y), col, th);
            dl->AddLine(ImVec2(c.x + r * 0.4f, c.y), ImVec2(c.x + r, c.y), col, th);
            dl->AddLine(ImVec2(c.x, c.y - r), ImVec2(c.x, c.y - r * 0.4f), col, th);
            dl->AddLine(ImVec2(c.x, c.y + r * 0.4f), ImVec2(c.x, c.y + r), col, th);
        } else if (kind == 1) { // Aimbot / AimLock Icon
            dl->AddCircle(c, r * 0.6f, col, 20, th);
            dl->AddCircleFilled(c, r * 0.25f, col);
            dl->AddLine(ImVec2(c.x - r * 1.2f, c.y), ImVec2(c.x - r * 0.7f, c.y), col, th);
            dl->AddLine(ImVec2(c.x + r * 0.7f, c.y), ImVec2(c.x + r * 1.2f, c.y), col, th);
        } else if (kind == 2) { // Settings / Gear Icon
            dl->AddCircle(c, r * 0.55f, col, 20, th + 1.0f);
            for (int i = 0; i < 8; i++) {
                float a = i * 0.7853982f;
                dl->AddLine(ImVec2(c.x + cosf(a) * r * 0.7f, c.y + sinf(a) * r * 0.7f),
                            ImVec2(c.x + cosf(a) * r, c.y + sinf(a) * r), col, th + 1.0f);
            }
        } else { // Info / About Icon
            dl->AddRect(ImVec2(c.x - r, c.y - r * 0.7f), ImVec2(c.x + r, c.y + r * 0.7f), col, 3.0f, 0, th);
            dl->AddCircleFilled(ImVec2(c.x - r * 0.45f, c.y - r * 0.1f), r * 0.2f, col);
        }
    }

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
    // Hack Variables (ESP, Aimbot & AimLock)
    struct {
        bool AtivarEsp = false;
        bool EspLinha = false;
        bool EspCaixa = false;
        bool EspNome = false;
        bool EspVida = false;
        bool EspEsqueleto = false;
        bool EspSnapLine = false;

        bool AimbotEnable = false;
        bool AimLock = false;        // <--- අලුතින් එකතු කළ Aim Lock variable එක
        bool AimBoneHead = true;
        float AimFov = 30.0f;
        float AimSmooth = 5.0f;
    } HACK;

    int (*Screen_get_width)();
    int (*Screen_get_height)();
    bool* p_open;

    void DrawMenu()
    {
        static bool show_demo_window = false;
        static bool show_theme_window = false;
        static bool show_fps_overlay = false;

        ImGui::SetNextWindowSize(ImVec2(560, 380), ImGuiCond_FirstUseEver);
        ImGui::Begin("FFH4X ANDROID - PRO PANEL v2.1", p_open, ImGuiWindowFlags_NoCollapse);

        static int tab = 0;

        // Side Navigation Tabs
        ImGui::BeginChild("##side", ImVec2(ImGui::GetFrameHeight() * 2.0f + 6, 0), false);
        if (UI::SideTab("##t0", 0, tab == 0)) tab = 0; // ESP Menu
        if (UI::SideTab("##t1", 1, tab == 1)) tab = 1; // Aimbot / AimLock Menu
        if (UI::SideTab("##t2", 2, tab == 2)) tab = 2; // Settings / Themes
        if (UI::SideTab("##t3", 3, tab == 3)) tab = 3; // Info
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("##content", ImVec2(0, 0), true);

        if (tab == 0) { // --- ESP MENU ---
            ImGui::TextColored(ImVec4(0.65f, 0.48f, 1.0f, 1.0f), "== VISUAL / ESP SETTINGS ==");
            ImGui::Separator();
            ImGui::Checkbox("Ativar ESP", &HACK.AtivarEsp);
            
            if (HACK.AtivarEsp) {
                ImGui::Indent(15.0f);
                ImGui::Checkbox("ESP Linha (Lines)", &HACK.EspLinha);
                ImGui::Checkbox("ESP Caixa (Box)", &HACK.EspCaixa);
                ImGui::Checkbox("ESP Nome (Name)", &HACK.EspNome);
                ImGui::Checkbox("ESP Vida (Health)", &HACK.EspVida);
                ImGui::Checkbox("ESP Esqueleto (Bone)", &HACK.EspEsqueleto);
                ImGui::Checkbox("ESP SnapLine", &HACK.EspSnapLine);
                ImGui::Unindent(15.0f);
            }
        } 
        else if (tab == 1) { // --- AIMBOT & AIM LOCK MENU ---
            ImGui::TextColored(ImVec4(0.65f, 0.48f, 1.0f, 1.0f), "== AIMBOT & AIM LOCK ==");
            ImGui::Separator();
            ImGui::Checkbox("Ativar Aimbot", &HACK.AimbotEnable);

            if (HACK.AimbotEnable) {
                ImGui::Indent(15.0f);
                ImGui::Checkbox("Aim Lock (Target Lock)", &HACK.AimLock); // <--- මෙන්න Aim Lock Checkbox එක
                ImGui::Checkbox("Aim Head (Target Head)", &HACK.AimBoneHead);
                ImGui::SliderFloat("FOV Size", &HACK.AimFov, 5.0f, 180.0f, "%.1f");
                ImGui::SliderFloat("Smoothness", &HACK.AimSmooth, 1.0f, 20.0f, "%.1f");
                ImGui::Unindent(15.0f);
            }
        } 
        else if (tab == 2) { // --- SETTINGS & THEMES ---
            ImGui::TextColored(ImVec4(0.65f, 0.48f, 1.0f, 1.0f), "== MENU SETTINGS ==");
            ImGui::Separator();
            ImGui::Checkbox("FPS Overlay", &show_fps_overlay);
            ImGui::Checkbox("Theme Window", &show_theme_window);
            ImGui::Checkbox("ImGui Demo Window", &show_demo_window);
        } 
        else { // --- INFO ---
            ImGui::Text("FFH4X Mod Menu for Free Fire");
            ImGui::TextDisabled("Status: Hooked & Ready");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        }

        ImGui::EndChild();

        // Popups and Overlays
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        if (show_fps_overlay) {
            ImGui::Begin("FPS", p_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (show_theme_window) {
            ImGui::Begin("Theme Selector", &show_theme_window);
            static int themeChoice = 12;
            ImGui::RadioButton("Black Gold", &themeChoice, 1);
            ImGui::RadioButton("Dark Gray", &themeChoice, 3);
            ImGui::RadioButton("Neon Theme", &themeChoice, 11);
            ImGui::RadioButton("Purple Panel", &themeChoice, 12);

            switch (themeChoice) {
                case 1: Theme::SetBlackGoldTheme(); break;
                case 3: Theme::SetDarkGrayTheme(); break;
                case 11: Theme::SetModernNeonTheme(); break;
                case 12: Theme::SetPurplePanelTheme(); break;
            }
            ImGui::End();
        }

        ImGui::End();
    }

    void DrawImGuiMenu()
    {
        if (init && Screen_get_height)
        {
            ImGuiIO &io = ImGui::GetIO();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplAndroid_NewFrame(Screen_get_width(), Screen_get_height());
            ImGui::NewFrame();
            DrawMenu();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui::EndFrame();
        }
    }
}

#endif // MENU
