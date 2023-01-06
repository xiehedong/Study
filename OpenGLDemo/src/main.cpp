//GLAD蹇呴』鍖呭惈鍦℅LFW涔嬪墠锛孏LAD鐨勫ご鏂囦欢鍖呭惈浜嗘纭殑OpenGL澶存枃浠讹紙渚嬪GL/gl.h锛�
//鎵€浠ラ渶瑕佸湪鍏跺畠渚濊禆浜嶰penGL鐨勫ご鏂囦欢涔嬪墠鍖呭惈GLAD銆�
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
    //前锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟狡达拷锟斤拷锟斤拷锟铰角碉拷位锟矫★拷锟斤拷锟斤拷锟斤拷锟酵碉拷锟侥革拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷染锟斤拷锟节的匡拷锟饺和高度ｏ拷锟斤拷锟截ｏ拷锟斤拷
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
锟斤拷锟斤拷1锟斤拷图锟斤拷锟斤拷要
1锟斤拷锟斤拷VAO锟斤拷锟斤拷glGenVertexArrays锟斤拷glBindVertexArray锟斤拷
2锟斤拷锟斤拷VBO锟斤拷锟斤拷锟芥顶锟斤拷锟斤拷锟捷ｏ拷锟斤拷glGenBuffer锟斤拷glBindBuffer锟斤拷glBufferData锟斤拷
3锟斤拷锟斤拷EBO(也锟斤拷IBO)锟斤拷锟斤拷锟芥顶锟斤拷锟斤拷锟斤拷锟斤拷锟捷★拷glGenBuffer锟斤拷glBindBuffer锟斤拷glBufferData锟斤拷
4锟斤拷锟斤拷锟斤拷锟斤拷色锟斤拷锟斤拷锟斤拷锟斤拷锟狡拷危锟斤拷锟斤拷锟絞lCreateShader锟斤拷glShaderSource锟斤拷glCompileShader锟斤拷
5锟斤拷锟斤拷锟斤拷锟斤拷色锟斤拷锟斤拷锟斤拷glCreateProgram锟斤拷glAttachShader锟斤拷glLinkProgram锟斤拷glUseProgram锟斤拷
6锟斤拷锟斤拷锟接讹拷锟斤拷锟斤拷锟皆ｏ拷锟斤拷glVertexAttribPointer锟斤拷glEnableVertexAttribArray锟斤拷
7锟斤拷锟斤拷锟狡ｏ拷锟斤拷glPolygonMode锟斤拷锟斤拷选锟斤拷锟斤拷glDrawElements/glDrawArrays锟斤拷
*/

//float vertices[] = {
//    /*锟斤拷锟斤拷锟斤拷*/
//    -0.5f,-0.5f,0.0f,
//    0.5f, -0.5f,0.0f,
//    0.0f,  0.5f,0.0f
//
//    ///*锟斤拷锟斤拷*/
//    //0.5f, 0.5f, 0.0f,   // 锟斤拷锟较斤拷
//    //0.5f, -0.5f, 0.0f,  // 锟斤拷锟铰斤拷
//    //-0.5f, -0.5f, 0.0f, // 锟斤拷锟铰斤拷
//    //-0.5f, 0.5f, 0.0f   // 锟斤拷锟较斤拷
//};

