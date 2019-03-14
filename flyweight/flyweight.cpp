/*
    Flyweight Pattern

    Reuse existing instances of objects via a proxy container.
        - arush15june 2019/02/22
*/
#include<iostream>
#include<string>
#include<map>

#define key_type Side
#define val_type std::string

enum Side {
    NORTH_WEST,
    NORTH,
    NORTH_EAST,
    WEST,
    CENTER,
    EAST,
    SOUTH_WEST,
    SOUTH,
    SOUTH_EAST,
};

class Element {
    /*
        Stateless element containing all representations
        of the element.

        The representations are mapped as an std::map
            key_type:val_type
        Defaults:
            key_type = Side
            val_type = std::string
    */
protected:
    std::map<key_type, val_type> representations;
public:
    Element(std::map<key_type, val_type> reprs) {
        /*
            Set the representations via the constructor.
        */
        representations = reprs;
    }

    std::string repr(key_type key) {
        /*
            Return the representation based on the key
        */
        return representations[key];
    }
};

// NULL element
std::map<Side, std::string> NULL_REPRESENTATIONS = {
    { NORTH, "" },
    { NORTH_EAST, "" },
    { EAST, "" },
    { SOUTH_EAST, "" },
    { SOUTH, ""},
    { SOUTH_WEST, "" },
    { WEST, ""},
    { NORTH_WEST, "" },
};

Element* NULL_ELEMENT = new Element(NULL_REPRESENTATIONS);

// WALL Element
std::map<Side, std::string> WALL_REPRESENTATIONS = {
    { NORTH, "■" },
    { NORTH_EAST, "■" },
    { EAST, "■" },
    { SOUTH_EAST, "■" },
    { SOUTH, "■" },
    { SOUTH_WEST, "■" },
    { WEST, "■" },
    { NORTH_WEST, "■" },
    { CENTER, " " },
};

Element* WALL_ELEMENT = new Element(WALL_REPRESENTATIONS);

class ElementContainer {
    /*
        Stateful container to act as proxy for existing stateless elements.
        A pointer to an existing element is stored along with its state (i.e the Side to be represented).

        It powers the flyweight pattern by becoming the interface for accessing Elements,
        Subclassing ElementContainer for various elements allows easily build stateful elements
        without wasting memory on same representations for each instance of the Element.

        TODO: more elegant method to subclass for default elements.
    */
protected:
    Side state;
    Element* el_proxy;
public:
    ElementContainer(Side side, Element* el) {
        setSide(side);
        setElement(el);
    }

    Side getSide() {
        /*
            Return the current side (the current state of the element as known by the container)
        */
        return state;
    }

    void setSide(Side side) {
        /*
            Set the side for the container.
        */
        state = side;
    }

    void setElement(Element* el) {
        /*
            Set the element represented by the container.
        */
        el_proxy = el;
    }

    std::string repr() {
        /*
            Return the representation held by the container for the element.
        */
        return el_proxy->repr(state);
    }
  
};

class NullContainer : public ElementContainer {
    /* 
        Container for WALL_ELEMENT
    */
public:
    NullContainer(Side side, Element* el = NULL_ELEMENT) : ElementContainer(side, el) {}    
};

class WallContainer : public ElementContainer {
    /* 
        Container for WALL_ELEMENT
    */
public:
    WallContainer(Side side, Element* el = WALL_ELEMENT) : ElementContainer(side, el) {}
};

int main(int argc, char** argv) {
    std::cout<<"Accessing NORTH representations of WALL_ELEMENT."<<std::endl;
    std::cout<<WALL_ELEMENT->repr(NORTH)<<std::endl;
    std::cout<<std::endl;
    
    std::cout<<"Accessing all representations of WALL_ELEMENT."<<std::endl;
    for(auto i = 0; i < 3; i++) {
        for(auto j = 0; j < 3; j++) {
            Side curr_side = static_cast<Side>(i*3+j);
            std::cout<<WALL_ELEMENT->repr(curr_side)<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;

    WallContainer* wall = new WallContainer(NORTH);
    std::cout<<"Accessing NORTH representation of wall via WallContainer"<<std::endl;
    std::cout<<wall->repr()<<std::endl;

    WallContainer* wall_north_west = new WallContainer(NORTH_WEST);
    WallContainer* wall_north = new WallContainer(NORTH);
    WallContainer* wall_north_east = new WallContainer(NORTH_EAST);
    WallContainer* wall_west = new WallContainer(WEST);
    WallContainer* wall_center = new WallContainer(CENTER);
    WallContainer* wall_east = new WallContainer(EAST);
    WallContainer* wall_south_west = new WallContainer(SOUTH_WEST);
    WallContainer* wall_south = new WallContainer(SOUTH);
    WallContainer* wall_south_east = new WallContainer(SOUTH_EAST);

    WallContainer[3][3] wall_matrix = {
        {wall_north_west, wall_north, wall_north_east},
        {wall_west, wall_center, wall_east},
        {wall_south_west, wall_south, wall_south_east}
    };

    std::cout<<"Creating matrix using WallContainer's"<<std::endl;
    for(auto &row: wall_matrix) {
        for(auto &item: row) {
            std::cout<<item->repr()<<" ";
        }
        std::cout<<std::endl;
    }
    
}