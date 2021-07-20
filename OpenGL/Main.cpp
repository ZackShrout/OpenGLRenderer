#include "Common.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

constexpr int WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
Camera camera;
std::vector<Mesh*> meshes;
std::vector<Shader*> shaders;

float deltaTime{ 0.0f };
float lastTime{ 0.0f };

// <----------------- Get rid of this code ------------------>
bool shouldGoRight{ true };
bool shouldGoUp{ true };
bool shouldGrow{ true };
float triXOffset{ 0.0f };
float triYOffset{ 0.0f };
float triMaxOffset{ 0.6f };
float triXIncrement{ 0.00005f };
float triYIncrement{ 0.00008f };
float curAngle{ 0.0f };
float curSize{ 0.4f };
float maxSize{ 0.8f };
float minSize{ 0.1f };
// <-------------------------- end -------------------------->

// Vertex Shader
static const char* vShader = "..\\OpenGL\\Shaders\\vshader.glsl";

// Fragment shader
static const char* fShader = "..\\OpenGL\\Shaders\\fshader.glsl";

void CreateObjects()
{
	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	
	float vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0,  0.0f,
		 0.0f,  1.0f, 0.5f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshes.emplace_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshes.emplace_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaders.emplace_back(shader1);
}

int main(void)
{
	mainWindow = Window(WIDTH, HEIGHT);
	if (mainWindow.Initialize() != 0)
	{
		std::cout << "Could not initialize window." << std::endl;
		return 1;
	}

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.1f);

	unsigned int uniformProjection{ 0 }, uniformModel{ 0 }, uniformView{ 0 };
	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    // Loop until the user closes the window
    while (!mainWindow.GetShouldClose())
    {
		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// Render here
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel = shaders[0]->GetModelLocation();
		uniformProjection = shaders[0]->GetProjectionLocation();
		uniformView = shaders[0]->GetViewLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triXOffset, triYOffset, -2.5f)); // change to 0.0f, 0.0f, -2.5f
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // get rid of
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

		meshes[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-triXOffset, -triYOffset, -3.5f)); // change to 0.0f, 1.0f, -2.5f
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // get rid of
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshes[1]->RenderMesh();


		glUseProgram(0);

        // Swap front and back buffers
		mainWindow.SwapBuffers();

        // Poll for and process events
        glfwPollEvents();
		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		// <----------------- Get rid of this code ------------------>
		if (shouldGoRight)
		{
			triXOffset += triXIncrement;
		}
		else
		{
			triXOffset -= triXIncrement;
		}

		if (abs(triXOffset) >= triMaxOffset)
			shouldGoRight = !shouldGoRight;

		if (shouldGoUp)
		{
			triYOffset += triYIncrement;
		}
		else
		{
			triYOffset -= triYIncrement;
		}

		if (abs(triYOffset) >= triMaxOffset)
			shouldGoUp = !shouldGoUp;

		curAngle += 0.005f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (shouldGrow)
		{
			curSize += 0.00001f;
		}
		else
		{
			curSize -= 0.00001f;
		}

		if (abs(curSize) >= maxSize || abs(curSize) <= minSize)
		{
			shouldGrow = !shouldGrow;
		}
		// <-------------------------- end -------------------------->
    }

    return 0;
}