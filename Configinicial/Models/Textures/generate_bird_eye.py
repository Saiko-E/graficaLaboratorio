"""Generates a detailed bird eye texture with amber/golden iris, pupil, and specular catchlights."""

from pathlib import Path
from PIL import Image, ImageDraw
import math
import random

def generate_bird_eye(output_path: Path, size: int = 512):
    img = Image.new("RGBA", (size, size), (15, 12, 10, 255))
    draw = ImageDraw.Draw(img)

    cx, cy = size // 2, size // 2
    r_outer = size // 2 - 8
    r_iris = int(r_outer * 0.94)
    r_pupil = int(r_outer * 0.45)

    # Dark limbal ring / border
    draw.ellipse([cx - r_outer, cy - r_outer, cx + r_outer, cy + r_outer], fill=(22, 16, 12, 255))

    # Amber / golden iris radial gradient
    for r in range(r_iris, r_pupil, -1):
        t = (r - r_pupil) / (r_iris - r_pupil)
        # Deep amber at outer edge, vivid gold/yellow closer to pupil
        red = int(255 * (0.88 + 0.12 * (1.0 - t)))
        green = int(195 * t + 85 * (1.0 - t))
        blue = int(18 * t)
        draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=(red, green, blue, 255))

    # Iris radial detail fibers
    rng = random.Random(1337)
    for i in range(480):
        angle = math.radians(i * (360.0 / 480.0))
        length = rng.uniform(0.55, 1.0)
        r1 = r_pupil + int((r_iris - r_pupil) * 0.08)
        r2 = r_pupil + int((r_iris - r_pupil) * length)
        x1 = cx + int(r1 * math.cos(angle))
        y1 = cy + int(r1 * math.sin(angle))
        x2 = cx + int(r2 * math.cos(angle))
        y2 = cy + int(r2 * math.sin(angle))
        alpha = rng.randint(45, 130)
        color = (255, 235, 110, alpha) if i % 2 == 0 else (190, 80, 5, alpha)
        draw.line([(x1, y1), (x2, y2)], fill=color, width=2)

    # Thin dark ring around pupil
    draw.ellipse([cx - r_pupil - 5, cy - r_pupil - 5, cx + r_pupil + 5, cy + r_pupil + 5],
                 outline=(55, 25, 8, 200), width=3)

    # Deep black pupil
    draw.ellipse([cx - r_pupil, cy - r_pupil, cx + r_pupil, cy + r_pupil], fill=(8, 8, 12, 255))

    # Primary catchlight (bright white highlight)
    hl_x = cx + int(r_pupil * 0.42)
    hl_y = cy - int(r_pupil * 0.44)
    hl_r = int(r_pupil * 0.30)
    draw.ellipse([hl_x - hl_r, hl_y - hl_r, hl_x + hl_r, hl_y + hl_r], fill=(255, 255, 255, 245))

    # Secondary softer catchlight
    hl2_x = cx - int(r_pupil * 0.38)
    hl2_y = cy + int(r_pupil * 0.38)
    hl2_r = int(r_pupil * 0.15)
    draw.ellipse([hl2_x - hl2_r, hl2_y - hl2_r, hl2_x + hl2_r, hl2_y + hl2_r], fill=(255, 255, 255, 185))

    img.save(output_path, "PNG")
    print(f"Generated {output_path} ({size}x{size})")

if __name__ == "__main__":
    out = Path(__file__).resolve().parent / "bird_eye.png"
    generate_bird_eye(out)
