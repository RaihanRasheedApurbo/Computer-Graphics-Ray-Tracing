#include<bits/stdc++.h>
using namespace std;
#define pi (2*acos(0.0))
#include <windows.h>
#include <glut.h>
#define PRECISION_FIX 0.001

enum ObjectType {sphere, triangle, plane, board};

void printPoint(struct point p);
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
    enum ObjectType type;
    Vector3D reference_point; // should have x, y, z
    double color[3];
    double coEfficients[4] ;// reflection coefficients
    int shine; // exponent term of specular component
    virtual void draw();
    virtual double intersect(Ray &r, double* color, int recLevel);
    Object();
    virtual ~Object();
    void getPhongLighting(Ray &r, double t, double* color);
    virtual Vector3D getNormal(Vector3D &intersectPoint);
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
//    void getPhongLighting(Ray &r, double t, double* color);
    Vector3D getNormal(Vector3D &intersectPoint);

};

class Triangle:public Object{
public:
    Vector3D p1,p2,p3;
    Triangle();
//    Sphere(center, radius);
    void draw();
    double intersect(Ray &r, double* color, int recLevel);
    bool insideTriangle(Vector3D &intersection);
    Vector3D getNormal(Vector3D &intersectPoint);

};

class General:public Object{
public:
    double height, width, length;
    vector<double> constants;
    General();
//    Sphere(center, radius);
    void draw();
    double intersect(Ray &r, double* color, int recLevel);
    bool insideGeneral(Vector3D &p);
    Vector3D getNormal(Vector3D &intersectPoint);
//    void getPhongLighting(Ray &r, double t, double* color);

};


class Floor: public Object{
public:
    double tileLength;
    double width;
    Vector3D normal {0,0,1};
    double color1[3] = {1.0,1.0,1.0};
//    double color2[3] = {0.874, 0.455, 0.318}; //burnt sienna rgb(233,116,81)
    double color2[3] = {0, 0, 0};
    Floor(double floorWidth = 1000, double tileWidth = 20);
    bool insideFloor(Vector3D &intersect);
    double intersect(Ray &r, double* color, int recLevel);
    void draw();
    Vector3D getNormal(Vector3D &intersectPoint);
    void getPhongLighting(Ray &r, double t, double* color, bool color1Chosen);
};


class Light{
public:
    Vector3D lightPos;
    double color[3];
    void draw();

};




extern vector <Object*> objects;
extern vector <Light*> lights;
extern int levelOfRecursion;





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



double Object::intersect(Ray& r, double* color, int recLevel)
{
    return -5;
}

Vector3D Object::getNormal(Vector3D &intersectPoint)
{
    cout<<"shouldn't get called"<<endl;
    Vector3D ret;
    return ret;
}

