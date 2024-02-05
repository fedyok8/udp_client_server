#include "server.hpp"

namespace  {

std::string AddErrorHeaders(const std::string& message) {
  return std::string("-ERROR-\n") + message + "\n-END-";
}

std::string AddSuccessHeaders(const std::string& message) {
  return std::string("-BEGIN-\n") + message + "\n-END-";
}

const std::string kResourceNotFound(AddErrorHeaders("resource_not_found"));

} // namespace


Server::Server(uint16_t port, size_t max_request_size,
               uint8_t workers, size_t max_queue_size,
               const std::string& content_filepath)
  : ServerBase(port, max_queue_size, workers, max_queue_size)
  , resource_(content_filepath)
{}

Server::~Server() {}

std::string Server::Handle(const std::string& request) {
  if (!resource_.Contains(request)) {
    return kResourceNotFound;
  }
  return AddSuccessHeaders(resource_.Content(request));
}
