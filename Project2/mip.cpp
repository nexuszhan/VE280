#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "image.h"

using namespace std;

const int s = 1;
const int RANGE = 2*s+1;
typedef char (*Agg)(const char values[RANGE][RANGE]);

void readImage(istream &is, Image &imI);
void writeImage(std::ostream &os, const Image &imI);
void verticalFlip(const Image &imI, Image &imJ);
void rotate90(const Image &imI, Image &imJ);
void intensityInversion(const Image &imI, Image &imJ);
void filter(const Image &imI, Image &imJ, Agg f);
char Max(const char values[RANGE][RANGE]);
char Mean(const char values[RANGE][RANGE]);
char Median(const char values[RANGE][RANGE]);

// helper functions
int checkTransformation(string cmd);
void trans(int cmd,const Image & imI,Image & imJ);

int main(int argc,char* argv[])
{
    if (argc==1)
        return -1; // Not supposed to happen
    
    bool stop = false; // Indicate whether there is help option or incorrect argument
    string transformation; // Store transformation
    bool standardIn = true,standardOut = true; // Indicate what streams to use
    ifstream inFile; ofstream outFile;
    
    int i=1;
    while (i<argc)
    {
        string option(argv[i]);
        if (option == "--help" || option == "-h")
        {
            cout << HELP_MESSAGE << endl;
            stop = true;
            break;
        }
        else if (option == "-t")
        {
            string cmd(argv[++i]);
            if (checkTransformation(cmd)<0)
            {
                cout << TRANSFORM_ERROR << endl;
                stop = true;
                break;
            }
            transformation = cmd;
        }
        else if (option == "-i")
        {
            string input(argv[++i]);

            // Check file existence
            inFile.open(input);
            if (!inFile)
            {
                cout << FILE_NOT_EXIST_ERROR << endl;
                stop = true;
                inFile.close();
                break;
            }
            
            // Check file type
            string suffix(".ppm");
            if (input.compare(input.size()-suffix.size(),suffix.size(),suffix) != 0)
            {
                cout << FILE_TYPE_ERROR << endl;
                stop = true;
                inFile.close();
                break;
            }
            standardIn = false;
        }
        else if (option == "-o")
        {
            string output(argv[++i]);
            outFile.open(output,ios_base::out | ios_base::binary);
            standardOut = false;
        }
        else
        {
            exit(-1); // Not supposed to happen
        }

        i++;
    }

    if (!stop)
    {
        Image I;
        if (standardIn)
            readImage(cin,I);
        else
            readImage(inFile,I);

        Image J;
        int cmd = checkTransformation(transformation);
        trans(cmd,I,J);

        if (standardOut)  
            writeImage(cout,J);
        else
            writeImage(outFile,J);
    }

    if (!standardIn)
        inFile.close();
    if (!standardOut)
        outFile.close();

    return 0;
}

void readImage(istream &is, Image &imI)
{
    // Read the header
    string str1;
    unsigned int w,h,m;
    is >> str1 >> w >> h >> m;
    imI.h = h; imI.w = w;
    is.get(); // Eliminate the '\n' to make is.read() correct

    // Read the content 3 bytes per time
    for (unsigned int i=0; i<h; i++)
        for (unsigned int j=0; j<w; j++)
            is.read((char*)&imI.image[i][j],sizeof(Rgb));
}

void writeImage(ostream &os, const Image &imI)
{
    // Write the header
    os << "P6" << endl << imI.w << ' ' << imI.h << endl << "255" << endl;

    // Write the content
    for (unsigned int i=0; i<imI.h; i++)
        for (unsigned int j=0; j<imI.w; j++)
            os.write((char*)&imI.image[i][j],sizeof(Rgb));
}

void verticalFlip(const Image &imI, Image &imJ)
{
    imJ.w = imI.w; imJ.h = imI.h;
    for (unsigned int i=0; i<imJ.h; i++)
        for (unsigned int j=0; j<imJ.w; j++)
        {
            imJ.image[i][j].red = imI.image[imI.h-i-1][j].red;
            imJ.image[i][j].green = imI.image[imI.h-i-1][j].green;
            imJ.image[i][j].blue = imI.image[imI.h-i-1][j].blue;
        }
}

