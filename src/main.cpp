#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <limits>

struct Pixel {
    unsigned char b, g, r;
};

struct Image {
    int width, height;
    std::vector<Pixel> pixels;

    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
};

bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    file.seekg(12); // Skip to width and height in TGA header
    file.read(reinterpret_cast<char*>(&width), 2);
    file.read(reinterpret_cast<char*>(&height), 2);

    // Check if width and height are reasonable
    const int MAX_DIMENSION = 4096; // Set a sensible max dimension limit
    if (width <= 0 || height <= 0 || width > MAX_DIMENSION || height > MAX_DIMENSION) {
        std::cerr << "Error: Invalid or excessively large image dimensions in " << filename
                  << " (" << width << "x" << height << ")" << std::endl;
        return false;
    }

    file.seekg(18); // Skip to pixel data

    try {
        pixels.resize(width * height); // Resize to fit all pixels
    } catch (const std::length_error& e) {
        std::cerr << "Error: Failed to resize pixels vector - " << e.what() << std::endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    if (file.gcount() != pixels.size() * sizeof(Pixel)) {
        std::cerr << "Error: Pixel data read size mismatch in " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

bool Image::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    unsigned char header[18] = {0};
    header[2] = 2;
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24;

    file.write(reinterpret_cast<const char*>(header), 18);
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    file.close();
    return true;
}

// Manipulation Functions
Pixel multiply(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round((p1.b / 255.0) * (p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round((p1.g / 255.0) * (p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round((p1.r / 255.0) * (p2.r / 255.0) * 255));
    return result;
}

Pixel screen(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round(255 - (1 - p1.b / 255.0) * (1 - p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round(255 - (1 - p1.g / 255.0) * (1 - p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round(255 - (1 - p1.r / 255.0) * (1 - p2.r / 255.0) * 255));
    return result;
}

Pixel subtract(const Pixel& p1, const Pixel& p2) {
    return {std::max(0, p1.b - p2.b), std::max(0, p1.g - p2.g), std::max(0, p1.r - p2.r)};
}

Pixel add(const Pixel& p1, const Pixel& p2) {
    return {std::min(255, p1.b + p2.b), std::min(255, p1.g + p2.g), std::min(255, p1.r + p2.r)};
}

Pixel overlay(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    auto overlay_channel = [](float np1, float np2) {
        return (np2 <= 0.5) ? 2 * np1 * np2 : 1 - 2 * (1 - np1) * (1 - np2);
    };
    result.b = static_cast<unsigned char>(overlay_channel(p1.b / 255.0, p2.b / 255.0) * 255);
    result.g = static_cast<unsigned char>(overlay_channel(p1.g / 255.0, p2.g / 255.0) * 255);
    result.r = static_cast<unsigned char>(overlay_channel(p1.r / 255.0, p2.r / 255.0) * 255);
    return result;
}

// Task functions
void task1(const std::string& output) {
    Image layer1, pattern1;
    layer1.load("input/layer1.tga");
    pattern1.load("input/pattern1.tga");

    Image result = layer1;
    for (int i = 0; i < layer1.pixels.size(); ++i) {
        result.pixels[i] = multiply(layer1.pixels[i], pattern1.pixels[i]);
    }
    result.save(output);
}

void task2(const std::string& output) {
    Image layer2, car;
    layer2.load("input/layer2.tga");
    car.load("input/car.tga");

    Image result = car;
    for (int i = 0; i < car.pixels.size(); ++i) {
        result.pixels[i] = subtract(car.pixels[i], layer2.pixels[i]);
    }
    result.save(output);
}

void task3(const std::string& output) {
    Image layer1, pattern2, text;
    layer1.load("input/layer1.tga");
    pattern2.load("input/pattern2.tga");

    Image temp_result = layer1;
    for (int i = 0; i < layer1.pixels.size(); ++i) {
        temp_result.pixels[i] = multiply(layer1.pixels[i], pattern2.pixels[i]);
    }

    text.load("input/text.tga");
    for (int i = 0; i < text.pixels.size(); ++i) {
        temp_result.pixels[i] = screen(temp_result.pixels[i], text.pixels[i]);
    }
    temp_result.save(output);
}

void task4(const std::string& output) {
    Image layer2, circles, pattern2;
    layer2.load("input/layer2.tga");
    circles.load("input/circles.tga");

    Image temp_result = layer2;
    for (int i = 0; i < layer2.pixels.size(); ++i) {
        temp_result.pixels[i] = multiply(layer2.pixels[i], circles.pixels[i]);
    }

    pattern2.load("input/pattern2.tga");
    for (int i = 0; i < pattern2.pixels.size(); ++i) {
        temp_result.pixels[i] = subtract(temp_result.pixels[i], pattern2.pixels[i]);
    }
    temp_result.save(output);
}

void task5(const std::string& output) {
    Image layer1, pattern1;
    layer1.load("input/layer1.tga");
    pattern1.load("input/pattern1.tga");

    Image result = layer1;
    for (int i = 0; i < layer1.pixels.size(); ++i) {
        result.pixels[i] = overlay(layer1.pixels[i], pattern1.pixels[i]);
    }
    result.save(output);
}

void task6(const std::string& output) {
    Image car;
    car.load("input/car.tga");

    for (Pixel& p : car.pixels) {
        p.g = std::min(255, p.g + 200);
    }
    car.save(output);
}

void task7(const std::string& output) {
    Image car;
    car.load("input/car.tga");

    for (Pixel& p : car.pixels) {
        p.r = std::min(255, p.r * 4);
        p.b = 0;
    }
    car.save(output);
}

void task8(const std::string& output_r, const std::string& output_g, const std::string& output_b) {
    Image car;
    car.load("input/car.tga");

    Image red = car, green = car, blue = car;
    for (int i = 0; i < car.pixels.size(); ++i) {
        red.pixels[i] = {car.pixels[i].r, car.pixels[i].r, car.pixels[i].r};
        green.pixels[i] = {car.pixels[i].g, car.pixels[i].g, car.pixels[i].g};
        blue.pixels[i] = {car.pixels[i].b, car.pixels[i].b, car.pixels[i].b};
    }
    red.save(output_r);
    green.save(output_g);
    blue.save(output_b);
}

void task9(const std::string& output) {
    Image layer_red, layer_green, layer_blue;
    layer_red.load("input/layer_red.tga");
    layer_green.load("input/layer_green.tga");
    layer_blue.load("input/layer_blue.tga");

    Image result = layer_red;
    for (int i = 0; i < result.pixels.size(); ++i) {
        result.pixels[i] = {layer_blue.pixels[i].b, layer_green.pixels[i].g, layer_red.pixels[i].r};
    }
    result.save(output);
}

void task10(const std::string& output) {
    Image text2;
    text2.load("input/text2.tga");

    Image result = text2;
    int width = text2.width;
    int height = text2.height;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            result.pixels[y * width + x] = text2.pixels[(height - y - 1) * width + (width - x - 1)];
        }
    }
    result.save(output);
}

int main() {
    task1("output/part1.tga");
    task2("output/part2.tga");
    task3("output/part3.tga");
    task4("output/part4.tga");
    task5("output/part5.tga");
    task6("output/part6.tga");
    task7("output/part7.tga");
    task8("output/part8_r.tga", "output/part8_g.tga", "output/part8_b.tga");
    task9("output/part9.tga");
    task10("output/part10.tga");

    std::cout << "All tasks completed successfully!" << std::endl;
    return 0;
}
