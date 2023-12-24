#include <iostream>
#include <string>
#include <fstream>
#include "constants.h"
#include "image.h"

using namespace std;

void readImage(istream &is, Image &imI){
    string code;
    char endline;
    int M_value;
    getline(is, code);                                                                      //read the header of the input image
    is >> imI.w;
    is >> imI.h;
    is >> M_value;
    is.get(endline);                                                                            //read the "\n"
    for (unsigned int i = 0; i < imI.h; ++i) {
        for (unsigned int j = 0; j < imI.w; ++j) {
            is.read((char*)&imI.image[i][j].red, 1);                                         //read the image content
            is.read((char*)&imI.image[i][j].green, 1);
            is.read((char*)&imI.image[i][j].blue, 1);
        }
    }
}

void writeImage(ostream &os, const Image &imI){
    os << "P6" << endl << imI.w << ' ' << imI.h << endl << "255" << endl;                          //write the header of the output image
    for (unsigned int i = 0; i < imI.h; ++i) {
        for (unsigned int j = 0; j < imI.w; ++j) {
            os << imI.image[i][j].red << imI.image[i][j].green << imI.image[i][j].blue;            //write the image content in sequence of RGB
        }
    }
}

void verticalFlip(const Image &imI, Image &imJ){
    for (unsigned int i = 0; i < imI.h; ++i) {
        for (unsigned int j = 0; j < imI.w; ++j) {
            imJ.image[i][j].red = imI.image[imI.h - i - 1][j].red;                                 //flip the image vertically
            imJ.image[i][j].green = imI.image[imI.h - i - 1][j].green;
            imJ.image[i][j].blue = imI.image[imI.h - i - 1][j].blue;
        }
    }
    imJ.h = imI.h;
    imJ.w = imI.w;
}

void rotate90(const Image &imI, Image &imJ){
    for (unsigned int i = 0; i < imI.w; ++i) {
        for (unsigned int j = 0; j < imI.h; ++j) {
            imJ.image[i][j].red = imI.image[imI.h - j - 1][i].red;                                 //rotate the image by 90° (clockwise)
            imJ.image[i][j].green = imI.image[imI.h - j - 1][i].green;
            imJ.image[i][j].blue = imI.image[imI.h - j - 1][i].blue;
        }
    }
    imJ.h = imI.w;
    imJ.w = imI.h;
}

void intensityInversion(const Image &imI, Image &imJ){
    for (unsigned int i = 0; i < imI.h; ++i) {
        for (unsigned int j = 0; j < imI.w; ++j) {
            imJ.image[i][j].red = char(M) - imI.image[i][j].red;                                  //inverse the intensity of the image
            imJ.image[i][j].green = char(M) - imI.image[i][j].green;
            imJ.image[i][j].blue = char(M) - imI.image[i][j].blue;
        }
    }
    imJ.h = imI.h;
    imJ.w = imI.w;
}

