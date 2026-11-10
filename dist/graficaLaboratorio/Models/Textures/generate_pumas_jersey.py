"""Atlas 1024 x 512: frente izquierdo y espalda derecha, para modelo cúbico.

Requiere Pillow: python -m pip install pillow
Ejecutar: python generate_pumas_jersey.py
La espalda conserva los dibujos y coordenadas del código original.
El frente aproxima la referencia mediante curvas, sin usar la foto como textura.
"""

from pathlib import Path
import math
import random
from PIL import Image, ImageDraw, ImageFont

NAVY = (14, 30, 61)
GOLD = (218, 176, 80)
SCALE = 4  # Suavizado solo del frente: la espalda se dibuja a resolución original.
SIZE = 512


def find_font():
    candidates = [
        Path('/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf'),
        Path('/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf'),
        Path('C:/Windows/Fonts/arialbd.ttf'),
        Path('/System/Library/Fonts/Supplemental/Arial Bold.ttf'),
    ]
    for path in candidates:
        if path.is_file():
            return str(path)
    try:
        ImageFont.truetype('DejaVuSans-Bold.ttf', 20)
        return 'DejaVuSans-Bold.ttf'
    except OSError as exc:
        raise RuntimeError('Instala DejaVu Sans Bold o añade su ruta en find_font().') from exc


def bezier_shape(draw, commands, fill):
    """Rellena un contorno con segmentos M, L y curvas cúbicas C."""
    points = []
    current = (0, 0)
    for command in commands:
        kind, *v = command
        if kind in ('M', 'L'):
            current = (v[0], v[1])
            points.append(current)
        elif kind == 'C':
            start = current
            a, b, end = (v[0], v[1]), (v[2], v[3]), (v[4], v[5])
            for i in range(1, 41):
                t = i / 40
                u = 1 - t
                points.append(tuple(u**3 * start[j] + 3*u*u*t*a[j]
                                    + 3*u*t*t*b[j] + t**3*end[j]
                                    for j in (0, 1)))
            current = end
        else:
            raise ValueError(f'Comando desconocido: {kind}')
    draw.polygon([(round(x*SCALE), round(y*SCALE)) for x, y in points], fill=fill)


