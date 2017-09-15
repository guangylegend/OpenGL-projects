#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QGLWidget>
#include <QKeyEvent>
#include <gl/GLU.h>

class mywidget : public QGLWidget
{
    Q_OBJECT

public:

    mywidget(QWidget* parent = 0);
    ~mywidget();

protected:

    void keyPressEvent( QKeyEvent *e );
    void wheelEvent(QWheelEvent *event);
    void timerEvent( QTimerEvent * );

    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void loadGLTextures();

    GLuint texture[3];
    GLuint filter;
    GLfloat xRot, yRot, zRot;
    GLfloat hold;
    float points[45][45][3];
    int wiggle_count;

    bool light;
    bool blend;

};


#endif // MYWIDGET_H
