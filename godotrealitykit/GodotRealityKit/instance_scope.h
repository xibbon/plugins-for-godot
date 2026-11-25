//===----------------------------------------------------------------------===//
// Copyright © 2026 Apple Inc.
//
// Licensed under the MIT license (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// LICENSE
//
//===----------------------------------------------------------------------===//

#pragma once

#include <godot_cpp/godot.hpp>

#include <cstdint>

namespace gdrk {

class SceneLoader;

// Reads the host's GDTExtensionVolume class. A host that answers YES to +managesGodotScenes
// (Xogot) owns the Godot windows and the volume window, and may run several Godot instances in
// one process.
bool is_host_managed();

// Saves the host domain token and selectors when the host is managed. Falls back to the
// legacy multi-instance interface. Called while this library's engine domain is attached.
void instance_scope_setup(GDExtensionInterfaceGetProcAddress p_get_proc_address);

// Marks the instance that loaded this library as shut down, so late callbacks skip Godot.
void instance_scope_shutdown();

// Clears any queued material preload paths before scene or module teardown.
void clear_preloading_materials();

// Tracks the currently active SceneLoader session so stale GDRKBridgeDelegate copies held by
// a dismissing SwiftUI view cannot dereference a freed SceneLoader across stop/replay.
uint64_t activate_scene_loader(SceneLoader *p_loader);
void deactivate_scene_loader(uint64_t p_session_id, SceneLoader *p_loader);
bool is_scene_loader_session_active(uint64_t p_session_id, const SceneLoader *p_loader);

// RealityKit and SwiftUI call back outside the engine's iteration. In a multi-instance host
// another Godot domain may be selected then. Each callback selects the generation-bearing
// domain token saved by this library and restores the previous token. Legacy hosts use an
// instance index instead. A no-op in a single-instance host.
class InstanceScope {
public:
	InstanceScope();
	~InstanceScope();

	// False once the instance has shut down; the callback must not touch Godot.
	bool is_valid() const { return valid; }

private:
	uint64_t previous_domain_token = 0;
	bool restore_domain = false;
	uint8_t previous_instance = 0;
	bool restore = false;
	bool valid = true;
};

} // namespace gdrk

// Attaches this library's Godot instance for the rest of the scope, or returns m_retval if the
// instance has shut down.
#define GDRK_INSTANCE_SCOPE_OR_RETURN(m_retval) \
	gdrk::InstanceScope gdrk_instance_scope;    \
	if (!gdrk_instance_scope.is_valid()) {      \
		return m_retval;                        \
	}
