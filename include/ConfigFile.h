/*!
    \file ConfigFile.h
    \brief Config file parsing & saving stuff
    
    @warning
    Be sure that your config file is in proper format, there are not many checks.
    
    @warning
    Although current implementation might process some paths with whitespace
    (eg: node.justanarray [5] . foo), the future ones won't !!

    @note
    The code is quite crude, so if you encounter any problem, let me know. (otpetrik)
    
    TODO: rewrite it to provide help for correcting input files
    (depends on: input caches providing current position (row & column) and free time)
    
    @note
    Example of use:
    
    @code
    // root node of config file tree
    ConfigFile::NodePtr root;
    
    if (data != NULL)
    {
        // load the config file from a string (use data.c_str() if you have real String)
        root = ConfigFile::loadFile((const char *)data);
    }
     
    //
    //    make sure that requred nodes exist
    //    - this is simple version, it would fail if the config file exists but
    //    it doesn't have the required nodes...
    //    
    //    note: this won't be necessary once config file is fully finished...
    //    (set & bind functions will create the necessary nodes)
    
    if (root == NULL)
    {
        root = ConfigFile::NodePtr(new ConfigFile::MapNode());
        ConfigFile::MapNodePtr mapNode;
        ConfigFile::MapNodeNodes *map;
        mapNode = ConfigFile::getMapNode("", root);
        
        map = mapNode->getNodes();
        map->insert(std::make_pair("graphics_settings", new ConfigFile::MapNode()));
        map->insert(std::make_pair("game_settings", new ConfigFile::MapNode()));

        mapNode = ConfigFile::getMapNode("graphics_settings", root);
        map = mapNode->getNodes();
    
        map->insert(std::make_pair("width", new ConfigFile::ValueNode("640")));
        map->insert(std::make_pair("height", new ConfigFile::ValueNode("480")));
        map->insert(std::make_pair("fullscreen", new ConfigFile::ValueNode("0")));

        mapNode = ConfigFile::getMapNode("game_settings", root);
        map = mapNode->getNodes();

        map->insert(std::make_pair("difficulty", new ConfigFile::ValueNode("Easy")));
        map->insert(std::make_pair("player_name", new ConfigFile::ValueNode("Player")));
    };
    
    // variables to use
    int screen_width, screen_height;
    int fullscreen;
    String playerName;
    String difficulty;

    // bind the variables to config file
    //
    //    or you can use get & set
    //
    ConfigFile::bind("graphics_settings.width", root, screen_width, 640);
    ConfigFile::bind("graphics_settings.height", root, screen_height, 480);
    ConfigFile::bind("graphics_settings.fullscreen", root, fullscreen, 0);

    ConfigFile::bind("game_settings.player_name", root, playerName, String("Player"));
    ConfigFile::bind("game_settings.difficulty", root, difficulty, String("Easy"));
    
    // do whatever you need with variables
    fullscreen = 1;
    screen_width = 1024;
    screen_height = 768;
    difficulty = "Hard";
 
    // save the config file to a string
    String output = ConfigFile::saveFile(root);
    
    @endcode
*/    

#ifndef DUNE_CONFIGFILE_H
#define DUNE_CONFIGFILE_H

#include "Strings.h"

