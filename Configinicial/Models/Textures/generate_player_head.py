"""Texturas de las seis caras de una cabeza cúbica, estilo pixel art.

Instalar: python -m pip install pillow
Ejecutar: python generate_player_head.py
Genera seis PNG de 512 x 512 y un atlas RGBA en cruz de 2048 x 1536.
También genera 12 rostros PNG independientes en rostros/ y sus atlas
completos en atlas_emociones/. No necesita imágenes externas.

Coordenadas de las celdas del atlas (columna, fila), desde arriba a la izquierda:
arriba=(1,0), izquierda=(0,1), frente=(1,1), derecha=(2,1),
atras=(3,1), abajo=(1,2).
Izquierda y derecha son las del personaje. Las caras laterales y la nuca
se ven desde fuera con la coronilla arriba. En la cara superior, el borde
inferior conecta con la frente. En la inferior, el borde superior conecta
con la barbilla. Los UV concretos y sus giros dependen de tu modelo.

Usar filtrado NEAREST. Para evitar sangrado entre celdas con mipmaps,
es preferible asignar las seis imágenes por separado o añadir padding.
La expresión alegria conserva exactamente el diseño anterior.
"""

from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

GRID_SIZE = 32
OUTPUT_SIZE = 512

# Cambia SKIN para igualar el tono de los brazos del personaje.
SKIN = (193, 148, 96)
SKIN_LIGHT = (201, 156, 104)
SKIN_SHADOW = (178, 129, 80)
HAIR = (43, 30, 23)
HAIR_LIGHT = (58, 40, 28)
EYEBROW = (58, 38, 25)
EYE_WHITE = (245, 240, 223)
PUPIL = (38, 28, 22)
MOUTH = (103, 61, 39)


def make_face(output_size=OUTPUT_SIZE):
    """Dibuja a 32 x 32 y amplía sin suavizado ni dependencias adicionales."""
    if not isinstance(output_size, int) or output_size < GRID_SIZE or output_size % GRID_SIZE:
        raise ValueError('El tamaño debe ser un múltiplo entero de 32, mayor o igual a 32.')

    image = Image.new('RGB', (GRID_SIZE, GRID_SIZE), SKIN)
    d = ImageDraw.Draw(image)

    # Cabello corto: banda superior irregular y patillas discretas.
    d.rectangle((0, 0, 31, 3), fill=HAIR)
    d.rectangle((0, 4, 4, 7), fill=HAIR)
    d.rectangle((27, 4, 31, 7), fill=HAIR)
    d.rectangle((5, 4, 10, 4), fill=HAIR)
    d.rectangle((11, 4, 15, 5), fill=HAIR)
    d.rectangle((16, 4, 20, 4), fill=HAIR)
    d.rectangle((0, 8, 1, 10), fill=HAIR)
    d.rectangle((30, 8, 31, 10), fill=HAIR)
    d.rectangle((3, 1, 11, 1), fill=HAIR_LIGHT)
    d.rectangle((17, 1, 26, 1), fill=HAIR_LIGHT)

    # Cejas y ojos simétricos, grandes para leerse a distancia.
    d.rectangle((6, 10, 11, 10), fill=EYEBROW)
    d.rectangle((20, 10, 25, 10), fill=EYEBROW)
    d.rectangle((6, 13, 11, 15), fill=EYE_WHITE)
    d.rectangle((20, 13, 25, 15), fill=EYE_WHITE)
    d.rectangle((9, 13, 10, 15), fill=PUPIL)
    d.rectangle((21, 13, 22, 15), fill=PUPIL)

    # Nariz pequeña y mejillas con contraste tenue.
    d.rectangle((15, 17, 16, 19), fill=SKIN_LIGHT)
    d.rectangle((15, 20, 17, 20), fill=SKIN_SHADOW)
    d.rectangle((5, 19, 8, 19), fill=SKIN_LIGHT)
    d.rectangle((23, 19, 26, 19), fill=SKIN_LIGHT)

    # Sonrisa escalonada, sin contorno alrededor de la cara.
    d.point((11, 23), fill=MOUTH)
    d.rectangle((12, 24, 13, 24), fill=MOUTH)
    d.rectangle((14, 25, 17, 25), fill=MOUTH)
    d.rectangle((18, 24, 19, 24), fill=MOUTH)
    d.point((20, 23), fill=MOUTH)

    return image.resize((output_size, output_size), Image.Resampling.NEAREST)


