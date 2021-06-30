#include<bits/stdc++.h>
using namespace std;
#define pi (2*acos(0.0))
#include <windows.h>
#include <glut.h>


struct point
{
	double x,y,z;
};
typedef struct point Vector3D;

class Ray
{
public:
    Vector3D start;
    Vector3D dir;
    Ray(Vector3D &eye, Vector3D &pixel);


};

class Object
{
public:
    Vector3D reference_point; // should have x, y, z
    double color[3];
    double coEfficients[4] ;// reflection coefficients
    int shine; // exponent term of specular component
    virtual void draw();
    virtual double intersect(Ray &r, double* color, int recLevel);
    void setColor();
    void setShine();
    void setCoEfficients();
    Object();
    virtual ~Object();

protected:

private:
};


class Sphere:public Object{
public:
    Sphere();
    double radius;
//    Sphere(center, radius);
    void draw();
    double intersect(Ray &r, double* color, int recLevel);

};

class Triangle:public Object{
public:
    Vector3D p1,p2,p3;
    Triangle();
//    Sphere(center, radius);
    void draw();
    double intersect(Ray &r, double* color, int recLevel);

};

class General:public Object{
public:
    double height, width, length;
    vector<double> constants;
    General();
//    Sphere(center, radius);
    void draw();
    double intersect(Ray &r, double* color, int recLevel);

};


class Floor: public Object{
public:
    double tileLength;
    double width;
    Vector3D color1 {1.0,1.0,1.0};
    Vector3D color2 {0.874, 0.455, 0.318}; //burnt sienna rgb(233,116,81)
    Floor(double floorWidth = 1000, double tileWidth = 20){
        reference_point={-floorWidth/2,-floorWidth/2,0};
        tileLength=tileWidth;
        width = floorWidth;
    }
    double intersect(Ray &r, double* color, int recLevel);
    void draw();
};


class Light{
public:
    Vector3D lightPos;
    double color[3];

};




extern vector <Object*> objects;
extern vector <Light*> lights;






Object::Object()
{
    //ctor
    //cout<<"creating from Object"<<endl;

}

Object::~Object()
{
//    dtor
//    cout<<"destroying Object"<<endl;
}

void Object::draw()
{
    cout<<"drawing Object"<<endl;
}

void Object::setColor()
{

}

void Object::setCoEfficients()
{

}

void Object::setShine()
{


}

double Object::intersect(Ray& r, double* color, int recLevel)
{
    return 0;
}

Sphere::Sphere()
{
    //cout<<"creating from Sphere"<<endl;
}



void Sphere::draw()
{
//    cout<<"drawing Sphere"<<endl;
    struct point points[100][100];
	int i,j;
	double h,r;
	int stacks = 50;
	int slices = 50;
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
	glTranslatef(reference_point.x,reference_point.y,reference_point.z);
	glColor3f(color[0], color[1], color[2]);
	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(-reference_point.x,-reference_point.y,-reference_point.z);
//        glColor3f(1.0, 1.0, 1.0);
//		glBegin(GL_LINES);{
//		    glColor3f(1.0, 0, 0);
//			glVertex3f( 1000,0,0);
//			glVertex3f(-1000,0,0);
//            glColor3f(0, 1.0, 0);
//			glVertex3f(0,-1000,0);
//			glVertex3f(0, 1000,0);
//            glColor3f(0, 0, 1.0);
//			glVertex3f(0,0, 1000);
//			glVertex3f(0,0,-1000);
//		}glEnd();
//	cout<<"drawing sphere finished"<<endl;
}


double Sphere::intersect(Ray& r, double* color, int recLevel)
{
    return 0;
}
//void drawSphere(double radius,int slices,int stacks)
//{
//
//}

Triangle::Triangle()
{
    //cout<<"creating from Triangle"<<endl;
}

void Triangle::draw()
{
    //cout<<"drawing Triangle"<<endl;
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
    }
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
}

double Triangle::intersect(Ray& r, double* color, int recLevel)
{
    return 0;
}

General::General()
{
    //cout<<"creating from General"<<endl;
}

void General::draw()
{
    //cout<<"drawing General"<<endl;
}

double General::intersect(Ray& r, double* color, int recLevel)
{
    return 0;
}

void Floor::draw(){
// write codes for drawing a checkerboard-like
// floor with alternate colors on adjacent tiles

    int rows = width/tileLength;
    int cols = rows; // square
    bool white = true;
    for(int i=0;i<rows;i++)
    {
        if(i%2==0)
        {
            white = true;
        }
        else
        {
            white = false;
        }
        for(int j=0;j<cols;j++)
        {
            Vector3D bottomLeft = {reference_point.x + j * tileLength ,reference_point.y + i * tileLength , -20};
            Vector3D bottomRight = {bottomLeft.x+tileLength,bottomLeft.y,bottomLeft.z};
            Vector3D topLeft = {bottomLeft.x, bottomLeft.y + tileLength, bottomLeft.z};
            Vector3D topRight = {bottomLeft.x + tileLength, bottomLeft.y + tileLength, bottomLeft.z};



            if(white)
            {
                glColor3f(color1.x, color1.y, color1.z);
                white = false;
            }
            else
            {
                glColor3f(color2.x, color2.y, color2.z);
                white = true;
            }
            glBegin(GL_QUADS);{
                glVertex3f(topRight.x, topRight.y,topRight.z);
                glVertex3f(bottomRight.x, bottomRight.y,bottomRight.z);
                glVertex3f(bottomLeft.x, bottomLeft.y,bottomLeft.z);
                glVertex3f(topLeft.x, topLeft.y,topLeft.z);
            }glEnd();

        }
    }
    glColor3f(1.0, 1.0, 1.0);
}

