//GLAD���������GLFW֮ǰ��GLAD��ͷ�ļ���������ȷ��OpenGLͷ�ļ�������GL/gl.h��
//������Ҫ������������OpenGL��ͷ�ļ�֮ǰ����GLAD��
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "WindowMgr.h"

static float lastX = WindowMgr::SCR_WIDTH / 2.0f;
static float lastY = WindowMgr::SCR_HEIGHT / 2.0f;
static bool firstMouse = true;
static Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
    glViewport(0, 0, width, height);
}

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


/*
����1��ͼ����Ҫ
1����VAO����glGenVertexArrays��glBindVertexArray��
2����VBO�����涥�����ݣ���glGenBuffer��glBindBuffer��glBufferData��
3����EBO(Ҳ��IBO)�����涥���������ݡ�glGenBuffer��glBindBuffer��glBufferData��
4��������ɫ���������Ƭ�Σ�����glCreateShader��glShaderSource��glCompileShader��
5��������ɫ������glCreateProgram��glAttachShader��glLinkProgram��glUseProgram��
6�����Ӷ������ԣ���glVertexAttribPointer��glEnableVertexAttribArray��
7�����ƣ���glPolygonMode����ѡ����glDrawElements/glDrawArrays��
*/

//float vertices[] = {
//    /*������*/
//    -0.5f,-0.5f,0.0f,
//    0.5f, -0.5f,0.0f,
//    0.0f,  0.5f,0.0f
//
//    ///*����*/
//    //0.5f, 0.5f, 0.0f,   // ���Ͻ�
//    //0.5f, -0.5f, 0.0f,  // ���½�
//    //-0.5f, -0.5f, 0.0f, // ���½�
//    //-0.5f, 0.5f, 0.0f   // ���Ͻ�
//};

//float vertices[] = {
//	// λ��              // ��ɫ
//	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
//	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
//};

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
    0,1,2,
    1,2,3
};



