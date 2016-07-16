//
//  menu.hpp
//
//  Created by Joshua Brothers on 28/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#ifndef run_hpp
#define run_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include "files.hpp"

// Define constant for error flag in display function
const int displayFlag{-1};



namespace std{
    
    class menuClass{
    private:
        
        // Vectors to hold generated objects 
        std::vector <std::polygon> genShapes;
        std::vector <std::coord> genCoord;
        std::vector <std::matrix> genMatrix;
    public:
        
        // Conastructor and Destructor
        menuClass();
        ~menuClass();
        
        // Menu functions - Used for splitting up the menu into various functions
        void menu();
        void displayObjects(char type);
        void removeObject();
        void generateShape();
        void manipulateShape();
        void generateMatrix();
        void generateCoordinate();
        void saveObject();
        void loadObject();
        void runInfo();
        bool quit();
        
        // Question functions to allow for question asks and error handing to be handled in an outside
        // function to the menu system itself
        char question(string question, vector<char> options);
        int question(string question, size_t lowerBound, size_t higherBound);
        int question(vector<std::string> question, size_t lowerBound, size_t higherBound);
		int question(string quedstion, size_t lowerBound);
        double question(string question);
    };
}
#endif /* run_hpp */
