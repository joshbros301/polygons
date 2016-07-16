//
//  files.cpp
//  Includes all file manipulation functions for use with polygons
//  project, including displaying information from a file, loading,
//  and saving shapes.
//
//  Created by Joshua Brothers on 27/04/2016.
//  Copyright © 2016 Joshua Brothers. All rights reserved.
//

#include "files.hpp"

using namespace std;

namespace files{
    
    // save shape function to save a given shape to a given file for later retrival
    void saveShape(polygon shape, string fileName){
        
        // Define output file stream and a location for the file
        ofstream myFile;
        string fileString = fileName + ".txt";
        
        // Open the file
        // ios::app to append to the end of the file by default
        myFile.open (fileString,ios::app);
        
        // If the file cannot be opened display error and throw error flag
        if(!myFile.is_open()){cerr << "File could not be opened!" << endl; throw(fstreamFlag);}
        
        // Display to the user which file is to be saved and then specify on the file
        // that the object is a shape
        cout << "Writing " << shape.getType() << " to file." << endl;
        myFile << "Shape" << endl;
        
        // try catch to determine whether the type is a general polygon or a standard one
        // This is possible as all general ones begin with an integer, whereas all others are
        // text only - Thus stoi will return invalid_argument if it is not a general
        // Saves the number of edges if it is a general, and the first letter for standard polygons
        try{
            myFile << stoi(shape.getType().substr(0,10)) << endl;
        }catch(invalid_argument){
            myFile << shape.getType().substr(0,1) << endl;
        }
        
        // For loop runs through each coordinate of the shape and saves them into the file
        for (int i = 0; i < shape.getNoVertices();i++){
            coord temp = shape.getCoord(i);
            myFile << temp.getx() << "," << temp.gety() << endl;
        }
        
        // Save end to the file so the end point can be easily located, and then close the file
        myFile << "end" << endl;
        myFile.close();
        
    }
    
    // Function to save shape to standard file calling saveShape above
    void saveShape(polygon shape){
        saveShape(shape,"polygonsData");
    }
    
    // Save matrix function to allow the saving of a matrix to a file
    void saveMatrix(matrix m, string fileName){
        
        // Define ofstream and sting ot give the files location
        ofstream myFile;
        string fileString = fileName + ".txt";
        
        // open the file
        // ios::app to append to the end of the file by default
        myFile.open (fileString,ios::app);
        
        // If statement checks the file is open and if not throws file exception flag
        if(!myFile.is_open()){cerr << "Could not find file!" << endl; throw fstreamFlag;}
        
        // Tell the user that a matrix is being written to file and ten write matrix to the file
        // for identification
        cout << "Writing matrix to file" << endl;
        myFile << "Matrix" << endl;
        
        // for loop to run through each element of matrix and save it to the file with and end line
        //between each element
        for (int i = 0; i < 4; i++){
            myFile << m.getData(i) << endl;
        }
        
        // Add end to the end of the file to define the end of the file and then close it
        myFile << "end" << endl;
        myFile.close();
    }
    
    // Function to save matrix to standard file using save matrix function above
    void saveMatrix(matrix m){
        saveMatrix(m,"polygonsData");
    }
    
    // Function to save coordinates to file
    void saveCoordinate(coord c,string fileName){
        
        // Define ostream for file and a string for the location of the file
        ofstream myFile;
        string fileString = fileName + ".txt";
        
        // open the file and check that it is open
        //ios:app to append to the end of the fil by default
        myFile.open (fileString,ios::app);
        if(!myFile.is_open()){ cerr << "Could not find the file!" << endl; throw fstreamFlag;}
        
        // Tell the user we're writing the coordinate, and then write the word coordinate to  file for
        // identification followed by the word end and then close the file
        cout << "Writing coordinate to file" << endl;
        myFile << "Coordinate" << endl;
        myFile << c.getx() << "," << c.gety() << endl;
        myFile << "end" << endl;
        myFile.close();
        
    }
    
    // Function to save coordinate to standard file using save coordinate function above
    void saveCoordinate(coord c){
        saveCoordinate(c,"polygonsData");
    }
    
