"""Only the scoring or winning team's stand celebrates."""

from pathlib import Path

source = (Path(__file__).resolve().parents[1] / "main.cpp").read_text()
rule = source.split("int crowdCelebratingSide()\n{", 1)[1].split("\n}\n", 1)[0]
assert "SCENE_PUMAS_GOAL && sceneTime >= 5.2f" in rule
assert "SCENE_ATLANTE_GOAL && sceneTime >= 4.1f" in rule
assert 'penaltyResult == "GOAL"' in rule
assert "penaltyKickerTeam == 0 ? -1 : 1" in rule
assert "penaltyWinner == 0 ? -1 : 1" in rule
assert "pumasScore > atlanteScore" in rule and "atlanteScore > pumasScore" in rule
assert "return 0;" in rule  # No celebration before a goal or on an unresolved draw.

render = source.split("void drawCrowd(GLint modelLoc", 2)[-1].split("void drawDrone(", 1)[0]
assert "side == crowdCelebratingSide()" in render
assert "if (celebrating)" in render
assert "float jump" in render and "cheerTime" in render
assert "for (int leg = -1; leg <= 1; leg += 2)" in render
assert "if (celebrating)" in render.split("for (int arm = -1; arm <= 1; arm += 2)", 1)[-1]
assert "int winningSide = crowdCelebratingSide();" in source
assert "int champion = winningSide == 0 ? -1 : (winningSide < 0 ? 1 : 0);" in source
assert "pumasScore = std::max(1, pumasScore)" not in source
print("Crowd celebrations and winner mapping: OK")
