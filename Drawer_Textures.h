#pragma once
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <vector>
#include <iostream>
using namespace sf;
using namespace std;

class Textures {
public:
    vector<Texture> textures;
    const int n = 12;
    Textures() {
        textures.resize(n);
        for (int i = 0; i < n; ++i) {
            textures[i].loadFromFile("Textures/" + to_string(i) + ".png");
            textures[i].setRepeated(1);
        }
    }
    Texture& getTexture(int figure) {
        assert(figure < n && figure >= 0);
        return textures[figure];
    }
};