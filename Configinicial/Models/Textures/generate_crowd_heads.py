"""Generate four small crowd head atlases from the existing player-head design."""

from pathlib import Path
from tempfile import TemporaryDirectory

from PIL import Image, ImageDraw

import generate_player_head as head


def generate(output_dir):
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    people = (
        ((193, 148, 96), (43, 30, 23), 'alegria'),
        ((231, 192, 153), (100, 64, 39), 'guino'),
        ((124, 81, 56), (26, 25, 30), 'emocion'),
        ((173, 113, 77), (168, 104, 46), 'concentracion'),
    )
    with TemporaryDirectory() as scratch:
        for i, (skin, hair, expression) in enumerate(people):
            head.SKIN = skin
            head.SKIN_LIGHT = tuple(min(255, color + 12) for color in skin)
            head.SKIN_SHADOW = tuple(max(0, color - 18) for color in skin)
            head.HAIR = head.EYEBROW = hair
            head.HAIR_LIGHT = tuple(min(255, color + 18) for color in hair)
            atlas = head.generate(scratch, 128)
            face = head.make_expression(expression, 32)
            if i:
                d = ImageDraw.Draw(face)
                d.rectangle((i * 3, 4, 26 - i * 2, 4 + i), fill=hair)
            atlas.paste(face.resize((128, 128), Image.Resampling.NEAREST), (128, 128))
            atlas.save(output_dir / f'crowd_head_{i}.png', optimize=True)


if __name__ == '__main__':
    generate(Path(__file__).resolve().parent / 'player_head_textures/crowd')
