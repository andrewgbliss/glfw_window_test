#pragma once

#include "RenderDriver.h"
#include "../window/Window.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>

class OpenGLRenderDriver : public RenderDriver
{
private:
  GLFWwindow *m_window;
  bool m_initialized;

public:
  OpenGLRenderDriver() : m_window(nullptr), m_initialized(false) {}

  virtual ~OpenGLRenderDriver()
  {
    cleanup();
  }

  // Implementation of RenderDriver interface
  bool initialize(Window *window) override
  {
    if (m_initialized)
    {
      return true;
    }

    if (!window)
    {
      std::cerr << "OpenGLRenderDriver: Invalid window pointer" << std::endl;
      return false;
    }

    // Get the GLFW window handle from our Window class
    m_window = static_cast<GLFWwindow *>(window->getNativeWindow());
    if (!m_window)
    {
      std::cerr << "OpenGLRenderDriver: Failed to get GLFW window handle" << std::endl;
      return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    // Check if OpenGL is working
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
      std::cerr << "OpenGLRenderDriver: OpenGL error after context setup: " << error << std::endl;
      return false;
    }

    m_initialized = true;
    std::cout << "OpenGLRenderDriver: Initialized successfully" << std::endl;
    return true;
  }

  void cleanup() override
  {
    if (m_initialized)
    {
      m_initialized = false;
      m_window = nullptr;
      std::cout << "OpenGLRenderDriver: Cleaned up" << std::endl;
    }
  }

  void setup2DRendering(int viewportWidth, int viewportHeight) override
  {
    // Get the actual window size for the viewport (to fill the entire window)
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);

    // Set up viewport to fill the entire window
    glViewport(0, 0, windowWidth, windowHeight);

    // Set up orthographic projection for 2D rendering
    // Use the logical viewport dimensions for the projection
    // Flip Y-axis so (0,0) is at top-left
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewportWidth, viewportHeight, 0, -1, 1);

    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth testing for 2D
    glDisable(GL_DEPTH_TEST);
  }

  void clear(float r, float g, float b, float a) override
  {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void setTransform(float x, float y, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f) override
  {
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX, scaleY, 1.0f);
  }

  void resetTransform() override
  {
    glLoadIdentity();
  }

  void setColor(float r, float g, float b, float a = 1.0f) override
  {
    glColor4f(r, g, b, a);
  }

  void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) override
  {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
  }

  void drawRectangle(float x, float y, float width, float height) override
  {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
  }

  void drawSprite(float x, float y, float width, float height, unsigned int textureId, float texLeft = 0.0f, float texTop = 0.0f, float texRight = 1.0f, float texBottom = 1.0f) override
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x, y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + width, y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + width, y + height);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
  }

  unsigned int createTexture() override
  {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    return textureId;
  }

  void deleteTexture(unsigned int textureId) override
  {
    glDeleteTextures(1, &textureId);
  }

  void uploadTexture(unsigned int textureId, int width, int height, const void *data, bool useLinearFiltering = true) override
  {
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    int filterMode = useLinearFiltering ? GL_LINEAR : GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }

  std::string getDriverName() const override
  {
    return "OpenGL";
  }

  std::string getVersion() const override
  {
    return "1.0";
  }

  // Window management (rendering-related)
  void swapBuffers() override
  {
    glfwSwapBuffers(m_window);
  }

  void pollEvents() override
  {
    glfwPollEvents();
  }

  bool shouldClose() const override
  {
    return glfwWindowShouldClose(m_window);
  }
};
