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
//    Sphere(center, radius);
    void draw();
};

class Light{
public:
    Vector3D lightPos;
    double color[3];
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






