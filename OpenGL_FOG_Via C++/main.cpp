#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/glaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

const GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };

const GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.5f};

PFNGLMULTITEXCOORD1FARBPROC        glMultiTexCoord1fARB    = NULL;
PFNGLMULTITEXCOORD2FARBPROC        glMultiTexCoord2fARB    = NULL;
PFNGLACTIVETEXTUREARBPROC          glActiveTextureARB      = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC    glClientActiveTextureARB= NULL;

bool light = true;
bool blend = false;
bool fog = true;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zRot = 0;
GLfloat zoom = -5;
std::vector<GLuint> texture;


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
    glTranslatef(0.0,  0.0, zoom );

    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );

    glActiveTextureARB(GL_TEXTURE0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glActiveTextureARB(GL_TEXTURE1);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture[1] );
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(-0.25,0.4,0.0);
    glMatrixMode(GL_MODELVIEW);
    glActiveTextureARB(GL_TEXTURE2);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture[2] );
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(-0.25,0.2,0.0);
    glMatrixMode(GL_MODELVIEW);




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
        case 'B':
            blend = !blend;
            if ( blend )
            {
                glEnable( GL_BLEND );
                glDisable( GL_DEPTH_TEST );
            }
            else
            {
                glDisable( GL_BLEND );
                glEnable( GL_DEPTH_TEST );
            }
            break;
        case 'F':
            if ( fog )
            {
                glDisable( GL_FOG );
            }
            else
            {
                glEnable( GL_FOG );
            }
            fog = !fog;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TEXTURE->sizeX, TEXTURE->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, TEXTURE->data );
    texture.push_back(i);
}
static void loadGLTextures()
{
    loadGLTexture("grass1.bmp");
    loadGLTexture("grass2.bmp");
    loadGLTexture("grass3.bmp");
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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT");

    glMultiTexCoord1fARB    = (PFNGLMULTITEXCOORD1FARBPROC)        wglGetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)        wglGetProcAddress("glMultiTexCoord2fARB");
    glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)        wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)    wglGetProcAddress("glClientActiveTextureARB");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    loadGLTextures();
    glShadeModel( GL_SMOOTH );
    glClearColor(0.2f,0.2f,0.2f,0.5f);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable(GL_LIGHTING);

    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);

    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );



    glColor4f( 1.0, 1.0, 1.0, 0.5 );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    glutMainLoop();

    return EXIT_SUCCESS;
}
