//GLAD必须包含在GLFW之前，GLAD的头文件包含了正确的OpenGL头文件（例如GL/gl.h）
//所以需要在其它依赖于OpenGL的头文件之前包含GLAD。
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
    //前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
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
绘制1个图形需要
1、绑定VAO；【glGenVertexArrays、glBindVertexArray】
2、绑定VBO、保存顶点数据；【glGenBuffer、glBindBuffer、glBufferData】
3、绑定EBO(也叫IBO)、保存顶点索引数据【glGenBuffer、glBindBuffer、glBufferData】
4、编译着色器（顶点和片段）；【glCreateShader、glShaderSource、glCompileShader】
5、链接着色器；【glCreateProgram、glAttachShader、glLinkProgram、glUseProgram】
6、链接顶点属性；【glVertexAttribPointer、glEnableVertexAttribArray】
7、绘制；【glPolygonMode（可选）、glDrawElements/glDrawArrays】
*/

//float vertices[] = {
//    /*三角形*/
//    -0.5f,-0.5f,0.0f,
//    0.5f, -0.5f,0.0f,
//    0.0f,  0.5f,0.0f
//
//    ///*矩形*/
//    //0.5f, 0.5f, 0.0f,   // 右上角
//    //0.5f, -0.5f, 0.0f,  // 右下角
//    //-0.5f, -0.5f, 0.0f, // 左下角
//    //-0.5f, 0.5f, 0.0f   // 左上角
//};

//float vertices[] = {
//	// 位置              // 颜色
//	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
//	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
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

    //----------------绑定VAO--------------------start
    //顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，
    //任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，
    //当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了
    //以下设置的所有状态都将存储在VAO中(包括EBO)
    //一个VAO对象会储存以下这些内容：
    //glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
    //通过glVertexAttribPointer设置的顶点属性配置。
    //通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //OpenGL的核心模式要求我们使用VAO，所以它知道该如何处理我们的顶点输入。
    //如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。
    //----------------绑定VAO--------------------end

    //----------------保存顶点数据--------------------start
    //用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。OpenGL允许我们同时绑定多个缓冲，
    //只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
    //然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中：
    //glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
    //它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
    //第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
    //第三个参数是我们希望发送的实际数据。
    //第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
    //GL_STATIC_DRAW ：数据不会或几乎不会改变。
    //GL_DYNAMIC_DRAW：数据会被改变很多。
    //GL_STREAM_DRAW ：数据每次绘制时都会改变。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //----------------保存顶点数据--------------------end

    //----------------保存顶点索引数据--------------------start
    /*unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    //----------------保存顶点索引数据--------------------end



    //----------------链接顶点属性--------------------start
    //顶点缓冲数据会被解析为下面这样子：
    //位置数据被储存为32位（4字节）浮点值。
    //每个位置包含3个这样的值。
    //在这3个值之间没有空隙（或其他值）。这几个值在数组中紧密排列(Tightly Packed)。
    //数据中第一个值在缓冲开始的位置。
    //有了这些信息我们就可以使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了：
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glVertexAttribPointer函数的参数非常多：
    // 第一个参数指定要配置的顶点属性的位置值。
    // 第二个参数指定顶点属性的大小。
    // 第三个参数指定数据的类型，这里是GL_FLOAT
    // 第四个参数指定是否希望数据被标准化(Normalize)：如果我们设置为GL_TRUE，
    // 所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
    // 第五个参数叫做步长(Stride)，即连续的两个顶点属性之间的间隔。
    // 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。
    // 它表示位置数据在缓冲中起始位置的偏移量(Offset)
    // 
    // glEnableVertexAttribArray以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    // 
    // 
    //----------------链接顶点属性--------------------end


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)函数配置OpenGL如何绘制图元。
    //第一个参数表示我们打算将其应用到所有的三角形的正面和背面，
    //第二个参数告诉我们用线来绘制。
    //之后的绘制调用会一直以线框模式绘制三角形，
    //直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式。
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，
    //如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
    //glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、
    //更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
    //glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
    //它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。


    Shader ourShader("../../../shader/vert.shader", "../../../shader/frag.shader");

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../../../texture/container.jpg", &width, &height, &nrChannels, 0);//load
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//generate
        glGenerateMipmap(GL_TEXTURE_2D);//为当前绑定的纹理自动生成所有需要的多级渐远纹理
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);//free

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("../../../texture/awesomeface.png", &width, &height, &nrChannels, 0);//load
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//generate
        glGenerateMipmap(GL_TEXTURE_2D);//为当前绑定的纹理自动生成所有需要的多级渐远纹理
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
        //输入
        windowMgr.MonitorInputEvent();

        //渲染
        //glClearColor 设置清空屏幕所用的颜色
        //glClear 清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
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

        //glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型。
        //第二个参数指定了顶点数组的起始索引，我们这里填0
        //最后一个参数指定我们打算绘制多少个顶点

        //第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
        //第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
        //第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
        //最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候）
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//从索引缓冲渲染
        //glBindVertexArray(0);//解绑VAO

        windowMgr.SwapBufferAndPollIOEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    return 0;
}