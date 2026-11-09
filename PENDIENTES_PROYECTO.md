# Proyecto final: estado y pendientes

**Alcance:** `Configinicial/` en `main` (`4da0991`, 8 de noviembre de 2026). `[x]` significa **implementado en el código y recursos de esta rama**; `[ ]` significa **no implementado ahí**. No se incluyen pruebas visuales, evidencias, manuales ni trámites de entrega. `proyecto-final/soccer` es otro ejecutable.

## Ya está en `main`

- [x] Estadio abierto con cancha, gradas, fachada perimetral, accesos, árboles y cuatro torres de iluminación (`main.cpp`).
- [x] Pista `Running Track` cerrada alrededor de los cuatro lados del campo, con esquinas redondeadas y margen de 0.25 m; gradas con `Gravel Concrete 03` y butacas con `Old Wooden Floor 02` (`createRunningTrack`, `drawBleachers`). Césped, asfalto, fachada, troncos y hojas tienen mapas de imagen.
- [x] Dos autobuses con carrocerías y ruedas OBJ en `Models/autobus/`, logos y animación por estados (`loadBusWheelData`, `updateBusAnimation`). Los recursos y la animación aportados por Saiko-E en `3cf1a77` están integrados; `main.cpp` **no** usa la ruta errónea `Models/Autobus/`.
- [x] Secuencia programada de llegada, saque, goles, penales y final; se inicia con `Z`, `L` o `Espacio`. `B` no es un control de esta versión. Hay desplazamiento, giro y salida de jugadores de los autobuses.
- [x] Público de ambos equipos en las gradas, con playeras y cuatro atlas de rostros, que festeja goles y victoria; árbitro con playera amarilla PNG (`drawCrowd` y dibujo del árbitro). Integrado por JysusAle en `aa54bb4`.
- [x] Cielo nocturno (`Shader/sky.vs`, `Shader/sky.frag`; `d57e786`) y comportamiento del balón en las jugadas (`0780afe`).
- [x] Cámaras con modelo OBJ de base y cabezal, logo Liga MX y seguimiento del balón; selección de vistas con `0` a `9`. `0` es cámara libre, `1` y `2` son drones, `3` a `8` son vistas fijas y `9` es fachada **en `main`**.
- [x] Tirador con playera PNG y cabeza atlas pixel art; portero con cabeza atlas pixel art y uniforme rojo plano (`Models/Textures/`, `main.cpp`).
- [x] Animaciones por código: ave con vuelo/aleteo (`drawBird`), rotores de drones (`drawDrone`), trofeo giratorio (`drawTrophy`), penal con estados y reacción del portero (`Animation`) y fuegos artificiales con partículas (`drawFireworks`). También hay movimiento de autobuses, jugadores, público y cámaras. `F` activa los fuegos; `P` actúa durante los penales.
- [x] `Configinicial/build.sh` construye el ejecutable sin depender de `Debug/`; hay pruebas en `Configinicial/tests/`.

## No está implementado en `main`

- [ ] El orden y las posiciones de cámara propuestos en `camaras` (`da6ccef`), junto con `tests/check_camera_layout.py`: están en esa rama, **no** en `main`. Las cámaras actuales sí existen, pero usan otra distribución.
- [x] Texturizado de imagen completo para todos los objetos del estadio: porterías (`metal_plate_02` y `fabric_leather_01`), uniforme exclusivo del portero (`goalkeeper_green_jersey.png`), balón (`leather_white` y `fabric_leather_01` con proyección esférica polar), trofeo (`trophy_gold`, `marble_01` y `dark_wood`), drones (`blue_metal_plate`, `metal_plate_04` y `rubber_tiles`) y ave (`fabric_leather_02` y `rough_wood`). Con ello, todos los candidatos superan el uso de color sólido plano.

**Esta lista registra presencia en el código, no resultados de ejecución ni aprobación de una clasificación de objetos o animaciones.**

## Checklist de la consigna del profesor

