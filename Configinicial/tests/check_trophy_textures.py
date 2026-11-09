"""Trophy uses dedicated gold, marble and dark wood textures with spherical and triplanar mapping."""

from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures'

for filename in ('trophy_gold_diff_1k.jpg', 'marble_01_diff_1k.jpg', 'dark_wood_diff_1k.jpg'):
    path = textures / filename
    assert path.is_file(), f'Missing texture file: {filename}'
    with Image.open(path) as image:
        assert image.size == (1024, 1024)

main_src = (root / 'main.cpp').read_text()
assert 'LoadDiffuseTexture("Models/Textures/trophy_gold_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/marble_01_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/dark_wood_diff_1k.jpg")' in main_src
assert 'drawTrophy(modelLoc, textureModeLoc' in main_src

print('Trophy gold, marble and wood textures: OK')
