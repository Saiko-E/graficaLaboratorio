"""Drones and bird use dedicated Poly Haven PBR textures and procedural mapping."""

from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures'

files = (
    'blue_metal_plate_diff_1k.jpg',
    'metal_plate_04_diff_1k.jpg',
    'rubber_tiles_diff_1k.jpg',
    'fabric_leather_02_diff_1k.jpg',
    'rough_wood_diff_1k.jpg'
)

for filename in files:
    path = textures / filename
    assert path.is_file(), f'Missing file: {filename}'
    with Image.open(path) as img:
        assert img.size == (1024, 1024)

eye_path = textures / 'bird_eye.png'
assert eye_path.is_file(), 'Missing bird_eye.png'
with Image.open(eye_path) as img:
    assert img.size == (512, 512)

main_src = (root / 'main.cpp').read_text()
assert 'LoadDiffuseTexture("Models/Textures/blue_metal_plate_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/metal_plate_04_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/rubber_tiles_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/fabric_leather_02_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/rough_wood_diff_1k.jpg")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/bird_eye.png")' in main_src

assert 'drawDrone(modelLoc, textureModeLoc' in main_src
assert 'drawBird(modelLoc, textureModeLoc' in main_src
assert 'texBirdEye' in main_src

print('Drone and bird PBR textures: OK')
