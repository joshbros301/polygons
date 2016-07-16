//
//  matrix.cpp
//  Contains matrix class definition for simple 2x2 matrices used
//  for rotations for polygons
//
//  Created by Joshua Brothers on 27/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#include "matrix.hpp"

namespace std{

    // Initialise static member

	int matrix::numberCreated = 0;
    // Default constructor - Set all values of the matrix to 0
    matrix::matrix(){
        for (size_t i = 0; i <4; i++){
            data[i] = 0;
        }
		numberCreated++;
    }
    
    // Paramaterised constructor where each value of the matrix is given by the user
    matrix::matrix(double const newx11, double const newx12, double const newx21, double const newx22){
        data[0] = newx11;
        data[1] = newx12;
        data[2] = newx21;
        data[3] = newx22;
		// Lastly increase the number created
		numberCreated++;
    }
    
    // Paramaterised Constructor to generate the standard rotation matrix from a given angle
    // in radians
    matrix::matrix(double angle){
        data[0] = cos(angle);
        data[1] = -sin(angle);
        data[2] = sin(angle);
        data[3] = cos(angle);
		// Lastly increase the number created
		numberCreated++;
    }
    
    // Accessor and set functions
    double matrix::getData(int const index) const {
        return data[index];
    }
    void matrix::setData(double const newData, int const index){
        data[index] = newData;
    }

	int matrix::getNumberCreated() const {
		return numberCreated;
	}
    
    // Overloaded stream operators to allow input and output of matrices
    ostream& operator<<(ostream &os, matrix &a){
        os << a.getData(0) << " " << a.getData(1) << endl;
        os << a.getData(2) << " " << a.getData(3) << endl;
        return os;
    }
    istream& operator>>(istream &is, matrix &a){
        double newData[4];
		for (int i = 0; i < 4; i++){
			is >> newData[i];
			a.setData(newData[i], i);
		}
        return is;
    }
}