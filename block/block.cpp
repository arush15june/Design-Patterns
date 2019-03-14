/*
    Maze Generator
    Inspired from the Maze example in the creational patterns
    from the Design Patterns (Gang Of Four).

        - arush15june

    TODO:
        Use std::shared_ptr or std::unique_ptr.
*/
#include<iostream>
#include<string>
#include<map>
#include<vector>

/* Helpers */

#define BLOCK_SIZE 3
#define key_type Side
#define val_type std::string

enum class Side {
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

std::map<Side, std::array<int, 2>> side_map = {
    {Side::NORTH, {0, 1} },
    {Side::NORTH_EAST, {0, 2} },
    {Side::EAST, {1, 2} },
    {Side::CENTER, {1, 1} },
    {Side::SOUTH_EAST, {2, 2} },
    {Side::SOUTH, {2, 1} },
    {Side::SOUTH_WEST, {2, 0} },
    {Side::WEST, {1, 0} },
    {Side::NORTH_WEST, {0, 0} },
};

std::map<std::array<int, 2>, Side> reverse_side_map = {
    {{0, 1}, Side::NORTH},
    {{0, 2}, Side::NORTH_EAST},
    {{1, 1}, Side::CENTER},
    {{1, 2}, Side::EAST},
    {{2, 2}, Side::SOUTH_EAST},
    {{2, 1}, Side::SOUTH},
    {{2, 0}, Side::SOUTH_WEST},
    {{1, 0}, Side::WEST},
    {{0, 0}, Side::NORTH_WEST},
};

Side index_to_side(int x, int y) {
    std::array<int, 2> query_pos = {x, y};
    return reverse_side_map[query_pos];
}

std::array<int, 2> side_to_index(Side s) {
    return side_map[s];
}

/* Element and ElementContainer Flyweights */

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
    { Side::NORTH, "" },
    { Side::NORTH_EAST, "" },
    { Side::EAST, "" },
    { Side::SOUTH_EAST, "" },
    { Side::SOUTH, ""},
    { Side::SOUTH_WEST, "" },
    { Side::WEST, ""},
    { Side::NORTH_WEST, "" },
};

Element* NULL_ELEMENT = new Element(NULL_REPRESENTATIONS);

// WALL Element
std::map<Side, std::string> WALL_REPRESENTATIONS = {
    { Side::NORTH, "■" },
    { Side::NORTH_EAST, "■" },
    { Side::EAST, "■" },
    { Side::SOUTH_EAST, "■" },
    { Side::SOUTH, "■" },
    { Side::SOUTH_WEST, "■" },
    { Side::WEST, "■" },
    { Side::NORTH_WEST, "■" },
    { Side::CENTER, " " },
};

Element* WALL_ELEMENT = new Element(WALL_REPRESENTATIONS);

// FLOOR Element
std::map<Side, std::string> FLOOR_REPRESENTATIONS = {
    { Side::NORTH, " " },
    { Side::NORTH_EAST, " " },
    { Side::EAST, " " },
    { Side::SOUTH_EAST, " " },
    { Side::SOUTH, " " },
    { Side::SOUTH_WEST, " " },
    { Side::WEST, " " },
    { Side::NORTH_WEST, " " },
    { Side::CENTER, " " },
};

Element* FLOOR_ELEMENT = new Element(FLOOR_REPRESENTATIONS);

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
        Container for NULL_ELEMENT
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

class FloorContainer : public ElementContainer {
    /* 
        Container for WALL_ELEMENT
    */
public:
    FloorContainer(Side side, Element* el = FLOOR_ELEMENT) : ElementContainer(side, el) {}
};


class Block {
    /* 
        Store ElementContainers in a 3x3 (BLOCK_SIZE x BLOCK_SIZE) Grid.
    */
protected:
    ElementContainer* grid[3][3];
    
public:
    /* 
        Constructors
        Block()
            Initialize a Block with all NullContainers
        
        Block(ElementContainer* el) #TODO
            Set all the elements with `el`.

        Block(ElementContainer[3][3] el) #TODO
            Set the grid equivalent to the passed grid.
    */
    Block() {
        /* Fill grid with NullContainers */

        for(auto i = 0; i < BLOCK_SIZE; i++) {
            for(auto j = 0; j < BLOCK_SIZE; j++) {
                Side mapped_side = index_to_side(i, j); 
                grid[i][j] = new NullContainer(mapped_side);
            }
        }
    }

    // Block(ElementContainer* [BLOCK_SIZE][BLOCK_SIZE] el_list) {
    //     /*
    //         Assign el_list containers to grid

    //         iterate over the grid as a 2D matrix,
    //         get the current mapped_side to the grid(i, j),
    //         get the curr_container at el_list(i, j),
    //         set grid(i, j) to curr_container.
    //     */

    //     for(auto i = 0; i < BLOCK_SIZE; i++) {
    //         for(auto j = 0; j < BLOCK_SIZE; j++) {
    //             Side mapped_side = index_to_side(i, j);
    //             auto curr_container = el_list[i][j];
    //             curr_container->setSide(mapped_side)
    //             grid[i][j] = curr_container;
    //         }
    //     }
    // }

    ElementContainer* getElement(int x, int y) {
        return grid[x][y];
    }

    void setContainer(ElementContainer* el) {
        /* 
            get index via container's stored side and reverse_side_map,
            get current container at index,
            delete it,
            assign new container.
        */
        auto el_side = el->getSide();
        std::array<int, 2> mapped_index = side_to_index(el_side);
        
        int x = mapped_index[0];
        int y = mapped_index[1];
        
        auto curr_el = grid[x][y];
        delete curr_el;
        
        grid[x][y] = el;
    }
        
    void render() {
        /* Render the Block to stdout */
        for(auto &row: grid) {
            for(auto &item: row) {
                std::cout<<item->repr()<<" ";
            }
            std::cout<<std::endl;
        }
    }
        
    // Store position of the Block on the Maze
    int pos_x;
    int pos_y;

    void setPosition(int x, int y) {
        pos_x = x;
        pos_y = y;
    }

    std::array<int, 2> getPosition() {
        return { pos_x, pos_y };
    }
};

int main(int argc, char** argv) {
    /*
        Create a new block,
        it will contain NullContainer's,
        Fill the corners with WallContainers
    */
    
    Block* new_block = new Block();

    new_block->setContainer(new WallContainer(Side::NORTH_WEST));
    new_block->setContainer(new WallContainer(Side::NORTH));
    new_block->setContainer(new WallContainer(Side::NORTH_EAST));
    new_block->setContainer(new WallContainer(Side::WEST));
    new_block->setContainer(new FloorContainer(Side::CENTER));
    new_block->setContainer(new WallContainer(Side::EAST));
    new_block->setContainer(new WallContainer(Side::SOUTH_WEST));
    new_block->setContainer(new WallContainer(Side::SOUTH));
    new_block->setContainer(new WallContainer(Side::SOUTH_EAST));

    new_block->render();
}