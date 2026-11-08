"""Keep the requested camera order and preserve manual selection between scenes."""

import re
from pathlib import Path

source = (Path(__file__).resolve().parents[1] / "main.cpp").read_text()
layout = source.split("glm::vec3 broadcastCameraPositions[8] = {", 1)[1].split("};", 1)[0]
positions = [tuple(map(float, point)) for point in re.findall(r"glm::vec3\(([-\d.]+)f, ([-\d.]+)f, ([-\d.]+)f\)", layout)]
assert len(positions) == 8
assert positions[0][0] < -52.5 and positions[1][0] > 52.5  # Pumas, Atlante goals
assert positions[2][0] < 0 and positions[3][0] > 0  # Pumas, Atlante halves
assert positions[4][2] < -52 and positions[5][2] > 52  # Left, right stands
assert positions[6][1] > 15 and positions[7][1] > 15  # Two drones

for signature in ("startArrival()", "resetMatch()", "beginMatchScene(int next, bool automatic)"):
    body = source.split("void " + signature + "\n{", 1)[1].split("\n}\n", 1)[0]
    assert "activeCamera =" not in body, signature + " changes the selected camera"
assert len(re.findall(r"\bactiveCamera\s*=\s*(?!=)", source)) == 3  # Initial value and the 0/1–9 keys.

assert "if (activeCamera == 7 || activeCamera == 8)" in source
assert "if (activeCamera == i + 7) continue;" in source
assert "broadcastCameraPositions[i + 6]" in source
assert "activeCamera >= 7 && activeCamera <= 8" in source.split("glm::vec3 getBroadcastPosition()\n{", 1)[1]
assert "activeCamera >= 1 && activeCamera <= 6" in source.split("glm::vec3 getBroadcastPosition()\n{", 1)[1]
assert "for (int i = 0; i < 6; ++i)" in source.split("void drawBroadcastCameraModels(", 2)[-1]
assert "glm::vec3(0.0f, 0.0f, -1.0f)" in source.split("glm::mat4 getBroadcastView()\n{", 1)[1]
print("Camera layout and selection: OK")
