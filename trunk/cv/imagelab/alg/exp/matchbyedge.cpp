#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "class.h"
#include "highgui.h"
#include "cv.h"
#include "util.h"
#include "alg.h"

using namespace std;
using namespace cv;

double chamferMatching(const string & filename,
		const Mat & tarImg, Point2i & pos);

struct MatchInfo
{
	string filename;
	double score;
	Point2i pos;

	MatchInfo(string filename, double score, Point2i pos)
	{
		this->filename = filename;
		this->score = score;
		this->pos = pos;
	}

	MatchInfo(const MatchInfo & m)
	{
		filename = m.filename;
		score = m.score;
		pos = m.pos;
	}

	bool operator < (const MatchInfo & m) const
	{
		return score < m.score;
	}
};

int main (int argc, char * argv[])
{

	if (argc != 2) {
		cout << "Usage: main <image_patch>\n";
		exit(0);
	}


	// Open target image
	Mat image = imread(argv[1], 0);
	Mat tarImg(image.size(), CV_8UC1);
	image.convertTo(tarImg, CV_8UC1);
	image.release();
	toWB8UC1(tarImg, tarImg);

	// Read all the source images
	string dir = "./edge_data/";
	vector<string> filenames;

	getFileNames(dir, filenames);
	sort(filenames.begin(), filenames.end());

	cout << "There are totally " << filenames.size() << " files\n\n";

	//for (int i = 0; i < filenames.size(); i++) {
	//	cout << filenames[i] << endl;
	//}

	//filenames.resize(5);
	vector<double> scores(filenames.size());
	vector<Point2i> poses(filenames.size());

	for (int i = 0; i < filenames.size(); i++) {
		cout << "Idx: " << i << " File name: " << filenames[i] << endl;
		scores[i] = chamferMatching(dir+filenames[i], tarImg, poses[i]);
		cout << "The score is " << scores[i] << endl;
		cout << "The position is " << poses[i].x << " " << poses[i].y << endl;
	}
	
	vector<double>::iterator minScoreIt = min_element(scores.begin(), scores.end());
	int minIdx = minScoreIt - scores.begin();
	cout << "Min score: " << *minScoreIt << endl;
	cout << "File name: " << filenames[minIdx] << endl;
	cout << "Position: " << poses[minIdx].x << " " << poses[minIdx].y << endl;
	tarImg.release();

	// Output the rank to the file 'rank'
	vector<MatchInfo> scoreboard;
	for (int i = 0; i < filenames.size(); i++) {
		scoreboard.push_back(MatchInfo(filenames[i], scores[i], poses[i]));
	}
	sort(scoreboard.begin(), scoreboard.end());

	ofstream fs("rank");
	//fs << setw(10) << "Hello\n";
	fs << "Rank " << setw(10) << "score" << setw(6) << "x" << setw(6) << "y"
		<< "\t" << "filename" << endl;
	for (int i = 0; i < scoreboard.size(); i++) {
		fs << setw(4) << i+1 << setw(10) << scoreboard[i].score
			<< setw(6) << scoreboard[i].pos.x
			<< setw(6) << scoreboard[i].pos.y
			<< "\t" << scoreboard[i].filename
			<< endl;
	}
	fs.close();

	cout << "\n\nMove the top images\n";
	int topN = 10;
	string topDir = string("./top_images/");
	for (int i = 0; i < topN && i < scoreboard.size(); i++) {
		string cmd = "cp " + dir + scoreboard[i].filename + " "
						+ topDir + scoreboard[i].filename;
		cout << "Command: " << cmd << endl;
		system(cmd.c_str());
	}

	return 0;
}

double chamferMatching(const string & filename,
		const Mat & tarImg, Point2i & pos)
{
	Mat image = imread(filename, 0);
	Mat srcImg(image.size(), CV_8UC1);
	image.convertTo(srcImg, CV_8UC1);

	toWB8UC1(srcImg, srcImg);

	// Distance transform of the source image
	Mat srcDist(srcImg.size(), CV_32FC1);
	distanceTransform(srcImg, srcDist, CV_DIST_L1, 3);
	
	double score = chamferMatching(srcDist, tarImg, pos);

	image.release();
	srcImg.release();
	srcDist.release();

	return score;
}
