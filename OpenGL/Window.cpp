#include "Window.h"

// PUBLIC
Window::Window() : m_width{ 800 }, m_height{ 600 }, m_bufferWidth{ 0 }, m_bufferHeight{ 0 }, m_mainWindow{ nullptr },
                    m_lastX{ 0 }, m_lastY{ 0 }, m_xChange{ 0 }, m_yChange{ 0 }, m_mouseFirstMoved{ true }, m_keys{ 0 }
{
    /*for (auto& key : m_keys)
    {
        key = 0;
    }*/
}

Window::Window(GLint windowWidth, GLint windowHeight) : m_width{ windowWidth }, m_height{ windowHeight }, m_bufferWidth{ 0 } , m_bufferHeight{ 0 }, m_mainWindow{ nullptr },
                                                        m_lastX{ 0 }, m_lastY{ 0 }, m_xChange{ 0 }, m_yChange{ 0 }, m_mouseFirstMoved{ true }, m_keys{ 0 }
{
    /*for (auto& key : m_keys)
    {
        key = 0;
    }*/
}

Window::~Window()
{
    glfwDestroyWindow(m_mainWindow);
    glfwTerminate();
}

int Window::Initialize()
{
    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    m_mainWindow = glfwCreateWindow(m_width, m_height, "Test Window", NULL, NULL);
    if (!m_mainWindow)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_mainWindow);

    // Get buffer size information
    glfwGetFramebufferSize(m_mainWindow, &m_bufferWidth, &m_bufferHeight);

    // Setup key & mouse input
    CreateCallbacks();

    // Lock cursor to window && make it invisible
    glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize Glad." << std::endl;
        glfwDestroyWindow(m_mainWindow);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, m_bufferWidth, m_bufferHeight);

    // Set the main window as the user
    glfwSetWindowUserPointer(m_mainWindow, this);
	
	return 0;
}

float Window::GetXChange()
{
    GLfloat change = m_xChange;
    m_xChange = 0.0f;
    
    return change;
}

float Window::GetYChange()
{
    GLfloat change = m_yChange;
    m_yChange = 0.0f;

    return change;
}

// PRIVATE
void Window::CreateCallbacks()
{
    glfwSetKeyCallback(m_mainWindow, HandleKeys);
    glfwSetCursorPosCallback(m_mainWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* hWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            hWindow->m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            hWindow->m_keys[key] = false;
        }
    }
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* hWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (hWindow->m_mouseFirstMoved)
    {
        hWindow->m_lastX = (float)xPos;
        hWindow->m_lastY = (float)yPos;
        hWindow->m_mouseFirstMoved = false;
    }

    hWindow->m_xChange = (float)xPos - hWindow->m_lastX;
    hWindow->m_yChange = hWindow->m_lastY - (float)yPos; // flip this for inverted controls
    hWindow->m_lastX = (float)xPos;
    hWindow->m_lastY = (float)yPos;
}
