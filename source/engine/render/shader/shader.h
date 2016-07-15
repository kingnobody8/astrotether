#pragma once
#include "shader_program.h"

namespace engine
{
	namespace render
	{
#define REGISTER_SHADER(CLASS) engine::render::IShaderProgram::RegisterShader(#CLASS, CLASS::Type, util::CNewType<CLASS>())

		void RegisterShaders();
	}
}