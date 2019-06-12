#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

//Global variables
static sf::Vector2f WindowSize;
static std::vector<sf::Vector2f> ArrayOfPoints;

class Shot : public sf::CircleShape{
    //On lab :D
};

class Tank : public sf::RectangleShape{
    float Speed;
    std::string Side;

    int IndexLeft,IndexCenter,IndexRight;

    float EquationA,EquationB,EquationC; //Line equation values
    float Angle;
public:
    Tank(sf::Vector2f Size, float NewSpeed, std::string NewSide) : sf::RectangleShape(Size){
        Speed = NewSpeed;
        Side = NewSide;

        setOrigin(getLocalBounds().width / 2 , getLocalBounds().height / 2);

        if(Side == "Left"){
            IndexCenter = 2;
            IndexLeft = IndexCenter - 1;
            IndexRight = IndexCenter + 1;
            setPosition(ArrayOfPoints[IndexCenter].x,0);
        }else if(Side == "Right"){
            IndexCenter = ArrayOfPoints.size() - 5;
            IndexLeft = IndexCenter + 1;
            IndexRight = IndexCenter - 1;
            setPosition(ArrayOfPoints[IndexCenter].x,0);
        }
    }


    void FindClosestPointsIndexes(std::string Direction){
        float CurrentDistance,SmallestDistane = WindowSize.x + 200;
        IndexCenter = 0; //Make sure that there is no center point

        if(Direction == "Left"){
            for(int i = 0 ; i < ArrayOfPoints.size() ; i++){
                CurrentDistance = getPosition().x - ArrayOfPoints[i].x;
                if(CurrentDistance < 0){
                    break;
                }else{
                    if(CurrentDistance == 0){
                        IndexCenter = i;
                    }else if(CurrentDistance < SmallestDistane){
                        SmallestDistane = CurrentDistance;
                        IndexLeft = i;
                    }
                }
            }
        }else if(Direction == "Right"){
            for(int i = ArrayOfPoints.size() - 1 ; i > 0 ; i--){
                CurrentDistance = ArrayOfPoints[i].x - getPosition().x;

                if(CurrentDistance < 0){
                    break;
                }else{
                    if(CurrentDistance == 0){
                        IndexCenter = i;
                    }else if(CurrentDistance < SmallestDistane){
                        SmallestDistane = CurrentDistance;
                        IndexRight = i;
                    }
                }
            }
        }
    }

    void CalculateLineEquation(std::string Direction){
        if(IndexCenter != 0){
            if(Direction == "Left"){
                EquationA = ArrayOfPoints[IndexCenter].y - ArrayOfPoints[IndexLeft].y;
                EquationB = ArrayOfPoints[IndexLeft].x - ArrayOfPoints[IndexCenter].x;
                EquationC = EquationA * ArrayOfPoints[IndexLeft].x + EquationB * ArrayOfPoints[IndexLeft].y;

                Angle = std::atan2(ArrayOfPoints[IndexLeft].y - ArrayOfPoints[IndexCenter].y, ArrayOfPoints[IndexLeft].x - ArrayOfPoints[IndexCenter].x);
            }else if(Direction == "Right"){
                EquationA = ArrayOfPoints[IndexRight].y - ArrayOfPoints[IndexCenter].y;
                EquationB = ArrayOfPoints[IndexCenter].x - ArrayOfPoints[IndexRight].x;
                EquationC = EquationA * ArrayOfPoints[IndexCenter].x + EquationB * ArrayOfPoints[IndexCenter].y;

                Angle = std::atan2(ArrayOfPoints[IndexCenter].y - ArrayOfPoints[IndexRight].y, ArrayOfPoints[IndexCenter].x - ArrayOfPoints[IndexRight].x);
            }
        }else{
            if(Direction == "Left"){
                EquationA = ArrayOfPoints[IndexRight].y - ArrayOfPoints[IndexLeft].y;
                EquationB = ArrayOfPoints[IndexLeft].x - ArrayOfPoints[IndexRight].x;
                EquationC = EquationA * ArrayOfPoints[IndexLeft].x + EquationB * ArrayOfPoints[IndexLeft].y;

                Angle = std::atan2(ArrayOfPoints[IndexLeft].y - ArrayOfPoints[IndexRight].y, ArrayOfPoints[IndexLeft].x - ArrayOfPoints[IndexRight].x);
            }else if(Direction == "Right"){
                EquationA = ArrayOfPoints[IndexLeft].y - ArrayOfPoints[IndexRight].y;
                EquationB = ArrayOfPoints[IndexRight].x - ArrayOfPoints[IndexLeft].x;
                EquationC = EquationA * ArrayOfPoints[IndexRight].x + EquationB * ArrayOfPoints[IndexRight].y;

                Angle = std::atan2(ArrayOfPoints[IndexRight].y - ArrayOfPoints[IndexLeft].y, ArrayOfPoints[IndexRight].x - ArrayOfPoints[IndexLeft].x);
            }
        }

        std::cout << EquationA << "x + " << EquationB << "y = " << EquationC << std::endl;
    }

