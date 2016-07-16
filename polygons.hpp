//
//  polygons.hpp
//
//  Created by Joshua Brothers on 27/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#ifndef polygons_hpp
#define polygons_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include "matrix.hpp"

const int generateShapeFlag{-1}; // try-throw-catch flag for generating shape errors

namespace std{
    
    // Polygon bass class: All Shape classes derive from this polygon
    class polygon {
    protected:
        // coordsData holds the coordinates of the vertices of the polygon, type is used as an identifier for the shape
        vector<coord> coordsData;
        int noVertices;
        double* angles;
        double totalAngle;
		string type; 
		static int numberCreated;
    public:
        
        // Destructor and paramaterised constructor
        // No default constructor as a polygon shape should never be generated outside of inherited classes
        // paramaterised constructor used to speed up the process of generating standard elements for derived shapes
        virtual ~polygon();
        polygon(int newNoVertices, bool regular);
        
        // Rotate functions to rotate polygons invarious ways, using matrices and angles, and about either the centre
        // or a given coordinate
        void rotate(matrix rotMatrix);
        void rotate(double angle);
        void rotate(double angle, coord about);
        void rotate(matrix m, coord about);
        
        // Translate functions to move the shape. Overloaded to allow for the movement to be given as a coordinate or
        // as an x and y value
        void translate(coord move);
        void translate(double xMove, double yMove);
        
        // Scale functions to allow the shape to be scaled by a given amount in each direction, as doubles or as a coordinate
        // and also about various points, or about its centre
        void scale(double xScale, double yScale);
        void scale(double xScale, double yScale, coord about);
        void scale(coord scaleCoord, coord about);
        void scale(coord scaleCoord);
        void scale(double scaleFactor,coord about);
        void scale(double scaleFactor);
        
        // Accessor functions for accessing polygon information
        int getNoVertices();
        coord getCoord(int index);
		vector<coord> getAllCoord();
        string getType() const;
		int getNumberCreated() const;
        void info();
        
        // Set function to allow all coordinate points to be set at once
        void setAllCoords(vector<coord> newCoordsData);
        
        // Overlap function to determine the overlap points for the given shape and another shape
        vector<coord> overlap(polygon* a);
        
        // Friend functions to overload the istream and ostream operators
        friend ostream& operator<<(ostream &os, polygon &a);
        friend istream& operator>>(istream &is, polygon &a);
        
    };
    
    // Rectangle class to allow the creation of rectangles - Inherited from polygon
    class rectangle : public polygon{
    protected:
    public:
        // Constructors
        rectangle();
        rectangle(coord v1, coord v2, coord v3, coord v4);
        ~rectangle(){};
        
    };
    
    // Isoceles class to allow the creation of isoceles triangles - Inherited from polygon
    class isoceles : public polygon{
    protected:
    public:
        
        // Constructors
        ~isoceles(){};
        isoceles();
        isoceles(coord v1, coord v2, coord v3);
        isoceles(coord v1, coord v2, double angle);
    };
    
    // Pentagon class to allow the creation of Pentagons - Inherited from polygon
    class pentagon :public polygon{
    protected:
    public:
        
        // Constructors
        ~pentagon(){};
        pentagon();
        pentagon(coord v1, coord v2, coord v3, coord v4, coord v5);
    };
    
    // Hexagon class to allow the creation of hexagons - Inherited from polygon
    class hexagon : public polygon{
    protected:
    public:
        
        // Constructors
        ~hexagon(){};
        hexagon();
        hexagon(coord v1, coord v2, coord v3, coord v4, coord v5, coord v6);
    };
    
    // General class to allow the creation of regular polygons - Inherited from polygon
    class general :public polygon {
    protected:
    public:
        
        // Constructors
        // Paramaterised constructor differs from that of other polygons as we generate the coordinate positions from
        // the starting point, the type of polygon and the length of one edge
        ~general(){};
        general();
        general(int noEdges,coord start, double edgeLength);
    };
}

#endif /* polygons_hpp */
