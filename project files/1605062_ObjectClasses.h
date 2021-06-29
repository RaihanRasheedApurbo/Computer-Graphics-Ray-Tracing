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

};

class Triangle:public Object{
public:
    Vector3D p1,p2,p3;
    Triangle();
//    Sphere(center, radius);
    void draw();

};

class General:public Object{
public:
    double height, width, length;
    vector<double> constants;
    General();
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
    double length;
    Floor(double floorWidth = 1000, double tileWidth = 20){
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
    //cout<<"creating from Object"<<endl;

}

Object::~Object()
{
    //dtor
    //cout<<"destroying Object"<<endl;
}

void Object::draw()
{
    //cout<<"drawing Object"<<endl;
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
    //cout<<"creating from Sphere"<<endl;
}

//Sphere::Sphere(center, radius)
//{
//
//}

void Sphere::draw()
{
    //cout<<"drawing Sphere"<<endl;
}


Triangle::Triangle()
{
    //cout<<"creating from Triangle"<<endl;
}

void Triangle::draw()
{
    //cout<<"drawing Triangle"<<endl;
}


General::General()
{
    //cout<<"creating from General"<<endl;
}

void General::draw()
{
    //cout<<"drawing General"<<endl;
}

void printSphere(Sphere &s)
{
    cout<<"Object Type: Sphere"<<endl;
    cout<<"ref point: "<<s.reference_point.x<<" "<<s.reference_point.y<<" "<<s.reference_point.z<<endl;
    cout<<"radius: "<<s.radius<<endl;
    cout<<"color: "<<s.color[0]<<" "<<s.color[1]<<" "<<s.color[2]<<endl;
    cout<<"co-eff: "<<s.coEfficients[0]<<" "<<s.coEfficients[1]<<" "<<s.coEfficients[2]<<" "<<s.coEfficients[3]<<endl;
    cout<<"shininess: "<<s.shine<<endl;

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

void printTriangle(Triangle &s)
{
    cout<<"Object Type: Triangle"<<endl;
    cout<<"Point 1: "<<s.p1.x<<" "<<s.p1.y<<" "<<s.p1.z<<endl;
    cout<<"Point 2: "<<s.p2.x<<" "<<s.p2.y<<" "<<s.p2.z<<endl;
    cout<<"Point 3: "<<s.p3.x<<" "<<s.p3.y<<" "<<s.p3.z<<endl;
    cout<<"color: "<<s.color[0]<<" "<<s.color[1]<<" "<<s.color[2]<<endl;
    cout<<"co-eff: "<<s.coEfficients[0]<<" "<<s.coEfficients[1]<<" "<<s.coEfficients[2]<<" "<<s.coEfficients[3]<<endl;
    cout<<"shininess: "<<s.shine<<endl;

}
Triangle readTriangle(ifstream &inputFile)
{
    Triangle ret;
    inputFile>>ret.p1.x>>ret.p1.y>>ret.p1.z;
    inputFile>>ret.p2.x>>ret.p2.y>>ret.p2.z;
    inputFile>>ret.p3.x>>ret.p3.y>>ret.p3.z;
    inputFile>>ret.color[0]>>ret.color[1]>>ret.color[2];
    inputFile>>ret.coEfficients[0]>>ret.coEfficients[1]>>ret.coEfficients[2]>>ret.coEfficients[3];
    inputFile>>ret.shine;
    return ret;
}

void printGeneral(General &s)
{
    cout<<"Object Type: General"<<endl;
    cout<<"constants : ";
    for(int i=0;i<s.constants.size();i++)
    {
        cout<<s.constants[i]<<" ";
    }
    cout<<endl;
    cout<<"ref point: "<<s.reference_point.x<<" "<<s.reference_point.y<<" "<<s.reference_point.z<<endl;
    cout<<"dimension: "<<s.length<<" "<<s.width<<" "<<s.height<<endl;
    cout<<"color: "<<s.color[0]<<" "<<s.color[1]<<" "<<s.color[2]<<endl;
    cout<<"co-eff: "<<s.coEfficients[0]<<" "<<s.coEfficients[1]<<" "<<s.coEfficients[2]<<" "<<s.coEfficients[3]<<endl;
    cout<<"shininess: "<<s.shine<<endl;

}
General readGeneral(ifstream &inputFile)
{
    General ret;
    string constLine;
    getline(inputFile,constLine);
    stringstream ss(constLine);
    while(!ss.eof())
    {
        double t;
        ss>>t;
        ret.constants.push_back(t);
    }
    inputFile>>ret.reference_point.x>>ret.reference_point.y>>ret.reference_point.z;
    inputFile>>ret.length>>ret.width>>ret.height;
    inputFile>>ret.color[0]>>ret.color[1]>>ret.color[2];
    inputFile>>ret.coEfficients[0]>>ret.coEfficients[1]>>ret.coEfficients[2]>>ret.coEfficients[3];
    inputFile>>ret.shine;
    return ret;
}

void printLight(Light &s)
{
    cout<<"Object Type: Light"<<endl;
    cout<<"light position: "<<s.lightPos.x<<" "<<s.lightPos.y<<" "<<s.lightPos.z<<endl;
    cout<<"color: "<<s.color[0]<<" "<<s.color[1]<<" "<<s.color[2]<<endl;

}
Light readLight(ifstream &inputFile)
{
    Light ret;
    inputFile>>ret.lightPos.x>>ret.lightPos.y>>ret.lightPos.z;
    inputFile>>ret.color[0]>>ret.color[1]>>ret.color[2];
    return ret;
}






