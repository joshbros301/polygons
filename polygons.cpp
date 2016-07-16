//
//  polygons.cpp
//  Contains the full class heirarchy for polygons.
//  This includes special classes for rectangle, isoceles, pentagon and
//  hexagon, as well as the class for a general polygon.
//
//  Created by Joshua Brothers on 27/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#include "polygons.hpp"

namespace std{

	// initialise static member
	int polygon::numberCreated = 0;
    // Standard Destructor - Clears all values from vector storing coordinate data
    polygon::~polygon(){coordsData.clear();};
    
    // Parameterised constructor - Used as standard polygon constrcturo for all inherited classes
    // Checks the number of vertices and throws and error if in correct.
    // Also calculates the angles at vertices for regular shapes
    polygon::polygon (int newNoVertices,bool regular){
        
        // If statement to check that polygon is geometricall possible
        if (newNoVertices < 3){
            cerr << "Number of edges must be larger than 3 for any polygon!" << endl;
            throw generateShapeFlag;
        }
        // Set the number of vertices  and calculate the total interior angle for the shape
        noVertices = newNoVertices;
        totalAngle = pi * (noVertices-2);
        
        // Try catch to try to allocate memory for angles
        try{
            angles = new double[noVertices];
        } catch(bad_alloc memFail){
            throw generateShapeFlag;
        }
        
        // If statement checkes if the shape is regular, and if so calculates the angles
        if (regular == true){
            double standardAngle = totalAngle/noVertices;
            for (int i = 0; i < noVertices; i++){
                angles[i] = standardAngle;
            }
        }

		// Lastly increase the number created
		numberCreated++;
    }
    
    // Rotate functions - Three variations of matrices - Generates a matix and the about point
    // if they have not been given and then calls the final fourth rotate function using a matrix and a coordinate about point
    void polygon::rotate(matrix rotMatrix){ rotate (rotMatrix,centreOfShape(coordsData));}
    void polygon::rotate(double angle){ rotate(matrix{angle});}
    void polygon::rotate(double angle, coord about){rotate(matrix{angle},about);}
    
    // Calculates the coordinates of the new rotated shape using standard matrix * vector multipication
    void polygon::rotate(matrix m, coord about){
        for (size_t i = 0; i < coordsData.size(); i++){
            double newx = about.getx() + (m.getData(0) * (coordsData[i].getx() - about.getx())) + (m.getData(1) * (coordsData[i].gety() - about.gety()));
            double newy = about.gety() + (m.getData(2) * (coordsData[i].getx() - about.getx())) + (m.getData(3) * (coordsData[i].gety() - about.gety()));
            coordsData[i].setBoth(newx, newy);
        }
    }
    // Translate functions
    void polygon::translate(coord move){
        translate(move.getx(),move.gety());
    }
    void polygon::translate(double xMove, double yMove){
        for (int i = 0; i < noVertices; i++){
            coordsData[i].setx(coordsData[i].getx() + xMove);
            coordsData[i].sety(coordsData[i].gety() + yMove);
        }
    }
    // Scale functions
    // First two calculate a scale about a centre or about a given coordinate
    void polygon::scale(double xScale, double yScale){
        for (int i = 0; i <noVertices; i++){
            coordsData[i].setx(coordsData[i].getx()*xScale);
            coordsData[i].sety(coordsData[i].gety()*yScale);
        }
    }
    void polygon::scale(double xScale, double yScale, coord about){
        double relativeXScale = xScale - about.getx();
        double relativeYscale = yScale - about.gety();
        scale(relativeXScale,relativeYscale);
    }
    
    // These scale functions take in scale factors, or directional scaling, convert to doubles and call
    // the relevant scale function above
    void polygon::scale(coord scaleCoord, coord about){
        scale(scaleCoord.getx(),scaleCoord.gety(),about);
    }
    void polygon::scale(coord scaleCoord){
        scale(scaleCoord.getx(),scaleCoord.gety());
    }
    void polygon::scale(double scaleFactor,coord about){
        scale(scaleFactor,scaleFactor,about);
    }
    void polygon::scale(double scaleFactor){
        scale(scaleFactor,scaleFactor);
    }
    
