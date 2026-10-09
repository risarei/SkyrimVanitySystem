# Skyrim Vanity System menu API

The edited SkyrimVanitySystem.dll exposes these C exports for optional menu
integrations:

```cpp
extern "C" bool SkyrimVanitySystem_IsReady();
extern "C" bool SkyrimVanitySystem_Open();
extern "C" bool SkyrimVanitySystem_Close();
extern "C" bool SkyrimVanitySystem_IsMenuOpen();
extern "C" void SkyrimVanitySystem_SetHotkeyEnabled(bool enabled);
extern "C" std::uint32_t SkyrimVanitySystem_GetHotkey();
extern "C" bool SkyrimVanitySystem_SetHotkey(std::uint32_t scanCode);
```

Resolve the functions with `GetModuleHandleW` and `GetProcAddress`; do not load
the SKSE plugin yourself. Open and Close return whether a request was accepted,
not whether a frame has already appeared. They marshal work to Skyrim's UI task
queue. IsMenuOpen reports the actual menu state. IsReady becomes true after the
UI is initialized and Skyrim's data is loaded.

SetHotkeyEnabled controls only the native hotkey listener, in memory. The
configured key is unchanged and the listener starts enabled on each game launch.
SetHotkey accepts a valid non-modifier DirectInput scan code, applies it on the
UI task queue, and saves it to settings.json. GetHotkey reports the currently
applied scan code, or zero before initialization.

Risa uses these exports when available. Older Skyrim Vanity System DLLs keep
Risa's legacy F22 settings-file integration.

This build uses CommonLibSSE-NG v11.0.0. For a source checkout without populated
submodules, initialize ImGui and CommonLib, then check out CommonLib tag v11.0.0
and initialize its OpenVR submodule before running `xmake f -y -m release` and
`xmake -y -j 4`.
