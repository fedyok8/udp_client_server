#ifndef SRC_SERVER_RESOURCE_CONTENT_HPP__
#define SRC_SERVER_RESOURCE_CONTENT_HPP__

#include <string>
#include <map>

class ResourceContent {
 public:
  ResourceContent(const std::string& filepath);
  bool IsEmpty() const { return content_.empty(); }
  bool Contains(const std::string& resource) const;
  std::string Content(const std::string& resource) const;

 private:
  std::map<std::string /*resource*/, std::string /*content*/> content_;
};

#endif // SRC_SERVER_RESOURCE_CONTENT_HPP__