    // Accessor functions
    int polygon::getNoVertices(){
        return noVertices;
    }
    coord polygon::getCoord(int index){
        return coordsData[index];
    }
	vector<coord> polygon::getAllCoord(){
		return coordsData;
	}
    string polygon::getType() const {
        return type;
    }
	int polygon::getNumberCreated() const{
		return numberCreated;
	}
	
    void polygon::info(){
        for (size_t i = 0; i < coordsData.size();i++){
            cout << i+1 << ": " << coordsData[i] << endl;
        }
        cout << endl;
    }
    
    // Set all the coordinates for a polygon
    void polygon::setAllCoords(vector<coord> newCoordsData){
        coordsData.clear();
        coordsData = newCoordsData;
    }
    
    // Overlap function to return the points at which a polygon overlaps with another
    vector<coord> polygon::overlap(polygon *a){
        
        // Define a vector to store the overlapped points
        vector<coord> overlapPoints;
        
        // Nested for loops to run through each line of each shape
        for (int i= 0; i < noVertices; i++){
            for (int j = 0; j < a->noVertices; j++){
                
                // To ensure the last edge is checked, if statement to change i and j to the correct values
                int upperiCheck, upperjCheck;
                if (i == noVertices-1) upperiCheck = 0;
                else upperiCheck = i+1;
                if (j == a->noVertices-1) upperjCheck = 0;
                else upperjCheck = j+1;
                
                // Calculate the coordinate of the intersection point using the intersect function defined in coords.hpp
                // This returns a vector with empty set to true if they do not intersect
                coord check = intersect(coordsData[i],coordsData[upperiCheck],a->getCoord(j),a->getCoord(upperjCheck));
                
                // If statement checkes that the coordinate returned is not empty, and thus is an intersection point
                // Pushes back onto vector if so
                if (!check.getEmpty()){
                    overlapPoints.push_back(check);
                }
            }
        }
        return overlapPoints;
    }
    
    // Friend to overload the output stream for polygons
    // Displays by displaying the polygons type followed by a list of its coordinates
    ostream& operator<<(ostream &os, polygon &a){
        
        os << "Type: " << a.type << endl;
        os << "Coordinates: " << endl;
        for (size_t i = 0; i < a.coordsData.size();i++){
            os << i + 1 << ": " << a.coordsData[i] << endl;
        }
        return os;
    }
    
    // Friend to overload the input stream operator for polygons
    // Runs through a for loop for the number of coordinates and lets the user input each coordinate using
    // coordinates input stream operator
    istream& operator>>(istream &is, polygon &a){
        coord tempCoord;
        vector<coord> coordVector;
        
        for (int i = 0; i < a.noVertices;i++){
            cout << endl << i+1 << ": ";
            is >> tempCoord;
            coordVector.push_back(tempCoord);
        }
        a.setAllCoords(coordVector);
        return is;
    }
    
    // rectangle constructors
    // Default constructor generates a rectangle with all coordinates set to (0,0)
    rectangle::rectangle() : polygon(4,true){
        for (size_t i = 0; i < 4; i++){
            coordsData.push_back(coord{0,0});
        }
        type = "rectangle";
    }
    
    rectangle::rectangle(coord v1, coord v2, coord v3, coord v4) : polygon(4,true){

		double a = math::round(angle(v1, v2, v4));
		double b = round(pi / 2);
		double c = math::round(angle(v3, v2, v4));
		// If statement to check coordinates entered is a rectangle
		if (math::round(angle(v1, v2, v4)) != math::round(pi / 2) || math::round(angle(v3, v2, v4)) != math::round(pi / 2)){
			cerr << "Error: Not a rectangle!" << endl;
			throw generateShapeFlag;
		}

        coordsData.push_back(v1); coordsData.push_back(v2);
        coordsData.push_back(v3); coordsData.push_back(v4);
        type = "rectangle";
    }
    
