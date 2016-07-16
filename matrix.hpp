//
//  matrix.hpp
//
//  Created by Joshua Brothers on 27/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <stdio.h>
#include <iostream>
#include "coords.hpp"

const int matrixMakeFlag{ -1 };

namespace std{
    class matrix {
    private:
        double data[4];
		static int numberCreated;
    public:
        
        // Destructor and constructors
        ~matrix(){};
        matrix();
        matrix(double const newx11, double const newx12, double const newx21, double const newx22);
        matrix(double angle); // Constructs the rotation matrix for a given angle given in radians
        
        // Accessors and set functions
        double getData(int const index) const;
        void setData(double const newData, int const index);
		int getNumberCreated() const;
        
        // Define friend streams to allow overloading of input and output stream operators
        friend ostream& operator<<(ostream &os, matrix &a);
        friend istream& operator>>(istream &is, matrix &a);
    };
    
}

#endif /* matrix_hpp */
