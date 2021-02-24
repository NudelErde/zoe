//
// Created by Florian on 21.02.2021.
//

#include "zoe.h"
#include "Tetromino.h"
#include <random>
#include <array>

using namespace Zoe;

const std::string layerXML = R"(
<ComponentLayer defaultCamera="cam">
    <PhysicsGroup id="PhysicsGroup"/>
    <Camera2D x="0" y="0" width="600" height="1200" id="cam"/>
    <Sprite id="field" x="50" y="100">
        <Rectangle x="-5" y="-5" z="-0.2" width="510" height="1010" r="1" g="1" b="1"/>
        <Rectangle x="0" y="0" z="-0.1" width="500" height="1000" r="0" g="0" b="0"/>
        <Sprite id="pieces"/>
    </Sprite>
</ComponentLayer>
)";

std::shared_ptr<BaseComponent> playerPiece;
std::shared_ptr<ComponentLayer> game;
bool deadPieces[20 * 10]{};
std::array<int, 7> pieces{0, 1, 2, 3, 4, 5, 6};
int index = 7;

static void updatePlayerPiece() {
    Scheduler::addTask([]() {
        if (index == 7) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(pieces.begin(), pieces.end(), g);
            index = 0;
        }
        int nextPiece = pieces[index++];

        std::shared_ptr<BaseComponent> parent;
        if (playerPiece && (parent = playerPiece->getParent().lock())) {
            parent->remove(playerPiece);
        }

        playerPiece = BaseComponent::componentByXML(readXML(Tetromino::getIndexString(nextPiece)));
        game->getChildByID("field")->add(playerPiece);
    });
}

class Script : public NativeScript {
public:

    bool tryMoveTo(int tx, int ty) {
        auto cmp = getComponent();
        auto children = cmp->getChildByID("mode" + std::to_string(mode))->getChildren();
        bool die = false;
        for (const auto& child : children) {
            vec2 pos = child->getPosition();
            pos.x += 50 * tx;
            pos.y += 50 * ty;
            if (pos.x < 0) {
                tx += (int) (0 - pos.x / 50);
            } else if (pos.x > 9 * 50) {
                tx += (int) (9 - pos.x / 50);
            }

            if (pos.y > 19 * 50) {
                ty += (int) (19 - pos.y / 50);
                die = true;
            }

            int xi = (int) (pos.x / 50);
            int yi = (int) (pos.y / 50);
            if (xi < 10 && xi > 0 && yi < 20 && yi > 0 && deadPieces[xi + yi * 10]) {
                if (tx != x) {
                    //moving sideways
                    tx = x;
                } else if (ty != y) {
                    //moving down
                    ty = y;
                    die = true;
                } else {
                    return false;
                }
            }
        }

        x = tx;
        y = ty;
        if (die) {
            Scheduler::addTask([cmp = this->getComponent(), mode = this->mode]() {
                auto children = cmp->getChildByID("mode" + std::to_string(mode))->getChildren();
                for (const auto& child : children) {
                    child->setPosition(child->getPosition() + cmp->getPosition());

                    int x = (int) (child->getPosition().x / 50);
                    int y = (int) (child->getPosition().y / 50);
                    deadPieces[x + y * 10] = true;

                    child->getParent().lock()->remove(child);

                    auto rows = game->getChildByID("pieces")->getChildren();
                    for (auto row : rows) {
                        if (row->getPosition().y == child->getPosition().y) {
                            child->setPosition(vec3(child->getPosition().x, 0, 0));
                            row->add(child);
                            break;
                        }
                    }
                }
                updatePlayerPiece();
            });
        }
        return true;
    }

    void onUpdate(const double& d) override {
        if (lastGravity > 0) {
            lastGravity -= d;
        } else {
            lastGravity = 1;
            tryMoveTo(x, y + 1);
        }

        auto cmp = getComponent();

        if (Input::isKeyPressed(KEY_SPACE) && !rotatePressed) {
            cmp->getChildByID("mode" + std::to_string(mode))->setVisible(false);
            int startMode = mode;
            do {
                mode = mode + 1;
                mode %= 4;
            } while (!tryMoveTo(x, y) && mode != startMode);
            cmp->getChildByID("mode" + std::to_string(mode))->setVisible(true);
        }
        rotatePressed = Input::isKeyPressed(KEY_SPACE);

        if ((Input::isKeyPressed(KEY_D) || Input::isKeyPressed(KEY_RIGHT)) && !rightPressed) {
            tryMoveTo(x + 1, y);
        }
        rightPressed = Input::isKeyPressed(KEY_D) || Input::isKeyPressed(KEY_RIGHT);

        if ((Input::isKeyPressed(KEY_A) || Input::isKeyPressed(KEY_LEFT)) && !leftPressed) {
            tryMoveTo(x - 1, y);
        }
        leftPressed = Input::isKeyPressed(KEY_A) || Input::isKeyPressed(KEY_LEFT);

        if ((Input::isKeyPressed(KEY_S) || Input::isKeyPressed(KEY_DOWN)) && !downPressed) {
            tryMoveTo(x, y + 1);
        }
        downPressed = Input::isKeyPressed(KEY_S) || Input::isKeyPressed(KEY_DOWN);

        cmp->setPosition(vec3(x * 50, y * 50, 0));
        cmp->setVisible(true);
    }

    int mode = 0;
    double lastGravity = 0;
    int x = 4;
    int y = -1;
    bool rotatePressed = false;
    bool rightPressed = false;
    bool leftPressed = false;
    bool downPressed = false;
};

static void checkForRows(const std::shared_ptr<BaseComponent>& cmp) {
    auto rows = cmp->getChildren();
    int cleared = 0;
    std::sort(rows.begin(), rows.end(),
              [](const std::shared_ptr<BaseComponent>& a, const std::shared_ptr<BaseComponent>& b) {
                  return a->getPosition().y > b->getPosition().y;
              });

    for (uint8_t i = 0; i < rows.size(); ++i) {
        if (rows[i]->getChildren().size() == 10) {
            //full
            Scheduler::addTask([row = rows[i], cmp]() {
                cmp->remove(row);
            });
            ++cleared;
        }
        rows[i]->setPosition(vec3(0, rows[i]->getPosition().y + cleared * 50, 0));
    }

    bool changed = true;
    while(changed) {
        changed = false;
        bool foundLine = false;
        for(int8_t y = 19; y >= 0; --y) {
            if(foundLine) {
                memcpy(deadPieces + (y * 10) + 10, deadPieces + (y * 10), 10);
                changed = true;
            } else {
                foundLine = true;
                for(uint8_t x = 0; x < 10; ++x) {
                    if(!deadPieces[x + y * 10]) {
                        foundLine = false;
                        break;
                    }
                }
            }
        }
    }
}

static void addRows() {
    auto cmp = game->getChildByID("pieces");
    for (uint8_t i = 0; i < 20; ++i) {
        auto row = std::make_shared<Sprite>();
        row->setPosition(vec3(0, 50 * i, 0));
        cmp->add(row);
    }

    Scheduler::addCoroutine([cmp]() -> Task {
        while (true) {
            checkForRows(cmp);
            co_yield true;
        }
    });
}

class App : public Application {
public:
    App() {
        NativeScriptComponent::registerNativeScript<Script>("Script");

        game = std::make_shared<ComponentLayer>();
        game->load(readXML(layerXML));

        updatePlayerPiece();
        addRows();

        getLayerStack().pushLayer(game);

        getWindow().setTitle("Tetris");
        getWindow().setSize(300, 600);
    }
};

Application* Zoe::createApplication() {
    return new App();
}