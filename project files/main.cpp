#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <windows.h>
#include <glut.h>
#include <bits/stdc++.h>
#include "1605062_ObjectClasses.h"
#include "bitmap_image.hpp"
using namespace std;
#define pi (2*acos(0.0))


vector <Object*> objects;
vector <Light*> lights;
double globalColor[3];

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double shiftingAmount;
double xAmount;
double yAmount;
double zAmount;
double rotationAmount;



struct point pos;
struct point u;
struct point l;
struct point r;

void capture();

struct point vectorSum(struct point p1,struct point p2)
{
    struct point ans;
    ans.x = p1.x+p2.x;
    ans.y = p1.y+p2.y;
    ans.z = p1.z+p2.z;
    return ans;

}

struct point negateVector(struct point p1)
{
    struct point ans;
    ans.x = -p1.x;
    ans.y = -p1.y;
    ans.z = -p1.z;
    return ans;

}

struct point rotateVector(struct point p1, struct point p2, double angle)
{
    struct point ans;

    ans.x = p1.x * cos(angle) + p2.x * sin(angle);
    ans.y = p1.y * cos(angle) + p2.y * sin(angle);
    ans.z = p1.z * cos(angle) + p2.z * sin(angle);

    double magnitude = sqrt(ans.x*ans.x+ans.y*ans.y+ans.z*ans.z);
    ans.x /= magnitude;
    ans.y /= magnitude;
    ans.z /= magnitude;

    return ans;


}

//void printPoint(struct point p)
//{
//
//    cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;
//}

void counterClockRotateOFLookVectorINYAxis()
{
    struct point perpendicular;
    // i  j  k
    // l1 l2 l3
    // u1 u2 u3
    // = i*(-l3) - j(0) + k(l1)
    perpendicular.x = l.y*u.z - l.z*u.y;
    perpendicular.y = -(l.x*u.z-l.z*u.x);
    perpendicular.z = l.x*u.y-l.y*u.x;
    printPoint(l);
    l.x = l.x * cos(rotationAmount) + perpendicular.x * sin(rotationAmount);
    l.y = l.y * cos(rotationAmount) + perpendicular.y * sin(rotationAmount);
    l.z = l.z * cos(rotationAmount) + perpendicular.z * sin(rotationAmount);

    printPoint(l);

    double magnitude = sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
    l.x /= magnitude;
    l.y /= magnitude;
    l.z /= magnitude;

    printPoint(l);
    cout<<endl;
}

void clockRotateOFLookVectorINYAxis()
{
    struct point perpendicular;
    // i  j  k
    // l1 l2 l3
    // 0  1  0
    // = i*(-l3) - j(0) + k(l1)
    perpendicular.x = -(l.y*u.z - l.z*u.y);
    perpendicular.y = (l.x*u.z-l.z*u.x);
    perpendicular.z = -(l.x*u.y-l.y*u.x);

    printPoint(l);

    l.x = l.x * cos(rotationAmount) + perpendicular.x * sin(rotationAmount);
    l.y = l.y * cos(rotationAmount) + perpendicular.y * sin(rotationAmount);
    l.z = l.z * cos(rotationAmount) + perpendicular.z * sin(rotationAmount);

    printPoint(l);

    double magnitude = sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
    l.x /= magnitude;
    l.y /= magnitude;
    l.z /= magnitude;

    printPoint(l);
    cout<<endl;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0, 0);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);
            glColor3f(0, 1.0, 0);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);
            glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
//    glColor3f(1,0,0);
//    drawSquare(20);
//
//    glRotatef(angle,0,0,1);
//    glTranslatef(110,0,0);
//    glRotatef(2*angle,0,0,1);
//    glColor3f(0,1,0);
//    drawSquare(15);
//
//    glPushMatrix();
//    {
//        glRotatef(angle,0,0,1);
//        glTranslatef(60,0,0);
//        glRotatef(2*angle,0,0,1);
//        glColor3f(0,0,1);
//        drawSquare(10);
//    }
//    glPopMatrix();
//
//    glRotatef(3*angle,0,0,1);
//    glTranslatef(40,0,0);
//    glRotatef(4*angle,0,0,1);
//    glColor3f(1,1,0);
//    drawSquare(5);


