#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Header {
    char idLength;
    char colorMapType;
    char imageType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
    unsigned int pixelArray;
    unsigned int rgbArray;
};

enum RGBValues {
    Blue, Green, Red
};

enum BlendMode {
    Multiply, Subtract, Screen, Overlay, Add
};

class Pixel {
    private:
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        unsigned short blueShort;
        unsigned short greenShort;
        unsigned short redShort;





        Pixel applyMultiplyBlendMode(Pixel layer) {
            unsigned char blueTemp;
            unsigned char greenTemp;
            unsigned char redTemp;

            float f1Blue = ((float) this->getBlueShort() / 255);
            float f2Blue = ((float) layer.getBlueShort() / 255);
            float finalFloatBlue = f1Blue * f2Blue * 255;
            int finalBlue = int(finalFloatBlue + 0.5);
            blueTemp = (unsigned char)finalBlue;

            float f1Green = ((float) this->getGreenShort() / 255);
            float f2Green = ((float) layer.getGreenShort() / 255);
            float finalFloatGreen = f1Green * f2Green * 255;
//            finalFloatGreen *= 255;
            int finalGreen = int(finalFloatGreen + 0.5);
            greenTemp = (unsigned char)finalGreen;

            float f1Red = ((float) this->getRedShort() / 255);
            float f2Red = ((float) layer.getRedShort() / 255);
            float finalFloatRed = f1Red * f2Red * 255;
            int finalRed = int(finalFloatRed + 0.5);
            redTemp = (unsigned char)finalRed;
            
            
            Pixel* pixel = new Pixel(blueTemp, greenTemp, redTemp);
            return *pixel;
        }

        Pixel applySubtractBlendMode(Pixel layer) {
            short blueTemp;
            short greenTemp;
            short redTemp;
            blueTemp = layer.getBlueShort() - this->getBlueShort();
            if (blueTemp < 0) {
                blueTemp = 0;
            }

            greenTemp = layer.getGreenShort() - this->getGreenShort();
            if (greenTemp < 0) {
                greenTemp = 0;
            }

            redTemp = layer.getRedShort() - this->getRedShort();
            if (redTemp < 0) {
                redTemp = 0;
            }

            if (blueTemp < 0 || blueTemp > 255) {
                cout << "blue temp out of bounds " << blueTemp << endl;
            }

            if (greenTemp < 0 || greenTemp > 255) {
                cout << "green temp out of bounds " << greenTemp << endl;
            }

            if (redTemp < 0 || redTemp > 255) {
                cout << "red temp out of bounds " << redTemp << endl;
            }

            Pixel* pixel = new Pixel((unsigned char) blueTemp, (unsigned char) greenTemp, (unsigned char) redTemp);
            return *pixel;

        }
        Pixel applyScreenBlendMode(Pixel layer) {
            unsigned char blueTemp;
            unsigned char greenTemp;
            unsigned char redTemp;

            float f1Blue = (float) this->getBlueShort();
            float f2Blue = (float) layer.getBlueShort();
            f1Blue /= 255;
            f2Blue /= 255;
            float finalBlue = (1 - ((1 - f1Blue) * (1 - f2Blue))) * 255;
            int finBlue = (int)(finalBlue + 0.5);
            blueTemp = (unsigned char)finBlue;

            float f1Green = (float) this->getGreenShort();
            float f2Green = (float) layer.getGreenShort();
            f1Green /= 255;
            f2Green /= 255;
            float finalGreen = (1 - ((1 - f1Green) * (1 - f2Green))) * 255;
            int finGreen = (int)(finalGreen + 0.5);
            greenTemp = (unsigned char)finGreen;

            float f1Red = (float) this->getRedShort();
            float f2Red = (float) layer.getRedShort();
            f1Red /= 255;
            f2Red /= 255;
            float finalRed = (1 - ((1 - f1Red) * (1 - f2Red))) * 255;
            int finRed = (int)(finalRed + 0.5);
            redTemp = (unsigned char)finRed;

            Pixel* pixel = new Pixel(blueTemp, greenTemp, redTemp);
            return *pixel;
            
        }
        Pixel applyOverlayBlendMode(Pixel layer) {
            float f1Blue = (float)this->getBlueShort();
            float f2Blue = (float)layer.getBlueShort();
            float finalBlue;

            float f1Green = (float)this->getGreenShort();
            float f2Green = (float)layer.getGreenShort();
            float finalGreen;

            float f1Red = (float)this->getRedShort();
            float f2Red = (float)layer.getRedShort();
            float finalRed;
            
            if (f2Blue/255 <= 0.5 ) {
                f1Blue /= 255;
                f2Blue /= 255;
                finalBlue = 2 * f1Blue * f2Blue;
                finalBlue *= 255;
            }
            else {
                f1Blue /= 255;
                f2Blue /= 255;
                finalBlue = 1 - (2 * (1- f1Blue) * (1- f2Blue));
                finalBlue *= 255;
            }

            if (f2Green/255 <= 0.5 ) {
                f1Green /= 255;
                f2Green /= 255;
                finalGreen = 2 * f1Green * f2Green;
                finalGreen *= 255;
            }
            else {
                f1Green /= 255;
                f2Green /= 255;
                finalGreen = 1 - (2 * (1 - f1Green) * (1 - f2Green));
                finalGreen *= 255;
            }

            if (f2Red/255 <= 0.5 ) {
                f1Red /= 255;
                f2Red /= 255;
                finalRed = 2 * f1Red * f2Red;
                finalRed *= 255;
            }
            else {
                f1Red /= 255;
                f2Red /= 255;
                finalRed = 1 - 2 * ((1 - f1Red) * (1 - f2Red));
                finalRed *= 255;
            }

            Pixel* pixel = new Pixel((unsigned char)finalBlue, (unsigned char)finalGreen, (unsigned char)finalRed);
            return *pixel;
        }

