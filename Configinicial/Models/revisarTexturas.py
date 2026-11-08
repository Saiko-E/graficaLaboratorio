from pathlib import Path
import argparse
import shutil


def main():
    parser = argparse.ArgumentParser(description='Verifica y, opcionalmente, repara rutas MTL relativas de modelos OBJ.')
    parser.add_argument('--root', default='.', help='Directorio del proyecto que contiene Models y Shader.')
    parser.add_argument('--fix', action='store_true', help='Copia texturas existentes y modifica los MTL; crea respaldo .bak.')
    args = parser.parse_args()
    root = Path(args.root).resolve()
    models = root / 'Models'
    if not models.is_dir():
        raise SystemExit(f'No existe: {models}. Ejecuta desde la carpeta del proyecto o usa --root.')
    sources = {}
    for folder in [models, root / 'images']:
        if folder.is_dir():
            for file in folder.rglob('*'):
                if file.is_file() and file.suffix.casefold() in ('.png', '.jpg', '.jpeg', '.tga', '.bmp', '.dds'):
                    sources.setdefault(file.name.casefold(), []).append(file)
    total = 0
    missing = 0
    resolved = 0
    for obj in models.rglob('*.obj'):
        content = obj.read_text(encoding='utf-8', errors='replace')
        if not any(line.startswith('vt ') for line in content.splitlines()):
            print(f'AVISO sin coordenadas UV: {obj.relative_to(root)}')
        for line in content.splitlines():
            if not line.startswith('mtllib '):
                continue
            mtl_rel = line[7:].strip().replace('\\', '/')
            mtl = obj.parent / mtl_rel
            if not mtl.is_file():
                print(f'FALTA MTL para {obj.relative_to(root)}: {mtl_rel}')
                continue
            lines = mtl.read_text(encoding='utf-8', errors='replace').splitlines(keepends=True)
            changed = False
            for idx, mline in enumerate(lines):
                if not mline.lstrip().startswith('map_Kd '):
                    continue
                total += 1
                texture = mline.strip().split(None, 1)[1].strip().strip('"').replace('\\', '/')
                p = mtl.parent / texture
                if p.is_file():
                    continue
                missing += 1
                candidates = [item for item in sources.get(Path(texture).name.casefold(), []) if item.exists()]
                if not candidates:
                    print(f'FALTA IMAGEN: {mtl.relative_to(root)} -> {texture}')
                    continue
                # Si hay duplicados, evitar sustituir por una textura equivocada.
                unique = list(dict.fromkeys(str(candidate.resolve()) for candidate in candidates))
                if len(unique) > 1:
                    print(f'AMBIGUA: {mtl.relative_to(root)} -> {texture} ({len(unique)} archivos con ese nombre)')
                    continue
                source = Path(unique[0])
                print(f'ENCONTRADA: {mtl.relative_to(root)} -> {source.relative_to(root)}')
                if args.fix:
                    target = mtl.parent / 'textures' / source.name
                    target.parent.mkdir(parents=True, exist_ok=True)
                    if source.resolve() != target.resolve():
                        shutil.copy2(source, target)
                    ending = '\r\n' if mline.endswith('\r\n') else '\n'
                    lines[idx] = 'map_Kd textures/' + source.name + ending
                    resolved += 1
                    changed = True
            if changed:
                backup = mtl.with_name(mtl.name + '.bak')
                if not backup.exists():
                    shutil.copy2(mtl, backup)
                mtl.write_text(''.join(lines), encoding='utf-8')
    print(f'Comprobadas {total} referencias map_Kd; faltaban {missing}.')
    if args.fix:
        print(f'Reparadas {resolved} rutas. Los MTL originales tienen copia .bak.')
    else:
        print('Solo verificacion. Para copiar imagenes encontradas y reparar las rutas, ejecuta con --fix.')


if __name__ == '__main__':
    main()