    // display function to read in a file and display each object contained in that file.
    // type char allows specification of object type to be displayed
    int display(string fileName,char type){
        
        // Define an input stream for the file,specify the location of the file
        ifstream myFile;
        string fileString = fileName + ".txt";
        
        // open the file and check if it is open. If not throw an error
        myFile.open(fileString);
        if(!myFile.is_open()){throw (fstreamFlag);}
        
        // Define lines for input when reading the file and a counter for the number of objects
        string line;
        int objectCounter = 0;
        
        if (myFile.peek() == ifstream::traits_type::eof()) throw emptyFileFlag;
        
        // while loop to run through each line of the file
        while(getline(myFile,line)){
            
            
            // If statement to determine whether we are looking at a shape in the file and whether
            // we want to be display shapes
            // If so run through the information and display it
            if (line == "Shape" && (type =='a' || type == 's')){
                objectCounter++;
                cout << objectCounter << ": ";
                getline(myFile,line);
                
                // If else statement to check the type of the object
                if(isdigit(line[0])){
                    cout << line << " sided Regular Polygon" << endl;
                } else {
                    if(line[0] == 'r') cout << "rectangle" << endl;
                    else if(line[0] == 'p') cout << "pentagon" << endl;
                    else if(line[0] == 'i') cout << "isoceles triangle" << endl;
                    else if(line[0] == 'h') cout << "hexagon" << endl;
                    else {cerr <<"Error identifying shape" << endl; throw fstreamFlag;}
                }
                
                // Use this to get a new line to ignore the number/ letter that specifies the object type stored
                getline(myFile,line);
                int coordinateCounter = 1;
                // While loop to run throuch each coordinate of the polygon in the file and display the information
                while(line !="end"){
                    
                    cout << coordinateCounter << ": (" << line << ")" << endl;
                    getline(myFile,line);
                    coordinateCounter++;
                }
                
                cout << endl;
                
                // Similar to above but using a coordinate
                // Simpler as coordinates don't have multiple types
            }else if (line == "Coordinate" && (type =='a' || type == 'c')){
				objectCounter++;
                cout << objectCounter << ": Coordinate: ";
                getline(myFile,line);
                cout << "(" << line << ")" << endl;
                cout << endl;
                
                // Same again with matrices
            }else if (line =="Matrix" && (type =='a' || type == 'm')){
				objectCounter++;
                cout << objectCounter << ": Matrix: " << endl;
                getline(myFile,line);
                cout << line << " ";
                getline(myFile,line);
                cout << line << endl;
                getline(myFile,line);
                cout << line << " ";
                getline(myFile,line);
                cout << line << endl;
                cout << endl;
            }
        }
        
        // Close the file, add a line for legibility, and return the number of objects looked at
        myFile.close();
        cout << endl;
        return objectCounter;
    }
    
    // display function to show all elements. Uses above function
    int display(string fileName){
        return display(fileName,'a');
    }
    
    // Displau function to show all elements in the standard file. Uses above function
    int display(){
        return display("polygonsData",'a');
    }
    
    // Display function to show object types specified from the standard file.
    // Uses above function
    int display(char type){
        return display("polygonsData",type);
    }
    
    // Load shape function to load a given shape from a given file
    polygon loadShape(int inputShape, string fileName){
        
        // Define an istream and open the file from the given file name
        ifstream myFile;
        string fileString = fileName + ".txt";
        
        // Open the file and check the file is open
        myFile.open(fileString);
        if(!myFile.is_open()){ throw fstreamFlag;}
        
        // Define a string to hold the shape type and each line read in from the file
        string line;
        string shapeType;
        
        // Define a vector to hold the coordinate data for inputted coordinates and a counter
        // to find the requested shape
        vector<coord> coordData;
        int counter = 0;
        
        // While loop to take a line from the file and find the required information
        while (getline(myFile,line)){
            
            // If statement to find when the file says shape as the start of shape information
            // Increase the counter by one to state which shape this is
            if (line == "Shape"){
                counter++;
                
                // If this is the correct shape begin loading the shape
                if (counter == inputShape){
                    
                    // Take the first line, and set it to the shape type
                    getline(myFile,shapeType);
                    
                    // Take another line which will be the first line of coordinates
                    string stringCoords;
                    getline(myFile,stringCoords);
                    
                    // While loop to reach through all of the information until the end flag is found
                    while (stringCoords != "end"){
                        
                        // Find the commas in each line that seperate the coordinates
                        size_t commaPos = stringCoords.find(",",0);
                        
                        // Set x and y to the string on the line each side of the comma using stod
                        // to convert as a double, and push them back as a coordinate onto the coordinate
                        // data vector
                        double x = stod(stringCoords.substr(0,commaPos));
                        double y = stod(stringCoords.substr(commaPos+1));
                        coordData.push_back(coord{x,y});
                        
                        // get a new line for the next iteration of the while loop
                        getline(myFile,stringCoords);
                    }
                    
                    // Close the file and break from the while loop[ as the shape has been found
                    myFile.close();
                    break;
                }
            }
        }
        
        // If else statement to determine which constructor to use to crete the shape from file information
        // Uses the first letter of the shape to determine
        // If the first character is a number, then it must be a general polygon and the else statement at
        // the bottom is called
        if (shapeType=="r"){
            return rectangle(coordData[0],coordData[1],coordData[2],coordData[3]);
        }else if (shapeType == "p"){
            return pentagon(coordData[0],coordData[1],coordData[2],coordData[3],coordData[4]);
        }else if (shapeType == "h"){
            return hexagon(coordData[0],coordData[1],coordData[2],coordData[3],coordData[4],coordData[5]);
        }else if (shapeType == "i"){
            return isoceles(coordData[0],coordData[1],coordData[2]);
        }else {
            int generalSize = stoi(shapeType);
            
            // Check that the size read in is the same as the number of coordinates for general shapes
            // If not throw an error
            if (generalSize != coordData.size()){
                cerr << "Error reading file!" << endl;
                throw fstreamFlag;
            }
            
            // Determine the edge length as the distnace between two coordinates
            double edgeLength = length(coordData[0],coordData[1]);
            
            // Use the general constructor to regenerate the shape from the given information
            return general(generalSize,coordData[0],edgeLength);
            
        }
    }
    