void Object::getPhongLighting(Ray &r, double t, double* color)
{
    Vector3D intersectionPoint = {r.start.x+t*r.dir.x, r.start.y+t*r.dir.y, r.start.z+t*r.dir.z};
    Vector3D normalAtIntersect = getNormal(intersectionPoint);
    // adding epsilon to elevate intersection point...
    double epsilon = 2*0.01;

//    cout<<"before phong: "<<color[0]<< " " << color[1] << " " << color[2]<<endl;
    for(int i=0;i<lights.size();i++)
    {
        if(true)
        {
            Light *curLight = lights[i];
//            Light &tk = *(lights[i]);
//            Object &tkk = *(this);
            Ray rl(intersectionPoint,curLight->lightPos);
            rl.start = {intersectionPoint.x + epsilon*rl.dir.x, intersectionPoint.y + epsilon*rl.dir.y, intersectionPoint.z + epsilon*rl.dir.z};
//            double lightT = (curLight->lightPos.x-rl.start.x)/rl.dir.x; //    Rox + t * Rdirl = light.x
            Vector3D distanceVec = {curLight->lightPos.x-intersectionPoint.x, curLight->lightPos.y-intersectionPoint.y, curLight->lightPos.z-intersectionPoint.z};
            double lightT = sqrt(distanceVec.x*distanceVec.x + distanceVec.y*distanceVec.y + distanceVec.z*distanceVec.z);

            double *dummyColor = new double[3];
            bool obsecured = false;
            for(int j=0;j<objects.size();j++)
            {
                double t = objects[j]->intersect(rl,dummyColor,0);
                if(t>=0)
                {
                          if(lightT>t)
                        {
                            obsecured = true;
                            break;
                        }


                }
            }
            delete[] dummyColor;
            if(obsecured == false)
            {

                double lambertValue = rl.dir.x * normalAtIntersect.x +
                rl.dir.y * normalAtIntersect.y + rl.dir.z * normalAtIntersect.z ;
                double diffuseCo = this->coEfficients[1];
//                lambertValue = abs(lambertValue);
                if(true)
                {
                    color[0] += curLight->color[0]*diffuseCo*lambertValue*this->color[0];
                    color[1] += curLight->color[1]*diffuseCo*lambertValue*this->color[1];
                    color[2] += curLight->color[2]*diffuseCo*lambertValue*this->color[2];
                }
                double z1 = color[0], z2 = color[1], z3 = color[2];
//                if(z1+z2+z3 ==100)
//                {
//                    cout<<"hello"<<endl;
//                }
                double dDotN = lambertValue;
//                d-2(d*n)n
                Vector3D incidentRay = {rl.dir.x-2*dDotN*normalAtIntersect.x,
                rl.dir.y-2*dDotN*normalAtIntersect.y, rl.dir.z-2*dDotN*normalAtIntersect.z};

                double phongValue = incidentRay.x * r.dir.x +
                incidentRay.y * r.dir.y + incidentRay.z * r.dir.z;
                double phongConst = pow(phongValue,this->shine);
//                phongConst = abs(phongConst);
                double specularCo = this->coEfficients[2];
    //            if(phongConst <0 || lambertValue < 0)
    //            {
    //                cout<<" kill me"<<endl;
    //            }
                if(true)
                {
                    color[0] += curLight->color[0]*specularCo*phongConst*this->color[0];
                    color[1] += curLight->color[1]*specularCo*phongConst*this->color[1];
                    color[2] += curLight->color[2]*specularCo*phongConst*this->color[2];
                }

                double t1 = color[0], t2 = color[1], t3 = color[2];
//                if(t1+t2+t3 ==100)
//                {
//                    cout<<"hello"<<endl;
//                }

            }




        }
    }
//    cout<<color[0]+color[1]+color[2]<<endl;
//    if(globalColor[0]+globalColor[1]+globalColor[2] < color[0]+color[1]+color[2])
//    {
//        cout<<"hi"<<endl;
//        globalColor[0] = color[0];
//        globalColor[1] = color[1];
//        globalColor[2] = color[2];
//
//    }
//    cout<<"after phong: "<<color[0]<< " " << color[1] << " " << color[2]<<endl;
    return;
}