#include <vector>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace ConfigFile
{

//------------------------------------------------------------------------------
// Binders
//------------------------------------------------------------------------------

//! Generic binder
/*!
    Common ancestor of different binders
*/
class GenericBinder
{
public:
    //! @name Constructors & Destructor
    //@{

    //! Constructor
    GenericBinder() { }
    
    //! Destructor
    virtual ~GenericBinder() { };
    
    //@}
    
    //! @name Access
    //@{

    //! Return value of target variable as string
    virtual String getString() = 0;
    
    //! Set value of target variable from given string
    virtual void setString(ConstString value) = 0;
    
    //@}
};

//! Binder specific to type T
/*!
    Allows binding of (almost) any variable.
    
    @note
    It doesnt work well with bool, don't know why :( (otpetrik)
    
    @see ValueNode
*/
template <typename T>
class Binder : public GenericBinder
{
public:
    //! @name Constructors & Destructor
    //@{

    //! Constructor
    /*!
        @param variable variable to bind to
    */
    Binder(T &variable) : variable(variable)
    {    
    };
    //@}
    
    virtual String getString()    
    {
        return toString<T>(variable);
    };
    virtual void setString(ConstString value)
    {
        variable = fromString<T>(value);
    };
protected:
    T &variable;
};

typedef boost::shared_ptr<GenericBinder> BinderPtr;

//------------------------------------------------------------------------------
// Nodes
//------------------------------------------------------------------------------

class Node;
class ArrayNode;
class MapNode;
class ValueNode;

typedef boost::shared_ptr<Node> NodePtr;
typedef boost::shared_ptr<ArrayNode> ArrayNodePtr;
typedef boost::shared_ptr<MapNode> MapNodePtr;
typedef boost::shared_ptr<ValueNode> ValueNodePtr;

typedef std::vector<NodePtr> ArrayNodeNodes;
typedef std::map<const String, NodePtr> MapNodeNodes;

//------------------------------------------------------------------------------
// Node class
//------------------------------------------------------------------------------
//! Generic node
/*!
    Common ancestor of all nodes.
*/
class Node : public boost::enable_shared_from_this<Node>
{
    public:
        //! @name Constructors & Destructor
        //@{
        
        //! Constructor
        Node();
        //! Destructor
        virtual ~Node();

        //@}
        
        //! @name Loading & Saving
        //@{
        
        //! Load contents of current node
        /*!
            @param cache cache to load from

            @note
            There is possibility, to call load more than once (for updates) !
        */
        virtual void load(StringInputCache &cache) = 0;
        //! Save contents of current node
        /*!
            @param cache cache to save to
        */
        virtual void save(StringOutputCache &cache) = 0;
        
        //@}
        
        //! @name Accessing child nodes
        //@{
        
        //! Save contents of current node
        /*!
            @param path input cache containing the path to the node (eg: something.another[2].thing)
        */
        virtual NodePtr getNode(StringInputCache &path) = 0;

        //@}        
    protected:
};

//------------------------------------------------------------------------------
// ArrayNode class
//------------------------------------------------------------------------------
//! Array node
/*!
    Contains child nodes indexed by an unsigned integer
*/
class ArrayNode : public Node
{
    public:
        virtual void load(StringInputCache &cache);
        virtual void save(StringOutputCache &cache);

        virtual NodePtr getNode(StringInputCache &path);
        
        //! Return underlying std::vector
        /*!
            @note
            Although it breaks encapsulation, it allows the most powerfull access
            to child nodes...
        */
        ArrayNodeNodes *getNodes() { return &nodes; };
    protected:
        ArrayNodeNodes nodes;
};

//------------------------------------------------------------------------------
// MapNode class
//------------------------------------------------------------------------------
//! Map node
/*!
    Contains child nodes indexed by a string
*/
class MapNode : public Node
{
    public:        
        virtual void load(StringInputCache &cache);
        virtual void save(StringOutputCache &cache);
        
        virtual NodePtr getNode(StringInputCache &path);

        //! Return underlying std::map
        /*!
            @note
            Although it breaks encapsulation, it allows the most powerfull access
            to child nodes...
        */
        MapNodeNodes *getNodes() { return &nodes; };
    protected:
        MapNodeNodes nodes;
        
        void loadChild(StringInputCache &cache);
        void loadChildValue(ConstString childName, StringInputCache &cache);
};

//------------------------------------------------------------------------------
// ValueNode class
//------------------------------------------------------------------------------
//! Value node
/*!
    Leaf node containg a string
*/
class ValueNode : public Node
{
    public:
        //! @name Constructors & Destructor
        //@{
        
        //! Constructor
        /*!
            @param value initial value of the node
        */
        ValueNode(ConstString value = "") : value(value), binder((GenericBinder *)NULL) { };

        //@}
        virtual void load(StringInputCache &cache);
        virtual void save(StringOutputCache &cache);
        
        virtual NodePtr getNode(StringInputCache &path);

        //! @name Value access
        //@{

        //! Return current value
        ConstString getValue();
        
        //! Set current string
        /*!
            @warning
            It is your responsibility to set the string to a value, that can be parsed again
            
            TODO: Remove this warning once there is protection agains special chars
            (add escape & unescape to Strings)
        */
        void setValue(ConstString value);

        //@}
        
        //! @name Binding
        //@{
        
        //! Get current binder
        BinderPtr getBinder()
        {
            return binder;
        }

        //! Set binder
        /*
            @note
            NULL is allowed.
        */
        void setBinder(BinderPtr binder)
        {
            this->binder = binder;
            if (binder != NULL)
                binder->setString(value);
        }
        
        //@}

    protected:
        String value;
        BinderPtr binder;
};

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//! Return a new node
/*
    Returns new node, type is guessed from contents of the cache
    
    @param cache cache to use for guessing    
    
    @note
    Do not use this function, it will be probably removed in the future !    
*/
NodePtr newNode(StringInputCache &cache);

//! @name Load & Save
//@{

//! Load a config file
/*
    @param input string to be used for assembling the tree
    @return root of the config file node tree
*/
NodePtr loadFile(ConstString input);

//! Save a config file
/*
    @param root of the config file node tree
    @return string containg the config file
*/
String saveFile(NodePtr root);

//@}

//! @name Get nodes by path
//@{

//! Return given node 
/*
    @param path path to the node
    @param base node the path is relative to
    
    @return pointer to the given node (or NULL if node not found)
*/
inline NodePtr getNode(ConstString path, NodePtr base)
{
    StringInputCache cache(path);
    NodePtr node;
    
    // TODO: or silently return NULL if base is NULL ?
    assert(base != NULL);
    return base->getNode(cache);
};

//! Return given node as array node (if possible)
/*
    @param path path to the node
    @param base node the path is relative to
    
    @return pointer to the given node (or NULL if node not found)    
*/
inline ArrayNodePtr getArrayNode(ConstString path, NodePtr base)
{
    NodePtr node = getNode(path, base);
    return (node != NULL) ? (boost::dynamic_pointer_cast<ArrayNode>(node)) : ArrayNodePtr((ArrayNode *)NULL);
}

//! Return given node as map node (if possible)
/*
    @param path path to the node
    @param base node the path is relative to
    
    @return pointer to the given node (or NULL if node not found)    
*/
inline MapNodePtr getMapNode(ConstString path, NodePtr base)
{
    NodePtr node = getNode(path, base);
    return (node != NULL) ? (boost::dynamic_pointer_cast<MapNode>(node)) : MapNodePtr((MapNode *)NULL);
}

//! Return given node as value node (if possible)
/*
    @param path path to the node
    @param base node the path is relative to
    
    @return pointer to the given node (or NULL if node not found)    
*/
inline ValueNodePtr getValueNode(ConstString path, NodePtr base)
{
    NodePtr node = getNode(path, base);
    return (node != NULL) ? (boost::dynamic_pointer_cast<ValueNode>(node)) : ValueNodePtr((ValueNode *)NULL);
}

//@}

//! @name Get & set node values by path
//@{

//! Get value of given node
/*
    @param path path to the node
    @param base node the path is relative to
    @param defaultValue value to return if node is not found
    
    @return node value of defaultValue
*/
template <typename T>
T get(ConstString path, NodePtr base, const T &defaultValue)
{
    ValueNodePtr valueNode = getValueNode(path,base);    
    if (valueNode == NULL)
        return defaultValue;
    return fromString<T>(valueNode->getValue());
};

//! Set value of given node
/*
    @param path path to the node
    @param value value to set node to
    @param base node the path is relative to
    
    @note
    The order of parameters is different from set on purpose !
    
    @note
    It will crash if node is not found.
    TODO: once the implemntation is changed, update this note
    
*/
template <typename T>
void set(ConstString path, const T &value, NodePtr base)
{
    // TODO: create node if possible !
    getValueNode(path,base)->setValue(toString(value));
};

//@}

//! @name Binding
//@{

//! Bind node to given variable
/*
    @param path path to the node
    @param base node the path is relative to
    @param variable reference to variable to bind to (the variable has to exist
    till the destruction of the node in question)
    @param defaultValue value to set newly created node to (if node is not found) !!! currently unused !!!
    
    @note
    It will crash if node is not found.
    TODO: once the implemntation is changed, update this note    
*/
template <typename T>
void bind(ConstString path, NodePtr base, T& variable, const T &defaultValue)
{
    // TODO: create node if possible ! (and set it to defaultValue)
    getValueNode(path,base)->setBinder(BinderPtr(new Binder<T>(variable)));
};

//@}

} // namespace ConfigFile

#endif
