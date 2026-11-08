"""Corner, header and shot must meet the ball in the C sequence."""

import math
import re
from pathlib import Path

source = (Path(__file__).resolve().parents[1] / "main.cpp").read_text()
render = source.split("else if (matchScene == SCENE_PUMAS_GOAL) {", 1)[1].split("else if (matchScene == SCENE_ATLANTE_GOAL) {", 1)[0]
flight = source.split("void updateMatch(float step)\n{", 1)[1].split("if (matchScene == SCENE_PUMAS_GOAL) {", 1)[1].split("if (matchScene == SCENE_ATLANTE_GOAL) {", 1)[0]

assert "pumasCornerKickTime" in flight and "pumasShotTime" in flight
assert "pumasCornerKickTime" in render and "pumasShotTime" in render
assert "(t - pumasCornerKickTime) / (2.5f - pumasCornerKickTime)" in flight
assert "(t - pumasShotTime) / (5.2f - pumasShotTime)" in flight
assert "sceneTime / 2.5f" in render  # Header arrives at the same time as the ball.

def point(name):
    text = re.search(r"glm::vec3 " + name + r"\(([^)]+)\)", flight).group(1)
    return tuple(float(v.strip().removesuffix("f")) for v in text.split(","))

start, header, scorer = point("start"), point("header"), point("scorer")
assert math.dist(start, (50.8, 0.11, -33.0)) < 0.01
assert math.dist(header, (43.0, 2.0, -5.0)) < 0.01
assert math.dist(scorer, (46.5, 0.35, 0.0)) < 0.01

# Foot centers from drawPlayer's right hip/knee/boot transforms, at maximum swing.
def boot(position, yaw, angle):
    yaw, angle = math.radians(yaw), math.radians(angle)
    z = 0.80 * math.sin(angle) + 0.09 * math.cos(angle)
    return (position[0] + 0.13 * math.cos(yaw) + z * math.sin(yaw),
            0.90 - 0.80 * math.cos(angle) + 0.09 * math.sin(angle),
            position[2] - 0.13 * math.sin(yaw) + z * math.cos(yaw))

assert "glm::vec3(50.9f, 0.0f, -33.5f)" in render and "yaw = -16.0f" in render
assert math.dist(boot((50.9, 0, -33.5), -16, 30), start) < 0.25
assert math.dist(header, (43.0, 1.77, -5.0)) < 0.26  # Head plus ball radii.
assert "yaw = 90.0f" in render
assert math.dist(boot((46.0, 0, 0), 90, 36), scorer) < 0.20
print("Pumas corner, header and shot contact: OK")
