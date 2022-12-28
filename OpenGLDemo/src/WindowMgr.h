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
		glfwInit(); //��ʼ��GLFW

	//ʹ��glfwWindowHint����������GLFW
	//��һ����������ѡ������ƣ��ڶ�����������һ�����ͣ������������ѡ���ֵ
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//�������汾��
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//���ôΰ汾��

		//��ȷ����GLFW��Ҫʹ�ú���ģʽ��ζ��ֻ��ʹ��OpenGL���ܵ�һ���Ӽ���û�����������ԣ�
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//���ú���ģʽ
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		_window = nullptr;
		_camera = camera;
	}
	~WindowMgr()
	{
		//����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ��
	//������main������������glfwTerminate��������ɡ�
		// glfw: terminate, clearing all previously allocated GLFW resources.
		glfwTerminate();
		_window = nullptr;
	}


	bool CreateWindow(int w, int h)
	{
		//����һ�����ڶ���
	//glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ����������������������ʾ������ڵ�����
	//�����������������ʱ����,��󷵻�һ��GLFWwindow����
		_window = glfwCreateWindow(w, h, "LearnOpenGL", NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		//�����ڵ�����������Ϊ��ǰ�̵߳���������
		glfwMakeContextCurrent(_window);

		//��ʼ��GLAD
		//GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
		//��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����
		//GLFW�����ǵ���glfwGetProcAddress�����������Ǳ����ϵͳ��������ȷ�ĺ�����
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
		//��鲢�����¼�����������
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	bool CheckWindowShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	void SetFramebufferSizeCallback(GLFWframebuffersizefun callback)
	{
		//�������OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)��
	//����OpenGL��ֻ��֪���������ݴ��ڴ�С��ʾ���ݺ����ꡣ
	//����ͨ������glViewport���������ô��ڵ�ά��
	// ���Խ��ӿڵ�ά������Ϊ��GLFW��ά��С��������֮�����е�OpenGL��Ⱦ
	// ������һ����С�Ĵ�������ʾ��Ҳ���Խ�һЩ����Ԫ����ʾ��OpenGL�ӿ�֮�⡣

	//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�������
	//���ǿ��ԶԴ���ע��һ���ص�������������ÿ�δ��ڴ�С��������ʱ�򱻵���
	//֡�����С������Ҫһ��GLFWwindow��Ϊ���ĵ�һ���������Լ�����������ʾ���ڵ���ά�ȡ�
	//ÿ�����ڸı��С��GLFW�������������������Ӧ�Ĳ������㴦��
	//ע���������������GLFW����ϣ��ÿ�����ڵ�����С��ʱ��������������
		glfwSetFramebufferSizeCallback(_window, callback);
		//�����ڱ���һ����ʾ��ʱ��framebuffer_size_callbackҲ�ᱻ���á�
	//��������Ĥ(Retina)��ʾ����width��height�������Ա�ԭ����ֵ����һ�㡣
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
