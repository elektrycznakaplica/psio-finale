#ifndef GAME_WORLD_CLASSES_H
#define GAME_WORLD_CLASSES_H
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include<math.h>
#include <algorithm>
#include <list>
#include <tuple>

//Map classes-------------------------------------------------------------------------
class Tile
{
private:
    sf::Sprite tile;
    sf::Vector2f position;
    int type;
    bool visited=false;
    float g=3000;
    float f=0;
    Tile* parent_pos;
public:
    int edge_id[4]={0,0,0,0};
    bool edge_exist[4]={0,0,0,0};
    Tile();
    Tile(float pos_x,float pos_y,int type);
    sf::Sprite getSprite();
    int getType();
    void setNode(float g,float h);
    void setParent(Tile* parent_pos);
    Tile* getParent();
    friend float calculate_gval(Tile start,Tile current);
    friend float calculate_distance(Tile start,Tile end);
    float getF();
    float getG();
    bool wasVisited();
    void setVisited(bool state);
};

class Edge
{
public:
    float sx,sy;
    float ex,ey;
};

#define Up 0
#define Down 1
#define Right 2
#define Left 3

class Map
{
private:
    std::vector<std::vector<Tile>> map;
    std::vector<Edge> edges;
public:
    Map(std::vector<std::vector<int>> maze);
    Map();
    void draw_map(sf::RenderWindow &window);
    std::vector<sf::Sprite> getWalls();
    sf::Vector2f getTile(sf::Vector2f pos);
    std::vector<Tile*> getSuccesors(Tile parent);
    std::vector<std::vector<Tile>> getMap();
    void convert_to_edges();
    std::vector<Edge> getEdges();

};

class Colectible
{
private:
    int type;
    sf::Sprite colectible;
    bool picked=false;
public:
    sf::Sound pick_sound;
    Colectible(float pos_x,float pos_y,int typ);
    Colectible();
    bool isPicked();
    void pick();
    void play_picksound();
    sf::Sprite getObject();
    int getType();
};


//Entity classes-------------------------------------------------------------------------------
class Entity
{
protected:
    sf::Sprite guy;
public:
    Entity(float pos_x, float pos_y);
    Entity();
    sf::Sprite getGuy();
};


class Character : public Entity
{
private:
    std::vector<sf::Sprite> walls;
    bool is_alive = true;
public:
    Character();
    Character(float pos_x, float pos_y,std::vector<sf::Sprite> shapes);
    void movement(const sf::Time elapsed, sf::RenderWindow &window);
    void rotate(sf::RenderWindow & window);
    void collect(std::vector<Colectible> &x);
    bool intersection();
    void contact(std::vector<sf::FloatRect> enemies);
    bool lives();
};

class Enemy : public Entity
{
private:
    Map map;
    int path_nr=0;
    bool can_move = true;
public:
    Enemy();
    Enemy(float pos_x, float pos_y,Map m);
    void is_in_circle(sf::CircleShape& circle);
    void is_in_convex(sf::ConvexShape& convex);
    void is_in_light(std::vector<sf::ConvexShape>& light);
    void movement(const sf::Time elapsed, sf::RenderWindow &window,Character target);
    void follow(sf::Time elapsed, std::vector<sf::Vector2f> path);
    void move(sf::Time elapsed,sf::Vector2f dest);
    std::vector<sf::Vector2f> find_path(sf::Vector2f t,Map m);
    void rotate(sf::RenderWindow & window,Character target);
    int get_path_nr();
    void reset_path_nr();
};

//Window  classes--------------------------------------------------------------------------------
class Button
{
private:
    sf::Text text;
    sf::Sound click_sound;
    bool clicked = false;
public:
    Button(double pos_x,double pos_y,unsigned int size, std::string txt,sf::Color color);
    void click(sf::Event);
    bool was_clicked();
    void draw(sf::RenderWindow* window);
    void update_text(std::string txt);
};

class Window
{
protected:
    sf::RenderWindow window;
    std::vector<Button> menu;
    sf::Sprite flashlight;
    sf::RenderTexture mask;
    sf::Sprite spr;
    sf::ConvexShape light;
public:
    Window();
    virtual int loop()=0;
    void menu_handling(bool& light_on);
};


class Menu : public Window
{
private:
    enum Option {Title,Play,Instructions,Exit};
public:
    Menu();
    int loop();
};

class Option : public Window
{
private:
    enum choice {Text,Back};
public:
    Option();
    int loop();
};

class Difficulty : public Window
{
private:
    enum difficulty {Text,Easy, Medium, Hard};
public:
    Difficulty();
    int loop();
};


//Scene classes-----------------------------------------------------------------
class Scene
{
private:
    sf::RenderWindow window;
    enum Mode {Easy=1,Medium,Hard};
    int diff_mode;
    Character hero;
    std::vector<Enemy> enemies;
    Map map;
    std::vector<Colectible> colectibles;
    std::vector<Button> hud;
    sf::Font font;
    int colectible_count = 0;
    std::vector<std::tuple<float,float,float>> fov;
    enum Pause_Mode {Pause,Win,Lose};
    bool State[3]={0,0,0};
public:
    Scene(int mode);
    int loop();
    std::vector<sf::ConvexShape> calculate_fov();
    void colectible_handling();
    //std::vector<sf::FloatRect> return_enemies_bounds();
};


#endif // GAME_WORLD_CLASSES_H
