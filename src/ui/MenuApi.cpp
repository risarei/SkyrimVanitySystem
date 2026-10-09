#include "MenuApi.h"

#include "Keycode.h"
#include "Menu.h"

#include <atomic>

namespace {
std::atomic<bool> g_ready{false};
std::atomic<bool> g_gameDataLoaded{false};
std::atomic<bool> g_menuOpen{false};
std::atomic<bool> g_nativeHotkeyEnabled{true};
std::atomic<std::uint32_t> g_hotkey{0};
} // namespace

namespace sosr::api {
void MarkReady(const std::uint32_t a_hotkey) {
  g_hotkey.store(a_hotkey, std::memory_order_release);
  g_ready.store(true, std::memory_order_release);
}

void SetGameDataLoaded(const bool a_loaded) {
  g_gameDataLoaded.store(a_loaded, std::memory_order_release);
}

void SetMenuOpenState(const bool a_open) {
  g_menuOpen.store(a_open, std::memory_order_release);
}

void SetObservedHotkey(const std::uint32_t a_hotkey) {
  g_hotkey.store(a_hotkey, std::memory_order_release);
}

bool NativeHotkeyEnabled() {
  return g_nativeHotkeyEnabled.load(std::memory_order_acquire);
}
} // namespace sosr::api

extern "C" DLLEXPORT bool SkyrimVanitySystem_IsReady() {
  return g_ready.load(std::memory_order_acquire) &&
         g_gameDataLoaded.load(std::memory_order_acquire);
}

extern "C" DLLEXPORT bool SkyrimVanitySystem_Open() {
  if (!SkyrimVanitySystem_IsReady()) {
    return false;
  }
  auto *tasks = SKSE::GetTaskInterface();
  if (tasks == nullptr) {
    return false;
  }
  tasks->AddUITask([]() { sosr::Menu::GetSingleton()->Open(); });
  return true;
}

extern "C" DLLEXPORT bool SkyrimVanitySystem_Close() {
  if (!SkyrimVanitySystem_IsReady()) {
    return false;
  }
  auto *tasks = SKSE::GetTaskInterface();
  if (tasks == nullptr) {
    return false;
  }
  tasks->AddUITask([]() { sosr::Menu::GetSingleton()->Close(); });
  return true;
}

extern "C" DLLEXPORT bool SkyrimVanitySystem_IsMenuOpen() {
  return g_menuOpen.load(std::memory_order_acquire);
}

extern "C" DLLEXPORT void SkyrimVanitySystem_SetHotkeyEnabled(
    const bool a_enabled) {
  g_nativeHotkeyEnabled.store(a_enabled, std::memory_order_release);
}

extern "C" DLLEXPORT std::uint32_t SkyrimVanitySystem_GetHotkey() {
  return g_hotkey.load(std::memory_order_acquire);
}

extern "C" DLLEXPORT bool SkyrimVanitySystem_SetHotkey(
    const std::uint32_t a_scanCode) {
  if (!SkyrimVanitySystem_IsReady() ||
      !sosr::keycode::IsValidHotkey(a_scanCode) ||
      sosr::keycode::IsKeyModifier(a_scanCode)) {
    return false;
  }
  auto *tasks = SKSE::GetTaskInterface();
  if (tasks == nullptr) {
    return false;
  }
  tasks->AddUITask([a_scanCode]() {
    sosr::Menu::GetSingleton()->SetNativeHotkey(a_scanCode);
  });
  return true;
}
