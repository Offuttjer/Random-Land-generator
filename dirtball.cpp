/* Jeremy Offutt 6/20/26
CSE 240 Dirt Ball assignment
Usage: input size for a 2D array, the width and height of the landmass you are terraforming. You will be prompted for the radius of each dirt ball you are throuwing and the strength of each dirt ball, and the number of dirt balls to throw.*/
#include "dirtball.h"

using std::cout;
using std::cin;
using std::rand;
using std::cerr;
using std::endl;
using std::ofstream;



int main(int argc, char** argv)
{
    
    int width =0, height =0, waterLine=0, radius=0, str=0, numDrop=0;
    int** ilandMass;
    char** clandMass;
    bool fail, tofile= true;
    //If SEED exists use that otherwise use time
    char* seed = getenv("SEED");
    const char * rawLand = "land_raw.txt";
    const char * NormalLand = "land_normalized.txt";
    const char * polishedLand = "land_polished.txt";
    const char * BMP = "land_map.bmp";
    if(seed != nullptr)
    {  
    std::srand(atoi(seed));
    }
    else
    {
        std::srand(time(0));
    }
    if(argc > 1 && 0 == strcmp(argv[1],"-c"))
    {
        tofile = false;
    }
    
    //user interface
            //Width
            cout<<"Welcome to the Terraforming program: Dirt Ball\n";
            do 
            {
                fail=false;
                cout<<"Enter the Width of the landmass: ";
                cin>>width; //height and width are read in reverse because the rest of the code base uses array [width][height] throughout.
                if(cin.fail() || width < 1)
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout<<"Enter a number larger than 0\n";
                    fail = true;
                }

            }while(fail);
            //Height 
            do 
            {
                fail=false;
                cout<<"Enter the Height of the landmass: ";
                cin>>height;
                if(cin.fail() || height < 1)
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout<<"Enter a number larger than 0\n";
                    fail = true;
                }

            }while(fail);
            //Water line
            do 
            {
                fail=false;
                cout<<"Enter the water line value (min 40, max 200): ";
                cin>>waterLine;
                if(cin.fail() || waterLine < 40 || waterLine > 200)
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout<<"Enter a number between 40 and 200\n";
                    fail = true;
                }

            }while(fail);
            //radius of each dirtball
            do 
            {
                fail=false;
                cout<<"Enter the radius of each dirt ball: ";
                cin>>radius;
                if(cin.fail() || radius < 1 || radius > width || radius > height)
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout<<"Enter a radius bigger than 1 and less than "<< width<<" and "<<height<< '\n';
                    fail = true;
                }

            }while(fail);
            //Strength of each dirtball (min: radius)
            do 
            {
                fail=false;
                cout<<"Enter the strength of each dirt ball: ";
                cin>>str;
                if(cin.fail() || str < radius)
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout<<"Enter a number larger than "<<radius<<'\n';
                    fail = true;
                }

            }while(fail);
            //Number of dirtballs to Drop 
            do 
            {
                fail=false;
                cout<<"Enter the number of dirt balls to drop: ";
                cin>>numDrop;
                if(cin.fail() || numDrop < 1)
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout<<"Enter a number larger than 0\n";
                    fail = true;
                }

            }while(fail);

    //make the landmass array for int 
    ilandMass = new int* [height];
    for(int i =0; i< height; i++)
    {
        ilandMass[i] = new int [width]();
    }

    //call dirt drop
   
    for(int i =0; i < numDrop; i++)
    {
    dropDirtball(ilandMass, width, height, radius, str);
    }
    printToFile(ilandMass, width, height, rawLand , tofile);

    normalize255(ilandMass, width, height);
    printToFile(ilandMass, width, height, NormalLand , tofile);
    clandMass = polish(ilandMass, width, height, waterLine);
    printToFile(clandMass, width, height, polishedLand , tofile);
    makeBMP(clandMass, width, height, BMP);

    //clean and delete
    for(int i =0; i< height; i++)
    {
        delete[] ilandMass[i];
        delete[] clandMass[i];
    }
    delete[] ilandMass;
    delete[] clandMass;
    return 0;
}
/* END MAIN ************************************
**************************************
**************************************
**************************************
**************************************
**************************************/




//main terraforming function
void dropDirtball(int** grid, int width, int height, int radius, int strength)
{
    int hTarget, wTarget, lowerWBound, lowerHBound, upperWBound, upperHBound, impact;

    
    wTarget = frand() * width;
    hTarget = frand() * height;
    

    //set lower bounds for dirtball minimum: at edge of array
    if(wTarget - radius-1 < 0)
    {
        lowerWBound =0;
    }else
    {
        lowerWBound = wTarget - radius-1;
    }
    if(hTarget - radius-1 < 0)
    {
        lowerHBound =0;
    }else
    {
        lowerHBound = hTarget - radius-1;
    }
    //set upper bounds for dirtball max: at edge of array
    if(wTarget + radius + 1 > width)
    {
        upperWBound = width;
    }else
    {
        upperWBound = wTarget + radius+1;
    }
    if(hTarget + radius+1 > height)
    {
        upperHBound = height;
    }else
    {
        upperHBound = hTarget + radius+1;
    }


    //loop through [lowerbounds][upperbounds]
    for(int i = lowerHBound; i < upperHBound; i++)
    {
        for(int j = lowerWBound; j < upperWBound; j++)
        {
            if( inCircle(i,j,hTarget,wTarget,radius))
            {
                impact = strength - floor(distance(i,j,hTarget,wTarget));
                grid[i][j] += impact;
            }
        }
    }
}

