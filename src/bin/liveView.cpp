#include <GL/glut.h>

void drawAxis(float length){
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(length, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, length, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, length);
  glEnd();
  glPointSize(100);
  glColor3f(0.2, 0.2, 0.8);
  glBegin(GL_POINTS);
  glVertex3f(0, 0, 0);
  glEnd();
}

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0, -10, -60);
  glRotatef(30, 1, 0, 0);
  glRotatef(0, 0, 1, 0);
  glRotatef(0, 0, 0, 1);
  drawAxis(3);
  glutSwapBuffers();
  glFlush();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(1000, 800);
  glutCreateWindow("Velodyne Live View");
  glShadeModel(GL_SMOOTH);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glutDisplayFunc(renderScene);
  glutMainLoop();
}
