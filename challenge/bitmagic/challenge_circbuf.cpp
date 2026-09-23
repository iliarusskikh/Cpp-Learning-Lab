//a fixed-capacity circular buffer (a ring buffer) for integers using std::vector<int>.

#include <iostream>
#include <vector>

class CircBuf {
    std::vector<int> q;
    int front = -1;
    int rear = -1;

public:
    explicit CircBuf(int n) : q(n) {}

    bool full() const {
        const int n = static_cast<int>(q.size());
        return (front == 0 && rear == n - 1) || (rear + 1 == front);
    }

    bool empty() const { return front == -1; }

    bool insert(int x) {
        if (full()) {
            return false;
        }
        const int n = static_cast<int>(q.size());
        if (rear == -1) {
            front = rear = 0;
        } else if (rear == n - 1) {
            rear = 0;
        } else {
            ++rear;
        }
        q[rear] = x;
        return true;
    }

    void print() const {
        if (empty()) {
            std::cout << "empty\n";
            return;
        }
        const int n = static_cast<int>(q.size());
        int i = front;
        while (true) {
            std::cout << q[i] << ' ';
            if (i == rear) {
                break;
            }
            i = (i + 1) % n;
        }
        std::cout << '\n';
    }
};

int main() {
    CircBuf buf(5);
    for (int x : {1, 2, 3, 4, 5, 6}) {
        if (!buf.insert(x)) {
            std::cout << "full, skipped " << x << '\n';
        }
    }
    buf.print();
    return 0;
}

/*
 #include <iostream>
 #include <vector>

 class CircBuf {
     std::vector<int> q;
     int front = 0;
     int count = 0;

 public:
     explicit CircBuf(int n) : q(n) {}

     bool full() const { return count == q.size(); }
     bool empty() const { return count == 0; }

     bool insert(int x) {
         if (full()) return false;
         
         int rear = (front + count) % q.size();
         q[rear] = x;
         ++count;
         return true;
     }

     bool remove(int& out) {
         if (empty()) return false;
         
         out = q[front];
         front = (front + 1) % q.size();
         --count;
         return true;
     }

     void print() const {
         if (empty()) {
             std::cout << "empty\n";
             return;
         }
         for (int i = 0; i < count; ++i) {
             std::cout << q[(front + i) % q.size()] << ' ';
         }
         std::cout << '\n';
     }
 };
 */
