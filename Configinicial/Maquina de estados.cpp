#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 1200, HEIGHT = 800;
const float PI = 3.14159265f;

// Para el teclado / camara
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

// Para el modelo
float	giro = 0.0f;     // giro del balon sobre su eje largo (X)
float	cabeceo = 0.0f;  // inclinacion del balon (eje Z)

// Dimensiones del balon (la malla mide 1x1x1 y se escala a esto)
const float LARGO = 2.0f;
const float DIAM = 1.2f;
const float PUNTA = 1.3f;   // 1.0 = elipsoide perfecto, >1 = puntas mas afiladas

// ---------------------------------------------------------------
// Utilidades para crear mallas (solo posiciones, 3 floats por vertice)
// ---------------------------------------------------------------
struct Mesh { GLuint vao, vbo; GLsizei count; };

Mesh crearMalla(const std::vector<float>& v) {
	Mesh m;
	m.count = (GLsizei)(v.size() / 3);
	glGenVertexArrays(1, &m.vao);
	glGenBuffers(1, &m.vbo);

	glBindVertexArray(m.vao);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);

	// Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return m;
}

// Cubo de 1x1x1 centrado en el origen (equivalente a tu arreglo vertices[])
std::vector<float> crearCubo() {
	const float c[8][3] = {
		{-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
		{-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f} };
	const int idx[36] = {
		4,5,6, 6,7,4,   1,0,3, 3,2,1,   5,1,2, 2,6,5,
		0,4,7, 7,3,0,   0,1,5, 5,4,0,   3,7,6, 6,2,3 };
	std::vector<float> v;
	for (int i = 0; i < 36; i++)
		for (int k = 0; k < 3; k++)
			v.push_back(c[idx[i]][k]);
	return v;
}

// Superficie de revolucion alrededor del eje X (eje largo del balon).
//   th va de 0 (punta derecha) a PI (punta izquierda)
//   x      = 0.5 * cos(th)            -> longitud total 1
//   radio  = 0.5 * sin(th)^punta      -> diametro maximo 1
// th0 y th1 permiten generar solo un tramo (las franjas blancas).
std::vector<float> crearBalon(int franjas, int lados, float th0, float th1, float punta) {
	std::vector<float> v;

	auto P = [&](float th, float a) {
		// fmaxf evita sin(PI) = -0.00000008 -> pow(negativo) = NaN
		float r = 0.5f * powf(fmaxf(sinf(th), 0.0f), punta);
		v.push_back(0.5f * cosf(th));
		v.push_back(r * cosf(a));
		v.push_back(r * sinf(a));
		};

	for (int i = 0; i < franjas; i++) {
		float tA = th0 + (th1 - th0) * i / franjas;
		float tB = th0 + (th1 - th0) * (i + 1) / franjas;
		for (int j = 0; j < lados; j++) {
			float a0 = 2.0f * PI * j / lados;
			float a1 = 2.0f * PI * (j + 1) / lados;
			// dos triangulos por cada cuadro de la malla
			P(tA, a0); P(tB, a0); P(tB, a1);
			P(tA, a0); P(tB, a1); P(tA, a1);
		}
	}
	return v;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Balon de futbol americano", nullptr, nullptr);

	// Verificacion de errores de creacion de ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// ---- Mallas ----
	Mesh cubo = crearMalla(crearCubo());
	Mesh balon = crearMalla(crearBalon(24, 32, 0.0f, PI, PUNTA));      // cuerpo completo
	Mesh franja = crearMalla(crearBalon(4, 32, 0.80f, 0.95f, PUNTA));   // tramo para la franja blanca

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	// Colores
	const glm::vec3 marron = glm::vec3(0.45f, 0.22f, 0.08f);
	const glm::vec3 blanco = glm::vec3(1.0f, 1.0f, 1.0f);

	// Altura de la superficie del balon (en la parte de arriba, +Y) para una posicion x
	// Misma formula que usa la malla: radio = (DIAM/2) * sin(th)^PUNTA, con cos(th) = x/(LARGO/2)
	auto alturaSuperficie = [](float x) {
		float t = x / (LARGO / 2.0f);
		return (DIAM / 2.0f) * powf(sqrtf(fmaxf(1.0f - t * t, 0.0f)), PUNTA);
		};

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// Dibuja una malla con su matriz de modelo y su color
		auto dibujar = [&](const Mesh& m, const glm::mat4& mdl, const glm::vec3& c) {
			glUniform3fv(uniformColor, 1, glm::value_ptr(c));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mdl));
			glBindVertexArray(m.vao);
			glDrawArrays(GL_TRIANGLES, 0, m.count);
			};

		// ================= JERARQUIA DEL BALON =================
		// 'base' NO lleva escala: es el "padre" de todas las piezas
		// (igual que modelTemp en el brazo). Si tuviera escala, las agujetas se deformarian.
		glm::mat4 base = glm::mat4(1.0f);
		base = glm::rotate(base, glm::radians(cabeceo), glm::vec3(0.0f, 0.0f, 1.0f)); // inclinacion
		base = glm::rotate(base, glm::radians(giro), glm::vec3(1.0f, 0.0f, 0.0f));    // giro en espiral

		// --- Cuerpo ---
		dibujar(balon, glm::scale(base, glm::vec3(LARGO, DIAM, DIAM)), marron);

		// --- Franjas blancas (la misma malla, una normal y otra espejada con escala X negativa) ---
		glm::vec3 esc = glm::vec3(LARGO, DIAM * 1.01f, DIAM * 1.01f);  // 1% mas grande para evitar z-fighting
		dibujar(franja, glm::scale(base, esc), blanco);
		dibujar(franja, glm::scale(base, glm::vec3(-esc.x, esc.y, esc.z)), blanco);

		// --- Agujetas (cubos pequenos sobre la parte de arriba, +Y) ---
		for (int i = 0; i < 7; i++) {
			float x = -0.45f + 0.15f * i;
			float y = alturaSuperficie(x) + 0.02f;
			// inclinacion segun la pendiente de la superficie en ese punto
			float ang = atan2f(alturaSuperficie(x + 0.01f) - alturaSuperficie(x - 0.01f), 0.02f);

			glm::mat4 pieza = glm::translate(base, glm::vec3(x, y, 0.0f));
			pieza = glm::rotate(pieza, ang, glm::vec3(0.0f, 0.0f, 1.0f));

			// Cordon central (segmento corto)
			dibujar(cubo, glm::scale(pieza, glm::vec3(0.18f, 0.05f, 0.05f)), blanco);

			// Travesanos (5 en total)
			if (i >= 1 && i <= 5)
				dibujar(cubo, glm::scale(pieza, glm::vec3(0.06f, 0.05f, 0.30f)), blanco);
		}

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &cubo.vao);   glDeleteBuffers(1, &cubo.vbo);
	glDeleteVertexArrays(1, &balon.vao);  glDeleteBuffers(1, &balon.vbo);
	glDeleteVertexArrays(1, &franja.vao); glDeleteBuffers(1, &franja.vbo);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Camara
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)     movX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)     movX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    movY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  movY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)     movZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)     movZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  rot -= 0.5f;

	// Balon
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) giro += 0.5f;     // gira sobre su eje largo
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) giro -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) cabeceo += 0.5f;  // inclina la punta
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) cabeceo -= 0.5f;
}