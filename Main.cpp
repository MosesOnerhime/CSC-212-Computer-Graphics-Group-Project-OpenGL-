#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stbimage.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VAO.h"
#include"EBO.h"


const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates for a cube
GLfloat vertices[] = 
{
    // Positions          // Colors           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.83f, 0.70f, 0.44f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.83f, 0.70f, 0.44f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.83f, 0.70f, 0.44f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.83f, 0.70f, 0.44f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.83f, 0.70f, 0.44f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.83f, 0.70f, 0.44f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.83f, 0.70f, 0.44f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.83f, 0.70f, 0.44f,  0.0f, 1.0f
};

// Indices for the vertices order for a cube
 GLuint indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 1, 5, 5, 4, 0,
    2, 3, 7, 7, 6, 2,
    0, 3, 7, 7, 4, 0,
    1, 2, 6, 6, 5, 1
};


int main()
{
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, width, height);

    
    
    Shader shaderProgram("default.vert", "default.frag");

    // Generate Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generate Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generate Elements Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture

    Texture aSummerDragon("Brick.jpeg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    aSummerDragon.texUnit(shaderProgram, "tex0", 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        //Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        //Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= static_cast<double>(1) / 60)
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Assign a value to the uniform; NOTE: Must always be done after activatin the Shader program 
        glUniform1f(uniID, 0.5f);
        aSummerDragon.Bind();
        // Bind the VAO so OpenGL knows to use it 
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        //Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    aSummerDragon.Delete();
    shaderProgram.Delete();

    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}