//float vertices[] = {
//	// 位锟斤拷              // 锟斤拷色
//	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 锟斤拷锟斤拷
//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 锟斤拷锟斤拷
//	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 锟斤拷锟斤拷
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

    //----------------锟斤拷VAO--------------------start
    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�(Vertex Array Object, VAO)锟斤拷锟斤拷锟今顶点缓锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷蠖ǎ锟�
    //锟轿猴拷锟斤拷锟侥讹拷锟斤拷锟斤拷锟皆碉拷锟矫讹拷锟结储锟斤拷锟斤拷锟斤拷锟絍AO锟叫★拷锟斤拷锟斤拷锟侥好达拷锟斤拷锟角ｏ拷
    //锟斤拷锟斤拷锟矫讹拷锟斤拷锟斤拷锟斤拷指锟斤拷时锟斤拷锟斤拷只锟斤拷要锟斤拷锟斤拷些锟斤拷锟斤拷执锟斤拷一锟轿ｏ拷之锟斤拷锟劫伙拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟街伙拷锟揭拷锟斤拷锟接︼拷锟絍AO锟斤拷锟斤拷锟斤拷
    //锟斤拷锟斤拷锟斤拷锟矫碉拷锟斤拷锟斤拷状态锟斤拷锟斤拷锟芥储锟斤拷VAO锟斤拷(锟斤拷锟斤拷EBO)
    //一锟斤拷VAO锟斤拷锟斤拷岽拷锟斤拷锟斤拷锟斤拷锟叫╋拷锟斤拷荩锟�
    //glEnableVertexAttribArray锟斤拷glDisableVertexAttribArray锟侥碉拷锟矫★拷
    //通锟斤拷glVertexAttribPointer锟斤拷锟矫的讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟矫★拷
    //通锟斤拷glVertexAttribPointer锟斤拷锟斤拷锟诫顶锟斤拷锟斤拷锟皆癸拷锟斤拷锟侥讹拷锟姐缓锟斤拷锟斤拷锟�
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //OpenGL锟侥猴拷锟斤拷模式要锟斤拷锟斤拷锟斤拷使锟斤拷VAO锟斤拷锟斤拷锟斤拷锟斤拷知锟斤拷锟斤拷锟斤拷未锟斤拷锟斤拷锟斤拷堑亩锟斤拷锟斤拷锟斤拷搿�
    //锟斤拷锟斤拷锟斤拷前锟絍AO失锟杰ｏ拷OpenGL锟斤拷芫锟斤拷锟斤拷锟斤拷魏味锟斤拷锟斤拷锟�
    //----------------锟斤拷VAO--------------------end

    //----------------锟斤拷锟芥顶锟斤拷锟斤拷锟斤拷--------------------start
    //锟斤拷glGenBuffers锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷ID锟斤拷锟斤拷一锟斤拷VBO锟斤拷锟斤拷
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //OpenGL锟叫很多缓锟斤拷锟斤拷锟斤拷锟斤拷停锟斤拷锟斤拷慊猴拷锟斤拷锟斤拷幕锟斤拷锟斤拷锟斤拷锟斤拷锟紾L_ARRAY_BUFFER锟斤拷OpenGL锟斤拷锟斤拷锟斤拷锟斤拷同时锟襟定讹拷锟斤拷锟斤拷澹�
    //只要锟斤拷锟斤拷锟角诧拷同锟侥伙拷锟斤拷锟斤拷锟酵★拷锟斤拷锟角匡拷锟斤拷使锟斤拷glBindBuffer锟斤拷锟斤拷锟斤拷锟铰达拷锟斤拷锟侥伙拷锟斤拷蠖ǖ锟紾L_ARRAY_BUFFER目锟斤拷锟较ｏ拷
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷使锟矫碉拷锟轿何ｏ拷锟斤拷GL_ARRAY_BUFFER目锟斤拷锟较的ｏ拷锟斤拷锟斤拷锟斤拷枚锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷玫锟角帮拷蠖ǖ幕锟斤拷锟�(VBO)锟斤拷
    //然锟斤拷锟斤拷锟角匡拷锟皆碉拷锟斤拷glBufferData锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟街帮拷锟斤拷锟侥讹拷锟斤拷锟斤拷锟捷革拷锟狡碉拷锟斤拷锟斤拷锟斤拷诖锟斤拷校锟�
    //glBufferData锟斤拷一锟斤拷专锟斤拷锟斤拷锟斤拷锟斤拷锟矫伙拷锟斤拷锟斤拷锟斤拷锟斤拷莞锟斤拷频锟斤拷锟角帮拷蠖ɑ锟斤拷锟侥猴拷锟斤拷锟斤拷
    //锟斤拷锟侥碉拷一锟斤拷锟斤拷锟斤拷锟斤拷目锟疥缓锟斤拷锟斤拷锟斤拷停锟斤拷锟斤拷慊猴拷锟斤拷锟斤拷前锟襟定碉拷GL_ARRAY_BUFFER目锟斤拷锟较★拷
    //锟节讹拷锟斤拷锟斤拷锟斤拷指锟斤拷锟斤拷锟斤拷锟斤拷锟捷的达拷小(锟斤拷锟街斤拷为锟斤拷位)锟斤拷锟斤拷一锟斤拷锟津单碉拷sizeof锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷荽锟叫★拷锟斤拷小锟�
    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷希锟斤拷锟斤拷锟酵碉拷实锟斤拷锟斤拷锟捷★拷
    //锟斤拷锟侥革拷锟斤拷锟斤拷指锟斤拷锟斤拷锟斤拷锟斤拷希锟斤拷锟皆匡拷锟斤拷喂锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷荨锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞斤拷锟�
    //GL_STATIC_DRAW 锟斤拷锟斤拷锟捷诧拷锟斤拷蚣负锟斤拷锟斤拷锟侥变。
    //GL_DYNAMIC_DRAW锟斤拷锟斤拷锟捷会被锟侥憋拷芏唷�
    //GL_STREAM_DRAW 锟斤拷锟斤拷锟斤拷每锟轿伙拷锟斤拷时锟斤拷锟斤拷谋洹�
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //----------------锟斤拷锟芥顶锟斤拷锟斤拷锟斤拷--------------------end

    //----------------锟斤拷锟芥顶锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷--------------------start
    /*unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    //----------------锟斤拷锟芥顶锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷--------------------end



    //----------------锟斤拷锟接讹拷锟斤拷锟斤拷锟斤拷--------------------start
    //锟斤拷锟姐缓锟斤拷锟斤拷锟捷会被锟斤拷锟斤拷为锟斤拷锟斤拷锟斤拷锟斤拷锟接ｏ拷
    //位锟斤拷锟斤拷锟捷憋拷锟斤拷锟斤拷为32位锟斤拷4锟街节ｏ拷锟斤拷锟斤拷值锟斤拷
    //每锟斤拷位锟矫帮拷锟斤拷3锟斤拷锟斤拷锟斤拷锟斤拷值锟斤拷
    //锟斤拷锟斤拷3锟斤拷值之锟斤拷没锟叫匡拷隙锟斤拷锟斤拷锟斤拷锟斤拷值锟斤拷锟斤拷锟解几锟斤拷值锟斤拷锟斤拷锟斤拷锟叫斤拷锟斤拷锟斤拷锟斤拷(Tightly Packed)锟斤拷
    //锟斤拷锟斤拷锟叫碉拷一锟斤拷值锟节伙拷锟藉开始锟斤拷位锟矫★拷
    //锟斤拷锟斤拷锟斤拷些锟斤拷息锟斤拷锟角就匡拷锟斤拷使锟斤拷glVertexAttribPointer锟斤拷锟斤拷锟斤拷锟斤拷OpenGL锟斤拷锟斤拷谓锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷荩锟接︼拷玫锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟较ｏ拷锟剿ｏ拷
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glVertexAttribPointer锟斤拷锟斤拷锟侥诧拷锟斤拷锟角筹拷锟洁：
    // 锟斤拷一锟斤拷锟斤拷锟斤拷指锟斤拷要锟斤拷锟矫的讹拷锟斤拷锟斤拷锟皆碉拷位锟斤拷值锟斤拷
    // 锟节讹拷锟斤拷锟斤拷锟斤拷指锟斤拷锟斤拷锟斤拷锟斤拷锟皆的达拷小锟斤拷
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷指锟斤拷锟斤拷锟捷碉拷锟斤拷锟酵ｏ拷锟斤拷锟斤拷锟斤拷GL_FLOAT
    // 锟斤拷锟侥革拷锟斤拷锟斤拷指锟斤拷锟角凤拷希锟斤拷锟斤拷锟捷憋拷锟斤拷准锟斤拷(Normalize)锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟轿狦L_TRUE锟斤拷
    // 锟斤拷锟斤拷锟斤拷锟捷讹拷锟结被映锟戒到0锟斤拷锟斤拷锟斤拷锟叫凤拷锟斤拷锟斤拷signed锟斤拷锟斤拷锟斤拷-1锟斤拷锟斤拷1之锟斤拷
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�(Stride)锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷之锟斤拷募锟斤拷锟斤拷
    // 锟斤拷锟揭伙拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟絭oid*锟斤拷锟斤拷锟斤拷锟斤拷要锟斤拷锟角斤拷锟斤拷锟斤拷锟斤拷锟街碉拷强锟斤拷锟斤拷锟斤拷转锟斤拷锟斤拷
    // 锟斤拷锟斤拷示位锟斤拷锟斤拷锟斤拷锟节伙拷锟斤拷锟斤拷锟斤拷始位锟矫碉拷偏锟斤拷锟斤拷(Offset)
    // 
    // glEnableVertexAttribArray锟皆讹拷锟斤拷锟斤拷锟斤拷位锟斤拷值锟斤拷为锟斤拷锟斤拷锟斤拷锟斤拷锟矫讹拷锟斤拷锟斤拷锟皆ｏ拷锟斤拷锟斤拷锟斤拷锟斤拷默锟斤拷锟角斤拷锟矫碉拷
    // 
    // 
    //----------------锟斤拷锟接讹拷锟斤拷锟斤拷锟斤拷--------------------end


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)锟斤拷锟斤拷锟斤拷锟斤拷OpenGL锟斤拷位锟斤拷锟酵荚拷锟�
    //锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷示锟斤拷锟角达拷锟姐将锟斤拷应锟矫碉拷锟斤拷锟叫碉拷锟斤拷锟斤拷锟轿碉拷锟斤拷锟斤拷捅锟斤拷妫�
    //锟节讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟狡★拷
    //之锟斤拷幕锟斤拷频锟斤拷没锟揭恢憋拷锟斤拷呖锟侥Ｊ斤拷锟斤拷锟斤拷锟斤拷锟斤拷危锟�
    //直锟斤拷锟斤拷锟斤拷锟斤拷glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)锟斤拷锟斤拷锟斤拷锟矫伙拷默锟斤拷模式锟斤拷
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //glfwWindowShouldClose锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷每锟斤拷循锟斤拷锟侥匡拷始前锟斤拷锟揭伙拷锟紾LFW锟角凤拷要锟斤拷锟剿筹拷锟斤拷
    //锟斤拷锟斤拷堑幕锟斤拷煤锟斤拷锟斤拷锟斤拷锟絫rue然锟斤拷锟斤拷染循锟斤拷锟斤拷锟斤拷锟斤拷耍锟街拷锟轿拷锟斤拷蔷涂锟斤拷怨乇锟接︼拷贸锟斤拷锟斤拷恕锟�
    //glfwPollEvents锟斤拷锟斤拷锟斤拷锟斤拷锟矫伙拷写锟斤拷锟绞裁达拷录锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟诫、锟斤拷锟斤拷贫锟斤拷龋锟斤拷锟�
    //锟斤拷锟铰达拷锟斤拷状态锟斤拷锟斤拷锟斤拷锟矫讹拷应锟侥回碉拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷通锟斤拷锟截碉拷锟斤拷锟斤拷锟街讹拷锟斤拷锟矫ｏ拷锟斤拷
    //glfwSwapBuffers锟斤拷锟斤拷锟结交锟斤拷锟斤拷色锟斤拷锟藉（锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷GLFW锟斤拷锟斤拷每一锟斤拷锟斤拷锟斤拷锟斤拷色值锟侥大缓冲）锟斤拷
    //锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟叫憋拷锟斤拷锟斤拷锟斤拷锟狡ｏ拷锟斤拷锟揭斤拷锟斤拷锟斤拷为锟斤拷锟斤拷锟绞撅拷锟斤拷锟侥伙拷稀锟�


    Shader ourShader("../../../../shader/vert.shader", "../../../../shader/frag.shader");

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为锟斤拷前锟襟定碉拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟矫伙拷锟狡★拷锟斤拷锟剿凤拷式
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
        glGenerateMipmap(GL_TEXTURE_2D);//为锟斤拷前锟襟定碉拷锟斤拷锟斤拷锟皆讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷要锟侥多级锟斤拷远锟斤拷锟斤拷
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);//free

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 为锟斤拷前锟襟定碉拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟矫伙拷锟狡★拷锟斤拷锟剿凤拷式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("../../../../texture/awesomeface.png", &width, &height, &nrChannels, 0);//load
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//generate
        glGenerateMipmap(GL_TEXTURE_2D);//为锟斤拷前锟襟定碉拷锟斤拷锟斤拷锟皆讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷要锟侥多级锟斤拷远锟斤拷锟斤拷
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
        //锟斤拷锟斤拷
        windowMgr.MonitorInputEvent();

        //锟斤拷染
        //glClearColor 锟斤拷锟斤拷锟斤拷锟斤拷锟侥伙拷锟斤拷玫锟斤拷锟缴�
        //glClear 锟斤拷锟斤拷锟侥伙拷锟斤拷锟缴拷锟斤拷澹拷锟斤拷锟斤拷锟揭伙拷锟斤拷锟斤拷锟轿�(Buffer Bit)锟斤拷指锟斤拷要锟斤拷盏幕锟斤拷锟�
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

        //glDrawArrays锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟角达拷锟斤拷锟斤拷频锟絆penGL图元锟斤拷锟斤拷锟酵★拷
        //锟节讹拷锟斤拷锟斤拷锟斤拷指锟斤拷锟剿讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞硷拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�0
        //锟斤拷锟揭伙拷锟斤拷锟斤拷锟街革拷锟斤拷锟斤拷谴锟斤拷锟斤拷锟狡讹拷锟劫革拷锟斤拷锟斤拷

        //锟斤拷一锟斤拷锟斤拷锟斤拷指锟斤拷锟斤拷锟斤拷锟角伙拷锟狡碉拷模式锟斤拷锟斤拷锟斤拷锟絞lDrawArrays锟斤拷一锟斤拷锟斤拷
        //锟节讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟角达拷锟斤拷锟斤拷贫锟斤拷锟侥革拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷6锟斤拷也锟斤拷锟斤拷说锟斤拷锟斤拷一锟斤拷锟斤拷要锟斤拷锟斤拷6锟斤拷锟斤拷锟姐。
        //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟酵ｏ拷锟斤拷锟斤拷锟斤拷GL_UNSIGNED_INT锟斤拷
        //锟斤拷锟揭伙拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷强锟斤拷锟街革拷锟紼BO锟叫碉拷偏锟斤拷锟斤拷锟斤拷锟斤拷锟竭达拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷锟介，锟斤拷锟斤拷锟斤拷锟角碉拷锟姐不锟斤拷使锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷时锟斤拷
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷染
        //glBindVertexArray(0);//锟斤拷锟絍AO

        windowMgr.SwapBufferAndPollIOEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    return 0;
}