typedef unsigned char (*Agg)(const unsigned char values[KERNEL_SIZE][KERNEL_SIZE]);
void filter(const Image &imI, Image &imJ, Agg f){
    unsigned char valuesR[KERNEL_SIZE][KERNEL_SIZE];
    unsigned char valuesG[KERNEL_SIZE][KERNEL_SIZE];
    unsigned char valuesB[KERNEL_SIZE][KERNEL_SIZE];
    for (unsigned int i = 0; i < imI.h; ++i) {
        for (unsigned int j = 0; j < imI.w; ++j) {
            for (int m = 0; m < KERNEL_SIZE; ++m) {
                for (int n = 0; n < KERNEL_SIZE; ++n) {
                    if (i - 1 + m < 0 || j - 1 + n < 0 || i - 1 + m >= imI.h || j - 1 + n >= imI.w){
                        valuesR[m][n] = 0;                                                     //check whether the pixel is at the boundary of the image
                        valuesG[m][n] = 0;                                                     //if so, value of the pixels that go beyond the boundary should be zero
                        valuesB[m][n] = 0;
                    }
                    else{                                                                      //otherwise, create a 3*3 matrix around each pixel
                        valuesR[m][n] = imI.image[i - 1 + m][j - 1 + n].red;
                        valuesG[m][n] = imI.image[i - 1 + m][j - 1 + n].green;
                        valuesB[m][n] = imI.image[i - 1 + m][j - 1 + n].blue;
                    }
                }
            }
            imJ.image[i][j].red = f(valuesR);                                                  //assign the value to the pixel by calling corresponding function
            imJ.image[i][j].green = f(valuesG);
            imJ.image[i][j].blue = f(valuesB);
        }
    }
    imJ.h = imI.h;
    imJ.w = imI.w;
}
unsigned char maxFilter(const unsigned char values[KERNEL_SIZE][KERNEL_SIZE]){
    int valuesNew[KERNEL_SIZE * KERNEL_SIZE];
    int k = 0;
    for (int i = 0; i < KERNEL_SIZE; ++i) {                                                    //convert the 3*3 matrix into a new char with length 9
        for (int j = 0; j < KERNEL_SIZE; ++j) {
            if (int(values[i][j]) < 0){
                valuesNew[k] = int(values[i][j]) + 256;
            }
            else{
                valuesNew[k] = int(values[i][j]);
            }
            k++;
        }
    }
    int max = valuesNew[0];
    for (int i = 0; i < k; ++i) {
            if (valuesNew[i] > max){
                max = valuesNew[i];                                                            //return the maximum value of the 3*3 matrix
            }
    }
    return char(max);
}
unsigned char meanFilter(const unsigned char values[KERNEL_SIZE][KERNEL_SIZE]){
    unsigned char mean;
    int sum = 0;
    for (int i = 0; i < KERNEL_SIZE; ++i) {
        for (int j = 0; j < KERNEL_SIZE; ++j) {
            if (int(values[i][j]) < 0){
                sum += int(values[i][j]) + 256;                                                //obtain the sum of all 9 elements of the 3*3 matrix
            }
            else{
                sum += int(values[i][j]);
            }
        }
    }
    mean = char(sum / 9);                                                                     //then return the mean value of the 3*3 matrix
    return mean;
}
unsigned char medianFilter(const unsigned char values[KERNEL_SIZE][KERNEL_SIZE]){
    unsigned char median;
    int valuesNew[KERNEL_SIZE * KERNEL_SIZE];
    int k = 0;
    for (int i = 0; i < KERNEL_SIZE; ++i) {                                                    //convert the 3*3 matrix into a new char with length 9
        for (int j = 0; j < KERNEL_SIZE; ++j) {
            if (int(values[i][j]) < 0){
                valuesNew[k] = int(values[i][j]) + 256;
            }
            else{
                valuesNew[k] = int(values[i][j]);
            }
            k++;
        }
    }
    int m, temp;
    for (int i = 0; i < k - 1; ++i) {                                                          //using selection sort to sort all 9 elements from smallest to biggest
        m = i;
        for (int j = i + 1; j < k; ++j) {
            if (valuesNew[j] < valuesNew[m]){
                m = j;
            }
        }
        if (m != i){
            temp = valuesNew[i];
            valuesNew[i] = valuesNew[m];
            valuesNew[m] = temp;
        }
    }
    median = char(valuesNew[4]);                                                               //then return the median value of the 3*3 matrix
    return median;
}

bool flagHelp = false;                                                                         //some flags to check input, output and incorrect arguments
bool flagInput = false;
bool flagOutput = false;
bool flagNotPPM = false;
bool flagInvalidTrans = false;

