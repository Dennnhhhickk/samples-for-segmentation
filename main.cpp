#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define x first
#define y second
typedef unsigned char* UImagePoint;

int alpha = 0;
int betha = 0;
int thetha = 0;

bool check(std::pair<int, int> point, int width, int height)
{
    return 0 <= point.x && point.x < height && 0 <= point.y && point.y < width;
}

UImagePoint bfs(UImagePoint img, int width, int height, int value, UImagePoint used, long long *ans, std::pair<int, int> point_in, int b)
{
    std::queue<std::pair<int, int> > q;
    q.push(point_in);
    used[point_in.x * width + point_in.y] = 200;
    while (!q.empty()) {
        std::pair<int, int> tmp = q.front();
        q.pop();
        ans[tmp.x * width + tmp.y] = value;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                if (x == 0 || y == 0) {
                    std::pair<int, int> point;
                    point.x = tmp.x + x;
                    point.y = tmp.y + y;
                    if (check(point, width, height) && used[point.x * width + point.y] == 0 && 
                    std::abs(img[(point.x * width + point.y)] - img[(point_in.x * width + point_in.y)]) <= b) {
                        used[point.x * width + point.y] = 200;
                        q.push(point);
                    }
                }
            }
        }
    }
}

UImagePoint zalivka(UImagePoint img, int width, int height)
{
    long long *ans = (long long*)calloc(width * height, sizeof(long long));
    UImagePoint used = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    int *visited = (int*)calloc(width * height, sizeof(int));
    int k = 0;
    std::pair<int, int> dir = std::make_pair(0, 1);
    std::pair<int, int> point = std::make_pair(0, 0);
    for (int i = 0; i < width * height; ++i) {
        visited[point.x * width + point.y] = 1;
        if (used[point.x * width + point.y] == 0) {
            ++k;
            bfs(img, width, height, k, used, ans, std::make_pair(point.x, point.y), alpha);
        }
        std::pair<int, int> tmp;
        tmp.x = point.x + dir.x;
        tmp.y = point.y + dir.y;
        if (!check(tmp, width, height) || visited[tmp.x * width + tmp.y] == 1) {
            tmp.x = dir.y;
            tmp.y = -dir.x;
            dir = tmp;
        }
        point.x += dir.x;
        point.y += dir.y;
    }
    std::cout << "MAX K:" << k << std::endl;
    free(used);
    free(visited);
    UImagePoint res = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    for (int i = 0; i < width * height; ++i) {
            res[i] = (unsigned char)(ans[i] * 255.0 / k);
    }
    free(ans);
    return res;
}

UImagePoint cleaner(UImagePoint img, int width, int height)
{
    long long *ans = (long long*)calloc(width * height, sizeof(long long));
    UImagePoint used = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    int k = 0;
    for (int i = 0; i < width * height; ++i) {
        if (used[i] == 0) {
            ++k;
            bfs(img, width, height, k, used, ans, std::make_pair(i / width, i % width), alpha);
        }
    }
    std::cout << "MAX K:" << k << std::endl;
    free(used);
    UImagePoint res = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    std::vector<long long> cnt(k + 1);
    for (int i = 0; i <= k; ++i) {
        cnt[i] = 0;
    }
    for (int i = 0; i < width * height; ++i) {
        ++cnt[ans[i]];
    }
    for (int i = 0; i < width * height; ++i) {
        if (cnt[ans[i]] < betha) {
            res[i] = (img[i] ? 0 : 255);
        } else {
            res[i] = img[i];
        }
    }
    free(ans);
    return res;
}

