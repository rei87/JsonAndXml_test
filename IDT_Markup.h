// IDT_Markup.h
//Gemstone Copyright?
#ifndef IDTMARKUP_H
#define IDTMARKUP_H

#include <string>
#include <list>
#include <map>

#define IN
#define OUT
#define IN_OUT

//Error Codes
#define IDT_SUCCESS                        0


#define MARKUP_INVALID_PARAMETER                    -600
#define MARKUP_VALUE_NOT_FOUND                        -601
#define MARKUP_INDEX_OUT_OF_RANGE                    -602
#define MARKUP_NODE_CANNOT_BE_LIST_AND_ELEMENT        -603
#define MARKUP_INVALID_URL                            -604
#define MARKUP_INVALID_SCHEMA                        -608
#define MARKUP_MALFORMED_DATA                        -609

//Validation Warnings
#define MARKUP_SCHEMA_URL_EMPTY                            -611
#define MARKUP_NO_SCHEMA_DATA                            -612
#define MARKUP_NO_DATA                                    -613
#define MARKUP_CHILD_NULL_REFERENCE_REPAIRED            -614
#define MARKUP_ORDER_INCONSISTENT                        -615

//XML Validation Warnings
#define MARKUP_XML_VERSION_INVALID                        -620
#define MARKUP_XML_ENCODING_INVALID                        -621

//JSON Validation Warnings
#define MARKUP_JSON_TITLE_INVALID                        -630
#define MARKUP_JSON_DESCRIPTION_INVALID                    -631

//Validation Errors
#define MARKUP_SCHEMA_MISSING_REQUIRED_HEADER_TAG        -650
#define MARKUP_SCHEMA_URL_IS_INVALID                    -651
#define MARKUP_MISSING_SCHEMA_URL                        -652
#define MARKUP_TAG_IS_NOT_CLOSED                        -653
#define MARKUP_UNBALANCED_FORMATTING_CHARACTER            -654
#define MARKUP_INVALID_CHARACTER                        -655
#define MARKUP_MISSING_FORMAT_CHARACTER                    -656
#define MARKUP_INVALID_WHITESPACE                        -657
#define MARKUP_MISSING_OR_INVALID_TAG_DATA                -658
#define MARKUP_DUPLICATE_CHILD_LINK                        -659
#define MARKUP_PARENT_NULL_REFERENCE                    -660
#define MARKUP_INVALID_ELEMENT                            -661
#define MARKUP_ELEMENT_FORMAT_MISMATCH                    -662
#define MARKUP_TAG_MISMATCH                                -663
#define MARKUP_ATTRIBUTE_MISMATCH                        -664

enum class Markup_Type {
    MARKUP_TYPE_NULL = 0,
    MARKUP_TYPE_STRING,
    MARKUP_TYPE_ATTRIBUTE,
    MARKUP_TYPE_BOOLEAN,
    MARKUP_TYPE_ARRAY,
    MARKUP_TYPE_OBJECT,
    MARKUP_TYPE_DECIMAL,
    MARKUP_TYPE_FLOAT,
    MARKUP_TYPE_DOUBLE,
    MARKUP_TYPE_DURATION,
    MARKUP_TYPE_DATETIME,
    MARKUP_TYPE_TIME,
    MARKUP_TYPE_DATE
};

if()// To add with error.
class Markup_Node {
public:
    Markup_Node();

    int setTag(IN const std::string &tag);

    int getTag(OUT std::string &tag) const;

    int getChildListSize(OUT int *pSize) const; // TODO: Why a pointer an not a reference (&)

    // TODO: See change in parameter. Need to pass *& to be able to change the pointer value.
    int getChildByIndex(IN const int index, OUT Markup_Node *&pChild);

    int getChildList(OUT std::list<Markup_Node *> &pChildren) const;

    int setChildList(IN const std::list<Markup_Node *> &pChildren);

    int addChild(IN const Markup_Node *pChild);

    int removeChild(IN const Markup_Node *pChild);

    int clearChildList(void);

    int setType(IN const Markup_Type type);

    int getType(OUT Markup_Type &type) const;

    int setElement(IN const std::string element);

    int getElement(OUT std::string &element) const;

    int setParent(IN const Markup_Node *pParent);

