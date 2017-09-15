#include <windows.h>
#include "FreeImagePlus.h"
#include "GL/glut.h"
#include "GL/glext.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Vector3D.h"
#include <stdarg.h>
#include <string>
#include "FreeType.h"

const GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };
const GLfloat GL_white[4] = {1.0,1.0,1.0,1.0};
const GLfloat GL_origin[4] = {0.2,0.2,0.2,1.0};

const GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.5f};

const GLfloat piover180 = 0.0174532925f;

PFNGLMULTITEXCOORD1FARBPROC        glMultiTexCoord1fARB    = NULL;
PFNGLMULTITEXCOORD2FARBPROC        glMultiTexCoord2fARB    = NULL;
PFNGLACTIVETEXTUREARBPROC          glActiveTextureARB      = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC    glClientActiveTextureARB= NULL;

std::vector<GLuint> listname;
std::vector<GLuint> texture;
int mat_terrain[100][100][100];
int mapsizex;
int mapsizey;
int mapsizez;
GLuint base;
HDC hDC = NULL;
HWND hWnd = NULL;
CVector3 m_vPosition;
CVector3 m_vView;
CVector3 m_vUpVector;
CVector3 m_vStrafe;
int Window_Height = 480;
int Window_Width = 640;
bool keystate[256];

float movespeed,strafespeed,jumpspeed;
int eyesight=8;
const float movespeedmax = 10,strafespeedmax = 10;
const float facc=-0.1;
char FPS[1000];
int positionx,positiony,positionz;
int targetx,targety,targetz;
int mousedelay=100;
int mousedelaymax=100;


freetype::font_data our_font;

void DrawCenter()
{

    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
        glVertex2f(Window_Width/2-10.0,Window_Height/2);
        glVertex2f(Window_Width/2+10.0,Window_Height/2);
        glVertex2f(Window_Width/2,Window_Height/2-10.0);
        glVertex2f(Window_Width/2,Window_Height/2+10.0);
    glEnd();
    glPopMatrix();

}

void ShowHint()
{
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor3f(0.0,1.0,0.0);

    static float framesPerSecond    = 0.0f;
    static float lastTime           = 0.0f;

    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    if( currentTime - lastTime > 1.0f )
    {
        lastTime = currentTime;
        sprintf(FPS, "FPS: %d", int(framesPerSecond));
        framesPerSecond = 0;
    }
    freetype::print(our_font, 0,460, FPS);
    glPopMatrix();
}

