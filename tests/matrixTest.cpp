//
// Created by florian on 20.05.20.
//

#include <zoe.h>

using namespace Zoe;

class App : public Application {
public:
    App() : Application(false) {
        //--------------------------------------------------------------------------------------------------------------
        //Constructor tests
        //--------------------------------------------------------------------------------------------------------------
        {
            mat2x2 m2;
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    if (m2[i][j] != 0) {
                        throw std::runtime_error("mat2x2 is not initialized with 0");
                    }
                }
            }
            info("mat2x2 is initialized with 0");
        }
        {
            mat3x3 m3;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (m3[i][j] != 0) {
                        throw std::runtime_error("mat3x3 is not initialized with 0");
                    }
                }
            }
            info("mat3x3 is initialized with 0");
        }
        {
            mat4x4 m4;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    if (m4[i][j] != 0) {
                        throw std::runtime_error("mat4x4 is not initialized with 0");
                    }
                }
            }
            info("mat4x4 is initialized with 0");
        }

        //--------------------------------------------------------------------------------------------------------------
        //mat2x2
        //--------------------------------------------------------------------------------------------------------------
        {
            mat2x2 a;
            info("mat2x2 tests: ");
            a[0][0] = 3;
            a[1][0] = -1;
            a[0][1] = 2;
            a[1][1] = 7;
            float det = a.determinant();
            if (det == 23) {
                info("mat2x2.determinant passed!");
            } else {
                error("mat2x2.determinant failed!");
                throw std::runtime_error("mat2x2.determinant failed!");
            }
            if (a == a.transpose().transpose()) {
                debug("(a ^ T) ^ T == a");
                info("mat2x2.transpose passed!");
            } else {
                debug("(a ^ T) ^ T != a");
                error("mat2x2.transpose failed!");
                throw std::runtime_error("mat2x2.transpose failed!");
            }

            mat2x2 div = a * a.inverse() - mat2x2::identity();
            float sum = std::abs(div[0][0]) + std::abs(div[0][1]) + std::abs(div[1][0]) + std::abs(div[1][1]);
            if (sum < 0.1) {
                debug("a ^-1 == I");
                info("mat2x2.inverse passed!");
            } else {
                debug("a ^-1 != I");
                error("mat2x2.inverse failed!");
                throw std::runtime_error("mat2x2.inverse failed!");
            }
        }

        //--------------------------------------------------------------------------------------------------------------
        //mat3x3
        //--------------------------------------------------------------------------------------------------------------
        {
            mat3x3 a;
            info("mat3x3 tests: ");
            a[0][0] = 3;
            a[1][0] = -1;
            a[2][0] = 8;
            a[0][1] = 2;
            a[1][1] = 7;
            a[2][1] = -3;
            a[0][2] = 6;
            a[1][2] = 0;
            a[2][2] = -3;
            float det = a.determinant();
            if (det == -387) {
                info("mat3x3.determinant passed!");
            } else {
                error("mat3x3.determinant failed!");
                throw std::runtime_error("mat3x3.determinant failed!");
            }
            if (a == a.transpose().transpose()) {
                debug("(a ^ T) ^ T == a");
                info("mat3x3.transpose passed!");
            } else {
                debug("(a ^ T) ^ T != a");
                error("mat3x3.transpose failed!");
                throw std::runtime_error("mat3x3.transpose failed!");
            }

            mat3x3 div = a * a.inverse() - mat3x3::identity();
            float sum = std::abs(div[0][0]) + std::abs(div[0][1]) +std::abs(div[0][2])
                    + std::abs(div[1][0]) + std::abs(div[1][1]) +std::abs(div[1][2])
                    + std::abs(div[2][0]) + std::abs(div[2][1]) +std::abs(div[2][2]);
            if (sum < 0.1) {
                debug("a ^-1 == I");
                info("mat3x3.inverse passed!");
            } else {
                debug("a ^-1 != I");
                error("mat3x3.inverse failed!");
                throw std::runtime_error("mat3x3.inverse failed!");
            }
        }

        //--------------------------------------------------------------------------------------------------------------
        //mat4x4
        //--------------------------------------------------------------------------------------------------------------
        {
            mat4x4 a;
            info("mat4x4 tests: ");
            a[0][0] = 1;
            a[0][1] = 2;
            a[0][2] = -3;
            a[0][3] = 4;
            a[1][0] = 5;
            a[1][1] = 6;
            a[1][2] = -7;
            a[1][3] = 0;
            a[2][0] = 9;
            a[2][1] = 10;
            a[2][2] = 11;
            a[2][3] = 12;
            a[3][0] = 13;
            a[3][1] = 14;
            a[3][2] = 15;
            a[3][3] = 16;
            float det = a.determinant();
            if (det == -192) {
                info("mat4x4.determinant passed!");
            } else {
                error("mat4x4.determinant failed!");
                throw std::runtime_error("mat4x4.determinant failed!");
            }
            if (a == a.transpose().transpose()) {
                debug("(a ^ T) ^ T == a");
                info("mat4x4.transpose passed!");
            } else {
                debug("(a ^ T) ^ T != a");
                error("mat4x4.transpose failed!");
                throw std::runtime_error("mat4x4.transpose failed!");
            }

            mat4x4 div = a * a.inverse() - mat4x4::identity();
            float sum = std::abs(div[0][0]) + std::abs(div[0][1]) +std::abs(div[0][2]) + std::abs(div[0][3])
                        + std::abs(div[1][0]) + std::abs(div[1][1]) +std::abs(div[1][2]) + std::abs(div[1][3])
                        + std::abs(div[2][0]) + std::abs(div[2][1]) +std::abs(div[2][2]) + std::abs(div[2][3])
                        + std::abs(div[3][0]) + std::abs(div[3][1]) +std::abs(div[3][2]) + std::abs(div[3][3]);
            if (sum < 0.1) {
                debug("a ^-1 == I");
                info("mat4x4.inverse passed!");
            } else {
                debug("a ^-1 != I");
                error("mat4x4.inverse failed!");
                throw std::runtime_error("mat4x4.inverse failed!");
            }
        }
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
