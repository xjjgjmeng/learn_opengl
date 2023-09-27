#include "Volume.h"

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <learnopengl/shader_m.h>

#include "Common.h"

const int volumn_w = 651;
const int volumn_h = 651;
const int volumn_d = 384;
Volume& Volume::instance()
{
    static Volume obj{};
    return obj;
}
Volume::Volume()
{
    pShader = new Shader("transform.vs", "transform.fs");;
    float vertices[] = {
#if 1 // back
        - 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,   //左下
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,    // 右下
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,     // 右上
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,     // 右上
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,     // 左上
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,    // 左下
#endif

#if 1 // front
        - 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,   // 左下
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,     // 右下
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,    // 右上
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,   //右上
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,  // 左上
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  // 左下
#endif

#if 1 // left
        - 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f, // 上前
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  // 上后
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, // 下后
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, // 下后
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, // 下前
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f, // 上前
#endif

#if 1 // right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,// 上前
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  // 上后
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, // 下后
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,  // 下后
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  // 下前
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f, // 上前
#endif

#if 1 // bottom
        - 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, //  左后
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,  // 右后
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f, // 右前
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f,   // 右前
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,   // 左前
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f, //  左后
#endif

#if 1 // top
        - 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  //  左后
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,  // 右后
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,  // 右前
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f,    // 右前
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,  // 左前
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f  //  左后
#endif
    };


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_3D, texture0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const std::string filename = "ReconData_CT_0_384_651_651_20210316151257.dat";
    const int size = volumn_w * volumn_h * volumn_d;
    auto mydata = std::make_unique<short[]>(size);

    auto pFile = std::fopen(filename.c_str(), "rb");
    std::fread(mydata.get(), sizeof mydata[0], size, pFile);
    std::fclose(pFile);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(texture0);
    // GL_LUMINANCE
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16_SNORM, volumn_w, volumn_h, volumn_d, 0, GL_RED, GL_SHORT, mydata.get());
    glGenerateMipmap(GL_TEXTURE_3D);

    pShader->use();
    glUniform1i(glGetUniformLocation(pShader->ID, "texture0"), 0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

Volume::~Volume()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
const glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
void Volume::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, texture0);

    glm::mat4 matMolde = glm::mat4(1.0f);
#if 1
    matMolde = glm::rotate(matMolde, glm::radians(m_rotateY), glm::vec3(0.0, 1.0, 0.0));
    matMolde = glm::rotate(matMolde, glm::radians(m_rotateX), glm::vec3(1.0, 0.0, 0.0));
    matMolde = glm::rotate(matMolde, glm::radians(m_rotateZ), glm::vec3(0.0, 0.0, 1.0));
    m_rotateX++;
#else
    static int n = 0;
    const auto ang = static_cast<float>(std::sin(m_rotateY / 3) * 360.f);
    matMolde = glm::rotate(matMolde, glm::radians(ang), glm::vec3{ 1.f,1.f,1.f });
#endif
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)GlNs::gData.windowW / GlNs::gData.windowH, 0.1f, 100.0f);

    pShader->use();
    pShader->setMat4("model", matMolde);
    pShader->setMat4("view", view);
    pShader->setMat4("projection", projection);

    {
        unsigned int transformLoc7 = glGetUniformLocation(pShader->ID, "camPos");
        glUniformMatrix4fv(transformLoc7, 1, GL_FALSE, glm::value_ptr(camPos));
    }

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);
}