    // TODO: See change in parameter. Need to pass *& to be able to change the pointer value.
    int getParent(OUT Markup_Node *&pParent) const;

    Markup_Node& operator=(const Markup_Node &node);

    ~Markup_Node();

protected:
    std::string tag;
    std::list<Markup_Node *> pChildElements;    //Can have pChildElements or element; not both.
    Markup_Type type;                            //Should default to MARKUP_TYPE_STRING.
    std::string element;                        //Can have pChildElements or element; not both.
    Markup_Node *pParent;
};


class Markup_Base {
public:
    int setSchemaURL(IN const std::string &schemaURL);

    int getSchemaURL(OUT std::string &schemaURL) const;

    int setRootNode(IN const Markup_Node *pRoot);

    // TODO: See change in parameter. Need to pass *& to be able to change the pointer value.
    int getRootNode(OUT Markup_Node *&pRoot) const;

    int deleteData(void);

    int setRootSchemaNode(IN const Markup_Node *pRootSchema);

    // TODO: See change in parameter. Need to pass *& to be able to change the pointer value.
    int getRootSchemaNode(OUT Markup_Node *&pRootSchema) const;

    int deleteSchema(void);

    virtual int importSchema(IN const std::string &schema) = 0;

    virtual int exportSchema(OUT std::string &schema) = 0;

    int importSchemaFromFile(IN const std::string &schemaFilePath);

    int exportSchemaToFile(IN std::string &schemaFilePath);

    virtual int importData(IN const std::string &data) = 0;

    virtual int exportData(OUT std::string &data) = 0;

    int importDataFromFile(IN const std::string &dataFilePath);

    int exportDataToFile(IN const std::string &dataFilePath);

    virtual int validate(OUT std::map<Markup_Node *, std::string> &issues) = 0;

    virtual int validateString(IN const std::string &markup, OUT std::map<int, std::string> &issues) = 0;

    int validateFile(IN const std::string &markupFilePath, OUT std::map<int, std::string> &issues);

    virtual void operator=(Markup_Base &) = 0;

    int find(IN const std::string &searchString, OUT std::list<Markup_Node *> &searchResults);

protected:
    std::string schemaURL;    //example: "http://json-schema.org/schema#" or "http:www.w3.org/2001/XMLSchema"
    Markup_Node *pRootNode;
    Markup_Node *pRootSchemaNode;

    void validateNodes(OUT std::map<Markup_Node *, std::string> &issues);
};


class Markup_XML : public Markup_Base {
public:
    int setVersion(IN const std::string &version);

    int getVersion(OUT std::string &version);

    int setEncoding(IN const std::string &encoding);

    int getEncoding(OUT std::string &encoding);

    int importSchema(IN const std::string &schema);

    int exportSchema(OUT std::string &schema);

    int importData(IN const std::string &data);

    int exportData(OUT std::string &data);

    int validate(OUT std::map<Markup_Node *, std::string> &issues);

    int validateString(IN const std::string &markup, OUT std::map<int, std::string> &issues);

    void operator=(Markup_Base &);

protected:
    std::string version;  //example: "1.0"
    std::string encoding;  //example: "ISO-8859-1"
};

class Markup_JSON : public Markup_Base {
public:
    int setTitle(IN const std::string &title);

    int getTitle(OUT std::string &title);

    int setDescription(IN const std::string &description);

    int getDescription(OUT std::string &description);

    int importSchema(IN const std::string &schema);

    int exportSchema(OUT std::string &schema);

    int importData(IN const std::string &data);

    int exportData(OUT std::string &data);

    int validate(OUT std::map<Markup_Node *, std::string> &issues);

    int validateString(IN const std::string &markup, OUT std::map<int, std::string> &issues);

    void operator=(Markup_Base &);

protected:
    std::string title;
    std::string description;
};

//String conversions - move to its own file
int stringToReadableHex(IN const std::string &str, OUT std::string &readableHex);

int readableHexToString(IN const std::string &readableHex, OUT std::string &str);

int stringToBCD(IN const std::string &str, OUT std::string &BCD);

int BCDToString(IN const std::string &BCD, OUT std::string &str);

int stringToBase64(IN const std::string &str, OUT std::string &base64);

int base64ToString(IN const std::string &base64, OUT std::string &str);

#endif



