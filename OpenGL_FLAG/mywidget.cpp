#include "mywidget.h"

GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };


mywidget::mywidget(QWidget* parent)
    : QGLWidget( QGLFormat(QGL::SampleBuffers), parent )
{
    xRot = yRot = zRot = 0.0;
    hold = 0.0;

    wiggle_count = 0;

    light = true;
    blend = false;
    filter = 1;

    setGeometry(100,100,800,640);
    setWindowTitle ( "myOpenGL" );
    startTimer(5);
}

mywidget::~mywidget()
{
}

void mywidget::initializeGL()
{
    loadGLTextures();
    glEnable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.5 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glPolygonMode( GL_BACK, GL_FILL );
    glPolygonMode( GL_FRONT, GL_FILL );

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );
    glEnable( GL_LIGHTING );

    glColor4f( 1.0, 1.0, 1.0, 0.5 );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    for ( int x = 0; x < 45; x++ )
    {
      for ( int y = 0; y < 45; y++ )
      {
        points[x][y][0] = float( ( x/5.0 ) - 4.5 );
        points[x][y][1] = float( ( y/5.0 ) - 4.5 );
        points[x][y][2] = float( sin( ( ( ( x/5.0 ) * 40.0 )/360.0 ) * 3.141592654 * 2.0 ) );
      }
    }

}
void mywidget::paintGL()
{
    int x, y;
    float float_x, float_y, float_xb, float_yb;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    glTranslatef( 0.0, 0.0, -12.0 );

    glRotatef( xRot, 1.0, 0.0, 0.0 );
    glRotatef( yRot, 0.0, 1.0, 0.0 );
    glRotatef( zRot, 0.0, 0.0, 1.0 );

    glBindTexture( GL_TEXTURE_2D, texture[filter] );

    glBegin( GL_QUADS );
      for ( x = 0; x < 44; x++ )
      {
        for ( y = 0; y < 44; y++ )
        {
          float_x = float(x)/44.0;
          float_y = float(y)/44.0;
          float_xb = float(x+1)/44.0;
          float_yb = float(y+1)/44.0;

          glTexCoord2f( float_x, float_y );
          glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

          glTexCoord2f( float_x, float_yb );
          glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

          glTexCoord2f( float_xb, float_yb );
          glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

          glTexCoord2f( float_xb, float_y );
          glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
        }
      }
    glEnd();
    if ( wiggle_count == 2 )
    {
      for ( y = 0; y < 45; y++ )
      {
        hold = points[0][y][2];
        for ( x = 0; x < 44; x++ )
        {
          points[x][y][2] = points[x+1][y][2];
        }
        points[44][y][2] = hold;
      }
      wiggle_count = 0;
    }

    wiggle_count++;
    xRot += 0.3;
    yRot += 0.2;
    zRot += 0.4;
}
void mywidget::timerEvent(QTimerEvent*)
{
  updateGL();
}

void mywidget::resizeGL( int width, int height )
{
    if (height==0)
    {
       height=1;
    }
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void mywidget::loadGLTextures()
{
    QImage tex, buf;
    if ( !buf.load( "BG.bmp" ) )
    {
        qWarning( "Could not read image file, using single-color instead." );
        QImage dummy( 128, 128, QImage::Format_RGB32 );
        dummy.fill( QColor(Qt::white).rgb() );
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat( buf );
    glGenTextures( 3, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, tex.width(), tex.height(), 0,GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    glBindTexture( GL_TEXTURE_2D, texture[1] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, tex.width(), tex.height(), 0,GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    glBindTexture( GL_TEXTURE_2D, texture[2] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
}
void mywidget::keyPressEvent( QKeyEvent *e )
{
    if(e->key()==Qt::Key_Escape)
        close();
    if(e->key()==Qt::Key_Right)
    {
        updateGL();
    }
    if(e->key()==Qt::Key_Left)
    {
        updateGL();
    }
    if(e->key()==Qt::Key_Up)
    {

        updateGL();
    }
    if(e->key()==Qt::Key_Down)
    {
        updateGL();
    }
    if(e->key()==Qt::Key_PageDown)
    {
        updateGL();
    }
    if(e->key()==Qt::Key_PageUp)
    {
        updateGL();
    }
    if(e->key()==Qt::Key_L)
    {
        if ( light )
        {
            glDisable( GL_LIGHTING );
        }
        else
        {
            glEnable( GL_LIGHTING );
        }
        light = !light;
        updateGL();
    }
    if(e->key()==Qt::Key_B)
    {
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
        updateGL();
    }
    if(e->key()==Qt::Key_F)
    {
        filter += 1;;
        if ( filter > 2 )
        {
            filter = 0;
        }
        updateGL();
    }
}
void mywidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta();
    int num = numDegrees / 120;
    updateGL();
}


