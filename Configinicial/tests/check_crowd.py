"""Both stands have their own jersey and visibly different faces."""

from pathlib import Path
from tempfile import TemporaryDirectory

from PIL import Image

root = Path(__file__).resolve().parents[1]
textures = root / "Models/Textures"
scene = (root / "main.cpp").read_text()

with TemporaryDirectory() as directory:
    from sys import path
    path.insert(0, str(textures))
    from generate_crowd_heads import generate

    generate(Path(directory))
    faces = [Image.open(Path(directory) / f"crowd_head_{i}.png") for i in range(4)]
    assert all(face.size == (512, 384) for face in faces)
    assert len({face.getpixel((192, 240)) for face in faces}) >= 3  # Different skin tones.
    assert len({face.getpixel((192, 140)) for face in faces}) >= 3  # Different hairstyles.
    assert len({face.crop((128, 128, 256, 256)).tobytes() for face in faces}) == 4

assert all((textures / f"player_head_textures/crowd/crowd_head_{i}.png").is_file() for i in range(4))

assert "drawCrowd(modelLoc" in scene
assert "if (section == 5) continue;" in scene.split("void drawCrowd(")[-1]
assert "side < 0 ? texPumas : texAtlante" in scene.split("void drawCrowd(")[-1]
print("Crowd faces and team stands: OK")
