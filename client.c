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
    int sock;
    struct sockaddr_un server_addr;
    char buffer[BUF_SIZE];
    char message[BUF_SIZE];

    // Soket oluştur
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Sunucu adresini ayarla
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCK_PATH, sizeof(server_addr.sun_path) - 1);

    // Sunucuya bağlan
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(sock);
        return -2;
    }

    printf("Connected to the server.\n");

    // Mesaj gönderme ve alma döngüsü
    while (1) {
        printf("Enter message: ");
        fgets(message, BUF_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0'; // Yeni satırı kaldır

        // Mesajı gönder
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }

        // Sunucudan mesaj al
        int received = recv(sock, buffer, BUF_SIZE, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Received: %s\n", buffer);
        } else if (received == 0) {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
