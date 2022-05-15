// C++ Standard Library
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// PicoJSON
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include <picojson/picojson.h>
#pragma GCC diagnostic pop

struct Element
{
  enum Type
  {
    TRANSLATION_UNIT,
    NAMESPACE,
    ACCESS_SPECIFIER,
    COMMENT,
    TYPEDEF,
    BUILTIN,
    CLASS,
    CLASS_CTOR,
    CLASS_VARIABLE,
    CLASS_METHOD,
    CLASS_TEMPLATE,
    CLASS_TEMPLATE_PARAM,
    FUNCTION_TEMPLATE,
    ENUM,
    ENUM_LABEL,
    IGNORED,
  };

  struct Properties
  {
    std::uint16_t is_private : 1;
  };

  std::size_t parent = std::numeric_limits<std::size_t>::max();
  Type type = Type::TRANSLATION_UNIT;
  const char* name = "";
  Properties properties = {};
  std::vector<std::size_t> children = {};

  Element() = default;
  Element(Type _type, const char* _name) : type{_type}, name{_name}, children{} {};
};

const static std::unordered_map<std::string, Element::Type> StringToElementType{
  {"TranslationUnitDecl", Element::TRANSLATION_UNIT},
  {"NamespaceDecl", Element::NAMESPACE},
  {"AccessSpecDecl", Element::ACCESS_SPECIFIER},
  {"CXXRecordDecl", Element::CLASS},
  {"CXXConstructorDecl", Element::CLASS_CTOR},
  {"TypedefDecl", Element::TYPEDEF},
  {"BuiltinType", Element::BUILTIN},
  {"FieldDecl", Element::CLASS_VARIABLE},
  {"CXXMethodDecl", Element::CLASS_METHOD},
  {"ClassTemplateDecl", Element::CLASS_TEMPLATE},
  {"TemplateTypeParmDecl", Element::CLASS_TEMPLATE_PARAM},
  {"FunctionTemplateDecl", Element::FUNCTION_TEMPLATE},
  {"FullComment", Element::COMMENT},
  {"TextComment", Element::COMMENT},
  {"BlockCommandComment", Element::COMMENT},
  {"ParagraphComment", Element::COMMENT},
  {"ParmVarDecl", Element::IGNORED},
  {"InitListExpr", Element::IGNORED},
};

struct Context
{
  std::uint16_t is_private : 1;
};

typedef void (*ParsePropFunc)(Element&, Context&, const picojson::object&);

const static std::unordered_map<Element::Type, ParsePropFunc> ElementTypeToParse{
  {Element::TRANSLATION_UNIT,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::NAMESPACE,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::ACCESS_SPECIFIER,
   [](Element& e, Context& ctx, const picojson::object& object) {
     ctx.is_private = (object.at("access").get<std::string>() != "public");
   }},
  {Element::CLASS,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::CLASS_CTOR,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::TYPEDEF,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::BUILTIN,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::CLASS_VARIABLE,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::CLASS_METHOD,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::CLASS_TEMPLATE,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::CLASS_TEMPLATE_PARAM,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::FUNCTION_TEMPLATE,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
  {Element::COMMENT,
   [](Element& e, Context& ctx, const picojson::object& object) {

   }},
};

static std::size_t
parse(std::vector<Element>& element_tree, Context& ctx, const picojson::value& v, const std::size_t parent = 0)
{
  const auto& object = v.get<picojson::object>();

  const std::size_t pos = element_tree.size();

  Element element;
  element.parent = (parent == pos) ? std::numeric_limits<std::size_t>::max() : parent;
  element.properties.is_private = ctx.is_private;

  // Locate "kind" field
  {
    const auto itr = object.find("kind");
    if (itr != object.end())
    {
      const auto& kind = itr->second.get<std::string>();

      std::cout << " --> " << kind << std::endl;
      const auto kv_itr = StringToElementType.find(kind);
      if (kv_itr != StringToElementType.end() and kv_itr->second != Element::Type::IGNORED)
      {
        std::cout << " ----> got it" << std::endl;
        element.type = kv_itr->second;
      }
      else
      {
        return std::numeric_limits<std::size_t>::max();
      }
    }
  }

  // Locate "name" field
  {
    const auto itr = object.find("name");
    if (itr != object.end())
    {
      const auto& name = itr->second.get<std::string>();
      if (!element_tree.empty())
      {
        element.name = name.c_str();
      }
    }
  }

  // Run type specific parse
  ElementTypeToParse.at(element.type)(element, ctx, object);

  // Add to tree
  element_tree.emplace_back(std::move(element));
  {
    const auto itr = object.find("inner");
    if (itr != object.end())
    {
      Context inner_ctx{};
      for (const auto& inner_v : itr->second.get<picojson::array>())
      {
        const auto child_pos = parse(element_tree, inner_ctx, inner_v, pos);
        if (child_pos != std::numeric_limits<std::size_t>::max())
        {
          element_tree[pos].children.emplace_back(child_pos);
        }
      }
    }
  }

  return pos;
}

static void generate(const std::vector<Element>& tree, const std::size_t pos = 0)
{
  const auto& e = tree[pos];
  if (e.type == Element::Type::NAMESPACE or e.type == Element::Type::TRANSLATION_UNIT)
  {
    for (const auto c_pos : e.children)
    {
      generate(tree, c_pos);
    }
  }
  else if (e.type == Element::Type::CLASS or e.type == Element::Type::CLASS_TEMPLATE)
  {
    std::cout << e.name << "   " << e.children.size() << std::endl;
    for (const auto c_pos : e.children)
    {
      if (tree[c_pos].type == Element::Type::CLASS)
      {
        std::cout << "class: " << tree[c_pos].name << " -- " << tree[c_pos].children.size() << std::endl;
        generate(tree, c_pos);
      }
      else if (tree[c_pos].type == Element::Type::CLASS_VARIABLE)
      {
        std::cout << "var: " << tree[c_pos].name << " -- " << tree[c_pos].children.size() << std::endl;
      }
      else if (tree[c_pos].type == Element::Type::CLASS_METHOD)
      {
        std::cout << "mfn: " << tree[c_pos].name << " -- " << tree[c_pos].children.size() << std::endl;
      }
    }
  }
}

int main(int argc, char** argv)
{
  const picojson::value ast = [argv] {
    std::ifstream ifs{argv[1]};
    picojson::value v;
    ifs >> v;
    return v;
  }();

  for (int i = 2; i < argc; ++i)
  {
    std::ofstream ofs{argv[i]};
  }

  std::vector<Element> element_tree;

  Context ctx{};
  parse(element_tree, ctx, ast);

  generate(element_tree, 0);

  // const auto& obj = ast.get<picojson::object>();
  // const auto itr = obj.find("inner");
  // if (itr != obj.end())
  // {
  //   for (const auto& value : itr->second.get<picojson::array>())
  //   {
  //     auto& arr_obj = value.get<picojson::object>();
  //     auto name_itr = arr_obj.find("name");
  //     if (name_itr != arr_obj.end())
  //     {
  //       std::cout << name_itr->second.get<std::string>() << std::endl;
  //     }
  //   }
  // }

  return 0;
}