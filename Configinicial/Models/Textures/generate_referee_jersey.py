"""Generate the referee's 1024x512 front/back shirt atlas for jerseyVAO."""

from pathlib import Path

from PIL import Image, ImageDraw


def generate(path):
    image = Image.new('RGB', (1024, 512), (247, 205, 45))
    draw = ImageDraw.Draw(image)
    for offset in (0, 512):
        draw.rectangle((offset + 8, 8, offset + 504, 42), fill=(30, 31, 34))
        draw.rectangle((offset + 8, 465, offset + 504, 502), fill=(30, 31, 34))
        draw.polygon(((offset + 192, 8), (offset + 256, 80),
                      (offset + 320, 8)), fill=(30, 31, 34))
        draw.line((offset + 20, 98, offset + 492, 98), fill=(255, 225, 110), width=5)
    # Two black chest pockets and a small referee badge on the front.
    for left in (80, 350):
        draw.rounded_rectangle((left, 142, left + 82, 211), radius=7,
                               fill=(29, 31, 34), outline=(255, 237, 145), width=4)
        draw.line((left + 13, 160, left + 69, 160), fill=(255, 237, 145), width=3)
    draw.ellipse((219, 145, 293, 219), fill=(29, 31, 34))
    draw.ellipse((228, 154, 284, 210), outline=(255, 237, 145), width=5)
    draw.polygon(((256, 162), (274, 182), (256, 202), (238, 182)),
                 fill=(255, 237, 145))
    # Back seam, distinct from the badge and pockets on the front.
    draw.line((768, 110, 768, 435), fill=(232, 179, 33), width=6)
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    image.save(path, optimize=True)
    return image


if __name__ == '__main__':
    generate(Path(__file__).with_name('referee_yellow_jersey.png'))
