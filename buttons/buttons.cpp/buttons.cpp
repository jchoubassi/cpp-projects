#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class pixel_class {
private:
    int red, green, blue;
    bool exclude;  // do not check this pixel
public:
    void loaddata(int v1, int v2, int v3);
    void datatofile(fstream& ppmfile);
    int getR() { return red; }
    int getG() { return green; }
    int getB() { return blue; }
    void setexclude(bool ex) { exclude = ex; }
    bool getexclude() { return exclude; }
};

void loadButtons();
void checkpixel(int x, int y);
void DrawBox(int R, int G, int B);

int total, xmin, xmax, ymin, ymax; //global

int screenx, screeny, maxcolours;
pixel_class picture[600][600];

int main() {
    int x, y, grey;
    string outfilename;
    fstream outfile;
    bool excluded;

    // Step 1 : read in the image from Buttons.ppm
    loadButtons();

    // Step 2 : draw a box around each button
    for (y = 0; y < screeny; y++) {
        for (x = 0; x < screenx; x++) {
            grey = picture[x][y].getR();
            excluded = picture[x][y].getexclude();
            if ((grey > 128) && (excluded == false)) {
                total = 0;
                xmin = x;
                xmax = x;
                ymin = y;
                ymax = y;
                checkpixel(x, y);
                cout << "Total pixels = " << total << endl;
                if (total < 7700) {
                    DrawBox(255, 0, 0);  // draw a red box
                }
                else {
                    DrawBox(0, 255, 0);  // draw a green box
                }
            }
        }
    }

    // Step 3 : output the final .ppm file
    outfilename = "myImage.ppm";
    outfile.open(outfilename.c_str(), fstream::out);
    outfile << "P3\n";
    outfile << "# " << outfilename << endl;
    outfile << screenx << " " << screeny << endl;
    outfile << maxcolours << endl;
    for (y = 0; y < screeny; y++) {
        for (x = 0; x < screenx; x++) {
            picture[x][y].datatofile(outfile);
        }
        outfile << endl;
    }
    outfile.close();
}

void loadButtons() {
    // load the picture from Buttons.ppm
    int x, y, R, G, B;
    fstream infile;
    string infilename, line;
    infilename = "Buttons.ppm";
    infile.open(infilename.c_str(), fstream::in);
    if (infile.is_open() == false) {
        cout << "ERROR: not able to open " << infilename << endl;
        exit(2);
    };
    getline(infile, line);  
    getline(infile, line);  // this line is "# filename"
    infile >> screenx >> screeny;  // this line is the size
    infile >> maxcolours;  // this line is 256
    for (y = 0; y < screeny; y++) {
        for (x = 0; x < screenx; x++) {
            infile >> R >> G >> B;
            picture[x][y].loaddata(R, G, B);
            picture[x][y].setexclude(false);
        }
    }
    infile.close();
}

void checkpixel(int x, int y) {
    // checkpixel and its neighbour
    int grey;
    bool excluded;
    grey = picture[x][y].getR();
    if (grey <= 128) { return; }
    excluded = picture[x][y].getexclude();
    if (excluded == true) { return; }  // do not check
    total++;  
    if (x < xmin) { xmin = x; }
    if (x > xmax) { xmax = x; }
    if (y < ymin) { ymin = y; }
    if (y > ymax) { ymax = y; }
    picture[x][y].setexclude(true);  // do not check
    if (x > 0) { checkpixel(x - 1, y); }
    if (y > 0) { checkpixel(x, y - 1); }
    if (x < screenx - 1) { checkpixel(x + 1, y); }
    if (y < screeny - 1) { checkpixel(x, y + 1); }
}

void DrawBox(int R, int G, int B) {
    int x, y;
    for (x = xmin; x <= xmax; x++) {
        picture[x][ymin].loaddata(R, G, B);
        picture[x][ymin].setexclude(true);
        picture[x][ymax].loaddata(R, G, B);
        picture[x][ymax].setexclude(true);
    }
    for (y = ymin; y <= ymax; y++) {
        picture[xmin][y].loaddata(R, G, B);
        picture[xmin][y].setexclude(true);
        picture[xmax][y].loaddata(R, G, B);
        picture[xmax][y].setexclude(true);
    }
}

//methods for the pixel_class
void pixel_class::loaddata(int v1, int v2, int v3) {
    red = v1;
    green = v2;
    blue = v3;
}

void pixel_class::datatofile(fstream& ppmfile) {
    ppmfile << red << " " << green;
    ppmfile << " " << blue << "  ";
}

