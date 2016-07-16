//
//  coords.cpp
//  Defines the coordinate class, used for defining all shapes
//  for the polygons project
//
//  Created by Joshua Brothers on 26/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#include "coords.hpp"

const int istreamFlag{-1};

namespace std{
    namespace math{
        // Introduce a rounding function
        // Called math::round as round is already predefined as something else
        double round(double number, int const decimalplaces){
			number = number*pow(10,decimalplaces);
			number = std::round(number);
            number = number/pow(10,decimalplaces);
            return number;
		}
    
        double round(double const number){
			return round(number,3);
        }
    }
    //                                  //
    // Coord Class Definitions          //
    //                                  //
    
	// Initialise static member
	int coord::numberCreated = 0;

    // Default and parameterised constructors
	coord::coord() : x{ 0 }, y{ 0 }, empty{ true }{ numberCreated++; };
    
    coord::coord(double const newx, double const newy){
        x = math::round(newx,6);
        y = math::round(newy,6);
        empty = false;
		numberCreated++;
    }
    
    // Set functions
    void coord::setx(double const newx){
        x = math::round(newx,6);
        empty = false;
    }
    void coord::sety(double const newy){
        y = math::round(newy,6);
        empty = false;
    }
    void coord::setBoth(double const newx, double const newy){
        x = math::round(newx,6);
        y = math::round(newy,6);
        empty = false;
    }
    
    // Accessor Functions
    double coord::getx() const {return x;}
    double coord::gety() const{return y;}
    double coord::getEmpty() const{return empty;}
	int coord::getNumberCreated() const { return numberCreated; }
    
    
    // friend functions to overload input and output stream operators
    ostream& operator<<(ostream&os,coord&a){
        
        // fixed forces an output using 6 decimal points
        os << fixed;
        os << "(" << a.x << "," << a.y << ")";
        return os;
    }
    
	istream& operator>>(istream &is, coord&a){
		double x, y;
		is >> x >> y;
		while (is.fail()){
			is.clear();
			is.ignore(10000, '\n');
			cout << "Please enter only numeric values for coordinates" << endl;
			is >> x >> y;
		}
        a.setBoth(x,y);
        return is;
    }
    
    // Overload standard operators for coordinates using standard c++ meanings
    coord& coord::operator=(coord& c) {
        if (&c == this) return *this;
        x = c.x;
        y = c.y;
        
        return *this;
    }
    bool coord::operator==(coord& c)const {
        if (x == c.getx()){
            if (y == c.gety()){
                return true;
            }
        }
        return false;
    }
    bool coord::operator!=(coord& c)const {
        if (x == c.getx() && y == c.gety()) return false;
        return true;
    }
    
    //                                                                          //
    // Funcions for dealing with coordinates and determining line information   //
    //                                                                          //
    
    double length(const coord a, const coord b){
        double xdist = a.getx() - b.getx();
        double ydist = a.gety() - b.gety();
        return pow(pow(xdist,2) + pow(ydist,2),0.5);
    }
    
    coord centreOfShape(const vector<coord> shape){
        // Uses the concept that the centre of the shape will be the mean of its coordinate values
        double xTot{0},yTot{0};
        for (size_t i = 0; i < shape.size(); i++){
            xTot = xTot + shape[i].getx();
            yTot = yTot + shape[i].gety();
        }
        
        double x = xTot/shape.size();
        double y = yTot/shape.size();
        return coord{x,y};
    }
    
    double gradient(const coord a, const coord b){
        
        // If statement to check if the line goes straight up
        // If so set the gradient to infinity
        if( math::round(a.getx()) == math::round(b.getx())){
            return INFINITY;
        }
        
        double top = b.gety() - a.gety();
        double bottom = b.getx() - a.getx();
        double frac = top/bottom;
        return frac;
    }
    
    // yIntercept calculates the c value using y = mx+c
    double yIntercept(const double grad,const coord a){
        return a.gety() - grad*a.getx();
    }
    
	// Retuns the angle made between two lines that cross at the coordinate point cross
	double angle(const coord cross, const coord a, const coord b){
		double grad1 = gradient(a, cross);
		double grad2 = gradient(b, cross);
		double top = grad2 - grad1;
		double bottom = 1 + (grad2*grad1);

		// Check for division by 0
		if (!isnormal(bottom)){
			return pi / 2;
		}

		double frac = abs(top / bottom);
		double p = atan(frac);
		return p;
	}

