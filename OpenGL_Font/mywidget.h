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

    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void loadGLTextures();
    void timerEvent(QTimerEvent *);

    GLuint texture[3];
    GLuint filter;

    GLfloat	cnt1;
    GLfloat	cnt2;

    bool light;
    bool blend;

};


#endif // MYWIDGET_H
