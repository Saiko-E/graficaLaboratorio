"""Generates a high-contrast expressive bird eye texture with white sclera, golden iris, pupil, and catchlights."""

from pathlib import Path
from PIL import Image, ImageDraw

def generate_bird_eye(output_path: Path, size: int = 512):
    img = Image.new("RGB", (size, size), (250, 250, 250))
    draw = ImageDraw.Draw(img)

    cx, cy = size // 2, size // 2
    r_eye = size // 2 - 12
    r_iris = int(r_eye * 0.82)
    r_pupil = int(r_eye * 0.40)

    # Outer black border ring with white sclera
    draw.ellipse([cx - r_eye, cy - r_eye, cx + r_eye, cy + r_eye], fill=(245, 245, 248), outline=(10, 10, 15), width=18)

    # Golden-amber bright bird iris
    for r in range(r_iris, r_pupil, -1):
        t = (r - r_pupil) / (r_iris - r_pupil)
        red = 255
        green = int(140 + 105 * t)
        blue = int(10 + 30 * t)
        draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=(red, green, blue))

    # Sharp outer iris edge
    draw.ellipse([cx - r_iris, cy - r_iris, cx + r_iris, cy + r_iris], outline=(40, 20, 5), width=5)

    # Jet black pupil
    draw.ellipse([cx - r_pupil, cy - r_pupil, cx + r_pupil, cy + r_pupil], fill=(5, 5, 8))

    # Big bright white catchlight (reflection)
    hl1_x = cx + int(r_pupil * 0.45)
    hl1_y = cy - int(r_pupil * 0.45)
    hl1_r = int(r_pupil * 0.45)
    draw.ellipse([hl1_x - hl1_r, hl1_y - hl1_r, hl1_x + hl1_r, hl1_y + hl1_r], fill=(255, 255, 255))

    # Smaller secondary highlight
    hl2_x = cx - int(r_pupil * 0.42)
    hl2_y = cy + int(r_pupil * 0.42)
    hl2_r = int(r_pupil * 0.22)
    draw.ellipse([hl2_x - hl2_r, hl2_y - hl2_r, hl2_x + hl2_r, hl2_y + hl2_r], fill=(255, 255, 255))

    img.save(output_path, "PNG")
    print(f"Generated {output_path} ({size}x{size})")

if __name__ == "__main__":
    out = Path(__file__).resolve().parent / "bird_eye.png"
    generate_bird_eye(out)
