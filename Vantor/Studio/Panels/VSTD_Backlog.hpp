#pragma once

#include "../../Source/Core/BackLog/VCO_Backlog.hpp"

#include "../Interface/VSTD_Panel.hpp"

#include <Shared/imgui/imgui.h>

#include <memory>

namespace Vantor::Studio {
    inline ImVec4 GetLogColor(Vantor::Backlog::LogLevel level)
    {
        switch (level)
        {
            case Vantor::Backlog::LogLevel::INFO:    return ImVec4(0.8f, 0.8f, 0.8f, 0.15f);
            case Vantor::Backlog::LogLevel::DEBUG:   return ImVec4(0.5f, 0.9f, 1.0f, 0.15f);
            case Vantor::Backlog::LogLevel::WARNING: return ImVec4(1.0f, 0.8f, 0.3f, 0.15f);
            case Vantor::Backlog::LogLevel::ERR:     return ImVec4(1.0f, 0.3f, 0.3f, 0.15f);
        }
        return ImVec4(1, 1, 1, 0);
    }

    class VSBacklog : public VSEditorPanel {
        public:
            VSBacklog() {
            }
            
            virtual ~VSBacklog() = default;

            void OnImGuiRender() override {
                static int saveID = 0;
                static int filterLevel = 0; // 0: All, 1: INFO+, 2: WARNING+, 3: ERR only
                static bool autoScroll = true;

                ImGui::Combo("Filter", &filterLevel, "All\0Info+\0Warning+\0Error only\0");
                ImGui::SameLine();
                ImGui::Checkbox("Auto-scroll", &autoScroll);
                ImGui::SameLine();
                if (ImGui::Button("Save")) {
                    Vantor::Backlog::SaveLogs("VLog" + std::to_string(saveID) + ".log");
                    saveID += 1;
                }

                ImGui::Separator();

                ImGui::BeginChild("BacklogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                if (ImGui::BeginTable("BacklogTable", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable)) {
                    ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();

                    std::scoped_lock lock(Vantor::Backlog::logMutex);
                    for (const auto& entry : Vantor::Backlog::logEntries) {
                        int severity = static_cast<int>(entry.level);
                        if (severity < filterLevel) continue;

                        ImGui::TableNextRow();

                        // Set background color
                        ImU32 bgColor = ImGui::ColorConvertFloat4ToU32(GetLogColor(entry.level));
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, bgColor);

                        // Render columns
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", Vantor::Backlog::LogLevelToString(entry.level).c_str());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(entry.source.c_str());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::TextUnformatted(entry.text.c_str());
                    }

                    ImGui::EndTable();
                }

                if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f)
                    ImGui::SetScrollHereY(1.0f);

                ImGui::EndChild();
            }

            const char* GetName() const override { return "Backlog"; }
    };
}