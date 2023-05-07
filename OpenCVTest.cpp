// OpenCVTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <array>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <math.h>
using namespace cv;

void myMedianFilter(InputArray input, OutputArray& output, int windowSize) {
    const Mat& img = input.getMat();
    output.create(img.size(), img.type());
    Mat dst = output.getMat();
    std::vector<float> windowVector;
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            for (int t = -windowSize/2; t <= windowSize/2; t++) {
                for (int s = -windowSize/2; s <= windowSize/2; s++) {
                    windowVector.push_back(img.at<float>(min(img.rows - 1, max(0, y + t)), min(img.cols - 1, max(0, x + s))));
                }
            }
            std::sort(windowVector.begin(), windowVector.end());
            dst.at<float>(y, x) = windowVector.at((int)(windowSize * windowSize / 2));
            windowVector.clear();
        }
    }
}

//x축으로 cos을 더해주는 함수
void addCosX(InputArray inp, OutputArray out, int freq, float phase = 0, float amp = 0) {
    const Mat& src = inp.getMat();
    out.createSameSize(inp, inp.type());
    Mat dst = out.getMat();
    for (int y = 0; y < src.rows; y++) for (int x = 0; x < src.cols; x++) {
        dst.at<float>(y, x) = src.at<float>(y, x) + cos((x / float(src.cols) + phase) * freq * 2 * 3.14159226535);
    }
}

void imshowF(const std::string& windowName, const Mat& m) {
    Mat mag;//phase인 비율은 모르겠고 밝기만
    std::vector<Mat> channels;
    split(m, channels);
    magnitude(channels[0], channels[1], mag);
    //크기도 너무 크므로 좀 줄임
    log(mag + 1, mag); //log가 성립하려면 밑이 양수고 0이면 안된다. mag가 양수여야함
    Mat out(mag.size(), mag.type()); //mag를 가운데에 맞춰주기 위한 노력
    mag(Rect(0, 0, mag.cols / 2, mag.rows / 2)).copyTo(out(Rect(mag.cols / 2, mag.rows / 2, mag.cols / 2, mag.rows / 2))); //왼쪽 끝 1/4만 잘라서
    imshow(windowName, out);
}

int main(){
    /*
    Mat ff = Mat::zeros(512, 512, CV_32FC2); //0으로 된 이미지를 만들어라, 앞에 세로축이다(행렬은 그럼), 32float임, C2는 채널이 2개->complex(RGB는 채널이 3개)
    Mat img;
    ff.at<Vec2f>(0, 0)[0] = 0.5 * 512 * 512; //vec2f는 vector인데 크기가 2개(채널2개, 위에와 대응), at은 한 pixel의 값을 가지고 온다
    //0,0에 실수에다가 0.5를 넣었다 여기는 전체적인 밝기를 나타낸다. 이걸 DC(derect coreent 직류)
    // 0,1을 하면 x축 기준으로 cos함수
    // 즉 이미지를 구성하는, 이미지를 오르락 내리락하는 것으로 그 성분이 얼마나 들어있는지 보는 것
    // y축에 점을 찍으면 세로 방향으로
    // 여기까지는 실수 쪽을 건든 것이다
    ff.at<Vec2f>(2, 0)[1] = 0.5 * 512 * 512;
    //허수를 건들면 sin함수가 나온다
    // 둘다 줄 경우에는 약간 더 밝아지고(두개 제곱해서 더해서 루트) sin이 약간 움직인 느낌(두개의 비율이 위치를 좌우한다(phase))
    //inverse transform
    idft(ff, img, DFT_SCALE | DFT_REAL_OUTPUT);//scale은 transform하면 이미지만큼 커진다, 다시 복원하는 것. 뒤는 결과를 실수로 받기
    imshow("Img", img);
    
    Mat img = Mat::zeros(512, 512, CV_32FC1);
    resize(img, img, Size(256, 256)); //사이즈 크기 조정
    addCosX(img, img, 1); //phase는 sin과 관련이 있다, 얼만큼 delay가 잇냐
    Mat ff;
    dft(img, ff, DFT_COMPLEX_OUTPUT);
    //Mat channels[2];
    //split(ff, channels);
    GaussianBlur(img, img, Size(), 10); //사용하면 high frequency가 사라진다
    //ff2의 결과물로 어떤 주파수가 섞여 있는지 알 수 있다
    imshow("img", img); //채널 수를 맞춰 줘야한다
    imshowF("ff", ff); //점이 양 끝네 2개가 생긴다->원래 가져온 이미지의 크기는 512,512인데 지나면 512가 중복된다
    //imshow("ff_i", channels[1]);
    waitKey();
    */
    cv::Mat image = cv::imread("C:\\Users\\iyj94\\Desktop\\MedianFilterInput.png", 0);
    image.convertTo(image, CV_32F, 1 / 255.f);
    Mat out1;
    Mat out2;
    myMedianFilter(image, out1, 5);
    medianBlur(image, out2, 5);
    cv::imshow("myMedianFilter", out1);
    cv::imshow("medianBlur", out2);
    cv::imshow("Orgin", image);
    cv::waitKey();
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
