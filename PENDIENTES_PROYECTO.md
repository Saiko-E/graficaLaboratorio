# Proyecto final: estado y pendientes

**Proyecto evaluado:** `graficaLaboratorio/Configinicial/` (ejecutable `graficaLaboratorio`). Revisión del código y los archivos locales: 8 de noviembre de 2026. Esta lista adapta la consigna resumida en `../proyecto-final/PENDIENTES_PROYECTO.md`; **no** describe ni suma los avances del ejecutable `proyecto-final/soccer`.

## Requisitos de la consigna

- Fachada y espacio de OpenGL, con imágenes de referencia y ambientación.
- Al menos **7 tipos de objetos distintos** con geometría y texturizado adecuados (los repetidos cuentan una vez; puertas, escaleras y detalles de ambientación no cuentan). Cada modelo recreado o descargado debe pesar menos de 100 MB.
- **5 animaciones por código:** 3 sencillas y 2 complejas, donde las complejas impliquen más que transformaciones básicas o una trayectoria lineal.
- Conservar y documentar el uso del **código base del profesor**; confirmar con él que la adaptación lo cumple.
- Manual de usuario, manual técnico (objetivos, flujo, Gantt, alcance, limitaciones, metodología, documentación y conclusiones), video explicativo, exposición de máximo 5 minutos y entrega de la liga de GitHub por Classroom. Confirmar la fecha vigente con el profesor: la fecha **11/05/2026** del PDF ya pasó.

## Avance comprobado en este repositorio

- [x] Estadio pequeño abierto: cancha, gradas laterales, fachada perimetral de cuatro lados y accesos. La escena combina objetos hechos por código y modelos importados para la cámara (y autobús condicionado a sus recursos).
- [x] Público en ambas gradas: 120 aficionados por lado, Pumas en z negativo y Atlante en z positivo; usan las playeras existentes y cuatro atlas de rostro distintos generados por código. El pasillo central queda libre. La grada del equipo que anota o gana salta, mueve los brazos y muestra piernas solo durante el festejo.
- [x] Árbitro en saque, goles, penales y final: modelo por código con shorts negros y playera amarilla texturizada, generada como PNG desde `generate_referee_jersey.py`.
- [x] Pista cerrada junto a los cuatro lados del campo, con margen de 0.25 m y esquinas redondeadas que siguen los carriles (`Running Track`); gradas de `Gravel Concrete 03` y butacas de madera `Old Wooden Floor 02` (mapas Diffuse 1K de Poly Haven, CC0). Las cuatro torres iluminan la cancha y responden juntas a las flechas arriba/abajo, sin cambiar los faros de los autobuses.
- [x] Mapas de color diferenciados: césped `Grass005`, asfalto `clean_asphalt`, fachada y paredes laterales `concrete`, escalones `gravel_concrete_03`, pista `running_track`, butacas `old_wooden_floor_02`, troncos `pine_bark` y hojas `LeafSet024`. Los pasillos conservan colores planos. La geometría por código recibe coordenadas procedurales desde `Shader/lighting.vs` y UV propios para la pista.
- [x] Logo de Liga MX en la cámara: `Models/Camaras/cameraHead.obj` usa `textures/liga_mx_logo.png`; se corrigieron sus UV para evitar la repetición. `tests/check_camera_logo.py` verifica la ruta y el rango de UV. La cámara se comprobó visualmente de cerca.
- [x] El tirador usa una playera azul marino/dorada de diseño propio: el torso tiene un VAO con UV para frente y espalda, y el PNG se puede regenerar con `Models/Textures/generate_pumas_jersey.py`. El uniforme rojo del portero no cambió.
- [x] Tirador y portero comparten la cabeza pixel art del atlas neutral generado por `Models/Textures/generate_player_head.py`. Un VAO independiente asigna UV a las seis caras; el uniforme del portero conserva el rojo.
- [x] `Configinicial/build.sh` compila localmente `graficaLaboratorio` sin depender de `Debug/`. El proyecto está en GitHub.

