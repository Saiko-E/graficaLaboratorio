"""Rival players and crowd show angry, sad, and surprised facial expressions when conceding a goal."""

from pathlib import Path

root = Path(__file__).resolve().parents[1]
textures = root / 'Models/Textures/player_head_textures/atlas_emociones'

for emo in ('atlas_alegria.png', 'atlas_enojo.png', 'atlas_tristeza.png', 'atlas_sorpresa.png'):
    assert (textures / emo).is_file(), f'Missing emotion atlas: {emo}'

main_src = (root / 'main.cpp').read_text()

# Verify emotion textures are loaded
assert 'LoadDiffuseTexture("Models/Textures/player_head_textures/atlas_emociones/atlas_alegria.png")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/player_head_textures/atlas_emociones/atlas_enojo.png")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/player_head_textures/atlas_emociones/atlas_tristeza.png")' in main_src
assert 'LoadDiffuseTexture("Models/Textures/player_head_textures/atlas_emociones/atlas_sorpresa.png")' in main_src

# Verify match goal logic assigns joy to scoring team and angry/sad/surprise to rival team
assert 'matchHead = texHeadJoy;' in main_src
assert 'matchHead = (player == 0) ? texHeadSurprise :' in main_src
assert 'texHeadAngry : texHeadSad' in main_src

# Verify penalty kick expressions
assert 'kickerHead = (celebratingSide != 0) ? texHeadJoy :' in main_src
assert 'keeperHead = (celebratingSide != 0) ? texHeadSurprise :' in main_src

# Verify crowd rival reactions
assert 'isRivalConceding' in main_src
assert 'drawCrowd(modelLoc, VAO, jerseyVAO, headVAO, modelTemp' in main_src

print('Rival goal emotions (angry, sad, surprise) and joy: OK')