//    glColor3f(1,0,0);
//    drawSphere(100,50,50);
}

void keyboardListener(unsigned char key, int x,int y){
    struct point prevLookVector = l;
    struct point prevUpVector = u;
//    struct point prevRightVector = r;
	switch(key){

		case '1':
//			drawgrid=1-drawgrid;

            l = rotateVector(l,negateVector(r),rotationAmount);
            r = rotateVector(r,prevLookVector,rotationAmount);
			break;

        case '2':
//			drawgrid=1-drawgrid;
            l = rotateVector(l,r,rotationAmount);
            r = rotateVector(r,negateVector(prevLookVector),rotationAmount);
			break;

        case '3':
//			drawgrid=1-drawgrid;
            l = rotateVector(l,u,rotationAmount);
            u = rotateVector(u,negateVector(prevLookVector),rotationAmount);
			break;

        case '4':
//			drawgrid=1-drawgrid;
            l = rotateVector(l,negateVector(u),rotationAmount);
            u = rotateVector(u,prevLookVector,rotationAmount);
			break;


        case '5':
//			drawgrid=1-drawgrid;
            u = rotateVector(u,negateVector(r),rotationAmount);
            r = rotateVector(r,prevUpVector,rotationAmount);
			break;

        case '6':
//			drawgrid=1-drawgrid;
            u = rotateVector(u,r,rotationAmount);
            r = rotateVector(r,negateVector(prevUpVector),rotationAmount);
            break;

        case '0':
            capture();
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			cameraHeight -= 3.0;

            pos = vectorSum(pos,negateVector(l));
			break;
		case GLUT_KEY_UP:		// up arrow key
//			cameraHeight += 3.0;
            pos = vectorSum(pos,l);
			break;

		case GLUT_KEY_RIGHT:
//			cameraAngle += 0.03;
            pos = vectorSum(pos,r);
			break;
		case GLUT_KEY_LEFT:
//			cameraAngle -= 0.03;
            pos = vectorSum(pos,negateVector(r));
			break;

		case GLUT_KEY_PAGE_UP:
		    pos = vectorSum(pos,u);
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos = vectorSum(pos,negateVector(u));
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

//	gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//	gluLookAt(0,0,200,	0,0,0,	0,1,0);
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

//	drawAxes();
//	cout<<objects[0]->coEfficients[0]<<endl;
//	objects[0]->draw();
//	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

//    drawSS();
//    cout<<"hi"<<endl;
//    cout<<objects.size()<<endl;
//    cout<<objects[0]->color[0]<<endl;
//    objects[0]->draw();
    drawAxes();
//    cout<<objects.size();
    for(int i=0;i<objects.size();i++)
    {
        if(true)
        {
//            cout<<"drawing"<<endl;
            objects[i]->draw();
        }

    }

    for(int i=0;i<lights.size();i++)
    {
        lights[i]->draw();
    }
//    objects[3]->draw();
//    objects[0]->draw();
//    objects[objects.size()-1]->draw();

//    t->draw();
//    drawSphere(10,10,10);
//    drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
//	angle+=0.5;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

double viewAngle = 80; // in degrees used in init and capture

void init(){
	//codes for initialization

    drawaxes = 1;
    shiftingAmount = 5;
    rotationAmount = (pi/18);
    xAmount = 0;
    yAmount = 0;
    zAmount = 0;

//
//    pos = {100,100,0};
//    u = {0,0,1};
//    r = {-1/sqrt(2),1/sqrt(2),0};
//    l = {-1/sqrt(2),-1/sqrt(2),0};

//
    pos = {30.1729,1.46954,6};
    u = {0,0,1};
    r = {-0.258819,-0.965926,0};
    l = {0.965926,-0.258819,0};
//capturing


//capturing
//du: 1 dv: 1 dist: 297.938
//width: 500 height: 500 dimension: 500
//eye:
//30.1729 1.46954 6
//look:
//0.965926 -0.258819 0
//up:
//0 0 1
//right:
//-0.258819 -0.965926 0
//topLeft:
//382.535 165.356 255.5
//0
//after phong: 0 0 0
//capture completed




	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(viewAngle,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int levelOfRecursion = 0;
double dimension = 0;
int numberOfObject = 0;
double windowHeight = 500;
double windowWidth = 500; // used in main and in capture function

void capture()
{
    globalColor[0] = 0;
    globalColor[1] = 0;
    globalColor[2] = 0;

    levelOfRecursion = 1;
    dimension = 500;
    bitmap_image image(dimension,dimension);
    cout<<"capturing"<<endl;
//    cout<<tan(pi*viewAngle/180/2)<<" "<<(windowHeight/2.0)<<endl;
    double planeDistance = (windowHeight/2.0)/tan(pi*viewAngle/180/2);
    Vector3D topLeft;
    topLeft.x = pos.x + l.x *planeDistance - r.x*windowWidth/2 + u.x*windowHeight/2;
    topLeft.y = pos.y + l.y *planeDistance - r.y*windowWidth/2 + u.y*windowHeight/2;
    topLeft.z = pos.z + l.z *planeDistance - r.z*windowWidth/2 + u.z*windowHeight/2;
    double du = windowWidth/dimension;
    double dv = windowHeight/dimension;

    topLeft.x = topLeft.x + r.x * (0.5*du) - u.x * (0.5*dv);
    topLeft.y = topLeft.y + r.y * (0.5*du) - u.y * (0.5*dv);
    topLeft.z = topLeft.z + r.z * (0.5*du) - u.z * (0.5*dv);

    cout<<"du: "<<du<<" dv: "<<dv<<" dist: "<<planeDistance<<endl;
    cout<<"width: "<<windowWidth<<" height: "<<windowHeight<<" dimension: "<<dimension<<endl;
    cout<<"eye:"<<endl;
    printPoint(pos);
    cout<<"look:"<<endl;
    printPoint(l);
    cout<<"up:"<<endl;
    printPoint(u);
    cout<<"right:"<<endl;
    printPoint(r);
    cout<<"topLeft: "<<endl;
    printPoint(topLeft);


    for(int i=0;i<dimension;i++)
    {
        for(int j=0;j<dimension;j++)
        {
            Vector3D curPixel;
            curPixel.x = topLeft.x - i*dv*u.x + j*du*r.x;
            curPixel.y = topLeft.y - i*dv*u.y + j*du*r.y;
            curPixel.z = topLeft.z - i*dv*u.z + j*du*r.z;

            Ray r(pos,curPixel);
            double *color = new double[3];
            double *minColor = new double[3];
//            if(i%256==0 && j%256==0)
//            {
//                cout<<i<<" "<<j<<endl;
//                printPoint(curPixel);
//            }
//            if(i==dimension/2 && j==dimension/2)
//            {
//                cout<<"kill meh"<<endl;
//            }
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            minColor[0] = 0;
            minColor[1] = 0;
            minColor[2] = 0;

            double tmin = INT_MAX;
            for(int k=0;k<objects.size();k++)
            {
//                if(k>=1)
//                {
//                    break;
//                }
                Object *curObj = objects[k];
                if(curObj->type == sphere)
                {
                    if(i==250 && j == 350)
                    {
                        cout<<curObj->type<<endl;
                    }
                    double t = curObj->intersect(r,color,levelOfRecursion);
                    if(t>=0)
                    {
                        if(tmin>t)
                        {

                            tmin = min(tmin,t);
                            if(curObj->type != board)
                            {
                                minColor[0] = color[0];
                                minColor[1] = color[1];
                                minColor[2] = color[2];
                            }
                            else
                            {
                                minColor[0] = color[0];
                                minColor[1] = color[1];
                                minColor[2] = color[2];
//                                cout<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
                            }


                        }

                    }
//                    break;

                }

            }
            if(tmin!=INT_MAX)
            {
//                if(i==250 && j == 350)
//                {
//                    cout<<i<<" "<<j<<" "<<tmin<<endl;
//
//                }

                image.set_pixel(j,i,minColor[0]*255,minColor[1]*255,minColor[2]*255);
            }
//            else
//            {
//                image.set_pixel(j,i,255,255,255);
//            }
            delete[] minColor;
            delete[] color;

        }
    }
    cout<<"after phong: "<<globalColor[0]<< " " << globalColor[1] << " " << globalColor[2]<<endl;
    image.save_image("test.bmp");
    cout<<"capture completed"<<endl;

}


void testData()
{
//    Sphere* t = new Sphere;
//    t->reference_point.x = 10;
//    t->reference_point.y = 10;
//    t->reference_point.z = 10;
//    t->radius = 10;
//    t->color[0] - 0.0;
//    t->color[1] - 1.0;
//    t->color[2] - 0.0;
//    Vector3D e = {0,0,0};
//    Vector3D d = {-1,-1,-1};
//    Ray r(e,d);
////    r.start = {20,0,0};
////    r.dir = {-1,0,0};
//    double *color;
//    double result = t->intersect(r,color,4);
//    cout<<result;

    Triangle* t = new Triangle;
    t->p1 = {50,30,0};
    t->p2 = {70,60,0};
    t->p3 = {50,45,50};
    t->color[0] = 0.0;
    t->color[1] = 1.0;
    t->color[2] = 0.0;
//    Vector3D e = {154,-23,12};
//    Vector3D d = {10,2.5,0};
//    Ray r(e,d);
////    r.start = {20,0,0};
////    r.dir = {-1,0,0};
//    double *color;
//    double result = t->intersect(r,color,4);
//    cout<<result<<endl;
//    cout<<(result>=0)<<endl;
    objects.push_back(t);


}

void loadData()
{
    cout<<"kill meh"<<endl;
    string file1 = "scene.txt";
    ifstream inputFile(file1);


    inputFile>>levelOfRecursion>>dimension>>numberOfObject;
    cout<<levelOfRecursion<<" "<<dimension<<" "<<numberOfObject<<endl;

    //after reading the variable numberofObject rest of the blank line
    // is needed to be parsed before going inside loop and read object
    // informations. so next two lines do that.
    string line;
    getline(inputFile,line);

    for(int i=0;i<numberOfObject;i++)
    {
        getline(inputFile,line);
        cout<<line<<endl;
        if(line.find("sphere") != std::string::npos)
        {
            Sphere *s = readSphere(inputFile);
            //printSphere(s);
            objects.push_back(s);
        }
        else if(line.find("triangle") != std::string::npos)
        {
            Triangle *s = readTriangle(inputFile);
            //printTriangle(s);
            objects.push_back(s);
        }
        else if(line.find("general") != std::string::npos)
        {
            General *s = readGeneral(inputFile);
            //printGeneral(s);
            objects.push_back(s);
        }
        else
        {
            cout<<"something wrong"<<endl;
            break;
        }
        // after reading shininess we have two newline
        getline(inputFile,line);
        getline(inputFile,line);
    }
    int numberOfLightSources = 0;
    inputFile>>numberOfLightSources;
    getline(inputFile,line);
    for(int i=0;i<numberOfLightSources;i++)
    {
        Light *t = readLight(inputFile);
        //printLight(t);
        lights.push_back(t);
    }
//    cout<<inputFile.eof()<<endl;

//    int t1,t2,t3;
//    inputFile>>t1;
//    inputFile>>t2>>t3;
//    cout<<"kill me"<<endl;
//    cout<<t1<<" "<<t2<<" "<<t3<<endl;
    printSphere((Sphere *)objects[0]);
    printTriangle((Triangle *)objects[3]);
    objects.push_back((Object*) new Floor());

//    capture();


}



int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(windowHeight, windowWidth);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);


    loadData();
//    testData();
	glutMainLoop();		//The main loop of OpenGL



	return 0;
}
