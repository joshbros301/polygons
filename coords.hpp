//
//  coords.hpp
//
//  Created by Joshua Brothers on 26/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#ifndef coords_hpp
#define coords_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>

const double pi{ 3.14159265359 };

namespace std{

	namespace math{
		double round(double number, int const decimalPlaces);
		double round(double const number);
	}
    
    class coord {
    private:
        double x;
        double y;
        bool empty;
		static int numberCreated;
    public:
        
        // Constructors & destructors
        ~coord(){};
        coord();
        coord(const double newx, const double newy);
        
        // Set functions
        void setx(double const newx);
        void sety(double const newy);
        void setBoth(double const newx, double const newy);
        
        // Accessor Functions
        double getx() const;
        double gety() const;
        double getEmpty() const;
		int getNumberCreated() const;
        
        // Define friend functions for streams
        friend ostream& operator<<(ostream &os, coord &a);
        friend istream& operator>>(istream &is, coord &a);
        
        // Overload standard operators for coordinate comparisons and arithmetic
        coord& operator=(coord& c);
        bool operator==(coord&  c) const ;
        bool operator!=(coord& c) const;
    };
    
	// Geometry functions
    double length(const coord a,const coord b);
    coord centreOfShape(const vector<coord> shape);
	double angle(const coord cross, const coord a, const coord b);
    double gradient(const coord a, const coord b);
    double yIntercept(const double grad,const coord a);
    
	// Line intersect functons
    coord intersectPoint(const coord line1p1, const coord line1p2,const coord line2p1,const coord line2p2);
    coord intersect(const coord line1p1,const coord line1p2, const coord line2p1, const coord line2p2);
    bool intersectInsideLines(coord line1p1, coord line1p2, coord line2p1, coord line2p2, double xIntercept, double yIntercept);
    coord maxPoint(const coord p1, const coord p2,const bool x);
}
#endif /* coords_hpp */
