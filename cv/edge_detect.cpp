////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file, inverts it, and displays the result. 
//
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

/*void trackbarHandler(int pos)
{
    printf("Trackbar position: %d\n",pos);
}

int main(int argc, char *argv[])
{
    IplImage* img = 0; 
    int height,width,step,channels;
    uchar *data;
    int i,j,k;

    if(argc<2){
	printf("Usage: main <image-file-name>\n\7");
	exit(0);
    }

    // load an image  
    img=cvLoadImage(argv[1]);
    if(!img){
	printf("Could not load image file: %s\n",argv[1]);
	exit(0);
    }

    // get the image data
    height    = img->height;
    width     = img->width;
    step      = img->widthStep;
    channels  = img->nChannels;
    data      = (uchar *)img->imageData;
    printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

    // create a window
    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("mainWin", 100, 100);

    int trackbarVal=25;
    int maxVal=100;
    cvCreateTrackbar("bar1", "win1", &trackbarVal ,maxVal , trackbarHandler);
    int pos = cvGetTrackbarPos("bar1","win1");
    cvSetTrackbarPos("bar1", "win1", 25);


contentsWindow = new QDockWidget(tr("Table of Contents"), this);
     contentsWindow->setAllowedAreas(Qt::LeftDockWidgetArea
                                   | Qt::RightDockWidgetArea);
     addDockWidget(Qt::LeftDockWidgetArea, contentsWindow);

     headingList = new QListWidget(contentsWindow);
     contentsWindow->setWidget(headingList);
    // invert the image
    //for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
//						   data[i*step+j*channels+k]=255-data[i*step+j*channels+k];

    // show the image
    cvShowImage("mainWin", img );


    // wait for a key
    cvWaitKey(0);

    // release the image
    cvReleaseImage(&img );

    return 0;
}*/

int high_switch_value = 0;
int highInt = 0;
int low_switch_value = 0;
int lowInt = 0;

void switch_callback_h( int position ){
	highInt = position;
}
void switch_callback_l( int position ){
	lowInt = position;
}

int main(int argc, char * argv[])
{
	const char* name = "Edge Detection Window";

	// Kernel size
	int N = 7;

	// Set up images
	IplImage* img = cvLoadImage( "yufu.jpg", 0 );
	IplImage* img_b = cvCreateImage( cvSize(img->width+N-1,img->height+N-1), img->depth, img->nChannels );
	IplImage* out = cvCreateImage( cvGetSize(img_b), IPL_DEPTH_8U, img_b->nChannels );

contentsWindow = new QDockWidget(tr("Table of Contents"), this);
     contentsWindow->setAllowedAreas(Qt::LeftDockWidgetArea
                                   | Qt::RightDockWidgetArea);
     addDockWidget(Qt::LeftDockWidgetArea, contentsWindow);

     headingList = new QListWidget(contentsWindow);
     contentsWindow->setWidget(headingList);
	// Add convolution boarders
	CvPoint offset = cvPoint((N-1)/2,(N-1)/2);
	cvCopyMakeBorder(img, img_b, offset, IPL_BORDER_REPLICATE, cvScalarAll(0));

	// Make window
	cvNamedWindow( name, 1 );
	
	// Edge Detection Variables
	int aperature_size = N;
	double lowThresh = 20;
	double highThresh = 40;

	// Create trackbars
	cvCreateTrackbar( "High", name, &high_switch_value, 4, switch_callback_h );
	cvCreateTrackbar( "Low", name, &low_switch_value, 4, switch_callback_l );

	while( 1 ) {
		switch( highInt ){
			case 0:
				highThresh = 200;
				break;
			case 1:
				highThresh = 400;
				break;
			case 2:
				highThresh = 600;
				break;
			case 3:
				highThresh = 800;
				break;
			case 4:
				highThresh = 1000;
				break;
		}
		switch( lowInt ){
			case 0:
				lowThresh = 0;
				break;
			case 1:
				lowThresh = 100;
				break;
			case 2:
				lowThresh = 200;
				break;
			case 3:
				lowThresh = 400;
				break;
			case 4:
				lowThresh = 600;
				break;
		}

		// Edge Detection
		cvCanny( img_b, out, lowThresh*N*N, highThresh*N*N, aperature_size );		
		cvShowImage(name, out);
		
		if( cvWaitKey( 15 ) == 27 ) 
			break;
	}

	// Release
	cvReleaseImage( &img );
	cvReleaseImage( &img_b );
	cvReleaseImage( &out );
	cvDestroyWindow( name );

	return 0;
}