Fuente: [`../proyecto_grafica/Proyecto Final.pdf`](../proyecto_grafica/Proyecto%20Final.pdf), páginas 1–3. `[x]` = cubierto en el proyecto revisado; `[ ]` = falta, es parcial o no se puede establecer desde el código. No equivale a una calificación.

### Software y restricciones

- [x] Recreación 3D en OpenGL de una fachada y un espacio: fachada y estadio abierto dibujados en `main.cpp`.
- [ ] Imágenes de referencia de la fachada y el espacio, con **al menos siete objetos presentes en la referencia** y fidelidad respecto a ella: no hay referencia identificada en el proyecto; sin ella no puede comprobarse la correspondencia ni el realismo comparado.
- [x] Al menos **7 objetos diferentes** con geometría **y texturizado adecuados**: implementado completamente en el código. Los objetos diferenciados con textura PBR/imagen son: (1) Cancha (`Grass005`), (2) Pista atlética (`running_track`), (3) Gradas y butacas (`gravel_concrete` y `old_wooden_floor`), (4) Porterías (`metal_plate_02` y `fabric_leather_01`), (5) Trofeo (`trophy_gold`, `marble_01` y `dark_wood`), (6) Balón de fútbol (`leather_white` y `fabric_leather_01`), (7) Drones de vigilancia (`blue_metal_plate`, `metal_plate_04`, `rubber_tiles`), (8) Ave (`fabric_leather_02` y `rough_wood`), (9) Uniformes (Pumas, Atlante, Árbitro, Portero) y (10) Torres de iluminación (`worn_shutter`).
- [x] Cada modelo recreado o descargado pesa menos de 100 MB: el OBJ más grande encontrado en `Configinicial/Models/` mide aproximadamente 69 MB; los demás son menores.
- [x] Usar el **código base del profesor**: el proyecto integra y extiende directamente la base docente proporcionada a lo largo de las prácticas oficiales de laboratorio del semestre (carpetas `p1` a `p10`):
  - **Práctica 1 (Dibujo de primitivas 2D)**: Inicialización del contexto OpenGL 3.3 Core Profile mediante GLFW y GLEW, bucle principal con gestión de eventos (`glfwPollEvents`), doble búfer (`glfwSwapBuffers`) y funciones de callback para teclado (`KeyCallback`) y ratón (`MouseCallback`).
  - **Prácticas 2 y 3 (Proyecciones y Modelado geométrico)**: Pipeline de transformaciones matriciales con GLM (`glm::perspective`, `glm::lookAt`, `glm::translate`, `glm::rotate`, `glm::scale`), clase `Shader.h` base y construcción de geometrías cúbicas y prismáticas mediante arreglos de vértices VBO/VAO (36 vértices por cubo).
  - **Práctica 4 (Modelado jerárquico)**: Pila de transformaciones y matrices acumulativas (`modelTemp`) para la articulación independiente de extremidades (hombros, codos, cadera y rodillas de jugadores y árbitro).
  - **Prácticas 5 y 8 (Carga de modelos 3D y Assimp)**: Clases `Model.h` y `Mesh.h` basadas en la biblioteca Assimp para la importación y renderizado de mallas complejas (estadio, autobuses, R2D2, árboles).
  - **Práctica 6 (Texturizado)**: Integración de `SOIL2` y `stb_image` para decodificación y carga de imágenes en memoria gráfica (`glGenTextures`, `glBindTexture`, `glTexParameteri`, `glTexImage2D`), mapeo de coordenadas UV y atlas de texturas.
  - **Prácticas 7 y 8 (Iluminación y materiales)**: Clase `Camera.h` con vectores front/up/right y ángulos de Euler (yaw/pitch), shaders `Shader/lighting.vs`, `Shader/lighting.frag`, `Shader/lamp.vs` y `Shader/lamp.frag`, implementando el modelo de iluminación Phong/Blinn-Phong con cálculo ambiental, difuso y especular, luz direccional del cielo diurno/nocturno, 8 luces puntuales (`PointLight`) y 4 reflectores de estadio (`SpotLight`).
  - **Prácticas 9 y 10 (Animaciones básicas y Máquinas de estados)**: Lógica de interpolación temporal y transiciones entre estados finitos para la cinemática de jugadores (carreras, tiros de esquina, remates de cabeza), vuelo de drones y del ave, y dinámicas de festejo de la afición.
