#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>

#define SOCK_PATH "sock.temp"
#define BUF_SIZE 1024

int main() {
    int server_fd, client1, client2;
    struct sockaddr_un address;
    char buffer[BUF_SIZE];

    // Sunucu soketi oluştur
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCK_PATH, sizeof(address.sun_path) - 1);

    // Daha önce kalan soket dosyasını temizle
    unlink(SOCK_PATH);

    // Soketi bağla
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
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
    client1 = accept(server_fd, NULL, NULL);
    if (client1 < 0) {
        perror("Accept client1 failed");
        return -4;
    }
    printf("Client 1 connected.\n");

    // İkinci istemciyi kabul et
    client2 = accept(server_fd, NULL, NULL);
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
            send(client2, buffer, received, 0); // Mesajı Client 2'ye ilet
        } else if (received == 0) {
            printf("Client 1 disconnected.\n");
            break;
        }

        // Client 2'den mesaj al
        received = recv(client2, buffer, BUF_SIZE, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Client 2: %s\n", buffer);
            send(client1, buffer, received, 0); // Mesajı Client 1'e ilet
        } else if (received == 0) {
            printf("Client 2 disconnected.\n");
            break;
        }
    }

    close(client1);
    close(client2);
    close(server_fd);
    unlink(SOCK_PATH);

    return 0;
}