    // Isoceles triangle constructors
    // Defulat constructor sets all elements to (0,0)
    isoceles::isoceles() : polygon(3,false){
        for (size_t i = 0; i < 3; i++){
            coordsData.push_back(coord{0,0});
        }
        angles = 0;
        type = "isoceles";
    }
    isoceles::isoceles(coord v1, coord v2, coord v3) : polygon(3,false){
        
        // Calculate the angles to ensure that the cooridnates inputted form an isoceles triangle
        double a1 = angle(v1,v2,v3);
        double a2 = angle(v2,v3,v1);
        double a3 = angle(v3,v1,v2);
        if(!((a1== a1 && a2!=a3) || (a1 == a3 && a3 != a2) || (a2 ==a3 || a1 != a2))){
            cout << "ERROR: Not an Isoceles triangle!";
            throw generateShapeFlag;
        }
        
        coordsData.push_back(v1);coordsData.push_back(v2);coordsData.push_back(v3);
        angles[0] = a1; angles[1] = a2; angles[2] = a3;
        type = "isoceles";
        
    }
    
    // Constructor uses two coordinates and an angle to generat the third coordinate that would produce an
    // isoceles triangle
    // Angle is set to be the interior angle at the first inputted coordinate
    isoceles::isoceles(coord v1, coord v2, double angle): polygon(3,false){
        totalAngle = pi;
        coordsData.push_back(v1);
        angles[0] = angle;
        angles[1] = (pi- angle)/2;
        angles[2] = angles[1];
        type = "isoceles";
    }
    
    // Pentagon constructors
    pentagon::pentagon() :polygon(5,true){
        type = "pentagon";
    }
    pentagon::pentagon(coord v1, coord v2, coord v3, coord v4, coord v5) : polygon(5,true){
        coordsData = vector<coord> {v1,v2,v3,v4,v5};
        type = "pentagon";
    }
    
    // Hexagon constructors
    hexagon::hexagon() : polygon(6,true){
        type = "hexagon";
    }
    hexagon::hexagon(coord v1, coord v2, coord v3, coord v4, coord v5, coord v6) : polygon(6,true){
        coordsData = vector<coord> {v1,v2,v3,v4,v5,v6};
        type = "hexagon";
    }
    
    // General constructors
    // Default constructor generates a 3 sided general polygon with (0,0) for all coordinates
    general::general() : polygon(3,true){type = "3 sided Regual Polygon";};
    
    // Parameterised constructor takes in number of edges, starting coordinate and an edge length.
    general::general(int noEdges, coord start, double edgeLength) : polygon(noEdges,true){
        
        // Set the type to be the number of edges plus "sided regular polygon"
        type = to_string(noEdges) + " Sided Regular Polygon";
        
        // Generate a vector to store all the coordinates and pushback the user given one
        vector<coord> newData;
        newData.push_back(start);
        
        // Define a temporary coordinate for future clalculations and set the starting angle to 0
        // NOTE: All future angles are defined as the angle from the positive x axis
        coord temp = start;
        double angle = 0;
        
        // For loop runs through each edge and calculates the position of the following coordinate from
        // the edge length and the current angle
        // Angle is defined from the number of edges
        for(int i = 0; i < noEdges; i++){
            double xMove = edgeLength*cos(angle);
            double yMove = edgeLength*sin(angle);
            
            // Set both x and y values to the new coordinate for pushing back
            temp.setBoth(temp.getx() + xMove, temp.gety()+ yMove);
            
            // Two nested if statements ensure that on the last run that the final coordinate would be the same
            // as the starting coordinate
            if (i == noEdges -1){
                if (temp!= newData[0]) {
					
					cerr << "Mathematical Error in generating general polygon" << endl; 
					throw generateShapeFlag;
				}

			}
			else{

				// Push back the temporary vector onto data vector and calculate the next angle to to be used for next angle
				newData.push_back(temp);
				angle = angle + (2 * pi) / noEdges;
			}
        }
        // Set all the coordinate for the new shape
        setAllCoords(newData);
    }
}
