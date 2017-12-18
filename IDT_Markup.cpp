#include "IDT_Markup.h"
#include <regex>
#include <fstream>

// class Markup_Node - Begin

static bool isLeafNode(const Markup_Node *node) {
    Markup_Type nodeType;
    node->getType(nodeType);
    // TODO: Are MARKUP_TYPE_ARRAY and MARKUP_TYPE_OBJECT the ONLY type of nodes that allow to set the
    //   pChildElements list????????
    return (nodeType == Markup_Type::MARKUP_TYPE_ARRAY ||
            nodeType == Markup_Type::MARKUP_TYPE_OBJECT);
}

Markup_Node::Markup_Node()
        : tag{}, pChildElements{nullptr}, type{Markup_Type::MARKUP_TYPE_STRING},
          element{}, pParent{nullptr} {
}

int Markup_Node::setTag(const std::string &tag) {
    // TODO: Check when to return MARKUP_INVALID_PARAMETER
    this->tag = tag;
    return IDT_SUCCESS;
}

int Markup_Node::getTag(std::string &tag) const {
    tag = this->tag;
    return IDT_SUCCESS;
}

int Markup_Node::getChildListSize(int *pSize) const {
    if (pSize != nullptr) {
        *pSize = pChildElements.size();
        return IDT_SUCCESS;
    }
    return MARKUP_INVALID_PARAMETER;    // TODO: See if this error is right
}

// TODO: See - this function lets the class client to delete the node!!!!!
int Markup_Node::getChildByIndex(const int index, Markup_Node *&pChild) {
    if (index >= 0 && index < pChildElements.size()) {         // TODO: See this condition
        pChild = nullptr;
        return MARKUP_INDEX_OUT_OF_RANGE;
    }
    auto iter = pChildElements.begin();
    if (iter == pChildElements.end()) {
        pChild = nullptr;
        return MARKUP_INDEX_OUT_OF_RANGE;   // TODO: Maybe another error type??
    }
    std::advance(iter, index);
    pChild = *iter;
    return IDT_SUCCESS;
}

int Markup_Node::getChildList(std::list<Markup_Node *> &pChildren) const {
    pChildren = pChildElements;
    return IDT_SUCCESS;
}

int Markup_Node::setChildList(const std::list<Markup_Node *> &pChildren) {
    // TODO:    See potential memory leaks! Don't know if I should free the nodes
    //  because using Markup_Node::getChildByIndex could lead to possibly
    //  dangling pointers
    if (!isLeafNode(this)) {
        pChildElements = pChildren;
        return IDT_SUCCESS;
    }
    return MARKUP_NODE_CANNOT_BE_LIST_AND_ELEMENT;
}

int Markup_Node::addChild(const Markup_Node *pChild) {
    if (pChild == nullptr) {
        return -1;      // TODO: What should I return in this case?
    }
    if (!isLeafNode(this)) {
        pChildElements.push_back(const_cast<Markup_Node *>(pChild));
        return IDT_SUCCESS;
    }
    return MARKUP_NODE_CANNOT_BE_LIST_AND_ELEMENT;
}

int Markup_Node::removeChild(const Markup_Node *pChild) {
    auto itElem = std::find(pChildElements.begin(), pChildElements.end(), pChild);
    if (itElem != pChildElements.end()) {    //TODO: To be better this condition
        pChildElements.erase(itElem);
        return IDT_SUCCESS;
    }
    return MARKUP_VALUE_NOT_FOUND;
}

int Markup_Node::clearChildList(void) {
    pChildElements.clear();
    return IDT_SUCCESS;
}

int Markup_Node::setType(const Markup_Type type) {
    // TODO: What if it's a node with a child list? Should I clear it? Is this possible?
    this->type = type;
    return IDT_SUCCESS;
}

int Markup_Node::getType(Markup_Type &type) const {
    type = this->type;
    return IDT_SUCCESS;
}

int Markup_Node::setElement(const std::string element) {
    if (isLeafNode(this)) {       //TODO: Believe is not necesary the comparison
        // TODO: MARKUP_INVALID_PARAMETER - binary string or somehting like that!
        this->element = element;
        return IDT_SUCCESS;
    }
    return MARKUP_NODE_CANNOT_BE_LIST_AND_ELEMENT;
}