    public:
    Pixel(unsigned char blue, unsigned char green, unsigned char red) {
        this->blue = blue;
        this->green = green;
        this->red = red;
        this->blueShort = (unsigned short) blue;
        this->greenShort = (unsigned short) green;
        this->redShort = (unsigned short) red;
    }

    /**
     * Goal: Given another pixel layer and a blend mode, create a new pixel
     *
     * @param layer
     * @param mode
     * @return
     */
    Pixel applyBlendMode(Pixel layer, BlendMode mode) {
        switch (mode) {
            case Multiply:
                return applyMultiplyBlendMode(layer);
            case Subtract:
                return applySubtractBlendMode(layer);
            case Screen:
                return applyScreenBlendMode(layer);
            case Overlay:
                return applyOverlayBlendMode(layer);
        }
    }

    unsigned char getBlue() const {
        return blue;
    }

    unsigned char getGreen() const {
        return green;
    }

    unsigned char getRed() const {
        return red;
    }

    short getBlueShort() const {
        return blueShort;
    }

    short getGreenShort() const {
        return greenShort;
    }

    short getRedShort() const {
        return redShort;
    }
};

class Image {
    private:
        vector<Pixel> *pixels;
//        vector<Pixel> image2;
//        vector<Pixel> image3;
    public:
        Image(vector<Pixel> *pixels){
            this->pixels = pixels;
        }

        vector<Pixel>* getPixels(){
            return pixels;
        }

        Image* applyBlendMode(Image layer, BlendMode blendMode) {
            auto* blendedPixels = new vector<Pixel>();
            for (unsigned int i = 0; i < pixels->size(); i++) {
                auto pixel = pixels->at(i);
                auto layerPixel = layer.pixels->at(i);
                Pixel blendedPixel = pixel.applyBlendMode(layerPixel, blendMode);
                blendedPixels->push_back(blendedPixel);
            }

            return new Image(blendedPixels);
        }

