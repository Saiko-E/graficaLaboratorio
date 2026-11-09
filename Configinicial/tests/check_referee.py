"""Referee shirt is reproducible, patterned and used by one player per match scene."""

import runpy
from pathlib import Path
from tempfile import TemporaryDirectory

from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures'
asset = textures / 'referee_yellow_jersey.png'
generate = runpy.run_path(str(textures / 'generate_referee_jersey.py'))['generate']
with TemporaryDirectory() as folder:
    image = generate(Path(folder) / asset.name)
    assert image.size == (1024, 512)
    assert (Path(folder) / asset.name).read_bytes() == asset.read_bytes()
    assert image.getpixel((120, 180)) != image.getpixel((260, 300))
    assert image.getpixel((260, 180)) != image.getpixel((772, 180))
    with Image.open(asset) as saved:
        assert saved.format == 'PNG'

source = (root / 'main.cpp').read_text()
assert 'LoadDiffuseTexture("Models/Textures/referee_yellow_jersey.png")' in source
assert 'texRefereeJersey, texHead, texBlack, texSkin, texShoes' in source
assert 'if (matchScene == SCENE_KICKOFF || matchScene == SCENE_PUMAS_GOAL ||' in source
assert 'matchScene == SCENE_ATLANTE_GOAL || matchScene == SCENE_PENALTIES ||' in source
assert 'matchScene == SCENE_FINAL)' in source
assert 'texRefereeJersey' in source.split('GLuint textures[] =', 1)[1]
print('Referee texture and scenes: OK')
