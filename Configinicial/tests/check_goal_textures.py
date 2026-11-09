"""Goal posts and net use dedicated image textures and procedural local triplanar mapping."""

from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures'

for filename in ('metal_plate_02_diff_1k.jpg', 'fabric_leather_01_diff_1k.jpg'):
    path = textures / filename
    assert path.is_file(), f'Missing texture file: {filename}'
    with Image.open(path) as image:
        assert image.size == (1024, 1024)

main_src = (root / 'main.cpp').read_text()
assert 'LoadDiffuseTexture("Models/Textures/metal_plate_02_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/fabric_leather_01_diff_1k.jpg")' in main_src
assert 'glUniform1i(textureModeLoc, 7);' in main_src

print('Goal posts and net textures: OK')
