#include "MatTransformer.h"


Mat MatTransformer::ToGray(const Mat input, bool blurBefore)
{
	Mat src = input.clone();

	if (blurBefore)
		blur(src, src, Size(3, 3));

	// Convert image to gray
	cvtColor(src, src, CV_BGR2GRAY);

	return src;
}

Mat MatTransformer::ToEdges(const Mat input)
{
	Mat src = input.clone();

	Mat otsuMat;
	double otsu_thresh_val = cv::threshold(
		src, otsuMat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU
	);

	double lower_thresh_val = otsu_thresh_val * 0.3;
	double high_thresh_val = otsu_thresh_val;

	Canny(src, src, lower_thresh_val, high_thresh_val, 3, false);
	return src;
}

Mat MatTransformer::FillGaps(const Mat input, int erodeTimes)
{
	Mat src = input.clone();

	Mat dilatated;

	dilate(src, dilatated, getStructuringElement(MORPH_RECT, Size(10, 2)));
	dilate(dilatated, dilatated, getStructuringElement(MORPH_RECT, Size(2, 10)));

	if (erodeTimes > 0)
		erode(dilatated, dilatated, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), erodeTimes);

	return dilatated;
}

Mat MatTransformer::ToHoughLinesP(const Mat input, int threshold, double rho, double theta, double minLineLength, double maxLineGap)
{
	Mat contourImage = Mat::zeros(input.size(), CV_8UC3);

	std::vector<Vec4i> lines;
	HoughLinesP(input, lines, rho, theta, threshold, minLineLength, maxLineGap);

	for (size_t i = 0; i < lines.size(); i++)
	{
		line(contourImage, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 4, 8);
	}

	return contourImage;
}