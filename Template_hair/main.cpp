#include <windows.h>
#include "FreeImagePlus.h"
#include "GL/glut.h"
#include "GL/glext.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdarg.h>
#include "string.h"
#include <cstring>
#include "Vector3D.h"
#include "ArcBall.h"
#include "FreeType.h"
#include "Physics2.h"
#include "LightScattering.h"
using namespace std;

//const GLfloat piover180 = 0.0174532925f;
const double pi = 3.141592653;
const double e = 2.7182818282135;

GLfloat zoom = -20;
GLfloat Xshift = 0.0;
GLfloat Yshift = 0.0;
GLuint base;
HDC hDC = NULL;
HWND hWnd = NULL;
int Window_Height = 480;
int Window_Width = 640;
bool keystate[256];
bool writelog = true;

char FPS[1000];
int hairnumber;
struct point
{
    float x;
    float y;
    float z;
}hairlocation[2000000];
int strandnumber;
int strand[1500000];


ArcBall_t* arc;
freetype::font_data our_font;


Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };
Point2fT    MousePt;

bool        isClicked  = false;
bool        isRClicked = false;
bool        isDragging = false;

double theta_i(CVector3 v1, CVector3 axisX , CVector3 axisY, CVector3 axisZ )
{
    if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))<=pi/2&&acos(v1.DotProduct(axisX)/(v1.GetLength()*axisX.GetLength()))<=pi/2)
        return acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()));
    else if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))<=pi/2&&acos(v1.DotProduct(axisX)/(v1.GetLength()*axisX.GetLength()))>=pi/2)
        return -acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()));
    else if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))>=pi/2&&acos(v1.DotProduct(axisX)/(v1.GetLength()*axisX.GetLength()))<=pi/2)
        return acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()));
    else if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))>=pi/2&&acos(v1.DotProduct(axisX)/(v1.GetLength()*axisX.GetLength()))>=pi/2)
        return -acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()));
}

double phi_i(CVector3 v1, CVector3 axisX , CVector3 axisY, CVector3 axisZ )
{
    double k = (v1.x*axisX.x+v1.y*axisX.y+v1.z*axisX.z)/(axisX.x*axisX.x+axisX.y*axisX.y+axisX.z*axisX.z);
    v1 = CVector3(v1.x-k*axisX.x,v1.y-k*axisX.y,v1.z-k*axisX.z);
    if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))<=pi/2&&acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()))<=pi/2)
        return acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()));
    else if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))<=pi/2&&acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()))>=pi/2)
        return acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()));
    else if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))>=pi/2&&acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()))<=pi/2)
        return -acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()));    //negative?
    else if(acos(v1.DotProduct(axisZ)/(v1.GetLength()*axisZ.GetLength()))>=pi/2&&acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()))>=pi/2)
        return -acos(v1.DotProduct(axisY)/(v1.GetLength()*axisY.GetLength()));    //negative?
}

