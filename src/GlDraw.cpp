#include "GlDraw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader_m.h"
#include "Common.h"

Shader* ourShader{};
unsigned int texture1, texture2;

void glInit()
{
    ourShader = new Shader("3.3.shader.vs", "3.3.shader.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int EBO;
    glGenVertexArrays(1, &GlNs::gData.VAO);
    glGenBuffers(1, &GlNs::gData.VBO);
    glBindVertexArray(GlNs::gData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GlNs::gData.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("../resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader->use();
    glUniform1i(glGetUniformLocation(ourShader->ID, "texture1"), 0);
    ourShader->setInt("texture2", 1);
}

void glDraw()
{
    if (GlNs::gData.depthTesting)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glClearColor(GlNs::gData.clear_color.x * GlNs::gData.clear_color.w, GlNs::gData.clear_color.y * GlNs::gData.clear_color.w, GlNs::gData.clear_color.z * GlNs::gData.clear_color.w, GlNs::gData.clear_color.w);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    auto transform = glm::mat4{ 1.0f };
    transform = glm::translate(transform, glm::vec3{ 0.5f, -0.5f, 0.0f });
    transform = glm::rotate(transform, glm::radians(GlNs::gData.ang), glm::vec3{ 0.f, 0.f, 1.f });

    ourShader->use();
    glm::mat4 model{ 1.f };
    glm::mat4 view{ 1.f };
    glm::mat4 projection{ 1.f };
    if (GlNs::gData.rotateByTime)
    {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    }
    else
    {
        model = glm::rotate(model, glm::radians(GlNs::gData.ang), glm::vec3{ 1.f,0.f,0.f });
    }
    //model = glm::rotate(model, (float)std::time(nullptr) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    if (GlNs::gData.camera.autoRotation)
    {
        auto radius = 10.f;
        float camX = std::sin(glfwGetTime()) * radius;
        float camZ = std::cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 0.f, camZ), glm::vec3(0.f, 0.f, 0.f), glm::vec3{ 0.f, 1.f, 0.f });
    }
    else
    {
        auto& camera = GlNs::gData.camera;
        view = camera.camera.GetViewMatrix();
        //view = glm::lookAt(camera.camera.Position, camera.camera.Position + camera.camera.Front, camera.camera.Up);
        //view = glm::translate(view, glm::vec3{ 0.f,0.f,-2.f });
    }
    projection = glm::perspective(glm::radians(GlNs::gData.camera.camera.Zoom), float(GlNs::gData.windowW) / GlNs::gData.windowH, GlNs::gData.nearP, GlNs::gData.farP);
    ourShader->setMat4("model", model);
    ourShader->setMat4("view", view);
    ourShader->setMat4("projection", projection);
    //auto transformLoc = glGetUniformLocation(ourShader->ID, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(GlNs::gData.VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    if (GlNs::gData.showMore)
    {
        glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        for (auto i = 0; i < std::size(cubePositions); ++i)
        {
            glm::mat4 model{ 1.f };
            model = glm::translate(model, cubePositions[i]);
            if (GlNs::gData.rotateByTime)
            {
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            else
            {
                float angle = 20.f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3{ 1.0f, 0.3f, 0.5f });
            }
            ourShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
