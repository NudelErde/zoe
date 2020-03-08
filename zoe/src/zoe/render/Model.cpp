//
// Created by florian on 14.02.20.
//

#include "Model.h"

Zoe::Model::~Model() = default;

Zoe::Model::Model() = default;

Zoe::Model::Model(const Zoe::File& file) {
    std::shared_ptr<std::istream> stream = file.getInputStream();
    int test;
    *stream >> test;
}