    // Function to load a given shape from standard file
    polygon loadShape(int inputShape)
    {
        return loadShape(inputShape, "polygonsData");
    }
    
    // Function to load all shapes from a given file name
    vector<polygon> loadAllShapes(string fileName){
        
        // Vector of polygons to hold all of the loaded shapes
        vector<polygon> shapes;
        cout << "Loading all shapes: " << endl;
        
        // use display to show all of the shapes to be loaded and calculate how many shapes there are in the file
        int noObjects = display(fileName,'s');
        
        // for loop to run through each shape in the file and push them back onto the vector shapes
        for (int i = 1; i <= noObjects; i++){
            shapes.push_back(loadShape(i));
        }
        return shapes;
    }
    
    // Function to load all shapes from standard file
    // Uses function defined above
    vector<polygon> loadAllShapes(){
        return loadAllShapes("polygonsData");
    }
    
    // Function to load one specific shape from given file
    polygon loadOneShape(string fileName){
        
        // First calculate the number of shapes in the file, and display each on to the user
        int noObjects = display(fileName,'s');
		if (noObjects == 0){
			cout << "No shapes to load" << endl;
			throw back;
		}
        
        // Ask the user which shape they wwould like to load and check for correct input
        cout  << "Which Shape would you like to load?" << endl;
        int inputShape;
        string inputShapeString;
        cin >> inputShapeString;
		if (inputShapeString == "b") {
			cin.clear();
			cin.ignore(10000, '\n');
			throw back;
		}
        // Pointer to string position
        string::size_type sz;
        try{
            inputShape = stoi(inputShapeString,&sz);} // &sz generates a pointer for the position where the numbers stop
        catch(invalid_argument){
            inputShape = 0;
        }
        while (cin.fail() || inputShape < 1 || inputShape > noObjects || inputShapeString.substr(sz).length()!=0){
            cout << "Sorry unaccpeted input." << endl;
            cout << "Which shape would you like to load?" << endl;
            cin.clear();
            cin.ignore(10000,'\n');
            cin >> inputShapeString;
			if (inputShapeString == "b") {
				cin.clear();
				cin.ignore(10000, '\n');
				throw back;
			}
            try { inputShape = stoi(inputShapeString,&sz);}
            catch(invalid_argument){
                inputShape = 0;
            }
        }
        cin.clear();
        cin.ignore(10000,'\n');
        
        // return the loaded shape.
        return loadShape(inputShape,fileName);
    }
    
    // Function to load one shape from the standard file
    // Calls the above function
    polygon loadOneShape(){
        return loadOneShape("polygonsData");
    }
    
