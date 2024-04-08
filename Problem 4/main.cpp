#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

int main() {
  if ( !glfwInit() ) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  GLFWwindow* window = glfwCreateWindow( 500, 500, "Sine Curve using OpenGL with GLEW and GLFW", NULL, NULL );
  if ( !window ) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent( window );

  if ( glewInit() != GLEW_OK ) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

  while ( !glfwWindowShouldClose( window ) ) {
    glClear( GL_COLOR_BUFFER_BIT );

    glBegin( GL_LINES );
    // Draw X and Y axis
    glColor3f( 1.0, 1.0, 1.0 );
    glVertex2f( -1.0, 0.0 );
    glVertex2f( 1.0, 0.0 );
    glVertex2f( 0.0, 1.0 );
    glVertex2f( 0.0, -1.0 );

    // Plot Ohms Law
    
    const float R = 10.0f;  // 10 ohms

    // Plot V=IR curve
    glColor3f(0.0, 0.0, 1.0);  // Blue color for the curve
    for (float I = -1.0; I <= 1.0; I += 0.001) {
        float V = I * R;
        glVertex2f(I, V);
    }
    glEnd();

    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}