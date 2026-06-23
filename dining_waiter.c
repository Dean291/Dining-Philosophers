#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h> // Thư viện dùng để đo thời gian chính xác

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// Xác định vị trí hàng xóm bên trái và bên phải
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N

int state[N];         // Mảng lưu trạng thái của 5 triết gia
int phil[N] = {0, 1, 2, 3, 4}; // Định danh ID cho từng triết gia

pthread_mutex_t waiter; // "Trọng tài" điều phối bàn ăn
pthread_cond_t cv[N];   // Biến điều kiện (Condition Variable) cho từng người ngủ/thức

// Trọng tài kiểm tra xem triết gia i có đủ điều kiện ăn hay không
void test(int i) {
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        printf("-> Trọng tài: Cho phép Triết gia %d được ĂN.\n", i + 1);
        pthread_cond_signal(&cv[i]); // Đánh thức triết gia i dậy để ăn
    }
}

// Triết gia vào xin phép Trọng tài để lấy đũa
void pickup_forks(int i) {
    pthread_mutex_lock(&waiter); // Khóa Trọng tài lại để thực hiện giao dịch độc quyền

    state[i] = HUNGRY;
    printf("Triết gia %d: Đang ĐÓI bụng...\n", i + 1);

    // --- BẮT ĐẦU ĐO THỜI GIAN CHỜ (Phục vụ cho báo cáo) ---
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // Trọng tài kiểm tra bàn ăn liền xem có đũa trống không
    test(i);

    // Nếu 2 bên hàng xóm đang ăn, triết gia i bắt buộc phải ngủ đợi
    while (state[i] != EATING) {
        pthread_cond_wait(&cv[i], &waiter); 
        // Hàm wait này sẽ tạm thời mở khóa 'waiter' để người khác vào xin, 
        // và tự động khóa lại khi triết gia i được đánh thức.
    }

    // --- KẾT THÚC ĐO THỜI GIAN CHỜ ---
    gettimeofday(&end_time, NULL);
    long wait_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
    printf("[THỐNG KÊ] Triết gia %d đã phải chờ %ld micro-seconds.\n", i + 1, wait_time);

    pthread_mutex_unlock(&waiter); // Rời phòng Trọng tài, bắt đầu ăn
}

// Triết gia ăn xong, báo với Trọng tài để trả đũa
void putdown_forks(int i) {
    pthread_mutex_lock(&waiter); // Khóa Trọng tài lại để trả tài nguyên

    state[i] = THINKING;
    printf("Triết gia %d: Ăn xong, trả đũa và quay lại SUY NGHĨ.\n", i + 1);

    // Trọng tài kiểm tra xem 2 người hàng xóm bên cạnh có ai đang đói và đang chờ đũa không
    test(LEFT);
    test(RIGHT);

    pthread_mutex_unlock(&waiter); // Mở khóa Trọng tài
}

// Hàm thực thi của từng Luồng (Thread) Triết gia
void* philosopher(void* num) {
    int i = *(int*)num;

    while (1) {
        // 1. Trạng thái Suy nghĩ (Giả lập bằng việc dừng ngẫu nhiên từ 1-3 giây)
        usleep((rand() % 3 + 1) * 100000); 

        // 2. Đói và Xin đũa từ Trọng tài
        pickup_forks(i);

        // 3. Trạng thái Ăn (Giả lập bằng việc dừng ngẫu nhiên từ 1-3 giây)
        usleep((rand() % 3 + 1) * 100000); 

        // 4. Trả đũa sau khi ăn xong
        putdown_forks(i);
    }
}

int main() {
    int i;
    pthread_t thread_id[N];
    srand(time(NULL)); // Khởi tạo hạt giống ngẫu nhiên thời gian

    // Khởi tạo Mutex Trọng tài và các Biến điều kiện
    pthread_mutex_init(&waiter, NULL);
    for (i = 0; i < N; i++) {
        pthread_cond_init(&cv[i], NULL);
        state[i] = THINKING;
    }

    // Tạo luồng cho 5 triết gia chạy song song
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("Triết gia %d đã ngồi vào bàn.\n", i + 1);
    }

    // Chờ các luồng kết thúc (Vòng lặp vô tận nên chương trình sẽ chạy liên tục)
    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Giải phóng tài nguyên hệ thống khi tắt chương trình
    pthread_mutex_destroy(&waiter);
    for (i = 0; i < N; i++) {
        pthread_cond_destroy(&cv[i]);
    }

    return 0;
}