def make_right():
    """Lado derecho del personaje: frente a la izquierda, nuca a la derecha."""
    image = Image.new('RGB', (GRID_SIZE, GRID_SIZE), SKIN)
    d = ImageDraw.Draw(image)
    # Contorno del cabello: conecta con el frente y desciende hacia la nuca.
    d.polygon([(0, 0), (31, 0), (31, 21), (25, 21), (25, 18),
               (22, 18), (22, 13), (18, 13), (18, 9), (8, 9),
               (8, 7), (2, 7), (2, 10), (0, 10)], fill=HAIR)
    d.rectangle((7, 2, 15, 2), fill=HAIR_LIGHT)
    d.rectangle((21, 4, 27, 4), fill=HAIR_LIGHT)
    # Oreja integrada en el plano del cubo, sin repetir ojos ni boca.
    d.rectangle((15, 13, 20, 19), fill=SKIN_LIGHT)
    d.rectangle((17, 15, 19, 18), fill=SKIN_SHADOW)
    d.point((17, 18), fill=SKIN)
    return image


def make_back():
    image = Image.new('RGB', (GRID_SIZE, GRID_SIZE), SKIN)
    d = ImageDraw.Draw(image)
    d.rectangle((0, 0, 31, 21), fill=HAIR)
    d.rectangle((5, 22, 26, 22), fill=HAIR)
    d.rectangle((10, 23, 21, 23), fill=HAIR)
    for box in [(5, 3, 12, 3), (19, 3, 26, 3),
                (8, 9, 13, 9), (20, 12, 25, 12), (13, 18, 18, 18)]:
        d.rectangle(box, fill=HAIR_LIGHT)
    return image


def make_top():
    image = Image.new('RGB', (GRID_SIZE, GRID_SIZE), HAIR)
    d = ImageDraw.Draw(image)
    # Mechones discretos, con bordes uniformes para unir las caras.
    for box in [(5, 5, 11, 5), (18, 5, 25, 5), (7, 12, 14, 12),
                (21, 15, 26, 15), (11, 21, 18, 21), (5, 26, 10, 26)]:
        d.rectangle(box, fill=HAIR_LIGHT)
    return image


def make_bottom():
    # Barbilla: mantener el borde del mismo tono que las caras contiguas.
    image = Image.new('RGB', (GRID_SIZE, GRID_SIZE), SKIN)
    d = ImageDraw.Draw(image)
    d.rectangle((9, 11, 22, 22), fill=SKIN_SHADOW)
    d.rectangle((11, 9, 20, 24), fill=SKIN_SHADOW)
    return image


ATLAS_CELLS = {
    'arriba': (1, 0),
    'izquierda': (0, 1),
    'frente': (1, 1),
    'derecha': (2, 1),
    'atras': (3, 1),
    'abajo': (1, 2),
}

EMOTIONS = (
    'alegria', 'emocion', 'concentracion', 'enojo',
    'tristeza', 'sorpresa', 'miedo', 'guino',
    'cansancio', 'neutral', 'llanto', 'confianza',
)