UImagePoint finder(UImagePoint img, int width, int height)
{
    long long *ans = (long long*)calloc(width * height, sizeof(long long));
    UImagePoint used = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    int *visited = (int*)calloc(width * height, sizeof(int));
    int k = 0;
    std::pair<int, int> dir = std::make_pair(0, 1);
    std::pair<int, int> point = std::make_pair(0, 0);
    for (int i = 0; i < width * height; ++i) {
        visited[point.x * width + point.y] = 1;
        if (used[point.x * width + point.y] == 0) {
            ++k;
            bfs(img, width, height, k, used, ans, std::make_pair(point.x, point.y), alpha);
        }
        std::pair<int, int> tmp;
        tmp.x = point.x + dir.x;
        tmp.y = point.y + dir.y;
        if (!check(tmp, width, height) || visited[tmp.x * width + tmp.y] == 1) {
            tmp.x = dir.y;
            tmp.y = -dir.x;
            dir = tmp;
        }
        point.x += dir.x;
        point.y += dir.y;
    }
    std::cout << "MAX K:" << k << std::endl;
    free(used);
    free(visited);
    std::vector<long long> cnt(k + 1);
    std::vector<bool> bol[2];
    for (int i = 0; i <= k; ++i) {
        cnt[i] = 0;
        bol[0].resize(k + 1);
        bol[1].resize(k + 1);
    }
    // std::vector<long long> bright(256), brightsum(256);
    // for (int i = 0; i < 256; ++i) {
    //     bright[i] = 0;
    //     brightsum[i] = 0;
    // }
    std::vector<std::set<int> > p(k + 1);
    UImagePoint res = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    for (int i = 0; i < width * height; ++i) {
        if (cnt[ans[i]] < img[i]) {
            cnt[ans[i]] = img[i];
        }
        for (int j = -3; j < 3; ++j) {
            std::pair<int, int> point = std::make_pair(i / width + j, i % width);
            if (check(point, width, height) && ans[i] != ans[point.x * width + point.y]) {
                p[ans[i]].insert(ans[point.x * width + point.y]);
                p[ans[point.x * width + point.y]].insert(ans[i]);
            }
            point = std::make_pair(i / width, i % width + j);
            if (check(point, width, height) && ans[i] != ans[point.x * width + point.y]) {
                p[ans[i]].insert(ans[point.x * width + point.y]);
                p[ans[point.x * width + point.y]].insert(ans[i]);
            }
        }
    }
    for (int i = 0; i <= k; ++i) {
        bol[0][i] = cnt[i] > 0;
        for (auto j : p[i]) {
            if (cnt[i] > cnt[j] && (cnt[i] - cnt[j]) < thetha) {
                bol[0][i] = false;
            }
        }
        if (bol[0][i]) {
            for (auto j : p[i]) {
                bol[1][j] = true;
            }
            bol[1][i] = true;
        }
    }
    for (int i = 0; i < width * height; ++i) {
        if (bol[1][ans[i]]) {
            res[i] = 255;
        } else {
            res[i] = 0;
        }
    }
    free(ans);
    return res;
}

UImagePoint to_gray(UImagePoint img, int width, int height, int channels)
{
    UImagePoint res = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int sum = 0;
            for (int k = 0; k < channels; ++k) {
                sum += img[(i * width + j) * channels + k];
            }
            res[i * width + j] = sum / 3.0;
        }
    }
    return res;
}

int main(int argc, char *argv[])
{
    std::string path = "";
    std::string outputdir = "";
    if (argc > 5) {
        path = argv[1];
        outputdir = argv[2];
        alpha = atoi(argv[3]);
        betha = atoi(argv[4]);
        thetha = atof(argv[5]);
    } else {
        return std::cout << "Error minimum 5 arguments" << std::endl, 0;
    }

    int width, height, channels;
    UImagePoint img = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    int max = -1, min = 256;

    for (int i = 0; i < width * height * channels; ++i) {
        if (img[i] < min) {
            min = img[i];
        }
        if (img[i] > max) {
            max = img[i];
        }
    }

    std::cout << "IMG MIN:" << min << " IMG MAX:" << max << std::endl;

    UImagePoint res = to_gray(img, width, height, channels);

    stbi_write_png((outputdir + "/0_GRAY.png").c_str(), width, height, 1, res, width * sizeof(unsigned char));

    UImagePoint ans = zalivka(res, width, height);

    stbi_write_png((outputdir + "/1_ZALIVKA.png").c_str(), width, height, 1, ans, width * sizeof(unsigned char));

    max = -1;
    min = 256;
    for (int i = 0; i < width * height; ++i) {
        if (ans[i] < min) {
            min = ans[i];
        }
        if (ans[i] > max) {
            max = ans[i];
        }
    }
    std::cout << "ANS MIN:" << min << " ANS MAX:" << max << std::endl;

    UImagePoint FUCK = (UImagePoint)calloc(width * height, sizeof(unsigned char));
    for (int i = 0; i < width * height; ++i) {
        if (ans[i] > 0) {
            FUCK[i] = 255;
        }
    }

    // UImagePoint f = podlivka(ans, width, height, answer);
    stbi_write_png((outputdir + "/2_MASK.png").c_str(), width, height, 1, FUCK, width * sizeof(unsigned char));

    UImagePoint FUCK2 = cleaner(FUCK, width, height);
    stbi_write_png((outputdir + "/3_CLEANER.png").c_str(), width, height, 1, FUCK2, width * sizeof(unsigned char));

    UImagePoint FUCK3 = (UImagePoint)calloc(width * height, sizeof(unsigned char));

    for (int i = 0; i < width * height; ++i) {
        if (FUCK2[i] == 255) {
            FUCK3[i] = res[i];
        } else {
            FUCK3[i] = 0;
        }
    }
    
    UImagePoint FUCK4 = finder(FUCK3, width, height);
    stbi_write_png((outputdir + "/4_FINDER.png").c_str(), width, height, 1, FUCK4, width * sizeof(unsigned char));


    stbi_image_free(img);
    stbi_image_free(res);
    stbi_image_free(ans);
    stbi_image_free(FUCK);
    stbi_image_free(FUCK2);
    stbi_image_free(FUCK3);
    stbi_image_free(FUCK4);
    return 0;
}