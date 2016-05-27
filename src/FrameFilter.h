/***********************************************************************
 FrameFilter - Class to filter streams of depth frames arriving from a
 depth camera, with code to detect unstable values in each pixel, and
 fill holes resulting from invalid samples.
 Forked from Oliver Kreylos' Augmented Reality Sandbox (SARndbox).
 ***********************************************************************/

#pragma once
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "Utils.h"
//#include <vector>

using namespace ofxCv;
using namespace cv;

/* Forward declarations: */
namespace Misc {
    template <class ParameterParam>
    class FunctionCall;
}
//namespace Geometry {
//    template <class ScalarParam,int dimensionParam>
//    class Plane;
//    template <class ScalarParam,int dimensionParam>
//    class ProjectiveTransformation;
//}

class FrameFilter /*: public ofThread */{
public:
	typedef unsigned short RawDepth; // Data type for raw depth values
	typedef float FilteredDepth; // Data type for filtered depth values

    FrameFilter();
    ~FrameFilter();
    
    bool setup(const unsigned int swidth,const unsigned int sheight,int sgradFieldresolution, float newMaxOffset);
    void initiateBuffers(void); // Reinitialise buffers
    void resetBuffers(void);
//    void setDepthRange(float nearclip, float farclip);
    bool isFrameNew();
    ofVec2f getGradFieldXY(int x, int y); // gradient field at pos x, y
    ofVec2f* getGradField(); // gradient field
    Point3f* getWrldcoordbuffer();
	void setValidElevationInterval(const ofVec3f sbasePlaneNormal, double newMinElevation,double newMaxElevation); // Sets the interval of elevations relative to the given base plane considered by the depth image filter
	void setStableParameters(unsigned int newMinNumSamples,unsigned int newMaxVariance); // Sets the statistical properties to consider a pixel stable
	void setHysteresis(float newHysteresis); // Sets the stable value hysteresis envelope
	void setRetainValids(bool newRetainValids); // Sets whether the filter retains previous stable values for instable pixels
	void setInstableValue(float newInstableValue); // Sets the depth value to assign to instable pixels
	void setSpatialFilter(bool newSpatialFilter); // Sets the spatial filtering flag
	void setROI(ofRectangle ROI); // Sets the ROI
    void setMaxOffset(float newMaxOffset);
    bool isInsideROI(int x, int y); // test is x, y is inside ROI
    void displayFlowField();
    void drawArrow(ofVec2f);
    void applySpaceFilter(ofFloatPixels& newOutputFrame);
    void updateGradientField();
    ofFloatPixels filter(ofShortPixels inputframe);
	float* statBuffer; // Buffer retaining the running means and variances of each pixel's depth value
    bool firstImageReady;
    
private:
    ofxKinect * backend;

    ofShortPixels inputframe;
    ofFloatPixels outputframe;
    ofTexture texture;
    bool newFrame;
    bool bufferInitiated;
    
    ofVec2f* gradField;
    int gradFieldcols, gradFieldrows;
    int gradFieldresolution;           //Resolution of grid relative to window width and height in pixels
    float maxgradfield, depthrange;
    
    Point3f* wrldcoordbuffer; // Buffer retaining world coordinates of points
    float nearclip, farclip; // nearclip and farclip of kinect
    
    unsigned int width, height; // Width and height of processed frames
    unsigned int inputFrameVersion; // Version number of input frame
	volatile bool runFilterThread; // Flag to keep the background filtering thread running
//	float min; // lower bound of valid depth values in depth image space
//	float max; // upper bound of valid depth values in depth image space
    
    int minX, maxX, ROIwidth; // ROI definition
    int minY, maxY, ROIheight;
    
//	ofVec4f basePlaneNormal; // Base plane normal
//	float minPlane, maxPlane; //  lower bound & upper bound of valid depth values in depth image space
    
	int numAveragingSlots; // Number of slots in each pixel's averaging buffer
	float* averagingBuffer; // Buffer to calculate running averages of each pixel's depth value
	int averagingSlotIndex; // Index of averaging slot in which to store the next frame's depth values
	unsigned int minNumSamples; // Minimum number of valid samples needed to consider a pixel stable
	float maxVariance; // Maximum variance to consider a pixel stable
//    float depthNorm; // Normalization of depth
    float unvalidValue;
	float hysteresis; // Amount by which a new filtered value has to differ from the current value to update
	bool retainValids; // Flag whether to retain previous stable values if a new pixel in instable, or reset to a default value
	float instableValue; // Value to assign to instable pixels if retainValids is false
	bool spatialFilter; // Flag whether to apply a spatial filter to time-averaged depth values
	float* validBuffer; // Buffer holding the most recent stable depth value for each pixel
//	void* filterThreadMethod(void); // Method for the background filtering thread
    float maxOffset;
	
};
