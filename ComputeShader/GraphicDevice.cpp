#include "GraphicDevice.h"
#include "Util.h"
#include "Math.h"

void OutputShaderCompileError(GLuint shader)
{
	GLchar lg[10240];
	GLsizei len;

	glGetShaderInfoLog(shader, 10239, &len, lg);
	Log::Info("Error Compiling Shader");
	Log::Info(lg);
	int actual_length = 0;
}

void OutputShaderLinkError(GLuint program)
{
	GLchar lg[10240];
	GLsizei len;

	glGetProgramInfoLog(program, 10239, &len, lg);
	Log::Info("Error Linking Program");
	Log::Info(lg);
}

#define HASERROR() HasError(__LINE__, __FILE__)

inline bool HasError(int line, const char* file)
{
	GLenum err;
	err = glGetError();
	bool rtn = false;
	while (err != GL_NO_ERROR)
	{
		Log::Info("Line: " + std::to_string(line) + " " + file);
		switch (err)
		{
		case GL_INVALID_ENUM:
			Log::Info("An unacceptable value is specified for an enumerated argument");
			break;
		case GL_INVALID_VALUE:
			Log::Info("A numeric argument is out of range");
			break;
		case GL_INVALID_OPERATION:
			Log::Info("The specified operation is not allowed in the current state");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			Log::Info("The framebuffer object is not complete");
			break;
		case GL_OUT_OF_MEMORY:
			Log::Info("There is not enough memory left to execute the command");
			break;
		case GL_STACK_UNDERFLOW:
			Log::Info("An attempt has been made to perform an operation that would cause an internal stack to underflow");
			break;
		case GL_STACK_OVERFLOW:
			Log::Info("An attempt has been made to perform an operation that would cause an internal stack to overflow");
			break;

		}
		err = glGetError();
		rtn = true;
	}

	//Assert(rtn == false);
	return rtn;
}

bool GraphicDevice::Initialize(Window * window)
{
	m_deviceContext = GetDC(window->GetHandle());
	m_renderingContext = 0;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;


	int pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);

	if (pixelFormat == 0)
	{
		return false;
	}

	if (!SetPixelFormat(m_deviceContext, pixelFormat, &pfd))
	{
		return false;
	}

	HGLRC tempContext = wglCreateContext(m_deviceContext);

	if (!wglMakeCurrent(m_deviceContext, tempContext))
	{
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(m_deviceContext, m_renderingContext); 
	}
	else
	{       //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		m_renderingContext = tempContext;
		return false;
	}

	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	if (!m_renderingContext)
	{
		return false;
	}

	// Set Depth Testing for painter algorithm
	// I am probably going to want to draw the object in reverse order at some point
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Alpha Blending operation
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Log::Info("OpenGL initalized");
	Log::Print();

	return true;
}

GLuint GraphicDevice::CreateComputeShader(const std::string src)
{
	GLuint program;
	GLuint cs;
	int rVal;
	const char* c_src = src.c_str();

	program = glCreateProgram();

	cs = glCreateShader(GL_COMPUTE_SHADER);

	glShaderSource(cs, 1, &c_src, NULL);
	glCompileShader(cs);

	glGetShaderiv(cs, GL_COMPILE_STATUS, &rVal);
	if (!rVal)
	{
		OutputShaderCompileError(cs);
		Log::Error("File " + src + " not a valid shader");
		return 0;
	}

	glAttachShader(program, cs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &rVal);
	if (rVal != GL_TRUE)
	{
		OutputShaderLinkError(program);
		Log::Error("File " + src + " not a valid shader");

		return 0;
	}

	glDeleteShader(cs);

	HASERROR();

	return program;
}

Shader* GraphicDevice::CreateShader(const std::string vs, const std::string fs)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	int rVal;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Create and compile the vertex shader
	const char * vscr = vs.c_str();

	glShaderSource(vertexShader, 1, &vscr, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &rVal);
	if (rVal != GL_TRUE)
	{
		Log::Info("Error with shader " + vs);
		OutputShaderCompileError(vertexShader);
	}

	// Create and compile the fragment shader
	const char* fscr = fs.c_str();

	glShaderSource(fragmentShader, 1, &fscr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &rVal);
	if (rVal != GL_TRUE)
	{
		Log::Info("Error with shader " + vs);
		OutputShaderCompileError(fragmentShader);
	}

	// Create and link the program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glBindAttribLocation(program, 0, "inputPosition");

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &rVal);
	if (rVal != GL_TRUE)
	{
		OutputShaderLinkError(program);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	HASERROR();

	return new Shader(program);
}

