/*
note: node->load gets string that starts on the real value (= no whitespace on the beginning)

TODO:
1) fix ValueNode to allow classic single word (see documentation of valuenode for delimiters) or " (drop ') string with ESCAPING !!! (extend Strings.cpp for escaping)
2) make an unified loading (see loading of map & array, it is a mess)
*/
#include "ConfigFile.h"

#include "Log.h"

namespace ConfigFile
{

//------------------------------------------------------------------------------
// Node class
//------------------------------------------------------------------------------
Node::Node()
{
}
Node::~Node()
{
}

//------------------------------------------------------------------------------
// ArrayNode class
//------------------------------------------------------------------------------
void ArrayNode::load(StringInputCache &cache)
{
    /*
    got:
    a) |(*)   -> '()' mode
    b) |[index] =  -> 'index' mode
    c) something other -> error
    */
//    cache.skipCharsUntil("([");

    switch (cache.peekChar())
    {
        case '(':
            // '()' mode
            LOG_INFO("CfgFile", "ArrayNode::load - '()' mode");                
            
            // skip '('
            cache.advance();

            // skip whitespace before element child value or ')'
            cache.skipWhitespace();

            // load elements
            while (cache.peekChar() != ')')
            {
                // skip whitespace here
                cache.skipWhitespace();
           
                // load the child element
                NodePtr child = newNode(cache);
                child->load(cache);
                nodes.push_back(child);

                cache.skipCharsUntil(",)");
                
                if (cache.peekChar() == ')')
                    break;
                    
                // skip ,
                cache.advance();
            
                // skip whitespace
                cache.skipWhitespace();
            }

            // skip ')'
            cache.advance();
            
            break;
        case '[':
            // 'index' mode
            LOG_INFO("CfgFile", "ArrayNode::load - 'index' mode");                
                        
            // to avoid error about skipping inicialization of indexString & index
            {
            
                // skip '['
                cache.advance();
            
                // skip whitespace
                cache.skipWhitespace();
            
                // load the index
                String indexString;
                unsigned index;
                NodePtr childNode;
                
                indexString = cache.getWord("]/*");
                LOG_INFO("CfgFile", "ArrayNode::load - 'index string' = '%s'", indexString.c_str());                

                // skip whitespace
                cache.skipWhitespace();

                // skip ']'
                cache.advance();
                           
                // and convert it to number
                index = atoi(indexString.c_str());
            
                // if the index is good enough, then load the value
                if (index < nodes.size())
                {
                    // good, update existing node
                    nodes[index]->load(cache);
                }
                else if (index == nodes.size())
                {
                    // good, create new node
                    NodePtr child = newNode(cache); // guess type of child
                    nodes.push_back(child);
                    child->load(cache);
                }
                else
                {
                    // bad, ignore too big index
                    LOG_ERROR("CfgFile", "ArrayNode::load - invalid index (%d) of child node for array (%d)", index, nodes.size());                
                };        
            };
            break;
        default:
            // this should not happen
            LOG_ERROR("CfgFile", "ArrayNode::load - tried to load array in unknown mode (char '%c') !", cache.peekChar());
    };
    LOG_INFO("CfgFile", "ArrayNode::load - done");   
}
void ArrayNode::save(StringOutputCache &cache)
{
    cache.add("(");
    for (ArrayNodeNodes::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        if (i != nodes.begin())
            cache.add(", ");
        (*i)->save(cache);
    }
    cache.add(")");
}

// understands [something]
NodePtr ArrayNode::getNode(StringInputCache &path)
{
    String indexString;
    unsigned index;
    MapNodeNodes::iterator i;
    NodePtr node;

    // skip possible whitespace
    path.skipWhitespace();

    // am i requested ?
    if (path.isEos())
        return shared_from_this(); // smartpointer to this

    // skip to opening '['        
    path.skipCharsUntil("[");
    path.skipChars("[");
    path.skipWhitespace();
               
    // get name index (stop by whitespace, comment, = or sub-child delimiter)
    indexString = path.getWord("\n\t ]/");

    // skip possible whitespace after the index
    path.skipWhitespace();   
    path.skipChars("]");
    path.skipWhitespace();
    
    // and convert it to number
    index = atoi(indexString.c_str());

    LOG_INFO("CfgFile", "ArrayNode::get - request index '%s' -> %d (array size %d)", indexString.c_str(), index, nodes.size());                

    // if the index is good enough, then retrieve the node
    if (index < nodes.size())
    {
        node = nodes[index];
    }
    else
    {
        // bad, ignore too big index
        LOG_ERROR("CfgFile", "ArrayNode::get - invalid index ('%s' -> %d) of child node for array (%d)", indexString.c_str(), index, nodes.size());                
        return NodePtr((Node *)NULL);
    };
    
    return node->getNode(path);
}

NodePtr ArrayNode::getChild(ConstString child)
{
    unsigned index;

    assert(child.size() != 0);        
    if (child[0] != '[')
        LOG_ERROR("CfgFile", "ArrayNode::getChild - requested invalid child '%s'", child.c_str());    
    index = fromString<unsigned>(child.substr(1, child.size() - 2));
    if (index >= nodes.size())
        LOG_ERROR("CfgFile", "ArrayNode::getChild - requested invalid index '%u'", index);
        
    return nodes[index];    
}

void ArrayNode::addChild(ConstString name, NodePtr child)
{
    unsigned index;

    assert(name.size() != 0);        

    LOG_INFO("CfgFile", "ArrayNode::addChild - adding child '%s'...", name.c_str());
    LOG_INDENT();

    if (name[0] != '[')
        LOG_ERROR("CfgFile", "ArrayNode::addChild - requested to add an invalid child '%s'", name.c_str());    
    index = fromString<unsigned>(name.substr(1, name.size() - 2));
    if (index < nodes.size())
        LOG_ERROR("CfgFile", "ArrayNode::addChild - child of that index (%u) is already present !", index);
    if (index > nodes.size())
        LOG_ERROR("CfgFile", "ArrayNode::addChild - child of that index (%u) would create gap in indexes !", index);

    LOG_UNINDENT();
    LOG_INFO("CfgFile", "ArrayNode::addChild - done");

    nodes.push_back(child);
}


//------------------------------------------------------------------------------
// MapNode class
//------------------------------------------------------------------------------
void MapNode::load(StringInputCache &cache)
{
    /*
    method can get:
    a) |{*}   -> '{}' mode
    b) |.mychild*  -> '.subchild' mode
    c) something other -> error
    */
    MapNodeNodes::iterator i;
    String childName;
    NodePtr childNode;
    
    // skip whitespace before name of child node
    // (actually requred only if there is whitespace before the toplevevel map)
    cache.skipWhitespace();
    
    // determine the mode
    switch(cache.peekChar())
    {
        case '{':
            // '{}' mode
            LOG_INFO("CfgFile", "MapNode::load - '{}' mode");                
            
            // skip '{'
            cache.advance(); //skip '{'
            
            // remove all children nodes 
            // (this run can be an update called on this node)
            nodes.clear();

            // skip whitespace before name of child node
            cache.skipWhitespace();

            // till the end of map            
            while (cache.peekChar() != '}')
            {
           
                // skip whitespace before name of child node
                cache.skipWhitespace();
                
                // load the child element (childelement = something)
                loadChild(cache);                
            
                // skip (possible text left after child loading) to ';'
                cache.skipCharsUntil(";");

                // skip ';'
                cache.advance();
                               
                // skip whitespace (next is either new pair of 'item = value', '}' or comment
                cache.skipWhitespace();
            };
            break;
        case '.':
            LOG_INFO("CfgFile", "MapNode::load - '.subchild' mode");
            
            // skip '.'
            cache.advance();           
            
            // load the child element (childelement = something)
            loadChild(cache);                
 
            // NOTE: do not check for ';' ! It will be ckecked by the topmost mapnode (the one processed in '{}' mode)
            break;
        default:
            // this should not happen
            LOG_ERROR("CfgFile", "MapNode::load - tried to load map in unknown mode !");
    };      
    LOG_INFO("CfgFile", "MapNode::load - done");
}
void MapNode::save(StringOutputCache &cache)
{    
    cache.add("{");
    cache.indent();
    for (MapNodeNodes::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        cache.add("\n");
        cache.add(i->first);
        cache.add(" = ");
        i->second->save(cache);
        cache.add(";");
    }
    cache.unindent();
    cache.add("\n}");
}
        
NodePtr MapNode::getNode(StringInputCache &path)
{    
    String childName;
    MapNodeNodes::iterator i;

    // skip possible whitespace
    path.skipWhitespace();
    
    // skip possible leading '.'
    if (path.peekChar() == '.')
        path.advance();
   
    path.skipWhitespace();

    // am i requested ?
    if (path.isEos())
        return shared_from_this(); // smartpointer to this
               
    // get name of child node (stop by whitespace, comment, = or sub-child delimiter)
    childName = path.getWord("\n\t /*.[=");
    LOG_INFO("CfgFile", "MapNode::get - requested child ('%s') !", childName.c_str());

    // skip possible whitespace after child name
    path.skipWhitespace();

    // find it
    i = nodes.find(childName);
    if (i == nodes.end())
    {
        // TODO: remove this warning, it would fill the log while querying the config file !
        LOG_WARNING("CfgFile", "MapNode::get - child unknown ('%s') !", childName.c_str());
        return NodePtr((Node *)NULL);
    };
    
    return (i->second)->getNode(path);
}
NodePtr MapNode::getChild(ConstString child)
{
    unsigned base;
    String realChild;

    assert(child.size() != 0);    
    base = child.find_first_not_of(".");
    if (base == String::npos)
    {
        LOG_ERROR("CfgFile", "MapNode::getChild - requested non dot starting child '%s', this is not supported !", child.c_str());
        return NodePtr((Node *)NULL);
    };
    realChild = child.substr(base, child.size() - base);
    
    MapNodeNodes::iterator i = nodes.find(realChild);
    if (i == nodes.end())
    {
        LOG_ERROR("CfgFile", "MapNode::getChild - requested non-existent child '%s'", realChild.c_str());
        return NodePtr((Node *)NULL);
    }            
    return i->second;    
}

// TODO: recode, looks like a mess
void MapNode::addChild(ConstString name, NodePtr child)
{
    unsigned base;
    String realName;

    assert(name.size() != 0);    

    LOG_INFO("CfgFile", "MapNode::addChild - adding child '%s'...", name.c_str());
    LOG_INDENT();

    base = name.find_first_not_of(".");
    if (base == String::npos)
    {
        LOG_ERROR("CfgFile", "MapNode::addChild - requested non dot starting child '%s', this is not supported !", name.c_str());
        LOG_UNINDENT();
        LOG_INFO("CfgFile", "MapNode::addChild - FAILED !");
        return;        
    };
    realName = name.substr(base, name.size() - base);
    
    MapNodeNodes::iterator i = nodes.find(realName);
    if (i == nodes.end())
    {
        LOG_INFO("CfgFile", "MapNode::addChild - requested non-existent child '%s'", realName.c_str());
        nodes[realName] = child;
    }            
    else
    {
        LOG_ERROR("CfgFile", "ArrayNode::addChild - child of that name (%s) is already present !", realName.c_str());
        LOG_UNINDENT();
        LOG_INFO("CfgFile", "MapNode::addChild - FAILED !");
        return;
    }

    LOG_UNINDENT();
    LOG_INFO("CfgFile", "MapNode::addChild - done");

}

void MapNode::loadChild(StringInputCache &cache)
{
    String childName;
           
    // get name of child node (stop by whitespace, comment, = or sub-child delimiter)
    childName = cache.getWord("\n\t /*.[=");
                
    // and read possible whitespace after it (-> next is '.[' for subchild updates or '=' for child update)
    cache.skipWhitespace();
                               
    // log name of the child
    LOG_INFO("CfgFile", "MapNode::loadChild - loading child '%s'", childName.c_str());
                
    // is child name is followed by delimiter ? 
    if (cache.peekChar() == '.' || cache.peekChar() == '[')
    {
        // yes, then delegate parsing to the child - it is an child's update
        // (means: update called on child (= subchild nodes are inserted/replaced))
        LOG_INFO("CfgFile", "MapNode::loadChild - delegating to child of type '%s'", (cache.peekChar() == '.' ) ? "map" : "array");

        // load the value
        loadChildValue(childName, cache);                
    }
    else
    {
        // no, it is change of one of this node childs, handle here
        LOG_INFO("CfgFile", "MapNode::loadChild - handling myself");    

        // skip to '='
        cache.skipCharsUntil("=");
                    
        // skip '='
        cache.advance();
                    
        // skip whitespace before the actual value;
        cache.skipWhitespace();
                    
        // load the value
        loadChildValue(childName, cache);                
    };
    LOG_INFO("CfgFile", "MapNode::loadChild - child '%s' loaded", childName.c_str());                
}


void MapNode::loadChildValue(ConstString childName, StringInputCache &cache)
{
    NodePtr child;
    /*
    got the part after |  :
    itemA = |value
    itemB|[2] = value
    itemC| = value        
    itemC|.subitem = value        
    */
    MapNodeNodes::iterator i;
    i = nodes.find(childName);

    cache.skipChars("\n\t "); // get to the word
    if (i == nodes.end())
    {
        LOG_INFO("CfgFile", "loadChildValue NEW CHILD '%s'", childName.c_str());    
        child = newNode(cache); // get new node of guessed type
        nodes[childName] = child;
    }
    else
    {
        LOG_INFO("CfgFile", "loadChildValue KNOWN CHILD'%s'", childName.c_str());    
        child = i->second;      // get alredy existing node of same name
    }
    
    child->load(cache);
    LOG_INFO("CfgFile", "loadChildValue DONE '%s'", childName.c_str());    
}


//        MapNodeNodes nodes;

//------------------------------------------------------------------------------
// ValueNode class
//------------------------------------------------------------------------------        
void ValueNode::load(StringInputCache &cache)
{
    char c;
    
    c = cache.peekChar();
    if ((c == '"') || (c == '\''))
    {
        cache.advance(); // skip opening 
        value = cache.getWord(String("")+c); // TODO: is there a better way ?
        cache.advance(); // skip closing
    }
    else
        value = cache.getWord("\n\t /*,);");
    
    if (binder != NULL)
        binder->setString(value);
        
    LOG_INFO("CfgFile", "ValueNode::load - value: '%s'", value.c_str());
}
void ValueNode::save(StringOutputCache &cache)
{
    if (binder != NULL)
        value = binder->getString();
    if (value.find_first_of("\n\t /*,);") != String::npos)
        value = "\"" + value + "\"";
    cache.add(value);
}
        
ConstString ValueNode::getValue()
{
    LOG_WARNING("CfgFile", "ValueNode::getValue - returned ('%s') !", value.c_str());
    return value;
}
void ValueNode::setValue(ConstString value)
{
    this->value = value;
}

NodePtr ValueNode::getNode(StringInputCache &path)
{
    // skip possible whitespace
    path.skipWhitespace();
    
    // warn if next char is not =
    #ifndef DISABLE_LOG
    if (path.isEos() == false)
    {
        LOG_WARNING("CfgFile", "ValueNode::get - got non empty path ('%s') !", path.peekWord().c_str());
    }
    #endif
    
    return shared_from_this();
}
NodePtr ValueNode::getChild(ConstString child)
{
    if (child.size() != 0)
    {
        LOG_ERROR("CfgFile", "ValueNode::getChild - requested a child on ValueNode, child '%s'", child.c_str());
        return NodePtr((Node *)NULL);
    };
    return shared_from_this();
}
void ValueNode::addChild(ConstString name, NodePtr child)
{
    LOG_ERROR("CfgFile", "ValueNode::addChild - requested adding a child on ValueNode, child '%s'", name.c_str());    
}

//        String value;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

ParsedPath parsePath(ConstString path)
{
    StringInputCache sic(path);
    ParsedPath output;
    bool inArray = false; // if not set, then in map

    LOG_INFO("CfgFile", "parsePath - parsing path: '%s'", path.c_str());
    
    if (sic.peekChar() == '[')
    {
        inArray = true;
        // skip '['
        sic.advance();
    }
    else if (sic.peekChar() == '.')
    {
        // skip '.'
        sic.advance();
    }
    
    while (sic.isEos() == false)
    {           
        if (inArray)
        {
            // in array
            output.push_back("[" + sic.getWord("]") + "]");
            
            // skip ]
            sic.advance();
        }
        else
        {
            // in map
            output.push_back("." + sic.getWord(".["));
        };
        
        // check the end of path
        if (sic.isEos())
            break;        
            
        switch (sic.peekChar())
        {
            case '.':
                inArray = false;
                break;
            case '[':
                inArray = true;
                break;                
            default:
                LOG_INFO("CfgFile", "parsePath - malformed path ! '%s' (part: '%s')", path.c_str(), sic.c_str() + sic.getPosition());
                return ParsedPath();
        };
        
        // skip '.' or '['
        sic.advance();
        
    };
    
    for (ParsedPath::iterator i = output.begin(); i != output.end(); i++)
    {
        LOG_INFO("CfgFile", "path part: '%s'", i->c_str());            
    };
    
    return output;    
}

String assemblePath(const ParsedPath &path)
{
    String output;
    for (ParsedPath::const_iterator i = path.begin(); i != path.end(); i++)
    {
        output += *i;
    }
    return output;
}

NodePtr getNode(ConstString path, NodePtr base)
{
    assert(base != NULL);

    ParsedPath parsed = parsePath(path);
    NodePtr currentNode = base;    
   
    for (ParsedPath::iterator i = parsed.begin(); (i != parsed.end()) && (currentNode != NULL); i++)
        currentNode = currentNode->getChild(*i);

    return currentNode;
}

NodePtr createPathToNode(ParsedPath::const_iterator begin, ParsedPath::const_iterator end, NodePtr targetNode)
{
    // the last link, just return the requested target
    if (begin+1 == end)
        return targetNode;
    
    // child node (begin+1)
    NodePtr child = createPathToNode(begin+1, end, targetNode);
    
    // this node (begin)
    NodePtr parent = createParent(*(begin+1));
    parent->addChild(*(begin+1), child);
    return parent;
}

void assurePathToNode(NodePtr root, const ParsedPath &path, NodePtr targetNode)
{
    assert(root != NULL);

    LOG_INFO("CfgFile", "assurePathToNode for '%s'", assemblePath(path).c_str());    

    ParsedPath::const_iterator i = path.begin();
    NodePtr currentParent = root;    
    NodePtr currentChild = currentParent->getChild(*i);
    i++;
   
    // go to the last existent node
    while (currentChild != NULL && i != path.end())
    {
        currentParent = currentChild;
        currentChild = currentChild->getChild(*i);
        i++;
    };

    
    if (currentChild == NULL)
    {

        // one step back, we need name of the chid
        i--;
        LOG_INFO("CfgFile", "creating path from '%s' ...", i->c_str());            
        
        // currentParent is the last existent node
        // i points to name of child of currentParent that should exist
        currentParent->addChild(*i, createPathToNode(i, path.end(), targetNode));
    }
    else
    {
        LOG_INFO("CfgFile", "path found to '%s' ", i->c_str());    
    };
}


NodePtr newNode(StringInputCache &cache)
{
    LOG_INFO("CfgFile", "newNode got '%c'", cache.peekChar());    
    switch (cache.peekChar())
    {
        case '(':   // normal mode
        case '[':   // update mode
            return NodePtr(new ArrayNode());
        case '{':   // normal mode
        case '.':   // update mode
            return NodePtr(new MapNode());
        default:
            return NodePtr(new ValueNode());
    }
}

NodePtr createParent(ConstString child)
{
    assert(child.size() != 0);

    LOG_INFO("CfgFile", "createParent for  '%s'", child.c_str());    
    
    switch (child[0])
    {
        case '[':   // parent is an array
            return NodePtr(new ArrayNode());
        case '.':   // parent is a map
            return NodePtr(new MapNode());
        default:
            LOG_ERROR("CfgFile", "failed to create parent !");                
            return NodePtr((Node *) NULL);
    };
}

NodePtr loadFile(ConstString input)
{
    StringInputCache c(input);
    c.skipWhitespace();
    ConfigFile::NodePtr node;
    
    switch (c.peekChar())
    {
        case '{':   // map config file
        case '(':   // array only config file
            node = ConfigFile::newNode(c);
            node->load(c);
            break;
        default:
            // not guessable, probably bad file, default to map node...
            LOG_ERROR("CfgFile", "loadFile - bad file !, contents: '%s'", input.c_str());                
            node = NodePtr(new MapNode());
    }
    return node;
}

String saveFile(NodePtr root)
{
    StringOutputCache c;

    assert(root != NULL);
    root->save(c);
    
    // file should end with a newline
    c.add("\n");
    
    return c.getString();
}
     
} // namespace ConfigFile
