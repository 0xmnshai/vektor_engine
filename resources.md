https://aistudio.google.com/prompts/1kk3cWWr1aVoE-oMjQqQpvKA4zNT4zqm- 

# Resources 

1. https://www.youtube.com/playlist?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj
2. https://www.youtube.com/playlist?list=PLn3eTxaOtL2NH5nbPHMK7gE07SqhcAjmk
4. https://www.youtube.com/playlist?list=PLA0dXqQjCx0RntJy1pqje9uHRF1Z5vZgA
5. https://www.youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR
6. https://www.youtube.com/@Life_Is_Coding/videos
7. https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
8. https://www.youtube.com/playlist?list=PL1I0ypT3n_QYnxkUgTvVzTYcAin-f3X2v
9. https://www.youtube.com/playlist?list=PL3HUvSWOJR7XRDwVVQqqWO-zyyscb8L-v
10. https://www.youtube.com/playlist?list=PLIwq-zzmw7wc5nzubsdB14fKLXwUnPzra


dependencies to use ?

opengl 
imgui : Dear ImGui: Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies.
imguizmo : Immediate mode 3D gizmo for scene editing and other controls based on Dear Imgui.
entt : Fast and reliable entity-component system (ECS)
glfw : A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input.
stb : Single-file public domain (or MIT licensed) libraries for C/C++.
tinygltf : Header only C++11 tiny glTF 2.0 library
tinyobjloader : Tiny but powerful single file wavefront obj loader
volk : Meta loader for Vulkan API.
Box2D : 2D physics engine.
sol2 : C++ <-> Lua API wrapper
cereal : A C++11 library for serialization
meshoptimizer : Mesh optimization library that makes meshes smaller and faster to render
google filament 






-----------------

TO IMPLEMENT


Custom shaders, materials, and rendering techniques.
Physically-based rendering.
Metallic workflow.
High dynamic range (HDR) rendering.
Tone mapping.
Color grading.
Auto-exposure.
Gamma correction.
Deferred shading.
Directional light.
Point lights + shadows.
Spotlights + shadows.
Screen-Space Ambient Occlusion (SSAO).
Soft shadows.
Volumetric light (spot, point).
Batching.
Instancing.
Fast Approximate Anti-Aliasing (FXAA).
Normal mapping.
Parallax mapping.
Render in texture.
Forward rendering for transparent objects.
Sky box.
Deferred decals.
Multi-camera rendering.
Lightmapping.
Soft particles.
Fully customizable vertex format.
Scene
Multiple scenes.
Full-featured scene graph.
Level-of-detail (LOD) support.
Various scene nodes.
Pivot.
Camera.
Decal.
Mesh.
Particle system.
Sprite.
Multilayer terrain.
Skinning.

User Interface (UI)
Advanced node-based UI with lots of widgets.
More than 32 widgets
Powerful layout system.
Full TTF/OTF fonts support.
Based on message passing.
Fully customizable.
GAPI-agnostic.
OS-agnostic.
Button widget.
Border widget.
Canvas widget.
Color picker widget.
Color field widget.
Check box widget.
Decorator widget.
Drop-down list widget.
Grid widget.
Image widget.
List view widget.
Popup widget.
Progress bar widget.
Scroll bar widget.
Scroll panel widget.
Scroll viewer widget.
Stack panel widget.
Tab control widget.
Text widget.
Text box widget.
Tree widget.
Window widget.
File browser widget.
File selector widget.
Docking manager widget.
NumericUpDown widget.
Vector3<f32> editor widget.
Menu widget.
Menu item widget.
Message box widget.
Wrap panel widget.
Curve editor widget.
User defined widget.


