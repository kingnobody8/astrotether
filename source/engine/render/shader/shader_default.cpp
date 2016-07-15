#include "shader_default.h"

namespace engine
{
	namespace render
	{
		DefaultShader::DefaultShader()
			: m_vertexPos2DLocation(-1)
			, m_vertexColorLocation(-1)
		{
		}

		VIRTUAL const bool DefaultShader::LoadProgram()
		{
			m_programID = glCreateProgram();



			//Get vertex source
			const std::string vertexShaderSource = \
				"attribute vec2 v_position;\n"
				"attribute vec2 v_coord;\n"
				"attribute vec4 v_color;\n"
				"varying vec4 f_color;\n"
				"void main()\n"
				"{\n"
				"	f_color = v_color;\n"
				"	gl_Position = vec4( v_position.x, v_position.y, 0, 1 );\n"
				"}\n";
			//Load vertex shader
			GLuint vertexShader = LoadShaderFromString(vertexShaderSource, GL_VERTEX_SHADER);
			//Check for errors
			if (vertexShader == 0)
			{
				glDeleteProgram(m_programID);
				m_programID = 0;
				return false;
			}
			//Attach vertex shader to program
			glAttachShader(m_programID, vertexShader);


			//Get fragment source
			const std::string fragmentShaderSource = \
				"precision highp float; \n"
				"varying vec4 f_color;\n"
				"void main()\n"
				"{\n"
				"	gl_FragColor = f_color;\n"
				"}\n";
			//Create fragment shader
			GLuint fragmentShader = LoadShaderFromString(fragmentShaderSource, GL_FRAGMENT_SHADER);
			//Check for errors
			if (fragmentShader == 0)
			{
				glDeleteShader(fragmentShader);
				glDeleteProgram(m_programID);
				m_programID = 0;
				return false;
			}
			//Attach fragment shader to program
			glAttachShader(m_programID, fragmentShader);



			//Link program
			glLinkProgram(m_programID);
			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(m_programID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				SDL_Log("Error linking program %d!\n", m_programID);
				PrintProgramLog(m_programID);
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
				glDeleteProgram(m_programID);
				m_programID = 0;
				return false;
			}



			//Clean up excess shader references
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);



			//Get variable locations
			m_vertexPos2DLocation = glGetAttribLocation(m_programID, "v_position");
			assert(m_vertexPos2DLocation != -1);
			m_vertexColorLocation = glGetAttribLocation(m_programID, "v_color");
			assert(m_vertexColorLocation != -1);

			return true;
		}

		void DefaultShader::EnableVertexPos2D()
		{
			glEnableVertexAttribArray(m_vertexPos2DLocation);
		}

		void DefaultShader::SetVertexPos2D(GLsizei stride, const GLvoid* data)
		{
			glVertexAttribPointer(m_vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
		}
		
		void DefaultShader::DisableVertexPos2D()
		{
			glDisableVertexAttribArray(m_vertexPos2DLocation);
		}
	
		void DefaultShader::EnableVertexColor()
		{
			glEnableVertexAttribArray(m_vertexColorLocation);
		}
		
		void DefaultShader::SetVertexColor(GLsizei stride, const GLvoid* data)
		{
			glVertexAttribPointer(m_vertexColorLocation, 4, GL_FLOAT, GL_FALSE, stride, data);
		}
		
		void DefaultShader::DisableVertexColor()
		{
			glDisableVertexAttribArray(m_vertexColorLocation);
		}
	}
}