#include <windows.h>
#include <GL/glaux.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

const GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };

bool light = true;
GLuint filter = 0;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zRot = 0;
GLfloat zoom = -5;
GLuint texture[3];

GLUquadricObj *quadratic;
GLuint  object=0;

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
    yRot++;
    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );

    glBindTexture( GL_TEXTURE_2D, texture[filter] );

    switch(object)
	{
	case 0:
		glTranslatef(0.0f,0.0f,-1.5f);
		gluCylinder(quadratic,1.0f,1.0f,3.0f,32,32);
		break;
	case 1:
		gluDisk(quadratic,0.5f,1.5f,32,32);
		break;
	case 2:
		gluSphere(quadratic,1.3f,32,32);
		break;
    case 3:
		glTranslatef(0.0f,0.0f,-1.5f);
		gluCylinder(quadratic,1.0f,0.0f,3.0f,32,32);
		break;
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
                glDisable( GL_LIGHTING );
            }
            else
            {
                glEnable( GL_LIGHTING );
            }
        break;
        case 'C':
            filter += 1;;
            if ( filter > sizeof(texture)/sizeof(int)-1 )
            {
                filter = 0;
            }
            break;
        case 'G':
            object++;
            if(object>3)
            object=0;
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

static void loadGLTextures()
{
    AUX_RGBImageRec* TEXTURE[1];
    memset(TEXTURE,0,sizeof(void *));
    FILE* file = NULL;
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\Crate.bmp") ;
    file = fopen(a.c_str(),"r");
    if(file)
    {
        fclose(file);
        TEXTURE[0] = auxDIBImageLoad(a.c_str());
    }
    else return;
    glGenTextures( 3, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, TEXTURE[0]->sizeX, TEXTURE[0]->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, TEXTURE[0]->data );
    glBindTexture( GL_TEXTURE_2D, texture[1] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, TEXTURE[0]->sizeX, TEXTURE[0]->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, TEXTURE[0]->data );
    glBindTexture( GL_TEXTURE_2D, texture[2] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, TEXTURE[0]->sizeX, TEXTURE[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TEXTURE[0]->data );
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

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glEnable( GL_TEXTURE_2D );
    loadGLTextures();
    glShadeModel( GL_SMOOTH );
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable( GL_LIGHTING );

    quadratic=gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);

    glutMainLoop();

    return EXIT_SUCCESS;
}
