//
// Created by iDste-PC on 2024-07-30.
//

#include "GLUtil.h"

#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <malloc.h>

#include "log.h"

GLuint loadShader(GLenum shaderType, const GLchar *source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled != GL_TRUE) {
        LOG_E("Compile shader failed. Shader type: %d", shaderType);
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char *errorLog = (char *) malloc(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
        LOG_E("Shader compilation error: %s", errorLog);
        free(errorLog);

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram(const char *vShaderSource, const char *fShaderSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vShaderSource);
    if (!vertexShader) {
        return 0;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderSource);
    if (!fragmentShader) {
        return 0;
    }
    return createProgram(vertexShader, fragmentShader);
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, fragmentShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

        glDetachShader(program, vertexShader);
        glDeleteShader(vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(fragmentShader);
        if (isLinked != GL_TRUE) {
            LOG_E("Link program failed.");
            GLint maxLength = 0;
            glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            char *errorLog = (char *) malloc(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);
            LOG_E("Program link error: %s", errorLog);
            free(errorLog);

            glDeleteProgram(program);
            return 0;
        }
    }
    return program;
}

void deleteProgram(GLuint &program) {
    if (program) {
        glUseProgram(0);
        glDeleteProgram(program);
        program = 0;
    }
}

void checkGlError(const char *pGLOperation) {
    GLenum error = glGetError();
    while (error) {
        LOG_E("CheckGLError GL Operation %s() glError (0x%x)\n", pGLOperation, error);
        error = glGetError();
    }
}
