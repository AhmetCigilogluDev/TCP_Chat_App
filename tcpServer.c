#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client1, client2;
    struct sockaddr_in address;
    char buffer[BUF_SIZE];
    socklen_t addrlen = sizeof(address);

    // Sunucu soketini oluştur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Soketi bağla
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -2;
    }

    // Dinlemeye başla
    if (listen(server_fd, 2) < 0) {
        perror("Listen failed");
        return -3;
    }

    printf("Awaiting clients...\n");

    // İlk istemciyi kabul et
    client1 = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client1 < 0) {
        perror("Accept client1 failed");
        return -4;
    }
    printf("Client 1 connected.\n");

    // İkinci istemciyi kabul et
    client2 = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client2 < 0) {
        perror("Accept client2 failed");
        close(client1);
        return -5;
    }
    printf("Client 2 connected.\n");

    // Mesaj alışverişi döngüsü
    while (1) {
        // Client 1'den mesaj al
        int received = recv(client1, buffer, BUF_SIZE, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Client 1: %s\n", buffer);
            send(client2, buffer, received, 0);
        } else if (received == 0) {
            printf("Client 1 disconnected.\n");
            break;
        }

        // Client 2'den mesaj al
        received = recv(client2, buffer, BUF_SIZE, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Client 2: %s\n", buffer);
            send(client1, buffer, received, 0);
        } else if (received == 0) {
            printf("Client 2 disconnected.\n");
            break;
        }
    }

    close(client1);
    close(client2);
    close(server_fd);

    return 0;
}