//returns a random number [0,1)
double frand() {
    return (double) rand() / ((double) RAND_MAX + 1);
}

//calculates the distance between 2 points
double distance(int xpoint, int ypoint, int xcenter, int ycenter)
{
    double distance =0;
    distance = sqrt( pow(xpoint - xcenter, 2) + pow(ypoint - ycenter, 2));
    return distance;
}

//determines if a point is in a circle given its center and radius
bool inCircle(int xpoint, int ypoint, int xcenter, int ycenter, int radius)
{
    bool inCirc = false;
    double dist = distance(xpoint, ypoint, xcenter, ycenter);
    
    if(dist <= radius)
    {
        inCirc = true;
    }
 return inCirc;
}

//normalizes the values: divide by highest value then multiply by 255
void normalize255(int** grid, int width, int height)
{
    double great=0;
    //find the greatest value
    for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                if(grid[i][j]> great)
                {
                    great = grid[i][j];
                }
            }
    }
    //normalize to max: 255
    for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                
                    grid[i][j] = grid[i][j] / great * 255;
                
            }
    }
}

//converts normalized to 255 array to characters
char** polish(int** grid, int width, int height, int waterLine)
{
    char** cGrid = new char* [height];
    for(int i =0; i< height; i++)
    {
        cGrid[i] = new char [width]();
    }
    for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                
                if(grid[i][j] <(int)(waterLine * 0.5))
                {
                    cGrid[i][j] = '#';
                }else if(grid[i][j] <= waterLine )
                {
                    cGrid[i][j] = '~';
                }else if(grid[i][j] < (int)((255-waterLine) * .15+ waterLine))
                {
                     cGrid[i][j] = '.';
                }else if(grid[i][j] < (int)((255-waterLine) * .4 + waterLine))
                {
                    cGrid[i][j] = '-';
                }else if(grid[i][j] < (int)((255-waterLine) * .8+ waterLine))
                {
                     cGrid[i][j] = '*';
                }else
                {
                    cGrid[i][j] = '^';   
                }
                
            }
    }

    return cGrid;
}
//prints to file unless flag is passed for console
int printToFile(int** grid, int width, int height, const char* fileName, bool tofile = false)
{
    if(!tofile)
    {
        for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                cout<< std::setw(4)<<grid[i][j]<<" ";
            }
            cout<<endl;
        }
        return 2;
    }
    std::ofstream outfile;
    outfile.open(fileName);
    if(!outfile.is_open())
    {
        cerr<<"File didn't open in printToFile"<<endl;
        return -1;
    }else
    {
        for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                outfile<< std::setw(4)<<grid[i][j];
            }
            outfile<<endl;
        }
    }
    return 0;
}

//prints to file unless flag is passed for console
int printToFile(char** grid, int width, int height, const char* fileName, bool tofile = false)
{
    if(!tofile)
    {
        for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                cout<< std::setw(4)<<grid[i][j]<<" ";
            }
            cout<<endl;
        }
        return 2;
    }
    std::ofstream outfile;
    outfile.open(fileName);
    if(!outfile.is_open())
    {
        cerr<<"File didn't open in printToFile"<<endl;
        return -1;
    }else
    {
        for(int i = 0; i<height; i++){
            for(int j=0; j<width; j++){
                outfile<<grid[i][j];
            }
            outfile<<endl;
        }
    }
    return 0;
}





//BMP attempt
void makeBMP(char** grid, int width, int height, const char * fileName)
{
    ofstream outfile;
    outfile.open(fileName);
    pixel* bmpLine = new pixel [width];
    pixel pix;
    int padding = (4 -((width*3) % 4))%4;
    int paddedRowSize = width *3 + padding;
    uint8_t padByte =0;

    //total pixel data: padded row * height
    uint32_t totPixel = paddedRowSize * height;
    BmpHeader bmpHeader;
    BMPInfo bmpInfoHeader;
    bmpHeader.sizeOfBitmapFile = 54 + totPixel;
    bmpInfoHeader.width = width;
    bmpInfoHeader.height = height;
    bmpInfoHeader.rawBitmapDataSize = totPixel;

    //write the BmpHeader
    outfile.write((char *)(&bmpHeader), 14);
    outfile.write((char *)(&bmpInfoHeader), 40);

    for(int i = height-1; i>=0; i--){
            for(int j=0; j<width; j++){
                switch(grid[i][j])
                {
                    case '#':
                        pix.blue = 185;
                        pix.green = 128;
                        pix. red = 41; //dark blue {185,128,41}
                        break;
                    case '~':
                        pix.blue = 238;
                        pix.green = 172;
                        pix. red = 85;   //light blue {238, 172, 85} BGR
                        break;
                    case '.':
                        pix.blue = 152;
                        pix.green = 214;
                        pix. red = 244; //sand color {152,214,244}
                        break;
                    case '-':
                        pix.blue = 113;
                        pix.green = 204;
                        pix. red = 46;// light green {113,204,46}
                        break;
                    case '*':
                        pix.blue = 96;
                        pix.green = 174;
                        pix. red = 39;  // forest green {96,174,39}
                        break;
                    case '^':
                        pix.blue = 141;
                        pix.green = 140;
                        pix. red = 127;        //dark grey {141,140,127}
                        break;
                    default:
                    cout<<"Error. polished grid has a bad char\n";
                }
                bmpLine[j] = pix;
            }
            outfile.write ((char*)bmpLine, width * 3);
            for(int i =0; i < padding; i++)
            {
                outfile.write ((char *) (&padByte), 1);
            }
    }
    delete[] bmpLine;
}
