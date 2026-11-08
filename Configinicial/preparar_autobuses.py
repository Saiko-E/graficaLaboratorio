from pathlib import Path
import re

root = Path(__file__).resolve().parent
folder = root / 'Models' / 'autobus'
source_obj = folder / 'carroceria.obj'
source_mtl = folder / 'carroceria.mtl'
if not source_obj.is_file() or not source_mtl.is_file():
    raise SystemExit('Faltan Models/autobus/carroceria.obj o carroceria.mtl.')
obj = source_obj.read_text(encoding='utf-8-sig', errors='replace')
mtl = source_mtl.read_text(encoding='utf-8-sig', errors='replace')
materials = re.findall(r'^usemtl\s+(.*)$', obj, flags=re.M)
if not materials:
    raise SystemExit('El OBJ no declara materiales (usemtl).')
old_logo_material = next((m.strip() for m in materials if 'logo' in m.lower() or 'cruz' in m.lower()), materials[-1].strip())
blocks = re.split(r'(?=^newmtl\s+)', mtl, flags=re.M)
logo_block = next((b for b in blocks if b.startswith('newmtl ') and
    ('logo' in b.splitlines()[0].lower() or 'cruz' in b.lower())), None)
if logo_block is None:
    raise SystemExit('No se encontró el material del logo en carroceria.mtl.')

for team in ('atlante', 'pumas'):
    name = f'logo_{team}'
    new_obj = re.sub(r'^mtllib\s+.*$', f'mtllib carroceria_{team}.mtl', obj, count=1, flags=re.M)
    new_obj = new_obj.replace('usemtl ' + old_logo_material, 'usemtl ' + name)
    new_blocks = []
    for block in blocks:
        if block != logo_block:
            new_blocks.append(block)
            continue
        lines = block.splitlines()
        lines[0] = f'newmtl {name}'
        lines = [line for line in lines if not line.startswith(('map_Kd ', 'map_d '))]
        lines += [f'map_Kd textures/{team}_logo.png', f'map_d textures/{team}_logo.png']
        new_blocks.append('\n'.join(lines) + '\n')
    (folder / f'carroceria_{team}.obj').write_text(new_obj, encoding='utf-8')
    (folder / f'carroceria_{team}.mtl').write_text(''.join(new_blocks), encoding='utf-8')
    print(f'Generado: Models/autobus/carroceria_{team}.obj y .mtl')

for name in ('atlante_logo.png', 'pumas_logo.png', 'autobus.png'):
    if not (folder / 'textures' / name).is_file():
        print('Falta textura:', folder / 'textures' / name)
