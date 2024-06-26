
# EFlame Engine

A Simple OpenGL game engine which I intend to use as a framework to practice computer physics in.

### Screenshots
![image](https://github.com/Maploop/EFlame/assets/76199586/4680e148-eef2-4346-855b-7607d7585ebe)


### Innerworkings
Using libraries such as:
* GLFW
* GLM
* PhysX (not as of now)
* imgui

And for logging I'm using my own single-class logging API which I just call "Logger", here's how it looks in the console!
```
[INFO]: StartupHandler > EFlame Engine version 1.0-PRI internal "E" starting...
[INFO]: (CALLBACK) StartupHandler > "gladLoadGL" -> OpenGL v3.3 has been initialized.
[INFO]: ShaderHandler > Loading in Shader './sh_res/sh_shader/Core.vs'...
[INFO]: PostProcessing > Initializing PostProcessManager...
[INFO]: ShaderHandler > Loading in Shader './sh_res/sh_shader/Framebuffer.vs'...
[INFO]: PostProcessing > PostProcessManager initialized successfully!
[INFO]: UIHandler > Initializing ImGui with OpenGL hook...
[INFO]: ModelHandler > Loading model "./sh_res/sh_3d/sword/scene.gltf"...
[INFO]: TextureHandler > Loading in texture "./sh_res/sh_3d/sword/textures/Object001_mtl_baseColor.jpeg"...
[INFO]: ModelHandler > Loading model "./sh_res/sh_3d/tomb/scene.gltf"...
[INFO]: TextureHandler > Loading in texture "./sh_res/sh_3d/tomb/textures/Yakovlevka_01.1001_baseColor.jpeg"...
[INFO]: ModelHandler > Loading model "./sh_res/sh_3d/statue/scene.gltf"...
[INFO]: TextureHandler > Loading in texture "./sh_res/sh_3d/statue/170516_mia337_032122_600_200Kfaces_8192px_OBJ_baseColor.jpg"...
[INFO]: ModelHandler > Loading model "./sh_res/sh_3d/ground/scene.gltf"...
[INFO]: ModelHandler > Loading model "./sh_res/sh_3d/grass/scene.gltf"...
[INFO]: PostProcessing > Skybox shader is loading...
[INFO]: ShaderHandler > Loading in Shader './sh_res/sh_shader/Skybox.vs'...
[INFO]: PostProcessing > Skybox shader is done loading!
```