def make_expression(emotion, size=OUTPUT_SIZE):
    """Un PNG frontal por expresión; las dimensiones y los bordes coinciden."""
    if emotion not in EMOTIONS:
        raise ValueError(f'Expresión desconocida: {emotion}. Opciones: {EMOTIONS}')
    if not isinstance(size, int) or size < GRID_SIZE or size % GRID_SIZE:
        raise ValueError('El tamaño debe ser múltiplo de 32.')
    if emotion == 'alegria':
        return make_face(size)

    image = make_face(GRID_SIZE)
    d = ImageDraw.Draw(image)
    # Limpiar únicamente la zona de expresión, conservando cabello y bordes.
    d.rectangle((3, 9, 28, 27), fill=SKIN)
    d.rectangle((15, 17, 16, 19), fill=SKIN_LIGHT)
    d.rectangle((15, 20, 17, 20), fill=SKIN_SHADOW)

    def stroke(points, color=EYEBROW, width=1):
        d.line(points, fill=color, width=width)

    # Ojos: abiertos, entrecerrados o cerrados, según la expresión.
    if emotion in ('emocion', 'llanto'):
        stroke([(6, 15), (8, 13), (10, 13), (12, 15)], PUPIL)
        stroke([(19, 15), (21, 13), (23, 13), (25, 15)], PUPIL)
    elif emotion == 'guino':
        d.rectangle((6, 13, 11, 15), fill=EYE_WHITE)
        d.rectangle((9, 13, 10, 15), fill=PUPIL)
        stroke([(20, 14), (22, 15), (25, 13)], PUPIL)
    elif emotion in ('concentracion', 'enojo', 'cansancio', 'confianza'):
        for x, pupil_x in ((6, 9), (20, 21)):
            d.rectangle((x, 14, x+5, 15), fill=EYE_WHITE)
            d.rectangle((pupil_x, 14, pupil_x+1, 15), fill=PUPIL)
        if emotion == 'cansancio':
            stroke([(6, 17), (11, 17)], SKIN_SHADOW)
            stroke([(20, 17), (25, 17)], SKIN_SHADOW)
    else:
        big = emotion in ('sorpresa', 'miedo')
        for x, pupil_x in ((6, 9), (20, 21)):
            d.rectangle((x, 12 if big else 13, x+5, 16 if big else 15), fill=EYE_WHITE)
            d.rectangle((pupil_x, 13, pupil_x+1, 15), fill=PUPIL)

    if emotion == 'enojo':
        brows = [[(6, 9), (12, 12)], [(19, 12), (25, 9)]]
    elif emotion == 'concentracion':
        brows = [[(6, 11), (12, 12)], [(19, 12), (25, 11)]]
    elif emotion in ('tristeza', 'miedo', 'llanto'):
        brows = [[(6, 11), (11, 9)], [(20, 9), (25, 11)]]
    elif emotion in ('sorpresa', 'emocion'):
        brows = [[(6, 9), (8, 8), (11, 8)], [(20, 8), (23, 8), (25, 9)]]
    elif emotion == 'confianza':
        brows = [[(6, 9), (11, 10)], [(20, 11), (25, 11)]]
    else:
        brows = [[(6, 10), (11, 10)], [(20, 10), (25, 10)]]
    for brow in brows:
        stroke(brow)

    if emotion == 'emocion':
        d.rectangle((10, 22, 21, 25), fill=MOUTH)
        d.rectangle((12, 26, 19, 27), fill=MOUTH)
        d.rectangle((11, 22, 20, 22), fill=EYE_WHITE)
        d.rectangle((13, 26, 18, 26), fill=(179, 96, 81))
    elif emotion in ('tristeza', 'llanto'):
        stroke([(11, 25), (13, 23), (18, 23), (20, 25)], MOUTH)
    elif emotion == 'enojo':
        stroke([(11, 25), (13, 24), (18, 24), (20, 25)], MOUTH)
    elif emotion == 'concentracion':
        stroke([(12, 24), (19, 24)], MOUTH)
        stroke([(14, 25), (17, 25)], SKIN_SHADOW)
    elif emotion in ('sorpresa', 'miedo'):
        d.rectangle((14, 22, 17, 26), fill=MOUTH)
        d.rectangle((13, 23, 18, 25), fill=MOUTH)
        if emotion == 'miedo':
            d.rectangle((14, 22, 17, 22), fill=EYE_WHITE)
            d.rectangle((27, 10, 27, 12), fill=(116, 186, 220))
            d.rectangle((26, 13, 28, 14), fill=(116, 186, 220))
    elif emotion == 'cansancio':
        d.rectangle((14, 23, 17, 25), fill=MOUTH)
    elif emotion == 'neutral':
        stroke([(12, 24), (19, 24)], MOUTH)
    elif emotion == 'confianza':
        stroke([(11, 25), (17, 25), (20, 23)], MOUTH)
    else:  # Guiño con sonrisa.
        stroke([(11, 23), (13, 25), (18, 25), (20, 23)], MOUTH)

    if emotion == 'llanto':
        for x in (7, 23):
            d.rectangle((x, 16, x+1, 22), fill=(107, 174, 215))
            d.point((x, 23), fill=(166, 211, 233))
    return image.resize((size, size), Image.Resampling.NEAREST)


