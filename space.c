#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

void init (void)
{
glClearColor(0,0,0,0);
}

void display (void)
{
glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
glutSwapBuffers ();
}

void reshape (int Width,int Height)
{
glViewport (0,0,Width,Height);
}

void tick (void)
{



glutPostRedisplay();
}





int main(int argc, char** argv)
{
  // GLUT Window Initialization:
  glutInit (&argc, argv);
  glutInitWindowSize (640, 480);
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("Space Thing");

  // Initialize OpenGL graphics state
  init();

  // Register callbacks:
  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  //glutKeyboardFunc (Keyboard);
  //glutMouseFunc (MouseButton);
  //glutMotionFunc (MouseMotion);
  glutIdleFunc (tick);

  // Turn the flow of control over to GLUT
  glutMainLoop ();
  return 0;
}


