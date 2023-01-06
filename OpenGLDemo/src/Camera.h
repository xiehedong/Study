#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 定义相机移动的几个可能选项,用作抽象、远离特定于窗口系统的输入方法
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// 摄像机默认值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// 一个抽象的camera类，用于处理输入并计算相应的Euler角度、向量和矩阵，以便在OpenGL中使用
class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// 欧拉角euler Angles
	float Yaw;//偏航角
	float Pitch;//俯仰角
	// 相机选项camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera()
	{

	}

	// 向量构造
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// 标量构造
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(const Camera& camera)
	{
		Position = camera.Position;
		WorldUp = camera.WorldUp;
		Yaw = camera.Yaw;
		Pitch = camera.Pitch;
		updateCameraVectors();
	}


	// 返回使用欧拉角计算的视图矩阵
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// 处理从类似键盘的输入系统接收的输入。接受摄像机定义的枚举形式的输入参数（从窗口系统中提取）
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// 处理从鼠标输入系统接收的输入。需要x和y方向的偏移值。
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// 确保当俯仰角超出范围时，屏幕不会翻转
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// 使用更新的Euler角度更新前、右和上矢量
		updateCameraVectors();
	}

	// 处理从鼠标滚轮事件接收的输入。只需要在垂直轮轴上输入
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 100.0f)
			Zoom = 100.0f;
	}

private:
	// 根据相机的（更新的）Euler角度计算前矢量
	void updateCameraVectors()
	{
		// 计算新的前矢量
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  //规范化向量，因为上下看得越多，向量的长度就越接近0，这会导致移动速度变慢。
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif
