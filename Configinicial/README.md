# Integración en el proyecto de fútbol soccer (OpenGL 3.3)

`main.cpp` toma como referencia `main(3).cpp`. Mantiene la sección marcada `//BIEN  HASTA AQUI`, salvo tres correcciones: (1) elimina la segunda declaración de `pedirPatada`, (2) añade la bandera `pedirReinicio`, y (3) comenta las inclusiones de `Model.h` y `stb_image.h` porque ya no se cargan OBJ ni imágenes. El arreglo `vertices[]` NO cambia.

## Archivos que debes mantener sin editar

Deja **intactos** los archivos de tu profesor: `Shader.h`, `Camera.h`, `Shader/lighting.vs`, `Shader/lighting.frag`, `Shader/lamp.vs`, `Shader/lamp.frag`, `Shader/modelLoading.vs` y `Shader/modelLoading.frag`. El main utiliza únicamente lighting y lamp, pero no modifica los demás.

Coloca `main.cpp`, `Campo.h`, `Porteria.h`, `Persona.h`, `Balon.h` junto a `Shader.h` y `Camera.h`, manteniendo los shader originales en una carpeta `Shader/`, tal como están referenciados por las rutas relativas.

## Por qué sigue estando CrearTexturaColor()

`lighting.frag` declara `uniform Material material;` con `sampler2D diffuse` y `sampler2D specular`, y los consulta con `texture(...)`. Si dibujas objetos sin enlazar texturas, **no obtendrás los colores previstos**. Esta función crea texturas de 1 pixel RGB, y cada pieza enlaza una en la unidad 0. El main fija `material.diffuse=0` y `material.specular=1`, y enlaza una textura especular compartida en la unidad 1.

No se añaden UV a los vértices. El shader declara `layout(location=2) in vec2 texCoords;` pero el VAO no habilita ese atributo; OpenGL proporciona el valor constante por defecto `(0,0)`. Esto es suficiente para una textura de 1x1 pixel, pero NO serviría para colocar una imagen con detalles o un patrón preciso.

## Clases y recursos OpenGL

- Campo: usa el VAO/VBO del cubo original del profesor. El objeto se forma por transformaciones, sin malla externa.
- Porteria: utiliza el mismo VAO para las estructuras y una red de barras delgadas.
- Persona: construye un jugador articulado (modelo jerárquico) sobre el mismo cubo.
- Balon: construye sus propias superficies de esfera blanca y negra trigonométricamente; sus dos VAO/VBO siguen el mismo formato de 6 floats por vértice (posición + normal), con `glDrawArrays`. El diseño de parches negros es **aproximado**, no pentágonos exactos.

Los objetos usan `lightingShader`, la lámpara usa `lampShader`, con los dos programas del profesor. `ConfigurarIluminacion` mantiene direccional, puntual y spotlight; solo se corrigen nombres de los sampler uniforms para que coincidan exactamente con `lighting.frag`.

La declaración global `Camera camera(vec3(0,0,3))` se mantiene, pero se reasigna su posición inicial tras iniciar OpenGL para poder visualizar la cancha a escala de metros. Se habilita un contexto OpenGL 3.3 para los shaders `#version 330 core`.

## Controles

En el ejecutable actual, `1`–`8` son vistas fijas de transmisión, `9` muestra
la fachada desde fuera y `0` activa la cámara libre (W/A/S/D y ratón). La vista
inicial es la cámara fija 4: para moverte debes pulsar `0`.

La fachada del estadio pequeño se construye por código con dos gradas laterales
abiertas y un muro perimetral en los cuatro lados. Los dos accesos a nivel de piso
se alinean con los pasillos entre gradas. No hay techo.

Texturas de prueba (mapas de color, licencia CC0): [Grass 005 de ambientCG](https://ambientcg.com/view?id=Grass005)
para la cancha, [Pine Bark de Poly Haven](https://polyhaven.com/a/pine_bark) para los
troncos y [Leaf Set 024 de ambientCG](https://ambientcg.com/view?id=LeafSet024) para
las copas. Los dos mapas de hojas en `Models/Textures/` son una composición densa
del atlas de color y opacidad CC0; el mapa oscuro reduce el brillo al 70 %. Las
esferas usan una proyección plana para esta prueba; el resto de la escena conserva
sus colores anteriores.

- `W/A/S/D` y flechas: mover cámara.
- Ratón: orientar cámara.
- `T/G`, `Y/H`, `U/J`: mover luz puntual en X/Y/Z.
- Espacio: alternar luz puntual animada.
- `N`: iniciar patada (máquina de estados: preparando, golpeando, regresando).
- `R`: reiniciar patada y balón.
- `Escape`: cerrar.

## Limitación / validación

No se han compilado ni ejecutado estos archivos con las bibliotecas reales en esta sesión. Se verificó el contenido del proyecto y las interfaces de los shaders, pero comprueba la compilación y el resultado de renderizado en tu Visual Studio / configuración OpenGL. Si un shader imprime error de compilación, transcribe el mensaje exacto antes de considerar cambios, porque el requisito es mantener los archivos `.vs` y `.frag` intactos.
