"""The night background must follow the view without covering the stadium."""

from pathlib import Path

root = Path(__file__).resolve().parents[1]
scene = (root / "main.cpp").read_text()
sky = (root / "Shader/sky.frag").read_text()

assert "Shader skyShader(\"Shader/sky.vs\", \"Shader/sky.frag\")" in scene
assert scene.index("skyShader.Use();") < scene.index("drawPerimeter(modelLoc")
assert scene.index("glDepthMask(GL_TRUE);", scene.index("skyShader.Use();")) < scene.index("drawPerimeter(modelLoc")
assert "worldDirection" in sky and "moonDirection" in sky and "stars" in sky
print("Night sky: OK")
