#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Camera.h"


class WindowMgr
{
private:
	GLFWwindow* _window;
	Camera* _camera;
public:
	static const unsigned int SCR_WIDTH = 800;
	static const unsigned int SCR_HEIGHT = 600;
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	GLFWwindow* GetWindow() { return _window; }

	WindowMgr(Camera *camera)
	{
		glfwInit(); //初始化GLFW

	//使用glfwWindowHint函数来配置GLFW
	//第一个参数代表选项的名称，第二个参数接受一个整型，用来设置这个选项的值
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//设置主版本号
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//设置次版本号

		//明确告诉GLFW需要使用核心模式意味着只能使用OpenGL功能的一个子集（没有向后兼容特性）
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//设置核心模式
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		_window = nullptr;
		_camera = camera;
	}
	~WindowMgr()
	{
		//当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。
	//可以在main函数的最后调用glfwTerminate函数来完成。
		// glfw: terminate, clearing all previously allocated GLFW resources.
		glfwTerminate();
		_window = nullptr;
	}


	bool CreateWindow(int w, int h)
	{
		//创建一个窗口对象
	//glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称
	//最后两个参数我们暂时忽略,最后返回一个GLFWwindow对象
		_window = glfwCreateWindow(w, h, "LearnOpenGL", NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		//将窗口的上下文设置为当前线程的主上下文
		glfwMakeContextCurrent(_window);

		//初始化GLAD
		//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
		//给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
		//GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		return true;
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	void MonitorInputEvent()
	{
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(_window, true);
		if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
			_camera->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
			_camera->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
			_camera->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
			_camera->ProcessKeyboard(RIGHT, deltaTime);
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	void SwapBufferAndPollIOEvents()
	{
		//检查并调用事件，交换缓冲
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	bool CheckWindowShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	void SetFramebufferSizeCallback(GLFWframebuffersizefun callback)
	{
		//必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，
	//这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。
	//可以通过调用glViewport函数来设置窗口的维度
	// 可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染
	// 将会在一个更小的窗口中显示，也可以将一些其它元素显示在OpenGL视口之外。

	//当用户改变窗口的大小的时候，视口也应该被调整。
	//我们可以对窗口注册一个回调函数，它会在每次窗口大小被调整的时候被调用
	//帧缓冲大小函数需要一个GLFWwindow作为它的第一个参数，以及两个整数表示窗口的新维度。
	//每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理。
	//注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
		glfwSetFramebufferSizeCallback(_window, callback);
		//当窗口被第一次显示的时候framebuffer_size_callback也会被调用。
	//对于视网膜(Retina)显示屏，width和height都会明显比原输入值更高一点。
	}

	void SetSetCursorPosCallback(GLFWcursorposfun callback)
	{
		glfwSetCursorPosCallback(_window, callback);
	}
	void SetScrollCallback(GLFWscrollfun callback)
	{
		glfwSetScrollCallback(_window, callback);
	}
};