    void GetNewPosition(std::string Direction, sf::Time ElapsedTime){
        if(Direction == "Left"){
            FindClosestPointsIndexes("Right");
        }else if(Direction == "Right"){
            FindClosestPointsIndexes("Left");
        }
        FindClosestPointsIndexes(Direction);

        std::cout << IndexLeft << ":" << IndexCenter << ":" << IndexRight <<std::endl;
        CalculateLineEquation(Direction);

        float TempX,TempY;

        if(Direction == "Left"){
            TempX = getPosition().x - (Speed * ElapsedTime.asSeconds());
        }else if(Direction == "Right"){
            TempX = getPosition().x + (Speed * ElapsedTime.asSeconds());
        }else{
            TempX = getPosition().x;
        }

        TempY = ((EquationC - (TempX * EquationA)) / EquationB);

        if(TempX > (getLocalBounds().height / 2) && TempX < (WindowSize.x - getLocalBounds().height / 2)){
            setPosition(sf::Vector2f(TempX,TempY - getLocalBounds().height / 2));
            setRotation(Angle * 180 / 3.14f);
        }
    }

    void Step(sf::Time ElapsedTime){
        if(Side == "Left"){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                GetNewPosition("Left",ElapsedTime);
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                GetNewPosition("Right",ElapsedTime);
            }
        }else if(Side == "Right"){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                GetNewPosition("Left",ElapsedTime);
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                GetNewPosition("Right",ElapsedTime);
            }
        }
    }
};

void GenerateMap(sf::ConvexShape &Polygon,int HorizontalOffset, int VerticalOffset){
    int PointCount = static_cast<int>((WindowSize.x / HorizontalOffset) + 2);
    int Reminder = static_cast<int>(WindowSize.x) % HorizontalOffset;
    if(Reminder != 0){
        PointCount += 1;
    }
    Polygon.setPointCount(static_cast<size_t>(PointCount + 1));

    sf::Vector2f CurrentPointPosition = sf::Vector2f(0,WindowSize.y / 2);
    sf::Vector2f LastPointPosition;

    for(int i = 0 ; i <= PointCount ; i++){
        if(i == 0){
            Polygon.setPoint(static_cast<size_t>(i),CurrentPointPosition);
        }
        else if(i == PointCount - 1){
            Polygon.setPoint(static_cast<size_t>(i),sf::Vector2f(WindowSize.x,WindowSize.y));
        }else if(i == PointCount){
            Polygon.setPoint(static_cast<size_t>(i),sf::Vector2f(0,WindowSize.y));
        }else{
            if((i == PointCount - 2) && Reminder != 0){
                CurrentPointPosition.x = LastPointPosition.x + Reminder;
            }else{
                CurrentPointPosition.x = LastPointPosition.x + HorizontalOffset;
            }
            if(rand()%2 > 0){
                do{
                    CurrentPointPosition.y = LastPointPosition.y - rand()%VerticalOffset;
                }while(CurrentPointPosition.y >= 600 || CurrentPointPosition.y <= 200);
            }else{
                do{
                    CurrentPointPosition.y = LastPointPosition.y + rand()%VerticalOffset;
                }while(CurrentPointPosition.y >= 600 || CurrentPointPosition.y <= 200);
            }
            Polygon.setPoint(static_cast<size_t>(i),CurrentPointPosition);
        }
        LastPointPosition = CurrentPointPosition;
        ArrayOfPoints.push_back(CurrentPointPosition);
    }
    Polygon.setFillColor(sf::Color(static_cast<unsigned char>(rand()%256),static_cast<unsigned char>(rand()%256),static_cast<unsigned char>(rand()%256)));
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    WindowSize = sf::Vector2f(800,600);

    sf::RenderWindow Window(sf::VideoMode(static_cast <unsigned int>(WindowSize.x), static_cast <unsigned int>(WindowSize.y)), "My window");
    Window.setFramerateLimit(60);
    Window.setVerticalSyncEnabled(true);

    sf::Event Event;
    sf::Clock GameClock;

    sf::ConvexShape Polygon;
    GenerateMap(Polygon,25,50);

    Tank Player1(sf::Vector2f(20,15),50, "Left");
    Tank Player2(sf::Vector2f(20,15),50, "Right");

    while (Window.isOpen()) {
        while (Window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed){
                Window.close();
            }
        }

        Window.clear(sf::Color::Black);
        Window.draw(Polygon);
        Window.draw(Player1);
        Window.draw(Player2);

        Player1.Step(GameClock.getElapsedTime());
        Player2.Step(GameClock.getElapsedTime());

        GameClock.restart();
        Window.display();
    }
    return 0;
}
