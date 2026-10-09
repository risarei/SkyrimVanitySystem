#pragma once

#include <cstdint>

namespace sosr::api {
void MarkReady(std::uint32_t a_hotkey);
void SetGameDataLoaded(bool a_loaded);
void SetMenuOpenState(bool a_open);
void SetObservedHotkey(std::uint32_t a_hotkey);
[[nodiscard]] bool NativeHotkeyEnabled();
} // namespace sosr::api