    Image* applyMultiplySpecific (short red, short blue) {
        unsigned char blueTemp;
        unsigned char greenTemp;
        unsigned char redTemp;
        auto* blendedPixels = new vector<Pixel>();
        for (int i = 0; i < this->getPixels()->size(); ++i) {
            if (this->getPixels()->at(i).getRedShort() * red >= 255 || this->getPixels()->at(i).getRedShort() * red < this->getPixels()->at(i).getBlueShort()) {
                redTemp = 255;
            }
            else {
                redTemp = this->getPixels()->at(i).getRedShort() * red;
            }
            blueTemp = 0;
            greenTemp = this->getPixels()->at(i).getGreen();

            auto* pixel = new Pixel(blueTemp, greenTemp, redTemp);
            blendedPixels->push_back(*pixel);
        }
        return new Image(blendedPixels);
    }

    Image* applyAddBlendMode(int numToAdd, RGBValues color) {
        unsigned char blueTemp;
        unsigned char greenTemp;
        unsigned char redTemp;
        auto* blendedPixels = new vector<Pixel>();

        switch (color) {
            case Blue:
                for (int i = 0; i < this->getPixels()->size(); ++i) {
                    if (this->getPixels()->at(i).getBlueShort() + numToAdd >= 255 || this->getPixels()->at(i).getBlueShort() + numToAdd < this->getPixels()->at(i).getBlueShort()) {
                        blueTemp = 255;
                    }
                    else {
                        blueTemp = this->getPixels()->at(i).getBlueShort() + numToAdd;
                    }

                    greenTemp = this->getPixels()->at(i).getGreenShort();
                    redTemp = this->getPixels()->at(i).getRedShort();
                    auto* pixel = new Pixel(blueTemp, greenTemp, redTemp);
                    blendedPixels->push_back(*pixel);
                }
                return new Image(blendedPixels);
            case Green:
                for (int i = 0; i < this->getPixels()->size(); ++i) {
                    if (this->getPixels()->at(i).getGreenShort() + numToAdd >= 255 || this->getPixels()->at(i).getGreenShort() + numToAdd < this->getPixels()->at(i).getGreenShort()) {
                        greenTemp = 255;
                    }
                    else {
                        greenTemp = this->getPixels()->at(i).getGreenShort() + numToAdd;
                    }

                    blueTemp = this->getPixels()->at(i).getBlueShort();
                    redTemp = this->getPixels()->at(i).getRedShort();
                    auto* pixel = new Pixel(blueTemp, greenTemp, redTemp);
                    blendedPixels->push_back(*pixel);
                }
                return new Image(blendedPixels);
            case Red:
                for (int i = 0; i < this->getPixels()->size(); ++i) {
                    if (this->getPixels()->at(i).getRedShort() + numToAdd >= 255 || this->getPixels()->at(i).getRedShort() + numToAdd < this->getPixels()->at(i).getRedShort()) {
                        redTemp = 255;
                    }
                    else {
                        redTemp = this->getPixels()->at(i).getRedShort() + numToAdd;
                    }

                    greenTemp = this->getPixels()->at(i).getGreenShort();
                    blueTemp = this->getPixels()->at(i).getBlueShort();
                    auto* pixel = new Pixel(blueTemp, greenTemp, redTemp);
                    blendedPixels->push_back(*pixel);
                }
                return new Image(blendedPixels);
        }
    }

    Image* returnChannel(RGBValues color) {
        unsigned char blueTemp;
        unsigned char greenTemp;
        unsigned char redTemp;
            auto* pixels = new vector<Pixel>;
            switch (color) {
                case Blue:
                    for (int i = 0; i < this->getPixels()->size(); ++i) {
                        blueTemp = this->getPixels()->at(i).getBlue();
                        greenTemp = this->getPixels()->at(i).getBlue();
                        redTemp = this->getPixels()->at(i).getBlue();
                        auto* currentPixel = new Pixel(blueTemp, greenTemp, redTemp);
                        pixels->push_back(*currentPixel);
                    }
                    return new Image(pixels);
                case Green:
                    for (int i = 0; i < this->getPixels()->size(); ++i) {
                        greenTemp = this->getPixels()->at(i).getGreen();
                        blueTemp = this->getPixels()->at(i).getGreen();
                        redTemp = this->getPixels()->at(i).getGreen();
                        auto* currentPixel = new Pixel(blueTemp, greenTemp, redTemp);
                        pixels->push_back(*currentPixel);
                    }
                    return new Image(pixels);
                case Red:
                    for (int i = 0; i < this->getPixels()->size(); ++i) {
                        redTemp = this->getPixels()->at(i).getRed();
                        greenTemp = this->getPixels()->at(i).getRed();
                        blueTemp = this->getPixels()->at(i).getRed();
                        auto* currentPixel = new Pixel(blueTemp, greenTemp, redTemp);
                        pixels->push_back(*currentPixel);
                    }
                    return new Image(pixels);
            }
        }

