"""Ball uses leather PBR textures with spherical projection; goalkeeper has a distinct jersey."""

from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures'

for filename in ('leather_white_diff_1k.jpg', 'fabric_leather_01_diff_1k.jpg'):
    path = textures / filename
    assert path.is_file(), f'Missing file: {filename}'
    with Image.open(path) as img:
        assert img.size == (1024, 1024)

gk_path = textures / 'goalkeeper_green_jersey.png'
assert gk_path.is_file(), 'Missing goalkeeper jersey'
with Image.open(gk_path) as img:
    assert img.size == (1024, 512)

main_src = (root / 'main.cpp').read_text()
assert 'LoadDiffuseTexture("Models/Textures/leather_white_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/fabric_leather_01_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/goalkeeper_green_jersey.png")' in main_src
assert 'glUniform1i(textureModeLoc, 6);' in main_src
assert 'texGoalkeeperJersey' in main_src

print('Ball leather and goalkeeper textures: OK')