    // inersectPoint returns the coordinate that two lines intersect from two coordinates taken from
    // each line.
    // Returns an empty coordinate if the lines do no intercept
	// using y = mx+ c notation
    coord intersectPoint(const coord line1p1, const coord line1p2,const coord line2p1, const coord line2p2){
        
        double line1m = gradient(line1p1,line1p2);
        double line2m = gradient(line2p1, line2p2);
        
        // Check if either of the gradients are infinity
        if (line1m == INFINITY || line2m == INFINITY){
            if (line1m == line2m){
                return coord{};
            }else if (line1m == INFINITY){
                double line2c = yIntercept(line2m,line2p1);
                if (!intersectInsideLines(line1p1,line1p2,line2p1,line2p2,line1p1.getx(),line2m*line1p1.getx() +line2c)){
                    return coord();
                }
                
                return coord{line1p1.getx(),line2m*line1p1.getx() +line2c};
            }else if (line2m == INFINITY){
                double line1c = yIntercept(line1m,line1p1);
                
                if (!intersectInsideLines(line1p1,line1p2,line2p1,line2p2,line2p1.getx(),line1m*line2p1.getx() + line1c)){
                    return coord();
                }

                return coord{line2p1.getx(),line1m*line2p1.getx() + line1c};
            }
        }
        
        if (line1m == 0 && line2m ==0) {
            return coord{};
        }
        
        double line1c = yIntercept(line1m,line1p1);
        double line2c = yIntercept(line2m,line2p1);
        
        double xInterceptPoint = (line2c - line1c)/(line1m - line2m);
        double yInterceptPoint = (line1m * xInterceptPoint) + line1c;
        
        // If statement to check that the intercept point of the lines is actually within the regions bounded by the coordinates given.
        if (!intersectInsideLines(line1p1,line1p2,line2p1,line2p2,xInterceptPoint,yInterceptPoint)){
            return coord();
        }
        
        
        return coord{xInterceptPoint,yInterceptPoint};
    }
    
	// Works out if two lines intersect, and if so calls intersectPoint which works out where they intersect
    coord intersect(const coord line1p1, const coord line1p2,const coord line2p1,const coord line2p2){
        double line1Grad = math::round(gradient(line1p1,line1p2));
        double line2Grad = math::round(gradient(line2p1,line2p2));
        
        if (line1Grad == line2Grad){
            if (yIntercept(line1Grad, line1p1)!=yIntercept(line2Grad,line2p1)){
                // Return an empty coordinate to show the lines do not intersect
                return coord{};
            }else {
                return intersectPoint(line1p1,line1p2,line2p1,line2p2);
            }
            
        } else{
            return intersectPoint(line1p1,line1p2,line2p1,line2p2);
        }
    }
    
    
    bool intersectInsideLines(coord line1p1, coord line1p2, coord line2p1, coord line2p2, double xIntercept, double yIntercept){
        // Each of these coordinates are not the technical standard use of the coordinate system.
        // Instead of them constituing an x and a y value, the 'x' value is the maximum point of a line,
        // in a predefined direction and 'y' is the minimum point.
        // This is used as the coordinate system is an easy way to pass back two variables.
        coord line1x = maxPoint(line1p1,line1p2,true);
        coord line2x = maxPoint(line2p1,line2p2,true);
        coord line1y = maxPoint(line1p1,line1p2,false);
        coord line2y = maxPoint(line2p1,line2p2,false);
        
        
        // getx() and gety() below are not used as originally intended. getx() returns the maximum value
        // of a line, and gety() returns the minimum value
        if (!(xIntercept <= line1x.getx() && xIntercept >= line1x.gety())) return false;
        if (!(xIntercept <= line2x.getx() && xIntercept >= line2x.gety())) return false;
        if (!(yIntercept <= line1y.getx() && yIntercept >= line1y.gety())) return false;
        if (!(yIntercept <= line2y.getx() && yIntercept >= line2y.gety())) return false;
        
        return true;
    }
    
	coord maxPoint(const coord p1, const coord p2, const bool x){

		// Returns value as coordinate of the maximum and minimum of a line
		// NOT a correct use of coordinate - BE WARY
		// Bool used to determin if its x or y we're looking at

		double max, min;
		if (x){
			if (p1.getx() == p2.getx()){
				max = p2.getx();
				min = p1.getx();
			}
			else if (p1.getx() < p2.getx()){
				max = p2.getx();
				min = p1.getx();
			}
			else {
				max = p1.getx();
				min = p2.getx();
			}
		}
		else {
			if (p1.gety() == p2.gety()){
				max = p2.gety();
				min = p1.gety();
			}
			else if (p1.gety() < p2.gety()){
				max = p2.gety();
				min = p1.gety();
			}
			else {
				max = p1.gety();
				min = p2.gety();
			}
		}
		return coord{ max, min };
	}
}