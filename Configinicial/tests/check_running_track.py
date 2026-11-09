"""The lane-aligned track hugs all four sides without covering the pitch."""

import math
import re
from pathlib import Path

from PIL import Image

root = Path(__file__).resolve().parents[1]
image = root / 'Models/Textures/running_track_diff_1k.jpg'
with Image.open(image) as texture:
    assert texture.format == 'JPEG' and texture.size == (1024, 1024)
    assert texture.getpixel((150, 510)) != texture.getpixel((150, 700))

source = (root / 'main.cpp').read_text()
assert 'LoadDiffuseTexture("Models/Textures/running_track_diff_1k.jpg")' in source
assert 'drawRunningTrack(modelLoc, modelTemp, runningTrackMesh, texRunningTrack);' in source
assert 'deleteBallMesh(runningTrackMesh);' in source
assert 'texRunningTrack' in source.split('GLuint textures[] =', 1)[1]
geometry = source.split('void createRunningTrack(BallMesh& mesh)\n{', 1)[1].split('\n}\n', 1)[0]
length, half_width = map(float, re.search(
    r'halfLength = ([\d.]+)f, halfWidth = ([\d.]+)f', geometry
).groups())
inner, width = map(float, re.search(
    r'innerRadius = ([\d.]+)f, trackWidth = ([\d.]+)f', geometry
).groups())
assert (length, half_width) == (52.5, 34.0)
assert 0.12 < inner <= 0.35  # Almost flush with the pitch; outside its white markings.
assert half_width + inner + width < 49.45  # Clear the stand railing.
assert length + inner + width < 103  # Inside the end walls.
assert 'for (int corner = 0; corner < 4; ++corner)' in geometry
assert 'for (int step = 1; step <= curveSteps; ++step)' in geometry
assert 'addSample(glm::vec2(halfLength + innerRadius, halfWidth)' in geometry
assert 'addSample(glm::vec2(-halfLength, halfWidth + innerRadius)' in geometry
assert 'addSample(glm::vec2(-halfLength - innerRadius, -halfWidth)' in geometry
assert 'vertex(a.inner, a.distance, 0.0f);' in geometry
assert 'vertex(a.inner + trackWidth * a.outward, a.distance, trackWidth);' in geometry
assert 'distance / 4.0f, across / 4.0f' in geometry  # Lane lines follow the path.
steps = int(re.search(r'curveSteps = (\d+)', geometry).group(1))
for corner in range(4):
    cx = length if corner < 2 else -length
    cz = -half_width if corner in (0, 3) else half_width
    for step in range(steps + 1):
        angle = -math.pi / 2 + (corner + step / steps) * math.pi / 2
        for radius in (inner, inner + width):
            x, z = cx + radius * math.cos(angle), cz + radius * math.sin(angle)
            assert abs(x) < 103 and abs(z) < 49.45
            assert abs(x) >= length - 1e-5 or abs(z) >= half_width - 1e-5
assert math.isclose(length + inner, 52.75)
bleachers = source.split('void drawBleachers(GLint modelLoc', 2)[-1].split('void drawCrowd(', 1)[0]
assert 'glm::vec3(106.0f, 0.045f, 15.5f)' not in bleachers
print('Running track hugs field and clears stands: OK')
