"""The bleachers and wooden seats have separate CC0 maps; all towers dim."""

from pathlib import Path

from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures'
for filename in ('gravel_concrete_03_diff_1k.jpg', 'old_wooden_floor_02_diff_1k.jpg'):
    with Image.open(textures / filename) as image:
        assert image.format == 'JPEG' and image.size == (1024, 1024)
        assert image.getpixel((30, 30)) != image.getpixel((600, 600))

scene = (root / 'main.cpp').read_text()
shader = (root / 'Shader/lighting.frag').read_text()
assert 'LoadDiffuseTexture("Models/Textures/gravel_concrete_03_diff_1k.jpg")' in scene
assert 'LoadDiffuseTexture("Models/Textures/old_wooden_floor_02_diff_1k.jpg")' in scene
bleachers = scene.split('void drawBleachers(GLint modelLoc', 2)[-1].split('void drawCrowd(', 1)[0]
assert 'glm::vec3(5.9f, 0.05f, 1.85f), texWalkway' in bleachers
assert 'glm::vec3(90.0f, height, 2.0f), texConcrete' in bleachers
assert 'glm::vec3(7.15f, 0.23f, 1.15f), texWood' in bleachers
assert 'glm::vec3(7.15f, 0.60f, 0.19f), texWood' in bleachers
assert 'texGravelConcrete' in scene.split('GLuint textures[] =', 1)[1]
assert 'texWood' in scene.split('GLuint textures[] =', 1)[1]
assert 'texGravelConcrete, texFacade,' in scene
lights = scene.split('// Each tower illuminates', 1)[1].split('// Set material properties', 1)[0]
assert lights.count('for (int i = 0; i < 4; ++i)') == 2
assert 'floodlightPositions[i]' in lights
assert 'stadiumLightIntensity' in lights
assert 'int lightIndex = 4 + team * 2 + lamp;' in scene
assert '#define NUMBER_OF_POINT_LIGHTS 8' in shader
assert '#define NUMBER_OF_SPOT_LIGHTS 4' in shader
assert 'spotLights[NUMBER_OF_SPOT_LIGHTS]' in shader
assert 'for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; i++)' in shader
assert 'key == GLFW_KEY_UP' in scene and 'key == GLFW_KEY_DOWN' in scene
assert 'face * 0.27f' in scene
assert '0.20f + 0.45f * stadiumLightIntensity' in scene
assert 'uniform float brightness = 1.0f;' in (root / 'Shader/lamp.frag').read_text()
print('Gravel concrete, wooden seats and four dimmable towers: OK')
