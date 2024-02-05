#include <iostream>

#include "client.hpp"
#include "client_request.hpp"

int main(int argc, char** argv) {
  CmdParser cmd(argc, argv);

  if (cmd.FindFlag('h', "help")) {
    ClientRequest::PrintUsage();
    return 0;
  }

  ClientRequest request(cmd);
  Client client;
  std::clog << "Sent " << request.to_string() << std::endl;
  std::string responce = client.Send(request.port_, request.resource_,
                                     request.max_responce_length_);
  std::clog << "Received:\n"
            << (responce.empty() ? std::string("EMPTY") : responce)
            << std::endl;
}
