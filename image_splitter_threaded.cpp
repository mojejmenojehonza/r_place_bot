#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <omp.h> // Include OpenMP library

using namespace cv;
using namespace std;

vector<vector<Point>> divide_colored_pixels(const vector<Point>& colored_pixels, int n) {
    // Function to divide colored pixels into N parts
    vector<vector<Point>> divided_pixels;
    int chunk_size = colored_pixels.size() / n;
    for (size_t i = 0; i < colored_pixels.size(); i += chunk_size) {
        divided_pixels.push_back(vector<Point>(colored_pixels.begin() + i, colored_pixels.begin() + i + chunk_size));
    }
    return divided_pixels;
}

Mat create_colored_image(const Mat& original_image, const vector<Point>& colored_pixels) {
    // Create an image with only the specified colored pixels
    Mat colored_image = Mat::zeros(original_image.size(), CV_8UC4);
    for (const Point& p : colored_pixels) {
        colored_image.at<Vec4b>(p) = original_image.at<Vec4b>(p);
    }
    return colored_image;
}

vector<Mat> create_n_images(const string& original_image_path, int n) {
    // Load the original image
    Mat image = imread(original_image_path, IMREAD_UNCHANGED);

    // Find the colored and transparent pixels
    vector<Point> colored_pixels;
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            if (image.at<Vec4b>(y, x)[3] != 0) {
                colored_pixels.push_back(Point(x, y));
            }
        }
    }

    // Divide the colored pixels into N parts
    vector<vector<Point>> divided_colored_pixels = divide_colored_pixels(colored_pixels, n);

    // Create N different images using parallelism
    vector<Mat> images(n);
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        images[i] = create_colored_image(image, divided_colored_pixels[i]);
    }

    return images;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <original_image_path> <N>" << endl;
        return -1;
    }

    string original_image_path = argv[1];
    int N = atoi(argv[2]);
    vector<Mat> images = create_n_images(original_image_path, N);

    for (size_t i = 0; i < images.size(); i++) {
        imwrite("maps/test/" + to_string(i) + ".png", images[i]);
    }

    return 0;
}
