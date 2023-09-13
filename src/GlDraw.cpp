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

Shader* lightCubeShader{};

void glInit()
{
    ourShader = new Shader("3.3.shader.vs", "3.3.shader.fs");
    lightCubeShader = new Shader{ "light_cube.vs", "light_cube.fs" };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };
    //unsigned int indices[] = {
    //    0, 1, 3, // first triangle
    //    1, 2, 3  // second triangle
    //};
    //unsigned int EBO;
    glGenVertexArrays(1, &GlNs::gData.VAO);
    glGenBuffers(1, &GlNs::gData.VBO);
    glBindVertexArray(GlNs::gData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GlNs::gData.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &GlNs::gData.lightCubeVAO);
    glBindVertexArray(GlNs::gData.lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, GlNs::gData.VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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
    GlNs::gData.depthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glClearColor(GlNs::gData.clear_color.x * GlNs::gData.clear_color.w, GlNs::gData.clear_color.y * GlNs::gData.clear_color.w, GlNs::gData.clear_color.z * GlNs::gData.clear_color.w, GlNs::gData.clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    if (GlNs::gData.lamp.autoRotation)
    {
        auto radius = 2.f;
        const auto camX = static_cast<float>(std::sin(glfwGetTime()) * radius);
        const auto camZ = static_cast<float>(std::cos(glfwGetTime()) * radius);
        //std::cout << camX << " " << camZ << std::endl;;// " " << std::sin(glfwGetTime()) << " " << glfwGetTime() << "\n";
        GlNs::gData.lamp.pos.x = camX;
        GlNs::gData.lamp.pos.z = camZ;
    }

    auto transform = glm::mat4{ 1.0f };
    transform = glm::translate(transform, glm::vec3{ 0.5f, -0.5f, 0.0f });
    transform = glm::rotate(transform, glm::radians(GlNs::gData.ang), glm::vec3{ 0.f, 0.f, 1.f });

    glm::mat4 model{ 1.f };
    glm::mat4 view{ 1.f };
    glm::mat4 projection{ 1.f };
    if (GlNs::gData.rotateByTime)
    {
        GlNs::gData.ang = static_cast<float>(std::sin(glfwGetTime() / 3) * 360.f);
    }
    model = glm::rotate(model, glm::radians(GlNs::gData.ang), glm::vec3{ 1.f,0.f,0.f });

    if (GlNs::gData.camera.autoRotation)
    {
#if 1
        auto radius = 3.f;
        const auto camX = static_cast<float>(std::sin(glfwGetTime()) * radius);
        const auto camZ = static_cast<float>(std::cos(glfwGetTime()) * radius);
        view = glm::lookAt(glm::vec3(camX, 0.f, camZ), glm::vec3(0.f, 0.f, 0.f), glm::vec3{ 0.f, 1.f, 0.f });
#else
        GlNs::gData.camera.camera.Position = glm::vec3(camX, 0.f, camZ);
        view = GlNs::gData.camera.camera.GetViewMatrix();
#endif
    }
    else
    {
        auto& camera = GlNs::gData.camera;
        view = camera.camera.GetViewMatrix();
    }
    projection = glm::perspective(glm::radians(GlNs::gData.camera.camera.Zoom), float(GlNs::gData.windowW) / GlNs::gData.windowH, GlNs::gData.nearP, GlNs::gData.farP);
    ourShader->use();
    ourShader->setVec3("lightColor", GlNs::gData.lamp_color.x, GlNs::gData.lamp_color.y, GlNs::gData.lamp_color.z);
    ourShader->setMat4("model", model);
    ourShader->setMat4("view", view);
    ourShader->setMat4("projection", projection);
    ourShader->setVec3("lightPos", GlNs::gData.lamp.pos);
    ourShader->setVec3("viewPos", GlNs::gData.camera.camera.Position);
    ourShader->setInt("shininess", GlNs::gData.shininess);

    glBindVertexArray(GlNs::gData.VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    if (GlNs::gData.showMore)
    {
        const glm::vec3 cubePositions[] = {
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
            model = glm::rotate(model, glm::radians(GlNs::gData.ang), glm::vec3{ 1.f,0.f,0.f });
            ourShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // »æÖÆ¹âÔ´
    {
        lightCubeShader->use();
        lightCubeShader->setVec3("lightColor", GlNs::gData.lamp_color.x, GlNs::gData.lamp_color.y, GlNs::gData.lamp_color.z);
        lightCubeShader->setMat4("projection", projection);
        lightCubeShader->setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, GlNs::gData.lamp.pos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader->setMat4("model", model);
        glBindVertexArray(GlNs::gData.lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