def make_front():
    front = Image.new('RGB', (SIZE*SCALE, SIZE*SCALE), NAVY)
    d = ImageDraw.Draw(front)

    def line(points, color, width=1):
        d.line([(round(x*SCALE), round(y*SCALE)) for x, y in points],
               fill=color, width=max(1, round(width*SCALE)), joint='curve')

    def ellipse(box, color, width=1):
        d.ellipse(tuple(round(v*SCALE) for v in box), outline=color,
                  width=max(1, round(width*SCALE)))

    # Patrón geométrico discreto, inspirado en el contraste de la referencia.
    # Semilla fija: la imagen es reproducible en cada ejecución.
    rng = random.Random(10)
    for row in range(12):
        for col in range(12):
            x = col*46 + rng.randint(-10, 10)
            y = row*46 + rng.randint(-10, 10)
            ink = rng.choice([(17, 35, 66), (18, 37, 69), (20, 40, 73)])
            if (row + col) % 3 == 0:
                line([(x, y+25), (x, y), (x+25, y), (x+25, y+18),
                      (x+10, y+18), (x+10, y+8)], ink, 2)
            elif (row + col) % 3 == 1:
                line([(x, y+15), (x+14, y), (x+28, y+15),
                      (x+14, y+30), (x, y+15)], ink, 2)
                line([(x+7, y+15), (x+21, y+15)], ink, 2)
            else:
                ellipse((x, y, x+28, y+28), ink, 2)
                line([(x+14, y-4), (x+14, y+32)], ink, 1)

    # Sello circular tenue detrás del emblema central.
    ellipse((72, 126, 440, 451), (22, 43, 77), 4)
    ellipse((82, 135, 430, 442), (18, 37, 70), 2)

    # Cuello azul oscuro y costuras raglán, sin los triángulos dorados.
    line([(3, 29), (87, 75), (112, 156)], (21, 38, 68), 2)
    line([(509, 29), (425, 75), (400, 156)], (21, 38, 68), 2)
    d.ellipse((185*SCALE, -48*SCALE, 327*SCALE, 45*SCALE),
              fill=(7, 15, 37), outline=(25, 39, 66), width=4*SCALE)

    # Cabeza de puma: bóveda ancha, ojos rectos y hocico geométrico.
    # La concavidad inferior forma el espacio azul central del rostro.
    emblem = Image.new('L', front.size, 0)
    mask_draw = ImageDraw.Draw(emblem)
    bezier_shape(mask_draw, [
        ('M', 256, 158),
        ('C', 226, 158, 202, 173, 158, 178),
        ('C', 127, 181, 112, 194, 112, 219),
        ('C', 112, 239, 126, 249, 137, 268),
        ('C', 147, 286, 147, 310, 151, 334),
        ('L', 157, 365),
        ('C', 158, 375, 163, 378, 174, 378),
        ('L', 221, 378),
        ('C', 236, 378, 236, 359, 220, 359),
        ('L', 184, 357), ('L', 194, 277),
        ('C', 178, 273, 169, 263, 165, 247),
        ('L', 221, 247), ('L', 215, 321),
        ('C', 214, 325, 216, 327, 221, 327),
        ('L', 291, 327),
        ('C', 296, 327, 298, 325, 297, 321),
        ('L', 291, 247), ('L', 347, 247),
        ('C', 343, 263, 334, 273, 318, 277),
        ('L', 328, 357), ('L', 292, 359),
        ('C', 276, 359, 276, 378, 291, 378),
        ('L', 338, 378),
        ('C', 349, 378, 354, 375, 355, 365),
        ('L', 361, 334),
        ('C', 365, 310, 365, 286, 375, 268),
        ('C', 386, 249, 400, 239, 400, 219),
        ('C', 400, 194, 385, 181, 354, 178),
        ('C', 310, 173, 286, 158, 256, 158),
    ], 255)
    mask_draw.rounded_rectangle((204*SCALE, 403*SCALE, 308*SCALE, 425*SCALE),
                                radius=11*SCALE, fill=255)

    # Dorado cálido con variación muy ligera, sin sombras de la fotografía.
    gold_layer = Image.new('RGB', front.size)
    gd = ImageDraw.Draw(gold_layer)
    for y in range(front.height):
        blend = (math.cos((y / SCALE - 170) / 270 * math.pi) + 1) / 2
        color = tuple(round(a + (b-a)*blend)
                      for a, b in zip((204, 159, 70), (235, 198, 120)))
        gd.line((0, y, front.width, y), fill=color)
    front.paste(gold_layer, (0, 0), emblem)
    return front.resize((SIZE, SIZE), Image.Resampling.LANCZOS)


def make_atlas(output):
    font = find_font()
    image = Image.new('RGB', (1024, 512), NAVY)
    image.paste(make_front(), (0, 0))
    draw = ImageDraw.Draw(image)

    # ESPALDA ORIGINAL: mismas coordenadas, colores, tamaños y orden.
    offset = 512
    draw.polygon([(offset+18, 18), (offset+88, 18), (offset+18, 110)], fill=GOLD)
    draw.polygon([(offset+494, 18), (offset+424, 18), (offset+494, 110)], fill=GOLD)
    draw.arc((offset+194, -65, offset+318, 58), 0, 180, fill=GOLD, width=12)
    draw.rectangle((offset+36, 468, offset+476, 482), fill=GOLD)
    for xy, text, size in [((768, 116), 'PUMAS', 36),
                           ((768, 275), '10', 174),
                           ((768, 405), 'UNAM', 29)]:
        draw.text(xy, text, fill=GOLD, font=ImageFont.truetype(font, size), anchor='mm')
    image.save(output, optimize=True)
    return image


if __name__ == '__main__':
    destination = Path(__file__).resolve().with_name('pumas_navy_jersey.png')
    make_atlas(destination)
    print(f'Textura guardada: {destination}')
