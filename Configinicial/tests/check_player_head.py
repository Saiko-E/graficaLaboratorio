"""Check the generated head atlas without writing into game assets."""

import runpy
from pathlib import Path
from tempfile import TemporaryDirectory

from PIL import Image


textures = Path(__file__).resolve().parents[1] / "Models" / "Textures"
head = runpy.run_path(str(textures / "generate_player_head.py"))
with TemporaryDirectory() as folder:
    atlas = head["generate"](folder, 64)
    assert atlas.size == (256, 192)
    assert Image.open(Path(folder) / "head_frente.png").size == (64, 64)
    assert atlas.getpixel((32, 32)) != atlas.getpixel((96, 32))
    assert head["make_expression"]("enojo", 64).tobytes() != head["make_expression"]("neutral", 64).tobytes()
    head["generate_emotions"](folder, 64)
    neutral = Image.open(Path(folder) / "atlas_emociones" / "atlas_neutral.png")
    assert neutral.size == (256, 192)
    assert neutral.getpixel((96, 68)) != neutral.getpixel((96, 96))
    assert neutral.getpixel((96, 96)) != neutral.getpixel((96, 112))
print("Head atlas and expressions: OK")

scene = (textures.parents[1] / "main.cpp").read_text()
assert scene.count("drawPlayer(modelLoc, VAO, jerseyVAO, headVAO, modelTemp") == 7
assert scene.count("drawPlayer(") == 9  # Seven calls plus declaration and definition; no legacy players.
assert "const bool penaltyPlayersVisible = matchScene == SCENE_PENALTIES;" in scene
print("Scene player calls: OK")
