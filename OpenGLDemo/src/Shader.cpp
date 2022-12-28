#include "Shader.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. 编译着色器
    unsigned int vertex, fragment;

    //----------------编译顶点着色器--------------------start
    //创建一个着色器对象，注意还是用ID来引用的。所以我们储存这个顶点着色器为unsigned int，
    //然后用glCreateShader创建这个着色器：
    //着色器类型:GL_VERTEX_SHADER表示顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    //把这个着色器源码附加到着色器对象上，然后编译它：
    //glShaderSource函数把要编译的着色器对象作为第一个参数。
    //第二参数指定了传递的源码字符串数量，这里只有一个。
    //第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
    glShaderSource(vertex, 1, &vShaderCode, NULL);//将源码附加到着色器对象
    glCompileShader(vertex);//编译着色器
    
    
    // 打印编译错误（如果有的话）
    //你可能会希望检测在调用glCompileShader后编译是否成功了，如果没成功的话，
    //你还会希望知道错误是什么，这样你才能修复它们。检测编译时错误可以通过以下代码来实现：
    CheckCompileErrors(ID, "VERTEX");
    //首先我们定义一个整型变量来表示是否成功编译，还定义了一个储存错误消息（如果有的话）的容器。
    //然后我们用glGetShaderiv检查是否编译成功。如果编译失败，
    //我们会用glGetShaderInfoLog获取错误消息，然后打印它。
    //如果编译的时候没有检测到任何错误，顶点着色器就被编译成功了。

    //----------------编译顶点着色器--------------------end

    // 片段着色器也类似
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // 打印编译错误（如果有的话）
    CheckCompileErrors(ID, "FRAGMENT");

    //----------------链接着色器--------------------start
    //着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
    //如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，
    //然后在渲染对象的时候激活这个着色器程序。已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
    // 着色器程序:glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
    ID = glCreateProgram();
    //现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们：
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // 打印连接错误（如果有的话）
    //就像着色器的编译一样，我们也可以检测链接着色器程序是否失败，并获取相应的日志。
    CheckCompileErrors(ID, "PROGRAM");

    //在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    //----------------链接着色器--------------------end
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::Use()
{
    //调用glUseProgram函数，激活程序对象
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);//??
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3f(const std::string& name, float r, float g, float b) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}

void Shader::SetVec4f(const std::string& name, float r, float g, float b, float a) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), r, g, b, a);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