void rotate90(const Image &imI, Image &imJ)
{
    imJ.w = imI.h; imJ.h = imI.w;
    for (unsigned int i=0; i<imJ.h; i++)
        for (unsigned int j=0; j<imJ.w; j++)
        {
            imJ.image[i][j].red = imI.image[imI.h-j-1][i].red;
            imJ.image[i][j].green = imI.image[imI.h-j-1][i].green;
            imJ.image[i][j].blue = imI.image[imI.h-j-1][i].blue;
        }
}

void intensityInversion(const Image &imI, Image &imJ)
{
    imJ.w = imI.w; imJ.h = imI.h;
    for (unsigned int i=0; i<imJ.h; i++)
        for (unsigned int j=0; j<imJ.w; j++)
        {
            imJ.image[i][j].red = M - imI.image[i][j].red;
            imJ.image[i][j].green = M - imI.image[i][j].green;
            imJ.image[i][j].blue = M - imI.image[i][j].blue;
        }
}

char Max(const char values[RANGE][RANGE])
{
    // (x<0)*256+x is for converting x from [-128,127] to [0,255]
    int result = 0;
    for (int i=0; i<RANGE; i++)
        for (int j=0; j<RANGE; j++)
            result = max((values[i][j]<0)*256+values[i][j],result);
    return (char)result;
}

char Mean(const char values[RANGE][RANGE])
{
    int result=0;
    for (int i=0; i<RANGE; i++)
        for (int j=0; j<RANGE; j++)
            result += ((values[i][j]<0)*256+values[i][j]);

    return (char)(result/(RANGE*RANGE));
}

char Median(const char values[RANGE][RANGE])
{
    int arr[RANGE*RANGE];
    for (int i=0; i<RANGE; i++)
        for (int j=0; j<RANGE; j++)
            arr[3*i+j] = ((values[i][j]<0)*256+values[i][j]);
    sort(arr,arr+RANGE*RANGE);
    return (char)arr[4];
}

void filter(const Image &imI, Image &imJ, Agg f)
{
    imJ.w = imI.w; imJ.h = imI.h;
    int width = imI.w; int height = imI.h;
    for (int i=0; i<height; i++)
        for (int j=0; j<width; j++)
        {
            // Extract the RGB components
            char reds[RANGE][RANGE]; char greens[RANGE][RANGE]; char blues[RANGE][RANGE];
            for (int k=0; k<RANGE; k++)
            {
                int row = i+k-1;
                for (int l=0; l<RANGE; l++)
                {
                    int col = j+l-1;
                    if (row<0 || row>=height || col<0 || col>=width)
                    {
                        reds[k][l] = 0;
                        greens[k][l] = 0;
                        blues[k][l] = 0;
                    }
                    else
                    {
                        reds[k][l] = imI.image[row][col].red;
                        greens[k][l] = imI.image[row][col].green;
                        blues[k][l] = imI.image[row][col].blue;
                    }
                }
            }
            // Apply filter function component-wisely
            imJ.image[i][j].red = f(reds);
            imJ.image[i][j].green = f(greens);
            imJ.image[i][j].blue = f(blues);
        }
}

int checkTransformation(string cmd)
{
    // If trans is a valid transformation, return the index of it;
    // otherwise return -1 to indicate invalid transformation
    if (cmd=="verticalFlip")
        return 0;
    else if (cmd=="rotate90")
        return 1;
    else if (cmd=="intensityInversion")
        return 2;
    else if (cmd=="maxFilter")
        return 3;
    else if (cmd=="meanFilter")
        return 4;
    else if (cmd=="medianFilter")
        return 5;
    else
        return -1;
}

void trans(int cmd,const Image & imI,Image & imJ)
{
    // Transform the image using corresponding function
    if (cmd==0)
        verticalFlip(imI,imJ);
    else if (cmd==1)
        rotate90(imI,imJ);
    else if (cmd==2)
        intensityInversion(imI,imJ);
    else if (cmd==3)
        filter(imI,imJ,Max);
    else if (cmd==4)
        filter(imI,imJ,Mean);
    else if (cmd==5)
        filter(imI,imJ,Median);
}