        Image* combineChannels(Image* red, Image* green) {
            unsigned char blueTemp;
            unsigned char greenTemp;
            unsigned char redTemp;
            auto* blendedPixels = new vector<Pixel>();

            for (int i = 0; i < this->getPixels()->size(); ++i) {
                blueTemp = this->getPixels()->at(i).getBlue();
                greenTemp = green->getPixels()->at(i).getGreen();
                redTemp = red->getPixels()->at(i).getRed();

                auto* pixel = new Pixel(blueTemp, greenTemp, redTemp);
                blendedPixels->push_back(*pixel);
            }
            return new Image(blendedPixels);
        }

        Image* rotate() {
            unsigned char blueTemp;
            unsigned char greenTemp;
            unsigned char redTemp;
            auto* blendedPixels = new vector<Pixel>();

            for (int i = this->getPixels()->size() - 1; i >= 0 ; i--) {
                blueTemp = this->getPixels()->at(i).getBlue();
                greenTemp = this->getPixels()->at(i).getGreen();
                redTemp = this->getPixels()->at(i).getRed();

                auto* pixel = new Pixel(blueTemp, greenTemp, redTemp);
                blendedPixels->push_back(*pixel);
            }
            return new Image(blendedPixels);
        }
};

class FileParser {
private:
    ifstream fileStream;
    Header *header;

public:
    FileParser(string filePath) {
        this->fileStream = ifstream(filePath, ios_base::binary);
    }

    void parseHeader() {
        auto *header = new Header;
        if (this->fileStream.is_open()) {
            this->fileStream.read(&header->idLength, 1);
            this->fileStream.read(&header->colorMapType, 1);
            this->fileStream.read(&header->imageType, 1);
            this->fileStream.read(reinterpret_cast<char *>(&header->colorMapOrigin), 2);
            this->fileStream.read(reinterpret_cast<char *>(&header->colorMapLength), 2);
            this->fileStream.read(&header->colorMapDepth, 1);
            this->fileStream.read(reinterpret_cast<char *>(&header->xOrigin), 2);
            this->fileStream.read(reinterpret_cast<char *>(&header->yOrigin), 2);
            this->fileStream.read(reinterpret_cast<char *>(&header->width), 2);
            this->fileStream.read(reinterpret_cast<char *>(&header->height), 2);
            this->fileStream.read(&header->bitsPerPixel, 1);
            this->fileStream.read(&header->imageDescriptor, 1);
            header->pixelArray = header->width * header->height;
            header->rgbArray = header->pixelArray * 3;
        }
        this->header = header;
    }

    Image parseImage() {
        auto *pixelVector = new vector<Pixel>;
        this->fileStream.seekg(18);
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        for (int i = 0; i < header->rgbArray; ++i) {
            this->fileStream.read(reinterpret_cast<char *>(&blue), 1);
            i++;
            this->fileStream.read(reinterpret_cast<char *>(&green), 1);
            i++;
            this->fileStream.read(reinterpret_cast<char *>(&red), 1);
            Pixel *currentPixel = new Pixel(blue, green, red);
            pixelVector->push_back(*currentPixel);
        }
        Image *image = new Image(pixelVector);
        return *image;
    }

    Header* getHeader() {
        return header;
    }

