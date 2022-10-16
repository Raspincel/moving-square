#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


void changeDirection(float& direction, sf::RectangleShape& rectangle);
void changeVelocity(float& velocity, float* options, int newV);
void move(sf::RectangleShape& rectangle, int x, int y, float moveX, float moveY);
void setText(sf::Text& text, sf::Font& font);

int main()
{
    sf::VideoMode fullscreen{ sf::VideoMode::getFullscreenModes()[0] };
    sf::RenderWindow window{ fullscreen, "Waiting screen", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen};
    window.setVerticalSyncEnabled(true);


    const int xSize{ static_cast<int>(fullscreen.width) };
    const int ySize{ static_cast<int>(fullscreen.height) };

    std::mt19937 number{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };

    // random places where the rectangle may appear at the start
    std::uniform_int_distribution<> randomNbX{ 0, xSize - 50 };
    std::uniform_int_distribution<> randomNbY{ 0, ySize - 50 };

    sf::RectangleShape rectangle{ sf::Vector2f(50, 50) };

    rectangle.setPosition(static_cast<float>(randomNbX(number)), static_cast<float>(randomNbY(number)));
    rectangle.setFillColor(sf::Color::White);

    // set of speeds to which the square can change with a mouse's click
    float velocities[]{ 0.5f, 1.f, 2.f, 3.f, 4.f, 5.f, 10.f, 15.f, 20.f, 30.f };
    int currentVelocity{ 0 };

    // the initial speed is defined here
    float xSpeed{ velocities[currentVelocity] };
    float ySpeed{ velocities[currentVelocity] };

    sf::Clock clock;
    sf::Event ev;
    sf::Time elapsed;

    // Indicates current velocity
    sf::Text text;
    sf::Font font;

    setText(text, font);

    while (window.isOpen())
    {
        elapsed = clock.getElapsedTime();

        if (elapsed.asMilliseconds() > 5)
        {
            // If it hits the wall...
            if (rectangle.getPosition().x + 50 > xSize || rectangle.getPosition().x < 0)
                changeDirection(xSpeed, rectangle);
            if (rectangle.getPosition().y + 80 > ySize || rectangle.getPosition().y < 0)
                changeDirection(ySpeed, rectangle);

            move(rectangle, xSize, ySize, xSpeed, ySpeed);
            clock.restart();
        }

        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonReleased:
                currentVelocity = ++currentVelocity % 10;
                changeVelocity(xSpeed, velocities, currentVelocity);
                changeVelocity(ySpeed, velocities, currentVelocity);
            }
        }

        text.setString(std::to_string(currentVelocity + 1));
        
        window.clear(sf::Color(50, 50, 50, 100));
        window.draw(rectangle);
        window.draw(text);
        window.display();
    }

    return 0;
}

void changeDirection(float& direction, sf::RectangleShape& rectangle)
{
    direction = -direction;

    // Changes the square's color every time it hits the wall
    auto xColor = static_cast<int>(rectangle.getPosition().x) % 256;
    auto yColor = static_cast<int>(rectangle.getPosition().y) % 256;
    auto zColor = (xColor + yColor) % 256;

    rectangle.setFillColor(sf::Color(xColor, yColor, zColor, 255));
}

void changeVelocity(float& velocity, float* options, int newV)
{
    // Keeps the direction
    if (velocity < 0)
        velocity = -options[newV];
    else
        velocity = options[newV];
}

void setText(sf::Text& text, sf::Font& font)
{
    font.loadFromFile("Fonts/monofont.otf");
    text.setFont(font);
    text.setLetterSpacing(1.5);
    text.setFillColor(sf::Color::White);
    text.setLineSpacing(2.0);
    text.setCharacterSize(50);
    text.setPosition(30, 30);
}

void move(sf::RectangleShape& rectangle, int x, int y, float moveX, float moveY)
{
    auto positionX = rectangle.getPosition().x;
    auto positionY = rectangle.getPosition().y;

    // Numbers adjusted because the position of the square is only one point, at the top-left corner
    // Also, because the top bar takes space
    if (positionX + 50 > x)
        positionX = x -50.f;
    else if (positionX < 0)
        positionX = 0;

    if (positionY + 80 > y)
        positionY = y - 80.f;
    else if (positionX < 0)
        positionY = 0;
    
    rectangle.move(moveX, moveY);
}