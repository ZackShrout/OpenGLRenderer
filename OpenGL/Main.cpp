#include "Common.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

constexpr int WIDTH = 1366, HEIGHT = 768;
const float toRadians = 3.14159265f / 180.0f;

unsigned int uniformProjection{ 0 }, uniformModel{ 0 }, uniformView{ 0 }, uniformEyePosition{ 0 }, uniformSpecularIntensity{ 0 }, uniformShininess{ 0 };
unsigned int pointLightCount{ 0 }, spotLightCount{ 0 }, uniformLightPos{ 0 }, uniformFarPlane{ 0 };
Window mainWindow;
Camera camera;
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Material shinyMaterial;
Material dullMaterial;
Model xWing;
Model blackHawk;
DirectionalLight mainLight;
PointLight pointLight[MAX_POINT_LIGHTS];
SpotLight spotLight[MAX_SPOT_LIGHTS];
Skybox skybox;
std::vector<Mesh*> meshes;
std::vector<Shader*> shaders;
Shader directionalShadowShader;
Shader omniShadowShader;

float deltaTime{ 0.0f };
float lastTime{ 0.0f };

GLfloat blackhawkAngle = 0.0f;

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

	unsigned int floorIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	float floorVertices[] =
	{
	//	 x		y	   z			 u		v		nx	   ny	 nz
		-10.0f, 0.0f, -10.0f,		 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,		10.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,		 0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshes.emplace_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshes.emplace_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshes.emplace_back(obj3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaders.emplace_back(shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFile("..\\OpenGL\\Shaders\\vdirShadowMap.glsl", "..\\OpenGL\\Shaders\\fdirShadowMap.glsl");
	omniShadowShader.CreateFromFile("..\\OpenGL\\Shaders\\vomniShadowMap.glsl", "..\\OpenGL\\Shaders\\fomniShadowMap.glsl", "..\\OpenGL\\Shaders\\gomniShadowMap.glsl");
}

void RenderScene()
{
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

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

	meshes[2]->RenderMesh();

	// X-Wing
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

	xWing.RenderModel();

	// Blackhawk
	blackhawkAngle += 0.03f;
	if (blackhawkAngle > 360.0f)
	{
		blackhawkAngle = 0.1f;
	}

	model = glm::mat4(1.0f);
	model = glm::rotate(model, -blackhawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

	blackHawk.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	
	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform()); // TODO: this should pass by const reference...

	omniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	// Set viewport size
	glViewport(0, 0, 1366, 768);

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw Skybox
	skybox.DrawSkybox(viewMatrix, projectionMatrix);
	
	// Draw rest of scene
	shaders[0]->UseShader();
	uniformModel = shaders[0]->GetModelLocation();
	uniformProjection = shaders[0]->GetProjectionLocation();
	uniformView = shaders[0]->GetViewLocation();
	uniformEyePosition = shaders[0]->GetEyePositionLocation();
	uniformSpecularIntensity = shaders[0]->GetSpecularIntensityLocation();
	uniformShininess = shaders[0]->GetShininessLocation();
	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaders[0]->SetDirectionalLight(&mainLight);
	shaders[0]->SetPointLight(pointLight, pointLightCount, 3, 0);
	shaders[0]->SetSpotLight(spotLight, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaders[0]->SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaders[0]->SetTexture(1);
	shaders[0]->SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.GetCameraPosition();
	lowerLight.y -= 0.3f;
	spotLight[0].SetFlash(lowerLight, camera.GetCameraDirection());

	shaders[0]->Validate();

	RenderScene();
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
	brickTexture = Texture((char*)"..\\OpenGL\\Textures\\brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture((char*)"..\\OpenGL\\Textures\\dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture((char*)"..\\OpenGL\\Textures\\plain.png");
	plainTexture.LoadTextureA();
	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	xWing = Model();
	xWing.LoadModel("..\\OpenGL\\Models\\x-wing.obj");

	blackHawk = Model();
	blackHawk.LoadModel("..\\OpenGL\\Models\\uh60.obj");

	mainLight = DirectionalLight(-10.0f, -12.0f, 18.5f, 1.0f, 0.53f, 0.3f, 0.1f, 0.9f, 2048, 2048);

	pointLight[0] = PointLight(2.0f, 2.0f, 0.0f, 0.3, 0.2, 0.1, 0.1, 100, 0.0f, 0.0f, 1.0f, 0.0f, 0.4f, 1024, 1024);
	pointLightCount++;
	pointLight[1] = PointLight(-2.0f, 2.0f, 0.0f, 0.3, 0.2, 0.1, 0.1, 100, 0.0f, 1.0f, 0.0f, 0.0f, 0.4f, 1024, 1024);
	pointLightCount++;

	spotLight[0] = SpotLight(0.0f, -1.0f, 0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.3, 0.2, 0.1, 0.1, 100, 1.0f, 1.0f, 1.0f, 0.1f, 2.0f, 1024, 1024);
	spotLightCount++;

	spotLight[1] = SpotLight(-100.0f, -1.0f, 0.0f, 20.0f, 0.0f, -1.5f, 0.0f, 0.3f, 0.2f, 0.1f, 0.1f, 100.0f, 1.0f, 1.0f, 1.0f, 0.1f, 0.2f, 1024, 1024);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.emplace_back("..\\OpenGL\\Textures\\Skybox\\cupertin-lake_rt.tga");
	skyboxFaces.emplace_back("..\\OpenGL\\Textures\\Skybox\\cupertin-lake_lf.tga");
	skyboxFaces.emplace_back("..\\OpenGL\\Textures\\Skybox\\cupertin-lake_up.tga");
	skyboxFaces.emplace_back("..\\OpenGL\\Textures\\Skybox\\cupertin-lake_dn.tga");
	skyboxFaces.emplace_back("..\\OpenGL\\Textures\\Skybox\\cupertin-lake_bk.tga");
	skyboxFaces.emplace_back("..\\OpenGL\\Textures\\Skybox\\cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    // Loop until the user closes the window
    while (!mainWindow.GetShouldClose())
    {
		float now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// Render here
		DirectionalShadowMapPass(&mainLight);

		for (int i{ 0 }; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLight[i]);
		}

		for (int i{ 0 }; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLight[i]);
		}

		RenderPass(projection, camera.CalculateViewMatrix());

		glUseProgram(0);

        // Swap front and back buffers
		mainWindow.SwapBuffers();

        // Poll for and process events
        glfwPollEvents();
		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		if (mainWindow.GetKeys()[GLFW_KEY_SPACE])
		{
			spotLight[0].Toggle();
			mainWindow.GetKeys()[GLFW_KEY_SPACE] = false;
		}
    }

    return 0;
}