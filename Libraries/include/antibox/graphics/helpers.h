#pragma once

#include "../core/log.h"

#include "glad/glad.h"
#include <string>

namespace antibox
{
	void CheckGLError();
}


#define ANTIBOX_CHECK_GL_ERROR CheckGLError();