    void writeFile(Header* header, Image image, string fileName) {
        ofstream outputFile(fileName, ios_base::binary);

        outputFile.write(&header->idLength, 1);
        outputFile.write(&header->colorMapType, 1);
        outputFile.write(&header->imageType, 1);
        outputFile.write(reinterpret_cast<char *>(&header->colorMapOrigin), 2);
        outputFile.write(reinterpret_cast<char *>(&header->colorMapLength), 2);
        outputFile.write(&header->colorMapDepth, 1);
        outputFile.write(reinterpret_cast<char *>(&header->xOrigin), 2);
        outputFile.write(reinterpret_cast<char *>(&header->yOrigin), 2);
        outputFile.write(reinterpret_cast<char *>(&header->width), 2);
        outputFile.write(reinterpret_cast<char *>(&header->height), 2);
        outputFile.write(&header->bitsPerPixel, 1);
        outputFile.write(&header->imageDescriptor, 1);

        unsigned long count = (image.getPixels()->size() * 3);
        auto* pixelArray = new unsigned char[count];
        unsigned long runningCount = 0;
        vector<Pixel> *pixels = image.getPixels();
        unsigned long size = pixels->size();

        for (unsigned long i = 0; i < size; ++i) {
            pixelArray[runningCount] = (unsigned char)pixels->at(i).getBlue();
            runningCount++;
            pixelArray[runningCount] = (unsigned char)pixels->at(i).getGreen();
            runningCount++;
            pixelArray[runningCount] = (unsigned char)pixels->at(i).getRed();
            runningCount++;
        }
        outputFile.write(reinterpret_cast<const char *>(pixelArray), count);
        outputFile.close();

    }
};