## Pendientes antes de entregar

### Escena y objetos

- [ ] Guardar **imágenes de referencia de la fachada y del espacio** y relacionarlas con el estadio entregado (**falta:** incluir referencias de la escena y sus créditos; las fotos usadas como texturas no sustituyen referencias de fachada/espacio).
- [ ] Acreditar **7 o más tipos de objetos distintos con texturizado adecuado** (**falta:** inventario evaluable y acabar el texturizado de candidatos todavía planos: porterías, uniforme del portero, balón, ave, drones y trofeo usan colores de `CreateSolidColorTexture()`; las cabezas de ambos jugadores, torres de iluminación, playera del tirador, cancha, árboles, gradas/fachada y cámaras sí usan imágenes. No contar objetos repetidos ni detalles como accesos; confirmar con el profesor cuáles admite).
- [ ] Hacer visible y animable el autobús en Linux (**falta:** corregir `Models/Autobus/` en `main.cpp:291–300`, pues los OBJ y `ruedas.txt` están en `Models/autobus/`; `busAssetsReady` queda desactivado y los dibujos/tecla `B` se omiten).
- [ ] Validar recursos y límites de entrega (**falta:** comprobar el peso por objeto, las rutas de texturas de los OBJ/MTL y ejecutar desde una copia limpia en el equipo final; aquí solo se verificó el código y la existencia local de archivos).
- [ ] Explicar el uso del **código base del profesor** (**falta:** identificar en el manual la procedencia de `Camera.h`, `Shader.h`, shaders, iluminación y máquina de estados, y solicitar confirmación de que la adaptación cumple la consigna).

### Animaciones

- [x] Hay **tres candidatas sencillas programadas**: vuelo/aleteo del ave (`drawBird`), giro de rotores de drones (`drawDrone`) y rotación del trofeo (`drawTrophy`). También se orienta el cabezal de las cámaras al balón.
- [x] Hay **dos candidatas complejas programadas**: penal (`Animation`, estados, trayectoria de balón y reacción del portero) y fuegos artificiales (`drawFireworks`, tres explosiones y partículas calculadas por código).
- [ ] Acreditar las **3 sencillas + 2 complejas** ante la rúbrica (**falta:** mostrar cada candidata en pantalla y documentar por qué son cinco distintas; confirmar con el profesor que los fuegos y el penal califican como complejas. El autobús aún no es una alternativa ejecutable en Linux).
- [ ] Ensayar las cinco durante la exposición de máximo 5 minutos (**falta:** guion y demostración grabada; el código no demuestra por sí solo la presentación).

### Documentación y entrega

- [ ] Terminar el manual de usuario (**falta:** completar y validar el boceto `Manual_graficaLaboratorio.tex` con capturas y pruebas en el equipo de entrega; el archivo ya documenta instalación y controles del código actual).
- [ ] Terminar el manual técnico (**falta:** completar el diagrama de flujo, Gantt con fechas y responsables reales, procedencia del código base, evidencias y conclusiones de cada integrante; solo hay un boceto en `Manual_graficaLaboratorio.tex`).
- [ ] Entregar video y ejecutable reproducible (**falta:** video de escena y cinco animaciones, prueba desde copia limpia con recursos correctos y comprobar que la entrega no incluya archivos comprimidos; hay un `.zip` en `Configinicial/Models/`).
- [ ] Confirmar plazo y entregar enlace en Classroom (**falta:** fecha vigente confirmada por el profesor y constancia de entrega; no es verificable desde el código).

**Criterio de esta lista:** `[x]` indica código presente o una comprobación local concreta; **no** significa que el profesor haya aceptado la clasificación ni que la entrega esté terminada. Revisión estática del `main.cpp` y recursos locales, sin volver a ejecutar las cinco animaciones en esta revisión.