Sphere::Sphere()
{
    this->type = sphere;
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

Vector3D Sphere::getNormal(Vector3D &intersectPoint)
{
    Vector3D ret = {intersectPoint.x-reference_point.x, intersectPoint.y-reference_point.y, intersectPoint.z-reference_point.z};
    double magnitude = sqrt(ret.x*ret.x + ret.y*ret.y + ret.z*ret.z);
    ret.x = ret.x/magnitude;
    if(abs(ret.x-0)<PRECISION_FIX)
    {
        ret.x = 0;
    }
    ret.y = ret.y/magnitude;
    if(abs(ret.y-0)<PRECISION_FIX)
    {
        ret.y = 0;
    }ret.z = ret.z/magnitude;
    if(abs(ret.z-0)<PRECISION_FIX)
    {
        ret.z = 0;
    }
    return ret;
}





double Sphere::intersect(Ray& r, double* color, int recLevel)
{
    Vector3D &co = reference_point;
    Vector3D &ro = r.start;
    Vector3D &rd = r.dir;
    // at^2 + bt + c = 0 is general form of the parametic equation
    //
    double c =  (ro.x-co.x)*(ro.x-co.x) + (ro.y-co.y)*(ro.y-co.y) +
                (ro.z-co.z)*(ro.z-co.z) - radius * radius;

    double b = 2 * ((ro.x-co.x)*rd.x + (ro.y-co.y)*rd.y + (ro.z-co.z)*rd.z);


    double a = rd.x*rd.x + rd.y*rd.y + rd.z*rd.z;

//    cout<<a<<" "<<b<<" "<<c<<endl;

    double d = (b*b-4*a*c); // discriminant
    if(d < 0)
    {
        return -5;
    }
    d = sqrt(d); // root over (b square - 4 a c)

    double t1 = (-b + d) / (2 * a);
    double t2 = (-b - d) / (2 * a);

//    cout<<t1<<" "<<t2<<endl;

    if(t1<0 && t2<0)
    {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        return -5;
    }
    // 0.752674 0.456943 -0.21253



    double t;


    if(t1>=0 && t2>=0)
    {
        t = min(t1,t2);

//        if(recLevel==0)
//        {
//            return t1;
//        }
//        else if(recLevel==1)
//        {
//            getPhongLighting(r,t1,color);
//            return t1;
//        }
    }

    // only 1 case left ... one of t1,t2 is positive
    else if(t1>=0)
    {
//        if(recLevel==0)
//        {
//            return t1;
//        }
//        else if(recLevel==1)
//        {
//            getPhongLighting(r,t1,color);
//            return t1;
//        }
        t = t1;
    }
    else
    {

        t = t2;
//        if(recLevel==0)
//        {
//            return t2;
//        }
//        else if(recLevel==1)
//        {
//            getPhongLighting(r,t2,color);
//            return t2;
//        }
    }

    //so t1 has the tight intersection value
    if(recLevel == 0)
    {
        return t;
    }


    double ambient = this->coEfficients[0];
    color[0] = this->color[0] * ambient;
    color[1] = this->color[1] * ambient;
    color[2] = this->color[2] * ambient;

    getPhongLighting(r,t,color);
    int lor = levelOfRecursion;
    if(recLevel>=lor)
    {
        return t;
    }
    Vector3D ip = {r.start.x+t*r.dir.x, r.start.y+t*r.dir.y, r.start.z+t*r.dir.z}; // intersect point
    Vector3D normal = getNormal(ip); //normal at intersection
    double dDotN = r.dir.x*normal.x + r.dir.y*normal.y + r.dir.z*normal.z;
    Vector3D reflectedRay = {r.dir.x - 2 * dDotN * normal.x,
                             r.dir.y - 2 * dDotN * normal.y,
                             r.dir.z - 2 * dDotN * normal.z};
    double epsilon = 2 * 0.01;
    Ray rReflected(ip,reflectedRay);
    rReflected.dir = reflectedRay;
    rReflected.start = {ip.x + epsilon*rReflected.dir.x, ip.y + epsilon*rReflected.dir.y, ip.z + epsilon*rReflected.dir.z};
    double *dummyColor = new double[3];
    dummyColor[0] = 0;
    dummyColor[1] = 0;
    dummyColor[2] = 0;

    double tmin = INT_MAX;
    Object *minObject = 0;
    for(int i=0;i<objects.size();i++)
    {
//
        Object *curObj = objects[i];
        if(true)
        {
//
            double tObj = curObj->intersect(rReflected,dummyColor,0);
            if(tObj>0)
            {
                if(tmin>tObj)
                {

                    tmin = tObj;
                    minObject = curObj;


                }

            }
//                    break;

        }

    }
    if(tmin!=INT_MAX && minObject != 0)
    {
//                if(i==250 && j == 350)
//                {
//                    cout<<i<<" "<<j<<" "<<tmin<<endl;
//
//                }
        double *colorReflected = new double[3];
        colorReflected[0] = 0;
        colorReflected[1] = 0;
        colorReflected[2] = 0;
        minObject->intersect(r,colorReflected,recLevel+1);


//        double z1 = colorReflected[0], z2 = colorReflected[1], z3 = colorReflected[2];
//        if(z1+z2+z3 ==100)
//        {
//            cout<<"hello"<<endl;
//        }

        double recReflectionCoEff = this->coEfficients[3];

        color[0] += colorReflected[0] * recReflectionCoEff;
        color[1] += colorReflected[1] * recReflectionCoEff;
        color[2] += colorReflected[2] * recReflectionCoEff;

    }
//            else
//            {
//                image.set_pixel(j,i,255,255,255);
//            }

    // this return will never get executed
    return t;
}
//void drawSphere(double radius,int slices,int stacks)
//{
//
//}

Triangle::Triangle()
{

    type = triangle;
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


bool Triangle::insideTriangle(Vector3D &intersection)
{
    // solving the system of linear equation
    double a1 = p2.x-p1.x;
    double b1 = p3.x-p1.x;
    double a2 = p2.y-p1.y;
    double b2 = p3.y-p1.y;
    double c1 = intersection.x;
    double c2 = intersection.y;
    double d = a1*b2-b1*a2;
    double dx = c1*b2-c2*b1;
    double dy = a1*c2-a2*c1;

    // if no solution exists d==0 ...
    //this case won't arise in my opinion because intersection point is always valid
    if(d==0)
    {
        return false;
    }

    double x = dx/d;
    double y = dy/d;
    if(x+y-1<=0 && 0<=x && x<= 1 && 0<=y && y<=1) // checking a+b <=1 constraints
    {

        return true;
    }

    return false;
}



double determinantSolver3by3(double *a, double *b, double *c)
{
    return a[0]*(b[1]*c[2]-b[2]*c[1])
          -b[0]*(a[1]*c[2]-a[2]*c[1])
          +c[0]*(a[1]*b[2]-a[2]*b[1]);
}

Vector3D Triangle::getNormal(Vector3D &intersectPoint)
{
    Vector3D ba = {(p1.x-p2.x),(p1.y-p2.y),(p1.z-p2.z)}; // ba = a-b
    Vector3D bc = {(p3.x-p2.x),(p3.y-p2.y),(p3.z-p2.z)}; // bc = c-b

    Vector3D normal = {ba.y*bc.z-ba.z*bc.y,ba.z*bc.x-ba.x*bc.z,ba.x*bc.y-bc.x*ba.y};
    double magnitude = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
    normal.x = -normal.x;
    normal.y = -normal.y;
    normal.z = -normal.z;   // axb = -bxa // I had done reference cross product so negating each component
    normal.x = normal.x/magnitude;
    if(abs(normal.x-0)<PRECISION_FIX)
    {
        normal.x = 0;
    }
    normal.y = normal.y/magnitude;
    if(abs(normal.y-0)<PRECISION_FIX)
    {
        normal.y = 0;
    }
    normal.z = normal.z/magnitude;
    if(abs(normal.z-0)<PRECISION_FIX)
    {
        normal.z = 0;
    }


    return normal;
}

double Triangle::intersect(Ray& r, double* color, int recLevel)
{
//    double a[3],b[3],c[3],s[3];
    double a[] = {p1.x-p2.x,p1.y-p2.y,p1.z-p2.z};
    double b[] = {p1.x-p3.x,p1.y-p3.y,p1.z-p3.z};
    double c[] = {r.dir.x,r.dir.y,r.dir.z};
    double s[] = {p1.x-r.start.x,p1.y-r.start.y,p1.z-r.start.z};

    double d = determinantSolver3by3(a,b,c);
    if(d==0)
    {
        return -10;
    }
    double db = determinantSolver3by3(s,b,c);
    double dg = determinantSolver3by3(a,s,c);
    double dt = determinantSolver3by3(a,b,s);

    double beta = db/d;
    double gama = dg/d;
    double t = dt/d;

    if(beta+gama<=1 && beta>0 && gama>0 and t>0)
    {


        if(recLevel == 0)
        {
            return t;
        }

        double ambient = this->coEfficients[0];
        color[0] = this->color[0] * ambient;
        color[1] = this->color[1] * ambient;
        color[2] = this->color[2] * ambient;

        getPhongLighting(r,t,color);
        int lor = levelOfRecursion;
        if(recLevel>=lor)
        {
            return t;
        }
        Vector3D ip = {r.start.x+t*r.dir.x, r.start.y+t*r.dir.y, r.start.z+t*r.dir.z}; // intersect point
        Vector3D normal = getNormal(ip); //normal at intersection
        double dDotN = r.dir.x*normal.x + r.dir.y*normal.y + r.dir.z*normal.z;
        Vector3D reflectedRay = {r.dir.x - 2 * dDotN * normal.x,
                                 r.dir.y - 2 * dDotN * normal.y,
                                 r.dir.z - 2 * dDotN * normal.z};
        double epsilon = 2 * 0.01;
        Ray rReflected(ip,reflectedRay);
        rReflected.dir = reflectedRay; // have to set manually
        rReflected.start = {ip.x + epsilon*rReflected.dir.x, ip.y + epsilon*rReflected.dir.y, ip.z + epsilon*rReflected.dir.z};
        double *dummyColor = new double[3];
        dummyColor[0] = 0;
        dummyColor[1] = 0;
        dummyColor[2] = 0;

        double tmin = INT_MAX;
        Object *minObject = 0;
        for(int i=0;i<objects.size();i++)
        {
    //
            Object *curObj = objects[i];
            if(true)
            {
    //
                double tObj = curObj->intersect(rReflected,dummyColor,0);
                if(tObj>0)
                {
                    if(tmin>tObj)
                    {

                        tmin = tObj;
                        minObject = curObj;


                    }

                }
    //                    break;

            }

        }
        if(tmin!=INT_MAX && minObject != 0)
        {
    //                if(i==250 && j == 350)
    //                {
    //                    cout<<i<<" "<<j<<" "<<tmin<<endl;
    //
    //                }
            double *colorReflected = new double[3];
            colorReflected[0] = 0;
            colorReflected[1] = 0;
            colorReflected[2] = 0;
            minObject->intersect(r,colorReflected,recLevel+1);


//            double z1 = colorReflected[0], z2 = colorReflected[1], z3 = colorReflected[2];
//            if(z1+z2+z3 ==100)
//            {
//                cout<<"hello"<<endl;
//            }

            double recReflectionCoEff = this->coEfficients[3];

            color[0] += colorReflected[0] * recReflectionCoEff;
            color[1] += colorReflected[1] * recReflectionCoEff;
            color[2] += colorReflected[2] * recReflectionCoEff;

        }
    //            else
    //            {
    //                image.set_pixel(j,i,255,255,255);
    //            }

        // this return will never get executed
        return t;
    }

    return -5;
    //double d = determinantSolver3by3(a,b,c);
}

General::General()
{

    type = plane;
    //cout<<"creating from General"<<endl;
}

void General::draw()
{
    //cout<<"drawing General"<<endl;
}

bool General::insideGeneral(Vector3D &p)
{
    if(length!=0)
    {
        if(p.x<reference_point.x || p.x>reference_point.x+length)
        {
            return false;
        }
    }
    if(width!=0)
    {
        if(p.y<reference_point.y || p.y>reference_point.y+width)
        {
            return false;
        }
    }

    if(height!=0)
    {
        if(p.z<reference_point.z || p.z>reference_point.z+height)
        {
            return false;
        }
    }

    return true;
}

Vector3D General::getNormal(Vector3D &ip)
{
    double A = constants[0], B = constants[1],  C = constants[2],
     D = constants[3],  E = constants[4], F = constants[5], G = constants[6],
      H = constants[7], I = constants[8], J = constants[9];
    Vector3D normal{2*A*ip.x+D*ip.y+E*ip.z+G,
                 2*B*ip.y+D*ip.x+F*ip.z+H,
                 2*C*ip.z+E*ip.x+F*ip.y+I
                };

    double magnitude = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

    normal.x = normal.x/magnitude;
    if(abs(normal.x-0)<PRECISION_FIX)
    {
        normal.x = 0;
    }
    normal.y = normal.y/magnitude;
    if(abs(normal.y-0)<PRECISION_FIX)
    {
        normal.y = 0;
    }
    normal.z = normal.z/magnitude;
    if(abs(normal.z-0)<PRECISION_FIX)
    {
        normal.z = 0;
    }


    return normal;




}



double General::intersect(Ray& r, double* color, int recLevel)
{
    Vector3D rd,ro;
    double A = constants[0], B = constants[1],  C = constants[2],
     D = constants[3],  E = constants[4], F = constants[5], G = constants[6],
      H = constants[7], I = constants[8], J = constants[9];
    rd = r.dir;
    ro = r.start;

    double a = A*rd.x*rd.x + B*rd.y*rd.y + C*rd.z*rd.z +
               D*rd.x*rd.y + E*rd.x*rd.z + F*rd.y*rd.z ;

    double b = 2*A*ro.x*rd.x + 2*B*ro.y*rd.y + 2*C*ro.z*rd.z +
               D*ro.x*rd.y + D*ro.y*rd.x + E*ro.x*rd.z + E*ro.z*rd.x + F*ro.y*rd.z + F*ro.z*rd.y +
               G*rd.x + H*rd.y + I*rd.z;

    double c = A*ro.x*ro.x + B*ro.y*ro.y + C*ro.z*ro.z +
               D*ro.x*ro.y + E*ro.x*ro.z + F*ro.y*ro.z +
               G*ro.x + H*ro.y + I*ro.z + J;

    double d = (b*b-4*a*c); // discriminant
    if(d < 0)
    {
        return -5;
    }
    d = sqrt(d);

    double t1 = (-b + d) / (2 * a);
    double t2 = (-b - d) / (2 * a);


    if(t1<0 && t2<0)
    {
        return -5;
    }

    double t;

    if(t1>=0 && t2>=0)
    {
        if(t1>t2) // assuring that t1 contains the small value of two ts
        {
            double temp = t2;
            t2 = t1;
            t1 = temp;
        }
        Vector3D i1 = {ro.x+t1*rd.x,ro.y+t1*rd.y,ro.z+t1*rd.z};
        Vector3D i2 = {ro.x+t2*rd.x,ro.y+t2*rd.y,ro.z+t2*rd.z};
        if(insideGeneral(i1))
        {
            t = t1;
        }
        else if(insideGeneral(i2))
        {
            t = t2;
        }
        else
        {
            return -10;
        }


    }
    else if(t1>=0)// only 1 case left ... one of t1,t2 is positive
    {
        Vector3D i1 = {ro.x+t1*rd.x,ro.y+t1*rd.y,ro.z+t1*rd.z};
        if(insideGeneral(i1))
        {
            t = t1;
        }
        else
        {
             return -15;
        }

    }
    else
    {

        Vector3D i2 = {ro.x+t2*rd.x,ro.y+t2*rd.y,ro.z+t2*rd.z};
        if(insideGeneral(i2))
        {
           t = t2;
        }
        else
        {
            return -20;
        }

    }

    if(recLevel == 0)
    {
        return t;
    }


    double ambient = this->coEfficients[0];
    color[0] = this->color[0] * ambient;
    color[1] = this->color[1] * ambient;
    color[2] = this->color[2] * ambient;


//    getPhongLighting(r,t,color);
    int lor = levelOfRecursion;
    if(recLevel>=lor)
    {
        return t;
    }
    Vector3D ip = {r.start.x+t*r.dir.x, r.start.y+t*r.dir.y, r.start.z+t*r.dir.z}; // intersect point
    Vector3D normal = getNormal(ip); //normal at intersection
    double dDotN = r.dir.x*normal.x + r.dir.y*normal.y + r.dir.z*normal.z;
    Vector3D reflectedRay = {r.dir.x - 2 * dDotN * normal.x,
                             r.dir.y - 2 * dDotN * normal.y,
                             r.dir.z - 2 * dDotN * normal.z};
    double epsilon = 2 * 0.01;
    Ray rReflected(ip,reflectedRay);
    rReflected.dir = reflectedRay;
    rReflected.start = {ip.x + epsilon*rReflected.dir.x, ip.y + epsilon*rReflected.dir.y, ip.z + epsilon*rReflected.dir.z};
    double *dummyColor = new double[3];
    dummyColor[0] = 0;
    dummyColor[1] = 0;
    dummyColor[2] = 0;

    double tmin = INT_MAX;
    Object *minObject = 0;
    for(int i=0;i<objects.size();i++)
    {
//
        Object *curObj = objects[i];
        if(true)
        {
//
            double tObj = curObj->intersect(rReflected,dummyColor,0);
            if(tObj>0)
            {
                if(tmin>tObj)
                {

                    tmin = tObj;
                    minObject = curObj;


                }

            }
//                    break;

        }

    }
    if(tmin!=INT_MAX && minObject != 0)
    {
//                if(i==250 && j == 350)
//                {
//                    cout<<i<<" "<<j<<" "<<tmin<<endl;
//
//                }
        double *colorReflected = new double[3];
        colorReflected[0] = 0;
        colorReflected[1] = 0;
        colorReflected[2] = 0;
        minObject->intersect(r,colorReflected,recLevel+1);


//        double z1 = colorReflected[0], z2 = colorReflected[1], z3 = colorReflected[2];
//        if(z1+z2+z3 ==100)
//        {
//            cout<<"hello"<<endl;
//        }

        double recReflectionCoEff = this->coEfficients[3];

        color[0] += colorReflected[0] * recReflectionCoEff;
        color[1] += colorReflected[1] * recReflectionCoEff;
        color[2] += colorReflected[2] * recReflectionCoEff;

    }
    return t;
//            else
//            {
//                image.set_pixel(j,i,255,255,255);
//            }

    // this return will never get executed





    return -100; // never gets called
}

Floor::Floor(double floorWidth, double tileWidth)
{


        shine = 10;
        type = board;
        reference_point={-floorWidth/2,-floorWidth/2,0};
        tileLength=tileWidth;
        width = floorWidth;
        this->coEfficients[0] = 0.4;

        this->coEfficients[1] = 0.2;

        this->coEfficients[2] = 0.2;

        this->coEfficients[3] = 0.2;
}

void Floor::draw()
{
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
            Vector3D bottomLeft = {reference_point.x + j * tileLength ,reference_point.y + i * tileLength , reference_point.z};
            Vector3D bottomRight = {bottomLeft.x+tileLength,bottomLeft.y,bottomLeft.z};
            Vector3D topLeft = {bottomLeft.x, bottomLeft.y + tileLength, bottomLeft.z};
            Vector3D topRight = {bottomLeft.x + tileLength, bottomLeft.y + tileLength, bottomLeft.z};



            if(white)
            {
                glColor3f(color1[0], color1[1], color1[2]);
                white = false;
            }
            else
            {
                glColor3f(color2[0], color2[1], color2[2]);
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

bool Floor::insideFloor(Vector3D &intersect)
{

//            if(true)

                if(-width/2<=intersect.x && intersect.x<=width/2
               &&
               -width/2<=intersect.y && intersect.y<=width/2)
                {
                    return true;
                }






    return false;
}

Vector3D Floor::getNormal(Vector3D &intersectPoint)
{
    return normal;
}

void Floor::getPhongLighting(Ray &r, double t, double* color, bool color1Chosen)
{
    Vector3D intersectionPoint = {r.start.x+t*r.dir.x, r.start.y+t*r.dir.y, r.start.z+t*r.dir.z};
    Vector3D normalAtIntersect = getNormal(intersectionPoint);
    // adding epsilon to elevate intersection point...
    double epsilon = 2*0.01;

//    cout<<"before phong: "<<color[0]<< " " << color[1] << " " << color[2]<<endl;
    for(int i=0;i<lights.size();i++)
    {
        if(true)
        {
            Light *curLight = lights[i];
//            Light &tk = *(lights[i]);
//            Object &tkk = *(this);
            Ray rl(intersectionPoint,curLight->lightPos);
            rl.start = {intersectionPoint.x + epsilon*rl.dir.x, intersectionPoint.y + epsilon*rl.dir.y, intersectionPoint.z + epsilon*rl.dir.z};
//            double lightT = (curLight->lightPos.x-rl.start.x)/rl.dir.x; //    Rox + t * Rdirl = light.x
            Vector3D distanceVec = {curLight->lightPos.x-intersectionPoint.x, curLight->lightPos.y-intersectionPoint.y, curLight->lightPos.z-intersectionPoint.z};
            double lightT = sqrt(distanceVec.x*distanceVec.x + distanceVec.y*distanceVec.y + distanceVec.z*distanceVec.z);

            double *dummyColor = new double[3];
            bool obsecured = false;
            for(int j=0;j<objects.size();j++)
            {
                double t = objects[j]->intersect(rl,dummyColor,0);
                if(t>=0)
                {
                          if(lightT>t)
                        {
                            obsecured = true;
                            break;
                        }


                }
            }
            delete[] dummyColor;
            if(obsecured == false)
            {

                double lambertValue = rl.dir.x * normalAtIntersect.x +
                rl.dir.y * normalAtIntersect.y + rl.dir.z * normalAtIntersect.z ;
                double diffuseCo = this->coEfficients[1];
                lambertValue = abs(lambertValue);
                double c1,c2,c3;
                if(color1Chosen)
                {
                    c1 = color1[0];
                    c2 = color1[1];
                    c3 = color1[2];
                }
                else
                {
                    c1 = color2[0];
                    c2 = color2[1];
                    c3 = color2[2];
                }
                if(true)
                {
                    color[0] += curLight->color[0]*diffuseCo*lambertValue*c1;
                    color[1] += curLight->color[1]*diffuseCo*lambertValue*c2;
                    color[2] += curLight->color[2]*diffuseCo*lambertValue*c3;
                }
//                double z1 = color[0], z2 = color[1], z3 = color[2];
//                if(z1+z2+z3 ==100)
//                {
//                    cout<<"hello"<<endl;
//                }
                double dDotN = lambertValue;
//                d-2(d*n)n
                Vector3D incidentRay = {rl.dir.x-2*dDotN*normalAtIntersect.x,
                rl.dir.y-2*dDotN*normalAtIntersect.y, rl.dir.z-2*dDotN*normalAtIntersect.z};

                double phongValue = incidentRay.x * r.dir.x +
                incidentRay.y * r.dir.y + incidentRay.z * r.dir.z;
                double phongConst = pow(phongValue,this->shine);
                phongConst = abs(phongConst);
                double specularCo = this->coEfficients[2];
    //            if(phongConst <0 || lambertValue < 0)
    //            {
    //                cout<<" kill me"<<endl;
    //            }
                if(true)
                {
                    color[0] += curLight->color[0]*specularCo*phongConst*c1;
                    color[1] += curLight->color[1]*specularCo*phongConst*c2;
                    color[2] += curLight->color[2]*specularCo*phongConst*c3;
                }

//                double t1 = color[0], t2 = color[1], t3 = color[2];
//                if(t1+t2+t3 ==100)
//                {
//                    cout<<"hello"<<endl;
//                }

            }




        }
    }
//    cout<<color[0]+color[1]+color[2]<<endl;
//    if(globalColor[0]+globalColor[1]+globalColor[2] < color[0]+color[1]+color[2])
//    {
//        cout<<"hi"<<endl;
//        globalColor[0] = color[0];
//        globalColor[1] = color[1];
//        globalColor[2] = color[2];
//
//    }
//    cout<<"after phong: "<<color[0]<< " " << color[1] << " " << color[2]<<endl;
    return;
}
double Floor::intersect(Ray& r, double* color, int recLevel)
{
    Vector3D n,ro,rd,po,p;
    n = normal;
    po = reference_point;
    ro = r.start;
    rd = r.dir;

    double d = n.x*rd.x + n.y*rd.y + n.z*rd.z;

    if(d==0) // plane and ray is parallel hence no intersection
    {
        return -10;
    }

    double t = - (n.x*(ro.x-po.x)+n.y*(ro.y-po.y)+n.z*(ro.z-po.z)) / d;

    if(t<0)
    {
        return -5;
    }
    // now we have to check whether the intersection point is inside the triangle
    Vector3D intersctionPoint = {ro.x+t*rd.x,ro.y+t*rd.y,ro.z+t*rd.z};
//    printPoint(intersctionPoint);
    if(insideFloor(intersctionPoint))
    {

        int colDist = floor(abs(reference_point.x - intersctionPoint.x) / tileLength);
        int rowDist = floor(abs(reference_point.y - intersctionPoint.y) / tileLength);

        bool color1Chosen = false;
        if(rowDist%2==0)
        {
            if(colDist%2==0)
            {
                color1Chosen = true;



//                cout<<color[0]<<endl;
            }
            else
            {

            }
        }
        else
        {
            if(colDist%2!=0)
            {
                color1Chosen = true;

            }
            else
            {

            }
        }
//
//        if(recLevel>=1)
//        {
//            getPhongLighting(r,t,color,color1Chosen);
//        }
//
//
//        return t;
        if(recLevel == 0)
        {
            return t;
        }

        double ambient = this->coEfficients[0];
        if(color1Chosen)
        {
            color[0] = this->color1[0] * ambient;
            color[1] = this->color1[1] * ambient;
            color[2] = this->color1[2] * ambient;
        }
        else
        {
            color[0] = this->color2[0] * ambient;
            color[1] = this->color2[1] * ambient;
            color[2] = this->color2[2] * ambient;
        }


        getPhongLighting(r,t,color,color1Chosen);
        int lor = levelOfRecursion;
        if(recLevel>=lor)
        {
            return t;
        }
        Vector3D ip = {r.start.x+t*r.dir.x, r.start.y+t*r.dir.y, r.start.z+t*r.dir.z}; // intersect point
        Vector3D normal = getNormal(ip); //normal at intersection
        double dDotN = r.dir.x*normal.x + r.dir.y*normal.y + r.dir.z*normal.z;
        Vector3D reflectedRay = {r.dir.x - 2 * dDotN * normal.x,
                                 r.dir.y - 2 * dDotN * normal.y,
                                 r.dir.z - 2 * dDotN * normal.z};
        double epsilon = 2 * 0.01;
        Ray rReflected(ip,reflectedRay);
        rReflected.dir = reflectedRay; // have to set manually
        rReflected.start = {ip.x + epsilon*rReflected.dir.x, ip.y + epsilon*rReflected.dir.y, ip.z + epsilon*rReflected.dir.z};
        double *dummyColor = new double[3];
        dummyColor[0] = 0;
        dummyColor[1] = 0;
        dummyColor[2] = 0;

        double tmin = INT_MAX;
        Object *minObject = 0;
        for(int i=0;i<objects.size();i++)
        {
    //
            Object *curObj = objects[i];
            if(true)
            {
    //
                double tObj = curObj->intersect(rReflected,dummyColor,0);
                if(tObj>0)
                {
                    if(tmin>tObj)
                    {

                        tmin = tObj;
                        minObject = curObj;


                    }

                }
    //                    break;

            }

        }
        if(tmin!=INT_MAX && minObject != 0)
        {
    //                if(i==250 && j == 350)
    //                {
    //                    cout<<i<<" "<<j<<" "<<tmin<<endl;
    //
    //                }
            double *colorReflected = new double[3];
            colorReflected[0] = 0;
            colorReflected[1] = 0;
            colorReflected[2] = 0;
            minObject->intersect(r,colorReflected,recLevel+1);


//            double z1 = colorReflected[0], z2 = colorReflected[1], z3 = colorReflected[2];
//            if(z1+z2+z3 ==100)
//            {
//                cout<<"hello"<<endl;
//            }

            double recReflectionCoEff = this->coEfficients[3];

            color[0] += colorReflected[0] * recReflectionCoEff;
            color[1] += colorReflected[1] * recReflectionCoEff;
            color[2] += colorReflected[2] * recReflectionCoEff;

        }
    //            else
    //            {
    //                image.set_pixel(j,i,255,255,255);
    //            }

        // this return will never get executed
        return t;
    }



    return -15;

}

Ray::Ray(Vector3D &eye, Vector3D &pixel)
{
    start = eye;
    dir.x = pixel.x-eye.x;
    dir.y = pixel.y-eye.y;
    dir.z = pixel.z-eye.z;
    double magnitude = sqrt(dir.x*dir.x+dir.y*dir.y+dir.z*dir.z);
    dir.x = dir.x/magnitude;
    if(abs(dir.x)<0.00001)
    {
        dir.x = 0;
    }
    dir.y = dir.y/magnitude;
    if(abs(dir.y)<0.00001)
    {
        dir.y = 0;
    }
    dir.z = dir.z/magnitude;
    if(abs(dir.z)<0.00001)
    {
        dir.z = 0;
    }
//    cout<<"start and dir of ray"<<endl;
//    cout<<start.x<<" "<<start.y<<" "<<start.z<<endl;
//    cout<<dir.x<<" "<<dir.y<<" "<<dir.z<<endl;

}

void Light::draw()
{
    struct point points[100][100];
	int i,j;
	double h,r;
	int stacks = 50;
	int slices = 50;
	double radius = 2;
	Vector3D reference_point = this->lightPos;


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


void printPoint(struct point p)
{

    cout<<p.x<<" "<<p.y<<" "<<p.z<<endl;
}