int Markup_Node::getElement(std::string &element) const {
    element = this->element;
    return IDT_SUCCESS;
}

int Markup_Node::setParent(const Markup_Node *pParent) {
    this->pParent = const_cast<Markup_Node *>(pParent);
    return IDT_SUCCESS;
}

int Markup_Node::getParent(Markup_Node *&pParent) const {
    pParent = this->pParent;
    return IDT_SUCCESS;
}
 
Markup_Node& Markup_Node::operator=(const Markup_Node &node) {
    if (this == &node) {
        return *this;
    }
    this->tag = node.tag;
    this->pChildElements = node.pChildElements;
    this->type = node.type;
    this->element = node.element;
    this->pParent = node.pParent;
    return *this;
}

Markup_Node::~Markup_Node() {
    std::for_each(pChildElements.begin(), pChildElements.end(),
                  [](Markup_Node *&elem) -> void { delete elem; });
}


// class Markup_Node - End


// class Markup_Base - Begin

static bool isValidUrl(const std::string strUrl) {
    // TODO: Need testing!
    std::regex url_reg(R"(^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?)",
                       std::regex::extended);
    return std::regex_search(strUrl, url_reg);
}

int Markup_Base::setSchemaURL(const std::string &schemaURL) {
    if (isValidUrl(schemaURL)) {
        this->schemaURL = schemaURL;
        return IDT_SUCCESS;
    }
    return MARKUP_INVALID_URL;
}

int Markup_Base::getSchemaURL(std::string &schemaURL) const {
    schemaURL = this->schemaURL;
    return IDT_SUCCESS;
}

int Markup_Base::setRootNode(const Markup_Node *pRoot) {
    // TODO: See - if this->pRootNode is pointing to something, is it OK to leak the memory?
    this->pRootNode = const_cast<Markup_Node *>(pRoot);
    return IDT_SUCCESS;
}

int Markup_Base::getRootNode(Markup_Node *&pRoot) const {
    pRoot = this->pRootNode;
    return IDT_SUCCESS;
}

int Markup_Base::deleteData(void) {
    if (pRootNode != nullptr) {
        delete pRootNode;
        pRootNode = nullptr;
    }
    return IDT_SUCCESS;
}

int Markup_Base::setRootSchemaNode(const Markup_Node *pRootSchema) {
    // TODO: See - if this->pRootSchemaNode is pointing to something, is it OK to leak the memory?
    this->pRootSchemaNode = const_cast<Markup_Node *>(pRootSchema);
    return IDT_SUCCESS;
}

int Markup_Base::getRootSchemaNode(Markup_Node *&pRootSchema) const {
    pRootSchema = this->pRootSchemaNode;
    return IDT_SUCCESS;
}

int Markup_Base::deleteSchema(void) {
    if (pRootSchemaNode != nullptr) {
        delete pRootSchemaNode;
        pRootSchemaNode = nullptr;
    }
    return IDT_SUCCESS;
}

int Markup_Base::importSchemaFromFile(const std::string &schemaFilePath) {
    std::ifstream istream{schemaFilePath, std::ios_base::in};
    if (!istream.is_open()) {
        return -1; // TODO: Error needs to be defined
    }
    std::string strData{std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>()};
    return importSchema(strData);
    // TODO: MARKUP_INVALID_SCHEMA
    // TODO: See IDT_SUCCESS
}

int Markup_Base::exportSchemaToFile(std::string &schemaFilePath) {
    std::string strData;
    exportSchema(strData);
    std::ofstream ostream{schemaFilePath, std::ios_base::out};
    if (!ostream.is_open()) {
        return -1; // TODO: Error needs to be defined
    }
    ostream << strData;
    // TODO: MARKUP_INVALID_SCHEMA
    // TODO: Other errors
    return IDT_SUCCESS;
}

int Markup_Base::importDataFromFile(const std::string &dataFilePath) {
    return 0;
}

int Markup_Base::exportDataToFile(const std::string &dataFilePath) {
    return 0;
}

int Markup_Base::validateFile(const std::string &markupFilePath, std::map<int, std::string> &issues) {
    return 0;
}

int Markup_Base::find(const std::string &searchString, std::list<Markup_Node *> &searchResults) {
    return 0;
}
