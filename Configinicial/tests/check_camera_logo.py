from pathlib import Path

camera = Path(__file__).resolve().parents[1] / "Models/Camaras"
material = ""
coords = []
logo_indices = set()
for line in (camera / "cameraHead.obj").read_text().splitlines():
    parts = line.split()
    if not parts:
        continue
    if parts[0] == "vt":
        coords.append(tuple(map(float, parts[1:3])))
    elif parts[0] == "usemtl":
        material = parts[1]
    elif parts[0] == "f" and material == "Material.002":
        logo_indices.update(int(vertex.split("/")[1]) for vertex in parts[1:])

assert logo_indices, "The camera has no Liga MX logo faces"
assert all(0 <= value <= 1 for index in logo_indices for value in coords[index - 1]), "Logo UVs repeat the image"
assert "map_Kd textures/liga_mx_logo.png" in (camera / "cameraHead.mtl").read_text()
assert (camera / "textures/liga_mx_logo.png").is_file()
print("Camera logo UVs and texture: OK")
