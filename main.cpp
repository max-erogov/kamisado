#include <set>

#include <SFML/Graphics.hpp>

int tile_color(int x, int y) {
    int z = x ^ y;
    return (z + (y * y + y) * (z * z + z)) & 7;
}


const sf::Color tile_colors[8] = {{191, 103, 0},
                                  {0,   0,   191},
                                  {127, 159, 191},
                                  {191, 95,  95},
                                  {191, 175, 0},
                                  {191, 0,   0},
                                  {0,   191, 0},
                                  {47,  15,  0}};

const sf::Color stone_colors[8] = {{207, 127, 0},
                                   {31,  31,  223},
                                   {159, 191, 223},
                                   {223, 127, 127},
                                   {223, 223, 0},
                                   {223, 31,  31},
                                   {31,  223, 31},
                                   {63,  31,  0}};


int board[8][8];

void generate_available_moves(int x, int y, bool positive, std::set<std::pair<int, int>> &st) {
    bool u = true, lu = true, ru = true;
    int dy = positive ? 1 : -1;
    int dist = 1;

    while (u | lu | ru) {
        if (u && 0 <= y + dy * dist && y + dy * dist <= 7 && board[y + dy * dist][x] == 0) {
            st.insert({x, y + dy * dist});
        } else u = false;
        if (lu && 0 <= y + dy * dist && y + dy * dist <= 7 && x - dist >= 0 && board[y + dy * dist][x - dist] == 0) {
            st.insert({x - dist, y + dy * dist});
        } else lu = false;
        if (ru && 0 <= y + dy * dist && y + dy * dist <= 7 && x + dist <= 7 && board[y + dy * dist][x + dist] == 0) {
            st.insert({x + dist, y + dy * dist});
        } else ru = false;
        dist++;
    }
}



int main() {
    for (int i = 0; i < 8; ++i) {
        board[0][i] = 16 + i;
        board[7][i] = 31 - i;
    }

    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    window.setVerticalSyncEnabled(true);

    bool pick_stone = false;
    int who_win = 0;
    int move_count = 0;
    std::pair<int, int> stone_pos = {-1, -1};
    std::set<std::pair<int, int>> available_moves;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && who_win == 0) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int u = (mousePos.x - 1) / 100, v = (mousePos.y - 1) / 100;
                if (std::make_pair(u, v) == stone_pos) {
                    pick_stone = !pick_stone;
                } else if (v == 7 && move_count == 0 && !pick_stone) {
                    available_moves.clear();
                    pick_stone = true;
                    stone_pos = {u, v};
                    generate_available_moves(u, v, false, available_moves);
                } else if (available_moves.find({u, v}) != available_moves.end()) {
                    available_moves.clear();
                    move_count++;
                    board[v][u] = board[stone_pos.second][stone_pos.first];
                    board[stone_pos.second][stone_pos.first] = 0;

                    for (int i = 0; i < 8; ++i) {
                        for (int j = 0; j < 8; ++j) {
                            if (board[i][j] == 24 + tile_color(u, v) - (move_count & 1) * 8) {
                                stone_pos = {j, i};
                                generate_available_moves(j, i, move_count & 1, available_moves);
                                break;
                            }
                        }
                    }
                }

            }
        }
        // Draw tiles
        sf::RectangleShape rectangleShape(sf::Vector2f(98.0f, 98.0f));
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                rectangleShape.setPosition((float) (j * 100 + 1), (float) (i * 100 + 1));
                rectangleShape.setFillColor(tile_colors[tile_color(j, i)]);
                window.draw(rectangleShape);
            }
        }

        // Draw stones
        sf::CircleShape stone(34);
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j]) {
                    stone.setPosition((float) (j * 100 + 16), (float) (i * 100 + 16));
                    stone.setFillColor(stone_colors[board[i][j] & 7]);
                    stone.setOutlineThickness(12.0f);
                    stone.setOutlineColor((board[i][j] & 8) ? sf::Color::White : sf::Color::Black);
                    window.draw(stone);


                }
            }
        }

        if (pick_stone) {
            sf::CircleShape hint(30);
            for (auto [j, i]: available_moves) {
                hint.setPosition((float) (j * 100 + 20), (float) (i * 100 + 20));
                hint.setFillColor(sf::Color(127, 223, 127, 127));
                window.draw(hint);
            }
        }
        window.display();
    }
    return 0;
}
