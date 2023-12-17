// Author: Sagnick Bhar
// Deque: Seam Carving-Apply seam carving content aware image-resizing algorithm on a given image. 
//Take the height and width (in pixels) of the output image as inputs from the user.

#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

float energyFormula(int i, int j,int height,int width, Mat &img){
    float ans;

    Vec3b pixelleft = img.at<Vec3b>(i, (j-1+width)%width);
    Vec3b pixelright= img.at<Vec3b>(i, ((j+1)%width));
    Vec3b pixelup = img.at<Vec3b>((i-1+height)%height, j);
    Vec3b pixeldown = img.at<Vec3b>((i+1)%height, j);

    int bx=abs(pixelleft[0]-pixelright[0]);  
    int gx=abs(pixelleft[1]-pixelright[1]); 
    int rx=abs(pixelleft[2]-pixelright[2]);   

    int delxsq=bx*bx + gx*gx + rx*rx;

    int by=abs(pixelup[0]-pixeldown[0]);    
    int gy=abs(pixelup[1]-pixeldown[1]);   
    int ry=abs(pixelup[2]-pixeldown[2]);    

    int delysq=by*by + gy*gy + ry*ry;

    ans=sqrt(delxsq+delysq);

    return ans;
}

float** createCostMatrix(int height,int width, Mat &img){
    float** costMatrix = new float*[height];
    for (int i = 0; i <height; ++i) {
        costMatrix[i] = new float[width];
        for (int j = 0; j < width; ++j) {
            costMatrix[i][j]=energyFormula(i,j, height, width, img);
        }
    }
    return costMatrix;
}

float** findMinHorizontalCostMatrix(int height, int width, float** &costMatrix){
    float** dp = new float*[height];
    for (int i = 0; i <height; ++i){
        dp[i] = new float[width];
        for(int j = 0; j < width; ++j){
            if(j==0){
                dp[i][j]=costMatrix[i][j];
            }
            else    dp[i][j]=0;
        }
    }  
    
    for(int j=1; j<width; j++){
        for(int i=0;i<height;i++){
            float leftcell = costMatrix[i][j] + dp[i][j-1];
            
            float lowerDiagonalcell= 1e9;
            if(i<=height-2)     lowerDiagonalcell =costMatrix[i][j]+ dp[i+1][j-1];
            
            float topDiagonalcell = 1e9;
            if(i>=1)    topDiagonalcell = costMatrix[i][j]+dp[i-1][j-1];
            
            dp[i][j] = min(leftcell, min(lowerDiagonalcell,topDiagonalcell));
        }
    }

    return dp;
}


float** findMinVerticalCostMatrix(int height, int width, float** &costMatrix){
    float** dp = new float*[height];
    for (int i = 0; i <height; i++){
        dp[i] = new float[width];
        for(int j = 0; j < width; j++){
            if(i==0){
                dp[i][j]=costMatrix[i][j];
            }
            else    dp[i][j]=0;
        }
    }

    for(int i=1; i<height; i++){
        for(int j=0;j<width;j++){
            float uppercell = costMatrix[i][j] + dp[i-1][j];
            
            float leftDiagonalcell= 1e9;
            if(j-1>=0) leftDiagonalcell =costMatrix[i][j]+ dp[i-1][j-1];
            
            float rightDiagonalcell = 1e9;
            if(j+1<width) rightDiagonalcell = costMatrix[i][j]+dp[i-1][j+1];
            
            dp[i][j] = min(uppercell, min(leftDiagonalcell,rightDiagonalcell));
        }
    }
    return dp;
}

void shiftRowElementsLeft(int x, int y, int width, Mat& img){
    for (int col = y; col < width- 1; ++col){
        img.at<Vec3b>(x, col) = img.at<Vec3b>(x, col+1);
    }
    img.at<Vec3b>(x, width- 1)[0] = 0;
    img.at<Vec3b>(x, width- 1)[1] = 0;
    img.at<Vec3b>(x, width- 1)[2] = 0;
}
void shiftRowElementsUp(int x, int y, int height, Mat& img){
    for (int row = x; row < height- 1; ++row){
        img.at<Vec3b>(row, y) = img.at<Vec3b>(row+1, y);
    }
    img.at<Vec3b>(height- 1, y)[0] = 0;
    img.at<Vec3b>(height- 1, y)[1] = 0;
    img.at<Vec3b>(height- 1, y)[2] = 0;
}

int findSmallestCostStartCol(float**& dp, int height, int width){
    float mi=1e8;
    int smallcol=-1;
    for(int j=0;j<width;j++){
        if(dp[height-1][j]<mi){
            mi=dp[height-1][j];
            smallcol=j;
        }
    }
    return smallcol;
}

int findSmallestCostStartRow(float**& dp, int height, int width){
    float mi=1e8;
    int smallestrow=-1;
    for(int i=0;i<height;i++){
        if(dp[i][width-1]<mi){
            mi=dp[i][width-1];
            smallestrow=i;
        }
    }
    return smallestrow;
}

