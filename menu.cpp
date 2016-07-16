//
//  menu.cpp
//  Contains class definition for menu system, and all the functions for
//  full user interface
//
//  Created by Joshua Brothers on 28/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#include "menu.hpp"

namespace std{
		
		// Following code has been taken from https://support.microsoft.com/en-us/kb/99261
		// Only change is to define hConsole
		/* Standard error macro for reporting API errors */
		#define PERR(bSuccess,api){if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(),api, __LINE__);}

		void clearScreen(){	
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD coordScreen = { 0, 0 };    /* here's where we'll home the
										 cursor */
			BOOL bSuccess;
			DWORD cCharsWritten;
			CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
			DWORD dwConSize;                 /* number of character cells in
												the current buffer */

			/* get the number of character cells in the current buffer */

			bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
			PERR(bSuccess,"GetConsoleScreenBufferInfo");
			dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

			/* fill the entire screen with blanks */

			bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
				dwConSize, coordScreen, &cCharsWritten);
			PERR(bSuccess, "FillConsoleOutputCharacter");

			/* get the current text attribute */

			bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
			PERR(bSuccess, "ConsoleScreenBufferInfo");

			/* now set the buffer's attributes accordingly */

			bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
				dwConSize, coordScreen, &cCharsWritten);
			PERR(bSuccess, "FillConsoleOutputAttribute");

			/* put the cursor at (0, 0) */

			bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
			PERR(bSuccess, "SetConsoleCursorPosition");
			return;
		}
    
    // Constructor for menu class - Calls basic menu functions and allows for all other menu functions to be called
    // depending on user input
    menuClass::menuClass(){
        
		
        // Give intouduction message to user, and define the repeat to be true for future repeats of first menu structure
		cout << "Welcome to the polygon generation and manipulation tool." << endl << endl;
		cout << "Please Note: For all coordinate inputs, seperate the two values by a space, or \npress enter between each value. ";
		cout << "For example, to enter the coordinate (1,2), \nplease type \"1 2\"." << endl;
		cout << endl << "Enter \"b\" at any menu to go back to the main menu." << endl;
		bool repeat = true;
        
        // Do-while loop to display the first menu and then call the correct sub menu function.
        // Forces the user to return to the top level menu after all commands
        do{
			try{
				// Ask the user the question. Uses question function to ensure error checking
				cout << endl;
				int choice1 = question(vector<string>{"What would you like to do?","1. Display Objects" ,"2. Remove Object",
					"3. Generate Shape","4. Manipulate Shape","5. Generate Matrix","6. Generare Coordinate","7. Save Object",
					"8. Load Object","9. Run Info","10. Quit"},1,10);
				clearScreen();
				// Switch statement to direct the user to the correct function depending on their answer to the first level question
				switch (choice1){
					case 1:{
						try{ displayObjects('a'); }
						catch (int){} break; }
					case 2:{removeObject(); break; }
					case 3:{generateShape(); break; }
					case 4:{manipulateShape(); break; }
					case 5:{generateMatrix(); break; }
					case 6:{generateCoordinate(); break; }
					case 7:{
						try{ saveObject(); }
						catch (int){} break; }
					case 8:{
						try{ loadObject(); }
						catch (int){} break; }
					case 10:{
						repeat = quit();
						if (repeat){ break; }
						else{ return; }
					}
					case 9:{runInfo();break;}
				}

			}
			catch (int){

			}
			
			// cin.ignore()/cin.clear() used to cause programme to wait for user to press enter
			cout << "Press enter to return to the main menu" << endl;
			cin.ignore(10000, '\n');
			clearScreen();
        }while (repeat);
    }
    
    // Display objects function - runs through the vector of objects, and depending on the request for object types,
    // displays the relevant objects
    // a = all objects, s = shapes, m = matrices, c = coordinates
	void menuClass::displayObjects(char type){
		// Cout one line to give a nice display method
		//cout << endl;

		// If statement to check that at least one object has been created, and if not throw an error flag
		if (genShapes.size() == 0 && genMatrix.size() == 0 && genCoord.size() == 0) { cout << "No objects found." << endl << endl; throw displayFlag; }

		// If statements to check which input has been given
		// For a, run through each vector and display each piece of information in order
		if (type == 'a') {
			for (size_t i = 0; i < genShapes.size() + genCoord.size() + genMatrix.size(); i++){
				cout << i + 1 << ": ";
				if (i < genShapes.size()){
					cout << "Shape: " << endl << genShapes[i] << endl;
				}
				else if (i >= genShapes.size() && i < genShapes.size() + genCoord.size()){
					cout << "Coordinate: " << endl << genCoord[ i - genShapes.size()] << endl << endl;
				}
				else if (i >= genShapes.size() + genCoord.size()){
					cout << "Matrix: " << endl << genMatrix[ i - genShapes.size()-genCoord.size()] << endl << endl;
				}
			}
		}

		// If statement to run through the shapes vector and display all information for shapes
		if (type == 's'){
			for (size_t i = 0; i < genShapes.size(); i++){
				cout << i + 1 << ": " << endl;
				cout << genShapes[i] << endl;
			}
		}

		// If statement to run through matrices vector and display all information for matrices
		if (type == 'm'){
			for (size_t j = 0; j < genMatrix.size(); j++){
				cout << j + 1 << ": " << endl;
				cout << genMatrix[j] << endl;
			}
		}

		// If statement to run through coordinate vector and display all information for coordinates
		if (type == 'c'){
			for (size_t k = 0; k < genCoord.size(); k++){
				cout << k + 1 << ": " << endl;
				cout << genCoord[k] << endl;
			}
		}
		
	}
    
    // Remove object function to allow the user to remove an object from a vector
    void menuClass::removeObject(){
        
        // First ask the user which type of object they would like to remove
        char objectType = question("What type of object would you like to remove? [s]hape, [m]atrix or [c]oordinate", vector<char> {'s','t','c'});
        int whichObject;
        
        // If - else if statement to display the relevant vector and then ask the user which one to remove, before removing it
        if (objectType == 's'){
            displayObjects('s');
            whichObject = question("Which shape would you like to remove?",1,genShapes.size());
            genShapes.erase(genShapes.begin() + whichObject -1);
        } else if (objectType == 'm'){
            displayObjects('m');
            whichObject = question("Which matrix would you like to remocve?",1,genMatrix.size());
            genMatrix.erase(genMatrix.begin() + whichObject -1);
        } else if (objectType == 'c'){
            displayObjects('c');
            whichObject = question("Which coordinate would you like to remove?",1,genCoord.size());
            genCoord.erase(genCoord.begin() + whichObject -1);
            
        }
    }
    
    // Generate shape function to allow the generation of all shape classes and pushes it back onto the shape vector
    void menuClass::generateShape(){
        // First ask the user which type of shape they would like to generate
        int choice = question(vector<string> {"What shape would you like to generate?",
            "1. Rectangle","2. Isoceles Triangle","3. Pentagon","4. Hexagon","5. Regular Polygon"},1,5);
        
        // try - catch to ensure that a shape has been created successfully
        try {
            // Switch statement to direct to the correct constructor and ask the user to give the relvant information
            switch(choice){
                case 1:{
                    coord r1,r2,r3,r4;
                    cout << "Please enter the coordinates for your rectangle." << endl;
                    cin >> r1 >> r2 >> r3 >> r4;
                    genShapes.push_back(rectangle(r1,r2,r3,r4));
                    break;
                }
                case 2:{
                    coord i1,i2,i3;
                    cout << "Please enter the coordinates for your Isoceles triangle" << endl;
                    cin >> i1 >> i2 >> i3;
                    genShapes.push_back(isoceles(i1, i2, i3));
                    break;
                }
                case 3:{
                    coord p1,p2,p3,p4,p5;
                    cout << "Please enter the coordinates for your pentagon." << endl;
                    cin >> p1 >> p2 >> p3 >> p4 >> p5;
                    genShapes.push_back(pentagon{p1,p2,p3,p4,p5});
                    break;
                }
                case 4:{
                    coord h1,h2,h3,h4,h5,h6;
                    cout << "Please enter the coordinates for your hexagon." << endl;
                    cin >> h1 >> h2 >> h3 >> h4 >> h5 >> h6;
                    genShapes.push_back(hexagon{h1,h2,h3,h4,h5,h6});
                    break;
                }
                case 5:{
                    
                    // For the general constructor ask the user for the relevant information
                    int noEdges;
                    double length;
                    coord c;
					noEdges = question("Please enter the number of edges for your general polygon", 3);

                    // If statemenet to given warning to the user if they decide to generate a shape with an excessively large number of edges.
                    // If they do, gives them the option to continue, or abort. If so throws shape generation flag
                    if (noEdges > 999){
						/*cin.clear();
						cin.ignore(10000, '\n');*/
                        char largeNoEdges = question("Creating polygon with more than 1000 edges is not recomended.Do you wish to continue? [y]es or [n]o",
							vector<char>{'y','n'});
                        if (largeNoEdges == 'n') throw back;
                    }
                    cout << "Please enter one of the coordinates for your general polygon" << endl;
                    cin >> c;
                    cout << "Please enter the edge length for your general polygon" << endl;
                    cin >> length;
					while (length <= 0 || cin.fail()){
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Sorry. Unaccepted Input" << endl;
						cout << "Please enter the edge length for your general polygon" << endl;
						cin >> length;
					}
                    genShapes.push_back(general(noEdges,c,length));
					cin.clear();
					cin.ignore(10000, '\n');
					cout << endl << genShapes.back();
                    break;
                }
            }
            
            // Define a vector for the overlapping points to be held
            vector <coord> overlapPoints;
            
            // For loop to run through every other shape thats been generated and calculate the points that they overlap with the newly created shape
            for (size_t i = 0; i < genShapes.size()-1;i++){
                overlapPoints = genShapes.back().overlap(&genShapes[i]);
                
                // If statement checks to see if the shape overlaps, and if so displays the overap points to the user
                if (overlapPoints.size() !=0){
                    cout << "New shape overlaps with: " << endl;
                    cout << genShapes[i] << endl;
                    cout << "At points: " << endl;
                    for (size_t j = 0; j < overlapPoints.size();j++){
                        cout << j+1 << ": " << overlapPoints[j] << endl;
                    }
                    cout << endl;
                    overlapPoints.clear();
					cin.clear(); cin.ignore(10000, '\n');
                }
            }
            // Catch allows for any errors in generating shapes to be caught and an error displayed
        }catch(int){
			cin.clear();
			cin.ignore(1000, '\n');
            cout << "Shape could not be generated" << endl;
			cout << "Please see above for further information" << endl << endl;
        }
        
    }
    
    // Manipulate shape function displays the shapes and asks the user which one they would like to manipulate
    // and then how they would like to manipulate it
    void menuClass::manipulateShape(){
        // try catch, attempts to display shapes, and should there be an issue, quits the manipulation menu
        try {
            displayObjects('s');
        }catch(int){return;}
        
        // ask the user which shape they would like to manipulate
        char shapeChoice = question("Which shape would like to manipulate?",1,genShapes.size());
        
        // Ask the user how they would like to manipulate it
        char shapeManipChar = question("Would you like to [s]cale, [t]ranslate or [r]otate this shape?", vector<char> {'s','t','r'});
        
        // Switch statement to direct user to the correct functions for shape manipulation
        switch (shapeManipChar){
                
                // For scaling, ask the user relevant information about scaling, and about the point they would like to scale
            case 's':{
                coord scaleCoord,aboutCoord;
                char centreQuestion;
                cout << "By what fator in the x and y coordinates would you like scale your shape?" << endl;
                cin >> scaleCoord;
				cin.clear();
				cin.ignore(1000, '\n');
                
                centreQuestion = question("Would you like to scale your shape about it's centre? [y]es or [n]o",vector<char>{'y','n'});
                
                if (centreQuestion == 'y'){
                    genShapes[shapeChoice - 1].scale(scaleCoord);
                }else {
                    cout << "Around which point would you like to scale your shape?" << endl;
                    cin >> aboutCoord;
					cin.clear();
					cin.ignore(1000, '\n');
                    genShapes[shapeChoice - 1].scale(scaleCoord,aboutCoord);
                }
                break;
                
            }
                
                // For translattion, ask the user how they would like to translate in each direction and then translate the shape
            case 't':{
                double x,y;
                
                x = question("How would you like to translate your shape in the x direction?");
                y = question("How would you like to translare your shape in the y direction?");
                genShapes[shapeChoice-1].translate(x,y);
                break;
            }
                
                // For rotation, ask the user if they would like to rotate about the centre of the shape, and whether they would like to do it by
                // angle or matrix
            case 'r':{
                double angle;
                char centreQuestion;
                char angleOrMatrix;
				coord rotatePoint;
                
                centreQuestion = question("Would you like to rotate about the centre of the shape? [y]es or [n]o",vector<char>{'y','n'});

				if (centreQuestion == 'y'){
					rotatePoint = centreOfShape(genShapes[shapeChoice - 1].getAllCoord());
				}
				else {
					cout << "About what point would you like to rotate?" << endl;
					cin >> rotatePoint;
					cin.clear();
					cin.ignore(10000, '\n');
				}

                angleOrMatrix = question("Would you like to rotate by [a]ngle or [m]atrix?",vector<char>{'a','m'});
                
                // If the user would like to rotate by angle, ask what angle, and then rotate the shape
                // If they earlier specified to rotate about a point != centre, now ask them for the point
                if (angleOrMatrix == 'a'){
                    angle = question("By what angle would you like to rotate your shape?");
                    genShapes[shapeChoice-1].rotate(angle, rotatePoint);
                    
                } else {
                   // If the user would like to rotate by matrix, ask them if they would like to use a previouslt constructed matrix, or generate a new one
					char ownMatrix;
					if (genMatrix.size() == 0) ownMatrix = '2';
					else{
						ownMatrix = question("Would you like to rotate by 1. Previouly Created Matrix, 2. A new one?", vector < char > {'1', '2'});
					}
					matrix temp;
                    // If they would like to use their own matrix, display the previously created matrices, and ask them which one they would like
                    // to use
                    if (ownMatrix == '1'){
                        
                        // try catch ensures that a matrix has been previouslt generated, and if not, asks them if they would like to make one
                        try{
                            displayObjects('m');
                            int matrixChoice = question("Which matrix would you like to rotate by?",1,genMatrix.size());
                            temp = genMatrix[matrixChoice-1];
                        }catch(int){
                            char createMatrix = question("You've not created any matrices yet! Would you like to make one? [y]es or [n]o",
                                                         vector<char>{'y','n'});
                            
                            // If they would like to make one, create a matrix and push it back onto generated matrices
                            if (createMatrix == 'n') return;
                            else{
                                cout << "Please enter your matrix below:" << endl;
                                cin >> temp;
                                genMatrix.push_back(temp);
                            }
                        }
                        
                        // If user opted to create a new matrix, let them create it, and push it back onto previously generated matrices
                    }else {
                        cout << "Please enter your matrix below:" << endl;
                        cin >> temp;
                        genMatrix.push_back(temp);
                    }
					//cin.clear();
					//cin.ignore(1000, '\n');
                    genShapes[shapeChoice-1].rotate(temp, rotatePoint);
                }
                break;
            }
        }
		cout << "Shape is now at the following coordinates: " << endl << genShapes[shapeChoice - 1];
    }

    
    // Generate new matrix function - Push it back onto generated matrices vector
    void menuClass::generateMatrix(){

		char matrixQuestion = question("Would you like to create a [r]otation matrix from an angle in radians, or a \n[n]ormal 2x2 matrix?", vector < char > {'r', 'n'});

		if (matrixQuestion == 'r'){
			double angle;
			cout << "Please enter an angle in radians" << endl;
			cin >> angle;
			if (cin.fail() || angle < 0 || angle > 2 * pi){
				cout << "Sorry. Unaccepted input" << endl;
				cout << "Please enter an angle in radians" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				cin >> angle; 
			}
			cin.clear();
			cin.ignore(1000, '\n');
			genMatrix.push_back(matrix(angle));
			cout << genMatrix.back();
		}
		else{
			matrix temp;
			cout << "Please enter your matrix." << endl;
			cin >> temp;
			cin.clear();
			cin.ignore(10000, '\n');
			genMatrix.push_back(temp);
		}
    }
    
    // Generate new coordinate functiojn - Push it back onto generated shapes vector
    void menuClass::generateCoordinate(){
        coord temp;
        cout << "Please enter your coordinate" << endl;
        cin >> temp;
        genCoord.push_back(temp);
    }
    
    // Save object function - Allows the user to save any generated object to the standard save file
    void menuClass::saveObject(){
        // Ask the user what type of object they would like to save
        char objectChoice;
        objectChoice = question("Would you like to save [a]ll objects, a [s]hape, [m]atrix or [c]oordinate?",vector<char>{'a','s','m','c'});
        
        // try catch allows for display errors - Such as the user has generated any of that type of object yet
        try{
            
            // Switch statement directs user to the correct type of object to save.
            // 'a' means all different shapes
            switch (objectChoice){
                case 'a':{
                    for (size_t i = 0; i < genShapes.size(); i++){
                        files::saveShape(genShapes[i]);
                    }
                    for (size_t i = 0; i < genMatrix.size();i++){
                        files::saveMatrix(genMatrix[i]);
                    }
                    for (size_t i = 0; i < genCoord.size(); i++){
                        files::saveCoordinate(genCoord[i]);
                    }
                    
                    break;
                }
                case 's':{
                    displayObjects('s');
                    
                    int shapeChoice = question("Which shape you like to save?",1,genShapes.size());
                    files::saveShape(genShapes[shapeChoice-1]);
                    break;
                    
                }
                case 'm':{
                    displayObjects('m');
                    int matrixChoice = question("Which matrix would you like to save?",1,genMatrix.size());
                    files::saveMatrix(genMatrix[matrixChoice-1]);
                    break;
                }
                case 'c':{
                    displayObjects('c');
                    int coordChoice = question("Which coordinate would you like to save?",1,genCoord.size());
                    files::saveCoordinate(genCoord[coordChoice-1]);
                    break;
                }   
            }
        }catch(int){}
		
    }
    
    // Load object function allows the user to load an object from the standard save file
    void menuClass::loadObject(){
        // First ask the user which object they would like to load
        char objectChoice = question("Would you like to load a [s]hape, [a]ll shapes, a [m]atrix or a [c]oordinate?",vector<char>{'s','m','c','a'});
        
        // try catch ensures that file could be loaded, and if not displays an error
        try {
            
            // If else statement directs to the correct object list and loads the required objects
            // 'a' allows for all shapes to be loaded in one go
            if (objectChoice == 'a'){
                vector<polygon> temp = files::loadAllShapes();
                genShapes.insert(genShapes.end(),temp.begin(),temp.end());
                temp.clear();
            }
            else if (objectChoice == 's'){
                genShapes.push_back(files::loadOneShape());
            }else if (objectChoice == 'm'){
                genMatrix.push_back(files::loadMatrix());
            }else if (objectChoice == 'c'){
                genCoord.push_back(files::loadCoord());
            }
        } catch (int flag){
            
            if (flag == -1) cout << "Couldn't find file" << endl;
            if (flag == -2) cout << "No files saved!" << endl;
            
        }
    }
    
    void menuClass::runInfo(){
        if (genShapes.empty() && genMatrix.empty() && genCoord.empty()){
            cout << "No objects generated yet" << endl;
        }
        else{
            cout << "Total objects generated: " << endl << endl;
            if (!genShapes.empty()) cout << "Shapes: " << genShapes[0].getNumberCreated() << endl;
            if (!genMatrix.empty()) cout << "Matrices: " << genMatrix[0].getNumberCreated() << endl;
            
            // Nested If statements to ensure that a generated coordinate can be found, regardless of
            // whether it is part of a shape or a stand alone coordinate
            if (!genCoord.empty() || !genShapes.empty()) {
                if (genCoord.empty()) cout << "Coordinates: :" << genShapes[0].getCoord(0).getNumberCreated() << endl;
                else cout << "Coordinates: :" << genCoord[0].getNumberCreated() << endl;
                cout << "Please not coordinate totals include those used to make shapes, and those \ngenerated for overlap testing." << endl;
            }
        }
    }
    
    // Quit function aks the user if they want to quit, and if so allows them to save all of work if neccessary
    bool menuClass::quit(){
        // Askl the user if they want to quit, and if so, return true
        char quitQuestion = question("Are you sure you want to quit? [y]es or [n]o",vector<char>{'y','n'});
        if (quitQuestion =='n') return true; // True is used instead of false as the repeat in menu class is used as false to repeat again.
        
        // Else asks the user if they want to save and then returns fale to quit the programme
        else {
            
            char saveQuestion = question("Would you like to save all before quitting? [y]es or [n]o",vector<char>{'y','n'});
            if (saveQuestion == 'y'){
                
                for (size_t i = 0; i < genShapes.size(); i++){
                    files::saveShape(genShapes[i]);
                }
                for (size_t i = 0; i < genMatrix.size();i++){
                    files::saveMatrix(genMatrix[i]);
                }
                for (size_t i = 0; i < genCoord.size(); i++){
                    files::saveCoordinate(genCoord[i]);
                }
            }
            
            return false;
        }
    }
    
    // Question functions allow for repeated error checking
    // Most use a given string and then either available options given in the form
    // of a vector in the case of character outputs, or as upper and lower bounds
    // for the case of numerical answers
	//
	// size_type has been used to give the position data stops being converted for stoi
	// This can then be used to check that text hasn't been entered after a numeric input, or a
	// space placed between numbers etc.
    char menuClass::question(string question, vector<char> options){
        char input;
        string inputString;
        cout << question << endl;
		getline(cin, inputString);
		if (inputString == "b") throw back;
        input = inputString[0];
        while (inputString.length() != 1 || cin.fail() || find(options.begin(),options.end(),input) == options.end()){
            cout << "Sorry, unaccepted input" << endl;
            cout << question << endl;
			getline(cin, inputString);
            input = inputString[0];
        }
        return input;
    }
    
    int menuClass::question(string question,size_t lowerBound, size_t higherBound){
		string inputString;
		string::size_type sz;
		size_t input;
		bool cannotConvert = false;
		cout << question << endl; 
		getline(cin, inputString);
		if (inputString == "b") throw back;
		try{ input = stoi(inputString, &sz); }
		catch (invalid_argument){
			cannotConvert = true; // Used to get into while loop
		}
		while (cin.fail() || input < lowerBound || input > higherBound || inputString.substr(sz).length() != 0 || cannotConvert){
			cout << "Sorry unaccepted input" << endl;
			cout << question << endl;
			getline(cin, inputString);
			try{ input = stoi(inputString, &sz); }
			catch (invalid_argument){
				cannotConvert = true;
			}
		}

		return input;
    }

	
    int menuClass::question(vector<string> question, size_t lowerBound, size_t higherBound){

        string inputString;
        string::size_type sz;
        size_t input;
		bool cannotConvert = false;
        for (size_t i = 0; i < question.size(); i ++){cout << question[i] << endl;}
		getline(cin, inputString);
		if (inputString == "b") throw back;
        try{input = stoi(inputString,&sz);}
        catch(invalid_argument){
            cannotConvert = true; // Used to get into while loop
        }
		while (cin.fail() || input < lowerBound || input > higherBound || inputString.substr(sz).length() != 0 || cannotConvert){
			cout << "Sorry unaccepted input" << endl;
			for (size_t i = 0; i < question.size(); i++){ cout << question[i] << endl; }
			getline(cin, inputString);
			if (inputString == "b") throw back;
			try{ input = stoi(inputString, &sz); }
			catch (invalid_argument){
				cannotConvert = true;
			}

		}
        return input;
    }
    
	int menuClass::question(string question, size_t lowerBound){
		string inputString;
		string::size_type sz;
		size_t input;
		bool cannotConvert = false;
		cout << question << endl;
		getline(cin, inputString);
		if (inputString == "b") throw back;
		try{ input = stoi(inputString, &sz); }
		catch (invalid_argument){
			cannotConvert = true; // Used to get into while loop
		}
		while (cin.fail() || input < lowerBound || inputString.substr(sz).length() != 0|| cannotConvert){
			cout << "Sorry unaccepted input" << endl;
			cout << question << endl;
			getline(cin, inputString);
			if (inputString == "b") throw back;
			try{ input = stoi(inputString, &sz); }
			catch (invalid_argument){
				cannotConvert = true;
			}

		}
		return input;
		
	}
    double menuClass::question(string question){
		string inputString;
        double input;
		string::size_type sz;
		bool cannotConvert = false;
        cout << question << endl;
		getline(cin, inputString);
		if (inputString == "b") throw back;
		try{ input = stod(inputString, &sz); }
		catch (invalid_argument){
			 cannotConvert = true;
		}

        while (cin.fail() || cannotConvert){
            cout << "Sorry unaccepted input" << endl;
            cout << question  << endl;
			getline(cin, inputString);
			if (inputString == "b") throw back;
			try{ input = stod(inputString, &sz); cannotConvert = false; }
			catch (invalid_argument){
				cannotConvert = true;
			}
        }
        return input;
    }

    
    // Destructor clears the vectors of all information
    menuClass::~menuClass(){
        genCoord.clear();
        genMatrix.clear();
        genShapes.clear();
    }   
}