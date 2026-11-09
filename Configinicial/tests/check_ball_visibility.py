"""The ball appears in play, not during arrival or celebrations."""

from pathlib import Path

source = (Path(__file__).resolve().parents[1] / "main.cpp").read_text()
loop = source.split("// Game loop", 1)[1].split("//DELETE BLOCK", 1)[0]
rule = "const bool ballVisible = matchScene == SCENE_KICKOFF || matchScene == SCENE_PUMAS_GOAL ||\n            matchScene == SCENE_ATLANTE_GOAL || matchScene == SCENE_PENALTIES;"
assert rule in loop
assert "if (ballVisible)\n            drawBall(" in loop
assert "ballVisible ? ballPosition : glm::vec3(0.0f, 0.11f, 0.0f)" in loop
assert "glm::vec3 trackedBallPosition(0.0f, 0.11f, 0.0f)" in source
print("Ball visibility and camera tracking: OK")
