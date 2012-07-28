#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>



void init (void)
{}


void display (void)
{}

void reshape (int Width,int Height)
{
glViewport (0,0,Width,Height);
}


void tick (void)
{}





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


