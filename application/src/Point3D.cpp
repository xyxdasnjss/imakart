#include "Point3D.hpp"

Point3D::Point3D():
			x(0.f),y(0.f),z(0.f){
				
	}
	Point3D::Point3D(float x_,float y_, float z_):
			x(x_),y(y_),z(z_){
				
	}
	Point3D::Point3D(Point3D& other):
			x(other.x),y(other.y),z(other.z){
				
	}
