#include "Common.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"

constexpr int WIDTH = 1366, HEIGHT = 768;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
Camera camera;
Texture brickTexture;
Texture dirtTexture;
Material shinyMaterial;
Material dullMaterial;
DirectionalLight mainLight;
std::vector<Mesh*> meshes;
std::vector<Shader*> shaders;

float deltaTime{ 0.0f };
float lastTime{ 0.0f };

// Vertex Shader
static const char* vShader = "..\\OpenGL\\Shaders\\vshader.glsl";

// Fragment shader
static const char* fShader = "..\\OpenGL\\Shaders\\fshader.glsl";

void CalcAverageNormals(unsigned int* indices, unsigned int numIndices, float* vertices, unsigned int numVertices, unsigned int vLength, unsigned int nOffset)
{
	for (unsigned int i{ 0 }; i < numIndices; i += 3)
	{
		unsigned int in0{ indices[i] * vLength };
		unsigned int in1{ indices[i + 1] * vLength };
		unsigned int in2{ indices[i + 2] * vLength };

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += nOffset;
		in1 += nOffset;
		in2 += nOffset;

		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (unsigned int i{ 0 }; i < numVertices / vLength; i++)
	{
		unsigned int normalOffset = i * vLength + nOffset;
		glm::vec3 vec(vertices[normalOffset], vertices[normalOffset + 1], vertices[normalOffset + 2]);
		vec = glm::normalize(vec);
		vertices[normalOffset] = vec.x; vertices[normalOffset + 1] = vec.y; vertices[normalOffset + 2] = vec.z;
	}
}

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
	//	 x		y	  z				u	  v			nx	  ny	nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshes.emplace_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
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

	camera = Camera(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.1f);
	brickTexture = Texture((char*)"..\\OpenGL\\Assets\\brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture((char*)"..\\OpenGL\\Assets\\dirt.png");
	dirtTexture.LoadTexture();
	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(2.0f, -1.0f, -2.0f, 1.0f, 1.0f, 1.0f, 0.2f, 0.3f);

	unsigned int uniformProjection{ 0 }, uniformModel{ 0 }, uniformView{ 0 }, uniformEyePosition{ 0 }, uniformSpecularIntensity{ 0 }, uniformShininess{ 0 };
	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    // Loop until the user closes the window
    while (!mainWindow.GetShouldClose())
    {
		float now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// Render here
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel = shaders[0]->GetModelLocation();
		uniformProjection = shaders[0]->GetProjectionLocation();
		uniformView = shaders[0]->GetViewLocation();
		uniformEyePosition = shaders[0]->GetEyePositionLocation();
		uniformSpecularIntensity = shaders[0]->GetSpecularIntensityLocation();
		uniformShininess = shaders[0]->GetShininessLocation();

		shaders[0]->SetDirectionalLight(&mainLight);
		

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshes[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshes[1]->RenderMesh();


		glUseProgram(0);

        // Swap front and back buffers
		mainWindow.SwapBuffers();

        // Poll for and process events
        glfwPollEvents();
		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());
    }

    return 0;
}