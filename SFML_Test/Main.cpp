#include <SFML/Graphics.hpp>
using namespace sf;

class object {
public:
    bool is_ball;
    Vector2f position, size, velocity; //phys
    RectangleShape Rect; //graph
    void set_position(Vector2f pos) {
        position = pos;
        Rect.setPosition(position);
    }
    void set_size(Vector2f sz) {
        size = sz;
        Rect.setSize(size);
    }
    void set_speed(Vector2f vel) {
        velocity = vel;
    }
    void autoplay(object b, Vector2f vel[2]) {
        if (position.y > b.position.y){
            velocity = vel[0];
        }
        else {
            velocity = vel[1];
        }
    }
    bool move(object P[3]) {
        // vertical
        if (position.y + velocity.y < 3 or position.y + velocity.y + size.y > 715) {
            velocity.y *= -1;
        }
        //horisontal
        if (is_ball) {
            if (position.x < 20 or position.x + size.x > 1250) {
                velocity.x *= -1;
                return true;
            }

            for (int i = 0; i < 2; i++) {
                if (position.y + velocity.y < P[i].position.y + P[i].size.y and position.y + size.y + velocity.y > P[i].position.y) {
                    if (position.x + velocity.x < P[i].position.x + P[i].size.x and position.x + size.x + velocity.x > P[i].position.x) {
                        velocity.x *= -1;
                    }
                }
            }
        }
        position += velocity;
        Rect.setPosition(position);
        return false;
    }
};
void restart(object O[3], Vector2f st_vel) {
    Vector2f start_position[3] = { {90, 280},{1190, 280}, {620, 340} };
    Vector2f start_size[3] = { {10, 120}, {10, 120}, {40, 40} };
    Vector2f start_speed[3] = { {0, 0}, {0, 0}, {4, -3} };
    for (int i = 0; i < 3; i++) {
        O[i].set_position(start_position[i]);
        O[i].set_size(start_size[i]);
        O[i].set_speed(start_speed[i]);
        O[i].Rect.setFillColor(Color::White);
    }
    O[2].is_ball = true;
    O[1].velocity = st_vel;
}
int main()
{
    RenderWindow window(VideoMode(1280, 720), L"Новый Понг", Style::Default);
    window.setVerticalSyncEnabled(true);
    RectangleShape field;
    field.setPosition(0, 0);
    field.setSize({ 1280,720 });
    Texture font_texture;
    font_texture.loadFromFile("Field.png");
    field.setTexture(&font_texture);
    RectangleShape counter_shape[2];
    counter_shape[0].setSize({60, 120});
    counter_shape[1].setSize({ 60, 120 });
    counter_shape[0].setPosition({ 540, 64 });
    counter_shape[1].setPosition({ 680, 64 });

    Texture number[10];
    number[0].loadFromFile("Count0.png");
    number[1].loadFromFile("Count1.png");
    number[2].loadFromFile("Count2.png");
    number[3].loadFromFile("Count3.png");
    number[4].loadFromFile("Count4.png");
    number[5].loadFromFile("Count5.png");
    number[6].loadFromFile("Count6.png");
    number[7].loadFromFile("Count7.png");
    number[8].loadFromFile("Count8.png");
    number[9].loadFromFile("Count9.png");

    counter_shape[0].setTexture(&number[0]);
    counter_shape[1].setTexture(&number[0]);

    CircleShape shape(80.f, 50);
    shape.rotate(45);
    shape.setPosition(100, 100);
    shape.setFillColor(Color::White);
    
    object O[3];
    Vector2f pl_velocity[2] = { {0, -3}, {0, 3} };
    int counter[2] = { 0,0 };
    bool goal = false;

    restart(O, pl_velocity[0]);


    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


        //input
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            O[0].velocity = pl_velocity[0];
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            O[0].velocity = pl_velocity[1];
        }
        O[1].autoplay(O[2], pl_velocity);

        //logic Логика епт
        for (int i = 0; i < 3; i++) {
            goal = O[i].move(O);
        }

        if (goal) {
            goal = false;
            if (O[2].position.x > 600) {
                counter[0] ++;
            }
            else {
                counter[1] ++;
            }
            restart(O, pl_velocity[0]);
            counter_shape[0].setTexture(&number[counter[0]]);
            counter_shape[1].setTexture(&number[counter[1]]);
        }

        //print
        window.clear(Color::Black);
        window.draw(field);
        window.draw(counter_shape[0]);
        window.draw(counter_shape[1]);

        for (int i = 0; i < 3; i++) {
            window.draw(O[i].Rect);
        }
        
        sleep(milliseconds(15));
        window.display();
    }
    return 0;
}