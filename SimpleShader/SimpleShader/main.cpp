#pragma comment (lib, "glew32.lib")
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float time = 0;
GLint loc,colorloc;

char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}
 
GLuint v,f,f2,p;
float lpos[4] = {1,0.5,1,0};

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);


}


void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);
	//glUniform1f(loc, time);
	//glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	
	colorloc = glGetAttribLocation(p,"vColor"); 
	for (int i=-1; i<2; i++)
		for (int j=-1; j<2; j++)
			for (int k=-1;k<2; k++){
			glBegin(GL_LINE_STRIP);
				glVertexAttrib3f(colorloc,1,1, 0);  
				glVertex3f(i,j,k);
				glVertexAttrib3f(colorloc,1,0,0);  
				glVertex3f(i+1,j+1,k+1);
				glVertexAttrib3f(colorloc,0,0,1);  
				glVertex3f(i+1,j-2,k-3);
			glEnd();
	}
	
	time+=0.1;
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
} 

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void setShaders() {

	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	//f2 = glCreateShader(GL_FRAGMENT_SHADER);


	vs = textFileRead("toon.vert");
	fs = textFileRead("toon.frag");
	//fs2 = textFileRead("toon2.frag");

	const char * ff = fs;
	//const char * ff2 = fs2;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);
	//glShaderSource(f2, 1, &ff2,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);
	//glCompileShader(f2);

	printShaderInfoLog(v);
	printShaderInfoLog(f);
	//printShaderInfoLog(f2);

	p = glCreateProgram();
	glAttachShader(p,f);
	//glAttachShader(p,f2);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
	//colorloc =  glGetUniformLocation(p, "mycolor");
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("MM 2004-05");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0,1.0,1.0,1.0);
//	glEnable(GL_CULL_FACE);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
	setShaders();

	glutMainLoop();

	// just for compatibiliy purposes
	return 0;
}
