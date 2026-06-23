#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

pthread_mutex_t forks[N]; // 5 chiếc đũa là 5 cái khóa (Mutex) độc lập
int phil[N] = {0, 1, 2, 3, 4};

void* philosopher(void* num) {
    int i = *(int*)num;
    int left = i;
    int right = (i + 1) % N;

    while (1) {
        printf("Triết gia %d: Đang suy nghĩ...\n", i + 1);
        usleep(100000); // Nghĩ một chút

        printf("Triết gia %d: Đói bụng. Đang cố gắng lấy đũa trái (%d)...\n", i + 1, left);
        pthread_mutex_lock(&forks[left]);
        printf("Triết gia %d: Đã lấy đũa trái (%d). Đang đợi đũa phải (%d)...\n", i + 1, left, right);
        
        // MẤU CHỐT GÂY DEADLOCK: 
        // Thêm delay ở đây để ép tất cả triết gia đều kịp lấy đũa trái 
        // TRƯỚC KHI bất kỳ ai kịp lấy đũa phải -> Chắc chắn 100% treo máy.
        usleep(50000); 

        pthread_mutex_lock(&forks[right]);
        printf("Triết gia %d: Đã lấy được 2 đũa. ĐANG ĂN!\n", i + 1);
        
        usleep(100000); // Thời gian ăn

        // Trả đũa
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
        printf("Triết gia %d: Đã ăn xong và trả đũa.\n", i + 1);
    }
}

int main() {
    int i;
    pthread_t thread_id[N];

    // Khởi tạo 5 mutex cho 5 chiếc đũa
    for (i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Bắt đầu tạo luồng cho các triết gia
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
    }

    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