int main() {
//    auto* layer2Parserq = new FileParser("input/layer2.tga");
//    layer2Parserq->parseHeader();
//    Image layer2q = layer2Parserq->parseImage();
//    layer2Parserq->writeFile(layer2Parserq->getHeader(), layer2q, "output/test2.tga");
//    return 0;

//    short foo = 10;
//    short bar = 5;
//    char baz[1];
//    baz[0] = (char)(foo - bar);
//    cout << baz[0] << endl;
//    return 0;
    //task 1
    auto* fileParser = new FileParser("input/layer1.tga");
    fileParser->parseHeader();
    Image layer1 = fileParser->parseImage();
    auto* fileParser2 = new FileParser("input/pattern1.tga");
    fileParser2->parseHeader();
    Image pattern1 = fileParser2->parseImage();
    Image* multipliedImage = layer1.applyBlendMode(pattern1, Multiply);
    //could be an error here, I'm not sure which fileparser header to write from or if I have to make a new one
    //error with the write function which needs to be fixed
    fileParser->writeFile(fileParser->getHeader(), *multipliedImage, "output/part1.tga");

    delete fileParser;
    delete fileParser2;

    //task 2
    auto* layer2Parser = new FileParser("input/layer2.tga");
    layer2Parser->parseHeader();
    Image layer2 = layer2Parser->parseImage();
    auto* carParser = new FileParser("input/car.tga");
    carParser->parseHeader();
    Image car = carParser->parseImage();
    Image* subtractedImage = layer2.applyBlendMode(car, Subtract);
    layer2Parser->writeFile(layer2Parser->getHeader(), *subtractedImage, "output/part2.tga");

    //task 3
    auto* layer1Parser = new FileParser("input/layer1.tga");
    layer1Parser->parseHeader();
    Image layer1p3 = layer1Parser->parseImage();
    auto* pattern2Parser = new FileParser("input/pattern2.tga");
    pattern2Parser->parseHeader();
    Image pattern2p3 = pattern2Parser->parseImage();
    Image* multipliedLayer1Pattern2 = layer1p3.applyBlendMode(pattern2p3, Multiply);
    auto* textParser = new FileParser("input/text.tga");
    textParser->parseHeader();
    Image textImage = textParser->parseImage();
    Image* screenedImage = textImage.applyBlendMode(*multipliedLayer1Pattern2, Screen);
    layer1Parser->writeFile(layer1Parser->getHeader(), *screenedImage, "output/part3.tga");

    //task 4
    auto* layer2Parserp4 = new FileParser("input/layer2.tga");
    layer2Parserp4->parseHeader();
    Image layer2p4 = layer2Parserp4->parseImage();
    auto* circlesParser = new FileParser("input/circles.tga");
    circlesParser->parseHeader();
    Image circlesp4 = circlesParser->parseImage();
    Image* multipliedLayer2Circles = layer2p4.applyBlendMode(circlesp4, Multiply);

    auto* pattern2Parserp4 = new FileParser("input/pattern2.tga");
    pattern2Parserp4->parseHeader();
    Image pattern2p4 = pattern2Parserp4->parseImage();
    Image* subtractedImagep4 = pattern2p4.applyBlendMode(*multipliedLayer2Circles, Subtract);
    layer2Parserp4->writeFile(layer2Parserp4->getHeader(), *subtractedImagep4, "output/part4.tga");


    //task 5
    auto* layer1Parserp5 = new FileParser("input/layer1.tga");
    layer1Parserp5->parseHeader();
    Image layer1p5 = layer1Parserp5->parseImage();

    auto* patter1Parserp5 = new FileParser("input/pattern1.tga");
    patter1Parserp5->parseHeader();
    Image pattern1p5 = patter1Parserp5->parseImage();

    Image* overlayp5 = layer1p5.applyBlendMode(pattern1p5, Overlay);
    layer1Parserp5->writeFile(layer1Parserp5->getHeader(), *overlayp5, "output/part5.tga");

    //task 6
    auto* carParserp6 = new FileParser("input/car.tga");
    carParserp6->parseHeader();
    Image carp6 = carParserp6->parseImage();
    Image* addedCar = carp6.applyAddBlendMode(200, Green);
    carParserp6->writeFile(carParserp6->getHeader(), *addedCar, "output/part6.tga");

    //task 7
    auto* carParserp7 = new FileParser("input/car.tga");
    carParserp7->parseHeader();
    Image carp7 = carParserp7->parseImage();
    Image* multipliedCarp7 = carp7.applyMultiplySpecific(4, 0);
    carParserp7->writeFile(carParserp7->getHeader(), *multipliedCarp7, "output/part7.tga");

    //task 8
    auto* carParserp8 = new FileParser("input/car.tga");
    carParserp8->parseHeader();
    Image carp8 = carParserp8->parseImage();
    Image* part8_r = carp8.returnChannel(Red);
    Image* part8_g = carp8.returnChannel(Green);
    Image* part8_b = carp8.returnChannel(Blue);
    //fix the header being written here
    carParserp8->writeFile(carParserp8->getHeader(), *part8_r, "output/part8_r.tga");
    carParserp8->writeFile(carParserp8->getHeader(), *part8_g, "output/part8_g.tga");
    carParserp8->writeFile(carParserp8->getHeader(), *part8_b, "output/part8_b.tga");

    //task 9
    auto* layer_redParserp9 = new FileParser("input/layer_red.tga");
    layer_redParserp9->parseHeader();
    Image layer_redp9 = layer_redParserp9->parseImage();

    auto* layer_greenParserp9 = new FileParser("input/layer_green.tga");
    layer_greenParserp9->parseHeader();
    Image layer_greenp9 = layer_greenParserp9->parseImage();

    auto* layer_blueParserp9 = new FileParser("input/layer_blue.tga");
    layer_blueParserp9->parseHeader();
    Image layer_bluep9 = layer_blueParserp9->parseImage();

    Image* trippleBlend = layer_bluep9.combineChannels(&layer_redp9, &layer_greenp9);
    layer_redParserp9->writeFile(layer_redParserp9->getHeader(), *trippleBlend, "output/part9.tga");

    //task 10
    auto* text2Parserp10 = new FileParser("input/text2.tga");
    text2Parserp10->parseHeader();
    Image text2p10 = text2Parserp10->parseImage();

    Image* rotatedImage = text2p10.rotate();
    text2Parserp10->writeFile(text2Parserp10->getHeader(), *rotatedImage, "output/part10.tga");

    //ll
    auto* parser = new FileParser("input/texxt2.tga");
    parser->parseHeader();
    Image parsed = parser->parseImage();
    parser->writeFile(parser->getHeader(), parsed, "output/test.tga");
    return 0;

}