void ApplyHorizontalSeamCarving(float**& dp, int height, int width, float** costMatrix, Mat& img){
    int smallrow=findSmallestCostStartRow(dp, height, width);
    // Shift pixels to up
    shiftRowElementsUp(smallrow, width-1, height, img);

    for(int j=width-1; j>=1; j--){
        float lowerDiagonalcell=1e8;
        if(smallrow+1<height) lowerDiagonalcell=costMatrix[smallrow+1][j-1];
        float leftcell=costMatrix[smallrow][j-1];
        float upDiagonalcell=1e8;
        if(smallrow>=1) upDiagonalcell=costMatrix[smallrow-1][j-1];

        if(lowerDiagonalcell<leftcell && lowerDiagonalcell<upDiagonalcell ){
            shiftRowElementsUp(smallrow+1,j-1, height, img);
            smallrow+=1;
        }
        else if(leftcell<lowerDiagonalcell && leftcell<upDiagonalcell ){
            shiftRowElementsUp(smallrow,j-1, height, img);
        }
        else if(upDiagonalcell<lowerDiagonalcell && upDiagonalcell<leftcell ){
            shiftRowElementsUp(smallrow-1,j-1, height, img);
            smallrow-=1;
        }
    }
}

void ApplyVerticalSeamCarving(float**& dp, int height, int width, float** costMatrix, Mat& img){

    int smallcol=findSmallestCostStartCol(dp, height, width);
    // Shift pixels to the left
    shiftRowElementsLeft(height-1, smallcol, width, img); 

    for(int i=height-1; i>=1; i--){
        float leftDiagonalcell=1e8;
        if(smallcol>=1) leftDiagonalcell=costMatrix[i-1][smallcol-1];
        float uppercell=costMatrix[i-1][smallcol];
        float rightDiagonalcell=1e8;
        if(smallcol+1<width) rightDiagonalcell=costMatrix[i-1][smallcol+1];

        if(leftDiagonalcell<uppercell && leftDiagonalcell<rightDiagonalcell ){
            shiftRowElementsLeft(i-1,smallcol-1, width, img);
            smallcol-=1;
        }
        else if(uppercell<leftDiagonalcell && uppercell<rightDiagonalcell ){
            shiftRowElementsLeft(i-1,smallcol, width, img);
        }
        else if(rightDiagonalcell<leftDiagonalcell && rightDiagonalcell<uppercell ){
            shiftRowElementsLeft(i-1,smallcol+1, width, img);
            smallcol+=1;
        }
    }
}

int main()
{
    // Reading input image path
    Mat img = imread("/home/sagnick/DSAPS_Assignments/2023201008_A1/sample_input/sample1.jpeg");
    namedWindow("First OpenCV Application", WINDOW_AUTOSIZE);
    Mat originalImage=img;
    int originalHeight = img.rows; 
	int originalWidth = img.cols;
    
    int height=originalHeight;
    int width=originalWidth;

    printf("Original height: %d\n",originalHeight);
    printf("Original width: %d\n",originalWidth);

    // Taking user input width and height
    int userHeight, userWidth;
    cout<<"Enter desired height: ";
    cin>>userHeight;
    cout<<"Enter desired width: ";
    cin>>userWidth;
    cout<<endl;
    
    if(userHeight>height){
        printf("Height given is too large!\n");
        exit(EXIT_FAILURE);
    }
    if(userWidth>width){
        printf("Width given is too large!\n");
        exit(EXIT_FAILURE);
    }

    // Width Wise Seam Carving
    while(width>userWidth){
        float** costMatrix=createCostMatrix(height, width, img);
        float** dp=findMinVerticalCostMatrix(height, width, costMatrix);
        
        ApplyVerticalSeamCarving(dp, height, width, costMatrix, img);

        width--;
    }
   
    // Height Wise Seam Carving
    while(height>userHeight){
        float** costMatrix=createCostMatrix(height, width, img);
        float** dp=findMinHorizontalCostMatrix(height, width, costMatrix);
        
        ApplyHorizontalSeamCarving(dp, height, width, costMatrix, img);


        height--;
    }
    // Create a new matrix with user desired dimensions
    Mat finalImage(userHeight, userWidth, CV_8UC3);

    // Copy pixels from the original image to the new image
    for (int row = 0; row < userHeight; ++row){
        for (int col = 0; col < userWidth; ++col){
            finalImage.at<Vec3b>(row, col) = img.at<Vec3b>(row, col);
        }
    }
    if (img.empty()) {
        cout << "Image not loaded properly" << endl;
    }

    imwrite("/home/sagnick/DSAPS_Assignments/2023201008_A1/out1.jpg",finalImage );
    imshow("First OpenCV Application", finalImage);
    moveWindow("First OpenCV Application", 0, 45);
    waitKey(0);
    destroyAllWindows();
    
    exit(EXIT_SUCCESS);
}