vektor
├── CMakeLists.txt
├── cmake/
│   ├── toolchains/
│   ├── sanitizers.cmake
│   ├── platform.cmake
│   └── vktr_config.cmake
│
├── tools/
│   ├── shader_compiler/
│   ├── asset_pipeline/
│   └── codegen/
│
├── vendor/
│   ├── glad/
│   ├── imgui/
│   ├── stb/
│   └── entt/
│
├── include/
│   └── vktr/
│       ├── vktr.hpp
│       ├── config.hpp
│       ├── platform.hpp
│       ├── core.hpp
│       ├── application.hpp
│       ├── renderer.hpp
│       ├── ecs.hpp
│       ├── events.hpp
│       ├── input.hpp
│       └── world.hpp
│
│       ├── core/
│       │   ├── fwd.hpp
│       │   ├── time.hpp
│       │   ├── uuid.hpp
│       │   └── memory.hpp
│       │
│       │   └── detail
│       │       ├── platform
│       │       │   ├── win32.hpp
│       │       │   ├── macos.hpp
│       │       │   └── linux.hpp
│       │       │
│       │       ├── traits
│       │       │   ├── type_traits.hpp
│       │       │   └── numeric_traits.hpp
│       │       │
│       │       └── allocators
│       │           ├── linear_allocator.hpp
│       │           ├── pool_allocator.hpp
│       │           └── tlsf_allocator.hpp
│
│       ├── application/
│       │   ├── app.hpp
│       │   ├── layer.hpp
│       │   ├── layer_stack.hpp
│       │   └── entry_point.hpp
│       │
│       │   └── detail
│       │       ├── app_singleton.hpp
│       │       ├── app_context.hpp
│       │       └── layer_dispatch.hpp
│
│       ├── events/
│       │   ├── event.hpp
│       │   ├── dispatcher.hpp
│       │   └── categories.hpp
│       │
│       │   └── detail
│       │       ├── event_traits.hpp
│       │       ├── type_id.hpp
│       │       ├── event_storage.hpp
│       │       └── dispatch_impl.hpp
│
│       ├── ecs/
│       │   ├── entity.hpp
│       │   ├── registry.hpp
│       │   ├── view.hpp
│       │   └── system.hpp
│       │
│       │   └── detail
│       │       ├── archetype
│       │       │   ├── archetype.hpp
│       │       │   ├── archetype_storage.hpp
│       │       │   └── archetype_view.hpp
│       │       │
│       │       ├── component
│       │       │   ├── component_traits.hpp
│       │       │   ├── component_storage.hpp
│       │       │   └── component_policy.hpp
│       │       │
│       │       ├── entity
│       │       │   ├── entity_handle.hpp
│       │       │   └── entity_traits.hpp
│       │       │
│       │       └── storage
│       │           ├── sparse_set.hpp
│       │           └── packed_array.hpp
│
│       ├── renderer/
│       │   ├── renderer_api.hpp
│       │   ├── command_buffer.hpp
│       │   ├── resource_handle.hpp
│       │   ├── pipeline.hpp
│       │   └── frame_graph.hpp
│       │
│       │   └── detail
│       │       ├── traits
│       │       │   ├── backend_traits.hpp
│       │       │   ├── resource_traits.hpp
│       │       │   └── shader_traits.hpp
│       │       │
│       │       ├── policy
│       │       │   ├── threading_policy.hpp
│       │       │   ├── lifetime_policy.hpp
│       │       │   └── cache_policy.hpp
│       │       │
│       │       ├── crtp
│       │       │   ├── renderer_crtp.hpp
│       │       │   └── command_encoder_crtp.hpp
│       │       │
│       │       └── impl
│       │           ├── renderer_base.hpp
│       │           ├── command_encoder.hpp
│       │           └── resource_allocator.hpp
│       │
│       ├── input/
│       │   ├── input.hpp
│       │   └── keycodes.hpp
│       │
│       │   └── detail
│       │       └── input_backend.hpp
│
│       └── world/
│           ├── scene.hpp
│           └── transform.hpp
│
│           └── detail
│               ├── scene_graph.hpp
│               └── spatial_index.hpp
│
├── src/
│   └── vktr/
│       ├── core/
│       ├── application/
│       ├── ecs/
│       ├── events/
│       ├── input/
│       ├── world/
│       └── renderer/
│           └── backend/
│               ├── opengl/
│               │   ├── gl_backend.cpp
│               │   ├── gl_command_encoder.cpp
│               │   └── gl_resource_allocator.cpp
│               │
│               ├── vulkan/
│               │   ├── vk_backend.cpp
│               │   └── vk_command_encoder.cpp
│               │
│               └── null/
│                   └── null_backend.cpp
│
└── sandbox/
    └── src/
        └── sandbox_app.cpp