    // Function to load matrices from a given file name
    matrix loadMatrix(string fileName){
        
        // Display all the matrices in the file and count the number of them
        int noObjects = display(fileName,'m');
		if (noObjects == 0){
			cout << "No matrices to load" << endl;
			throw back;
		}
        
        // Ask the user which matrix they would like to load and check for errorless user input
        cout << "Which matrix would you like to load?" << endl;
        string inputMatrixString;
        cin >> inputMatrixString;
		if (inputMatrixString == "b") {
			cin.clear();
			cin.ignore(10000, '\n');
			throw back;
		}
        string::size_type sz;
        int inputMatrix;
        try{inputMatrix = stoi(inputMatrixString,&sz);}
        catch(invalid_argument){
            inputMatrix = 0;
        }
        while (cin.fail() || inputMatrix < 1 || inputMatrix > noObjects || inputMatrixString.substr(sz).length() !=0){
            cout << "Sorry. Unaccepted input" << endl;
            cout << "Which matrix would you like to load?" << endl;
            cin.clear();
            cin.ignore(10000,'\n');
            cin >> inputMatrixString;
			if (inputMatrixString == "b") {
				cin.clear();
				cin.ignore(10000, '\n');
				throw back;
			}
            try{inputMatrix = stoi(inputMatrixString,&sz);}
            catch(invalid_argument){
                inputMatrix = 0;
            }
        }

		cin.clear();
		cin.ignore(10000, '\n');
        
        // Define input steam and the location of the file
        ifstream myFile;
        string fileString = fileName + ".txt";
        
        // Open the file and check it has succesffully openeed
        myFile.open(fileString);
        if(!myFile.is_open()){throw fstreamFlag;}
        string line;
        
        // Define a counter and start a while loop to reach through the file line by line
        int counter = 0;
        while (getline(myFile,line)){
            
            // If statement to flag when a matrix has been found
            if (line == "Matrix"){
                // Increase the counter by one, and check if the matrix is the one asked for by the user
                // If so load the information into doubles and return the matrix it forms
                counter++;
                if (counter == inputMatrix){
                    getline(myFile,line);
                    double x1 = stod(line);
                    getline(myFile,line);
                    double x2 = stod(line);
                    getline(myFile,line);
                    double x3 = stod(line);
                    getline(myFile,line);
                    double x4 = stod(line);
                    return matrix(x1,x2,x3,x4);
                }
            }
        }
        
        // Throw error if the requested matrix could not be found
        cerr << "Couldn't find the matrix you requested!" << endl;
        throw fstreamFlag;
    }
    
    // Function to load matrix from standard file.
    // Calls the function above
    matrix loadMatrix(){
        return loadMatrix("polygonsData");
    }
    
    // Function to load coordinate from a given file
    coord loadCoord(string fileName){
        
        // Display the available coordinates and calculate how many there are, then display them to the user
        int noObjects = display(fileName,'c');
		if (noObjects == 0){
			cout << "No Coordinates to load" << endl;
			throw back;
		}
        // Ask the user which coordinate they would like to load, and do error checking on the input
        cout << "Which coordinate would you like to load?" << endl;
        string inputCoordString;
        string::size_type sz;
        cin >> inputCoordString;
        int inputCoord;
        try{inputCoord = stoi(inputCoordString,&sz);}
        catch(invalid_argument){
            inputCoord = 0;
        }
        while (cin.fail()|| inputCoord < 1 || inputCoord > noObjects||inputCoordString.substr(sz).length() !=0){
            cout << "Sorry unaccepted input" << endl;
            cout << "Which coordinare would you like to load?" << endl;
            cin.clear();
            cin.ignore(10000,'\n');
            cin >> inputCoordString;
			if (inputCoordString == "b") {
				cin.clear();
				cin.ignore(10000, '\n');
				throw back;
			}
            try{inputCoord = stoi(inputCoordString,&sz);}
            catch(invalid_argument){
                inputCoord = 0;
            }
        }

		cin.clear();
		cin.ignore(10000, '\n');
        
        // Declare an input file stream and the location of the file
        ifstream myFile;
        string fileString =  fileName + ".txt";
        
        // Open the file and check that the file has been opened
        myFile.open(fileString);
        if (!myFile.is_open()){throw fstreamFlag;}
        
        // Define a string to take in each line and a counter to ensure the correct
        // coordinate is checked
        string line;
        int counter = 0;
        
        // While loop takes in each line at a time in the text file, and then an if statement checks until a
        // coordinate has been found. Counter is then increased by one so that the correct one can be determined
        while(getline(myFile,line)){
            if (line == "Coordinate"){
                counter++;
                
                // If statement checks when the corret coordinate has been found
                if (counter == inputCoord){
                    
                    // Get the line after, determine the comma position on the line, and define each side of it
                    // as the elements of the new coordinte
                    getline(myFile,line);
                    size_t commaPos = line.find(",",0);
                    double x = stod(line.substr(0,commaPos));
                    double y = stod(line.substr(commaPos+1));
                    return coord{x,y};
                    
                }
                
            }
        }
        cerr << "Error: Coordinate could not be found!" << endl;
        throw fstreamFlag;
    }
    
    // Function to load coordinate from the standard coordinate file
    // Uses the above function
	coord loadCoord(){
		return loadCoord("polygonsData");
	}
}