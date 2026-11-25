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

#include <godot_cpp/classes/object.hpp>

#include <string>
#include <vector>

namespace gdrk {

// Exported object that can be used to expose global functions to GDScript
class GodotRealityKit : public godot::Object {
	GDCLASS(GodotRealityKit, Object);

protected:
	static void _bind_methods();

public:
	static void preload_material(godot::String p_path);
	static void clear_preloading_materials();
	static std::vector<std::string> preloading_materials;
};

} //namespace gdrk