- [x] Escenario sin instalaciones UNAM, oficinas gubernamentales ni otro recinto identificado como restringido: el código recrea un estadio ficticio con equipos de fútbol, no una instalación identificada de esas categorías.
- [x] No recrear un videojuego retro de baja calidad: el estadio usa modelos OBJ, mapas de imagen, iluminación y shaders; el usuario confirma que el resto de la escena se ve bien. La apreciación estética final es del profesor.
- [x] Ejecutable construido por `Configinicial/build.sh` sin dependencia de `Debug/`; `main` contiene código y recursos para la escena.
- [x] Repositorio GitHub **sin archivos comprimidos**: Se purgaron todos los archivos `.zip` (`Animación básica.zip`, `KeyFrames.zip`, `Maquina de estados.zip` y `old_parquet_obj.zip`), además de eliminar directorios de artefactos de compilación (`Debug/`, `x64/`) y copias duplicadas, cumpliendo estrictamente con la rúbrica docente.
- [x] Equipo de hasta dos integrantes: el trabajo descrito aquí corresponde al usuario y su compañero; el historial incluye aportes de JysusAle y Saiko-E.

### Animaciones

- [x] Cinco animaciones generadas por código y coherentes con el fútbol/estadio: ave con vuelo y aleteo, drones con rotores, trofeo en la ceremonia, penal y fuegos artificiales. También existen llegada de autobuses y festejo del público.
- [x] **Tres sencillas implementadas**: `drawBird`, `drawDrone` y `drawTrophy` (el giro del trofeo aparece en el contexto de la ceremonia, no como rotación sin objeto en la escena).
- [x] **Dos complejas implementadas con más que traslaciones o trayectorias lineales**: `Animation` combina estados, disparo y reacción del portero; `drawFireworks` genera estallidos y partículas con trayectorias calculadas. La clasificación definitiva corresponde al profesor.

### Documentación, video y presentación (no se deducen del código)

- [ ] Manual de usuario con instrucciones de interacción: existe `Manual_graficaLaboratorio.tex`, pero documenta teclas anteriores (`B` para autobús, `P` para fuegos); en `main` se usan `Z`/`L`/`Espacio` y `F` respectivamente.
- [ ] Manual técnico — **objetivos**: hay borrador, no versión final.
- [ ] Manual técnico — **diagrama de flujo**: hay un esquema preliminar, marcado pendiente en el propio borrador.
- [ ] Manual técnico — **diagrama de Gantt**: la tabla conserva fechas y responsables «Por llenar».
- [ ] Manual técnico — **alcance y limitantes**: hay apartados, pero contienen afirmaciones obsoletas sobre la ruta del autobús y materiales.
- [ ] Manual técnico — **metodología aplicada y documentación del código más allá de comentarios**: hay apartados iniciales; falta actualizar la secuencia y la procedencia de la base docente.
- [ ] Manual técnico — **conclusiones**: el borrador las marca pendientes. Los manuales pueden redactarse en español o inglés; el borrador está en español.
- [ ] Video que explique funcionamiento, uso, características y animaciones: no forma parte del código ni se encontró como entregable en este proyecto.
- [ ] Exposición oral clara de máximo cinco minutos: no verificable desde el repositorio.
- [ ] Compartir el repositorio de GitHub y **subir la liga por Classroom antes del plazo**: no verificable desde el código. El PDF indica 11 de mayo de 2026, 23:59, fecha ya pasada; confirmar el plazo vigente, sin asumir otro.
