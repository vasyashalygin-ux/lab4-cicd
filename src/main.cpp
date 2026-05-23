#include <iostream>
#include <string>
#include <sstream>
#include <atomic>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

static std::atomic<int> request_count{0};

std::string build_response(int status_code, const std::string& body,
                           const std::string& content_type = "text/plain") {
    std::string status_text = (status_code == 200) ? "OK" : "Not Found";
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status_code << " " << status_text << "\r\n"
        << "Content-Type: " << content_type << "\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << body;
    return oss.str();
}

std::string handle_request(const std::string& request) {
    request_count++;
    int count = request_count.load();

    if (request.find("GET /health") != std::string::npos) {
        return build_response(200, "OK");
    }
    if (request.find("GET /metrics") != std::string::npos) {
        std::string body = "http_requests_total " + std::to_string(count) + "\n";
        return build_response(200, body);
    }
    if (request.find("GET /") != std::string::npos) {
        std::string body = "Hello from CI/CD #" + std::to_string(count) + "\n";
        return build_response(200, body);
    }
    return build_response(404, "Not Found");
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); return 1;
    }
    listen(server_fd, 128);
    std::cout << "Server listening on port 8080" << std::endl;

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) continue;

        char buf[4096] = {};
        ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
        std::string response;
        if (n > 0) {
            response = handle_request(std::string(buf, n));
        } else {
            response = build_response(400, "Bad Request");
        }
        write(client_fd, response.c_str(), response.size());
        close(client_fd);
    }
    close(server_fd);
    return 0;
}