static void makelist()
{
    GLuint temp;
    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, texture[1] );
    glEnable( GL_TEXTURE_2D );
    glBegin(GL_QUADS);
      glNormal3f(  0.0,  0.0, 1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 0.0, 1.0 );glVertex3f( -1.0,  1.0,  1.0 );

      glNormal3f(  0.0,  0.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );



      glNormal3f(  1.0,  0.0,  0.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

      glNormal3f( -1.0,  0.0,  0.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[0] );
    glEnable( GL_TEXTURE_2D );
    glBegin( GL_QUADS );
      glNormal3f(  0.0,  1.0,  0.0 );
      glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[2] );
    glEnable( GL_TEXTURE_2D );
    glBegin( GL_QUADS );
      glNormal3f(  0.0, -1.0,  0.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 );glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glEnd();
    glEndList();


    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, texture[3] );
    glEnable( GL_TEXTURE_2D );
    glBegin(GL_QUADS);
      glNormal3f(  0.0,  0.0, 1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 0.0, 1.0 );glVertex3f( -1.0,  1.0,  1.0 );

      glNormal3f(  0.0,  0.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );



      glNormal3f(  1.0,  0.0,  0.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

      glNormal3f( -1.0,  0.0,  0.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );

      glNormal3f(  0.0,  1.0,  0.0 );
      glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

      glNormal3f(  0.0, -1.0,  0.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 );glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glEnd();


    glEndList();

    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, texture[4] );
    glEnable( GL_TEXTURE_2D );
    glBegin(GL_QUADS);
      glNormal3f(  0.0,  0.0, 1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 0.0, 1.0 );glVertex3f( -1.0,  1.0,  1.0 );

      glNormal3f(  0.0,  0.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );



      glNormal3f(  1.0,  0.0,  0.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

      glNormal3f( -1.0,  0.0,  0.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f(0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );

      glNormal3f(  0.0,  1.0,  0.0 );
      glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
      glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

      glNormal3f(  0.0, -1.0,  0.0 );
      glTexCoord2f(1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 1.0 );glVertex3f(  1.0, -1.0, -1.0 );
      glTexCoord2f(0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
      glTexCoord2f(1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glEnd();

    glEndList();

    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);
    glDisable(GL_LIGHTING);
    glColor3f(0.0,0.0,1.0);
    //glLineWidth(2.0);
    glBegin(GL_LINES);

        glVertex3f(-1.0,1.0,1.0);
        glVertex3f(-1.0,1.0,-1.0);

        glVertex3f(-1.0,1.0,1.0);
        glVertex3f(-1.0,-1.0,1.0);

        glVertex3f(1.0,-1.0,1.0);
        glVertex3f(1.0,1.0,1.0);

        glVertex3f(1.0,1.0,-1.0);
        glVertex3f(1.0,1.0,1.0);

        glVertex3f(-1.0,-1.0,-1.0);
        glVertex3f(-1.0,-1.0,1.0);

        glVertex3f(-1.0,1.0,-1.0);
        glVertex3f(-1.0,-1.0,-1.0);

        glVertex3f(1.0,-1.0,-1.0);
        glVertex3f(1.0,-1.0,1.0);

        glVertex3f(1.0,-1.0,-1.0);
        glVertex3f(1.0,1.0,-1.0);

        glVertex3f(-1.0,1.0,1.0);
        glVertex3f(1.0,1.0,1.0);

        glVertex3f(-1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,1.0);

        glVertex3f(-1.0,1.0,-1.0);
        glVertex3f(1.0,1.0,-1.0);

        glVertex3f(-1.0,-1.0,-1.0);
        glVertex3f(1.0,-1.0,-1.0);
    glEnd();

    glEndList();
}

static void reshape(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

static void loadMap(std::string filename)
{
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\resources\\readins\\"+filename);
    freopen(a.c_str(),"r",stdin);
    scanf("%d %d %d",&mapsizex,&mapsizey,&mapsizez);
    for(int k=0;k<mapsizez;k++)
    {
        for(int i=0;i<mapsizex;i++)
        {
            for(int j=0;j<mapsizey;j++)
            {
            scanf("%d",&mat_terrain[j][i][k]);
            }
        }
    }

}

void loadGLTexture(std::string filename)
{
    FIBITMAP* dib = NULL;
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\resources\\graphs\\"+filename);
    //FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //fif = FreeImage_GetFileType(a.c_str(),0);
    dib = FreeImage_Load(FIF_BMP,a.c_str(),BMP_DEFAULT);
    FIBITMAP *temp = FreeImage_ConvertTo24Bits(dib);
    FreeImage_Unload(dib);
    dib = temp;
    BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];
    BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
    for(int pix=0; pix<FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++)
    {
        bits[pix*3+0]=pixels[pix*3+2];
        bits[pix*3+1]=pixels[pix*3+1];
        bits[pix*3+2]=pixels[pix*3+0];
    }

    GLuint i;
    glGenTextures( 1, &i );
    glBindTexture( GL_TEXTURE_2D, i );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,GL_RGB, GL_UNSIGNED_BYTE, bits );
    texture.push_back(i);
    FreeImage_Unload(dib);
}

static void loadGLTextures()
{
    loadGLTexture("grass1.bmp");
    loadGLTexture("grass2.bmp");
    loadGLTexture("grass3.bmp");
    loadGLTexture("sand.bmp");
    loadGLTexture("stone.bmp");
}

static void idle(void)
{
    glutPostRedisplay();
}

void PositionCamera(float posX, float posY, float posZ,
							 float viewX, float viewY, float viewZ,
							 float upX, float upY, float upZ)
{
	CVector3 vPos	= CVector3(posX, posY, posZ);
	CVector3 vView	= CVector3(viewX, viewY, viewZ);
	CVector3 vUp	= CVector3(upX, upY, upZ);

	m_vPosition = vPos;
	m_vView     = vView;
	m_vUpVector = vUp;

	m_vView = m_vPosition + (m_vView-m_vPosition).GetNormalized();
	SetCursorPos((Window_Width >> 1)+100, (Window_Height >> 1)+100);
}

void RotateView(float angle, const CVector3 &vAxis)
{
	CVector3 vNewView;
	CVector3 vView = m_vView - m_vPosition;

	vNewView = vView.GetRotatedAxis(angle, vAxis);

	m_vView = m_vPosition + vNewView;
}

//--这个人有3单位高，实际碰撞位置在2单位处,然后现在只检测了点的碰撞，完全没有考虑人的体积啊囧----//

void StrafeCamera(float speed)
{
    if(mat_terrain[(int)((m_vPosition.x+(float)(m_vStrafe.x * speed/1000)+1)/2)][(int)((m_vPosition.z + (float)(m_vStrafe.z * speed/1000)+1)/2)][(int)((m_vPosition.y+1)/2)]!=-1)return;
	m_vPosition.x += (float)(m_vStrafe.x * speed/1000);
	m_vPosition.z += (float)(m_vStrafe.z * speed/1000);
	m_vView.x += (float)(m_vStrafe.x * speed/1000);
	m_vView.z += (float)(m_vStrafe.z * speed/1000);
}

void MoveCamera(float speed)
{
	CVector3 vView = m_vView - m_vPosition;
	if(mat_terrain[(int)((m_vPosition.x+(float)(vView.x * speed/1000)+1)/2)][(int)((m_vPosition.z + (float)(vView.z * speed/1000)+1)/2)][(int)((m_vPosition.y+1)/2)]!=-1)return;
	m_vPosition.x += (float)(vView.x * speed/1000);
	m_vPosition.z += (float)(vView.z * speed/1000);
	m_vView.x += (float)(vView.x * speed/1000);
	m_vView.z += (float)(vView.z * speed/1000);
}

void movecharacter()
{
    MoveCamera(movespeed);
    StrafeCamera(strafespeed);
}

void accelratemovespeed(float acc,int f)
{

    movespeed+=acc;
    if(f==1&&movespeed>0)movespeed=0;
    if(f==-1&&movespeed<0)movespeed=0;
    if(movespeed*positionx>=movespeedmax)
    {
        movespeed = positionx*movespeedmax;
    }
}

void accelratestrafespeed(float acc,int f)
{
    strafespeed+=acc;
    if(f==1&&strafespeed>0)strafespeed=0;
    if(f==-1&&strafespeed<0)strafespeed=0;
    if(strafespeed*positiony>=strafespeedmax)
    {
        strafespeed = positiony*strafespeedmax;
    }
}


void mousemove(int x,int y)
{
    int middleX = Window_Width >> 1;
	int middleY = Window_Height >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	if ((x == middleX) && (y == middleY))
	{
		return;
	}



	angleY = (float)((middleX - x)) / 1000.0f;
	angleZ = (float)((middleY - y)) / 1000.0f;

	RotateView(angleZ, m_vStrafe);
    RotateView(angleY, CVector3(0,1,0));


    glutPostRedisplay();
}

void testkey()
{
    if(keystate['Q']==true||keystate['q']==true||keystate[27]==true)exit(0);
    if(keystate['D']==true||keystate['d']==true)
    {
        accelratestrafespeed(2,0);
        positiony = 1;
    }
    if(keystate['A']==true||keystate['a']==true)
    {
        accelratestrafespeed(-2,0);
        positiony=-1;
    }
    if(keystate['W']==true||keystate['w']==true)
    {
        accelratemovespeed(2,0);
        positionx=1;
    }
    if(keystate['S']==true||keystate['s']==true)
    {
        accelratemovespeed(-2,0);
        positionx=-1;
    }
    if(movespeed>0)accelratemovespeed(facc,-1);
    if(movespeed<0)accelratemovespeed(-facc,1);
    if(strafespeed>0)accelratestrafespeed(facc,-1);
    if(strafespeed<0)accelratestrafespeed(-facc,1);


}

static void keydown(unsigned char key, int x, int y)
{
    if(keystate[key]==false)keystate[key]=true;


}

static void keyup(unsigned char key, int x, int y)
{
    if(keystate[key]==true)keystate[key]=false;

}


static void mouse(int button,int state,int x,int y)
{
      if(button== GLUT_WHEEL_DOWN)
      {
          CVector3 k_position = (m_vView-m_vPosition).GetNormalized();
          m_vPosition =  m_vPosition-k_position;
          m_vView = m_vView-k_position;
      }
      if(button== GLUT_WHEEL_UP)
      {
          CVector3 k_position = (m_vView-m_vPosition).GetNormalized();
          m_vPosition =  m_vPosition+k_position;
          m_vView = m_vView+k_position;
      }
      if(button== GLUT_LEFT_BUTTON)
      {
          if(targetx!=-1&&mousedelay==0)
          {
              mat_terrain[targetx][targetz][targety]=-1;
              mousedelay=mousedelaymax;
          }
      }

    glutPostRedisplay();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
			  m_vView.x,	 m_vView.y,     m_vView.z,
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
    m_vStrafe = ((m_vView - m_vPosition).CrossProduct(m_vUpVector)).GetNormalized();

    SetCursorPos(420, 340);
    if(mousedelay>0)mousedelay--;
    testkey();
    movecharacter();

//----------------------------------------------------------------------------------------//
    glEnable(GL_LIGHTING);

    for(int k=0;k<mapsizez;k++)
    {
        for(int i=0;i<mapsizex;i++)
        {
            for(int j=0;j<mapsizey;j++)
            {
                if(mat_terrain[i][j][k]==-1)continue;
                glPushMatrix();
                glTranslatef(2.0*i,2.0*k, 2.0*j);
                glCallList(listname[mat_terrain[i][j][k]]);
                glPopMatrix();
            }
        }
    }

    glPushMatrix();
    for(int i=1;i<=eyesight;i++)
    {
        int tmpx=(int)(((m_vPosition+(m_vView-m_vPosition).GetNormalized()*i).x+1)/2);
        int tmpy=(int)(((m_vPosition+(m_vView-m_vPosition).GetNormalized()*i).y+1)/2);
        int tmpz=(int)(((m_vPosition+(m_vView-m_vPosition).GetNormalized()*i).z+1)/2);
        if(mat_terrain[tmpx][tmpz][tmpy]!=-1)
        {
            targetx=tmpx;
            targety=tmpy;
            targetz=tmpz;
            glTranslatef(2.0*tmpx,2.0*tmpy,2.0*tmpz);
            glCallList(listname[3]);
            break;
        }
        targetx=targety=targetz=-1;
    }
    glPopMatrix();

//----------------------平面数据处理-----------------------------------------------------//

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0,Window_Width,Window_Height,0.0);
    glMatrixMode( GL_MODELVIEW );

    DrawCenter();
    ShowHint();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

//----------------------------------------------------------------------------------------//

    glutSwapBuffers();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(Window_Width,Window_Height);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

    glutCreateWindow("GLUT");

    hWnd = FindWindow(NULL,"GLUT");
    hDC = GetDC(hWnd);

    glMultiTexCoord1fARB    = (PFNGLMULTITEXCOORD1FARBPROC)        wglGetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)        wglGetProcAddress("glMultiTexCoord2fARB");
    glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)        wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)    wglGetProcAddress("glClientActiveTextureARB");

    memset(keystate,0,sizeof(keystate));

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mousemove);
    glutIdleFunc(idle);

    loadGLTextures();
    loadMap("map.txt");
    glShadeModel( GL_SMOOTH );
    glClearColor(0.0f,0.0f,0.0f,0.5f);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable(GL_LIGHTING);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    /*glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);*/

    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


    makelist();
    FreeImage_Initialise();
    our_font.init("resources\\fonts\\test.TTF", 16);

	SetCursorPos((Window_Width >> 1)+100, (Window_Height >> 1)+100);
	ShowCursor (FALSE);

    CVector3 vPos(6.0, 2.5, 18.0);
    CVector3 vView(6.0, 2.5,17.0);
    CVector3 vUp(0.0, 1.0, 0.0);

	m_vPosition	= vPos;
	m_vView		= vView;
	m_vUpVector	= vUp;

	movespeed = 0.0;
	strafespeed = 0.0;

    glutMainLoop();

    return EXIT_SUCCESS;
}