int getCommand(const string& command, int argc, char *argv[]){                                 //find the place of one specific command in the argument
    string argument;
    for (int i = 0; i < argc; ++i) {
        argument = argv[i];
        if (argument == command){
            return i;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    string argument;
    int inputCom, outputCom, transCom;
    ifstream inputFile;
    ofstream outputFile;
    for (int i = 0; i < argc; ++i) {
        argument = argv[i];
        if (argument == "--help" || argument == "-h"){                                         //check whether the help command is called
            flagHelp = true;
        }
        if (argument == "-i"){
            flagInput = true;
        }
        if (argument == "-o"){
            flagOutput = true;
        }
    }
    if (flagHelp){                                                                            //print the help message and stop the program
        cout << HELP_MESSAGE << endl;
        return 0;
    }
    if (flagOutput){
        outputCom = getCommand("-o", argc, argv);                                   //get the place of the -o command
    }
    if (flagInput){
        inputCom = getCommand("-i", argc, argv);                                    //get the place of the -i command
        inputFile.open(argv[inputCom + 1]);                                               //then get the name of the input file
        if (!inputFile){
            cout << FILE_NOT_EXIST_ERROR << endl;                                            //print the error message and stop the program
            return 0;
        }
        else{
            string inFileName = argv[inputCom + 1];
            string ppmFormat = "ppm";
            if (ppmFormat != inFileName.substr(inFileName.find_last_of('.')+ 1)){    //get the extension of the input file and compare it with "ppm"
                flagNotPPM = true;
            }
            if (flagNotPPM){
                cout << FILE_TYPE_ERROR << endl;                                             //print the error message and stop the program
                return 0;
            }
        }
    }
    transCom = getCommand("-t", argc, argv);                                       //get the place of the -t command
    string transArg;
    transArg = argv[transCom + 1];                                                          //then get the specific transformation command
    if (transArg != "verticalFlip" && transArg != "rotate90" && transArg != "intensityInversion" \
    && transArg != "maxFilter" && transArg != "meanFilter" && transArg != "medianFilter"){  //check whether the transformation command is valid
        flagInvalidTrans = true;
    }
    if (flagInvalidTrans){
        cout << TRANSFORM_ERROR << endl;                                                    //print the error message and stop the program
        return 0;
    }
    Image imI, imJ;                                                                         //after checking incorrect arguments, start to do image calculation
    if (transArg == "verticalFlip"){
        if (flagInput){
            readImage(inputFile,imI);                                                //input from a file
        }
        else{
            readImage(cin, imI);                                                     //standard input
        }
        verticalFlip(imI,imJ);
        if (flagOutput){
            outputFile.open(argv[outputCom + 1]);
            writeImage(outputFile, imJ);                                            //output through a file
        }
        else{
            writeImage(cout, imJ);                                                  //standard output
        }
    }
    if (transArg == "rotate90"){
        if (flagInput){
            readImage(inputFile,imI);
        }
        else{
            readImage(cin, imI);
        }
        rotate90(imI,imJ);
        if (flagOutput){
            outputFile.open(argv[outputCom + 1]);
            writeImage(outputFile, imJ);
        }
        else{
            writeImage(cout, imJ);
        }
    }
    if (transArg == "intensityInversion"){
        if (flagInput){
            readImage(inputFile,imI);
        }
        else{
            readImage(cin, imI);
        }
        intensityInversion(imI,imJ);
        if (flagOutput){
            outputFile.open(argv[outputCom + 1]);
            writeImage(outputFile, imJ);
        }
        else{
            writeImage(cout, imJ);
        }
    }
    if (transArg == "maxFilter"){
        if (flagInput){
            readImage(inputFile,imI);
        }
        else{
            readImage(cin, imI);
        }
        filter(imI,imJ,maxFilter);
        if (flagOutput){
            outputFile.open(argv[outputCom + 1]);
            writeImage(outputFile, imJ);
        }
        else{
            writeImage(cout, imJ);
        }
    }
    if (transArg == "meanFilter"){
        if (flagInput){
            readImage(inputFile,imI);
        }
        else{
            readImage(cin, imI);
        }
        filter(imI,imJ,meanFilter);
        if (flagOutput){
            outputFile.open(argv[outputCom + 1]);
            writeImage(outputFile, imJ);
        }
        else{
            writeImage(cout, imJ);
        }
    }
    if (transArg == "medianFilter"){
        if (flagInput){
            readImage(inputFile,imI);
        }
        else{
            readImage(cin, imI);
        }
        filter(imI,imJ,medianFilter);
        if (flagOutput){
            outputFile.open(argv[outputCom + 1]);
            writeImage(outputFile, imJ);
        }
        else{
            writeImage(cout, imJ);
        }
    }
    inputFile.close();
    outputFile.close();
    return 0;
}
