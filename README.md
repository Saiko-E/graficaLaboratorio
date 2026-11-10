# graficaLaboratorio

Proyecto final de Computación Gráfica: estadio de fútbol en OpenGL.

## Cómo ejecutar en Windows

### Requisitos

- Windows de 64 bits.
- Tarjeta gráfica compatible con **OpenGL 3.3** o superior y sus controladores instalados y actualizados.
- La carpeta completa del programa: el ejecutable, las DLL y los recursos incluidos en `dist\graficaLaboratorio`.

Para usar el ejecutable incluido no necesitas instalar Visual Studio, MSYS2 ni un compilador.

### Opción 1: desde el Explorador de archivos

1. Descarga el repositorio con **Code → Download ZIP** en GitHub, o clónalo con Git.
2. Si descargaste el ZIP, haz clic derecho sobre él y selecciona **Extraer todo**.
3. Abre la carpeta del proyecto y entra en `dist\graficaLaboratorio`.
4. Haz doble clic en **`graficaLaboratorio.exe`** para iniciar el estadio.

### Opción 2: desde PowerShell

Abre PowerShell en la carpeta raíz del proyecto (la que contiene este `README.md`) y ejecuta:

```powershell
.\dist\graficaLaboratorio\graficaLaboratorio.exe
```

### Controles básicos

| Tecla | Acción |
| --- | --- |
| `1` a `8` | Cambiar entre las cámaras del estadio. |
| `9` | Ver el estadio desde el exterior. |
| `0` | Activar la cámara libre. |
| `W`, `A`, `S`, `D` y ratón | Mover y orientar la cámara libre. |
| `Espacio` | Iniciar o pausar la secuencia. |
| `L` | Reiniciar la secuencia automática. |
| `N` | Ir a la tanda de penales. |
| `P` | Tirar un penal. |
| `R` | Reiniciar el partido. |
| `Escape` | Cerrar el programa. |

### Compartir el programa

Copia o comprime **toda la carpeta `dist\graficaLaboratorio`** y extráela en el otro equipo antes de ejecutarlo. Mantén las DLL y las carpetas `Models` y `Shader` junto al `.exe`: son necesarias para cargar las bibliotecas, los modelos, las texturas y los shaders.

### Solución de problemas

- **Falta una DLL:** vuelve a extraer o copiar la carpeta completa del programa; comprueba que las DLL estén junto al ejecutable.
- **No aparecen modelos o texturas, o hay errores al cargar shaders:** comprueba que las carpetas `Models` y `Shader` conserven su contenido y estén junto al `.exe`.
- **No se abre la ventana o aparece un error de OpenGL:** actualiza los controladores de la tarjeta gráfica y verifica que el equipo soporte OpenGL 3.3.
- **La cámara no se mueve:** pulsa `0` para activar la cámara libre antes de usar `W/A/S/D` y el ratón.

## Código fuente

El código fuente está en `Configinicial/`. Consulta [requisitos y pendientes](PENDIENTES_PROYECTO.md) para el estado de la entrega.

**Nota sobre la compilación:** el script `build-windows.ps1` mencionado en [EJECUTABLE.md](EJECUTABLE.md) no está incluido en este repositorio. Las instrucciones anteriores utilizan el ejecutable ya generado en `dist\graficaLaboratorio`.
