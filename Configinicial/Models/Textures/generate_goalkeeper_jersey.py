"""Generate the goalkeeper's 1024x512 front/back shirt atlas for jerseyVAO."""

from pathlib import Path
from PIL import Image, ImageDraw


def generate(path):
    # Vibrant neon emerald / keeper green base
    image = Image.new('RGB', (1024, 512), (32, 185, 95))
    draw = ImageDraw.Draw(image)

    # Collar and waistbands for front and back
    for offset in (0, 512):
        # Collar and shoulder trim
        draw.rectangle((offset + 8, 8, offset + 504, 38), fill=(18, 20, 24))
        draw.rectangle((offset + 8, 468, offset + 504, 504), fill=(18, 20, 24))
        # V-neck shape
        draw.polygon(((offset + 190, 8), (offset + 256, 75),
                      (offset + 322, 8)), fill=(18, 20, 24))
        # Shoulder accent stripes
        for y in (50, 68, 86):
            draw.line((offset + 30, y, offset + 482, y), fill=(15, 230, 140), width=4)

    # Front design: dynamic geometric keeper pattern & chest shield
    for y in range(120, 380, 40):
        draw.line((80, y, 432, y + 25), fill=(20, 140, 75), width=6)
        draw.line((80, y + 25, 432, y), fill=(15, 230, 140), width=4)

    # Front emblem / shield
    draw.polygon(((256, 150), (295, 180), (256, 245), (217, 180)),
                 fill=(18, 20, 24), outline=(245, 245, 245), width=4)
    draw.ellipse((236, 175, 276, 215), fill=(245, 245, 245))
    draw.ellipse((244, 183, 268, 207), fill=(18, 20, 24))

    # Back design: Large number 1
    # Number 1 vertical bar and top hook
    draw.rectangle((740, 160, 795, 380), fill=(245, 245, 245), outline=(18, 20, 24), width=4)
    draw.polygon(((740, 220), (700, 200), (740, 160)), fill=(245, 245, 245), outline=(18, 20, 24), width=4)
    draw.rectangle((705, 360, 830, 395), fill=(245, 245, 245), outline=(18, 20, 24), width=4)

    # Arc text bar above number
    draw.rectangle((670, 120, 865, 145), fill=(18, 20, 24))

    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    image.save(path, optimize=True)
    return image


if __name__ == '__main__':
    generate(Path(__file__).with_name('goalkeeper_green_jersey.png'))
