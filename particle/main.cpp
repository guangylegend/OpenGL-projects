#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/glaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "sneak_yg.h"

const GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };

const GLfloat fogColor[4]= {0.0f, 0.0f, 0.0f, 0.3f};

const int lim =2;
const int imitationsize=200;

GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zRot = 0;
GLfloat zoom = -50;
bool light = true;
GLint delay = 0;
std::vector<GLuint> texture;

Flame* test;


static void reshape(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    vector<Spark> temp = test->state();
    int vsize = temp.size();

    for(int i=0;i<vsize;i++)
    {
        glLoadIdentity();

        glTranslatef(0.0,  0.0, zoom );
        glRotatef( xRot,  1.0,  0.0,  0.0 );
        glRotatef( yRot,  0.0,  1.0,  0.0 );
        glTranslatef(0.0,  0.0, temp[i].getY()/lim);
        glTranslatef(0.0, temp[i].getZ()/lim,  0.0);
        glTranslatef(temp[i].getX()/lim,  0.0, 0.0 );
        glColor4f((255-temp[i].getR()+0.0)/255,(255-temp[i].getG()+0.0)/255,(255-temp[i].getB()+0.0)/255,0.5f);
        glutSolidSphere(1,20,16);

        /*glBegin(GL_QUADS);

				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();
        glBegin(GL_POINTS);
            glVertex3f(0.0f,0.0f,0.0f);
        glEnd();*/
    }



    delay--;
    if(delay<=0)
    {
        delay=3;
        test->move();
    }


    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'Q':
            exit(0);
            break;
        case 'D':
            yRot+=2;
            break;
        case 'A':
            yRot-=2;
            break;
        case 'W':
            xRot-=2;
            break;
        case 'S':
            xRot+=2;
            break;
        case 'L':
            if ( light )
            {
                glDisable(GL_LIGHTING);
            }
            else
            {
                glEnable( GL_LIGHTING);
            }
            light = !light;
            break;
    }


    glutPostRedisplay();
}
static void mouse(int button,int state,int x,int y)
{
    switch(button)
    {
        case GLUT_WHEEL_UP:
        zoom++;
        break;
        case GLUT_WHEEL_DOWN:
        zoom--;
        break;
    }
    glutPostRedisplay();
}

void loadGLTexture(std::string filename)
{
    AUX_RGBImageRec* TEXTURE;
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\"+filename);
    TEXTURE = auxDIBImageLoad(a.c_str());
    GLuint i;
    glGenTextures( 1, &i );
    glBindTexture( GL_TEXTURE_2D, i );
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TEXTURE->sizeX, TEXTURE->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, TEXTURE->data );
    texture.push_back(i);
}
static void loadGLTextures()
{
    loadGLTexture("SPARK.BMP");
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    test = new Flame(imitationsize);
    test->init();

    glutCreateWindow("GLUT");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);


    glShadeModel( GL_SMOOTH );
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glEnable(GL_POINT_SMOOTH);
    loadGLTextures();
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_COLOR_MATERIAL);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable(GL_LIGHTING);

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 50.0f);
    glFogf(GL_FOG_END, 65.0f);
    glEnable(GL_FOG);

    glClearDepth( 1.0 );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glColor4f( 1.0, 1.0, 1.0, 0.5 );
    glEnable(GL_BLEND);

    glutMainLoop();

    return EXIT_SUCCESS;
}
