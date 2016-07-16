//
//  files.hpp
//
//  Created by Joshua Brothers on 27/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#ifndef data_hpp
#define data_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "polygons.hpp"

// Try-Throw-Catch flags
const int fstreamFlag{-1}; // Used for errors reading files
const int emptyFileFlag{-2}; // Used for empty files
const int back{ 1 }; // Used for going back at any stage in menu

using namespace std;

namespace files{

	// Save functions
    void saveShape(polygon shape,string fileName);
    void saveShape(polygon shape);
    
    void saveMatrix(matrix m,string fileName);
    void saveMatrix(matrix m);
    
    void saveCoordinate(coord c,string fileName);
    void saveCoordinate(coord c);
    
	// Load functions
    polygon loadShape(int shapeNumber, string fileName);
    polygon loadShape();
    
    vector<polygon> loadAllShapes(string fileName);
    vector<polygon> loadAllShapes();
    
    polygon loadOneShape(string fileName);
    polygon loadOneShape();
    
    matrix loadMatrix(string fileName);
    matrix loadMatrix();
    
    coord loadCoord(string fileName);
    coord loadCoord();
    
	// display functions
    int display(string fileName,char type);
    int display(string fileName);
    int display();
	int display(char type);
}

#endif /* data_hpp */
