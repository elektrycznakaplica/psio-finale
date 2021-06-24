#include <game_world_classes.h>

//--class Tile-------------------------------------------
Tile::Tile()
{}
Tile::Tile(float pos_x,float pos_y,int type)
{
    this->type=type;
    if(type==1){
        sf::Texture *txt_wall;
        txt_wall = new sf::Texture;
        if(!(txt_wall->loadFromFile("wall.png"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        txt_wall->setRepeated(true);
        tile.setTexture(*txt_wall);
        tile.setOrigin(20.0,20.0);
        tile.setPosition(pos_x,pos_y);
        position = tile.getPosition();
    }
    if(type==0){
        sf::Texture *txt_floor;
        txt_floor = new sf::Texture;
        if(!(txt_floor->loadFromFile("floor.jpg"))){
            std::cerr<<"could not load texture"<<std::endl;
        }
        txt_floor->setRepeated(true);
        tile.setTexture(*txt_floor);
        tile.setOrigin(20.0,20.0);
        tile.setPosition(pos_x,pos_y);
        position = tile.getPosition();
    }
}

sf::Sprite Tile::getSprite()
{
    return this->tile;
}

int Tile::getType()
{
    return this->type;
}

void Tile::setNode(float g, float h)
{
    this->g=g;
    this->f=g+h;
}

void Tile::setParent(Tile* parent_pos)
{
        this->parent_pos = parent_pos;
}

Tile* Tile::getParent()
{
    return parent_pos;
}

float calculate_distance(Tile start,Tile end)
{
    float dx = abs(start.getSprite().getPosition().x-end.getSprite().getPosition().x);
    float dy = abs(start.getSprite().getPosition().y-end.getSprite().getPosition().y);
    float h = dx+dy;
    return h;
}

float calculate_gval(Tile start, Tile current)
{
    float g = start.g + calculate_distance(start,current);
    return g;
}

float Tile::getF()
{
    return this->f;
}

float Tile::getG()
{
    return this->g;
}

void Tile::setVisited(bool state)
{
    this->visited = state;
}

bool Tile::wasVisited()
{
    return this->visited;
}




//------------------clas Map-----------------------------------------
Map::Map(std::vector<std::vector<int>> maze)
{
    for(int i=0;i<16;i++){
        std::vector<Tile> row;
        for(int j=0;j<25;j++){
            Tile x(j*40.0+20.0,i*40.0+20.0,maze[i][j]);
            row.emplace_back(x);
            }
        map.emplace_back(row);
        }
    this->convert_to_edges();
}

Map::Map()
{}

void Map::draw_map(sf::RenderWindow &window)
{
    for(auto &x : this->map){
        for(auto &y : x){
          window.draw(y.getSprite());
        }
    }
}

std::vector<sf::Sprite> Map::getWalls()
{
    std::vector<sf::Sprite> walls;
    for(auto &x : this->map){
        for(auto &y : x){
            if(y.getType()==1)
                walls.emplace_back(y.getSprite());
        }
    }
    return walls;
}

sf::Vector2f Map::getTile(sf::Vector2f pos)
{
    sf::Vector2f current={0,0};
    for(int i=0;i<16;i++){
        for(int j=0;j<25;j++){
            sf::FloatRect tile = map[i][j].getSprite().getGlobalBounds();
            if(tile.contains(pos.x,pos.y)){
                current.x = i;
                current.y = j;
            }
        }
    }
    return current;
}

std::vector<std::vector<Tile>> Map::getMap()
{
    return this->map;
}

std::vector<Tile*> Map::getSuccesors(Tile parent)
{
    sf::Vector2f parent_cords = getTile(parent.getSprite().getPosition());
    std::vector<Tile*> succesors;
    Tile* a = &map[parent_cords.x][parent_cords.y+1];
    succesors.emplace_back(a);
    Tile* a1 = &map[parent_cords.x+1][parent_cords.y];
    succesors.emplace_back(a1);
    Tile* a2 = &map[parent_cords.x][parent_cords.y-1];
    succesors.emplace_back(a2);
    Tile* a3 = &map[parent_cords.x-1][parent_cords.y];
    succesors.emplace_back(a3);
//    Tile* a4 = &map[parent_cords.x-1][parent_cords.y];
//    if(a4->getType()==0)
//        succesors.emplace_back(a4);
//    Tile* a5 = &map[parent_cords.x+1][parent_cords.y-1];
//    if(a5->getType()==0)
//        succesors.emplace_back(a5);
//    Tile* a6 = &map[parent_cords.x+1][parent_cords.y];
//    if(a6->getType()==0)
//        succesors.emplace_back(a6);
//    Tile* a7 = &map[parent_cords.x+1][parent_cords.y+1];
//    if(a7->getType()==0)
//        succesors.emplace_back(a7);

    return succesors;
}

void Map::convert_to_edges()
{
    for(int y=0;y<16;y++){
        for(int x=0;x<25;x++){

            //jeżeli płytka jest ścianą
            if(map[y][x].getType()==1){

                if(y==0){
                    if(map[y+1][x].getType()==0){

                        //jezeli lewy sasiad ma dolna krawedz to ja przedluz
                        if(map[y][x-1].edge_exist[Down]==1){
                            edges[map[y][x-1].edge_id[Down]].ex += 40.0;
                            map[y][x].edge_id[Down]=map[y][x-1].edge_id[Down];
                            map[y][x].edge_exist[Down]=true;
                        }
                        else{
                            Edge ed;
                            sf::Vector2f pos = map[y][x].getSprite().getPosition();
                            ed.sx = pos.x - 20.0; ed.sy = pos.y + 20.0;
                            ed.ex = pos.x + 20.0; ed.ey = pos.y + 20.0;

                            int ed_id = edges.size();
                            edges.emplace_back(ed);

                            map[y][x].edge_id[Down] = ed_id;
                            map[y][x].edge_exist[Down] = true;
                        }

                    }
                }//koniec
                else if(y==15){

                    if(map[y-1][x].getType()==0){

                        //jezeli lewy sasiad ma gorna krawedz to ja przedluz
                        if(map[y][x-1].edge_exist[Up]==1){
                            edges[map[y][x-1].edge_id[Up]].ex += 40.0;
                            map[y][x].edge_id[Up]=map[y][x-1].edge_id[Up];
                            map[y][x].edge_exist[Up]=true;
                        }
                        else{
                            Edge ed;
                            sf::Vector2f pos = map[y][x].getSprite().getPosition();
                            ed.sx = pos.x - 20.0; ed.sy = pos.y - 20.0;
                            ed.ex = pos.x + 20.0; ed.ey = pos.y - 20.0;

                            int ed_id = edges.size();
                            edges.emplace_back(ed);

                            map[y][x].edge_id[Up] = ed_id;
                            map[y][x].edge_exist[Up] = true;
                        }

                    }

                }//koniec
                else if(x==0 && y!=0 && y!=15){

                    if(map[y][x+1].getType()==0){

                        //jezeli gorny sasiad ma prawa krawedz to ja przedluz
                        if(map[y-1][x].edge_exist[Right]==1){
                            edges[map[y-1][x].edge_id[Right]].ey += 40.0;
                            map[y][x].edge_id[Right]=map[y-1][x].edge_id[Right];
                            map[y][x].edge_exist[Right]=true;
                        }
                        else{
                            Edge ed;
                            sf::Vector2f pos = map[y][x].getSprite().getPosition();
                            ed.sx = pos.x + 20.0; ed.sy = pos.y - 20.0;
                            ed.ex = pos.x + 20.0; ed.ey = pos.y + 20.0;

                            int ed_id = edges.size();
                            edges.emplace_back(ed);

                            map[y][x].edge_id[Right] = ed_id;
                            map[y][x].edge_exist[Right] = true;
                        }

                    }
                }//koniec
                else if(x==24 && y!=0 && y!=15){

                    if(map[y][x-1].getType()==0){

                        //jezeli gorny sasiad ma lewa krawedz to ja przedluz
                        if(map[y-1][x].edge_exist[Left]==1){
                            edges[map[y-1][x].edge_id[Left]].ey += 40.0;
                            map[y][x].edge_id[Left]=map[y-1][x].edge_id[Left];
                            map[y][x].edge_exist[Left]=true;
                        }
                        else{
                            Edge ed;
                            sf::Vector2f pos = map[y][x].getSprite().getPosition();
                            ed.sx = pos.x - 20.0; ed.sy = pos.y - 20.0;
                            ed.ex = pos.x - 20.0; ed.ey = pos.y + 20.0;

                            int ed_id = edges.size();
                            edges.emplace_back(ed);

                            map[y][x].edge_id[Left] = ed_id;
                            map[y][x].edge_exist[Left] = true;
                        }

                    }
                }//koniec
                else {


                //jezeli lewy sasiad nie istnieje
                if(map[y][x-1].getType()==0){

                    //jezeli gorny sasiad ma lewa krawedz to ja przedluz
                    if(map[y-1][x].edge_exist[Left]==1){
                        edges[map[y-1][x].edge_id[Left]].ey += 40.0;
                        map[y][x].edge_id[Left]=map[y-1][x].edge_id[Left];
                        map[y][x].edge_exist[Left]=true;
                    }
                    else{
                        Edge ed;
                        sf::Vector2f pos = map[y][x].getSprite().getPosition();
                        ed.sx = pos.x - 20.0; ed.sy = pos.y - 20.0;
                        ed.ex = pos.x - 20.0; ed.ey = pos.y + 20.0;

                        int ed_id = edges.size();
                        edges.emplace_back(ed);

                        map[y][x].edge_id[Left] = ed_id;
                        map[y][x].edge_exist[Left] = true;
                    }

                }

                //jezeli prawy sasiad nie istnieje
                if(map[y][x+1].getType()==0){

                    //jezeli gorny sasiad ma prawa krawedz to ja przedluz
                    if(map[y-1][x].edge_exist[Right]==1){
                        edges[map[y-1][x].edge_id[Right]].ey += 40.0;
                        map[y][x].edge_id[Right]=map[y-1][x].edge_id[Right];
                        map[y][x].edge_exist[Right]=true;
                    }
                    else{
                        Edge ed;
                        sf::Vector2f pos = map[y][x].getSprite().getPosition();
                        ed.sx = pos.x + 20.0; ed.sy = pos.y - 20.0;
                        ed.ex = pos.x + 20.0; ed.ey = pos.y + 20.0;

                        int ed_id = edges.size();
                        edges.emplace_back(ed);

                        map[y][x].edge_id[Right] = ed_id;
                        map[y][x].edge_exist[Right] = true;
                    }

                }

                //jezeli gorny sasiad nie istnieje
                if(map[y-1][x].getType()==0){

                    //jezeli lewy sasiad ma gorna krawedz to ja przedluz
                    if(map[y][x-1].edge_exist[Up]==1){
                        edges[map[y][x-1].edge_id[Up]].ex += 40.0;
                        map[y][x].edge_id[Up]=map[y][x-1].edge_id[Up];
                        map[y][x].edge_exist[Up]=true;
                    }
                    else{
                        Edge ed;
                        sf::Vector2f pos = map[y][x].getSprite().getPosition();
                        ed.sx = pos.x - 20.0; ed.sy = pos.y - 20.0;
                        ed.ex = pos.x + 20.0; ed.ey = pos.y - 20.0;

                        int ed_id = edges.size();
                        edges.emplace_back(ed);

                        map[y][x].edge_id[Up] = ed_id;
                        map[y][x].edge_exist[Up] = true;
                    }

                }

                //jezeli dolny sasiad nie istnieje
                if(map[y+1][x].getType()==0){

                    //jezeli lewy sasiad ma dolna krawedz to ja przedluz
                    if(map[y][x-1].edge_exist[Down]==1){
                        edges[map[y][x-1].edge_id[Down]].ex += 40.0;
                        map[y][x].edge_id[Down]=map[y][x-1].edge_id[Down];
                        map[y][x].edge_exist[Down]=true;
                    }
                    else{
                        Edge ed;
                        sf::Vector2f pos = map[y][x].getSprite().getPosition();
                        ed.sx = pos.x - 20.0; ed.sy = pos.y + 20.0;
                        ed.ex = pos.x + 20.0; ed.ey = pos.y + 20.0;

                        int ed_id = edges.size();
                        edges.emplace_back(ed);

                        map[y][x].edge_id[Down] = ed_id;
                        map[y][x].edge_exist[Down] = true;
                    }

                }
               }

            }

        }
    }
}

std::vector<Edge> Map::getEdges()
{
    return this->edges;
}

//-----------------------------class Colectible------------------------------------------------
Colectible::Colectible(float pos_x,float pos_y,int typ) : type(typ)
    {
        if(type==0){
            sf::Texture *txt_collectible;
            txt_collectible = new sf::Texture;
            if(!(txt_collectible->loadFromFile("collectible.png"))){
                std::cerr<<"could not load texture"<<std::endl;
            }
            txt_collectible->setRepeated(true);
            colectible.setPosition(pos_x,pos_y);
            colectible.setTexture(*txt_collectible);
            colectible.scale(0.8,0.8);

            sf::SoundBuffer *sound;
            sound = new sf::SoundBuffer;
            if(!sound->loadFromFile("collect_heart.wav")){
                std::cout<<"could not load sound";
            }
            this->pick_sound.setBuffer(*sound);
        }
        if(type==1){
            sf::Texture *txt_collectible;
            txt_collectible = new sf::Texture;
            if(!(txt_collectible->loadFromFile("key.png"))){
                std::cerr<<"could not load texture"<<std::endl;
            }
            txt_collectible->setRepeated(true);
            colectible.setPosition(pos_x,pos_y);
            colectible.setTexture(*txt_collectible);
            colectible.scale(0.8,0.8);

            sf::SoundBuffer *sound;
            sound = new sf::SoundBuffer;
            if(!sound->loadFromFile("collect_key.wav")){
                std::cout<<"could not load sound";
            }
            this->pick_sound.setBuffer(*sound);
        }
        if(type==2){
            sf::Texture *txt_collectible;
            txt_collectible = new sf::Texture;
            if(!(txt_collectible->loadFromFile("door.png"))){
                std::cerr<<"could not load texture"<<std::endl;
            }
            txt_collectible->setRepeated(true);
            colectible.setPosition(pos_x,pos_y);
            colectible.setTexture(*txt_collectible);

            sf::SoundBuffer *sound;
            sound = new sf::SoundBuffer;
            if(!sound->loadFromFile("open_door.wav")){
                std::cout<<"could not load sound";
            }
            this->pick_sound.setBuffer(*sound);
        }

    }
bool Colectible::isPicked()
    {
        return this->picked;
    }

void Colectible::pick()
    {
        this->picked=true;
        play_picksound();
    }

sf::Sprite Colectible::getObject()
    {
        return this->colectible;
    }

int Colectible::getType()
{
    return this->type;
}

void Colectible::play_picksound()
{
    this->pick_sound.play();
}
