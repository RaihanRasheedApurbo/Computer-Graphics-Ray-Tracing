#include<bits/stdc++.h>
using namespace std;



struct point
{
	double x,y,z;
};
typedef struct point Vector3D;

class Object
{
public:
    Vector3D reference_point; // should have x, y, z
    double height, width, length;
    double color[3];
    double coEfficients[4] ;// reflection coefficients
    int shine; // exponent term of specular component
    virtual void draw();
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
    void input();
};

class Triangle:public Object{
public:
    Triangle();
//    Sphere(center, radius);
    void draw();
    void input();
};

class Shape:public Object{
public:

    Shape();
//    Sphere(center, radius);
    void draw();
    void input();
};

class Light{
public:
    Vector3D lightPos;
    double color[3];
    void input();
};


extern vector <Object> objects;
extern vector <Light> lights;


class Floor: public Object{
public:

    Floor(double floorWidth, double tileWidth){
        reference_point={-floorWidth/2,-floorWidth/2,0};
        length=tileWidth;
    }
    void draw(){
    // write codes for drawing a checkerboard-like
    // floor with alternate colors on adjacent tiles
    }
};


Object::Object()
{
    //ctor
    cout<<"creating from object"<<endl;

}

Object::~Object()
{
    //dtor
    cout<<"destroying object"<<endl;
}

void Object::draw()
{
    cout<<"drawing objects"<<endl;
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



Sphere::Sphere()
{
    cout<<"creating from sphere"<<endl;
}

//Sphere::Sphere(center, radius)
//{
//
//}

void Sphere::draw()
{
    cout<<"drawing Sphere"<<endl;
}


void printSphere(Sphere &s)
{
    cout<<s.reference_point.x<<" "<<s.reference_point.y<<" "<<s.reference_point.z<<endl;
    cout<<s.radius;
    cout<<s.color[0]<<" "<<s.color[1]<<" "<<s.color[2]<<endl;
    cout<<s.coEfficients[0]<<" "<<s.coEfficients[1]<<" "<<s.coEfficients[2]<<" "<<s.coEfficients[3]<<endl;
    cout<<s.shine<<endl;

}
Sphere readSphere(ifstream &inputFile)
{
    Sphere ret;
    inputFile>>ret.reference_point.x>>ret.reference_point.y>>ret.reference_point.z;
    inputFile>>ret.radius;
    inputFile>>ret.color[0]>>ret.color[1]>>ret.color[2];
    inputFile>>ret.coEfficients[0]>>ret.coEfficients[1]>>ret.coEfficients[2]>>ret.coEfficients[3];
    inputFile>>ret.shine;
    return ret;
}