int bytesToInt(byte* bytes,int size = 4)
{
    int addr = bytes[0] & 0xFF;
    addr |= ((bytes[1] << 8) & 0xFF00);
    addr |= ((bytes[2] << 16) & 0xFF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

float bytesTofloat(byte* bytes)
{
    int bits = bytesToInt(bytes);
    int
    s = ( bits >> 31 ) == 0 ? 1 : -1,
    e = ( bits >> 23 ) & 0xff,
    m = ( e == 0 ) ?
                     ( bits & 0x7fffff ) << 1 :
                     ( bits & 0x7fffff ) | 0x800000;
    return s * m * ( float ) pow( 2, e - 150 );
}

string bytestohexstring(char* bytes,int bytelength)
{
  string str("");
  string str2("0123456789abcdef");
   for (int i=0;i<bytelength;i++) {
     int b;
     b = 0x0f&(bytes[i]>>4);
     char s1 = str2.at(b);
     str.append(1,str2.at(b));
     b = 0x0f & bytes[i];
     str.append(1,str2.at(b));
     char s2 = str2.at(b);
   }
   return str;
}

void fileread()
{
    freopen(".\\straight.hair","rb",stdin);
    char a[100];
    scanf("%c%c%c%c",&a[0],&a[1],&a[2],&a[3]);
    hairnumber = bytesToInt(a);
    for(int i=1;i<=hairnumber;i++)
    {
        char tmp[100];
        scanf("%c%c%c%c",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
        hairlocation[i].x = bytesTofloat(tmp)*0.1;
        scanf("%c%c%c%c",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
        hairlocation[i].y = bytesTofloat(tmp)*0.1;
        scanf("%c%c%c%c",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
        hairlocation[i].z = bytesTofloat(tmp)*0.1;
    }

    scanf("%c%c%c%c",&a[0],&a[1],&a[2],&a[3]);
    strandnumber = bytesToInt(a);
    for(int i=1;i<=strandnumber;i++)
    {
        char tmp[100];
        scanf("%c%c%c%c",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
        strand[i] = bytesToInt(tmp);
    }
    //cout<<hairlocation[1].x<<hairlocation[1].y<<hairlocation[1].z;
}

void ShowFPS()
{
    glPushMatrix();
    glLoadIdentity();
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
    //sprintf(FPS, "FPS: %.5f", PerlinNoise_2D(1000,3.0));
    freetype::print(our_font, 0,460, FPS);
    glPopMatrix();
}

void ShowText()
{
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0,1.0,0.0);

    static float framesPerSecond    = 0.0f;
    static float lastTime           = 0.0f;

    char text[1000] = "this is text";
    int number = 100;
    //sprintf(text, "%d", strandnumber);
    //sprintf(FPS, "FPS: %.5f", PerlinNoise_2D(1000,3.0));
    freetype::print(our_font, 100,460, text);
    glPopMatrix();
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

static void idle(void)
{
    glutPostRedisplay();
}

void testkey()
{

    if(keystate['Q']==true||keystate['q']==true||keystate[27]==true)exit(0);
    if(keystate['D']==true||keystate['d']==true)
    {
        Xshift+=0.1;
    }
    if(keystate['A']==true||keystate['a']==true)
    {
        Xshift-=0.1;
    }
    if(keystate['W']==true||keystate['w']==true)
    {
        Yshift+=0.1;
    }
    if(keystate['S']==true||keystate['s']==true)
    {
        Yshift-=0.1;
    }
    if(keystate['O']==true||keystate['o']==true)
    {
        zoom++;
    }
    if(keystate['P']==true||keystate['p']==true)
    {
        zoom--;
    }
}

static void keydown(unsigned char key, int x, int y)
{
    if(keystate[key]==false)keystate[key]=true;


}

static void keyup(unsigned char key, int x, int y)
{
    if(keystate[key]==true)keystate[key]=false;

}

void testdrag()
{
    if (isRClicked)													// 如果右键按下，这重置所有的变量
    {
		Matrix3fSetIdentity(&LastRot);
		Matrix3fSetIdentity(&ThisRot);
        Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
    }

    if (!isDragging)
    {
        if (isClicked)
        {
			isDragging = true;
			LastRot = ThisRot;
			arc->click(&MousePt);
        }
    }
    else
    {
        if (isClicked)
        {
            Quat4fT     ThisQuat;

            arc->drag(&MousePt, &ThisQuat);
            Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
            Matrix3fMulMatrix3f(&ThisRot, &LastRot);
            Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
        }
        else
            isDragging = false;
    }
}


void mouse(int button,int state,int x,int y)
{

    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
    {
        isClicked = false;
    }
    else if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
        isClicked = true;
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_UP)
    {
        isRClicked = false;
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN)
    {
        isRClicked = true;
    }

    if(button==GLUT_WHEEL_DOWN)
    {
        zoom--;
    }
    else if(button==GLUT_WHEEL_UP)
    {
        zoom++;
    }

    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}

void mousemove(int x,int y)
{
    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}


static void display(void)
{
//-----------------------------------------------------------//
    testkey();
    testdrag();
//-----------------------------------------------------------//
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0,0.0, zoom );
    glMultMatrixf(Transform.M);

//-----------------------------------------------------------//

    if(writelog)
    {
        freopen("output.txt","w",stdout);
        writelog = false;
    }


	CVector3 light = CVector3(-hairlocation[1].z+1.0+Xshift,-hairlocation[1].y-4.0+Yshift,6.0+hairlocation[1].x);
	CVector3 view = CVector3(-hairlocation[1].z+1.0,-hairlocation[1].y-7.0,4.0+hairlocation[1].x);

    int cnt = 1;

    //glColor3ub(255, 255, 255);
    //glTranslatef(-hairlocation[1].x,hairlocation[1].y+5.0,-hairlocation[1].z-5.0);


    for(int i=1;i<=strandnumber;i+=1)
    {
        for(int j=1;j<=strand[i]-1;j++)
        {
            if(hairlocation[cnt].x>0)
            {
                //glLineWidth(1);
                CVector3 position = CVector3(-hairlocation[cnt].z,-hairlocation[cnt].y,hairlocation[cnt].x);
                CVector3 positionv = CVector3(hairlocation[cnt].z-hairlocation[cnt+1].z,hairlocation[cnt].y-hairlocation[cnt+1].y,-hairlocation[cnt].x+hairlocation[cnt+1].x);

                positionv.Normalize();
                CVector3 axisY = CVector3(-positionv.y,positionv.x,0);
                axisY.Normalize();
                CVector3 axisZ = CVector3(positionv.y*axisY.z-positionv.z*axisY.y,positionv.z*axisY.x-positionv.x*axisY.z,positionv.x*axisY.y-positionv.y*axisY.x);
                axisZ.Normalize();

                CVector3 lightv = light - position;
                CVector3 viewv = view - position;
                LightScattering l = LightScattering();
                //cout<<phi_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<phi_i(viewv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(viewv,positionv,axisY,axisZ)/piover180<<endl;
                double Sr = l.scattering(phi_i(lightv,positionv,axisY,axisZ),phi_i(viewv,positionv,axisY,axisZ),theta_i(lightv,positionv,axisY,axisZ),theta_i(viewv,positionv,axisY,axisZ),0);
                if(Sr>1)Sr=1;
                double Sg = l.scattering(phi_i(lightv,positionv,axisY,axisZ),phi_i(viewv,positionv,axisY,axisZ),theta_i(lightv,positionv,axisY,axisZ),theta_i(viewv,positionv,axisY,axisZ),1);
                if(Sg>1)Sg=1;
                double Sb = l.scattering(phi_i(lightv,positionv,axisY,axisZ),phi_i(viewv,positionv,axisY,axisZ),theta_i(lightv,positionv,axisY,axisZ),theta_i(viewv,positionv,axisY,axisZ),2);
                if(Sb>1)Sb=1;
                glColor3ub(255*Sr,255*Sg,255*Sb);
                //cout<<Sr<<endl;
                //glColor3ub(0,0,0);
                glBegin(GL_LINES);
                glVertex3f(-hairlocation[cnt].z,-hairlocation[cnt].y,hairlocation[cnt].x);
                glVertex3f(-hairlocation[cnt+1].z,-hairlocation[cnt+1].y,hairlocation[cnt+1].x);
                glEnd();
            }

            cnt++;
            if(j==strand[i]-1)cnt++;


        }
        cnt+=(1-1)*strand[i];
    }
    fclose(stdout);

    /*glPointSize(10);
    glColor3ub(255, 0, 0);
    glBegin(GL_POINTS);
    glVertex3f(-hairlocation[1].z+1.0+Xshift,-hairlocation[1].y-4.0+Yshift,6.0+hairlocation[1].x);
    glEnd();

    glColor3ub(0, 255, 0);
    glBegin(GL_POINTS);
    glVertex3f(-hairlocation[1].z+1.0,-hairlocation[1].y-7.0,4.0+hairlocation[1].x);
    glEnd();*/



    /*CVector3 light = CVector3(0,0,10.0);
	CVector3 view = CVector3(0,0,5.0);

    glScalef(0.1,0.1,0.1);
    glTranslatef(0,5,0);
    int cnt = 1;

    for(int i=1;i<=1;i++)
    {
        for(int j=1;j<=2000;j++)
        {
            CVector3 position = CVector3(0,20-j*0.02,0);
            CVector3 positionv = CVector3(0,-0.02,0);
            positionv.Normalize();
            CVector3 axisY = CVector3(-positionv.y,positionv.x,0);
            axisY.Normalize();
            CVector3 axisZ = CVector3(positionv.y*axisY.z-positionv.z*axisY.y,positionv.z*axisY.x-positionv.x*axisY.z,positionv.x*axisY.y-positionv.y*axisY.x);
            axisZ.Normalize();

            CVector3 lightv = position - light;
            CVector3 viewv = view - position;
            LightScattering l = LightScattering();
            double S = l.scattering(phi_i(lightv,positionv,axisY,axisZ),phi_i(viewv,positionv,axisY,axisZ),theta_i(lightv,positionv,axisY,axisZ),theta_i(viewv,positionv,axisY,axisZ));
            //cout<<phi_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<phi_i(viewv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(viewv,positionv,axisY,axisZ)/piover180<<endl;
            //cout<<-hairlocation[cnt].y<<endl;
            if(255*S*10>255)S=0.1;
            glColor3ub(255*S*10,255*S*10,255*S*10);
            glBegin(GL_QUADS);
            glVertex3f(position.x,position.y,position.z);
            glVertex3f(position.x+positionv.x,position.y+positionv.y,position.z+positionv.z);
            glVertex3f(position.x+positionv.x+1.0,position.y+positionv.y,position.z+positionv.z);
            glVertex3f(position.x+1.0,position.y,position.z);
            glEnd();
            //cout<<S<<endl;
            cnt++;
        }
    }*/
//------------------------------------------------------------//



//----------------------平面数据处理-------------------------//

    ShowFPS();
    ShowText();

//------------------------------------------------------------//
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

    memset(keystate,0,sizeof(keystate));

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
    glutIdleFunc(idle);

    glShadeModel( GL_SMOOTH );
    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glClearColor(0.8f,0.8f,0.8f,0.0f);
    //glClearColor(0.0f,0.0f,0.0f,0.0f);

    //glEnable(GL_BLEND);
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glClearDepth( 1.0 );
    //glEnable( GL_DEPTH_TEST );
    //glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


    //makelist();
    //gentexture();
    fileread();
    FreeImage_Initialise();
    our_font.init("resources\\fonts\\test.TTF", 16);
    arc = new ArcBall_t(640.0f, 480.0f);

    //----------------------------------------------------------------------------------------

    /*CVector3 light = CVector3(hairlocation[1].x-2.5+Xshift,-hairlocation[1].y-5.0+Yshift,10.0+hairlocation[1].z);
	CVector3 view = CVector3(hairlocation[1].x-2.5,-hairlocation[1].y-5.0,5.0+hairlocation[1].z);

    int cnt = 1000*strand[1]+1;

    for(int i=1;i<=1;i+=50)
    {
        for(int j=1;j<=strand[i]-1;j++)
        {
            glLineWidth(1);
            CVector3 position = CVector3(hairlocation[cnt].x,-hairlocation[cnt].y,hairlocation[cnt].z);
            CVector3 positionv = CVector3(-hairlocation[cnt].x+hairlocation[cnt+1].x,hairlocation[cnt].y-hairlocation[cnt+1].y,-hairlocation[cnt].z+hairlocation[cnt+1].z);

            positionv.Normalize();
            CVector3 axisY = CVector3(-positionv.y,positionv.x,0);
            axisY.Normalize();
            CVector3 axisZ = CVector3(positionv.y*axisY.z-positionv.z*axisY.y,positionv.z*axisY.x-positionv.x*axisY.z,positionv.x*axisY.y-positionv.y*axisY.x);
            axisZ.Normalize();

            CVector3 lightv = position - light;
            CVector3 viewv = view - position;
            LightScattering l = LightScattering();
            double S = l.scattering(phi_i(lightv,positionv,axisY,axisZ),phi_i(viewv,positionv,axisY,axisZ),theta_i(lightv,positionv,axisY,axisZ),theta_i(viewv,positionv,axisY,axisZ));
            //cout<<phi_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<phi_i(viewv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(viewv,positionv,axisY,axisZ)/piover180<<endl;
            //cout<<S<<endl;

            cnt++;
        }
        cnt+=(50-1)*strand[i];
    }*/

    /*CVector3 light = CVector3(0,0,10.0);
	CVector3 view = CVector3(0,0,5.0);
    int cnt = 1;

    for(int i=1;i<=1;i++)
    {
        for(int j=1;j<=2000;j++)
        {
            CVector3 position = CVector3(0,20-j*0.02,0);
            CVector3 positionv = CVector3(0,-0.02,0);
            positionv.Normalize();
            CVector3 axisY = CVector3(-positionv.y,positionv.x,0);
            axisY.Normalize();
            CVector3 axisZ = CVector3(positionv.y*axisY.z-positionv.z*axisY.y,positionv.z*axisY.x-positionv.x*axisY.z,positionv.x*axisY.y-positionv.y*axisY.x);
            axisZ.Normalize();

            CVector3 lightv = light - position;
            CVector3 viewv = view - position;
            LightScattering l = LightScattering();
            double S = l.scattering(phi_i(lightv,positionv,axisY,axisZ),phi_i(viewv,positionv,axisY,axisZ),theta_i(lightv,positionv,axisY,axisZ),theta_i(viewv,positionv,axisY,axisZ));
            cout<<phi_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<phi_i(viewv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(lightv,positionv,axisY,axisZ)/piover180<<' '<<theta_i(viewv,positionv,axisY,axisZ)/piover180<<endl;
            //cout<<-hairlocation[cnt].y<<endl;
            //cout<<S<<endl;
            cnt++;
        }
    }*/




    //-------------------------------------------------------------------------

    glutMainLoop();

    return EXIT_SUCCESS;
}