int main()
{
    float array[1000];
    for (int i = 0; i < 1000; ++i)
    {
        array[i] = 0.05f / 0.31f;
    }
    srand(0);

    WindowMgr windowMgr(&camera);
    bool success = windowMgr.CreateWindow(windowMgr.SCR_WIDTH, windowMgr.SCR_HEIGHT);
    if (!success)
    {
        return -1;
    }
    windowMgr.SetFramebufferSizeCallback(framebuffer_size_callback);
    windowMgr.SetSetCursorPosCallback(mouse_callback);
    windowMgr.SetScrollCallback(scroll_callback);

    //----------------��VAO--------------------start
    //�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣�
    //�κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ�
    //�����ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO������
    //�������õ�����״̬�����洢��VAO��(����EBO)
    //һ��VAO����ᴢ��������Щ���ݣ�
    //glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ��á�
    //ͨ��glVertexAttribPointer���õĶ����������á�
    //ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //OpenGL�ĺ���ģʽҪ������ʹ��VAO��������֪������δ������ǵĶ������롣
    //������ǰ�VAOʧ�ܣ�OpenGL��ܾ������κζ�����
    //----------------��VAO--------------------end

    //----------------���涥������--------------------start
    //��glGenBuffers������һ������ID����һ��VBO����
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������GL_ARRAY_BUFFER��OpenGL��������ͬʱ�󶨶�����壬
    //ֻҪ�����ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
    //Ȼ�����ǿ��Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ��У�
    //glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
    //���ĵ�һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
    //�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
    //����������������ϣ�����͵�ʵ�����ݡ�
    //���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
    //GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
    //GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
    //GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //----------------���涥������--------------------end

    //----------------���涥����������--------------------start
    /*unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    //----------------���涥����������--------------------end



    //----------------���Ӷ�������--------------------start
    //���㻺�����ݻᱻ����Ϊ���������ӣ�
    //λ�����ݱ�����Ϊ32λ��4�ֽڣ�����ֵ��
    //ÿ��λ�ð���3��������ֵ��
    //����3��ֵ֮��û�п�϶��������ֵ�����⼸��ֵ�������н�������(Tightly Packed)��
    //�����е�һ��ֵ�ڻ��忪ʼ��λ�á�
    //������Щ��Ϣ���ǾͿ���ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ��ˣ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glVertexAttribPointer�����Ĳ����ǳ��ࣺ
    // ��һ������ָ��Ҫ���õĶ������Ե�λ��ֵ��
    // �ڶ�������ָ���������ԵĴ�С��
    // ����������ָ�����ݵ����ͣ�������GL_FLOAT
    // ���ĸ�����ָ���Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE��
    // �������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮��
    // �����������������(Stride)����������������������֮��ļ����
    // ���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת����
    // ����ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)
    // 
    // glEnableVertexAttribArray�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
    // 
    // 
    //----------------���Ӷ�������--------------------end


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)��������OpenGL��λ���ͼԪ��
    //��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ��棬
    //�ڶ������������������������ơ�
    //֮��Ļ��Ƶ��û�һֱ���߿�ģʽ���������Σ�
    //ֱ��������glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�������û�Ĭ��ģʽ��
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳���
    //����ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
    //glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ���
    //���´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
    //glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��
    //������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�


    Shader ourShader("../../../../shader/vert.shader", "../../../../shader/frag.shader");

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../../../../texture/container.jpg", &width, &height, &nrChannels, 0);//load
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//generate
        glGenerateMipmap(GL_TEXTURE_2D);//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);//free

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("../../../../texture/awesomeface.png", &width, &height, &nrChannels, 0);//load
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//generate
        glGenerateMipmap(GL_TEXTURE_2D);//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);//free

    ourShader.Use();
    ourShader.SetInt("texture1", 0);
    ourShader.SetInt("texture2", 1);

    float angles[10];
    for (int i = 0; i < 10; ++i)
    {
        angles[i] = rand() % 150 + 1.0f;
    }


    glEnable(GL_DEPTH_TEST);


    while (!windowMgr.CheckWindowShouldClose())
    {
        float currentFrame = (float)glfwGetTime();
        windowMgr.deltaTime = currentFrame - windowMgr.lastFrame;
        windowMgr.lastFrame = currentFrame;
        //����
        windowMgr.MonitorInputEvent();

        //��Ⱦ
        //glClearColor ���������Ļ���õ���ɫ
        //glClear �����Ļ����ɫ���壬������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ���
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDisable(GL_POLYGON_OFFSET_FILL);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        ourShader.Use();

        float timeValue = (float)glfwGetTime();
        ourShader.SetFloat("interpolation", sin(timeValue) / 2.0f + 1.0f);

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.SetMat4("view", view);
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)windowMgr.SCR_WIDTH / windowMgr.SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.SetMat4("proj", proj);
        /*glm::mat4 model;
        model = glm::translate(model, cubePositions[0]);
        model = glm::rotate(model, glm::radians(50 * timeValue), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.SetMat4("model", model);*/

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 m;
            m = glm::translate(m, cubePositions[i]);
            float angle = angles[i] * timeValue;
            m = glm::rotate(m, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.SetMat4("model", m);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glEnable(GL_POLYGON_OFFSET_FILL);
        //glPolygonOffset(-1, -1);

        //glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�
        //�ڶ�������ָ���˶����������ʼ����������������0
        //���һ������ָ�����Ǵ�����ƶ��ٸ�����

        //��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ����
        //�ڶ������������Ǵ�����ƶ���ĸ�����������6��Ҳ����˵����һ����Ҫ����6�����㡣
        //���������������������ͣ�������GL_UNSIGNED_INT��
        //���һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ��
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//������������Ⱦ
        //glBindVertexArray(0);//���VAO

        windowMgr.SwapBufferAndPollIOEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    return 0;
}