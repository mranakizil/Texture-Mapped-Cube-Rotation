// CSE4083 Computer Graphics
// Last Homework
// Merve Rana Kızıl - 150119825

#include<windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float crotate = 0;
static GLuint textureName;
static int height = 256;
static int width = 256;
// Arbitrary axis for rotation
static float x = 1;
static float y = 0;
static float z = 1;

// Faces of the cube
GLint faces[][4]    = {{4,5,6,7},
                       {1,2,6,5},
                       {0,1,5,4},
                       {0,3,2,1},
                       {0,4,7,3},
                       {2,3,7,6}
                      };

// Vertices
GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},
                         { 1.0,-1.0,-1.0},
                         { 1.0, 1.0,-1.0},
                         {-1.0, 1.0,-1.0},
                         {-1.0,-1.0, 1.0},
                         { 1.0,-1.0, 1.0},
                         { 1.0, 1.0, 1.0},
                         {-1.0, 1.0, 1.0}};

// If the user changes the size of the screen, the contents of it will stay at a good ratio
void Reshape(int w, int h){

    if(h == 0)
        h = 1;

    float ratio = w*1.0/h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 1000);

    glMatrixMode(GL_MODELVIEW);

}


// Where updating happens
void Display(){

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select the texture object
    glBindTexture(GL_TEXTURE_2D, textureName);

     // Draw the cube
    for (int f = 0; f < 6; f++) {
        glBegin(GL_POLYGON);
        int v;

        v = faces[f][0];
        glTexCoord2f(0.0, 0.0);  // Map to the bottom left texel
        glVertex3fv(vertices[v]);

        v = faces[f][1];
        glTexCoord2f(0.0, 1.0);  // Map to the bottom right texel
        glVertex3fv(vertices[v]);

        v = faces[f][2];
        glTexCoord2f(1.0, 1.0);  // Map to the top right texel
        glVertex3fv(vertices[v]);

        v = faces[f][3];
        glTexCoord2f(1.0, 0.0);  // Map to the top left texel
        glVertex3fv(vertices[v]);

        glEnd();
    }


    // Force the rendering (off-screen)
    glFlush();

    // Handle the double buffering
    glutSwapBuffers();

    glLoadIdentity();

    glTranslatef(0, 0, -5);

    // Rotate the cube around an arbitrary axis
    crotate = crotate + 0.07;
    glRotatef(crotate, x, y, z);

    glutPostRedisplay();
}


void readImage(char* filename, GLbyte data[height][width][3]) {
  FILE * file;

  file = fopen(filename, "rb");
  if (file != NULL) {
    fread(data, height * width * 3, 1, file);
    fclose(file);
  }
  else{

    printf("Texture image could not found.");
    exit(0);
  }
}


void TextureMap(){

    glShadeModel(GL_FLAT);

    // Read the image
    GLbyte image[height][width][3];
    readImage("foto.rgb", image);


    // Get a name for the texture
    glGenTextures(1, &textureName);

    // Bind the texture object to its name
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Specify the parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    // Specify the application mode
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Create the texture object
    glTexImage2D(GL_TEXTURE_2D, 0,// One resolution (i.e. level 0)
               3,                 // 3 components (i.e., RGB)
               width,             // Width
               height,            // Height
               0,                 // Border width
               GL_RGB,            // Format
               GL_UNSIGNED_BYTE,  // Data type of the texels
               image);

    // Enable textures
    glEnable(GL_TEXTURE_2D);

}


void Init(){

    glClearColor(0.8, 0.9, 0.9, 0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.5, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    TextureMap();
}


int main(int argc, char** argv){

    glutInit(&argc, argv);

    // GLUT Initialization
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Cube Rotation");

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    Init();
    glutMainLoop();
    return 0;
}
