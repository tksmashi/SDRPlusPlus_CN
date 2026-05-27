#include <gui/style.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <config.h>
#include <utils/flog.h>
#include <filesystem>

namespace style {
    ImFont* baseFont;
    ImFont* bigFont;
    ImFont* hugeFont;
    ImVector<ImWchar> baseRanges;
    ImVector<ImWchar> bigRanges;
    ImVector<ImWchar> hugeRanges;

#ifndef __ANDROID__
    float uiScale = 1.0f;
#else
    float uiScale = 3.0f;
#endif

    // Find a CJK font on the system
    static std::string findCJKFont() {
        // List of CJK font paths to try, in order of preference
        const char* cjkFontPaths[] = {
#ifdef _WIN32
            // Windows: Microsoft YaHei (most common on Chinese Windows)
            "C:/Windows/Fonts/msyh.ttc",
            // Windows: Microsoft YaHei Bold
            "C:/Windows/Fonts/msyhbd.ttc",
            // Windows: SimHei (classic Chinese font)
            "C:/Windows/Fonts/simhei.ttf",
            // Windows: SimSun
            "C:/Windows/Fonts/simsun.ttc",
#elif __APPLE__
            // macOS: PingFang SC
            "/System/Library/Fonts/PingFang.ttc",
            "/Library/Fonts/Arial Unicode.ttf",
#elif defined(__ANDROID__)
            // Android: NotoSans CJK (Android 7+, most common)
            "/system/fonts/NotoSansCJK-Regular.ttc",
            "/system/fonts/NotoSansSC-Regular.otf",
            "/system/fonts/NotoSansSC-Regular.ttf",
            // Android: DroidSans Fallback (older Android)
            "/system/fonts/DroidSansFallbackFull.ttf",
            "/system/fonts/DroidSansFallback.ttf",
            // Android: NotoSerif CJK (some devices)
            "/system/fonts/NotoSerifCJK-Regular.ttc",
#else
            // Linux: common CJK font paths
            "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/noto/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/opentype/noto/NotoSansSC-Regular.otf",
            "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",
            "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",
            "/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc",
#endif
            NULL
        };

        for (int i = 0; cjkFontPaths[i] != NULL; i++) {
            if (std::filesystem::exists(cjkFontPaths[i])) {
                flog::info("Found CJK font: {0}", cjkFontPaths[i]);
                return cjkFontPaths[i];
            }
        }

        return "";
    }

    bool loadFonts(std::string resDir) {
        ImFontAtlas* fonts = ImGui::GetIO().Fonts;
        if (!std::filesystem::is_directory(resDir)) {
            flog::error("Invalid resource directory: {0}", resDir);
            return false;
        }

        // Find a CJK font for Chinese character support
        std::string cjkFontPath = findCJKFont();
        bool hasCJKFont = !cjkFontPath.empty();

        // Create base font range (with full CJK support)
        ImFontGlyphRangesBuilder baseBuilder;
        baseBuilder.AddRanges(fonts->GetGlyphRangesDefault());
        baseBuilder.AddRanges(fonts->GetGlyphRangesCyrillic());
        if (hasCJKFont) {
            baseBuilder.AddRanges(fonts->GetGlyphRangesChineseFull());
        }
        baseBuilder.BuildRanges(&baseRanges);

        // Create big font range (digits + CJK for frequency window labels)
        ImFontGlyphRangesBuilder bigBuilder;
        const ImWchar bigRange[] = { '.', '9', 0 };
        bigBuilder.AddRanges(bigRange);
        if (hasCJKFont) {
            bigBuilder.AddRanges(fonts->GetGlyphRangesChineseFull());
        }
        bigBuilder.BuildRanges(&bigRanges);

        // Create huge font range
        ImFontGlyphRangesBuilder hugeBuilder;
        const ImWchar hugeRange[] = { 'S', 'S', 'D', 'D', 'R', 'R', '+', '+', ' ', ' ', 0 };
        hugeBuilder.AddRanges(hugeRange);
        hugeBuilder.BuildRanges(&hugeRanges);
        
        // Load base font (Roboto for Latin, with CJK glyphs merged from system font)
        baseFont = fonts->AddFontFromFileTTF(((std::string)(resDir + "/fonts/Roboto-Medium.ttf")).c_str(), 16.0f * uiScale, NULL, baseRanges.Data);

        // Merge CJK font into baseFont so Chinese characters render correctly (full range)
        if (hasCJKFont) {
            ImFontConfig cjkConfig;
            cjkConfig.MergeMode = true;
            cjkConfig.PixelSnapH = true;
            fonts->AddFontFromFileTTF(cjkFontPath.c_str(), 16.0f * uiScale, &cjkConfig, fonts->GetGlyphRangesChineseFull());
            flog::info("CJK font merged into base font (full range)");
        } else {
            flog::warn("No CJK font found! Chinese characters may not display correctly.");
        }

        // Add bigger fonts for frequency select and title
        bigFont = fonts->AddFontFromFileTTF(((std::string)(resDir + "/fonts/Roboto-Medium.ttf")).c_str(), 45.0f * uiScale, NULL, bigRanges.Data);

        // Merge CJK font into bigFont for frequency window labels
        if (hasCJKFont) {
            ImFontConfig bigCjkConfig;
            bigCjkConfig.MergeMode = true;
            bigCjkConfig.PixelSnapH = true;
            fonts->AddFontFromFileTTF(cjkFontPath.c_str(), 45.0f * uiScale, &bigCjkConfig, fonts->GetGlyphRangesChineseFull());
            flog::info("CJK font merged into big font");
        }

        hugeFont = fonts->AddFontFromFileTTF(((std::string)(resDir + "/fonts/Roboto-Medium.ttf")).c_str(), 128.0f * uiScale, NULL, hugeRanges.Data);

        return true;
    }

    void beginDisabled() {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        auto& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;
        ImVec4 btnCol = colors[ImGuiCol_Button];
        ImVec4 frameCol = colors[ImGuiCol_FrameBg];
        ImVec4 textCol = colors[ImGuiCol_Text];
        btnCol.w = 0.15f;
        frameCol.w = 0.30f;
        textCol.w = 0.65f;
        ImGui::PushStyleColor(ImGuiCol_Button, btnCol);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, frameCol);
        ImGui::PushStyleColor(ImGuiCol_Text, textCol);
    }

    void endDisabled() {
        ImGui::PopItemFlag();
        ImGui::PopStyleColor(3);
    }
}

namespace ImGui {
    void LeftLabel(const char* text) {
        float vpos = ImGui::GetCursorPosY();
        ImGui::SetCursorPosY(vpos + GImGui->Style.FramePadding.y);
        ImGui::TextUnformatted(text);
        ImGui::SameLine();
        ImGui::SetCursorPosY(vpos);
    }

    void FillWidth() {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    }
}