VertexBuffer GraphicDevice::CreateVertexBuffer(BufferData * data)
{
	GLuint vertexArray;
	GLuint vb;

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, data->GetBufferSize(), data->GetData(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	HASERROR();

	glBindBuffer(GL_ARRAY_BUFFER, vb);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, data->GetElementSize() / sizeof(float), GL_FLOAT, false, data->GetElementSize(), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	HASERROR();

	return VertexBuffer(vertexArray, vb, data->GetNumElements(), data->GetElementSize());
}

Buffer GraphicDevice::CreateIndexBuffer(BufferData * data)
{
	GLuint ib;

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->GetBufferSize(), data->GetData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	HASERROR();

	return Buffer(ib, data->GetNumElements(), data->GetElementSize());
}

Buffer GraphicDevice::CreateSBO(BufferData* data)
{
	GLuint buf;

	glGenBuffers(1, &buf);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, data->GetBufferSize(), data->GetData(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	HASERROR();
	
	return Buffer(buf, data->GetNumElements(), data->GetElementSize());
}

void GraphicDevice::Delete(Buffer buffer)
{
	unsigned int buf = buffer.GetBuffer();
	glDeleteBuffers(1, &buf);
}

void GraphicDevice::Delete(Shader* shader)
{
	if(shader)
		glDeleteProgram(shader->GetShader());
}

void* GraphicDevice::GetBufferData(Buffer bufferData)
{
	void* ptr;
	void* data;
	data = malloc(bufferData.GetBufferSize());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferData.GetBuffer());
	
	ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	HASERROR();

	memcpy(data, ptr, bufferData.GetBufferSize());

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return data;
}

void GraphicDevice::Render(Camera* camera, RenderObject* renderObject)
{
	glUseProgram(renderObject->GetShader()->GetShader());
	HASERROR();

	// Set the camera data
	SetUniformMatrix(renderObject->GetShader(), "ViewMatrix", camera->GetViewMatrix());
	SetUniformMatrix(renderObject->GetShader(), "ProjectionMatrix", camera->GetProjectionMatrix());


	glBindVertexArray(renderObject->GetVertexBuffer().GetVertexArray());
	HASERROR();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderObject->GetIndexBuffer().GetBuffer());

	HASERROR();

	glDrawElements(GL_TRIANGLES, renderObject->GetElementCount(), GL_UNSIGNED_INT, 0);
	HASERROR();
}

void GraphicDevice::BeginScene(float red, float green, float blue)
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicDevice::EndScene()
{
	SwapBuffers(m_deviceContext);
}

void GraphicDevice::DispatchShader(ComputeShader* shader)
{
	int x, y, z;
	int xx, yy, zz;
	glUseProgram(shader->GetShader());

	auto buffers = shader->GetBuffers();

	for (unsigned int i = 0; i < buffers.size(); ++i)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, buffers[i].GetBuffer());
	}

	HASERROR();

	shader->GetComputeSize(x, y, z);
	shader->GetGroupSize(xx, yy, zz);

	glDispatchComputeGroupSizeARB(x, y, z, xx, yy, zz);
	//glDispatchCompute(x, y, z);

	HASERROR();

	for (unsigned int i = 0; i < buffers.size(); ++i)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, 0);
	}

	HASERROR();

	glUseProgram(0);
}

void GraphicDevice::SetUniformFloat(Shader* shader, std::string param, float * data, unsigned int size)
{
	GLint loc;

	glUseProgram(shader->GetShader());

	loc = glGetUniformLocation(shader->GetShader(), param.c_str());
	if (loc == -1)
	{
		Log::Error("Param: " + param + " does not exist in shader");
		return;
	}

	HASERROR();

	switch (size)
	{
	case 1:
		glUniform1f(loc, data[0]);
		break;
	case 2:
		glUniform2f(loc, data[0], data[1]);
		break;
	case 3:
		glUniform3f(loc, data[0], data[1], data[2]);
		break;
	case 4:
		glUniform4f(loc, data[0], data[1], data[2], data[3]);
		break;
	default:
		break;
	}

	HASERROR();
}

void GraphicDevice::SetUniformMatrix(Shader * shader, std::string param, Matrix4 m)
{
	//glUseProgram(shader->GetShader());

	GLuint loc = glGetUniformLocation(shader->GetShader(), param.c_str());
	if (loc == -1)
	{
		Log::Error("Param: " + param + " does not exist in shader");
		return;
	}

	glUniformMatrix4fv(loc, 1, false, m);
}