double Floor::intersect(Ray& r, double* color, int recLevel)
{
    return 0;
}


Ray::Ray(Vector3D &eye, Vector3D &pixel)
{
    start = eye;
    dir.x = pixel.x-eye.x;
    dir.y = pixel.y-eye.y;
    dir.z = pixel.z-eye.z;
    double magnitude = sqrt(dir.x*dir.x+dir.y*dir.y+dir.z*dir.z);
    dir.x = dir.x/magnitude;
    dir.y = dir.y/magnitude;
    dir.z = dir.z/magnitude;

}

void printSphere(Sphere *s)
{
    cout<<"Object Type: Sphere"<<endl;
    cout<<"ref point: "<<s->reference_point.x<<" "<<s->reference_point.y<<" "<<s->reference_point.z<<endl;
    cout<<"radius: "<<s->radius<<endl;
    cout<<"color: "<<s->color[0]<<" "<<s->color[1]<<" "<<s->color[2]<<endl;
    cout<<"co-eff: "<<s->coEfficients[0]<<" "<<s->coEfficients[1]<<" "<<s->coEfficients[2]<<" "<<s->coEfficients[3]<<endl;
    cout<<"shininess: "<<s->shine<<endl;

}
Sphere* readSphere(ifstream &inputFile)
{
    Sphere *ret = new Sphere;
    inputFile>>ret->reference_point.x>>ret->reference_point.y>>ret->reference_point.z;
    inputFile>>ret->radius;
    inputFile>>ret->color[0]>>ret->color[1]>>ret->color[2];
    inputFile>>ret->coEfficients[0]>>ret->coEfficients[1]>>ret->coEfficients[2]>>ret->coEfficients[3];
    inputFile>>ret->shine;
    return ret;
}

void printTriangle(Triangle *s)
{
    cout<<"Object Type: Triangle"<<endl;
    cout<<"Point 1: "<<s->p1.x<<" "<<s->p1.y<<" "<<s->p1.z<<endl;
    cout<<"Point 2: "<<s->p2.x<<" "<<s->p2.y<<" "<<s->p2.z<<endl;
    cout<<"Point 3: "<<s->p3.x<<" "<<s->p3.y<<" "<<s->p3.z<<endl;
    cout<<"color: "<<s->color[0]<<" "<<s->color[1]<<" "<<s->color[2]<<endl;
    cout<<"co-eff: "<<s->coEfficients[0]<<" "<<s->coEfficients[1]<<" "<<s->coEfficients[2]<<" "<<s->coEfficients[3]<<endl;
    cout<<"shininess: "<<s->shine<<endl;

}
Triangle* readTriangle(ifstream &inputFile)
{
    Triangle *ret = new Triangle;
    inputFile>>ret->p1.x>>ret->p1.y>>ret->p1.z;
    inputFile>>ret->p2.x>>ret->p2.y>>ret->p2.z;
    inputFile>>ret->p3.x>>ret->p3.y>>ret->p3.z;
    inputFile>>ret->color[0]>>ret->color[1]>>ret->color[2];
    inputFile>>ret->coEfficients[0]>>ret->coEfficients[1]>>ret->coEfficients[2]>>ret->coEfficients[3];
    inputFile>>ret->shine;
    return ret;
}

void printGeneral(General *s)
{
    cout<<"Object Type: General"<<endl;
    cout<<"constants : ";
    for(int i=0;i<s->constants.size();i++)
    {
        cout<<s->constants[i]<<" ";
    }
    cout<<endl;
    cout<<"ref point: "<<s->reference_point.x<<" "<<s->reference_point.y<<" "<<s->reference_point.z<<endl;
    cout<<"dimension: "<<s->length<<" "<<s->width<<" "<<s->height<<endl;
    cout<<"color: "<<s->color[0]<<" "<<s->color[1]<<" "<<s->color[2]<<endl;
    cout<<"co-eff: "<<s->coEfficients[0]<<" "<<s->coEfficients[1]<<" "<<s->coEfficients[2]<<" "<<s->coEfficients[3]<<endl;
    cout<<"shininess: "<<s->shine<<endl;

}
General* readGeneral(ifstream &inputFile)
{
    General *ret = new General;
    string constLine;
    getline(inputFile,constLine);
    stringstream ss(constLine);
    while(!ss.eof())
    {
        double t;
        ss>>t;
        ret->constants.push_back(t);
    }
    inputFile>>ret->reference_point.x>>ret->reference_point.y>>ret->reference_point.z;
    inputFile>>ret->length>>ret->width>>ret->height;
    inputFile>>ret->color[0]>>ret->color[1]>>ret->color[2];
    inputFile>>ret->coEfficients[0]>>ret->coEfficients[1]>>ret->coEfficients[2]>>ret->coEfficients[3];
    inputFile>>ret->shine;
    return ret;
}

void printLight(Light *s)
{
    cout<<"Object Type: Light"<<endl;
    cout<<"light position: "<<s->lightPos.x<<" "<<s->lightPos.y<<" "<<s->lightPos.z<<endl;
    cout<<"color: "<<s->color[0]<<" "<<s->color[1]<<" "<<s->color[2]<<endl;

}
Light* readLight(ifstream &inputFile)
{
    Light *ret = new Light;
    inputFile>>ret->lightPos.x>>ret->lightPos.y>>ret->lightPos.z;
    inputFile>>ret->color[0]>>ret->color[1]>>ret->color[2];
    return ret;
}