def generate_emotions(output_dir, size=OUTPUT_SIZE):
    """Guarda cada rostro y cada atlas en archivos PNG independientes."""
    output_dir = Path(output_dir)
    faces_dir = output_dir / 'rostros'
    atlas_dir = output_dir / 'atlas_emociones'
    faces_dir.mkdir(parents=True, exist_ok=True)
    atlas_dir.mkdir(parents=True, exist_ok=True)
    # Reutiliza las otras cinco caras ya generadas.
    base_atlas = generate(output_dir, size)
    col, row = ATLAS_CELLS['frente']
    for emotion in EMOTIONS:
        face = make_expression(emotion, size)
        face.save(faces_dir / f'rostro_{emotion}.png', optimize=True)
        atlas = base_atlas.copy()
        atlas.paste(face, (col*size, row*size))
        atlas.save(atlas_dir / f'atlas_{emotion}.png', optimize=True)

    # Hoja comparativa para elegir expresiones; los PNG individuales son aparte.
    tile, label_height, margin = 192, 36, 16
    preview = Image.new('RGB', (4*(tile+margin)+margin,
                                3*(tile+label_height+margin)+margin), (239, 242, 246))
    d = ImageDraw.Draw(preview)
    try:
        font = ImageFont.truetype('DejaVuSans.ttf', 18)
    except OSError:
        font = ImageFont.load_default()
    labels = {'alegria': 'Alegría', 'emocion': 'Emoción',
              'concentracion': 'Concentración', 'enojo': 'Enojo',
              'tristeza': 'Tristeza', 'sorpresa': 'Sorpresa',
              'miedo': 'Miedo', 'guino': 'Guiño', 'cansancio': 'Cansancio',
              'neutral': 'Neutral', 'llanto': 'Llanto', 'confianza': 'Confianza'}
    for index, emotion in enumerate(EMOTIONS):
        x = margin + (index % 4)*(tile+margin)
        y = margin + (index // 4)*(tile+label_height+margin)
        preview.paste(make_expression(emotion, tile), (x, y))
        d.text((x+tile/2, y+tile+label_height/2), labels[emotion],
               fill=(30, 40, 55), font=font, anchor='mm')
    preview.save(output_dir / 'vista_emociones.png', optimize=True)


def generate(output_dir, size=OUTPUT_SIZE):
    if not isinstance(size, int) or size < GRID_SIZE or size % GRID_SIZE:
        raise ValueError('El tamaño de cada cara debe ser un múltiplo de 32.')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    right = make_right()
    small_faces = {
        'frente': make_face(GRID_SIZE),
        'atras': make_back(),
        'derecha': right,
        'izquierda': right.transpose(Image.Transpose.FLIP_LEFT_RIGHT),
        'arriba': make_top(),
        'abajo': make_bottom(),
    }
    atlas = Image.new('RGBA', (size*4, size*3), (0, 0, 0, 0))
    for name, small_image in small_faces.items():
        face = small_image.resize((size, size), Image.Resampling.NEAREST)
        face.save(output_dir / f'head_{name}.png', optimize=True)
        col, row = ATLAS_CELLS[name]
        atlas.paste(face, (col*size, row*size))
    atlas.save(output_dir / 'player_head_atlas.png', optimize=True)
    return atlas


if __name__ == '__main__':
    folder = Path(__file__).resolve().parent / 'player_head_textures'
    generate_emotions(folder)
    print(f'{len(EMOTIONS)} rostros PNG: {folder / "rostros"}')
    print(f'{len(EMOTIONS)} atlas PNG: {folder / "atlas_emociones"}')

