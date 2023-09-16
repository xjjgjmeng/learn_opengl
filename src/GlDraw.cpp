#include "GlDraw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "shader_m.h"
#include "Common.h"
#include <learnopengl/model.h>

Shader* ourShader{};
unsigned int texture1, texture2;

Shader* lightCubeShader{};

Model* myModel{};

namespace
{
    // utility function for loading a 2D texture from file
// ---------------------------------------------------
    unsigned int loadTexture(char const* path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    unsigned int diffuseMap, specularMap;
}

void glInit()
{
    ourShader = new Shader("3.3.shader.vs", "3.3.shader.fs");
    lightCubeShader = new Shader{ "light_cube.vs", "light_cube.fs" };

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

    diffuseMap = loadTexture("../resources/textures/container2.png");
    specularMap = loadTexture("../resources/textures/container2_specular.png");
    ourShader->use();
    ourShader->setInt("material.texture_diffuse1", 0);
    ourShader->setInt("material.texture_specular1", 1);

    myModel = new Model("../resources/objects/backpack/backpack.obj");
    //myModel = new Model("../resources/objects/nanosuit/nanosuit.obj");
}

void glDraw()
{
    GlNs::gData.depthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glClearColor(GlNs::gData.clear_color.x * GlNs::gData.clear_color.w, GlNs::gData.clear_color.y * GlNs::gData.clear_color.w, GlNs::gData.clear_color.z * GlNs::gData.clear_color.w, GlNs::gData.clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    if (GlNs::gData.lamp.autoRotation)
    {
        auto radius = 2.f;
        const auto camX = static_cast<float>(std::sin(glfwGetTime()) * radius);
        const auto camZ = static_cast<float>(std::cos(glfwGetTime()) * radius);
        //std::cout << camX << " " << camZ << std::endl;;// " " << std::sin(glfwGetTime()) << " " << glfwGetTime() << "\n";
        GlNs::gData.pointLightPositions[0].x = camX;
        GlNs::gData.pointLightPositions[0].z = camZ;
    }

    glm::mat4 model{ 1.f };
    glm::mat4 view{ 1.f };
    glm::mat4 projection{ 1.f };
    if (GlNs::gData.rotateByTime)
    {
        GlNs::gData.ang = static_cast<float>(std::sin(glfwGetTime() / 3) * 360.f);
    }
    model = glm::rotate(model, glm::radians(GlNs::gData.ang), glm::vec3{ 1.f,1.f,0.f });

    if (GlNs::gData.camera.autoRotation)
    {
#if 1
        auto radius = 8.f;
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

    ourShader->setBool("u_light.dir", GlNs::gData.lamp.dir);
    ourShader->setBool("u_light.spot", GlNs::gData.lamp.spot);
    ourShader->setBool("u_light.point", GlNs::gData.lamp.point);
    ourShader->setBool("u_light.attenuation", GlNs::gData.lamp.attenuation);
    ourShader->setBool("u_light.softEdges", GlNs::gData.lamp.softEdges);
    //switch (GlNs::gData.lamp.type)
    {
    //case LightType::Directional:
        //ourShader->setVec3("light.direction", -GlNs::gData.lamp.pos);
#if 0
        ourShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
#else
        ourShader->setVec3("dirLight.direction", -GlNs::gData.pointLightPositions[0]);
#endif
        ourShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //break;
    //case LightType::Point:

        // point light 1
        ourShader->setVec3("pointLights[0].position", GlNs::gData.pointLightPositions[0]);
        ourShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[0].constant", 1.0f);
        ourShader->setFloat("pointLights[0].linear", 0.09f);
        ourShader->setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        ourShader->setVec3("pointLights[1].position", GlNs::gData.pointLightPositions[1]);
        ourShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[1].constant", 1.0f);
        ourShader->setFloat("pointLights[1].linear", 0.09f);
        ourShader->setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        ourShader->setVec3("pointLights[2].position", GlNs::gData.pointLightPositions[2]);
        ourShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[2].constant", 1.0f);
        ourShader->setFloat("pointLights[2].linear", 0.09f);
        ourShader->setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        ourShader->setVec3("pointLights[3].position", GlNs::gData.pointLightPositions[3]);
        ourShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[3].constant", 1.0f);
        ourShader->setFloat("pointLights[3].linear", 0.09f);
        ourShader->setFloat("pointLights[3].quadratic", 0.032f);
        //break;
    //case LightType::Spot:

#if 0
        ourShader->setVec3("spotLight.position", GlNs::gData.camera.camera.Position);
        ourShader->setVec3("spotLight.direction", GlNs::gData.camera.camera.Front);
#else
        ourShader->setVec3("spotLight.position", GlNs::gData.pointLightPositions[0]);
        ourShader->setVec3("spotLight.direction", -GlNs::gData.pointLightPositions[0]);
#endif
        ourShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("spotLight.constant", 1.0f);
        ourShader->setFloat("spotLight.linear", 0.09f);
        ourShader->setFloat("spotLight.quadratic", 0.032f);
        ourShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        //break;
    //default:
        //break;
    }

    ourShader->setVec3("viewPos", GlNs::gData.camera.camera.Position);
    //ourShader->setInt("shininess", GlNs::gData.shininess);
    ourShader->setBool("showSmile", GlNs::gData.showSmile);
    glm::vec3 lightColor;
    lightColor.x = static_cast<float>(std::sin(glfwGetTime()*2.0));
    lightColor.y = static_cast<float>(std::sin(glfwGetTime()*0.7));
    lightColor.z = static_cast<float>(std::sin(glfwGetTime()*1.3));
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
#if 0
    ourShader->setVec3("light.ambient", ambientColor);
    ourShader->setVec3("light.diffuse", diffuseColor);
    ourShader->setVec3("light.specular", 1.f, 1.f, 1.f);
#else
    ourShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    ourShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    ourShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
#endif
    ourShader->setVec3("material.ambient", 1.f, .5f, .31f);
    ourShader->setVec3("material.diffuse", 1.f, .5f, .31f);
    ourShader->setVec3("material.specular", .5f, .5f, .5f);
    ourShader->setFloat("material.shininess", GlNs::gData.shininess);

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

    const auto modelData = GlNs::gData.model;
    if (GlNs::gData.model.visible)
    {
        glm::mat4 model{ 1.f };
        model = glm::translate(model, modelData.pos);
        model = glm::rotate(model, glm::radians(modelData.rotate), glm::vec3{ 1.f,0.f,0.f });
        model = glm::scale(model, glm::vec3(modelData.scale));
        ourShader->setMat4("model", model);
        myModel->Draw(*ourShader);
    }

    // »æÖÆ¹âÔ´
    {
        lightCubeShader->use();
        lightCubeShader->setVec3("lightColor", GlNs::gData.lamp_color.x, GlNs::gData.lamp_color.y, GlNs::gData.lamp_color.z);
        lightCubeShader->setMat4("projection", projection);
        lightCubeShader->setMat4("view", view);

        glBindVertexArray(GlNs::gData.lightCubeVAO);
        for (auto i = 0; i < 4; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, GlNs::gData.pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}
